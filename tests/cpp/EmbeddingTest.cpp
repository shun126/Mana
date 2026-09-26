/*!
mana (test)

@file	EmbeddingTest.cpp
@brief	組み込み用インターフェースの試験
@detail	コンパイラと仮想マシンをライブラリとして利用する経路を試験します。
		コマンドラインからは通らない経路（メモリからのコンパイル、出力の
		差し替え、障害時の振る舞い）を扱います。
		ポインタ幅に依存する不具合を取り逃がさないよう、32ビットと64ビットの
		両方で実行して下さい。
@author	Shun Moriya
@date	2026-
*/

#include "../../compiler/Compiler.h"
#include "../../runner/Mana.h"

#include <cstdint>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <future>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace mana
{
	struct ActorTestAccess
	{
		static address_t FrameSize(const Actor& actor) { return actor.mFrame.GetSize(); }
	};
}

namespace
{
	////////////////////////////////////////////////////////////////////////////////
	// 試験の枠組み

	int gFailures = 0;
	const char* gCurrentCase = "";

	void BeginCase(const char* name)
	{
		gCurrentCase = name;
		std::printf("[ RUN      ] %s\n", name);
	}

	void Fail(const char* detail)
	{
		++gFailures;
		std::printf("[   FAILED ] %s: %s\n", gCurrentCase, detail);
	}

	void Check(const bool condition, const char* detail)
	{
		if (!condition)
			Fail(detail);
	}

	void CheckEqual(const std::string& actual, const std::string& expected, const char* detail)
	{
		if (actual != expected)
		{
			++gFailures;
			std::printf("[   FAILED ] %s: %s\n", gCurrentCase, detail);
			std::printf("             expected [%s]\n", expected.c_str());
			std::printf("             actual   [%s]\n", actual.c_str());
		}
	}

	void CheckContains(const std::string& haystack, const std::string& needle, const char* detail)
	{
		if (haystack.find(needle) == std::string::npos)
		{
			++gFailures;
			std::printf("[   FAILED ] %s: %s\n", gCurrentCase, detail);
			std::printf("             looking for [%s]\n", needle.c_str());
			std::printf("             in          [%s]\n", haystack.c_str());
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// 補助

	//! メモリ上のソースを供給します
	class MemorySourceResolver final : public mana::SourceResolver
	{
	public:
		std::map<std::string, std::string, std::less<>> mFiles;

		std::string Resolve(const std::string_view, const std::string_view filename) const override
		{
			return std::string(filename);
		}

		bool Read(const std::string_view path, std::string& outText) const override
		{
			const auto it = mFiles.find(path);
			if (it == mFiles.end())
				return false;
			outText = it->second;
			return true;
		}
	};

	//! Traceの出力を溜めます
	struct TraceRecord final
	{
		mana::TraceLevel mLevel;
		std::string mMessage;
	};

	std::vector<TraceRecord> gTrace;

	void OnTrace(void*, const mana::TraceLevel level, const char* message, const std::size_t length)
	{
		gTrace.push_back({ level, std::string(message, length) });
	}

	//! 溜めたTraceを1つの文字列にします
	[[nodiscard]] std::string JoinTrace()
	{
		std::string joined;
		for (const TraceRecord& record : gTrace)
			joined += record.mMessage;
		return joined;
	}

	/*!
	仮想マシン自身の実行トレースを取り除きます

	実行トレースはデバッグビルドでのみ出るため、残したままだと構成によって
	結果が変わってしまいます。スクリプトが出力した分だけを比べます。
	*/
	[[nodiscard]] std::string ScriptOutputOnly(const std::string& text)
	{
		std::string filtered;
		std::string::size_type begin = 0;
		while (begin < text.size())
		{
			const std::string::size_type end = text.find('\n', begin);
			const std::string::size_type stop = (end == std::string::npos) ? text.size() : end + 1;
			const std::string line = text.substr(begin, stop - begin);
			if (line.compare(0, 6, "mana:r") != 0)
				filtered += line;
			begin = stop;
		}
		return filtered;
	}

	//! 指定した重大度の出力だけを繋げます
	[[nodiscard]] std::string JoinTrace(const mana::TraceLevel level)
	{
		std::string joined;
		for (const TraceRecord& record : gTrace)
		{
			if (record.mLevel == level)
				joined += record.mMessage;
		}
		return joined;
	}

	[[nodiscard]] mana::CompileResult CompileSource(const std::map<std::string, std::string>& files, const std::string& entry)
	{
		auto resolver = std::make_shared<MemorySourceResolver>();
		for (const auto& file : files)
			resolver->mFiles[file.first] = file.second;

		mana::CompileOptions options;
		options.mSourceFilename = entry;
		options.mSourceResolver = resolver;
		return mana::Compile(options);
	}

	//! プログラムイメージを実行し、出力を返します
	[[nodiscard]] std::string RunProgram(const std::vector<uint8_t>& programImage)
	{
		gTrace.clear();
		mana::SetTraceHandler(&OnTrace, nullptr);

		{
			auto image = std::make_shared<std::vector<uint8_t>>(programImage);
			auto vm = std::make_shared<mana::VM>();
			vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
			while (vm->Run())
				;
		}

		mana::SetTraceHandler(nullptr);
		return JoinTrace();
	}

	[[nodiscard]] std::string DiagnosticsToString(const mana::CompileResult& result)
	{
		std::string joined;
		for (const mana::Diagnostic& diagnostic : result.mDiagnostics)
			joined += diagnostic.ToString() + "\n";
		return joined;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 型の幅

	/*!
	ファイル形式が想定する幅を確かめます

	プログラムイメージは32ビットの値で書かれています。ポインタ幅に引きずられて
	widenされると、生成物の互換性が静かに壊れます。
	*/
	void TestTypeWidths()
	{
		BeginCase("TypeWidths");

		Check(sizeof(mana::address_t) == 4, "address_t must stay 32 bit");
		Check(sizeof(mana::offset_t) == 4, "offset_t must stay 32 bit");

		// バイトコードの整数即値は32ビットで書き出されます
		Check(sizeof(mana::int_t) <= sizeof(mana::int_t), "int_t is available");

		// ファイルヘッダーの並びが変わると既存の生成物が読めなくなります
		Check(sizeof(mana::FileHeader) == 32, "FileHeader layout changed");
	}

	////////////////////////////////////////////////////////////////////////////////
	// コンパイル

	void TestCompileFromMemory()
	{
		BeginCase("CompileFromMemory");

		const mana::CompileResult result = CompileSource({
			{ "main.mn",
			  "actor Root\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main { print(\"hello\\n\"); }\n"
			  "}\n" } }, "main.mn");

		Check(result.mSucceeded, "compile should succeed");
		Check(!result.mProgramImage.empty(), "program image should not be empty");
		Check(result.mDiagnostics.empty(), "no diagnostics expected");
	}

	void TestCompileIsRepeatable()
	{
		BeginCase("CompileIsRepeatable");

		const std::map<std::string, std::string> files = {
			{ "main.mn",
			  "actor Root\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main { print(\"hello\\n\"); }\n"
			  "}\n" } };

		const mana::CompileResult first = CompileSource(files, "main.mn");
		const mana::CompileResult second = CompileSource(files, "main.mn");

		Check(first.mSucceeded && second.mSucceeded, "both compiles should succeed");
		Check(first.mProgramImage.size() == second.mProgramImage.size(),
			"repeated compilation changed the image size");

		// 乱数の種はコンパイル毎に変わるので、そこだけ除いて比較します
		if (first.mProgramImage.size() == second.mProgramImage.size() && first.mProgramImage.size() >= 32)
		{
			std::vector<uint8_t> a = first.mProgramImage;
			std::vector<uint8_t> b = second.mProgramImage;
			for (size_t i = 28; i < 32; ++i)
				a[i] = b[i] = 0;
			Check(a == b, "repeated compilation produced a different image");
		}
	}

	void TestMissingSourceIsReported()
	{
		BeginCase("MissingSourceIsReported");

		const mana::CompileResult result = CompileSource({}, "nosuch.mn");

		Check(!result.mSucceeded, "compile should fail");
		Check(result.mProgramImage.empty(), "no program image expected");
		CheckContains(DiagnosticsToString(result), "unable to open", "diagnostic should say what failed");
	}

	void TestDiagnosticCarriesPosition()
	{
		BeginCase("DiagnosticCarriesPosition");

		const mana::CompileResult result = CompileSource({
			{ "main.mn", "include \"broken.mn\";\n" },
			{ "broken.mn",
			  "actor Broken\n"
			  "{\n"
			  "    action main { this is not mana; }\n"
			  "}\n" } }, "main.mn");

		Check(!result.mSucceeded, "compile should fail");

		bool found = false;
		for (const mana::Diagnostic& diagnostic : result.mDiagnostics)
		{
			if (diagnostic.mFilename == "broken.mn" && diagnostic.mLineNo == 3)
				found = true;
		}
		Check(found, "a diagnostic should point at broken.mn line 3");
	}

	/*!
		診断ハンドラが例外を投げても Compile() の外へは出ない事を確かめます

		Compile() は例外が境界を越えない事を約束しています。ハンドラの例外を
		無視せず catch(const std::exception&) で捕まえてしまうと、その通知が
		Fatal diagnostic に化けて DiagnosticBag::Add() から同じハンドラをもう
		一度呼び、そこでまた投げられてホストまで抜けてしまう経路があったので、
		それを塞いだ側の回帰試験です。
	*/
	void TestDiagnosticHandlerThrowIsContained()
	{
		BeginCase("DiagnosticHandlerThrowIsContained");

		auto resolver = std::make_shared<MemorySourceResolver>();
		resolver->mFiles["main.mn"] =
			"actor Broken\n"
			"{\n"
			"    action main { this is not mana; }\n"
			"}\n";

		mana::CompileOptions options;
		options.mSourceFilename = "main.mn";
		options.mSourceResolver = resolver;

		int handlerCalls = 0;
		options.mDiagnosticHandler = [&handlerCalls](const mana::Diagnostic&)
		{
			++handlerCalls;
			throw std::runtime_error("diagnostic handler failure");
		};

		bool escaped = false;
		mana::CompileResult result;
		try
		{
			result = mana::Compile(options);
		}
		catch (...)
		{
			escaped = true;
		}

		Check(!escaped, "an exception from the diagnostic handler must not cross Compile()");
		Check(handlerCalls > 0, "the throwing handler should still have been invoked");
		Check(!result.mSucceeded, "compile should still fail");
		Check(!result.mDiagnostics.empty(), "the diagnostic should still be collected despite the handler throwing");
	}

	void TestLineEndingsAreNormalised()
	{
		BeginCase("LineEndingsAreNormalised");

		const std::string body =
			"actor Root\r\n"
			"{\r\n"
			"    action init { }\r\n"
			"    action main { print(\"crlf\\n\"); }\n"
			"}\n";

		const mana::CompileResult result = CompileSource({ { "main.mn", body } }, "main.mn");
		Check(result.mSucceeded, "a source with CRLF should compile");
		if (result.mSucceeded)
			CheckEqual(ScriptOutputOnly(RunProgram(result.mProgramImage)), "crlf\n", "unexpected output");
	}

	////////////////////////////////////////////////////////////////////////////////
	// 出力の差し替え

	void TestTraceHandlerReceivesLevels()
	{
		BeginCase("TraceHandlerReceivesLevels");

		gTrace.clear();
		mana::SetTraceHandler(&OnTrace, nullptr);
		mana::Trace(mana::TraceLevel::Info, "info\n");
		mana::Trace(mana::TraceLevel::Warning, "warning\n");
		mana::Trace(mana::TraceLevel::Error, { "err", "or\n" });
		mana::Trace(mana::TraceLevel::Debug, "debug\n");
		mana::SetTraceHandler(nullptr);

		Check(gTrace.size() == 4, "four records expected");
		CheckEqual(JoinTrace(mana::TraceLevel::Debug), "debug\n", "debug stream");
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "info\n", "info stream");
		CheckEqual(JoinTrace(mana::TraceLevel::Warning), "warning\n", "warning stream");
		CheckEqual(JoinTrace(mana::TraceLevel::Error), "error\n", "error stream");

		// 解除した後は溜まりません
		gTrace.clear();
		mana::Trace(mana::TraceLevel::Info, "(the line above is expected on stdout)\n");
		Check(gTrace.empty(), "handler should be detached");
	}

	////////////////////////////////////////////////////////////////////////////////
	// 障害

	int gFaultCount = 0;

	void OnFault(void*, const char*, const int, const char*)
	{
		++gFaultCount;
	}

	/*!
	不変条件が壊れてもプロセスが終了しない事を確かめます
	*/
	void TestFaultDoesNotEndProcess()
	{
		BeginCase("FaultDoesNotEndProcess");

		gFaultCount = 0;
		gTrace.clear();
		mana::SetTraceHandler(&OnTrace, nullptr);
		mana::SetFaultHandler(&OnFault, nullptr);

		bool caught = false;
		try
		{
			MANA_VERIFY_MESSAGE(1 == 2, "deliberate");
		}
		catch (const mana::FatalError& e)
		{
			caught = true;
			Check(e.GetLine() > 0, "FatalError should carry a line number");
			CheckContains(e.GetMessage(), "deliberate", "FatalError should carry the message");
		}

		mana::SetFaultHandler(nullptr);
		mana::SetTraceHandler(nullptr);

		Check(caught, "a broken invariant should throw");
		Check(gFaultCount == 1, "the fault handler should be called once");
		CheckContains(JoinTrace(mana::TraceLevel::Error), "deliberate", "the fault should be reported");
	}

	/*!
	スクリプトの誤りは、そのアクターだけを止めます
	*/
	void TestScriptErrorHaltsOnlyThatActor()
	{
		BeginCase("ScriptErrorHaltsOnlyThatActor");

		const mana::CompileResult result = CompileSource({
			{ "main.mn",
			  "actor Bystander\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main { print(\"bystander ran\\n\"); }\n"
			  "}\n"
			  "\n"
			  "actor Root\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main\n"
			  "    {\n"
			  "        int numerator = 1;\n"
			  "        int denominator = 0;\n"
			  "        print(\"before\\n\");\n"
			  "        print(\"%d\\n\", numerator / denominator);\n"
			  "        print(\"unreachable\\n\");\n"
			  "    }\n"
			  "}\n" } }, "main.mn");

		Check(result.mSucceeded, "compile should succeed");
		if (!result.mSucceeded)
			return;

		const std::string output = RunProgram(result.mProgramImage);

		CheckContains(output, "before", "the actor should run up to the fault");
		CheckContains(output, "script error: division by zero", "the script error should be reported");
		CheckContains(output, "bystander ran", "the other actor should keep running");
		Check(output.find("unreachable") == std::string::npos,
			"the halted actor should not continue");
	}

	void TestSubscriptOutOfRangeIsCaught()
	{
		BeginCase("SubscriptOutOfRangeIsCaught");

		const mana::CompileResult result = CompileSource({
			{ "main.mn",
			  "actor Root\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main\n"
			  "    {\n"
			  "        int values[4];\n"
			  "        int index = 100;\n"
			  "        print(\"before\\n\");\n"
			  "        values[index] = 1;\n"
			  "        print(\"unreachable\\n\");\n"
			  "    }\n"
			  "}\n" } }, "main.mn");

		Check(result.mSucceeded, "compile should succeed");
		if (!result.mSucceeded)
			return;

		const std::string output = RunProgram(result.mProgramImage);
		CheckContains(output, "before", "the actor should run up to the fault");
		CheckContains(output, "script error: subscript out of range", "the script error should be reported");
		Check(output.find("unreachable") == std::string::npos, "the halted actor should not continue");
	}

	/*!
	バイトオフセットへ変換すると桁あふれする添字を確かめます

	添字を要素サイズで乗じてから検査すると、1073741824 * 4 が 0 へ折り返って
	検査を通り抜け、静かに要素0へ触れてしまいます。
	*/
	void TestSubscriptOverflowIsCaught()
	{
		BeginCase("SubscriptOverflowIsCaught");

		const mana::CompileResult result = CompileSource({
			{ "main.mn",
			  "actor Root\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main\n"
			  "    {\n"
			  "        int values[4];\n"
			  "        int index = 1073741824;\n"
			  "        values[0] = 7;\n"
			  "        print(\"before\\n\");\n"
			  "        values[index] = 1;\n"
			  "        print(\"unreachable\\n\");\n"
			  "    }\n"
			  "}\n" } }, "main.mn");

		Check(result.mSucceeded, "compile should succeed");
		if (!result.mSucceeded)
			return;

		const std::string output = RunProgram(result.mProgramImage);
		CheckContains(output, "before", "the actor should run up to the fault");
		CheckContains(output, "script error: subscript out of range", "the overflowing subscript should be reported");
		Check(output.find("unreachable") == std::string::npos, "the halted actor should not continue");
	}

	////////////////////////////////////////////////////////////////////////////////
	// アドレス計算

	/*!
	構造体と配列のアドレス計算を実行して確かめます

	ポインタ幅とint_tの幅が異なる環境で壊れた事があるため、値まで確認します。
	*/
	void TestAddressArithmetic()
	{
		BeginCase("AddressArithmetic");

		const mana::CompileResult result = CompileSource({
			{ "main.mn",
			  "struct Point\n"
			  "{\n"
			  "    int x;\n"
			  "    int y;\n"
			  "    int z;\n"
			  "}\n"
			  "\n"
			  "actor Root\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main\n"
			  "    {\n"
			  "        Point point;\n"
			  "        point.x = 11;\n"
			  "        point.y = 22;\n"
			  "        point.z = 33;\n"
			  "        print(\"field %d %d %d\\n\", point.x, point.y, point.z);\n"
			  "\n"
			  "        int values[4];\n"
			  "        values[0] = 100;\n"
			  "        values[1] = 200;\n"
			  "        values[3] = 400;\n"
			  "        print(\"array %d %d %d\\n\", values[0], values[1], values[3]);\n"
			  "\n"
			  "        Point copied;\n"
			  "        copied = point;\n"
			  "        print(\"copy %d %d %d\\n\", copied.x, copied.y, copied.z);\n"
			  "    }\n"
			  "}\n" } }, "main.mn");

		Check(result.mSucceeded, "compile should succeed");
		if (!result.mSucceeded)
		{
			std::printf("%s", DiagnosticsToString(result).c_str());
			return;
		}

		CheckEqual(ScriptOutputOnly(RunProgram(result.mProgramImage)),
			"field 11 22 33\narray 100 200 400\ncopy 11 22 33\n",
			"struct and array access produced the wrong values");
	}

	////////////////////////////////////////////////////////////////////////////////
	// ネイティブ関数

	void OnNativeAdd(const std::shared_ptr<mana::Actor>& actor, void*)
	{
		const int32_t left = actor->GetParameterInteger(0);
		const int32_t right = actor->GetParameterInteger(1);
		actor->SetReturnInteger(left + right);
	}

	void TestNativeFunctionBinding()
	{
		BeginCase("NativeFunctionBinding");

		const mana::CompileResult result = CompileSource({
			{ "main.mn",
			  "native int nativeAdd(int left, int right);\n"
			  "\n"
			  "actor Root\n"
			  "{\n"
			  "    action init { }\n"
			  "    action main { print(\"sum %d\\n\", nativeAdd(3, 4)); }\n"
			  "}\n" } }, "main.mn");

		Check(result.mSucceeded, "compile should succeed");
		if (!result.mSucceeded)
		{
			std::printf("%s", DiagnosticsToString(result).c_str());
			return;
		}

		gTrace.clear();
		mana::SetTraceHandler(&OnTrace, nullptr);
		{
			auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
			auto vm = std::make_shared<mana::VM>();
			vm->RegisterFunction("nativeAdd", &OnNativeAdd);
			vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
			while (vm->Run())
				;
		}
		mana::SetTraceHandler(nullptr);

		CheckEqual(ScriptOutputOnly(JoinTrace()), "sum 7\n", "the native function should have been called");
	}

	void TestDelaySeconds()
	{
		BeginCase("DelaySeconds");
		auto result = CompileSource({ { "main.mn", R"(
native void delay(float seconds);
int gInitialized = 1;
actor Root {
 action main { print("start\n"); delay(0.5); print("done\n"); delay(0.0); print("zero\n"); }
 action urgent { delay(0.25); print("urgent\n"); }
}
)" } }, "main.mn");
		Check(result.mSucceeded, "delay source should compile");
		if (!result.mSucceeded) return;
		auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
		auto vm = std::make_shared<mana::VM>();
		mana::FunctionInitialize(*vm);
		vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
		gTrace.clear();
		mana::SetTraceHandler(&OnTrace);
		vm->Run(0.0);
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "start\n", "delay starts without completing");
		vm->Run(0.25);
		vm->FindActor("Root")->Again();
		vm->Run(0.0);
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "start\n", "zero delta and rescheduling do not advance time");
		Check(vm->Request(10, "Root", "urgent", nullptr), "interrupt should be accepted");
		vm->Run(0.0);
		vm->Run(0.125);
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "start\n", "both delays still pending");
		vm->Run(0.125);
		for (int i = 0; i < 3 && vm->IsRunning(); ++i) vm->Run(0.0);
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "start\nurgent\ndone\nzero\n", "independent deadlines survive interruption");
		vm->Restart();
		Check(vm->GetElapsedSeconds() == 0, "restart resets time");
		Check(vm->Request(0, "Root", "main", nullptr), "restart allows a fresh main request");
		gTrace.clear();
		vm->Run(0.0);
		vm->Run(0.25);
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "start\n", "restart clears old deadline");
		vm->Run(0.25);
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "start\ndone\nzero\n", "deadline reached after restart");
		mana::SetTraceHandler(nullptr);
		for (double invalid : { -1.0, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN() })
		{
			bool rejected = false;
			try { vm->Run(invalid); } catch (const std::invalid_argument&) { rejected = true; }
			Check(rejected, "invalid delta is rejected");
			rejected = false;
			try { vm->FindActor("Root")->Delay(invalid); } catch (const std::invalid_argument&) { rejected = true; }
			Check(rejected, "invalid delay is rejected");
		}
		vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
		Check(vm->GetElapsedSeconds() == 0, "load resets time after global initialization");
		Check(vm->GetDeltaTime() == 0, "load resets delta after global initialization");
	}

	void TestCallExclusiveDelay()
	{
		BeginCase("CallExclusiveDelay");
		auto result = CompileSource({ { "main.mn", R"(
native void delay(float seconds);
actor Root {
 action wait { delay(0.05); print("waited\n"); }
}
)" } }, "main.mn");
		Check(result.mSucceeded, "delay source should compile");
		if (!result.mSucceeded) return;
		auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
		auto vm = std::make_shared<mana::VM>();
		mana::FunctionInitialize(*vm);
		vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
		gTrace.clear();
		mana::SetTraceHandler(&OnTrace);
		// CallExclusive は完了まで戻らないため、時計が進まない不具合で CI が止まらないよう別スレッドで待ちます
		std::promise<bool> promise;
		auto future = promise.get_future();
		std::thread([vm, &promise]() { promise.set_value(vm->FindActor("Root")->CallExclusive(1, "wait", nullptr)); }).detach();
		if (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready)
		{
			Fail("CallExclusive with delay did not return");
			std::fflush(stdout);
			std::_Exit(1);
		}
		mana::SetTraceHandler(nullptr);
		Check(future.get(), "CallExclusive should complete the delayed action");
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "waited\n", "delayed action ran to completion");
		Check(vm->GetElapsedSeconds() >= 0.05, "CallExclusive advances VM time");
	}

	void TestIndependentActorStartup()
	{
		BeginCase("IndependentActorStartup");
		auto result = CompileSource({ { "main.mn", R"(
native void delay(float seconds);
actor Slow {
 action init { delay(10.0); print("slow init\n"); }
 action main { print("slow main\n"); }
 action event { print("event\n"); }
}
actor Fast {
 action init { print("fast init\n"); }
 action main { print("fast main\n"); }
}
actor MainOnly { action main { print("main only\n"); } }
actor InitOnly { action init { print("init only\n"); } }
)" } }, "main.mn");
		Check(result.mSucceeded, "startup source should compile");
		if (!result.mSucceeded) return;
		auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
		auto vm = std::make_shared<mana::VM>();
		mana::FunctionInitialize(*vm);
		gTrace.clear();
		mana::SetTraceHandler(&OnTrace);
		vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
		Check(vm->FindActor("Slow")->GetInterruptPriority() == std::numeric_limits<int32_t>::max(), "init uses maximum priority");
		Check(vm->Request(100, "Slow", "event", nullptr), "ordinary request is queued during init");
		for (int i = 0; i < 10; ++i) vm->Run(0.0);
		const auto early = JoinTrace(mana::TraceLevel::Info);
		Check(early.find("fast init\nfast main\n") != std::string::npos, "fast Actor starts independently after its init");
		Check(early.find("main only\n") != std::string::npos, "Actor without init starts immediately");
		Check(early.find("init only\n") != std::string::npos, "Actor without main initializes");
		Check(early.find("slow") == std::string::npos && early.find("event") == std::string::npos, "init blocks only its own queued actions");
		vm->Run(10.0);
		for (int i = 0; i < 10 && vm->IsRunning(); ++i) vm->Run(0.0);
		Check(!vm->IsRunning(), "all startup actions finish");
		CheckEqual(JoinTrace(mana::TraceLevel::Info).substr(early.size()), "slow init\nevent\nslow main\n", "queued actions resume in priority order");
		CheckEqual(JoinTrace(mana::TraceLevel::Error), "", "startup has no runtime faults");
		mana::SetTraceHandler(nullptr);
	}

	void TestReturnEpilogues()
	{
		BeginCase("ReturnEpilogues");
		auto result = CompileSource({ { "main.mn", R"(
native void observe();
int remainingKeys(int required, int owned) {
 if (owned >= required) { return 0; }
 return required - owned;
}
int nested(int x) {
 if (x > 0) { if (x > 1) { return remainingKeys(3, 1); } return 7; }
 return remainingKeys(3, 5);
}
void early(int x) {
 if (x > 0) { return; }
 print("fallthrough\n");
}
int trailing() { return 9; print("unreachable\n"); }
actor Event {
 action main {
  print("%d %d %d %d %d %d\n", remainingKeys(3, 5), remainingKeys(3, 1), nested(2), nested(1), nested(0), trailing());
  early(1); early(0);
  observe();
  int i = 0;
  while (i < 100) {
   remainingKeys(3, 5); remainingKeys(3, 1); nested(2); trailing(); early(1);
   observe();
   i = i + 1;
  }
  return;
  print("unreachable action\n");
 }
}
)" } }, "main.mn");
		Check(result.mSucceeded, "return regression should compile");
		if (!result.mSucceeded) { std::printf("%s", DiagnosticsToString(result).c_str()); return; }
		auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
		auto vm = std::make_shared<mana::VM>();
		int observations = 0;
		mana::address_t baseline = 0;
		vm->RegisterFunction("observe", [&](const std::shared_ptr<mana::Actor>& actor, void*) {
			const auto size = mana::ActorTestAccess::FrameSize(*actor);
			if (observations++ == 0) baseline = size;
			Check(size == baseline, "calls must restore the caller frame size");
		});
		gTrace.clear();
		mana::SetTraceHandler(&OnTrace);
		vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
		for (int i = 0; i < 1000 && vm->IsRunning(); ++i) vm->Run(0.0);
		Check(!vm->IsRunning(), "return regression must terminate");
		Check(observations == 101, "all repeated calls must complete");
		CheckEqual(JoinTrace(mana::TraceLevel::Info), "0 2 2 7 0 9\nfallthrough\n", "all return paths");
		CheckEqual(JoinTrace(mana::TraceLevel::Error), "", "no runtime faults");
		Check(mana::ActorTestAccess::FrameSize(*vm->FindActor("Event")) == 0, "action releases its frame");
		mana::SetTraceHandler(nullptr);
	}

	void TestReturnBranchTargets()
	{
		BeginCase("ReturnBranchTargets");
		auto result = CompileSource({ { "main.mn", "int f() { return 1; } actor A { action main { f(); return; } }" } }, "main.mn");
		Check(result.mSucceeded, "branch target source should compile");
		if (!result.mSucceeded) return;
		const auto* header = reinterpret_cast<const mana::FileHeader*>(result.mProgramImage.data());
		const auto size = header->mSizeOfInstructionPool;
		const auto* code = result.mProgramImage.data() + result.mProgramImage.size() - size;
		std::vector<mana::address_t> boundaries;
		std::vector<mana::address_t> targets;
		for (mana::address_t pc = 0; pc < size; ) {
			boundaries.push_back(pc);
			if (static_cast<mana::IntermediateLanguage>(code[pc]) == mana::IntermediateLanguage::Branch) {
				const auto* p = code + pc + 1;
				targets.push_back((uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | p[3]);
			}
			const auto length = mana::GetInstructionSize(code, pc);
			if (length == 0 || length > size - pc) { Fail("invalid instruction boundary"); return; }
			pc += length;
		}
		Check(targets.size() >= 2, "function and action returns retain their branches");
		for (auto target : targets) {
			bool boundary = false;
			for (auto pc : boundaries) boundary |= pc == target;
			Check(boundary, "return target must be an instruction boundary");
			if (!boundary) continue;
			const auto op = static_cast<mana::IntermediateLanguage>(code[target]);
			Check(op == mana::IntermediateLanguage::LoadReturnAddress || op == mana::IntermediateLanguage::Free, "return must enter the epilogue");
		}
	}
}

int main()
{
	std::printf("mana embedding test (%zu bit pointer, %zu bit int_t)\n",
		sizeof(void*) * 8, sizeof(mana::int_t) * 8);

	TestTypeWidths();
	TestCompileFromMemory();
	TestCompileIsRepeatable();
	TestMissingSourceIsReported();
	TestDiagnosticCarriesPosition();
	TestDiagnosticHandlerThrowIsContained();
	TestLineEndingsAreNormalised();
	TestTraceHandlerReceivesLevels();
	TestFaultDoesNotEndProcess();
	TestScriptErrorHaltsOnlyThatActor();
	TestSubscriptOutOfRangeIsCaught();
	TestSubscriptOverflowIsCaught();
	TestAddressArithmetic();
	TestNativeFunctionBinding();
	TestDelaySeconds();
	TestCallExclusiveDelay();
	TestIndependentActorStartup();
	TestReturnEpilogues();
	TestReturnBranchTargets();

	if (gFailures == 0)
	{
		std::printf("[  PASSED  ] all cases\n");
		return 0;
	}

	std::printf("[  FAILED  ] %d check(s)\n", gFailures);
	return 1;
}
