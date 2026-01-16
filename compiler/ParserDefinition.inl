/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once

namespace mana
{
	/*!
	print error message
	@param	message		error message
	*/
	inline void Parser::error(const std::string& message)
	{
		mana::CompileError(message);
	}
}
