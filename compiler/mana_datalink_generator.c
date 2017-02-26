/*
 * mana (compiler)
 *
 * @file	mana_datalink_generator.c
 * @brief	データセクションに関するソースファイル
 * @detail	このファイルはデータセクションに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_DATALINK_GENERATOR_H___)
#include "mana_datalink_generator.h"
#endif
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct mana_datalink_generator_file_entry
{
	char* file_name;
	size_t file_size;
	size_t padding_size;
	struct mana_datalink_generator_file_entry* next;
} mana_datalink_generator_file_entry;

static size_t mana_datalink_generator_number_of_files = 0;
static mana_datalink_generator_file_entry* mana_datalink_generator_file_entry_root = NULL;

/*!
 * get last entry
 * @return	last mana_datalink_generator_file_entry pointer
 */
static mana_datalink_generator_file_entry* mana_datalink_generator_get_last_entry(void)
{
	if(mana_datalink_generator_file_entry_root)
	{
		mana_datalink_generator_file_entry* last_entry;
		for(last_entry = mana_datalink_generator_file_entry_root; last_entry->next != NULL; last_entry = last_entry->next)
			;
		return last_entry;
	}
	else
	{
		return MANA_NIL;
	}
}

/*!
 * initialize datalink object
 */
void mana_datalink_generator_initialize(void)
{
	mana_datalink_generator_finalize();
}

/*!
 * finalize datalink object
 */
void mana_datalink_generator_finalize(void)
{
	mana_datalink_generator_file_entry* entry = mana_datalink_generator_file_entry_root;
	while(entry)
	{
		mana_datalink_generator_file_entry* next = entry->next;
		if(entry->file_name)
			mana_free(entry->file_name);
		mana_free(entry);
		entry = next;
	}
	mana_datalink_generator_file_entry_root = NULL;
	mana_datalink_generator_number_of_files = 0;
}

/*!
 * get data count in datalink
 * @return	data count
 */
size_t mana_datalink_generator_get_number_of_files(void)
{
	return mana_datalink_generator_number_of_files;
}

/*!
 * append data
 * @param[in]	file_name	file name
 * @return		index number. error if return value is negative.
 */
int32_t mana_datalink_generator_append(const char* file_name)
{
	FILE* file;
	mana_datalink_generator_file_entry* entry;
	size_t alignment_size = MANA_DATALINK_ALIGNMENT_SIZE;
	size_t string_length;

	if(file_name == NULL)
	{
		mana_parse_error("file name not found");
		return -1;
	}

	string_length = strlen(file_name) + 1;

	entry = mana_calloc(sizeof(mana_datalink_generator_file_entry), 1);
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
		mana_parse_error("unable to open '%s'", file_name);
		return -1;
	}
	else
	{
		mana_datalink_generator_file_entry* last_entry;
		size_t index;

		fseek(file, 0L, SEEK_END);
		entry->file_size = ftell(file);
		entry->padding_size = ((entry->file_size + (alignment_size - 1)) / alignment_size * alignment_size) - entry->file_size;
		entry->next = NULL;
		fclose(file);

		if(alignment_size != MANA_DATALINK_ALIGNMENT_SIZE)
		{
			last_entry = mana_datalink_generator_get_last_entry();
			if(last_entry)
			{
				last_entry->padding_size = ((last_entry->file_size + (alignment_size - 1)) / alignment_size * alignment_size) - last_entry->file_size;
			}
		}
		last_entry = mana_datalink_generator_get_last_entry();
		if(last_entry)
			last_entry->next = entry;
		else
			mana_datalink_generator_file_entry_root = entry;

		index = mana_datalink_generator_number_of_files;
		mana_datalink_generator_number_of_files++;
		return (int32_t)index;
	}
}

/*!
 * write datalink file
 * @param[in]	file	file descriptor
 * @retval		TRUE	success
 * @retval		FALSE	write failed
 */
int32_t mana_datalink_generator_write_data(mana_stream* stream)
{
	mana_datalink_generator_file_entry* entry;
	mana_datalink_file_header header;
	size_t file_size;
	size_t header_size;

	srand( (unsigned)time( NULL ) );

	header.total_data_size = mana_datalink_generator_get_number_of_files();
	header.number_of_datas = mana_datalink_generator_get_number_of_files();
	mana_stream_push_data(stream, &header, sizeof(header));
	/*
	{
		mana_parse_error("file write failed");
		return false;
	}
	*/

	file_size = 0;
	for(entry = mana_datalink_generator_file_entry_root; entry != NULL; entry = entry->next)
	{
		mana_datalink_info_header entry_header;

		entry_header.offset = file_size;
		entry_header.size = entry->file_size;

		mana_stream_push_data(stream, &entry_header, sizeof(entry_header));
		/*
		if(fwrite(&entry_header, sizeof(entry_header), 1, file) != 1)
		{
			mana_parse_error("file write failed");
			return false;
		}
		*/

		file_size += entry->file_size + entry->padding_size;
	}

	header_size = (mana_datalink_generator_get_number_of_files() * sizeof(mana_datalink_info_header)
		+ sizeof(mana_datalink_file_header)) % MANA_DATALINK_ALIGNMENT_SIZE;
	if(header_size > 0)
	{
		size_t i;

		header_size = MANA_DATALINK_ALIGNMENT_SIZE - header_size;
		for(i = 0; i < header_size; i++)
			mana_stream_push_unsigned_char(stream, (uint8_t)rand());
	}

	for(entry = mana_datalink_generator_file_entry_root; entry != NULL; entry = entry->next)
	{
		FILE* in;

#if defined(__STDC_WANT_SECURE_LIB__)
		if(fopen_s(&in, entry->file_name, "rb") != 0)
#else
		if((in = fopen(entry->file_name, "rb")) == NULL)
#endif
		{
			mana_parse_error("unable to open '%s'", entry->file_name);
			return false;
		}

		void* program = malloc(entry->file_size);
		if(program == NULL)
		{
			fclose(in);
			printf("memory allocation failed\n");
			return 1;
		}

		if(fread(program, 1, entry->file_size, in) != entry->file_size)
		{
			fclose(in);
			free(program);
			printf("file read failed: %s\n", entry->file_name);
			return 1;
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
