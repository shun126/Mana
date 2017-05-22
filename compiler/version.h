/*!
mana (compiler)

@file	Version.h
@brief	バージョン情報に関するヘッダーファイル
@detail	このファイルはバージョン情報に関するヘッダーファイルです。
		Version.rbにより自動生成されるので注意して下さい
@author	Shun Moriya
@date	2003-
*/

#pragma once
#include <cstdint>
namespace mana
{
	namespace build
	{
		static constexpr int32_t Year = 2020;
		static constexpr int32_t Month = 10;
		static constexpr int32_t Day = 19;
		static constexpr int32_t Hour = 11;
		static constexpr int32_t Minute = 34;
		static const char* MajorVersion = "0";
		static const char* MinorVersion = "9.7";
	}
}