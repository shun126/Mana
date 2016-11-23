/*
 * mana (compiler)
 *
 * @file	mana_symbol.c
 * @brief	型情報に関するヘッダーファイル
 * @detail	このファイルは型情報に関係するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_TYPE_H___)
#define ___MANA_TYPE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stdio.h>
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

#define CBSZ sizeof(char)
#define BBSZ sizeof(unsigned char)
#define SBSZ sizeof(short)
#define IBSZ sizeof(int)
#define FBSZ sizeof(float)

extern void mana_type_initialize(void);
extern void mana_type_finalize(void);
extern mana_type_description* mana_type_get(mana_symbol_data_type_id);
extern mana_type_description* mana_type_create(mana_symbol_data_type_id, mana_type_description*, mana_type_description*);
extern mana_type_description* mana_type_create_reference(mana_type_description*);
extern mana_type_description* mana_type_create_array(int);
extern void mana_type_set_array(mana_type_description*, mana_type_description*);
extern int mana_type_compare(mana_type_description*, mana_type_description*);
extern int mana_type_compatible(mana_type_description*, mana_type_description*);
extern void mana_type_print(FILE*, mana_type_description*);

extern mana_type_description* mana_type_actor;
extern mana_type_description* mana_type_string;
extern mana_type_description* mana_type_pointer;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
