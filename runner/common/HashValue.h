/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Assert.h"

namespace mana
{
	struct HashValueGenerate
	{
		std::size_t operator()(const char* name) const
		{
			MANA_ASSERT(name);

			uint32_t h = 0;
			while (*name != '\0')
			{
				h = (h << 4) + static_cast<uint32_t>(*name);
				uint32_t g = h & 0xf0000000;
				if (g)
				{
					h = (h ^ g >> 24) ^ g;
				}
				++name;
			}
			return static_cast<size_t>(h);
		}
	};

	class HashValueCompare// : public std::binary_function<const char*, const char*, bool>
	{
	public:
		using first_argument_type = const char*;
		using second_argument_type = const char*;
		using result_type = bool;

		bool operator()(const char* lhs, const char* rhs) const
		{
			return std::strcmp(lhs, rhs) == 0;
		}
	};
}
