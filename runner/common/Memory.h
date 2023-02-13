/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include <cstdint>

namespace mana
{
	/*
	std::endian is available from C++20
	*/

	inline bool IsBigEndian()
	{
		static const int16_t one = 1;
		const char* pointer = (const char*)&one;
		return pointer[0] == 0;
	}

	inline int16_t SwapEndian(const int16_t value)
	{
		return (value << 8 & 0xFF00) | (value >> 8 & 0x00FF);
	}

	inline uint16_t SwapEndian(const uint16_t value)
	{
		return (value << 8 & 0xFF00) | (value >> 8 & 0x00FF);
	}

	inline int32_t SwapEndian(const int32_t value)
	{
		return (value << 24 & 0xFF000000) | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | (value >> 24 & 0x000000FF);
	}

	inline uint32_t SwapEndian(const uint32_t value)
	{
		return (value << 24 & 0xFF000000) | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | (value >> 24 & 0x000000FF);
	}

	inline float SwapEndian(const float value)
	{
		return static_cast<float>(SwapEndian(static_cast<uint32_t>(value)));
	}
}
