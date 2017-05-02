/*!
 * mana (library)
 *
 * @file	libmana.c
 * @brief	システム全体に関するソースファイル
 * @detail	このファイルはライブラリ全体に関係するソースファイルです。
 * @author	Shun Moriya
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

/*! 外部関数用連装記憶配列 */
mana_hash mana_external_function_hash;

/*! スタティック変数サイズ */
size_t mana_static_memory_size = 0;

/*! スタティック変数格納エリア */
uint8_t* mana_static_memory = NULL;

/*! デバックモードフラグ */
bool mana_debug_mode = false;

/*! エンディアンフラグ */
bool mana_big_endian = false;

/*! ユニークな名前を生成するためのカウンタ */
static uint32_t mana_generate_unique_name_count = 0;

/*!
 * ライブラリの初期化を行ないます。
 * 他の関数を実行する前に呼び出してください。
 */
void mana_initialize(void)
{
	mana_big_endian = mana_is_big_endian();
#if defined(NDEBUG)
	mana_debug_mode = false;
#else
	mana_debug_mode = true;
#endif

	mana_hash_initialize(&mana_external_function_hash);
	mana_function_initialize();

	mana_plugin_initialize();
}

/*!
 * ライブラリを終了を行ないます。
 * この関数以降、他の関数を呼び出す事はできません。
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
 * @param[in]	name		外部命令名称
 * @param[in]	function	関数へのポインタ
 * @retval		true		登録成功
 * @retval		false		登録失敗
 */
bool mana_regist_function(const char* name, mana_external_funtion_type* function)
{
	return mana_hash_set(&mana_external_function_hash, name, function) != NULL;
}

/*!
 * @param[in]	name		外部命令名称
 */
void mana_unregist_function(const char* name)
{
	mana_hash_erase(&mana_external_function_hash, name);
}

/*!
 * @param[in]	enable
 * - true	デバックモード
 * - false	リリースモード
 */
void mana_set_debug_mode(const bool enable)
{
	mana_debug_mode = enable;
}

/*!
 * @retval	true	デバックモード
 * @retval	false	リリースモード
 */
bool mana_is_debug_mode(void)
{
	return mana_debug_mode;
}

/*!
 * xで表される角度をdivの範囲内に収めます
 * @param	x		角度
 * @param	div		範囲
 * @return	角度
 */
float mana_angle_mod(const float x, const float div)
{
	const float div2 = div * 2.0f;
	return x - (int32_t)(((x >= 0.0f) ? (x + div) : (x - div)) / div2) * div2;
}

/*!
 * スタティック変数領域を確保します。最初の確保は内容を０で初期化します。
 */
void mana_reallocate_static_variables(const size_t allocate_size)
{
	if(mana_static_memory == NULL)
	{
		mana_static_memory = (uint8_t*)mana_malloc(allocate_size);
		memset(mana_static_memory, 0, allocate_size);
	}
	else
	{
		mana_static_memory = (uint8_t*)mana_realloc(mana_static_memory, allocate_size);
		if(allocate_size > mana_static_memory_size)
		{
			const size_t delta = allocate_size - mana_static_memory_size;
			memset(&mana_static_memory[mana_static_memory_size], 0, delta);
		}
	}
	mana_static_memory_size = allocate_size;
}

/*!
 * @param[out]	stream	mana_stream オブジェクト
 */
void mana_serialize_static_variables(mana_stream* stream)
{
	MANA_ASSERT(stream);

	if(mana_static_memory)
		mana_stream_push_data(stream, mana_static_memory, mana_static_memory_size);
}

/*!
 * @param[in]	stream	mana_stream オブジェクト
 */
void mana_deserialize_static_variables(mana_stream* stream)
{
	MANA_ASSERT(stream);

	if(mana_static_memory)
		mana_stream_pop_data(stream, mana_static_memory, mana_static_memory_size);
}

/*!
 * @return	グローバル変数領域のアドレス
 */
uint8_t* mana_get_static_variables(void)
{
	return mana_static_memory;
}

/*!
 * @return	グローバル変数領域のサイズ
 */
size_t mana_get_static_variables_size(void)
{
	return mana_static_memory_size;
}

/*!
 * セーブ領域以外のグローバル変数領域を０で初期化します。
 * 全グローバル変数領域を初期化した場合はvoid mana_initialize_global_variables(void)関数を利用してください。
 */
void mana_clear_static_variables(void)
{
	if(mana_static_memory)
		memset(mana_static_memory, 0, mana_static_memory_size);
}

/*!
 * アクターに名付けられるユニークな名前を生成します
 * @param[out]	name	出力先バッファアドレス
 * @param[in]	size	出力先バッファサイズ
 * @see
 * struct mana_actor* mana_create_actor(mana* self, struct mana_actor* actor, char* new_name),
 * struct mana_actor* mana_create_actor_name(mana* self, char* name, char* new_name),
 * struct mana_actor* mana_create_actor_from_phantom(mana* self, char* name, char* new_name)
 */
void mana_generate_unique_name(char* name, const size_t size)
{
	if(name && size >= 8)
	{
		static const char table[0x10] = {
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
			uint8_t* s = (uint8_t*)&mana_generate_unique_name_count;

			for(; i < sizeof(mana_generate_unique_name_count) * 2; i += 2)
			{
				uint8_t value = *s++;

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
				char value = *s++;

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
