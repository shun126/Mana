/*!
 * mana (library)
 *
 * @file	mana_frame.c
 * @brief	mana_frame�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana_frame�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana_frame�N���X��mana_actor�N���X�̃��[�J���ϐ��̑�����s�Ȃ��܂��B
 * �{���̓X�^�b�N�ő��삵���ق����ǂ��̂�������܂��񂪁c
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_FRAME_H___)
#include "mana_frame.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <assert.h>
#include <string.h>

#define MANA_FRAME_PAGE_SIZE (256)

/*!
 * @return	mana_frame �I�u�W�F�N�g
 * @warning	void mana_frame_initialize(mana_frame* self)���ĂԕK�v�͂���܂���B
 */
mana_frame* mana_frame_create(void)
{
	mana_frame* self = mana_malloc(sizeof(mana_frame));
	mana_frame_initialize(self);
	return self;
}

/*!
 * @param[in]	size	�m�ۃT�C�Y
 * @return		mana_frame �I�u�W�F�N�g
 * @warning		void mana_frame_initialize_with_size(mana_frame* self, size_t size)���ĂԕK�v�͂���܂���B
 */
mana_frame* mana_frame_create_with_size(const size_t size)
{
	mana_frame* self = mana_malloc(sizeof(mana_frame));
	mana_frame_initialize_with_size(self, size);
	return self;
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @warning		void mana_frame_finalize(mana_frame* self)���ĂԕK�v�͂���܂���B
 */
void mana_frame_destroy(mana_frame* self)
{
	if(self)
	{
		mana_frame_finalize(self);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 */
void mana_frame_initialize(mana_frame* self)
{
	self->allocated_size = self->used_size = 0;
	self->buffer = NULL;
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	size	�m�ۃT�C�Y
 */
void mana_frame_initialize_with_size(mana_frame* self, const size_t size)
{
	self->allocated_size = size;
	self->used_size = 0;
	self->buffer = mana_malloc(size);
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 */
void mana_frame_finalize(mana_frame* self)
{
	self->allocated_size = self->used_size = 0;

	mana_free(self->buffer);
	self->buffer = NULL;
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[out]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_frame_serialize(const mana_frame* self, mana_stream* stream)
{
	assert(self && stream);

	mana_stream_push_integer(stream, self->used_size);
	mana_stream_push_data(stream, self->buffer, self->used_size);
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_frame_deserialize(mana_frame* self, mana_stream* stream)
{
	assert(self && stream);

	self->allocated_size = self->used_size = (size_t)mana_stream_pop_integer(stream);
	self->buffer = mana_realloc(self->buffer, self->allocated_size);

	mana_stream_pop_data(stream, self->buffer, self->allocated_size);
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 */
void mana_frame_clear(mana_frame* self)
{
	if(self)
	{
		self->used_size = 0;
	}
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	size	�ǉ��T�C�Y
 */
void mana_frame_allocate(mana_frame* self, const size_t size)
{
	if(self)
	{
		self->used_size += size;
		if(self->used_size >= self->allocated_size)
		{
			size_t new_size = (self->used_size + MANA_FRAME_PAGE_SIZE) / MANA_FRAME_PAGE_SIZE * MANA_FRAME_PAGE_SIZE;
			self->buffer = mana_realloc(self->buffer, new_size);
			memset(&self->buffer[self->allocated_size], 0, new_size - self->allocated_size);
			self->allocated_size = new_size;
		}
	}
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	size	�J���T�C�Y
 */
void mana_frame_release(mana_frame* self, const size_t size)
{
	if(self)
	{
		self->used_size -= size;
	}
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	index	�t���[���|�C���^�ւ̃I�t�Z�b�g�l
 * @return		�A�h���X
 */
void* mana_frame_get_address(const mana_frame* self, const size_t index)
{
	return self ? &self->buffer[self->used_size - index] : NULL;
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @return		�t���[���o�b�t�@�̃T�C�Y/�t���[���|�C���^
 */
size_t mana_frame_get_size(const mana_frame* self)
{
	return self ? self->used_size : 0;
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	size	�t���[���o�b�t�@�̃T�C�Y/�t���[���|�C���^
 */
void mana_frame_set_size(mana_frame* self, const size_t size)
{
	if(self)
	{
		self->used_size = size;

		assert(self->used_size <= self->allocated_size);
	}
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	other	mana_frame �I�u�W�F�N�g
 * @retval		== 0	����̓��e
 * @retval		!= 0	�قȂ���e
 */
int32_t mana_frame_compare(const mana_frame* self, const mana_frame* other)
{
	if(self == NULL)
		return 1;
	if(other == NULL)
		return 1;
	if(self->used_size != other->used_size)
		return 1;
	return memcmp(self->buffer, other->buffer, self->used_size);
}
