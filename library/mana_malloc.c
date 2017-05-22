/*!
mana (compiler/library)

@file	mana_malloc.c
@brief	manaライブラリ内で共通利用されるメモリ管理に関するソースファイル
@detail	このファイルはmanaライブラリ内で共通利用されるメモリ管理に関するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <stdio.h>
#include <stdlib.h>

/*! メモリブロックの確保に失敗した時のエラー番号 */
#define MANA_MALLOC_NO_MEMORY_ERROR_CODE (1000)

static mana_malloc_function malloc_function = NULL;
static mana_calloc_function calloc_function = NULL;
static mana_realloc_function realloc_function = NULL;
static mana_free_function free_function = NULL;

/*!
 * @param[in]	function	メモリブロックの確保関数
 */
void mana_set_malloc_function(mana_malloc_function function)
{
	malloc_function = function;
}

/*!
 * @param[in]	function	メモリブロックを確保後初期化関数
 */
void mana_set_calloc_function(mana_calloc_function function)
{
	calloc_function = function;
}

/*!
 * @param[in]	function	メモリブロックの再確保関数の登録
 */
void mana_set_realloc_function(mana_realloc_function function)
{
	realloc_function = function;
}

/*!
 * @param[in]	function	メモリブロックの開放関数の登録
 */
void mana_set_free_function(mana_free_function function)
{
	free_function = function;
}

/*!
 * @param[in]	size	確保するメモリブロックサイズ
 * @return		メモリブロックアドレス
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
 * @param[in]	size	確保するメモリブロックサイズ
 * @param[in]	count	確保するメモリブロックの数
 * @return		メモリブロックアドレス
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
 * @param[in]	buffer	変更するメモリブロックアドレス
 * @param[in]	size	確保するメモリブロックサイズ
 * @return		メモリブロックアドレス
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
 * @param[in]	buffer	開放するメモリブロックアドレス
 */
void mana_free(void* buffer)
{
	if(free_function)
		free_function(buffer);
	else
		free(buffer);
}
