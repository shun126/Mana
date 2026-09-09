/*!
mana (compiler/library)

@file	Fault.h
@brief	不変条件が壊れた事の報告に関係するヘッダーファイル
@detail	このファイルは、内部の不変条件が壊れた事を報告する仕組みのヘッダーファイルです。
		manaはプロセスを終了させません。報告した後は例外を送出し、復帰の意味が定義
		できる場所（コンパイルの単位、アクターの単位）で受け止めます。
@author	Shun Moriya
@date	2026-
*/

#pragma once
#include "Platform.h"
#include "String.h"
#include <exception>
#include <stdexcept>
#include <string>

namespace mana
{
	/*!
	内部の不変条件が壊れた事を表す例外

	スクリプトの誤りではなく、manaの実装またはmanaを組み込んだ側の誤りを表します。
	*/
	class FatalError final : public std::runtime_error
	{
	public:
		FatalError(const char* file, const int line, const std::string& message)
			: std::runtime_error(Concat({ file, "(", std::to_string(line), "): ", message }))
			, mFile(file)
			, mLine(line)
			, mMessage(message)
		{
		}

		//! 発生したファイル名
		[[nodiscard]] const char* GetFile() const noexcept { return mFile; }

		//! 発生した行番号
		[[nodiscard]] int GetLine() const noexcept { return mLine; }

		//! 本文（ファイル名と行番号を含みません）
		[[nodiscard]] const std::string& GetMessage() const noexcept { return mMessage; }

	private:
		const char* mFile;
		int mLine;
		std::string mMessage;
	};

	/*!
	不変条件が壊れた時に呼ばれるコールバック

	巻き戻しが始まる前に呼ばれるため、その場のスタックが残っています。
	manaの開発中にデバッガーへ入りたい場合は、この中で停止して下さい。

	@param[in]	userData	SetFaultHandlerに渡した値
	@param[in]	file		発生したファイル名
	@param[in]	line		発生した行番号
	@param[in]	message		本文。NUL終端のUTF-8
	*/
	using FaultHandler = void(*)(void* userData, const char* file, const int line, const char* message);

	struct FaultSink final
	{
		FaultHandler mHandler = nullptr;
		void* mUserData = nullptr;
	};

	//! 現在の通知先を保持します
	[[nodiscard]] inline FaultSink& GetFaultSink() noexcept
	{
		static FaultSink sink;
		return sink;
	}

	/*!
	不変条件が壊れた時の通知先を設定します

	既定では何も行いません。報告はTraceへ、復帰は例外で行われます。

	@attention	利用を開始する前に一度だけ設定して下さい。
	@attention	handlerから戻ると例外が送出されます。handler自身は例外を
				送出しないで下さい。

	@param[in]	handler		通知先。nullptrで既定に戻ります
	@param[in]	userData	handlerへそのまま渡される値
	*/
	inline void SetFaultHandler(const FaultHandler handler, void* userData = nullptr) noexcept
	{
		FaultSink& sink = GetFaultSink();
		sink.mHandler = handler;
		sink.mUserData = userData;
	}

	/*!
	不変条件が壊れた事を報告し、例外を送出します

	プロセスは終了しません。呼び出し元の境界がアクターまたはコンパイルを
	停止させます。

	@param[in]	file	発生したファイル名
	@param[in]	line	発生した行番号
	@param[in]	message	本文
	*/
	[[noreturn]] inline void RaiseFault(const char* file, const int line, const std::string& message)
	{
		Trace(TraceLevel::Error, { file, "(", std::to_string(line), "): ", message, "\n" });

		const FaultSink& sink = GetFaultSink();
		if (sink.mHandler != nullptr)
		{
			sink.mHandler(sink.mUserData, file, line, message.c_str());
		}

		throw FatalError(file, line, message);
	}

	[[noreturn]] inline void RaiseFault(const char* file, const int line, const std::initializer_list<std::string_view> message)
	{
		RaiseFault(file, line, Concat(message));
	}
}
