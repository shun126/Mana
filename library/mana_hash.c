/*
 * mana (compiler/library)
 *
 * @file	mana_hash.c
 * @brief	�n�b�V���Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana_hash�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana_hash�N���X�͕�������L�[�ɂ����A�z�z��Ɋ֐�������s�Ȃ��܂��B
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_HASH_H___)
#include "mana_hash.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <assert.h>
#include <string.h>

static void doubly_circularly_linked_list_push_front(mana_hash_table_link* self, mana_hash_table_link* list)
{
	list->previousChain = self->previousChain;
	list->nextChain = self;
	self->previousChain->nextChain = list;
	self->previousChain = list;
}
#if 0
static void doubly_circularly_linked_list_push_back(mana_hash_table_link* self, mana_hash_table_link* list)
{
	list->previousChain = self;
	list->nextChain = self->nextChain;
	self->nextChain->previousChain = list;
	self->nextChain = list;
}

static void doubly_circularly_linked_list_erase(mana_hash_table_link* self)
{
	self->previousChain->nextChain = self->nextChain;
	self->nextChain->previousChain = self->previousChain;
}
#endif

static int32_t mana_hash_get_hash_value(const char* name)
{
	unsigned h;

	for(h = 0; *name != '\0'; ++name)
	{
		unsigned g;

		h = (h << 4) + (unsigned)(*name);
		g = h & 0xf0000000;

		if(g)
		{
			h = (h ^ g >> 24) ^ g;
		}
	}

	return h % MANA_HASH_TABLE_SIZE;
}

/*!
 * @return	mana_hash �I�u�W�F�N�g
 * @warning	void mana_hash_initialize(mana_hash* self)���ĂԕK�v�͂���܂���B
 */
mana_hash* mana_hash_create(void)
{
	mana_hash* self = mana_malloc(sizeof(mana_hash));
	mana_hash_initialize(self);
	return self;
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 * @warning		void mana_hash_finalize(mana_hash* self)���ĂԕK�v�͂���܂���B
 */
void mana_hash_destroy(mana_hash* self)
{
	if(self)
	{
		mana_hash_finalize(self);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 */
void mana_hash_initialize(mana_hash* self)
{
	assert(self);
	memset(self, 0, sizeof(mana_hash));
	self->chain.previousChain = self->chain.nextChain = &self->chain;
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 */
void mana_hash_finalize(mana_hash* self)
{
	assert(self);
	mana_hash_clear(self);
}

/*!
 * @param[out]	self	��r��mana_hash �I�u�W�F�N�g
 * @param[in]	source	��r��mana_hash �I�u�W�F�N�g
 * @retval	!= 0		��v
 * @retval	== 0		�s��v
 */
int32_t mana_hash_compare(const mana_hash* self, const mana_hash* other)
{
	mana_hash_iterator source_iterator;
	mana_hash_iterator destination_iterator;

	assert(self && other);

	mana_hash_iterator_rewind(&source_iterator);
	mana_hash_iterator_rewind(&destination_iterator);
	while(mana_hash_iterator_hash_next(self, &source_iterator))
	{
		const int8_t *source_name, *destination_name;
		const void *source_data, *destination_data;

		if(!mana_hash_iterator_hash_next(other, &destination_iterator))
			return 0;

		source_name = mana_hash_iterator_get_name(&source_iterator);
		destination_name = mana_hash_iterator_get_name(&destination_iterator);
		if(strcmp(source_name, destination_name) != 0)
			return 0;

		source_data = mana_hash_iterator_get_value(&source_iterator);
		destination_data = mana_hash_iterator_get_value(&destination_iterator);
		if(source_data != destination_data)
			return 0;
	}

	return 1;
}

/*!
 * @param[out]	self	�R�s�[��mana_hash �I�u�W�F�N�g
 * @param[in]	source	�R�s�[��mana_hash �I�u�W�F�N�g
 */
void mana_hash_duplicate(mana_hash* self, const mana_hash* source)
{
	mana_hash_iterator iterator;

	assert(self && source);

	mana_hash_iterator_rewind(&iterator);
	while(mana_hash_iterator_hash_next(source, &iterator))
	{
		const char* name = mana_hash_iterator_get_name(&iterator);
		void* data = mana_hash_iterator_get_value(&iterator);

		mana_hash_set(self, name, data);
	}
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 */
void mana_hash_clear(mana_hash* self)
{
	int32_t i;

	assert(self);

	for(i = 0; i < MANA_HASH_TABLE_SIZE; i ++)
	{
		mana_hash_table* table = self->table[i];
		while(table)
		{
			mana_hash_table* next = table->next;
			mana_free(table);
			table = next;
		}
		self->table[i] = NULL;
	}
	self->chain.previousChain = self->chain.nextChain = &self->chain;
	self->last = NULL;
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 * @param[in]	name	�L�[
 */
void mana_hash_erase(mana_hash* self, const char* name)
{
	mana_hash_table* current;
	mana_hash_table** last;

	assert(self);

	last = &self->table[mana_hash_get_hash_value(name)];

	current = *last;
	while(current)
	{
		if(strcmp(current->name, name) == 0)
		{
			if(self->last == current)
				self->last = NULL;

			*last = current->next;
			break;
		}
		last = &current->next;
		current = current->next;
	}
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 * @param[in]	name	�L�[
 * @retval	!=0	�f�[�^�͑��݂���
 * @retval	==0	�f�[�^�͑��݂��Ȃ�
 */
int32_t mana_hash_test(mana_hash* self, const char* name)
{
	mana_hash_table* table;

	assert(self);

	if(self->last && strcmp(self->last->name, name) == 0)
	{
		return 1;
	}

	table = self->table[mana_hash_get_hash_value(name)];
	while(table)
	{
		if(strcmp(table->name, name) == 0)
		{
			self->last = table;
			return 1;
		}
		table = table->next;
	}

	return 0;
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 * @param[in]	name	�L�[
 * @return		�l
 */
void* mana_hash_get(mana_hash* self, const char* name)
{
	mana_hash_table* table;

	assert(self);

	/* �Ō�Ƀq�b�g�����A�C�e���Ɣ�r */
	if(self->last && strcmp(self->last->name, name) == 0)
	{
		return self->last->data;
	}

	table = self->table[mana_hash_get_hash_value(name)];
	while(table)
	{
		if(strcmp(table->name, name) == 0)
		{
			/* ����q�b�g�����A�C�e����ۑ� */
			self->last = table;
			return table->data;
		}
		table = table->next;
	}

	return NULL;
}

/*!
 * @param[in]	self	mana_hash �I�u�W�F�N�g
 * @param[in]	name	�L�[
 * @param[in]	data	�l
 */
void* mana_hash_set(mana_hash* self, const char* name, void* data)
{
	mana_hash_table* table;
	int32_t value;

	assert(self);

	value = mana_hash_get_hash_value(name);

	table = self->table[value];
	while(table)
	{
		if(strcmp(table->name, name) == 0)
		{
			return table->data;
		}
		table = table->next;
	}

	table = mana_malloc(sizeof(mana_hash_table));
	table->name = name;
	table->data = data;
	table->next = self->table[value];
	self->table[value] = table;

	table->chain.previousChain = table->chain.nextChain = &table->chain;
	doubly_circularly_linked_list_push_front(&self->chain, &table->chain);

	return table->data;
}

/*****************************************************************************/
/* iterator */
/*!
 * @param[in]	iterator	mana_hash_iterator
 */
void mana_hash_iterator_rewind(mana_hash_iterator* iterator)
{
	assert(iterator);

	iterator->iterator = NULL;
}

/*!
 * @param[in]	self		mana_hash �I�u�W�F�N�g
 * @param[in]	iterator	mana_hash_iterator
 */
int32_t mana_hash_iterator_hash_next(const mana_hash* self, mana_hash_iterator* iterator)
{
	assert(iterator);

	iterator->iterator = (mana_hash_table*)(iterator->iterator ? iterator->iterator->chain.nextChain : self->chain.nextChain);

	return (mana_hash_table_link*)iterator->iterator != (mana_hash_table_link*)&self->chain;
}

/*!
 * @param[in]	iterator	mana_hash_iterator
 * @return		�L�[
 */
const char* mana_hash_iterator_get_name(const mana_hash_iterator* iterator)
{
	assert(iterator);

	return iterator->iterator->name;
}

/*!
 * @param[in]	iterator	mana_hash_iterator
 * @return		�l
 */
void* mana_hash_iterator_get_value(const mana_hash_iterator* iterator)
{
	assert(iterator);

	return iterator->iterator->data;
}
