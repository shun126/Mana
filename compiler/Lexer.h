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

	namespace lexer
	{
		/*!
		initialize scanner
		@param[in]	parsingDriver	Bison parser object
		@param[in]	filename		file name
		@retval		true			success
		@retval		false			failed
		*/
		extern bool Initialize(const std::shared_ptr<mana::ParsingDriver>& parsingDriver, const std::string_view& filename);

		/*!
		finalize scanner
		*/
		extern void Finalize();

		/*!
		open file
		@param	filename	file name
		@param	check		Checks for duplicate file openings
		@retval	true		success
		@retval	false		failed
		*/
		extern bool Open(const std::string_view& filename, const bool check);

		/*!
		close file
		@retval	true	scanning complete.
		@retval	false	still need scanning.
		*/
		extern bool Close();

		/*!
		get current file name
		@return	current file name pointer
		*/
		extern const std::string& GetCurrentFilename();

		/*!
		set current file name
		@param[in]	filename	current file name pointer
		*/
		extern void SetCurrentFilename(const std::string& filename);

		/*!
		get current line number
		@return	current line number
		*/
		extern int GetCurrentLineNo();

		/*!
		set current line number
		@param[in]	lineNo	current line number
		*/
		extern void SetCurrentLineNo(const int lineNo);
	}
}
