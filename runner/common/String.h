/*!
mana (compiler/library)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "Platform.h"
#include <cstdarg>
#include <cstring>
#include <string>

#if defined(MANA_TARGET_WINDOWS)
#define NOMINMAX
#include <windows.h>
#endif

namespace mana
{
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

	inline std::string FormatText(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		std::string output = std::move(FormatText(format, ap));
		va_end(ap);
		return output;
	}

	inline std::string FormatText(const char* format, va_list ap)
	{
		static const size_t gainSize = 100;
		size_t size = gainSize;
		char* p = nullptr;
		int n;

		do{
			p = reinterpret_cast<char*>(realloc(p, size));
			if (p == nullptr)
				throw std::bad_alloc();

			n = std::vsnprintf(p, size, format, ap);
			size += gainSize;
		} while(n < 0);

		const std::string a(p);
		std::free(p);

		return a; // TODO
	}

	inline void Trace1(const char* message)
	{
#if defined(MANA_TARGET_WINDOWS)
		OutputDebugStringA(message);
#else
		std::puts(message);
#endif
	}

	inline void Trace(const char* format, ...)
	{
		va_list argptr;
		char* message = nullptr;
		size_t size = 0;
		size_t length;

		do{
			size += 256;

			message = (char*)std::realloc(message, size);

			va_start(argptr, format);
#if defined(MANA_TARGET_WINDOWS)
			length = vsprintf_s(message, size, format, argptr);
#else
			length = std::vsprintf(message, format, argptr);
#endif
			va_end(argptr);
		}while(length <= 0 || length > size);

		Trace1(message);

		std::free(message);
	}

	inline void Trace(const char* format, va_list ap)
	{

		static const size_t gainSize = 100;
		size_t size = gainSize;
		char* message = nullptr;
		int n;

		do{
			message = reinterpret_cast<char*>(realloc(message, size));
			if (message == nullptr)
				throw std::bad_alloc();

			n = std::vsnprintf(message, size, format, ap);
			size += gainSize;
		} while(n < 0);

		Trace1(message);

		std::free(message);
	}

	inline int32_t mana_string_find(const int8_t text[], const int8_t pattern[])
	{
		int32_t i, j, k, c;

		c = pattern[0];
		i = 0;
		while(text[i] != 0)
		{
			if(text[i++] == c)
			{
				k = i;
				j = 1;
				while(text[k] == pattern[j] && pattern[j] != 0)
				{
					k++;
					j++;
				}
				if(pattern[j] == '\0')
				{
					return k - j;
				}
			}
		}

		return -1;
	}
}

#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
//! コンソールに文字列を出力
#define MANA_PRINT(...)		mana::Trace(__VA_ARGS__)
//! コンソールに文字列を出力(デバッグビルドのみ)
#define MANA_TRACE(...)		mana::Trace(__VA_ARGS__)
#else
//! コンソールに文字列を出力
#define MANA_PRINT(...)		printf(__VA_ARGS__)
//! コンソールに文字列を出力(デバッグビルドのみ)
#define MANA_TRACE(...)		((void)0)
#endif
