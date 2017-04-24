/*!
mana (compiler)

@file	error.c
@brief	エラーや警告に関係するソースファイル
@detail	このファイルはエラーや警告に関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#if !defined(___MANA_LEXER_H___)
#include "lexer.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#include <stdarg.h>
#include <stdio.h>

#define MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE	(1024)

void mana_compile_error(const char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	yyerror(string);
}

void mana_compile_warning(const char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

#if defined(_MSC_VER)
	MANA_PRINT("%s(%d): warning: %s\n", lexer_get_current_filename(), lexer_get_current_line(), string);
#else
	MANA_PRINT("%s%d: warning: %s\n", lexer_get_current_filename(), lexer_get_current_line(), string);
#endif
}

void mana_linker_error(const char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	MANA_PRINT("%s: error: %s\n", mana_get_output_filename(), string);
}

void mana_linker_warning(const char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	MANA_PRINT("%s: warning: %s\n", mana_get_output_filename(), string);
}

void mana_fatal(const char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	MANA_PRINT("%s: fatal: %s\n", mana_get_output_filename(), string);
	++yynerrs;
}

void mana_fatal_no_memory(void)
{
	mana_fatal("no memory error");
}
