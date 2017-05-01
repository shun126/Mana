/*!
mana (compiler)

@file	data.c
@brief	データセクションに関するソースファイル
@detail	このファイルはデータセクションに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_INFO_H___)
#include "../library/mana_info.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "../library/mana_malloc.h"
#endif
#if !defined(___SYMBOL_DATA_H___)
#include "data.h"
#endif
#include <stdio.h>
#include <string.h>

// データリスト
typedef struct data_list
{
	uint32_t address;			//!< address
	struct data_list* next;		//!< next data_list block address
} data_list;

//! データクラス
typedef struct data_entry
{
	data_list* constant;		//!< Constant list top address
	char* buffer;				//!< Constant pool data address
	uint32_t used_size;			//!< Constant pool size
	uint32_t allocated_size;	//!< Allocated size
} data_entry;

static data_entry data_instance;

void data_initialzie(void)
{
	data_instance.constant = NULL;
	data_instance.buffer = NULL;
	data_instance.used_size = 0;
	data_instance.allocated_size = 0;
}

void data_finalize(void)
{
	data_list* list;
	list = data_instance.constant;
	while(list)
	{
		data_list* next;
		next = list->next;
		mana_free(list);
		list = next;
	}
	data_instance.constant = NULL;
	if(data_instance.buffer)
	{
		mana_free(data_instance.buffer);
		data_instance.buffer = NULL;
	}
	data_instance.allocated_size = 0;
	data_instance.used_size = 0;
}

char* data_get_buffer(void)
{
	return data_instance.buffer;
}

uint32_t data_get_size(void)
{
	return data_instance.used_size;
}

/*!
検索
@param[in]	text	文字列
@return		data_listのアドレス
*/
static data_list* data_find(const char* text)
{
	if(text)
	{
		data_list* list;
		list = data_instance.constant;
		while(list)
		{
			if(strcmp((const char*)&data_instance.buffer[list->address], (const char*)text) == 0)
				return list;
			list = list->next;
		}
	}
	return NULL;
}

uint32_t data_get(const char* text)
{
	data_list* list;
	list = data_find(text);
	return list == NULL ? (uint32_t)-1 : list->address;
}

uint32_t data_set(const char* text)
{
	data_list* list = data_find(text);
	if(list == NULL)
	{
		list = (data_list*)mana_calloc(1, sizeof(data_list));
		if(list == NULL)
		{
			data_finalize();
			/* error message */
			return (uint32_t)~0;
		}
		uint32_t length = strlen(text) + 1;
		if(data_instance.used_size + length >= data_instance.allocated_size)
		{
			data_instance.allocated_size += (data_instance.used_size + length + 4096);
			data_instance.buffer = (char*)mana_realloc(data_instance.buffer, data_instance.allocated_size);
			if(data_instance.buffer == NULL)
			{
				data_finalize();
				/* error message */
				return (uint32_t)~0;
			}
		}
		memcpy(data_instance.buffer + data_instance.used_size, text, length);
		list->address = data_instance.used_size;
		list->next = data_instance.constant;
		data_instance.constant = list;
		data_instance.used_size += length;
	}
	return list->address;
}

bool data_write(mana_stream* stream)
{
	mana_stream_push_data(stream, data_instance.buffer, data_instance.used_size);
	return true;
}
