/*!
 * mana (compiler/library)
 *
 * @file	mana_malloc.c
 * @brief	mana���C�u�������ŋ��ʗ��p����郁�����Ǘ��Ɋւ���\�[�X�t�@�C��
 * @detail	���̃t�@�C����mana���C�u�������ŋ��ʗ��p����郁�����Ǘ��Ɋւ���\�[�X�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <stdio.h>
#include <stdlib.h>

/*! �������u���b�N�̊m�ۂɎ��s�������̃G���[�ԍ� */
#define MANA_MALLOC_NO_MEMORY_ERROR_CODE (1000)

static mana_malloc_function malloc_function = NULL;
static mana_calloc_function calloc_function = NULL;
static mana_realloc_function realloc_function = NULL;
static mana_free_function free_function = NULL;

/*!
 * @param[in]	function	�������u���b�N�̊m�ۊ֐�
 */
void mana_set_malloc_function(mana_malloc_function function)
{
	malloc_function = function;
}

/*!
 * @param[in]	function	�������u���b�N���m�ی㏉�����֐�
 */
void mana_set_calloc_function(mana_calloc_function function)
{
	calloc_function = function;
}

/*!
 * @param[in]	function	�������u���b�N�̍Ċm�ۊ֐��̓o�^
 */
void mana_set_realloc_function(mana_realloc_function function)
{
	realloc_function = function;
}

/*!
 * @param[in]	function	�������u���b�N�̊J���֐��̓o�^
 */
void mana_set_free_function(mana_free_function function)
{
	free_function = function;
}

/*!
 * @param[in]	size	�m�ۂ��郁�����u���b�N�T�C�Y
 * @return		�������u���b�N�A�h���X
 */
void* mana_malloc(const size_t size)
{
	void* buffer = malloc_function ? malloc_function(size) : malloc(size);
	if(buffer == NULL)
	{
		fprintf(stderr, "fatal: not enough memory error\n");
		exit(MANA_MALLOC_NO_MEMORY_ERROR_CODE);
	}
	return buffer;
}

/*!
 * @param[in]	size	�m�ۂ��郁�����u���b�N�T�C�Y
 * @param[in]	count	�m�ۂ��郁�����u���b�N�̐�
 * @return		�������u���b�N�A�h���X
 */
void* mana_calloc(const size_t size, const size_t count)
{
	void* buffer = calloc_function ? calloc_function(size, count) : calloc(size, count);
	if(buffer == NULL)
	{
		fprintf(stderr, "fatal: not enough memory error\n");
		exit(MANA_MALLOC_NO_MEMORY_ERROR_CODE);
	}
	return buffer;
}

/*!
 * @param[in]	buffer	�ύX���郁�����u���b�N�A�h���X
 * @param[in]	size	�m�ۂ��郁�����u���b�N�T�C�Y
 * @return		�������u���b�N�A�h���X
 */
void* mana_realloc(void* buffer, const size_t size)
{
	buffer = realloc_function ? realloc_function(buffer, size) : realloc(buffer, size);
	if(buffer == NULL)
	{
		fprintf(stderr, "fatal: not enough memory error\n");
/*
		exit(MANA_MALLOC_NO_MEMORY_ERROR_CODE);
*/
	}
	return buffer;
}

/*!
 * @param[in]	buffer	�J�����郁�����u���b�N�A�h���X
 */
void mana_free(void* buffer)
{
	if(free_function)
		free_function(buffer);
	else
		free(buffer);
}
