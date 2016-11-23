/*
 * mana (compiler)
 *
 * @file	mana_register.c
 * @brief	レジスタ割り当てに関するソースファイル
 * @detail	このファイルは型情報に関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_INFO_H___)
#include "../library/mana_info.h"
#endif
#if !defined(___MANA_REGISTER_H___)
#include "mana_register.h"
#endif

static mana_register_entity entities[MANA_REGISTER_COUNT];

void mana_register_initialzie(void)
{
	char i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		mana_register_entity* entity = &entities[i];
		entity->symbol = NULL;
		entity->number = i;
	}
}

void mana_register_finalize(void)
{
	mana_register_clear();
}

void mana_register_clear(void)
{
	size_t i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		entities[i].symbol = NULL;
	}
}

mana_register_entity* mana_register_find(mana_symbol_entry* symbol)
{
	size_t i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		mana_register_entity* entity = &entities[i];
		if(entity->symbol == symbol)
		{
			return entity;
		}
	}

	return NULL;
}

/* TODO:無名レジスタの登録が必要
*/

mana_register_entity* mana_register_allocate(mana_symbol_entry* symbol, size_t address)
{
	size_t i;

	for(i = 0; i < MANA_REGISTER_COUNT; ++i)
	{
		mana_register_entity* entity = &entities[i];
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

void mana_register_release(mana_register_entity* entity)
{
	if(entity)
	{
		entity->symbol = NULL;
	}
}
