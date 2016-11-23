/*
 * mana (compiler)
 *
 * @file	mana_pool.c
 * @brief	文字列格納に関するソースファイル
 * @detail	このファイルは文字列を高速に参照するためのハッシュに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MALLOC_H___)
#include "../library/mana_malloc.h"
#endif
#if !defined(___MANA_POOL_H___)
#include "mana_pool.h"
#endif
#include <string.h>

/*! String hash table size */
#define MANA_POOL_HASH_TABLE_SIZE	(887)

/*! String hash table type */
typedef struct mana_pool_string_hash_table
{
	char* name;									/*!< 文字列 */
	int length;									/*!< 文字列の長さ */
	struct mana_pool_string_hash_table* next;	/*!< 次のmana_pool_string_hash_table */
} mana_pool_string_hash_table;

/*! String hash table */
static mana_pool_string_hash_table *s_mana_pool_hash_table[MANA_POOL_HASH_TABLE_SIZE];

/*!
 * @brief		ハッシュ値を取得
 * @param[in]	string		文字列
 * @return		ハッシュ値
 */
static int mana_pool_get_hash_value(char* string)
{
	unsigned h, g;

	for(h = 0; *string != '\0'; string++)
	{
		h = (h << 4) + (unsigned)(*string);
		g = h & 0xf0000000;
		if(g)
			h = (h ^ g >> 24) ^ g;
	}

	return(h % MANA_POOL_HASH_TABLE_SIZE);
}

#if 0
// ハッシュキーを作る
GzUInt32 hashstr(const char* str, size_t len)
{
	GzUInt32 h	= len;
	size_t step = (len>>5) | 1;

	for(int i=0; i<=len; i += step )
	{
		h = h ^ ((h<<5)+(h>>2)+(unsigned short)*(str++));
	}
	return h;
}
#endif

/*!
 */
void mana_pool_initialize(void)
{
	int i;

	for(i = 0; i < MANA_POOL_HASH_TABLE_SIZE; i ++)
	{
		s_mana_pool_hash_table[i] = 0;
	}
}

/*!
 */
void mana_pool_finalize(void)
{
	int i;

	for(i = 0; i < MANA_POOL_HASH_TABLE_SIZE; i++)
	{
		mana_pool_string_hash_table* hash;
		hash = s_mana_pool_hash_table[i];
		while(hash != 0)
		{
			mana_pool_string_hash_table* pNext;
			pNext = hash->next;
			if(hash->name)
				mana_free(hash->name);
			mana_free(hash);
			hash = pNext;
		}
		s_mana_pool_hash_table[i] = 0;
	}
}

/*!
 * @param[in]	string	文字列
 * @return
 * 文字列のアドレス。
 * 重複していた場合は設定済みの文字列のアドレスが返る。
 * 設定失敗の場合はNULLが返る。
 */
char* mana_pool_set(char* string)
{
	mana_pool_string_hash_table* hash;
	int hash_value;
	int length;

	if(string == NULL)
		return NULL;

	hash_value = mana_pool_get_hash_value(string);
	length = strlen(string);

	for(hash = s_mana_pool_hash_table[hash_value]; hash != 0; hash = hash->next)
	{
		if((hash->length) == length && strcmp(hash->name, string) == 0)
		{
			return hash->name;
		}
	}
	hash = (mana_pool_string_hash_table*)mana_malloc(sizeof(mana_pool_string_hash_table));
	if(hash)
	{
		hash->name = (char*)mana_malloc(length + 1);
		if(hash->name)
		{
			hash->length = length;
			hash->next = s_mana_pool_hash_table[hash_value];
			s_mana_pool_hash_table[hash_value] = hash;

#if defined(__STDC_WANT_SECURE_LIB__)
			strcpy_s(hash->name, length + 1, string);
#else
			strcpy(hash->name, string);
#endif
			return hash->name;
		}
		else
		{
			mana_free(hash);
		}
	}

	return NULL;
}
