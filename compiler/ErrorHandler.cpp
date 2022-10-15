/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Main.h"
#include <cstdio>

namespace mana
{
	struct Message
	{
		const char* mJaJp;
		const char* mEnUs;
	};

	void CompileError(const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
#if defined(MANA_TARGET_WINDOWS)
		MANA_PRINT("%s(%d): error: %s\n", lexer_get_current_filename().data(), lexer_get_current_lineno(), FormatText(format, argptr).c_str());
#else
		MANA_PRINT("%s:%d: error: %s\n", lexer_get_current_filename().data(), lexer_get_current_lineno(), FormatText(format, argptr).c_str());
#endif
		va_end(argptr);
		++yynerrs;
	}

	void CompileWarning(const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
#if defined(MANA_TARGET_WINDOWS)
		MANA_PRINT("%s(%d): warning: %s\n", lexer_get_current_filename().data(), lexer_get_current_lineno(), FormatText(format, argptr).c_str());
#else
		MANA_PRINT("%s%d: warning: %s\n", lexer_get_current_filename().data(), lexer_get_current_lineno(), FormatText(format, argptr).c_str());
#endif
		va_end(argptr);
	}

	void LinkerError(const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
		MANA_PRINT("%s: error: %s\n", GetTargetFilename(), FormatText(format, argptr).c_str());
		va_end(argptr);
	}

	void LinkerWarning(const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
		MANA_PRINT("%s: warning: %s\n", GetTargetFilename(), FormatText(format, argptr).c_str());
		va_end(argptr);
	}

	void Fatal(const FatalType type)
	{
		Message message[] = {
			{ "メモリが足りません", "Not enough memory" }
		};
		MANA_PRINT("%s: fatal: %s\n", GetTargetFilename(), message[static_cast<uint8_t>(type)].mJaJp);
		++yynerrs;
	}

	void Fatal(const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
		MANA_PRINT("%s: fatal: %s\n", GetTargetFilename(), FormatText(format, argptr).c_str());
		va_end(argptr);
		++yynerrs;
	}

	void FatalNoMemory()
	{
		Fatal(FatalType::NoMemory);
	}
}
