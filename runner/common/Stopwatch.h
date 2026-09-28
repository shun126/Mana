/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include <chrono>
#include <cstdint>

namespace mana
{
	[[nodiscard]] inline uint64_t GetMicroSecond()
	{
		const auto elapsed = std::chrono::steady_clock::now().time_since_epoch();
		return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count());
	}

	[[nodiscard]] inline float_t GetSecond()
	{
		return static_cast<float_t>(GetMicroSecond()) / static_cast<float_t>(1000000);
	}
}
