/*!
 * mana (library)
 *
 * @file	libmana.c
 * @brief	�V�X�e���S�̂Ɋւ���\�[�X�t�@�C��
 * @detail	���̃t�@�C���̓��C�u�����S�̂Ɋ֌W����\�[�X�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___LIBMANA_H___)
#include "libmana.h"
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#endif

/*! �O���֐��p�A���L���z�� */
mana_hash mana_external_function_hash;

/*! �X�^�e�B�b�N�ϐ��T�C�Y */
size_t mana_static_memory_size = 0;

/*! �X�^�e�B�b�N�ϐ��i�[�G���A */
unsigned char* mana_static_memory = NULL;

/*! �f�o�b�N���[�h�t���O */
mana_bool mana_debug_mode = MANA_FALSE;

/*! �G���f�B�A���t���O */
mana_bool mana_big_endian = MANA_FALSE;

/*! ���j�[�N�Ȗ��O�𐶐����邽�߂̃J�E���^ */
static unsigned int mana_generate_unique_name_count = 0;

/*!
 * ���C�u�����̏��������s�Ȃ��܂��B
 * ���̊֐������s����O�ɌĂяo���Ă��������B
 */
void mana_initialize(void)
{
	mana_big_endian = mana_is_big_endian();
#if defined(NDEBUG)
	mana_debug_mode = MANA_FALSE;
#else
	mana_debug_mode = MANA_TRUE;
#endif

	mana_hash_initialize(&mana_external_function_hash);
	mana_function_initialize();

	mana_plugin_initialize();
}

/*!
 * ���C�u�������I�����s�Ȃ��܂��B
 * ���̊֐��ȍ~�A���̊֐����Ăяo�����͂ł��܂���B
 */
void mana_finalize(void)
{
	mana_plugin_finalize();

	mana_hash_finalize(&mana_external_function_hash);

	mana_free(mana_static_memory);
	mana_static_memory = NULL;
	mana_static_memory_size = 0;
}

/*!
 * @param[in]	name		�O�����ߖ���
 * @param[in]	function	�֐��ւ̃|�C���^
 * @retval		MANA_TRUE		�o�^����
 * @retval		MANA_FALSE		�o�^���s
 */
mana_bool mana_regist_function(const char* name, mana_external_funtion_type* function)
{
	return mana_hash_set(&mana_external_function_hash, name, function) != NULL;
}

/*!
 * @param[in]	name		�O�����ߖ���
 */
void mana_unregist_function(const char* name)
{
	mana_hash_erase(&mana_external_function_hash, name);
}

/*!
 * @param[in]	enable
 * - MANA_TRUE	�f�o�b�N���[�h
 * - MANA_FALSE	�����[�X���[�h
 */
void mana_set_debug_mode(const mana_bool enable)
{
	mana_debug_mode = enable;
}

/*!
 * @retval	MANA_TRUE	�f�o�b�N���[�h
 * @retval	MANA_FALSE	�����[�X���[�h
 */
mana_bool mana_is_debug_mode(void)
{
	return mana_debug_mode;
}

/*!
 * x�ŕ\�����p�x��div�͈͓̔��Ɏ��߂܂�
 * @param	x		�p�x
 * @param	div		�͈�
 * @return	�p�x
 */
float mana_angle_mod(const float x, const float div)
{
	float div2 = div * 2.0f;
	return x - (int)(((x >= 0.0f) ? (x + div) : (x - div)) / div2) * div2;
}

/*!
 * �X�^�e�B�b�N�ϐ��̈���m�ۂ��܂��B�ŏ��̊m�ۂ͓��e���O�ŏ��������܂��B
 */
void mana_reallocate_static_variables(const size_t allocate_size)
{
	if(mana_static_memory == NULL)
	{
		mana_static_memory = (unsigned char*)mana_malloc(allocate_size);
		memset(mana_static_memory, 0, allocate_size);
	}
	else
	{
		mana_static_memory = (unsigned char*)mana_realloc(mana_static_memory, allocate_size);
		if(allocate_size > mana_static_memory_size)
		{
			const size_t delta = allocate_size - mana_static_memory_size;
			memset(&mana_static_memory[mana_static_memory_size], 0, delta);
		}
	}
	mana_static_memory_size = allocate_size;
}

/*!
 * @param[out]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_serialize_static_variables(mana_stream* stream)
{
	MANA_ASSERT(stream);

	if(mana_static_memory)
		mana_stream_push_data(stream, mana_static_memory, mana_static_memory_size);
}

/*!
 * @param[in]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_deserialize_static_variables(mana_stream* stream)
{
	MANA_ASSERT(stream);

	if(mana_static_memory)
		mana_stream_pop_data(stream, mana_static_memory, mana_static_memory_size);
}

/*!
 * @return	�O���[�o���ϐ��̈�̃A�h���X
 */
unsigned char* mana_get_static_variables(void)
{
	return mana_static_memory;
}

/*!
 * @return	�O���[�o���ϐ��̈�̃T�C�Y
 */
size_t mana_get_static_variables_size(void)
{
	return mana_static_memory_size;
}

/*!
 * �Z�[�u�̈�ȊO�̃O���[�o���ϐ��̈���O�ŏ��������܂��B
 * �S�O���[�o���ϐ��̈�������������ꍇ��void mana_initialize_global_variables(void)�֐��𗘗p���Ă��������B
 */
void mana_clear_static_variables(void)
{
	if(mana_static_memory)
		memset(mana_static_memory, 0, mana_static_memory_size);
}

/*!
 * �A�N�^�[�ɖ��t�����郆�j�[�N�Ȗ��O�𐶐����܂�
 * @param[out]	name	�o�͐�o�b�t�@�A�h���X
 * @param[in]	size	�o�͐�o�b�t�@�T�C�Y
 * @see
 * struct mana_actor* mana_create_actor(mana* self, struct mana_actor* actor, char* new_name),
 * struct mana_actor* mana_create_actor_name(mana* self, char* name, char* new_name),
 * struct mana_actor* mana_create_actor_from_phantom(mana* self, char* name, char* new_name)
 */
void mana_generate_unique_name(char* name, const size_t size)
{
	if(name && size >= 8)
	{
		char table[0x10] = {
			'0', '1', '2', '3', '4', '5', '6', '7',
			'8', '9', 'A', 'b', 'C', 'd', 'E', 'f'
		};

		char* text = name;
		size_t i = 2;

		*text++ = '_';
		if((size_t)(text - name) >= size)
			goto EXIT;

		*text++ = '_';
		if((size_t)(text - name) >= size)
			goto EXIT;

		{
			char* s = (char*)&mana_generate_unique_name_count;

			for(; i < sizeof(mana_generate_unique_name_count) * 2; i += 2)
			{
				unsigned char value = *s++;

				if(i >= size - 1)
					break;

				*text++ = table[value >> 4];
				if((size_t)(text - name) >= size)
					goto EXIT;

				*text++ = table[value];
				if((size_t)(text - name) >= size)
					goto EXIT;
		}

			mana_generate_unique_name_count++;
		}

		{
			long now = (long)time(NULL);
			char* s = (char*)&now;

			for(; i < sizeof(long); i++)
			{
				unsigned char value = *s++;

				if(i >= size - 1)
					break;

				*text++ = table[value >> 4];
				if((size_t)(text - name) >= size)
					goto EXIT;

				*text++ = table[value];
				if((size_t)(text - name) >= size)
					goto EXIT;
			}
		}

		for(; i < MANA_UNIQUE_NAME_LENGTH - 1; i++)
		{
			name[i] = table[(rand() >> 8) & 0xF];
		}

EXIT:
		name[size - 1] = 0;
	}
	else
	{
		abort();
	}
}
