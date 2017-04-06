/*!
 * mana (library)
 *
 * @file	mana_stream.c
 * @brief	mana_stream�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana_stream�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana_stream�N���X�̓������X�g���[�~���O�Ɋւ��鑀����s�Ȃ��܂��B
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <stdio.h>
#include <string.h>

/* �������m�ێ��̃y�[�W�T�C�Y */
#define MANA_STREAM_ALLOCATE_PAGE_SIZE (0x1000)

/*!
 * @return	mana_stream �I�u�W�F�N�g
 */
mana_stream* mana_stream_create()
{
	return (mana_stream*)mana_calloc(1, sizeof(mana_stream));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
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
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	size	�V���Ɋm�ۂ���T�C�Y
 */
static void mana_stream_resize_buffer(mana_stream* self, const size_t size)
{
	MANA_ASSERT(self);

	if(self->allocated_size < self->used_size + size)
	{
		size_t new_size = (self->allocated_size + size + MANA_STREAM_ALLOCATE_PAGE_SIZE - 1) / MANA_STREAM_ALLOCATE_PAGE_SIZE * MANA_STREAM_ALLOCATE_PAGE_SIZE;

		self->buffer = mana_realloc(self->buffer, new_size);
		self->allocated_size = new_size;
	}
}

/* �t�@�C������f�[�^��ǂݍ��� */
int32_t mana_stream_load(mana_stream* self, const char* filename)
{
	FILE* infile;
	int32_t result = 0;

	MANA_ASSERT(self);

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

/* �t�@�C���փf�[�^���������� */
int32_t mana_stream_save(mana_stream* self, const char* filename)
{
	FILE* outfile;
	int32_t result = 0;

	MANA_ASSERT(self);

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
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�o�b�t�@�[�̐擪�A�h���X
 */
void* mana_stream_get_buffer(const mana_stream* self)
{
	MANA_ASSERT(self);

	return self->buffer;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	index	�C���f�b�N�X�ԍ�
 * @return		�o�b�t�@�[�̃A�h���X
 */
void* mana_stream_index_at(const mana_stream* self, const size_t index)
{
	if(index >= mana_stream_get_used_size(self))
		return NULL;
	else
		return &self->buffer[index];
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�o�b�t�@�[�̊m�ۍς݃T�C�Y
 */
size_t mana_stream_get_allocated_size(const mana_stream* self)
{
	MANA_ASSERT(self);

	return self->allocated_size;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�o�b�t�@�[�̎g�p���T�C�Y
 */
size_t mana_stream_get_used_size(const mana_stream* self)
{
	MANA_ASSERT(self);

	return self->used_size;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	value	�l
 */
void mana_stream_push_char(mana_stream* self, const int8_t value)
{
	MANA_ASSERT(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	value	�l
 */
void mana_stream_push_short(mana_stream* self, const int16_t value)
{
	MANA_ASSERT(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	value	�l
 */
void mana_stream_push_integer(mana_stream* self, const int32_t value)
{
	MANA_ASSERT(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	value	�l
 */
void mana_stream_push_unsigned_char(mana_stream* self, const uint8_t value)
{
	MANA_ASSERT(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	value	�l
 */
void mana_stream_push_unsigned_short(mana_stream* self, const uint16_t value)
{
	MANA_ASSERT(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	value	�l
 */
void mana_stream_push_unsigned_integer(mana_stream* self, const uint32_t value)
{
	MANA_ASSERT(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	value	�l
 */
void mana_stream_push_float(mana_stream* self, const float value)
{
	MANA_ASSERT(self);

	mana_stream_push_data(self, &value, sizeof(value));
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	text	������
 */
void mana_stream_push_string(mana_stream* self, const char* text)
{
	size_t size;

	MANA_ASSERT(self && text);

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
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	pointer	�f�[�^�̐擪�A�h���X
 * @param[in]	size	�f�[�^�̃T�C�Y
 */
void mana_stream_push_data(mana_stream* self, const void* pointer, const size_t size)
{
	MANA_ASSERT(self);

	if(pointer)
	{
		mana_stream_resize_buffer(self, size);

		memcpy(&self->buffer[self->used_size], pointer, size);

		self->used_size += size;
	}
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 */
void mana_stream_rewind(mana_stream* self)
{
	MANA_ASSERT(self);

	self->pointer = 0;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
int8_t mana_stream_pop_char(mana_stream* self)
{
	int8_t value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
int16_t mana_stream_pop_short(mana_stream* self)
{
	int16_t value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
int32_t mana_stream_pop_integer(mana_stream* self)
{
	int32_t value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
uint8_t mana_stream_pop_unsigned_char(mana_stream* self)
{
	uint8_t value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
uint16_t mana_stream_pop_unsigned_short(mana_stream* self)
{
	uint16_t value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
uint32_t mana_stream_pop_unsigned_integer(mana_stream* self)
{
	uint32_t value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
float mana_stream_pop_float(mana_stream* self)
{
	float value;

	mana_stream_pop_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[out]	pointer	�|�b�v��̃o�b�t�@�擪�A�h���X
 * @param[out]	size	�|�b�v��̃o�b�t�@�̃T�C�Y
 * @return		������
 */
void mana_stream_pop_string(mana_stream* self, char* pointer, const size_t size)
{
	size_t length, copy_length;

	MANA_ASSERT(self && self->used_size >= self->pointer);

	length = copy_length = strlen(&self->buffer[self->pointer]) + 1;

	if(copy_length > size)
		copy_length = size;

	memcpy(pointer, &self->buffer[self->pointer], copy_length);

	self->pointer += length;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[out]	pointer	�|�b�v��̃o�b�t�@�擪�A�h���X
 * @param[out]	size	�|�b�v����f�[�^�̃T�C�Y
 * @return		������
 */
void mana_stream_pop_data(mana_stream* self, void* pointer, const size_t size)
{
	MANA_ASSERT(self && self->used_size >= self->pointer);
	MANA_ASSERT(pointer);

	memcpy(pointer, &self->buffer[self->pointer], size);

	self->pointer += size;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
int8_t mana_stream_get_char(const mana_stream* self)
{
	int8_t value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
int16_t mana_stream_get_short(const mana_stream* self)
{
	int16_t value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
int32_t mana_stream_get_integer(const mana_stream* self)
{
	int32_t value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
uint8_t mana_stream_get_unsigned_char(const mana_stream* self)
{
	uint8_t value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
uint16_t mana_stream_get_unsigned_short(const mana_stream* self)
{
	uint16_t value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
uint32_t mana_stream_get_unsigned_integer(const mana_stream* self)
{
	uint32_t value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		�l
 */
float mana_stream_get_float(const mana_stream* self)
{
	float value;

	mana_stream_get_data(self, &value, sizeof(value));

	return value;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[out]	pointer	�|�b�v��̃o�b�t�@�擪�A�h���X
 * @param[out]	size	�|�b�v��̃o�b�t�@�̃T�C�Y
 * @return		������
 */
void mana_stream_get_string(const mana_stream* self, char* pointer, const size_t size)
{
	size_t copy_length;

	MANA_ASSERT(self && self->used_size >= self->pointer);

	copy_length = strlen(&self->buffer[self->pointer]) + 1;

	if(copy_length > size)
		copy_length = size;

	memcpy(pointer, &self->buffer[self->pointer], copy_length);
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		������̃A�h���X
 */
const char* mana_stream_get_string_pointer(const mana_stream* self)
{
	MANA_ASSERT(self && self->used_size >= self->pointer);

	return &self->buffer[self->pointer];
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @return		������̒���
 */
size_t mana_stream_get_string_length(const mana_stream* self)
{
	MANA_ASSERT(self && self->used_size >= self->pointer);

	return strlen(&self->buffer[self->pointer]);
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[out]	pointer	�|�b�v��̃o�b�t�@�擪�A�h���X
 * @param[out]	size	�|�b�v����f�[�^�̃T�C�Y
 * @return		������
 */
void mana_stream_get_data(const mana_stream* self, void* pointer, const size_t size)
{
	MANA_ASSERT(self && self->used_size >= self->pointer);
	MANA_ASSERT(pointer);

	memcpy(pointer, &self->buffer[self->pointer], size);
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	offset	�I�t�Z�b�g
 */
void mana_steram_seek(mana_stream* self, const int32_t offset)
{
	size_t pointer;

	MANA_ASSERT(self);

	pointer = (size_t)((long)self->pointer + (long)offset);
	if(pointer >= self->used_size)
		pointer = self->used_size;
	self->pointer = pointer;
}

static uint16_t crc(const uint8_t* buffer, const size_t size)
{
	size_t i, j;
	uint32_t r = (uint32_t)(~0);

	for(i = 0; i < size; i++)
	{
		r ^= (uint32_t)buffer[i] << (16 - 8);
		for(j = 0; j < 8; j++)
		{
			if(r & 0x8000)
				r = (r << 1) ^ 0x1021;
			else
				r <<= 1;
		}
	}
	return (uint16_t)(~r);
}

void mana_stream_mark(mana_stream* self)
{
	uint16_t CRC;

	MANA_ASSERT(self);

	CRC = crc((uint8_t*)self->buffer, self->used_size);
	mana_stream_push_unsigned_short(self, CRC);
	mana_stream_push_unsigned_integer(self, self->used_size);
}

void mana_stream_check(mana_stream* self)
{
	uint16_t CRC1;
	uint16_t CRC2;
	size_t pointer1;
	size_t pointer2;

	MANA_VERIFY(self);

	CRC1 = crc((uint8_t*)self->buffer, self->pointer);
	CRC2 = mana_stream_pop_unsigned_short(self);
	MANA_VERIFY(CRC1 == CRC2);

	pointer1 = self->pointer;
	pointer2 = mana_stream_pop_unsigned_integer(self);
	MANA_VERIFY(pointer1 == pointer2);
}
