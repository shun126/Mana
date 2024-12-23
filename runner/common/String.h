/*!
mana (compiler/library)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "Platform.h"
#include <cstring>
#include <iostream>
#include <string>

#if defined(MANA_TARGET_WINDOWS)
#define NOMINMAX
#include <windows.h>
#endif

namespace mana
{
	[[nodiscard]] inline std::string Concat(const std::initializer_list<std::string_view>& r)
	{
		std::size_t n = 0;
		for (auto& it : r)
			n += it.size();
		std::string l;
		l.reserve(n);
		for (auto& it : r)
			l += it;
		return l;
	}

	inline void Trace(const std::string& message)
	{
#if defined(MANA_TARGET_WINDOWS) && (MANA_BUILD_TARGET == MANA_BUILD_DEBUG)
		OutputDebugStringA(message.c_str());
#endif
		std::cout << message;
	}

	inline void Trace(const std::initializer_list<std::string_view> message)
	{
		Trace(Concat(message));
	}

	inline void strcpy(char* outBuffer, size_t outBufferSize, const char* inBuffer)
	{
#if defined(MANA_TARGET_WINDOWS)
		strcpy_s(outBuffer, outBufferSize, inBuffer);
#else
		std::strncpy(outBuffer, inBuffer, outBufferSize);
#endif
	}

	inline void strcat(char* outBuffer, size_t outBufferSize, const char* inBuffer)
	{
#if defined(MANA_TARGET_WINDOWS)
		strcat_s(outBuffer, outBufferSize, inBuffer);
#else
		std::strncat(outBuffer, inBuffer, outBufferSize);
#endif
	}

	inline int strcmp(char const* buffer1, char const* buffer2)
	{
		return std::strcmp(buffer1, buffer2);
	}
}

//! コンソールに文字列を出力
#define MANA_PRINT(...)		mana::Trace(__VA_ARGS__)
#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
//! コンソールに文字列を出力(デバッグビルドのみ)
#define MANA_TRACE(...)		mana::Trace(__VA_ARGS__)
#else
//! コンソールに文字列を出力(デバッグビルドのみ)
#define MANA_TRACE(...)		((void)0)
#endif
