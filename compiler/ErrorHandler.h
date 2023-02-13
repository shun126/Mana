/*!
mana (compiler)

@file	ErrorHandler.h
@brief	エラーや警告に関係するヘッダーファイル
@detail	このファイルはエラーや警告に関係するヘッダーファイルです。
@author	Shun Moriya
@date	2017-
*/

#pragma once
#include <string>

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

	extern void CompileError(const std::string& message);
	extern void CompileError(std::initializer_list<std::string_view> message);

	extern void CompileWarning(const std::string& message);
	extern void CompileWarning(std::initializer_list<std::string_view> message);

	extern void LinkerError(const std::string& message);
	extern void LinkerError(std::initializer_list<std::string_view> message);

	extern void LinkerWarning(const std::string& message);
	extern void LinkerWarning(std::initializer_list<std::string_view> message);

	extern void Fatal(const FatalType type);
	extern void Fatal(const std::string& message);
	extern void Fatal(std::initializer_list<std::string_view> message);

	extern void FatalNoMemory();
}

extern int yynerrs;
//extern void yyerror(const char* message);
