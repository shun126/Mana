/*!
mana (compiler)

@file	register.h
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

typedef struct register_entity
{
	symbol_entry* symbol;
	size_t address;
	int8_t number;
}register_entity;

extern void register_initialzie(void);
extern void register_finalize(void);
extern void register_clear(void);

extern register_entity* register_find(symbol_entry* symbol);

extern register_entity* register_allocate(symbol_entry* symbol, size_t address);
extern void register_release(register_entity* entity);

#endif
