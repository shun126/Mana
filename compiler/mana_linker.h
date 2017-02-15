/*
 * mana (compiler)
 *
 * @file	mana_link.h
 * @brief	リンクに関するヘッダファイル
 * @detail	このファイルはリンクに関係するヘッダファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_LINKER_H___)
#define ___MANA_LINKER_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_NODE_H___)
#include "mana_node.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

extern void mana_linker_generate_symbol(mana_node* node);
extern void mana_linker_generate_code(mana_node* node);

extern void mana_linker_return(mana_symbol_entry* symbol, mana_node* node);
extern void mana_linker_rollback(mana_node* node);
extern void mana_linker_call_print(mana_node* node);
extern int32_t mana_linker_argument(mana_symbol_entry* symbol, mana_node* node);
extern void mana_linker_expression(mana_node* node, int32_t enable_assign);
extern int32_t mana_linker_condition(mana_node* node, int32_t match);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
