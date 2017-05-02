/*!
mana (compiler)

@file	register.c
@brief	レジスタ割り当てに関するソースファイル
@detail	このファイルは型情報に関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_INFO_H___)
#include "../library/mana_info.h"
#endif
#if !defined(___MANA_REGISTER_H___)
#include "register.h"
#endif

static register_entity entities[MANA_REGISTER_COUNT];

void register_initialzie(void)
{
	int8_t i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		register_entity* entity = &entities[i];
		entity->symbol = NULL;
		entity->number = i;
	}
}

void register_finalize(void)
{
	register_clear();
}

void register_clear(void)
{
	size_t i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		entities[i].symbol = NULL;
	}
}

register_entity* register_find(symbol_entry* symbol)
{
	size_t i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		register_entity* entity = &entities[i];
		if(entity->symbol == symbol)
		{
			return entity;
		}
	}

	return NULL;
}

/* TODO:無名レジスタの登録が必要
*/

register_entity* register_allocate(symbol_entry* symbol, size_t address)
{
	size_t i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		register_entity* entity = &entities[i];
		if(entity->symbol == NULL)
		{
			entity->symbol = symbol;
			entity->address = address;
			return entity;
		}
		else if(entity->symbol == symbol)
		{
			// already allocated
			return entity;
		}
	}

	return NULL;
}

void register_release(register_entity* entity)
{
	if(entity)
	{
		entity->symbol = NULL;
	}
}
