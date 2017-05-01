/*!
mana (compiler)

@file	linker.h
@brief	リンクに関するヘッダファイル
@detail	このファイルはリンクに関係するヘッダファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_LINKER_H___)
#define ___MANA_LINKER_H___

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void mana_linker_initialize(void);
extern void mana_linker_finalize(void);

extern void mana_linker_add_call_list(const int32_t code_address, symbol_entry* symbol);
extern void mana_linker_resolve_address();

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
