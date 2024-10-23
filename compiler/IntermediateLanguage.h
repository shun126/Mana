/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"

namespace mana
{
	extern const char* GetString(const void* program, const address_t address, const void* buffer);
	extern const char* GetInstructionText(const char* data, const void* program, const address_t address);
}
