/*!
mana (library)

@author	Shun Moriya
@date	2026-
*/

/*
Do not use the macros in this file,
as the handling of strings within mana has not been finalized.
*/

#pragma once
#include "common/Platform.h"
#include <string>
#include <string_view>

#if defined(UE_BUILD)
// Unreal Engine environment (applies to Shipping/Development/Debug)
#define MANA_CHAR        TCHAR
#define MANA_TEXT(text)  TEXT(text)

#elif defined(_WIN32) || defined(_WIN64)
// Non‑UE Windows environment
#define NOMINMAX
#include <stdio.h>
#include <tchar.h>
#define MANA_CHAR        TCHAR
#define MANA_TEXT(text)  _T(text)

#else
// Non‑UE, non‑Windows environment
#include <iostream>
#define MANA_CHAR        char
#define MANA_TEXT(text)  (text)
#endif

namespace mana
{
	[[nodiscard]] inline std::basic_string<MANA_CHAR> LogConcat(std::initializer_list<std::basic_string_view<MANA_CHAR>> r)
	{
		std::size_t n = 0;
		for (auto s : r)
			n += s.size();
		std::basic_string<MANA_CHAR> l;
		l.reserve(n);
		for (auto s : r)
			l.append(s);
		return l;
	}
}

#if defined(UE_BUILD)
    // Unreal Engine environment (applies to Shipping/Development/Debug)
	#define MANA_LOG_ERROR(...)   UE_LOG(LogTemp, Error, MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }).c_str())
	#define MANA_LOG_WARNING(...) UE_LOG(LogTemp, Warning, MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }).c_str())
	#define MANA_LOG_DISPLAY(...) UE_LOG(LogTemp, Display, MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }).c_str())
	#define MANA_LOG_INFO(...)    UE_LOG(LogTemp, Log, MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }).c_str())

#elif defined(_WIN32) || defined(_WIN64)
    // Non‑UE Windows environment
	#define MANA_LOG_ERROR(...)   _tprintf(MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }))
	#define MANA_LOG_WARNING(...) _tprintf(MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }))
	#define MANA_LOG_DISPLAY(...) _tprintf(MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }))
	#define MANA_LOG_INFO(...)    _tprintf(MANA_TEXT("%s\n"), mana::LogConcat({ __VA_ARGS__ }))

#else
    // Non‑UE, non‑Windows environment
	#define MANA_LOG_ERROR(...)   (std::cout << mana::LogConcat({ __VA_ARGS__ }) << '\n')
	#define MANA_LOG_WARNING(...) (std::cout << mana::LogConcat({ __VA_ARGS__ }) << '\n')
	#define MANA_LOG_DISPLAY(...) (std::cout << mana::LogConcat({ __VA_ARGS__ }) << '\n')
	#define MANA_LOG_INFO(...)    (std::cout << mana::LogConcat({ __VA_ARGS__ }) << '\n')
#endif
