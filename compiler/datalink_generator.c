/*!
mana (compiler)

@file	datalink_generator.c
@brief	データセクションに関するソースファイル
@detail	このファイルはデータセクションに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_DATALINK_GENERATOR_H___)
#include "datalink_generator.h"
#endif
#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MANA_DATALINK_ALIGNMENT_SIZE	0x10

#if !defined(MANA_NIL)
#define MANA_NIL 0
#endif

// データリンクリスト
typedef struct datalink_generator_entry
{
	char* file_name;
	size_t file_size;
	size_t padding_size;
	struct datalink_generator_entry* next;
} datalink_generator_entry;

static size_t number_of_files = 0;
static datalink_generator_entry* instance = NULL;

/*!
 * get last entry
 * @return	last datalink_generator_entry pointer
 */
static datalink_generator_entry* datalink_generator_get_last_entry(void)
{
	if(instance)
	{
		datalink_generator_entry* last_entry;
		for(last_entry = instance; last_entry->next != NULL; last_entry = last_entry->next)
			;
		return last_entry;
	}
	else
	{
		return MANA_NIL;
	}
}

void datalink_generator_initialize(void)
{
	datalink_generator_finalize();
}

void datalink_generator_finalize(void)
{
	datalink_generator_entry* entry = instance;
	while(entry)
	{
		datalink_generator_entry* next = entry->next;
		if(entry->file_name)
			mana_free(entry->file_name);
		mana_free(entry);
		entry = next;
	}
	instance = NULL;
	number_of_files = 0;
}

size_t datalink_generator_get_number_of_files(void)
{
	return number_of_files;
}

int32_t datalink_generator_append(const char* file_name)
{
	FILE* file;
	datalink_generator_entry* entry;
	size_t alignment_size = MANA_DATALINK_ALIGNMENT_SIZE;
	size_t string_length;

	if(file_name == NULL)
	{
		mana_compile_error("file name not found");
		return -1;
	}

	string_length = strlen(file_name) + 1;

	entry = mana_calloc(sizeof(datalink_generator_entry), 1);
	entry->file_name = mana_malloc(string_length);

#if defined(__STDC_WANT_SECURE_LIB__)
	strcpy_s(entry->file_name, string_length, file_name);
#else
	strcpy(entry->file_name, file_name);
#endif

#if defined(__STDC_WANT_SECURE_LIB__)
	if(fopen_s(&file, file_name, "rb") != 0)
#else
	if((file = fopen(file_name, "rb")) == NULL)
#endif
	{
		mana_free(entry->file_name);
		mana_compile_error("unable to open '%s'", file_name);
		return -1;
	}
	else
	{
		datalink_generator_entry* last_entry;
		size_t index;

		fseek(file, 0L, SEEK_END);
		entry->file_size = ftell(file);
		entry->padding_size = ((entry->file_size + (alignment_size - 1)) / alignment_size * alignment_size) - entry->file_size;
		entry->next = NULL;
		fclose(file);

		if(alignment_size != MANA_DATALINK_ALIGNMENT_SIZE)
		{
			last_entry = datalink_generator_get_last_entry();
			if(last_entry)
			{
				last_entry->padding_size = ((last_entry->file_size + (alignment_size - 1)) / alignment_size * alignment_size) - last_entry->file_size;
			}
		}
		last_entry = datalink_generator_get_last_entry();
		if(last_entry)
			last_entry->next = entry;
		else
			instance = entry;

		index = number_of_files;
		number_of_files++;
		return (int32_t)index;
	}
}

bool datalink_generator_write_data(mana_stream* stream)
{
	datalink_generator_entry* entry;
	mana_datalink_file_header header;
	size_t file_size;
	size_t header_size;

	srand( (unsigned)time( NULL ) );

	header.total_data_size = datalink_generator_get_number_of_files();
	header.number_of_datas = datalink_generator_get_number_of_files();
	mana_stream_push_data(stream, &header, sizeof(header));
	/*
	{
		mana_compile_error("file write failed");
		return false;
	}
	*/

	file_size = 0;
	for(entry = instance; entry != NULL; entry = entry->next)
	{
		mana_datalink_info_header entry_header;

		entry_header.offset = file_size;
		entry_header.size = entry->file_size;

		mana_stream_push_data(stream, &entry_header, sizeof(entry_header));
		/*
		if(fwrite(&entry_header, sizeof(entry_header), 1, file) != 1)
		{
			mana_compile_error("file write failed");
			return false;
		}
		*/

		file_size += entry->file_size + entry->padding_size;
	}

	header_size = (datalink_generator_get_number_of_files() * sizeof(mana_datalink_info_header)
		+ sizeof(mana_datalink_file_header)) % MANA_DATALINK_ALIGNMENT_SIZE;
	if(header_size > 0)
	{
		size_t i;

		header_size = MANA_DATALINK_ALIGNMENT_SIZE - header_size;
		for(i = 0; i < header_size; i++)
			mana_stream_push_unsigned_char(stream, (uint8_t)rand());
	}

	for(entry = instance; entry != NULL; entry = entry->next)
	{
		FILE* in;

#if defined(__STDC_WANT_SECURE_LIB__)
		if(fopen_s(&in, entry->file_name, "rb") != 0)
#else
		if((in = fopen(entry->file_name, "rb")) == NULL)
#endif
		{
			mana_compile_error("unable to open '%s'", entry->file_name);
			return false;
		}

		void* program = mana_malloc(entry->file_size);
		if(program == NULL)
		{
			fclose(in);
			printf("memory allocation failed\n");
			return false;
		}

		if(fread(program, 1, entry->file_size, in) != entry->file_size)
		{
			fclose(in);
			free(program);
			printf("file read failed: %s\n", entry->file_name);
			return false;
		}

		fclose(in);

		mana_stream_push_data(stream, program, entry->file_size);

		free(program);

		for(size_t i = 0; i < entry->padding_size; i ++)
			mana_stream_push_unsigned_char(stream, (uint8_t)rand());

		fclose(in);
	}

	return true;
}
