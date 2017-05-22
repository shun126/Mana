/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Memory.h"
#include <cstdint>

namespace mana
{
	template <typename T>
	T get(const void* program, const int32_t address)
	{
		T value;
		uint8_t* pointer = reinterpret_cast<uint8_t*>(&value);

		if (IsBigEndian())
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				pointer[i] = static_cast<const int8_t*>(program)[address + i];
		}
		else
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				pointer[i] = static_cast<const int8_t*>(program)[address + static_cast<uint_fast8_t>(sizeof(T) - 1) - i];
		}

		return value;
	}

	extern const char* get_string(const void* program, const int32_t address, const void* buffer);

	extern const char* mana_get_instruction_text(const char* data, const void* program, const int32_t address);
}
