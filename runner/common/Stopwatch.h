/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"

#if defined(MANA_TARGET_WINDOWS)
#define NOMINMAX
#include <windows.h>
#elif defined(MANA_TARGET_APPLE)
#include <sys/time.h>
#else
#include <sys/time.h>
#endif

namespace mana
{
	[[nodiscard]] inline uint64_t GetMicroSecond()
	{
#if defined(MANA_TARGET_WINDOWS)
		LARGE_INTEGER frequency, counter;

		if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&counter))
			return 0;

		return static_cast<uint64_t>(counter.QuadPart * 1000000 / frequency.QuadPart);
#else
		struct timeval current;
		return (gettimeofday(&current, nullptr) == 0) ? current.tv_usec : 0;
#endif
	}

	[[nodiscard]] inline float_t GetSecond()
	{
		return static_cast<float_t>(GetMicroSecond()) / static_cast<float_t>(1000000);
	}
}
