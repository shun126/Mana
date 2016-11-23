/*!
 * mana (library)
 *
 * @file	mana_stream.c
 * @brief	mana_streamクラスに関するソースファイル
 * @detail
 * このファイルはmana_streamクラスに関係するソースファイルです。
 * mana_streamクラスはメモリストリーミングに関する操作を行ないます。
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* メモリ確保時のページサイズ */
#define MANA_STREAM_ALLOCATE_PAGE_SIZE (0x1000)

/*!
 * @return	mana_stream オブジェクト
 */
mana_stream* mana_stream_create()
{
	return (mana_stream*)mana_calloc(1, sizeof(mana_stream));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 */
void mana_stream_destroy(mana_stream* self)
{
	if(self)
	{
		mana_free(self->buffer);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	size	新たに確保するサイズ
 */
static void mana_stream_resize_buffer(mana_stream* self, const size_t size)
{
	assert(self);

	if(self->allocated_size < self->used_size + size)
	{
		size_t new_size = (self->allocated_size + size + MANA_STREAM_ALLOCATE_PAGE_SIZE - 1) / MANA_STREAM_ALLOCATE_PAGE_SIZE * MANA_STREAM_ALLOCATE_PAGE_SIZE;

		self->buffer = mana_realloc(self->buffer, new_size);
		self->allocated_size = new_size;
	}
}

/* ファイルからデータを読み込む */
int mana_stream_load(mana_stream* self, const char* filename)
{
	FILE* infile;
	int result = 0;

	assert(self);

#if __STDC_WANT_SECURE_LIB__
	if(fopen_s(&infile, filename, "rb") == 0)
#else
	if((infile = fopen(filename, "rb")))
#endif
	{
		if(fseek(infile, 0, SEEK_END) == 0)
		{
			size_t filesize = (size_t)ftell(infile);

			mana_stream_resize_buffer(self, filesize);

			rewind(infile);

			if(fread(&self->buffer[self->used_size], 1, filesize, infile) == filesize)
			{
				self->used_size += filesize;
				result = 1;
			}
		}

		fclose(infile);
	}

	return result;
}

/* ファイルへデータを書き込む */
int mana_stream_save(mana_stream* self, const char* filename)
{
	FILE* outfile;
	int result = 0;

	assert(self);

#if __STDC_WANT_SECURE_LIB__
	if(fopen_s(&outfile, filename, "wb") == 0)
#else
	if((outfile = fopen(filename, "wb")))
#endif
	{
		result = (fwrite(self->buffer, 1, self->used_size, outfile) == self->used_size);

		fclose(outfile);
	}

	return result;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		バッファーの先頭アドレス
 */
void* mana_stream_get_buffer(const mana_stream* self)
{
	assert(self);

	return self->buffer;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	index	インデックス番号
 * @return		バッファーのアドレス
 */
void* mana_stream_index_at(const mana_stream* self, const size_t index)
{
	if(index >= mana_stream_get_used_size(self))
		return NULL;
	else
		return &self->buffer[index];
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		バッファーの確保済みサイズ
 */
size_t mana_stream_get_allocated_size(const mana_stream* self)
{
	assert(self);

	return self->allocated_size;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		バッファーの使用中サイズ
 */
size_t mana_stream_get_used_size(const mana_stream* self)
{
	assert(self);

	return self->used_size;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	value	値
 */
void mana_stream_push_char(mana_stream* self, const char value)
{
	assert(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	value	値
 */
void mana_stream_push_short(mana_stream* self, const short value)
{
	assert(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	value	値
 */
void mana_stream_push_integer(mana_stream* self, const int value)
{
	assert(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	value	値
 */
void mana_stream_push_unsigned_char(mana_stream* self, const unsigned char value)
{
	assert(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	value	値
 */
void mana_stream_push_unsigned_short(mana_stream* self, const unsigned short value)
{
	assert(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	value	値
 */
void mana_stream_push_unsigned_integer(mana_stream* self, const unsigned int value)
{
	assert(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	value	値
 */
void mana_stream_push_float(mana_stream* self, const float value)
{
	assert(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	text	文字列
 */
void mana_stream_push_string(mana_stream* self, const char* text)
{
	size_t size;

	assert(self && text);

	size = strlen(text) + 1;

	mana_stream_resize_buffer(self, size);

#if __STDC_WANT_SECURE_LIB__
	strcpy_s(&self->buffer[self->used_size], self->allocated_size - self->used_size, text);
#else
	strcpy(&self->buffer[self->used_size], text);
#endif

	self->used_size += size;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	pointer	データの先頭アドレス
 * @param[in]	size	データのサイズ
 */
void mana_stream_push_data(mana_stream* self, const void* pointer, const size_t size)
{
	assert(self);

	if(pointer)
	{
		mana_stream_resize_buffer(self, size);

		memcpy(&self->buffer[self->used_size], pointer, size);

		self->used_size += size;
	}
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 */
void mana_stream_rewind(mana_stream* self)
{
	assert(self);

	self->pointer = 0;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
char mana_stream_pop_char(mana_stream* self)
{
	char value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
short mana_stream_pop_short(mana_stream* self)
{
	short value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
int mana_stream_pop_integer(mana_stream* self)
{
	int value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
unsigned char mana_stream_pop_unsigned_char(mana_stream* self)
{
	unsigned char value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
unsigned short mana_stream_pop_unsigned_short(mana_stream* self)
{
	unsigned short value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
unsigned int mana_stream_pop_unsigned_integer(mana_stream* self)
{
	unsigned int value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
float mana_stream_pop_float(mana_stream* self)
{
	float value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[out]	pointer	ポップ先のバッファ先頭アドレス
 * @param[out]	size	ポップ先のバッファのサイズ
 * @return		文字列
 */
void mana_stream_pop_string(mana_stream* self, char* pointer, size_t size)
{
	size_t length, copy_length;

	assert(self && self->used_size >= self->pointer);

	length = copy_length = strlen(&self->buffer[self->pointer]) + 1;

	if(copy_length > size)
		copy_length = size;

	memcpy(pointer, &self->buffer[self->pointer], copy_length);

	self->pointer += length;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[out]	pointer	ポップ先のバッファ先頭アドレス
 * @param[out]	size	ポップするデータのサイズ
 * @return		文字列
 */
void mana_stream_pop_data(mana_stream* self, void* pointer, size_t size)
{
	assert(self && self->used_size >= self->pointer);
	assert(pointer);

	memcpy(pointer, &self->buffer[self->pointer], size);

	self->pointer += size;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
char mana_stream_get_char(const mana_stream* self)
{
	char value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
short mana_stream_get_short(const mana_stream* self)
{
	short value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
int mana_stream_get_integer(const mana_stream* self)
{
	int value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
unsigned char mana_stream_get_unsigned_char(const mana_stream* self)
{
	unsigned char value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
unsigned short mana_stream_get_unsigned_short(const mana_stream* self)
{
	unsigned short value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
unsigned int mana_stream_get_unsigned_integer(const mana_stream* self)
{
	unsigned int value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		値
 */
float mana_stream_get_float(const mana_stream* self)
{
	float value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[out]	pointer	ポップ先のバッファ先頭アドレス
 * @param[out]	size	ポップ先のバッファのサイズ
 * @return		文字列
 */
void mana_stream_get_string(const mana_stream* self, char* pointer, const size_t size)
{
	size_t length, copy_length;

	assert(self && self->used_size >= self->pointer);

	length = copy_length = strlen(&self->buffer[self->pointer]) + 1;

	if(copy_length > size)
		copy_length = size;

	memcpy(pointer, &self->buffer[self->pointer], copy_length);
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		文字列のアドレス
 */
const char* mana_stream_get_string_pointer(const mana_stream* self)
{
	assert(self && self->used_size >= self->pointer);

	return &self->buffer[self->pointer];
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @return		文字列の長さ
 */
size_t mana_stream_get_string_length(const mana_stream* self)
{
	assert(self && self->used_size >= self->pointer);

	return strlen(&self->buffer[self->pointer]);
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[out]	pointer	ポップ先のバッファ先頭アドレス
 * @param[out]	size	ポップするデータのサイズ
 * @return		文字列
 */
void mana_stream_get_data(const mana_stream* self, void* pointer, const size_t size)
{
	assert(self && self->used_size >= self->pointer);
	assert(pointer);

	memcpy(pointer, &self->buffer[self->pointer], size);
}

/*!
 * @param[in]	self	mana_stream オブジェクト
 * @param[in]	offset	オフセット
 */
void mana_steram_seek(mana_stream* self, const int offset)
{
	size_t pointer;

	assert(self);

	pointer = (size_t)((long)self->pointer + (long)offset);
	if(pointer >= self->used_size)
		pointer = self->used_size;
	self->pointer = pointer;
}

#define ENABLE_CHECK	1

#if !defined(NDEBUG) && ENABLE_CHECK
static unsigned short crc(const unsigned char* buffer, const size_t size)
{
	size_t i, j;
	unsigned int r = (unsigned int)(~0);

	for(i = 0; i < size; i++)
	{
		r ^= (unsigned int)buffer[i] << (16 - 8);
		for(j = 0; j < 8; j++)
		{
			if(r & 0x8000)
				r = (r << 1) ^ 0x1021;
			else
				r <<= 1;
		}
	}
	return (unsigned short)(~r);
}
#endif

void mana_stream_mark(mana_stream* self)
{
#if !defined(NDEBUG) && ENABLE_CHECK
	unsigned short CRC;

	assert(self);

	CRC = crc((unsigned char*)self->buffer, self->used_size);
	mana_stream_push_unsigned_short(self, CRC);
	mana_stream_push_unsigned_integer(self, self->used_size);
#endif
}

void mana_stream_check(mana_stream* self)
{
#if !defined(NDEBUG) && ENABLE_CHECK
	unsigned short CRC1;
	unsigned short CRC2;
	size_t pointer1;
	size_t pointer2;

	assert(self);

	CRC1 = crc((unsigned char*)self->buffer, self->pointer);
	CRC2 = mana_stream_pop_unsigned_short(self);
	assert(CRC1 == CRC2);

	pointer1 = self->pointer;
	pointer2 = mana_stream_pop_unsigned_integer(self);
	assert(pointer1 == pointer2);
#endif
}
