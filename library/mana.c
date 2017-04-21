/*!
 * mana (library)
 *
 * @file	mana.c
 * @brief	manaクラスに関するソースファイル
 * @detail
 * このファイルはmanaクラスに関係するソースファイルです。
 * manaクラスは更新やリクエストなどの制御、mana_actor オブジェクトの検索などの
 * 実行環境を表しています。
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___LIBMANA_H___)
#include "libmana.h"
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>

#if defined(_MSC_VER)
#include <windows.h>
#define alloca(size)	_alloca(size)
#else
#include <alloca.h>
#include <sched.h>
#endif

extern mana_hash mana_external_function_hash;
extern bool mana_big_endian;

static void mana_yield_thread(void)
{
#if defined(_MSC_VER)
	SwitchToThread();
#else
	sched_yield();
/*
	pthread_yield();
	pthread_yield_np();
*/
#endif
}

/*!
 * @return	mana オブジェクト
 */
mana* mana_create(void)
{
	mana* self = mana_malloc(sizeof(mana));
#if 0
	self->map_actor = NULL;
	self->player_actor = NULL;
#endif
	mana_array_initialize(&self->actor_array, 0);
	mana_hash_initialize(&self->actor_hash);
	mana_hash_initialize(&self->phantom_hash);
	mana_datalink_initialize(&self->datalink);
	self->global_memory = NULL;
	self->file_header = NULL;
	self->frame_counter = 0;
	self->instruction_pool = NULL;
	self->constant_pool = NULL;

	self->flag = MANA_FLAG_FRAME_CHANGED;

	return self;
}

/*!
 * @param[in]	self	mana オブジェクト
 */
void mana_destroy(mana* self)
{
	if(self)
	{
		mana_unload_program(self);

		mana_datalink_finalize(&self->datalink);
		mana_hash_finalize(&self->phantom_hash);
		mana_hash_finalize(&self->actor_hash);
		mana_array_finalize(&self->actor_array);

		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana オブジェクト
 * @param[out]	stream	mana_stream オブジェクト
 */
void mana_serialize(mana* self, mana_stream* stream)
{
	mana_hash_iterator iterator;

	assert(self && stream);

	{
		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor_serialize(mana_hash_iterator_get_value(&iterator), stream);
		}
/*
actor_array
 */
	}
#if 0
	mana_stream_push_string(stream, mana_get_actor_name(self, self->map_actor));
	mana_stream_push_string(stream, mana_get_actor_name(self, self->player_actor));
#endif
	mana_stream_push_integer(stream, self->frame_counter);

	mana_stream_push_unsigned_char(stream, self->flag);
}

/*!
 * @param[in]	self	mana オブジェクト
 * @param[in]	stream	mana_stream オブジェクト
 */
void mana_deserialize(mana* self, mana_stream* stream)
{
	mana_hash_iterator iterator;

	assert(self && stream);

	{
		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor_deserialize(mana_hash_iterator_get_value(&iterator), stream);
		}
	}
	/*
actor_array
	 */
#if 0
	{
		size_t size = mana_stream_get_string_length(stream) + 1;
		void* string = alloca(size);

		mana_stream_pop_string(stream, string, size);

		self->map_actor = mana_get_actor(self, string);
	}

	{
		size_t size = mana_stream_get_string_length(stream) + 1;
		void* string = alloca(size);

		mana_stream_pop_string(stream, string, size);

		self->player_actor = mana_get_actor(self, string);
	}
#endif
	self->frame_counter = mana_stream_pop_integer(stream);

	self->flag = mana_stream_pop_unsigned_char(stream);
}

/*!
 * プログラム実行中はプログラム領域を参照しているので、
 * void mana_unload_program(mana* self)よりも先に
 * プログラム領域を開放しないでください。
 * @param[in]	program			manaプログラムアドレス
 * @param[in]	auto_release	trueならば自動的にコード領域を開放します
 * @retval		true			読み込み成功
 * @retval		false			読み込み失敗
 */
bool mana_load_program(mana* self, void* program, int32_t auto_release)
{
	assert(self);
	assert(program);

	/* プログラムを開放します */
	mana_unload_program(self);

	if((intptr_t)program % MANA_DATALINK_STANDARD_ALIGNMENT_SIZE)
	{
		MANA_WARNING("The program address is NOT aligned on %lu-byte boundaries.\n", (int64_t)MANA_DATALINK_STANDARD_ALIGNMENT_SIZE);
		goto ABORT;
	}

	self->file_header = (mana_file_header*)program;
	if(!self->file_header || memcmp(MANA_SIGNATURE, self->file_header->header, sizeof(self->file_header->header)) != 0)
	{
		MANA_WARNING("abnormal mana program loaded.\n");
		goto ABORT;
	}
	if(self->file_header->major_version != MANA_MAJOR_VERSION || self->file_header->minor_version != MANA_MINOR_VERSION)
	{
		MANA_WARNING("file version error.\n");
		goto ABORT;
	}
	{
#if UINTPTR_MAX == UINT64_MAX
		const uint8_t flag = MANA_HEADER_FLAG_64BIT;
#else
		const uint8_t flag = 0;
#endif
		if ((self->file_header->flag & MANA_HEADER_FLAG_64BIT) != flag)
		{
			MANA_WARNING("different bit size by compiled.\n");
			goto ABORT;
		}
	}

	/* スタティック変数領域を確保します */
	mana_reallocate_static_variables(self->file_header->size_of_static_memory);

	/* グローバル変数領域を確保します */
	self->global_memory = (uint8_t*)mana_malloc(self->file_header->size_of_global_memory);

	mana_actor_info_header* actor_info = (mana_actor_info_header*)(self->file_header + 1);

	for(uint32_t i = 0; i < self->file_header->number_of_actors; i++)
	{
		mana_action_info_header* action_info = (mana_action_info_header*)(actor_info + 1);
		int32_t j;

		for(j = 0; j < actor_info->number_of_actions; j++)
		{
			action_info++;
		}

		actor_info = (mana_actor_info_header*)action_info;
	}

	self->constant_pool = (char*)actor_info;

	self->instruction_pool = (uint8_t*)self->constant_pool + self->file_header->size_of_constant_pool;

	if(!(self->file_header->flag & MANA_HEADER_FLAG_COMPILED))
	{
		uint8_t* constant_pool;
		uint32_t program_counter;

		constant_pool = (uint8_t*)self->constant_pool;

		program_counter = 0;
		while(program_counter < self->file_header->size_of_instruction_pool)
		{
			if (self->instruction_pool[program_counter] == MANA_IL_CALL)
			{
				const char* name = mana_get_string(self, &self->instruction_pool[program_counter + 1]);
				mana_external_funtion_type* function = mana_hash_get(&mana_external_function_hash, name);
				if (function == NULL)
				{
					MANA_WARNING("An external function called %s is not found.\n", name);
					goto ABORT;
				}
			}
			program_counter += mana_get_instruction_size(&self->instruction_pool[program_counter]);
		}

		self->file_header->flag |= MANA_HEADER_FLAG_COMPILED;
	}

	{
		int32_t counter = self->file_header->flag >> 2;
		counter++;
		self->file_header->flag &= 0x03;
		self->file_header->flag |= (counter << 2);
	}

	if(self->file_header->flag & MANA_HEADER_FLAG_RESOURCE)
	{
		uint8_t* p = self->instruction_pool + self->file_header->size_of_instruction_pool;
		p = (uint8_t*)(((intptr_t)p + (MANA_DATALINK_STANDARD_ALIGNMENT_SIZE - 1))
			/ MANA_DATALINK_STANDARD_ALIGNMENT_SIZE * MANA_DATALINK_STANDARD_ALIGNMENT_SIZE);
		mana_datalink_load(&self->datalink, (void*)p);
	}

	actor_info = (mana_actor_info_header*)(self->file_header + 1);

	for(uint32_t i = 0; i < self->file_header->number_of_actors; i++)
	{
		mana_actor* actor;
		char* actor_name = &self->constant_pool[actor_info->name];

		assert(actor_info->number_of_instances == 1);

		if(actor_info->flag & MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM)
		{
			actor = NULL;
			mana_hash_set(&self->phantom_hash, actor_name, actor_info);
		}
		else
		{
			actor = mana_actor_create(self, actor_info->variable_size);
			mana_hash_set(&self->actor_hash, actor_name, actor);
			mana_array_set(&self->actor_array, i, actor);
		}
		{
			mana_action_info_header* action_info;
			int32_t j;

			action_info = (mana_action_info_header*)(actor_info + 1);

			for(j = 0; j < actor_info->number_of_actions; j++)
			{
				if(!(actor_info->flag & MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM))
				{
					const char* acton_name = &self->constant_pool[action_info->name];
					mana_hash_set(&actor->actions, acton_name, (void*)action_info->address);
				}

				action_info++;
			}

			actor_info = (mana_actor_info_header*)action_info;
		}
	}

	/* フラグを初期化 */
	self->flag = MANA_FLAG_INIT_ACTION_RUNNING|MANA_FLAG_INITIALIZED|MANA_FLAG_SYSTEM_REQUEST;
	if(auto_release)
		self->flag |= MANA_FLAG_AUTO_RELEASE;

	/* 全アクターの initとmain アクションを実行 */
	mana_restart(self);
	mana_request_all(self, 1, "init", NULL);
	mana_request_all(self, 0, "main", NULL);

	return true;

ABORT:
	mana_unload_program(self);

	return false;
}

/*!
 * @param[in]	self	mana オブジェクト
 */
void mana_unload_program(mana* self)
{
	mana_hash_iterator iterator;

	assert(self);

	/* スクリプトで確保したリソースの開放 */
	/*
	GetResource().Clear();
	*/

	/* イベントボックスの開放 */
	/*
	DestroyIntersections();
	*/

	/* release actors */
	{
		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor_destroy(mana_hash_iterator_get_value(&iterator));
		}

		mana_hash_clear(&self->actor_hash);

		mana_array_clear(&self->actor_array);
	}
	/* release phantoms */
	{
		mana_hash_clear(&self->phantom_hash);
	}

	/* 変数の初期化 */
#if 0
	self->map_actor = NULL;
	self->player_actor = NULL;
#endif
	self->flag &= ~(MANA_FLAG_INIT_ACTION_RUNNING|MANA_FLAG_INIT_ACTION_FINISHED|MANA_FLAG_INITIALIZED|MANA_FLAG_SYSTEM_REQUEST);

	/* グローバル変数領域を解放します */
	mana_free(self->global_memory);
	self->global_memory = NULL;

	/* データの開放 */
	mana_datalink_release(&self->datalink);

	/* プログラムの開放 */
	if(self->file_header)
	{
		int32_t counter = self->file_header->flag >> 2;
		counter--;
		self->file_header->flag &= 0x03;
		self->file_header->flag |= (counter << 2);
		if(counter == 0 && (self->flag & MANA_FLAG_AUTO_RELEASE))
		{
			mana_free(self->file_header);
			self->file_header = NULL;

			self->constant_pool = NULL;
			self->instruction_pool = NULL;
		}
	}
}

/*!
 * @return	manaプログラムアドレス
 */
void* mana_get_program_buffer(mana* self)
{
	assert(self);

	return self->file_header;
}

/*!
 * @param[in]	self	mana オブジェクト
 */
void mana_restart(mana* self)
{
	mana_hash_iterator iterator;

	assert(self);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		mana_actor_restart(actor);
	}

	self->flag &= ~(MANA_FLAG_INIT_ACTION_RUNNING|MANA_FLAG_INIT_ACTION_FINISHED);
	self->frame_counter = 0;
}

/*!
 * yield命令など、次のフレーム待ちになる命令をフェッチするまで実行します。
 * @param[in]	self	mana オブジェクト
 * @param[in]	second	経過した時間(単位は秒)
 * @retval		true	スクリプト実行中
 * @retval		false	スクリプト停止
 */
bool mana_run(mana* self, const float second)
{
	mana_hash_iterator iterator;
	bool running = false;

	assert(self);

	self->flag &= ~MANA_FLAG_REQUESTED;
	self->flag |= MANA_FLAG_FRAME_CHANGED;

	mana_actor_set_delta_time(second);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		running |= mana_actor_run(actor);
	}

	self->flag &= ~MANA_FLAG_FRAME_CHANGED;
	while(self->flag & MANA_FLAG_REQUESTED)
	{
		self->flag &= ~MANA_FLAG_REQUESTED;

		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor* actor = mana_hash_iterator_get_value(&iterator);

			assert(actor);

			if(actor->flag & MANA_ACTOR_FLAG_REQUESTED)
			{
				running |= mana_actor_run(actor);
			}
		}
	}
	self->flag |= MANA_FLAG_FRAME_CHANGED;

	if((self->flag & MANA_FLAG_INIT_ACTION_RUNNING) && !running)
	{
		self->flag &= MANA_FLAG_INIT_ACTION_RUNNING;
		self->flag |= MANA_FLAG_INIT_ACTION_FINISHED;
	}

	self->frame_counter++;

	return running;
}

/*!
 * 実行終了までブロックします。
 * スクリプトによってはデッドロックが発生する事があるので注意してください。
 * @param[in]	self	mana オブジェクト
 */
void mana_execute(mana* self)
{
	while(mana_run(self, 1.0f / 60.0f))
	{
		mana_yield_thread();
	}
}

/*!
 * @param[in]	self	mana オブジェクト
 * @retval		true	スクリプト実行中
 * @retval		false	スクリプト停止中
 */
bool mana_is_running(mana* self)
{
	assert(self);

	if(self->flag & MANA_FLAG_INITIALIZED)
	{
		mana_hash_iterator iterator;

		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor* actor = mana_hash_iterator_get_value(&iterator);

			assert(actor);

			if(mana_actor_is_running(actor))
				return true;
		}
	}

	return false;
}

/*!
 * @param[in]	self		mana オブジェクト
 * @param[in]	level		優先度(高いほど優先)
 * @param[in]	action_name	アクション名
 * @param[in]	sender		リクエスト元アクター
 */
void mana_request_all(mana* self, int32_t level, const char* action_name, mana_actor* sender)
{
	mana_hash_iterator iterator;

	assert(self);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		mana_actor_request(actor, level, action_name, sender);
	}
}

/*!
 * @param[in]	self		mana オブジェクト
 * @param[in]	level		優先度(高いほど優先)
 * @param[in]	actor_name	アクター名
 * @param[in]	action_name	アクション名
 * @param[in]	sender		リクエスト元アクター
 * @retval		true		リクエスト成功
 * @retval		false		リクエスト失敗
 */
bool mana_request(mana* self, int32_t level, const char* actor_name, const char* action_name, mana_actor* sender)
{
	mana_actor* actor;

	assert(self);

	actor = mana_get_actor(self, actor_name);
	if(actor == NULL)
		return false;

	return mana_actor_request(actor, level, action_name, sender);
}

/*!
 * @param[in]	self		mana オブジェクト
 */
void mana_yield(mana* self)
{
	mana_hash_iterator iterator;

	assert(self);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		mana_actor_yield(actor);
	}
}

/*!
 * プログラム領域から１バイト分の数値を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上の１バイト分の数値
 */
int8_t mana_get_char(const mana* self, const uint8_t* address)
{
	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	return *address;
}

/*!
 * プログラム領域から２バイト分の数値を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上の２バイト分の数値
 */
int16_t mana_get_short(const mana* self, const uint8_t* address)
{
	int16_t value;
	uint8_t* pointer;

	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	pointer = (uint8_t*)&value;

	if(mana_big_endian)
	{
		pointer[0] = address[0];
		pointer[1] = address[1];
		pointer[2] = address[2];
		pointer[3] = address[3];
	}
	else
	{
		pointer[3] = address[0];
		pointer[2] = address[1];
		pointer[1] = address[2];
		pointer[0] = address[3];
	}

	return value;
}

/*!
 * プログラム領域から４バイト分の数値を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上の４バイト分の数値
 */
int32_t mana_get_integer(const mana* self, const uint8_t* address)
{
	int32_t value;
	uint8_t* pointer;

	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	pointer = (uint8_t*)&value;

	if(mana_big_endian)
	{
		pointer[0] = address[0];
		pointer[1] = address[1];
		pointer[2] = address[2];
		pointer[3] = address[3];
	}
	else
	{
		pointer[3] = address[0];
		pointer[2] = address[1];
		pointer[1] = address[2];
		pointer[0] = address[3];
	}

	return value;
}

/*!
 * プログラム領域から１バイト分の数値を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上の１バイト分の数値
 */
uint8_t mana_get_unsigned_char(const mana* self, const uint8_t* address)
{
	return (uint8_t)mana_get_char(self, address);
}

/*!
 * プログラム領域から２バイト分の数値を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上の２バイト分の数値
 */
uint16_t mana_get_unsigned_short(const mana* self, const uint8_t* address)
{
	return (uint16_t)mana_get_short(self, address);
}

/*!
 * プログラム領域から４バイト分の数値を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上の４バイト分の数値
 */
uint32_t mana_get_unsigned_integer(const mana* self, const uint8_t* address)
{
	return (uint32_t)mana_get_integer(self, address);
}

/*!
 * プログラム領域から単制度浮動小数を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上の単制度浮動小数
 */
float mana_get_float(const mana* self, const uint8_t* address)
{
	float value;
	uint8_t* pointer;

	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	pointer = (uint8_t*)&value;

	if(mana_big_endian)
	{
		pointer[0] = address[0];
		pointer[1] = address[1];
		pointer[2] = address[2];
		pointer[3] = address[3];
	}
	else
	{
		pointer[3] = address[0];
		pointer[2] = address[1];
		pointer[1] = address[2];
		pointer[0] = address[3];
	}

	return value;
}

/*!
 * 文字列領域から文字を取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		文字列へのポインタ
 */
const char* mana_get_string(const mana* self, const uint8_t* address)
{
	assert(self);

	return self->constant_pool + mana_get_integer(self, address);
}

/*!
 * プログラム領域からポインタを取得します
 * @param[in]	self	mana オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		プログラム領域上のポインタ
 */
uint8_t* mana_get_address(const mana* self, const uint8_t* address)
{
	assert(self);

	return self->instruction_pool + mana_get_integer(self, address);
}

/*!
 * @param[in]	self	mana オブジェクト
 * @param[in]	name	アクター名
 * @retval		!= NULL	アクターオブジェクトポインタ
 * @retval		== NULL	検索失敗
 */
mana_actor* mana_get_actor(mana* self, const char* name)
{
	assert(self);

	return mana_hash_get(&self->actor_hash, name);
}

/*!
 * 実行速度が遅いので注意が必要です。
 *
 * @param[in]	self	mana オブジェクト
 * @param[in]	actor	mana_actor オブジェクト
 * @retval		!= NULL	アクターの名前
 * @retval		== NULL	検索失敗
 */
const char* mana_get_actor_name(mana* self, mana_actor* actor)
{
	assert(self);

	if((self->flag & MANA_FLAG_INITIALIZED) && actor)
	{
		mana_hash_iterator iterator;

		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			if(actor == mana_hash_iterator_get_value(&iterator))
				return mana_hash_iterator_get_name(&iterator);
		}
	}

	return NULL;
}

/*!
 * @param[in]	self		mana オブジェクト
 * @param[in]	actor		コピー元になるmana_actor オブジェクト
 * @param[in]	new_name	新しいアクターの名前
 * @retval		!= NULL		コピーされたmana_actor オブジェクト
 * @retval		== NULL		コピー失敗
 */
struct mana_actor* mana_create_actor(mana* self, struct mana_actor* actor, const char* new_name)
{
	mana_actor* new_actor = NULL;

	if(actor && actor->parent == self)
	{
		if(mana_get_actor(self, new_name) == NULL)
		{
			new_actor = mana_actor_clone(actor);
			mana_hash_set(&self->actor_hash, new_name, new_actor);
			mana_array_append(&self->actor_array, new_actor);
		}
	}

	return new_actor;
}

/*!
 * @param[in]	self		mana オブジェクト
 * @param[in]	name		コピー元になるアクターの名前
 * @param[in]	new_name	新しいアクターの名前
 * @retval		!= NULL		コピーされたmana_actor オブジェクト
 * @retval		== NULL		コピー失敗
 */
struct mana_actor* mana_create_actor_name(mana* self, const char* name, const char* new_name)
{
	return mana_create_actor(self, mana_get_actor(self, name), new_name);
}

/*!
 * @param[in]	self		mana オブジェクト
 * @param[in]	name		コピー元になるファントムの名前
 * @param[in]	new_name	新しいアクターの名前
 * @retval		!= NULL		コピーされたmana_actor オブジェクト
 * @retval		== NULL		コピー失敗
 */
struct mana_actor* mana_create_actor_from_phantom(mana* self, const char* name, const char* new_name)
{
	mana_actor* actor = NULL;
	mana_actor_info_header* actor_info = mana_hash_get(&self->phantom_hash, name);
	if(actor_info && (actor_info->flag & MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM))
	{
		actor = mana_actor_create(self, actor_info->variable_size);
		mana_hash_set(&self->actor_hash, new_name, actor);
		mana_array_append(&self->actor_array, actor);

		{
			mana_action_info_header* action_info;
			int32_t j;

			action_info = (mana_action_info_header*)(actor_info + 1);

			for(j = 0; j < actor_info->number_of_actions; j++)
			{
				char* action_name = &self->constant_pool[action_info->name];
				mana_hash_set(&actor->actions, action_name, &self->instruction_pool[action_info->address]);

				action_info++;
			}
		}
	}
	return actor;
}

/*!
 * @param[in]	self	mana オブジェクト
 * @retval		true	initアクション中
 * @retval		false	initアクション終了
 */
bool mana_is_in_init_action(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_INIT_ACTION_RUNNING) ? true : false;
}

/*!
 * @param[in]	self	mana オブジェクト
 * @retval		true	initアクション終了
 * @retval		false	initアクションは実行していない
 */
bool mana_is_finish_init_action(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_INIT_ACTION_FINISHED) ? true : false;
}

/*!
 * @param[in]	self	mana オブジェクト
 * @param[in]	enable
 * - trueならばリクエスト許可
 * - falseならばリクエスト禁止
 */
void mana_enable_system_request(mana* self, const bool enable)
{
	assert(self);

	if(enable)
		self->flag |= MANA_FLAG_SYSTEM_REQUEST;
	else
		self->flag &= ~MANA_FLAG_SYSTEM_REQUEST;
}

/*!
 * @param[in]	self	mana オブジェクト
 * @retval		true	リクエスト許可
 * @retval		false	リクエスト禁止
 */
bool mana_is_system_request_enabled(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_SYSTEM_REQUEST) ? true : false;
}

/*!
 * @param[in]	self	mana オブジェクト
 * @return		フレームカウンタ
 */
uint32_t mana_get_frame_counter(mana* self)
{
	assert(self);

	return self->frame_counter;
}

/*!
 * @param[in]	self	mana オブジェクト
 * @retval		true	フレームが変更された
 * @retval		false	同じフレーム
 */
bool mana_is_frame_changed(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_FRAME_CHANGED) ? true : false;
}

/*!
 * @param[in]	self		mana オブジェクト
 * @param[in]	resouce_id	リソース番号
 * @param[out]	buffer		データアドレス
 * @param[out]	size		データサイズ
 */
void mana_get_data(const mana* self, const int32_t resouce_id, const void** buffer, size_t* size)
{
	assert(self);
	assert(resouce_id >= 0 && resouce_id < mana_datalink_get_number_of_datas(&self->datalink));

	*buffer = mana_datalink_get_data(&self->datalink, resouce_id);
	*size = mana_datalink_get_data_size(&self->datalink, resouce_id);
}
