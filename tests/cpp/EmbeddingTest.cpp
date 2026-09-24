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
#include <cstdio>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

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
		mana::SetTraceHandler(nullptr);

		Check(gTrace.size() == 3, "three records expected");
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

	if (gFailures == 0)
	{
		std::printf("[  PASSED  ] all cases\n");
		return 0;
	}

	std::printf("[  FAILED  ] %d check(s)\n", gFailures);
	return 1;
}
