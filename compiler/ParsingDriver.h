/*!
mana (compiler)

@author	Shun Moriya
@date	2022-
*/

#pragma once
#include <cstdint>

namespace mana
{
	class ParsingDriver final
	{
	public:
		ParsingDriver();
		~ParsingDriver() = default;

		int32_t Parse();
	};
}
