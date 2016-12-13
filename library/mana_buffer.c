/*!
 * mana (library)
 *
 * @file	mana_buffer.c
 * @brief	mana_buffer�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana_buffer�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana_stream�N���X�͉ϒ��o�b�t�@�Ɋւ��鑀����s�Ȃ��܂��B
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_BUFFER_H___)
#include "mana_buffer.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <string.h>

/*!
 * @return	mana_buffer �I�u�W�F�N�g
 */
mana_buffer* mana_buffer_create()
{
	return (mana_buffer*)mana_calloc(1, sizeof(mana_buffer));
}

/*!
 * @param[in]	self	mana_buffer �I�u�W�F�N�g
 */
void mana_buffer_destroy(mana_buffer* self)
{
	if(self)
	{
		mana_free(self->buffer);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana_buffer �I�u�W�F�N�g
 * @return		�o�b�t�@�̐擪�A�h���X
 */
void* mana_buffer_get_buffer(mana_buffer* self)
{
	return self ? self->buffer : NULL;
}

/*!
 * @param[in]	self	mana_buffer �I�u�W�F�N�g
 * @return		�o�b�t�@�[�̊m�ۍς݃T�C�Y
 */
size_t mana_buffer_get_allocated_size(mana_buffer* self)
{
	return self ? self->allocated_size : 0;
}

/*!
 * @param[in]	self	mana_buffer �I�u�W�F�N�g
 * @return		�o�b�t�@�[�̎g�p���T�C�Y
 */
size_t mana_buffer_get_used_size(mana_buffer* self)
{
	return self ? self->used_size : 0;
}

/*!
 * @param[in]	self	mana_stream �I�u�W�F�N�g
 * @param[in]	pointer	�f�[�^�̐擪�A�h���X
 * @param[in]	size	�f�[�^�̃T�C�Y
 */
int32_t mana_buffer_push(mana_buffer* self, const void* pointer, const size_t size)
{
	if(self && pointer)
	{
		if(self->allocated_size < self->used_size + size)
		{
			size_t new_size = (self->allocated_size + size + 255) / 256 * 256;
			self->buffer = mana_realloc(self->buffer, new_size);
			if(self->buffer == NULL)
				return 0;
			self->allocated_size = new_size;
		}

#if defined(__STDC_WANT_SECURE_LIB__)
		strcpy_s(&self->buffer[self->used_size], self->allocated_size - self->used_size, pointer);
#else
		strcpy(&self->buffer[self->used_size], pointer);
#endif

		self->used_size += size;

		return 1;
	}
	else
	{
		return 0;
	}
}
