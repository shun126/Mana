/*!
mana (compiler)

@file	error.h
@brief	エラーや警告に関係するヘッダーファイル
@detail	このファイルはエラーや警告に関係するヘッダーファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_ERROR_H___)
#define ___MANA_ERROR_H___

#include <stdarg.h>
#include <stddef.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern int yynerrs;

extern void mana_compile_error(const char* format, ...);
extern void mana_compile_warning(const char* format, ...);

extern void mana_linker_error(const char* format, ...);
extern void mana_linker_warning(const char* format, ...);

extern void mana_fatal(const char* format, ...);
extern void mana_fatal_no_memory(void);

extern void yyerror(const char* message);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
