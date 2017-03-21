/*
 * mana (compiler/library)
 *
 * @file	mana_datalink.c
 * @brief	mana_datalink�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana_datalink�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana�R���p�C���̓X�N���v�g��alias�w�肳��A���Q�Ƃ���Ă��郊�\�[�X��
 * �������܂��B
 * mana_datalink�N���X�͌������ꂽ�f�[�^���Q�Ƃ���@�\�������Ă��܂��B
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_DATALINK_H___)
#include "mana_datalink.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <string.h>

/*!
 * @return	mana_datalink �I�u�W�F�N�g
 * @warning	void mana_datalink_initialize(mana_datalink* self)���ĂԕK�v�͂���܂���B
 */
mana_datalink* mana_datalink_create(void)
{
	mana_datalink* self = mana_malloc(sizeof(mana_datalink));
	mana_datalink_initialize(self);
	return self;
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 * @warning		void mana_datalink_finalize(mana_datalink* self)���ĂԕK�v�͂���܂���B
 */
void mana_datalink_destroy(mana_datalink* self)
{
	mana_datalink_finalize(self);
	mana_free(self);
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 */
void mana_datalink_initialize(mana_datalink* self)
{
	if(self)
	{
		memset(self, 0, sizeof(mana_datalink));
	}
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 */
void mana_datalink_finalize(mana_datalink* self)
{
	if(self)
	{
		mana_datalink_release(self);
	}
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 * @param[in]	buffer	mana_datalink�t�@�C���f�[�^
 * @warning		�f�[�^�͎Q�Ƃ���邾���Ȃ̂ŁA�Q�ƒ��͊J�����Ȃ��ł��������B
 */
void mana_datalink_load(mana_datalink* self, const void* buffer)
{
	mana_datalink_file_header* p = (mana_datalink_file_header*)buffer;
	int32_t offset;

	self->number_of_entries = p->number_of_datas;
	self->datalink_info_header_pointer = (mana_datalink_info_header*)&p[1];

	offset	= sizeof(mana_datalink_file_header);
	offset += sizeof(mana_datalink_info_header) * self->number_of_entries;
	offset %= MANA_DATALINK_STANDARD_ALIGNMENT_SIZE;

	if(offset > 0)
		offset = MANA_DATALINK_STANDARD_ALIGNMENT_SIZE - offset;

	self->data_pointer = (uint8_t*)(&self->datalink_info_header_pointer[self->number_of_entries]) + offset;
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 */
void mana_datalink_release(mana_datalink* self)
{
	if(self)
	{
		self->datalink_info_header_pointer = NULL;
		self->data_pointer = 0;
		self->number_of_entries = 0;
	}
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 * @return		datalink�t�@�C�����̃f�[�^��
 */
int32_t mana_datalink_get_number_of_datas(const mana_datalink* self)
{
	return self->number_of_entries;
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 * @return		�f�[�^�̃T�C�Y
 */
int32_t mana_datalink_get_data_size(const mana_datalink* self, const int32_t index)
{
	return self->datalink_info_header_pointer[index].size;
}

/*!
 * @param[in]	self	mana_datalink �I�u�W�F�N�g
 * @return		�f�[�^�̃A�h���X
 */
const void* mana_datalink_get_data(const mana_datalink* self, const int32_t index)
{
	return self->data_pointer + self->datalink_info_header_pointer[index].offset;
}
