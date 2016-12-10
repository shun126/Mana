/*
 * mana (compiler)
 *
 * @file	mana_data.c
 * @brief	データセクションに関するソースファイル
 * @detail	このファイルはデータセクションに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_INFO_H___)
#include "../library/mana_info.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "../library/mana_malloc.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
#endif
#include <stdio.h>
#include <string.h>

typedef struct mana_data_list
{
	unsigned int address;							/*!< address */
	struct mana_data_list* next;					/*!< next mana_data_list block address */
} mana_data_list;

typedef struct mana_data
{
	mana_data_list* constant;						/*!< Constant list top address */
	char* buffer;									/*!< Constant pool data address */
	unsigned int used_size;							/*!< Constant pool size */
	unsigned int allocated_size;					/*!< Allocated size */
} mana_data;

static mana_data mana_data_buffer;

/*!
 */
void mana_data_initialzie(void)
{
	mana_data_buffer.constant = NULL;
	mana_data_buffer.buffer = NULL;
	mana_data_buffer.used_size = 0;
	mana_data_buffer.allocated_size = 0;
}

/*!
 */
void mana_data_finalize(void)
{
	mana_data_list* list;
	list = mana_data_buffer.constant;
	while(list)
	{
		mana_data_list* next;
		next = list->next;
		mana_free(list);
		list = next;
	}
	mana_data_buffer.constant = NULL;
	if(mana_data_buffer.buffer)
	{
		mana_free(mana_data_buffer.buffer);
		mana_data_buffer.buffer = NULL;
	}
	mana_data_buffer.allocated_size = 0;
	mana_data_buffer.used_size = 0;
}

/*!
 * @return	データセクションバッファのアドレス
 */
char* mana_data_get_buffer(void)
{
	return mana_data_buffer.buffer;
}

/*!
 * @return	データセクションのサイズ
 */
unsigned int mana_data_get_size(void)
{
	return mana_data_buffer.used_size;
}

static mana_data_list* mana_data_find(char* text)
{
	if(text)
	{
		mana_data_list* list;
		list = mana_data_buffer.constant;
		while(list)
		{
			if(strcmp((const char*)&mana_data_buffer.buffer[list->address], (const char*)text) == 0)
				return list;
			list = list->next;
		}
	}
	return NULL;
}

/*!
 * @param[in]	text	テキスト
 * @return		オフセットアドレス
 */
unsigned int mana_data_get(char* text)
{
	mana_data_list* list;
	list = mana_data_find(text);
	return list == NULL ? (unsigned int)-1 : list->address;
}

/*!
 * @param[in]	text	テキスト
 * @return		オフセットアドレス
 */
unsigned int mana_data_set(char* text)
{
	mana_data_list* list;
	list = mana_data_find(text);
	if(list == NULL)
	{
		int length;
		list = (mana_data_list*)mana_calloc(1, sizeof(mana_data_list));
		if(list == NULL)
		{
			mana_data_finalize();
			/* error message */
			return (unsigned int)~0;
		}
		length = strlen(text) + 1;
		if(mana_data_buffer.used_size + length >= mana_data_buffer.allocated_size)
		{
			mana_data_buffer.allocated_size += (mana_data_buffer.used_size + length + 4096);
			mana_data_buffer.buffer = (char*)mana_realloc(mana_data_buffer.buffer, mana_data_buffer.allocated_size);
			if(mana_data_buffer.buffer == NULL)
			{
				mana_data_finalize();
				/* error message */
				return (unsigned int)~0;
			}
		}
		memcpy(mana_data_buffer.buffer + mana_data_buffer.used_size, text, length);
		list->address = mana_data_buffer.used_size;
		list->next = mana_data_buffer.constant;
		mana_data_buffer.constant = list;
		mana_data_buffer.used_size += length;
	}
	return list->address;
}

/*!
 * @param[out]	file		ファイル識別子
 * @retval		MANA_TRUE	出力成功
 * @retval		MANA_FALSE	出力失敗
 */
unsigned int mana_data_write(mana_stream* stream)
{
	mana_stream_push_data(stream, mana_data_buffer.buffer, mana_data_buffer.used_size);
	return 0;
}
