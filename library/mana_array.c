/*!
 * mana (library)
 *
 * @file	mana_array.c
 * @brief	mana_array�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail	���̃t�@�C����mana_array�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_ARRAY_H___)
#include "mana_array.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <assert.h>
#include <string.h>

/*!
 * @param[in]	size	�m�ۃT�C�Y
 * @return		mana_array �I�u�W�F�N�g
 * @warning		void mana_array_initialize(mana_array* self, size_t size)���ĂԕK�v�͂���܂���B
 */
mana_array* mana_array_create(const size_t size)
{
	mana_array* self = mana_malloc(sizeof(mana_array));
	mana_array_initialize(self, size);
	return self;
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @warning		void mana_array_finalize(mana_array* self)���ĂԕK�v�͂���܂���B
 */
void mana_array_destroy(mana_array* self)
{
	if(self)
	{
		mana_array_finalize(self);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[in]	size	�m�ۃT�C�Y
 */
void mana_array_initialize(mana_array* self, const size_t size)
{
	assert(self);

	self->handle = NULL;
	self->allocated_size = 0;
	self->used_size = 0;
	mana_array_resize(self, size);
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 */
void mana_array_finalize(mana_array* self)
{
	self->allocated_size = 0;
	self->used_size = 0;

	mana_free(self->handle);
	self->handle = NULL;
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[out]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_array_serialize(const mana_array* self, mana_stream* stream)
{
	assert(self && stream);

	mana_stream_push_unsigned_integer(stream, self->allocated_size);
	mana_stream_push_unsigned_integer(stream, self->used_size);
	mana_stream_push_data(stream, self->handle, self->used_size * sizeof(void*));
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[in]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_array_deserialize(mana_array* self, mana_stream* stream)
{
	assert(self && stream);

	mana_array_finalize(self);

	self->allocated_size = (size_t)mana_stream_pop_unsigned_integer(stream);
	self->used_size = (size_t)mana_stream_pop_unsigned_integer(stream);

	self->handle = mana_malloc(self->used_size * sizeof(void*));

	mana_stream_pop_data(stream, self->handle, self->used_size * sizeof(void*));
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 */
void mana_array_clear(mana_array* self)
{
	assert(self);

	if(self->handle)
	{
		memset(self->handle, 0, self->used_size);
	}
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[in]	index	�z��ԍ�
 * @return		�|�C���^�[
 */
void* mana_array_get(const mana_array* self, const size_t index)
{
	assert(self);
	assert(index < self->used_size);
	return self->handle[index];
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[in]	index	�z��ԍ�
 * @param[in]	pointer	�|�C���^�[
 */
void mana_array_set(mana_array* self, const size_t index, const void* pointer)
{
	assert(self);

	if(self->allocated_size <= index)
	{
		const size_t allocate_size = self->allocated_size + 32;
		mana_array_resize(self, allocate_size);
	}
	if(self->used_size <= index)
	{
		self->used_size = index + 1;
	}
	self->handle[index] = (void*)pointer;
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @return		�t���[���o�b�t�@�̃T�C�Y/�t���[���|�C���^
 */
size_t mana_array_get_size(const mana_array* self)
{
	assert(self);
	return self->used_size;
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[in]	pointer	�|�C���^�[
 */
void mana_array_append(mana_array* self, const void* pointer)
{
	if(self->used_size >= self->allocated_size)
	{
		const size_t allocate_size = self->allocated_size + 32;
		mana_array_resize(self, allocate_size);
	}
	self->handle[self->used_size] = (void*)pointer;
	self->used_size++;
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[in]	size	�T�C�Y
 */
void mana_array_resize(mana_array* self, const size_t size)
{
	assert(self);

	self->handle = (void**)mana_realloc(self->handle, size * sizeof(void*));
	if(size > self->allocated_size)
	{
		const size_t delta = size - self->allocated_size;
		memset(&self->handle[self->allocated_size], 0, delta * sizeof(void*));
	}
	else if(self->used_size > size)
	{
		self->used_size = size;
	}

	self->allocated_size = size;
}

/*!
 * @param[in]	self	mana_array �I�u�W�F�N�g
 * @param[in]	other	mana_array �I�u�W�F�N�g
 * @retval		== 0	����̓��e
 * @retval		!= 0	�قȂ���e
 */
int32_t mana_array_compare(const mana_array* self, const mana_array* other)
{
	assert(self && other);

	if(self->used_size != other->used_size)
		return 1;
	return memcmp(self->handle, other->handle, self->used_size * sizeof(void*));
}
