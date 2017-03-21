/*
 * mana (compiler/library)
 *
 * @file	mana_datalink.c
 * @brief	mana_datalinkクラスに関するソースファイル
 * @detail
 * このファイルはmana_datalinkクラスに関係するソースファイルです。
 * manaコンパイラはスクリプトでalias指定され、かつ参照されているリソースを
 * 結合します。
 * mana_datalinkクラスは結合されたデータを参照する機能を持っています。
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
 * @return	mana_datalink オブジェクト
 * @warning	void mana_datalink_initialize(mana_datalink* self)を呼ぶ必要はありません。
 */
mana_datalink* mana_datalink_create(void)
{
	mana_datalink* self = mana_malloc(sizeof(mana_datalink));
	mana_datalink_initialize(self);
	return self;
}

/*!
 * @param[in]	self	mana_datalink オブジェクト
 * @warning		void mana_datalink_finalize(mana_datalink* self)を呼ぶ必要はありません。
 */
void mana_datalink_destroy(mana_datalink* self)
{
	mana_datalink_finalize(self);
	mana_free(self);
}

/*!
 * @param[in]	self	mana_datalink オブジェクト
 */
void mana_datalink_initialize(mana_datalink* self)
{
	if(self)
	{
		memset(self, 0, sizeof(mana_datalink));
	}
}

/*!
 * @param[in]	self	mana_datalink オブジェクト
 */
void mana_datalink_finalize(mana_datalink* self)
{
	if(self)
	{
		mana_datalink_release(self);
	}
}

/*!
 * @param[in]	self	mana_datalink オブジェクト
 * @param[in]	buffer	mana_datalinkファイルデータ
 * @warning		データは参照されるだけなので、参照中は開放しないでください。
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
 * @param[in]	self	mana_datalink オブジェクト
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
 * @param[in]	self	mana_datalink オブジェクト
 * @return		datalinkファイル内のデータ数
 */
int32_t mana_datalink_get_number_of_datas(const mana_datalink* self)
{
	return self->number_of_entries;
}

/*!
 * @param[in]	self	mana_datalink オブジェクト
 * @return		データのサイズ
 */
int32_t mana_datalink_get_data_size(const mana_datalink* self, const int32_t index)
{
	return self->datalink_info_header_pointer[index].size;
}

/*!
 * @param[in]	self	mana_datalink オブジェクト
 * @return		データのアドレス
 */
const void* mana_datalink_get_data(const mana_datalink* self, const int32_t index)
{
	return self->data_pointer + self->datalink_info_header_pointer[index].offset;
}
