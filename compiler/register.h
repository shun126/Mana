/*
mana (compiler)

@file	mana_register.h
@brief	レジスタ割り当てに関するヘッダーファイル
@detail	このファイルはレジスタ割り当てに関するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_REGISTER_H___)
#define ___MANA_REGISTER_H___

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif

typedef struct mana_register_entity
{
	mana_symbol_entry* symbol;
	size_t address;
	int8_t number;
}mana_register_entity;

extern void mana_register_initialzie(void);
extern void mana_register_finalize(void);
extern void mana_register_clear(void);

extern mana_register_entity* mana_register_find(mana_symbol_entry* symbol);

extern mana_register_entity* mana_register_allocate(mana_symbol_entry* symbol, size_t address);
extern void mana_register_release(mana_register_entity* entity);

#endif
