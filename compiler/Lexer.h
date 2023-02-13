/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"

extern void yyerror(const char* message);
extern int yynerrs;

namespace mana
{
	class ParsingDriver;

	/*!
	initialize scanner
	@param	filename	file name pointer
	@retval	true		success
	@retval	false		failed
	*/
	extern bool lexer_initialize(const std::shared_ptr<mana::ParsingDriver>& parsingDriver, const std::string_view filename);

	/*!
	finalize scanner
	*/
	extern void lexer_finalize(void);

	/*!
	open file
	@param	filename	file name pointer
	@param	check
	@retval	true		success
	@retval	false		failed
	*/
	extern bool lexer_open(const std::string_view filename, const bool check);

	/*!
	close file
	@retval	true	scanning complete.
	@retval	false	still need scanning.
	*/
	extern bool lexer_close(void);

	/*!
	get current file name
	@return	current file name pointer
	*/
	extern const std::string& lexer_get_current_filename(void);

	/*!
	set current file name
	@param[in]	filename	current file name pointer
	*/
	extern void lexer_set_current_filename(const std::string& filename);

	/*!
	get current line number
	@return	current line number
	*/
	extern int lexer_get_current_lineno(void);

	/*!
	set current line number
	@param[in]	lineno	current line number
	*/
	extern void lexer_set_current_lineno(const int lineno);
}
