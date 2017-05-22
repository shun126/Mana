/*!
mana (compiler)

@author	Shun Moriya
@date	2022-
*/

#include "ParsingDriver.h"
#include "../runner/common/Setup.h"
#include "SyntaxNode.h"
#include "Parser.hpp"

namespace mana
{
	ParsingDriver::ParsingDriver()
	{
	}

	int32_t ParsingDriver::Parse()
	{
		Parser parser;
		return parser.parse();
	}
}
