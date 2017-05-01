/*!
mana (compiler)

@file	lexer.h
@brief	字句解析に関係するヘッダーファイル
@detail	このファイルは字句解析に関係するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_LEXER_H___)
#define ___MANA_LEXER_H___

#include <stdint.h>
#include <stdbool.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*!
initialize scanner
@param	filename	file name pointer
@retval	true		success
@retval	false		failed
*/
extern int32_t mana_lexer_initialize(const char* filename);

/*!
finalize scanner
*/
extern void mana_lexer_finalize(void);

/*!
open file
@param	filename	file name pointer
@param	check
@retval	true		success
@retval	false		failed
*/
extern int32_t mana_lexer_open(const char* filename, const bool check);

/*!
close file
@retval	true	scanning complete.
@retval	false	still need scanning.
*/
extern int32_t mana_lexer_close(void);

/*!
get current file name
@return	current file name pointer
*/
extern const char* mana_lexer_get_current_filename(void);

/*!
set current file name
@param[in]	filename	current file name pointer
*/
extern void mana_lexer_set_current_filename(const char* filename);

/*!
get current line number
@return	current line number
*/
extern int mana_lexer_get_current_line(void);

/*!
set current line number
@param[in]	lineno	current line number
*/
extern void mana_lexer_set_current_line(const int lineno);

extern int yylex(void);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
