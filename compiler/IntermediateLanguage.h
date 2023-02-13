/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include "../runner/common/Memory.h"
#include <cstdint>

namespace mana
{
	extern const char* get_string(const void* program, const address_t address, const void* buffer);
	extern const char* mana_get_instruction_text(const char* data, const void* program, const address_t address);
}
