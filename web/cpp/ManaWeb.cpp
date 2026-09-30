/*!
mana (web)

@file	ManaWeb.cpp
@brief	Web Playground から Mana を利用する為の WebAssembly ブリッジ
@detail	デスクトップ版と同じコンパイラと VM を、JavaScript から呼べる小さな C の
		関数群として公開します。Web 専用の処理はこのファイルに閉じ込め、
		compiler/ と runner/ へは持ち込みません。

		基本的な流れ:
			mana_web_set_file("main.mn", source)
			mana_web_compile("main.mn")   -> 診断の JSON
			mana_web_start()              -> プログラムイメージを VM へ読み込み
			mana_web_step(ms) を 0 が返るまで繰り返す

		print() と VM の Trace は Module.onOutput(level, text) へ届きます。
@author	Shun Moriya
@date	2026-
*/

#include "../../runner/common/Setup.h"
#include "../../compiler/Compiler.h"
#include "../../runner/Mana.h"
#include "MemorySourceResolver.h"

#include <emscripten/emscripten.h>
#include <chrono>
#include <cstdio>
#include <exception>
#include <memory>
#include <string>
#include <vector>

// Trace の出力を JavaScript へ渡します。受け取り手が居なければ捨てます。
EM_JS_DEPS(mana_web, "$UTF8ToString");
EM_JS(void, mana_web_output, (int level, const char* text, size_t length), {
	if (typeof Module.onOutput === "function")
		Module.onOutput(level, UTF8ToString(text, length));
});

namespace
{
	//! ブリッジが保持する状態。Worker 1つにつき1組です
	struct State final
	{
		std::shared_ptr<mana::web::MemorySourceResolver> mSources = std::make_shared<mana::web::MemorySourceResolver>();
		std::shared_ptr<std::vector<uint8_t>> mProgram;
		std::shared_ptr<mana::VM> mVM;
		std::string mCompileResult;
	};

	State& GetState()
	{
		static State state;
		return state;
	}

	void OnTrace(void*, const mana::TraceLevel level, const char* message, const std::size_t length)
	{
		mana_web_output(static_cast<int>(level), message, length);
	}

	void AppendJsonString(std::string& out, const std::string_view text)
	{
		out.push_back('"');
		for (const char c : text)
		{
			switch (c)
			{
			case '"': out += "\\\""; break;
			case '\\': out += "\\\\"; break;
			case '\n': out += "\\n"; break;
			case '\r': out += "\\r"; break;
			case '\t': out += "\\t"; break;
			default:
				if (static_cast<unsigned char>(c) < 0x20)
				{
					char escaped[8];
					std::snprintf(escaped, sizeof(escaped), "\\u%04x", static_cast<unsigned int>(static_cast<unsigned char>(c)));
					out += escaped;
				}
				else
				{
					out.push_back(c);
				}
				break;
			}
		}
		out.push_back('"');
	}

	[[nodiscard]] const char* ToString(const mana::DiagnosticSeverity severity)
	{
		switch (severity)
		{
		case mana::DiagnosticSeverity::Warning: return "warning";
		case mana::DiagnosticSeverity::Error: return "error";
		case mana::DiagnosticSeverity::Fatal: return "fatal";
		}
		return "error";
	}

	[[nodiscard]] const char* ToString(const mana::DiagnosticPhase phase)
	{
		return phase == mana::DiagnosticPhase::Link ? "link" : "compile";
	}

	[[nodiscard]] std::string ToJson(const mana::CompileResult& result)
	{
		std::string json = "{\"success\":";
		json += result.mSucceeded ? "true" : "false";
		json += ",\"programSize\":";
		json += std::to_string(result.mProgramImage.size());
		json += ",\"diagnostics\":[";
		bool first = true;
		for (const mana::Diagnostic& diagnostic : result.mDiagnostics)
		{
			if (!first)
				json.push_back(',');
			first = false;
			json += "{\"severity\":";
			AppendJsonString(json, ToString(diagnostic.mSeverity));
			json += ",\"phase\":";
			AppendJsonString(json, ToString(diagnostic.mPhase));
			json += ",\"filename\":";
			AppendJsonString(json, diagnostic.mFilename);
			json += ",\"line\":";
			json += std::to_string(diagnostic.mLineNo);
			json += ",\"message\":";
			AppendJsonString(json, diagnostic.mMessage);
			json.push_back('}');
		}
		json += "]}";
		return json;
	}

	void ReportError(const std::string& message)
	{
		mana::Trace(mana::TraceLevel::Error, { "mana: ", message, "\n" });
	}
}

extern "C"
{
	//! 登録したソース、プログラムイメージ、VM を全て破棄します
	EMSCRIPTEN_KEEPALIVE void mana_web_reset()
	{
		State& state = GetState();
		state.mVM.reset();
		state.mProgram.reset();
		state.mSources->Clear();
		state.mCompileResult.clear();
	}

	//! コンパイルするソースを登録します。filename と text は UTF-8 です
	EMSCRIPTEN_KEEPALIVE void mana_web_set_file(const char* filename, const char* text)
	{
		GetState().mSources->SetFile(filename, text);
	}

	/*!
	登録したソースをコンパイルします

	成功したプログラムイメージは mana_web_start() の為に保持されます。

	@param[in]	entry	最初に読み込むファイル名
	@return		結果の JSON。次に mana_web_compile() を呼ぶまで有効です
	*/
	EMSCRIPTEN_KEEPALIVE const char* mana_web_compile(const char* entry)
	{
		State& state = GetState();
		state.mVM.reset();
		state.mProgram.reset();

		mana::CompileOptions options;
		options.mSourceFilename = entry;
		options.mSourceResolver = state.mSources;
		mana::CompileResult result = mana::Compile(options);

		state.mCompileResult = ToJson(result);
		if (result.mSucceeded)
			state.mProgram = std::make_shared<std::vector<uint8_t>>(std::move(result.mProgramImage));
		return state.mCompileResult.c_str();
	}

	/*!
	コンパイルしたプログラムイメージを新しい VM へ読み込みます

	@retval	1	成功。mana_web_step() で実行できます
	@retval	0	コンパイル済みのプログラムが無いか、読み込みに失敗しました
	*/
	EMSCRIPTEN_KEEPALIVE int mana_web_start()
	{
		State& state = GetState();
		state.mVM.reset();
		if (state.mProgram == nullptr)
		{
			ReportError("no compiled program");
			return 0;
		}

		try
		{
			auto vm = std::make_shared<mana::VM>();
			mana::FunctionInitialize(*vm);
			vm->LoadProgram(std::shared_ptr<const void>(state.mProgram, state.mProgram->data()));
			state.mVM = std::move(vm);
			return 1;
		}
		catch (const std::exception& e)
		{
			ReportError(e.what());
			return 0;
		}
	}

	/*!
	VM を実行します

	全てのアクターが停止するか、budgetMilliseconds を使い切るまで
	VM::Run() を繰り返します。呼び出し側はこの合間に出力を表示できます。

	@param[in]	budgetMilliseconds	今回の呼び出しで実行を続ける時間
	@retval	1	まだ実行中です。もう一度呼んで下さい
	@retval	0	実行が終わりました
	*/
	EMSCRIPTEN_KEEPALIVE int mana_web_step(const double budgetMilliseconds)
	{
		State& state = GetState();
		if (state.mVM == nullptr)
			return 0;

		const auto deadline = std::chrono::steady_clock::now()
			+ std::chrono::duration_cast<std::chrono::steady_clock::duration>(
				std::chrono::duration<double, std::milli>(budgetMilliseconds));
		try
		{
			do
			{
				if (!state.mVM->Run())
				{
					state.mVM.reset();
					return 0;
				}
			} while (std::chrono::steady_clock::now() < deadline);
			return 1;
		}
		catch (const std::exception& e)
		{
			ReportError(e.what());
			state.mVM.reset();
			return 0;
		}
	}

	//! 実行中の VM を破棄します
	EMSCRIPTEN_KEEPALIVE void mana_web_stop()
	{
		GetState().mVM.reset();
	}

	//! Mana のバージョン文字列を返します
	EMSCRIPTEN_KEEPALIVE const char* mana_web_version()
	{
		return mana::version::Number;
	}
}

int main()
{
	mana::SetTraceHandler(&OnTrace, nullptr);
	return 0;
}
