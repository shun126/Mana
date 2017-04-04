/*!
mana (compiler)

@file	linker.c
@brief	リンクに関するソースファイル
@detail	このファイルはリンクに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_LINKER_H___)
#include "linker.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "code.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "../library/mana_malloc.h"
#endif

typedef struct mana_linker_call_list
{
	symbol_entry* symbol;
	int32_t code_address;
	struct mana_linker_call_list* next;
}mana_linker_call_list;

mana_linker_call_list* mana_linker_call_list_root_pointer = NULL;

void mana_linker_initialize(void)
{
}

void mana_linker_finalize(void)
{
	mana_linker_call_list* self = mana_linker_call_list_root_pointer;
	while(self)
	{
		mana_linker_call_list* next = self->next;
		mana_free(self);
		self = next;
	}
	mana_linker_call_list_root_pointer = NULL;
}

void mana_linker_add_call_list(const int32_t code_address, symbol_entry* symbol)
{
	mana_linker_call_list* entity = mana_malloc(sizeof(mana_linker_call_list));
	entity->symbol = symbol;
	entity->code_address = code_address;

	entity->next = mana_linker_call_list_root_pointer;
	mana_linker_call_list_root_pointer = entity;
}

void mana_linker_resolve_address()
{
	for(const mana_linker_call_list* self = mana_linker_call_list_root_pointer; self; self = self->next)
	{
		// TODO:シンボルのアドレスが解決していない可能性が残るので対処をお願いします。
		mana_code_replace_address(self->code_address, self->symbol->address);
	}
}
