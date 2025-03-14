/*!
mana (compiler)

@file	ErrorHandler.cpp
@brief	エラーや警告に関係するソースファイル
@detail	このファイルはエラーや警告に関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Main.h"

namespace mana
{
	struct Message
	{
		const char* mJaJp;
		const char* mEnUs;
	};

	void CompileError(const std::string& message)
	{
#if defined(MANA_TARGET_WINDOWS)
		Trace({ lexer::GetCurrentFilename(), "(", std::to_string(lexer::GetCurrentLineNo()), "): error: ", message, "\n" });
#else
		Trace({ lexer::GetCurrentFilename(), ":", std::to_string(lexer::GetCurrentLineNo()), " error: ", message, "\n" });
#endif
		++yynerrs;
	}

	void CompileError(std::initializer_list<std::string_view> message)
	{
		CompileError(Concat(message));
	}

	void CompileWarning(const std::string& message)
	{
#if defined(MANA_TARGET_WINDOWS)
		Trace({ lexer::GetCurrentFilename(), "(", std::to_string(lexer::GetCurrentLineNo()), "): warning: ", message, "\n" });
#else
		Trace({ lexer::GetCurrentFilename(), ":", std::to_string(lexer::GetCurrentLineNo()), " warning: ", message, "\n" });
#endif
	}

	void CompileWarning(std::initializer_list<std::string_view> message)
	{
		CompileWarning(Concat(message));
	}

	void LinkerError(const std::string& message)
	{
		Trace({ GetTargetFilename(), ": error: ", message, "\n" });
	}

	void LinkerError(std::initializer_list<std::string_view> message)
	{
		LinkerError(Concat(message));
	}

	void LinkerWarning(const std::string& message)
	{
		Trace({ GetTargetFilename(), ": warning: ", message, "\n" });
	}

	void LinkerWarning(std::initializer_list<std::string_view> message)
	{
		LinkerWarning(Concat(message));
	}

	void Fatal(const FatalType type)
	{
		Message message[] = {
			{ "メモリが足りません", "Not enough memory" }
		};
		Trace({ GetTargetFilename(), ": fatal: ", message[static_cast<uint8_t>(type)].mJaJp, "\n" });
		++yynerrs;
	}

	void Fatal(const std::string& message)
	{
		Trace({ GetTargetFilename(), ": fatal: ", message, "\n" });
		++yynerrs;
	}

	void Fatal(std::initializer_list<std::string_view> message)
	{
		Fatal(Concat(message));
	}

	void FatalNoMemory()
	{
		Fatal(FatalType::NoMemory);
	}
}
