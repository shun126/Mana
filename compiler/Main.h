/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include <iosfwd>

namespace mana
{
	extern const char* GetInputFilename();
	extern const char* GetOutputFilename();
	extern const char* GetTargetFilename();
	extern std::ofstream& GetPublicTypeDeclStream();
}
