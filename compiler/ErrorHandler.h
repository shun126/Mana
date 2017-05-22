/*!
mana (compiler)

@file	ErrorHandler.h
@brief	エラーや警告に関係するヘッダーファイル
@detail	このファイルはエラーや警告に関係するヘッダーファイルです。
@author	Shun Moriya
@date	2017-
*/

#pragma once
#include <cstdarg>
#include <cstdint>

namespace mana
{
	enum class CompileErrorType : uint8_t
	{
		OK = 0,
	};
	enum class LinkerErrorType : uint8_t
	{
		OK = 0,
	};
	enum class CompileWarningType : uint8_t
	{
		OK = 0,
	};
	enum class LinkerWarningType : uint8_t
	{
		OK = 0,
	};
	enum class FatalType : uint8_t
	{
		NoMemory,
	};

	extern void CompileError(const char* format, ...);
	extern void CompileWarning(const char* format, ...);

	extern void LinkerError(const char* format, ...);
	extern void LinkerWarning(const char* format, ...);

	extern void Fatal(const FatalType type);
	extern void Fatal(const char* format, ...);
	extern void FatalNoMemory();
}

extern int yynerrs;
extern void yyerror(const char* message);
