/*!
 * mana (library)
 *
 * @file	mana_actor.c
 * @brief	mana_actorクラスに関するソースファイル
 * @detail
 * このファイルはmana_actorクラスに関係するソースファイルです。
 * mana_actorクラスは更新やリクエストなどの制御、外部関数の呼び出しなどの
 * 実行環境を表しています。
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_DEFINE_H___)
#include "mana_define.h"
#endif
#if !defined(___MANA_H___)
#include "mana.h"
#endif
#if !defined(___MANA_ACTOR_H___)
#include "mana_actor.h"
#endif
#if !defined(___MANA_HASH_H___)
#include "mana_hash.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

extern mana_hash mana_external_function_hash;
extern size_t mana_static_memory_size;
extern uint8_t* mana_static_memory;
extern bool mana_big_endian;

static mana_actor_file_callback* mana_async_file_callback = NULL;
static mana_actor_callback* mana_actor_request_callback = NULL;
static mana_actor_callback* mana_actor_rollback_callback = NULL;
static float mana_actor_delta_time = FLT_MAX;

/*!
 * @param[in]	プログラム領域から１バイト分の数値を取得します
 * @param[in]	プログラムアドレス
 * @return		プログラム領域上の１バイト分の数値
 */
static int8_t mana_actor_get_char_from_memory(const mana_actor* self, const uint32_t address)
{
	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_instruction_pool);

	return self->parent->instruction_pool[address];
}

/*!
 * @param[in]	プログラム領域から１バイト分の数値を取得します
 * @param[in]	プログラムアドレス
 * @return		プログラム領域上の１バイト分の数値
 */
static uint8_t mana_actor_get_unsigned_char_from_memory(const mana_actor* self, const uint32_t address)
{
	return (uint8_t )mana_actor_get_char_from_memory(self, address);
}

/*!
 * @param[in]	プログラム領域から２バイト分の数値を取得します
 * @param[in]	プログラムアドレス
 * @return		プログラム領域上の２バイト分の数値
 */
static uint16_t mana_actor_get_unsigned_short_from_memory(const mana_actor* self, const uint32_t address)
{
	uint8_t* pointer;
	uint16_t value;

	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_instruction_pool);

	pointer = (uint8_t*)&value;

	if(mana_big_endian)
	{
		pointer[0] = self->parent->instruction_pool[address + 0];
		pointer[1] = self->parent->instruction_pool[address + 1];
	}
	else
	{
		pointer[1] = self->parent->instruction_pool[address + 0];
		pointer[0] = self->parent->instruction_pool[address + 1];
	}

	return value;
}

/*!
 * @param[in]	プログラム領域から２バイト分の数値を取得します
 * @param[in]	プログラムアドレス
 * @return		プログラム領域上の２バイト分の数値
 */
static int16_t mana_actor_get_short_from_memory(const mana_actor* self, const uint32_t address)
{
	return (int16_t)mana_actor_get_unsigned_short_from_memory(self, address);
}

/*!
 * @param[in]	プログラム領域から４バイト分の数値を取得します
 * @param[in]	プログラムアドレス
 * @return		プログラム領域上の４バイト分の数値
 */
static uint32_t mana_actor_get_unsigned_integer_from_memory(const mana_actor* self, const uint32_t address)
{
	uint8_t* pointer;
	uint32_t value;

	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_instruction_pool);

	pointer = (uint8_t*)&value;

	if(mana_big_endian)
	{
		pointer[0] = self->parent->instruction_pool[address + 0];
		pointer[1] = self->parent->instruction_pool[address + 1];
		pointer[2] = self->parent->instruction_pool[address + 2];
		pointer[3] = self->parent->instruction_pool[address + 3];
	}
	else
	{
		pointer[3] = self->parent->instruction_pool[address + 0];
		pointer[2] = self->parent->instruction_pool[address + 1];
		pointer[1] = self->parent->instruction_pool[address + 2];
		pointer[0] = self->parent->instruction_pool[address + 3];
	}

	return value;
}

/*!
 * @param[in]	プログラム領域から４バイト分の数値を取得します
 * @param[in]	プログラムアドレス
 * @return		プログラム領域上の４バイト分の数値
 */
static int32_t mana_actor_get_integer_from_memory(const mana_actor* self, const uint32_t address)
{
	return (int32_t)mana_actor_get_unsigned_integer_from_memory(self, address);
}

/*!
 * @param[in]	プログラム領域から単制度浮動小数を取得します
 * @param[in]	プログラムアドレス
 * @return		プログラム領域上の単制度浮動小数
 */
static float mana_actor_get_float_from_memory(const mana_actor* self, const uint32_t address)
{
	uint8_t* pointer;
	float value;

	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_instruction_pool);

	pointer = (uint8_t*)&value;

	if(mana_big_endian)
	{
		pointer[0] = self->parent->instruction_pool[address + 0];
		pointer[1] = self->parent->instruction_pool[address + 1];
		pointer[2] = self->parent->instruction_pool[address + 2];
		pointer[3] = self->parent->instruction_pool[address + 3];
	}
	else
	{
		pointer[3] = self->parent->instruction_pool[address + 0];
		pointer[2] = self->parent->instruction_pool[address + 1];
		pointer[1] = self->parent->instruction_pool[address + 2];
		pointer[0] = self->parent->instruction_pool[address + 3];
	}

	return value;
}

/*!
 * プログラム領域からオフセットを取得後、文字列領域から文字を取得します
 * @param[in]	mana_actor オブジェクト
 * @param[in]	プログラムアドレス
 * @return	文字列へのポインタ
 */
static char* mana_actor_get_string_from_memory(const mana_actor* self, const uint32_t address)
{
	uint32_t offset;

	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_instruction_pool);

	offset = mana_actor_get_unsigned_integer_from_memory(self, address);

	MANA_ASSERT(offset < self->parent->file_header->size_of_constant_pool);

	return self->parent->constant_pool + offset;
}

#if 0
/*!
 * 文字列領域から文字を取得します
 * @param[in]	mana_actor オブジェクト
 * @param[in]	プログラムアドレス
 * @return	文字列へのポインタ
 */
static char* mana_actor_get_string_from_data(const mana_actor* self, const uint32_t address)
{
	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_constant_pool);

	return self->parent->constant_pool + address;
}
#endif

/*!
 * オペコードを取得
 * @param	address		プログラムアドレス
 * @return	オペコード
 */
static int32_t mana_actor_get_opecode(const mana_actor* self, const uint32_t address)
{
	int32_t opecode = self->parent->instruction_pool[address];

	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_instruction_pool);
	MANA_ASSERT(opecode >= 0 && opecode < MANA_IL_MAX_SIZE);

	return opecode;
}

static void mana_actor_cmd_restart(mana_actor* actor)
{
	mana_actor_restart(actor);
}

static void mana_actor_cmd_halt(mana_actor* actor)
{
	mana_actor_halt(actor);
}

static void mana_actor_cmd_yield(mana_actor* actor)
{
	mana_actor_yield(actor);
}

static void mana_actor_cmd_set_non_preemptive(mana_actor* actor)
{
	mana_actor_set_synchronized(actor, true);
}

static void mana_actor_cmd_set_preemptive(mana_actor* actor)
{
	mana_actor_set_synchronized(actor, false);
}

static void mana_actor_cmd_push_zero_integer(mana_actor* actor)
{
	mana_stack_push_integer(&actor->stack, 0);
}

static void mana_actor_cmd_push_zero_float(mana_actor* actor)
{
	mana_stack_push_real(&actor->stack, 0);
}

static void mana_actor_cmd_push_char(mana_actor* actor)
{
	mana_stack_push_integer(&actor->stack, mana_actor_get_char_from_memory(actor, actor->pc + 1));
}

static void mana_actor_cmd_push_short(mana_actor* actor)
{
	mana_stack_push_integer(&actor->stack, mana_actor_get_short_from_memory(actor, actor->pc + 1));
}

static void mana_actor_cmd_push_integer(mana_actor* actor)
{
	const int32_t value = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	mana_stack_push_integer(&actor->stack, value);
}

static void mana_actor_cmd_push_float(mana_actor* actor)
{
	const float value = mana_actor_get_float_from_memory(actor, actor->pc + 1);
	mana_stack_push_real(&actor->stack, value);
}

static void mana_actor_cmd_push_string(mana_actor* actor)
{
	mana_stack_push_string(&actor->stack, mana_actor_get_string_from_memory(actor, actor->pc + 1));
}

static void mana_actor_cmd_push_priority(mana_actor* actor)
{
	mana_stack_push_integer(&actor->stack, actor->interrupt_level);
}

static void mana_actor_cmd_push_self(mana_actor* actor)
{
	mana_stack_push_pointer(&actor->stack, actor);
}

static void mana_actor_cmd_push_sender(mana_actor* actor)
{
	mana_stack_push_pointer(&actor->stack, actor->interrupt[actor->interrupt_level].sender);
}

static void mana_actor_cmd_allocate(mana_actor* actor)
{
	const int32_t value = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	mana_frame_allocate(&actor->frame, value);
}

static void mana_actor_cmd_free(mana_actor* actor)
{
	const int32_t value = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	mana_frame_release(&actor->frame, value);
}

static void mana_actor_cmd_duplicate(mana_actor* actor)
{
	mana_stack_duplicate(&actor->stack);
}

static void mana_actor_cmd_remove(mana_actor* actor)
{
	mana_stack_remove(&actor->stack, 1);
}

static void mana_actor_cmd_load_static_address(mana_actor* actor)
{
	const uint32_t offset = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);
	MANA_ASSERT(offset < mana_static_memory_size);
	mana_stack_push_pointer(&actor->stack, &mana_static_memory[offset]);
}

static void mana_actor_cmd_load_global_address(mana_actor* actor)
{
	const uint32_t offset = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);
	MANA_ASSERT(offset < actor->parent->file_header->size_of_global_memory);
	mana_stack_push_pointer(&actor->stack, &actor->parent->global_memory[offset]);
}

static void mana_actor_cmd_load_frame_address(mana_actor* actor)
{
	uint32_t offset = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);
	void* address = mana_frame_get_address(&actor->frame, offset);
	mana_stack_push_pointer(&actor->stack, address);
}

static void mana_actor_cmd_load_self_address(mana_actor* actor)
{
	uint32_t offset = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);
	mana_stack_push_pointer(&actor->stack, &((char*)actor->variable)[offset]);
}

static void mana_actor_cmd_load_char(mana_actor* actor)
{
	const char* pointer = mana_stack_get_pointer(&actor->stack, 0);
	mana_stack_set_integer(&actor->stack, 0, *pointer);
}

static void mana_actor_cmd_load_short(mana_actor* actor)
{
	const int16_t* pointer = mana_stack_get_pointer(&actor->stack, 0);
	mana_stack_set_integer(&actor->stack, 0, *pointer);
}

static void mana_actor_cmd_load_integer(mana_actor* actor)
{
	const int32_t* pointer = mana_stack_get_pointer(&actor->stack, 0);
	mana_stack_set_integer(&actor->stack, 0, *pointer);
}

static void mana_actor_cmd_load_float(mana_actor* actor)
{
	const float* pointer = mana_stack_get_pointer(&actor->stack, 0);
	mana_stack_set_real(&actor->stack, 0, *pointer);
}

static void mana_actor_cmd_load_reffrence(mana_actor* actor)
{
	mana_actor_cmd_load_integer(actor);
}

static void mana_actor_cmd_store_char(mana_actor* actor)
{
	int8_t* pointer = (int8_t*)mana_stack_get_pointer(&actor->stack, 0);
	*pointer = (int8_t)mana_stack_get_integer(&actor->stack, 1);
	mana_stack_remove(&actor->stack, 2);
}

static void mana_actor_cmd_store_short(mana_actor* actor)
{
	int16_t* pointer = (int16_t*)mana_stack_get_pointer(&actor->stack, 0);
	*pointer = (int16_t)mana_stack_get_integer(&actor->stack, 1);
	mana_stack_remove(&actor->stack, 2);
}

static void mana_actor_cmd_store_integer(mana_actor* actor)
{
	int32_t* pointer = (int32_t*)mana_stack_get_pointer(&actor->stack, 0);
	*pointer = mana_stack_get_integer(&actor->stack, 1);
	mana_stack_remove(&actor->stack, 2);
}

static void mana_actor_cmd_store_float(mana_actor* actor)
{
	float* pointer = (float*)mana_stack_get_pointer(&actor->stack, 0);
	*pointer = mana_stack_get_real(&actor->stack, 1);
	mana_stack_remove(&actor->stack, 2);
}

static void mana_actor_cmd_store_reffrence(mana_actor* actor)
{
	mana_actor_cmd_store_integer(actor);
}

static void mana_actor_cmd_branch_equal(mana_actor* actor)
{
	if(!mana_stack_pop_integer(&actor->stack))
	{
		actor->pc = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);
		actor->interrupt[actor->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;

		MANA_ASSERT(actor->pc < actor->parent->file_header->size_of_instruction_pool);
	}
}

static void mana_actor_cmd_branch_not_equal(mana_actor* actor)
{
	if(mana_stack_pop_integer(&actor->stack))
	{
		actor->pc = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);
		actor->interrupt[actor->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;

		MANA_ASSERT(actor->pc < actor->parent->file_header->size_of_instruction_pool);
	}
}

static void mana_actor_cmd_branch_away(mana_actor* actor)
{
	actor->pc = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);
	actor->interrupt[actor->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;

	MANA_ASSERT(actor->pc < actor->parent->file_header->size_of_instruction_pool);
}

static void mana_actor_cmd_branch_sub_routine(mana_actor* actor)
{
	actor->interrupt[actor->interrupt_level].return_address = actor->pc + 4 + 1; /* saved return program address */
	actor->interrupt[actor->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;
	actor->pc = mana_actor_get_unsigned_integer_from_memory(actor, actor->pc + 1);

	MANA_ASSERT(actor->pc < actor->parent->file_header->size_of_instruction_pool);
}

static void mana_actor_cmd_call(mana_actor* actor)
{
	uint32_t last_pc = actor->pc;
	int32_t last_interrupt_level = actor->interrupt_level;

	/* 外部関数の実行 */
	const char* name = mana_actor_get_string_from_memory(actor, actor->pc + 1);
	mana_external_funtion_type* function = mana_hash_get(&mana_external_function_hash, name);
	if(!function)
	{
		MANA_ERROR("An external function called %s was not found.\n", name);
	}
	MANA_ASSERT(function);

	function(actor);

	/* 外部関数の終了 */
	if(mana_actor_is_running(actor))
	{
		if(!mana_actor_is_repeat(actor))
		{
			bool bHasReturnValue = mana_actor_has_return_value(actor, last_pc);
			int32_t nNumberOfArguments = mana_actor_get_argument_count_by_address(actor, last_pc);
			int32_t nSizeOfArguments   = mana_actor_get_argument_size(actor, last_pc);

			/* スタックに入っているパラメータをpopする */
			mana_stack_remove(&actor->stack, nSizeOfArguments);

			/* 戻り値があるなら、戻り値をpush */
			if(bHasReturnValue)
			{
				switch(actor->return_value.size)
				{
				case MANA_RETURN_VALUE_TYPE_INVALID:
					break;

				case MANA_RETURN_VALUE_TYPE_ACTOR:
					mana_stack_push_pointer(&actor->stack, actor->return_value.values.actor_value);
					break;

				case MANA_RETURN_VALUE_TYPE_INTEGER:
					mana_stack_push_integer(&actor->stack, actor->return_value.values.integer_value);
					break;

				case MANA_RETURN_VALUE_TYPE_FLOAT:
					mana_stack_push_real(&actor->stack, actor->return_value.values.float_value);
					break;

				case MANA_RETURN_VALUE_TYPE_STRING:
					mana_stack_push_string(&actor->stack, actor->return_value.values.string_value);
					break;

				default:
					mana_stack_push_data(&actor->stack, actor->return_value.values.pointer_value, actor->return_value.size);

					mana_free(actor->return_value.values.pointer_value);
					actor->return_value.values.pointer_value = NULL;
					break;
				}
			}

			/* 外部関数内でreq系の命令が実行された場合、スタックの状態を修正する */
			if(actor->interrupt_level > last_interrupt_level)
			{
				actor->interrupt[last_interrupt_level].address = last_pc + 4 + 2 + 2 + 1 + (nNumberOfArguments * sizeof(int16_t));
				actor->interrupt[actor->interrupt_level].stack_pointer = mana_stack_get_size(&actor->stack);
				actor->interrupt[actor->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;
			}
		}
	}
}

static void mana_actor_cmd_request(mana_actor* actor)
{
	mana_actor* target_actor = (mana_actor*)mana_stack_pop_pointer(&actor->stack);
	int32_t level = mana_stack_pop_integer(&actor->stack);
	char* action = mana_actor_get_string_from_memory(actor, actor->pc + 1);

	if(target_actor)
	{
		actor->interrupt[actor->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;
		actor->pc += sizeof(int32_t) + 1;
		mana_actor_request(target_actor, level, action, actor);
	}
}

static void mana_actor_cmd_request_wait_starting(mana_actor* actor)
{
	mana_actor* target_actor = (mana_actor*)mana_stack_get_pointer(&actor->stack, 0);
	mana_int level = mana_stack_get_integer(&actor->stack, 1);
	char* action = mana_actor_get_string_from_memory(actor, actor->pc + 1);

	if(target_actor == 0)
	{
		mana_stack_remove(&actor->stack, 2);
		return;
	}

	if(actor == target_actor)
	{
		mana_stack_remove(&actor->stack, 2);
		return;
	}

	if(mana_actor_is_init(actor) && !mana_actor_request(target_actor, level, action, actor))
	{
		mana_stack_remove(&actor->stack, 2);
		return;
	}

	if(target_actor->interrupt_level <= level)
	{
		mana_stack_remove(&actor->stack, 2);
	}
	else
	{
		mana_actor_repeat(actor, true);
	}
}

static void mana_actor_cmd_request_wait_ending(mana_actor* actor)
{
	mana_actor* target_actor = (mana_actor*)mana_stack_get_pointer(&actor->stack, 0);
	mana_int level = mana_stack_get_integer(&actor->stack, 1);
	char* action = mana_actor_get_string_from_memory(actor, actor->pc + 1);

	if(target_actor == 0)
	{
		mana_stack_remove(&actor->stack, 2);
		return;
	}

	if(actor == target_actor)
	{
		mana_stack_remove(&actor->stack, 2);
		return;
	}

	if(mana_actor_is_init(actor) && !mana_actor_request(target_actor, level, action, actor))
	{
		mana_stack_remove(&actor->stack, 2);
		return;
	}

	if(target_actor->interrupt_level < level)
	{
		mana_stack_remove(&actor->stack, 2);
	}
	else
	{
		mana_actor_repeat(actor, true);
	}
}

static void mana_actor_cmd_dynamic_request(mana_actor* actor)
{
	(void)actor;
/*
	CManaStack& Stack = actor->stack;
	const char* pszTargetActor = Stack.PopString();
	const int32_t level = Stack.PopInteger();
	const char* action = actor->GetStringFromMemory(1);

	actor->interrupt[actor->GetInterruptLevel()].repeat = true;
	actor->pc += sizeof(int32_t) + 1;

	mana_actor* target_actor = actor->GetParent().GetActor(pszTargetActor);
	if(target_actor)
		target_actor->Request(level, action, actor);
*/
}

static void mana_actor_cmd_DynamicRequestWaitStarting(mana_actor* actor)
{
	(void)actor;
/*
	CManaStack& Stack = actor->stack;
	const char* pszTargetActor = Stack.GetString(0);
	int32_t level = Stack.GetInteger(1);
	const char* action = actor->GetStringFromMemory(1);

	mana_actor* target_actor = actor->GetParent().GetActor(pszTargetActor);
	if(target_actor == 0)
	{
		actor->stack.Remove(2);
		return;
	}
	if(actor == target_actor)
	{
		actor->stack.Remove(2);
		return;
	}
	if(actor->IsInit() && !target_actor->Request(level, action, actor))
	{
		actor->stack.Remove(2);
		return;
	}
	if(target_actor->GetInterruptLevel() <= level)
	{
		actor->stack.Remove(2);
	}else{
		actor->Repeat();
	}
*/
}

static void mana_actor_cmd_dynamic_request_wait_ending(mana_actor* actor)
{
	(void)actor;
/*
	CManaStack& Stack = actor->stack;
	const char* pszTargetActor = Stack.GetString(0);
	int32_t level = Stack.GetInteger(1);
	const char* action = actor->GetStringFromMemory(1);

	mana_actor* target_actor = actor->GetParent().GetActor(pszTargetActor);
	if(target_actor == 0)
	{
		actor->stack.Remove(2);
		return;
	}
	if(actor == target_actor)
	{
		actor->stack.Remove(2);
		return;
	}
	if(actor->IsInit() && !target_actor->Request(level, action, actor))
	{
		actor->stack.Remove(2);
		return;
	}
	if(target_actor->interrupt_level < level)
	{
		actor->stack.Remove(2);
	}else{
		actor->Repeat();
	}
*/
}

static void mana_actor_cmd_join(mana_actor* actor)
{
	mana_actor* target_actor = (mana_actor*)mana_stack_get_pointer(&actor->stack, 0);
	if(target_actor == NULL)
	{
		mana_stack_remove(&actor->stack, 2);
		return;
	}

	if(target_actor->interrupt_level < mana_stack_get_integer(&actor->stack, 1))
	{
		mana_stack_remove(&actor->stack, 2);
	}
	else
	{
		mana_actor_repeat(actor, true);
	}
}

static void mana_actor_cmd_comply(mana_actor* actor)
{
	mana_actor_comply(actor);
}

static void mana_actor_cmd_refuse(mana_actor* actor)
{
	mana_actor_refuse(actor);
}

static void mana_actor_cmd_load_return_address(mana_actor* actor)
{
	uint32_t* pointer = (uint32_t*)mana_frame_get_address(&actor->frame, sizeof(int32_t));
	actor->interrupt[actor->interrupt_level].return_address = *pointer;
}

static void mana_actor_cmd_store_return_address(mana_actor* actor)
{
	uint32_t* pointer = (uint32_t*)mana_frame_get_address(&actor->frame, sizeof(int32_t));
	*pointer = actor->interrupt[actor->interrupt_level].return_address;
}

static void mana_actor_cmd_return_from_function(mana_actor* actor)
{
	actor->pc = actor->interrupt[actor->interrupt_level].return_address;
	actor->interrupt[actor->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;
}

static void mana_actor_cmd_return_from_action(mana_actor* actor)
{
	mana_actor_rollback(actor, actor->interrupt_level);
}

static void mana_actor_cmd_rollback(mana_actor* actor)
{
	mana_actor_rollback(actor, mana_stack_pop_integer(&actor->stack));
}

static void mana_actor_cmd_add_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left + right);
}

static void mana_actor_cmd_add_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_real(&actor->stack, 0, left + right);
}

static void mana_actor_cmd_divide_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left / right);
}

static void mana_actor_cmd_divide_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_real(&actor->stack, 0, left / right);
}

static void mana_actor_cmd_minus_integer(mana_actor* actor)
{
	mana_stack_set_integer(&actor->stack, 0, -mana_stack_get_integer(&actor->stack, 0));
}

static void mana_actor_cmd_minus_float(mana_actor* actor)
{
	mana_stack_set_real(&actor->stack, 0, -mana_stack_get_real(&actor->stack, 0));
}

static void mana_actor_cmd_MOD_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left % right);
}

static void mana_actor_cmd_MOD_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_real(&actor->stack, 0, fmodf(left,  right));
}

static void mana_actor_cmd_multiply_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left * right);
}

static void mana_actor_cmd_multiply_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_real(&actor->stack, 0, left * right);
}

static void mana_actor_cmd_subtract_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left - right);
}

static void mana_actor_cmd_subtract_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_real(&actor->stack, 0, left - right);
}

static void mana_actor_cmd_and(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left & right);
}

static void mana_actor_cmd_exclusive_or(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left ^ right);
}

static void mana_actor_cmd_logical_and(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left && right);
}

static void mana_actor_cmd_logical_or(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left || right);
}

static void mana_actor_cmd_not(mana_actor* actor)
{
	mana_stack_set_integer(&actor->stack, 0, ~mana_stack_get_integer(&actor->stack, 0));
}

static void mana_actor_cmd_logical_not(mana_actor* actor)
{
	mana_stack_set_integer(&actor->stack, 0, !mana_stack_get_integer(&actor->stack, 0));
}

static void mana_actor_cmd_or(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left | right);
}

static void mana_actor_cmd_shift_left(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left << right);
}

static void mana_actor_cmd_shift_right(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left >> right);
}

static void mana_actor_cmd_compare_equal_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left == right);
}

static void mana_actor_cmd_compare_equal_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left == right);
}

static void mana_actor_cmd_compare_greater_equal_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left >= right);
}

static void mana_actor_cmd_compare_greater_equal_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left >= right);
}

static void mana_actor_cmd_compare_greater_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left > right);
}

static void mana_actor_cmd_compare_greater_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left > right);
}

static void mana_actor_cmd_compare_not_equal_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left != right);
}

static void mana_actor_cmd_compare_not_equal_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left != right);
}

static void mana_actor_cmd_compare_less_equal_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left <= right);
}

static void mana_actor_cmd_compare_less_equal_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left <= right);
}

static void mana_actor_cmd_compare_less_integer(mana_actor* actor)
{
	mana_int left  = mana_stack_get_integer(&actor->stack, 1);
	mana_int right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left < right);
}

static void mana_actor_cmd_compare_less_float(mana_actor* actor)
{
	float left	= mana_stack_get_real(&actor->stack, 1);
	float right = mana_stack_get_real(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left < right);
}

static void mana_actor_cmd_integer_to_float(mana_actor* actor)
{
	mana_stack_set_real(&actor->stack, 0, (float)mana_stack_get_integer(&actor->stack, 0));
}

static void mana_actor_cmd_float_to_integer(mana_actor* actor)
{
	mana_stack_set_integer(&actor->stack, 0, (int32_t)mana_stack_get_real(&actor->stack, 0));
}

static void mana_actor_cmd_push_actor(mana_actor* actor)
{
	mana_actor* target_actor;
	char* pszActorName;

	pszActorName = mana_actor_get_string_from_memory(actor, actor->pc + 1);
	MANA_ASSERT(pszActorName);

	target_actor = mana_get_actor(actor->parent, pszActorName);
	MANA_ASSERT(target_actor);

	mana_stack_push_pointer(&actor->stack, target_actor);
}

static void mana_actor_cmd_load_data(mana_actor* actor)
{
	const int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	const void* pointer = mana_stack_pop_pointer(&actor->stack);
	mana_stack_push_data(&actor->stack, pointer, size);
}

static void mana_actor_cmd_store_data(mana_actor* actor)
{
	const int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	void* pointer = (void*)mana_stack_pop_pointer(&actor->stack);
	mana_stack_pop_data(&actor->stack, pointer, size);
}

static void mana_actor_cmd_duplicate_data(mana_actor* actor)
{
	int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	void* pointer = (void*)mana_stack_get_address(&actor->stack, size / sizeof(void*));
	mana_stack_push_data(&actor->stack, pointer, size);
}

static void mana_actor_cmd_remove_data(mana_actor* actor)
{
	mana_stack_remove(&actor->stack, mana_actor_get_integer_from_memory(actor, actor->pc + 1) / sizeof(void*));
}

static void mana_actor_cmd_compare_equal_data(mana_actor* actor)
{
	int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1 + sizeof(int32_t));
	const void* buf1 = mana_stack_get_address(&actor->stack, size * 1 / sizeof(void*));
	const void* buf2 = mana_stack_get_address(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_remove(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_push_integer(&actor->stack, memcmp(buf1, buf2, size) == 0);
}

static void mana_actor_cmd_compare_greater_equal_data(mana_actor* actor)
{
	int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1 + sizeof(int32_t));
	const void* buf1 = mana_stack_get_address(&actor->stack, size * 1 / sizeof(void*));
	const void* buf2 = mana_stack_get_address(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_remove(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_push_integer(&actor->stack, memcmp(buf1, buf2, size) >= 0);
}

static void mana_actor_cmd_compare_greater_data(mana_actor* actor)
{
	int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1 + sizeof(int32_t));
	const void* buf1 = mana_stack_get_address(&actor->stack, size * 1 / sizeof(void*));
	const void* buf2 = mana_stack_get_address(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_remove(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_push_integer(&actor->stack, memcmp(buf1, buf2, size) > 0);
}

static void mana_actor_cmd_compare_not_equal_data(mana_actor* actor)
{
	int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1 + sizeof(int32_t));
	const void* buf1 = mana_stack_get_address(&actor->stack, size * 1 / sizeof(void*));
	const void* buf2 = mana_stack_get_address(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_remove(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_push_integer(&actor->stack, memcmp(buf1, buf2, size) != 0);
}

static void mana_actor_cmd_compare_less_equal_data(mana_actor* actor)
{
	int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1 + sizeof(int32_t));
	const void* buf1 = mana_stack_get_address(&actor->stack, size * 1 / sizeof(void*));
	const void* buf2 = mana_stack_get_address(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_remove(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_push_integer(&actor->stack, memcmp(buf1, buf2, size) <= 0);
}

static void mana_actor_cmd_compare_less_data(mana_actor* actor)
{
	int32_t size = mana_actor_get_integer_from_memory(actor, actor->pc + 1 + sizeof(int32_t));
	const void* buf1 = mana_stack_get_address(&actor->stack, size * 1 / sizeof(void*));
	const void* buf2 = mana_stack_get_address(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_remove(&actor->stack, size * 2 / sizeof(void*));
	mana_stack_push_integer(&actor->stack, memcmp(buf1, buf2, size) < 0);
}

static void mana_actor_cmd_print(mana_actor* actor)
{
	int32_t number_of_arguments;
	int32_t message_pointer;
	char message[1024];

	message_pointer = 0;
	number_of_arguments = mana_actor_get_integer_from_memory(actor, actor->pc + 1);

	if(number_of_arguments > 0)
	{
		int32_t counter = 1;
		const char* format;

		for(format = mana_stack_get_pointer(&actor->stack, 0); *format; format++)
		{
			if(*format == '%')
			{
				if(counter > number_of_arguments)
				{
					message[message_pointer++] = '\n';
					break;
				}

				format++;

				switch(*format)
				{
				case 'd':
#if __STDC_WANT_SECURE_LIB__
					message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, "%d", mana_stack_get_integer(&actor->stack, counter++));
#else
					message_pointer += sprintf(&message[message_pointer], "%lld", mana_stack_get_integer(&actor->stack, counter++));
#endif
					break;

				case 'f':
#if __STDC_WANT_SECURE_LIB__
					message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, "%f", mana_stack_get_real(&actor->stack, counter++));
#else
					message_pointer += sprintf(&message[message_pointer], "%f", mana_stack_get_real(&actor->stack, counter++));
#endif
					break;

				case 's':
#if __STDC_WANT_SECURE_LIB__
					message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, mana_stack_get_string(&actor->stack, counter++));
#else
					message_pointer += sprintf(&message[message_pointer], "%s", mana_stack_get_string(&actor->stack, counter++));
#endif
					break;

				case 'p':
#if __STDC_WANT_SECURE_LIB__
					message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, "%p", mana_stack_get_pointer(&actor->stack, counter++));
#else
					message_pointer += sprintf(&message[message_pointer], "%p", mana_stack_get_pointer(&actor->stack, counter++));
#endif
					break;

				default:
					message[message_pointer] = *format;
					message_pointer++;
					break;
				}
			}
			else
			{
				message[message_pointer] = *format;
				message_pointer++;
			}
		}
	}
	message[message_pointer] = '\0';

#if defined(_WIN32) || defined(_WIN64)
	OutputDebugStringA(message);
#else
	fprintf(stdout, "%s", message);
#endif

	mana_stack_remove(&actor->stack, number_of_arguments);
}

/*! 内部命令ジャンプテーブル */
static void (*m_afnIntermediateLanguage[MANA_IL_MAX_SIZE])(mana_actor*) = {
	/* thread */
	&mana_actor_cmd_restart,						/* 00 */
	&mana_actor_cmd_halt,							/* 01 */
	&mana_actor_cmd_yield,							/* 02 */
	&mana_actor_cmd_set_non_preemptive,				/* 03 */
	&mana_actor_cmd_set_preemptive,					/* 04 */

	/* jump */
	&mana_actor_cmd_branch_equal,					/* 05 */
	&mana_actor_cmd_branch_not_equal,				/* 06 */
	&mana_actor_cmd_branch_away,					/* 07 */
	&mana_actor_cmd_branch_sub_routine,				/* 08 */
	&mana_actor_cmd_call,							/* 09 */
	&mana_actor_cmd_request,						/* 0A */
	&mana_actor_cmd_request_wait_starting,			/* 0B */
	&mana_actor_cmd_request_wait_ending,			/* 0C */
	&mana_actor_cmd_join,							/* 0D */
	&mana_actor_cmd_comply,							/* 0E */
	&mana_actor_cmd_refuse,							/* 0F */
	&mana_actor_cmd_load_return_address,			/* 10 */
	&mana_actor_cmd_store_return_address,			/* 11 */
	&mana_actor_cmd_return_from_function,			/* 12 */
	&mana_actor_cmd_return_from_action,				/* 13 */
	&mana_actor_cmd_rollback,						/* 14 */

	/* constant */
	&mana_actor_cmd_push_zero_integer,				/* 15 */
	&mana_actor_cmd_push_zero_float,				/* 16 */
	&mana_actor_cmd_push_char,						/* 17 */
	&mana_actor_cmd_push_short,						/* 18 */
	&mana_actor_cmd_push_integer,					/* 19 */
	&mana_actor_cmd_push_float,						/* 1A */
	&mana_actor_cmd_push_string,					/* 1B */
	&mana_actor_cmd_push_priority,					/* 1C */
	&mana_actor_cmd_push_actor,						/* 1D */
	&mana_actor_cmd_push_self,						/* 1E */
	&mana_actor_cmd_push_sender,					/* 1F */

	/* stack */
	&mana_actor_cmd_allocate,						/* 20 */
	&mana_actor_cmd_free,							/* 21 */
	&mana_actor_cmd_duplicate,						/* 22 */
	&mana_actor_cmd_duplicate_data,					/* 23 */
	&mana_actor_cmd_remove,							/* 24 */
	&mana_actor_cmd_remove_data,					/* 25 */
	&mana_actor_cmd_load_static_address,			/* 26 */
	&mana_actor_cmd_load_global_address,			/* 27 */
	&mana_actor_cmd_load_frame_address,				/* 28 */
	&mana_actor_cmd_load_self_address,				/* 29 */

	/* memory operation */
	&mana_actor_cmd_load_char,						/* 2A */
	&mana_actor_cmd_load_short,						/* 2B */
	&mana_actor_cmd_load_integer,					/* 2C */
	&mana_actor_cmd_load_float,						/* 2D */
	&mana_actor_cmd_load_reffrence,					/* 2E */
	&mana_actor_cmd_load_data,						/* 2F */
	&mana_actor_cmd_store_char,						/* 30 */
	&mana_actor_cmd_store_short,					/* 31 */
	&mana_actor_cmd_store_integer,					/* 32 */
	&mana_actor_cmd_store_float,					/* 33 */
	&mana_actor_cmd_store_reffrence,				/* 34 */
	&mana_actor_cmd_store_data,						/* 35 */

	/* caluclation */
	&mana_actor_cmd_add_integer,					/* 36 */
	&mana_actor_cmd_divide_integer,					/* 37 */
	&mana_actor_cmd_minus_integer,					/* 38 */
	&mana_actor_cmd_MOD_integer,					/* 39 */
	&mana_actor_cmd_multiply_integer,				/* 3A */
	&mana_actor_cmd_subtract_integer,				/* 3B */

	&mana_actor_cmd_add_float,						/* 3C */
	&mana_actor_cmd_divide_float,					/* 3D */
	&mana_actor_cmd_minus_float,					/* 3E */
	&mana_actor_cmd_MOD_float,						/* 3F */
	&mana_actor_cmd_multiply_float,					/* 40 */
	&mana_actor_cmd_subtract_float,					/* 41 */

	&mana_actor_cmd_and,							/* 42 */
	&mana_actor_cmd_exclusive_or,					/* 43 */
	&mana_actor_cmd_logical_and,					/* 44 && */
	&mana_actor_cmd_logical_or,						/* 45 || */
	&mana_actor_cmd_logical_not,					/* 46 ! */
	&mana_actor_cmd_not,							/* 47 ~ */
	&mana_actor_cmd_or,								/* 48 */
	&mana_actor_cmd_shift_left,						/* 49 */
	&mana_actor_cmd_shift_right,					/* 4A */

	&mana_actor_cmd_integer_to_float,				/* 4B */
	&mana_actor_cmd_float_to_integer,				/* 4C */

	/* compare and branch */
	&mana_actor_cmd_compare_equal_integer,			/* 4D */
	&mana_actor_cmd_compare_not_equal_integer,		/* 4E */
	&mana_actor_cmd_compare_greater_equal_integer,	/* 4F */
	&mana_actor_cmd_compare_greater_integer,		/* 50 */
	&mana_actor_cmd_compare_less_equal_integer,		/* 51 */
	&mana_actor_cmd_compare_less_integer,			/* 52 */

	&mana_actor_cmd_compare_equal_float,			/* 53 */
	&mana_actor_cmd_compare_not_equal_float,		/* 54 */
	&mana_actor_cmd_compare_greater_equal_float,	/* 55 */
	&mana_actor_cmd_compare_greater_float,			/* 56 */
	&mana_actor_cmd_compare_less_equal_float,		/* 57 */
	&mana_actor_cmd_compare_less_float,				/* 58 */

	&mana_actor_cmd_compare_equal_data,				/* 59 */
	&mana_actor_cmd_compare_not_equal_data,			/* 5A */
	&mana_actor_cmd_compare_greater_equal_data,		/* 5B */
	&mana_actor_cmd_compare_greater_data,			/* 5C */
	&mana_actor_cmd_compare_less_equal_data,		/* 5D */
	&mana_actor_cmd_compare_less_data,				/* 5E */

	/* support */
	&mana_actor_cmd_print,							/* 5F */

	&mana_actor_cmd_dynamic_request,				/* 60 */
	&mana_actor_cmd_DynamicRequestWaitStarting,		/* 61 */
	&mana_actor_cmd_dynamic_request_wait_ending,	/* 62 */

#if 0
	&mana_actor_cmd_dynamic_request_over_network,	/* 63 */
#endif
};

static void mana_actor_initialize(mana_actor* self)
{
	MANA_ASSERT(self);

	memset(self, 0, sizeof(mana_actor));
	mana_actor_restart(self);

	mana_frame_initialize(&self->frame);
	mana_stack_initialize(&self->stack);
	mana_hash_initialize(&self->actions);
}

/*!
 * @param[in]	parent			mana オブジェクト
 * @param[in]	variable_size	アクター変数のサイズ
 * @return		mana_actor オブジェクト
 */
mana_actor* mana_actor_create(mana* parent, const size_t variable_size)
{
	mana_actor* self = mana_malloc(sizeof(mana_actor));

	mana_actor_initialize(self);

	self->parent = parent;
	self->variable_size = variable_size;

	if(self->variable_size > 0)
	{
		self->variable = mana_malloc(self->variable_size);
		memset(self->variable, 0, self->variable_size);
	}

	return self;
}

/*!
 * @param[in]	actor	コピー元のmana_actor オブジェクト
 * @return		mana_actor オブジェクト
 */
mana_actor* mana_actor_clone(mana_actor* actor)
{
	mana_actor* self = mana_malloc(sizeof(mana_actor));

	MANA_ASSERT(actor);

	mana_actor_initialize(self);

	self->parent = actor->parent;
	self->variable_size = actor->variable_size;

	if(self->variable_size > 0)
	{
		self->variable = mana_malloc(self->variable_size);
		memset(self->variable, 0, self->variable_size);
	}

	mana_hash_duplicate(&self->actions, &actor->actions);

	return self;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_destroy(mana_actor* self)
{
	if(self)
	{
		int32_t i;

		/* 読み込み中にdeleteされた？ */
		for(i = 0 ; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			mana_interrupt* interrupt = &self->interrupt[i];

			/* ファイルエントリの削除 */
			if(mana_async_file_callback && interrupt->file_callback_parameter)
			{
				mana_async_file_callback(MANA_FILE_COMMAND_CLOSE, interrupt->file_callback_parameter);
				interrupt->file_callback_parameter = NULL;
			}
		}

		mana_frame_finalize(&self->frame);
		mana_stack_finalize(&self->stack);
		mana_hash_finalize(&self->actions);

		mana_free(self->variable);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @param[out]	stream	mana_stream オブジェクト
 */
void mana_actor_serialize(mana_actor* self, mana_stream* stream)
{
	int32_t i;

	MANA_ASSERT(self && stream);

	mana_frame_serialize(&self->frame, stream);
	mana_stack_serialize(&self->stack, stream);

	mana_stream_mark(stream);

	for(i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
	{
		mana_interrupt* p = &self->interrupt[i];

		const char* sender_name = mana_get_actor_name(self->parent, p->sender);
		if(sender_name)
		{
			mana_stream_push_char(stream, 1);
			mana_stream_push_string(stream, sender_name);
		}else{
			mana_stream_push_char(stream, 0);
		}

		mana_stream_push_integer(stream, p->address);
		mana_stream_push_integer(stream, p->return_address);
		mana_stream_push_integer(stream, p->counter);
		mana_stream_push_integer(stream, p->frame_pointer);
		mana_stream_push_integer(stream, p->stack_pointer);
		mana_stream_push_unsigned_char(stream, p->flag);

		mana_stream_mark(stream);
	}

	mana_stream_push_size(stream, self->variable_size);
	mana_stream_push_data(stream, self->variable, self->variable_size);
	mana_stream_push_integer(stream, self->pc);

	mana_stream_push_char(stream, self->interrupt_level);
	mana_stream_push_unsigned_char(stream, self->flag);

	mana_stream_mark(stream);
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	stream	mana_stream オブジェクト
 */
void mana_actor_deserialize(mana_actor* self, mana_stream* stream)
{
	int32_t i;

	MANA_ASSERT(self && stream);

	mana_frame_deserialize(&self->frame, stream);
	mana_stack_deserialize(&self->stack, stream);

	mana_stream_check(stream);

	for(i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
	{
		mana_interrupt* p = &self->interrupt[i];

		if(mana_stream_pop_char(stream))
		{
			p->sender = mana_get_actor(self->parent, mana_stream_get_string_pointer(stream));
			mana_steram_seek(stream, mana_stream_get_string_length(stream));
		}

		p->address = mana_stream_pop_integer(stream);
		p->return_address = mana_stream_pop_integer(stream);
		p->counter = mana_stream_pop_integer(stream);
		p->frame_pointer = mana_stream_pop_integer(stream);
		p->stack_pointer = mana_stream_pop_integer(stream);
		p->flag = mana_stream_pop_unsigned_char(stream);

		mana_stream_check(stream);
	}

	self->variable_size = mana_stream_pop_size(stream);
	self->variable = mana_malloc(self->variable_size);
	mana_stream_pop_data(stream, self->variable, self->variable_size);

	self->pc = mana_stream_pop_integer(stream);

	self->interrupt_level = mana_stream_pop_char(stream);
	self->flag = mana_stream_pop_unsigned_char(stream);

	mana_stream_check(stream);
}

/*!
 * 外部関数内から呼ばれる事を想定しています。
 * bool mana_run(mana* self, float second)のsecondの値が取得できるので、
 * 各外部関数で経過した時間が必要な時に使ってください。
 *
 * @return		経過時間
 */
float mana_actor_get_delta_time(void)
{
	return mana_actor_delta_time;
}

/*!
 * bool mana_run(mana* self, float second)のsecondを設定しています。
 * @param[in]	second	経過時間(秒)
 */
void mana_actor_set_delta_time(float second)
{
	mana_actor_delta_time = second;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @retval		true	実行中
 * @retval		false	停止
 */
int32_t mana_actor_run(mana_actor* self)
{
	int32_t timer = 0;

	MANA_ASSERT(self && self->parent);

	do{
		int32_t opecode;

		self->flag &= ~MANA_ACTOR_FLAG_REQUESTED;

		if(!mana_actor_is_running(self))
			return false;

		MANA_ASSERT(self->pc != ~0);

		opecode = mana_actor_get_opecode(self, self->pc);
		(*m_afnIntermediateLanguage[opecode])(self);

		if(!mana_actor_is_running(self))
			return false;

		if(mana_actor_is_repeat(self))
		{
			self->interrupt[self->interrupt_level].flag &= ~MANA_INTERRUPT_FLAG_REPEAT;
		}
		else
		{
			self->pc += mana_get_instruction_size(self->parent->instruction_pool + self->pc);
			self->interrupt[self->interrupt_level].flag &= ~MANA_INTERRUPT_FLAG_INITIALIZED;
		}

		if((self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_SUSPEND) && !mana_actor_is_synchronized(self))
		{
			self->interrupt[self->interrupt_level].flag &= ~MANA_INTERRUPT_FLAG_SUSPEND;
			break;
		}
	}while(++timer < 500 || mana_actor_is_synchronized(self));

	return (self->flag & MANA_ACTOR_FLAG_RUNNING) ? true : false;
}

/*!
 * アクション終了まで実行します。
 * bool mana_async_call(mana_actor* self, int32_t level, char* action, mana_actor* sender)
 * と違い他のアクターも並列動作します。
 * アクションが終了しないとdead lockするので注意してください。
 *
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	level	優先度(高いほど優先)
 * @param[in]	action	アクション名
 * @param[in]	sender	リクエスト元アクター
 * @retval		true	実行成功
 * @retval		false	実行失敗
 */
bool mana_sync_call(mana_actor* self, const int32_t level, const char* action, mana_actor* sender)
{
	MANA_ASSERT(self);

	if(mana_actor_request(self, level, action, sender))
	{
		self->interrupt[level].flag |= MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL;

		for(;;)
		{
			mana_run(self->parent, mana_actor_get_delta_time());

			if(self->interrupt_level < level)
				return true;
		}
	}

	return false;
}

/*!
 * アクション終了まで実行します。
 * bool mana_sync_call(mana_actor* self, int32_t level, char* action, mana_actor* sender)
 * と違い他のアクターは停止します。
 * アクションが終了しないとdead lockするので注意してください。
 * また、他のアクターに処理が移らないのでreqwsなどの命令はdead lockするので注意してください。
 *
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	level	優先度(高いほど優先)
 * @param[in]	action	アクション名
 * @param[in]	sender	リクエスト元アクター
 * @retval		true	実行成功
 * @retval		false	実行失敗
 */
bool mana_async_call(mana_actor* self, const int32_t level, const char* action, mana_actor* sender)
{
	MANA_ASSERT(self);

	if(mana_actor_request(self, level, action, sender))
	{
		self->interrupt[level].flag |= MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL;

		for(;;)
		{
			mana_actor_run(self);

			if(self->interrupt_level < level)
				return true;
		}
	}

	return false;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	level	優先度(高いほど優先)
 * @param[in]	action	アクション名
 * @param[in]	sender	リクエスト元アクター
 * @retval		true	リクエスト成功
 * @retval		false	リクエスト失敗
 */
bool mana_actor_request(mana_actor* self, const int32_t level, const char* action, mana_actor* sender)
{
	MANA_ASSERT(self);

	if(level < 0 || level >= MANA_ACTOR_MAX_INTERRUPT_LEVEL)
	{
		MANA_TRACE("MANA: %s::%s request failed. reason: level %d range over\n",
			mana_actor_get_name(self), action, level);
		return false;
	}

	if(self->flag & MANA_ACTOR_FLAG_HALT)
	{
		MANA_TRACE("MANA: level %d, %s::%s request failed. reason: halt\n",
			level, mana_actor_get_name(self), action);
		return false;
	}

	if(self->flag & MANA_ACTOR_FLAG_REFUSED)
	{
		MANA_TRACE("MANA: level %d, %s::%s request failed. reason: refuse\n",
			level, mana_actor_get_name(self), action);
		return false;
	}

	if(self->interrupt[level].address != ~0)
	{
#if defined(NDEBUG)
		MANA_TRACE("MANA: level %d, %s::%s request failed. reason: level %d running\n",
			level, mana_actor_get_name(self), action, level);
#else
		MANA_TRACE("MANA: level %d, %s::%s request failed. reason: %s running\n",
			level, mana_actor_get_name(self), action, self->interrupt[level].action_name);
#endif
		return false;
	}

	uint32_t address = mana_actor_get_action(self, action);
	if(address == ~0)
	{
		MANA_TRACE("MANA: level %d, %s::%s request failed. reason: not found\n",
			level, mana_actor_get_name(self), action);
		return false;
	}

	self->flag |= MANA_ACTOR_FLAG_RUNNING;
	self->interrupt[level].counter = 0;
	self->interrupt[level].sender = sender;
	self->interrupt[level].address = address;
	self->interrupt[level].flag &= ~MANA_INTERRUPT_FLAG_SYNCHRONIZED;
	self->interrupt[level].file_callback_parameter = NULL;

#if !defined(NDEBUG)
	if(self->interrupt[self->interrupt_level].action_name)
	{
		MANA_TRACE("Mana:request: level %d %s => level %d %s (%08x) succeed\n", self->interrupt_level, self->interrupt[self->interrupt_level].action_name, level, action, address);
	}else{
		MANA_TRACE("Mana:request: level %d %s (%08x) succeed\n", level, action, address);
	}

	/* 実行するアクション名を記録 */
	self->interrupt[level].action_name = action;
#endif

	if(level >= self->interrupt_level)
	{
		/* コールバック関数を呼びます */
		if(mana_actor_request_callback)
			mana_actor_request_callback(self, self->request_callback_parameter);

		/* 現在よりも高い優先度(高いほど優先)の場合、すぐに割り込む */
		mana_actor_again(self);

		/* 現在のFPとSPを保存します */
		self->interrupt[level].frame_pointer = mana_frame_get_size(&self->frame);
		self->interrupt[level].stack_pointer = mana_stack_get_size(&self->stack);

		/* 現在のプログラムカウンタを保存します */
		self->interrupt[self->interrupt_level].address = self->pc;

		/* 新しい優先度(高いほど優先)とプログラムカウンタを設定します */
		self->interrupt_level = (uint8_t)level;
		self->pc = address;

		/* 次のTickでプログラムカウンタを進めない処理 */
		self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;
	}
	else
	{
		/* 現在よりも低い優先度(高いほど優先)の場合、現在のアクション終了後に実行 */

		/* 現在の優先度(高いほど優先)に入ったときのFPとSPを保存します */
		self->interrupt[level].frame_pointer = self->interrupt[self->interrupt_level].frame_pointer;
		self->interrupt[level].stack_pointer = self->interrupt[self->interrupt_level].stack_pointer;

		/* 次のTickでプログラムカウンタを進める処理 */
		self->interrupt[level].flag &= ~MANA_INTERRUPT_FLAG_REPEAT;
	}

	MANA_ASSERT(self->pc != ~0);

	return true;
}

/*!
 * アクションを終了して指定優先度まで還元します
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	level	還元する優先度
 */
void mana_actor_rollback(mana_actor* self, int32_t level)
{
	size_t frame_pointer, stack_pointer;
	int32_t current_level;
	bool in_sync_call;

	MANA_ASSERT(self);

	/* 非同期モードに変更 */
	mana_actor_set_synchronized(self, false);

	/* リクエストが終了したことをsenderに通知する */
	if(self->interrupt[self->interrupt_level].sender)
	{
		mana_actor_again(self->interrupt[self->interrupt_level].sender);
		self->interrupt[self->interrupt_level].sender = NULL;
	}

	/* ファイル読み込み中ならば解放 */
	if(mana_async_file_callback && self->interrupt[self->interrupt_level].file_callback_parameter)
	{
		mana_async_file_callback(MANA_FILE_COMMAND_CLOSE, self->interrupt[self->interrupt_level].file_callback_parameter);
		self->interrupt[self->interrupt_level].file_callback_parameter = NULL;
	}

	/* 優先度開放 */
	self->interrupt[self->interrupt_level].address = (uint32_t)~0;

#if !defined(NDEBUG)
	/* 実行中のアクション名を消去 */
	self->interrupt[self->interrupt_level].action_name = NULL;
#endif

	/* SyncCall中か取得 */
	in_sync_call = self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL;
	if(in_sync_call)
	{
		self->interrupt[self->interrupt_level].flag &= ~MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL;
	}

	/* FPとSPを取得 */
	frame_pointer = self->interrupt[self->interrupt_level].frame_pointer;
	stack_pointer = self->interrupt[self->interrupt_level].stack_pointer;

	/* ログ出力 */
	/*
	MANA_TRACE("mana_actor::Rollback: %d, %s\n", interrupt_level, GetName());
	*/

	/* 優先度(高いほど優先)が指定されているならば、強制的に指定優先度まで戻る */
	current_level = self->interrupt_level - 1;
	if(current_level > level)
	{
		while(current_level > level)
		{
			mana_interrupt* interrupt = &self->interrupt[current_level];

			/* 優先度開放 */
			interrupt->address = (uint32_t)~0;

			/* ファイルエントリの削除 */
			if(mana_async_file_callback && interrupt->file_callback_parameter)
			{
				MANA_TRACE("mana_actor_rollback: %d: waitting for file loading\n", current_level);

				mana_async_file_callback(MANA_FILE_COMMAND_CLOSE, interrupt->file_callback_parameter);
				interrupt->file_callback_parameter = NULL;
			}

			/* フレームポインタとスタックポインタを取得 */
			frame_pointer = interrupt->frame_pointer;
			stack_pointer = interrupt->stack_pointer;

			/* コールバック関数を呼びます */
			if(mana_actor_rollback_callback)
				mana_actor_rollback_callback(self, self->rollback_callback_parameter);

			current_level--;
		}
	}

	/* FPとSPを復元します */
	mana_frame_set_size(&self->frame, frame_pointer);
	mana_stack_set_size(&self->stack, stack_pointer);

	/* 中断されたアクションの再開 */
	while(current_level >= 0)
	{
		mana_interrupt* interrupt = &self->interrupt[current_level];
		if(interrupt->address != ~0)
		{
			/* 中断していた場所から復帰させます */
			self->pc = interrupt->address;

			/* コールバック関数を呼びます */
			if(mana_actor_rollback_callback)
				mana_actor_rollback_callback(self, self->rollback_callback_parameter);

			/* 優先度(高いほど優先)変更 */
			self->interrupt_level = (uint8_t)current_level;

			/* 次回のTickでプログラムカウンターを加算しない */
			self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;

			/* SyncCall中ならばTickを抜ける */
			if(in_sync_call)
				mana_actor_yield(self);

			/*
			MANA_TRACE("rollback: %d, %s succeed\n", interrupt_level, GetName());
			MANA_ASSERT(
				self->pc >= GetParent().instruction_pool &&
				self->pc < &GetParent().instruction_pool[GetParent().file_header->size_of_instruction_pool]);
			*/

			return;
		}
		current_level--;
	}

	/* 実行可能なアクションが無いので、アクターは停止する */
	mana_actor_restart(self);
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_restart(mana_actor* self)
{
	int32_t i;

	MANA_ASSERT(self);

	self->pc = (uint32_t)~0;
	self->interrupt_level = 0;
	self->flag &= ~(MANA_ACTOR_FLAG_HALT|MANA_ACTOR_FLAG_RUNNING|MANA_ACTOR_FLAG_REFUSED);

	memset(&self->interrupt, 0, sizeof(self->interrupt));
	for(i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
	{
		self->interrupt[i].address = (uint32_t)~0;
		self->interrupt[i].return_address = (uint32_t)~0;
	}

	mana_frame_clear(&self->frame);
	mana_stack_clear(&self->stack);
}

/*!
 * 実行速度は遅いので注意が必要
 * @param[in]	self	mana_actor オブジェクト
 * @retval		!= NULL			アクターの名前
 * @retval		== NULL			検索失敗
 */
const char* mana_actor_get_name(mana_actor* self)
{
	MANA_ASSERT(self);

	return mana_get_actor_name(self->parent, self);
}

/*!
 * 実行速度は遅いので注意が必要
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	action	アクションの名前
 * @return		アクションのプログラムアドレス。0なら失敗
 */
uint32_t mana_actor_get_action(mana_actor* self, const char* action)
{
	MANA_ASSERT(self);

	if(!mana_hash_test(&self->actions, action))
		return (uint32_t)(~0);

	const uint32_t address = (uint32_t)mana_hash_get(&self->actions, action);
	return (address == 0) ? (uint32_t)(~0) : address;
}

/*!
 * 汎用カウンターは優先度毎に設定できます
 * @param[in]	self	mana_actor オブジェクト
 * @return		汎用カウンター
 */
int32_t mana_actor_get_counter(mana_actor* self)
{
	MANA_ASSERT(self);

	return self->interrupt[self->interrupt_level].counter;
}

/*!
 * 汎用カウンターは優先度毎に設定できます
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	counter	汎用カウンター
 */
void mana_actor_set_counter(mana_actor* self, const int32_t counter)
{
	MANA_ASSERT(self);

	self->interrupt[self->interrupt_level].counter = counter;
}

/*!
 * 外部関数に積まれた引数の数を取得します
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @return		引数の数
 */
int32_t mana_actor_get_argument_count(mana_actor* self)
{
	MANA_ASSERT(self);

	return mana_actor_get_argument_count_by_address(self, self->pc);
}

/*!
 * 外部関数に積まれた引数の数を取得します
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		引数の数
 */
int32_t mana_actor_get_argument_count_by_address(mana_actor* self, const uint32_t address)
{
	return (int32_t)mana_actor_get_unsigned_char_from_memory(self, address + 1 + sizeof(int32_t) + sizeof(uint8_t));
}

/*!
 * 外部関数に積まれた引数のワードサイズを取得します
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	address	プログラムアドレス
 * @return		外部関数に積まれた引数のワードサイズ
 */
int32_t mana_actor_get_argument_size(mana_actor* self, const uint32_t address)
{
	return (int32_t)mana_actor_get_unsigned_short_from_memory(self, address + 1 + sizeof(int32_t) + sizeof(uint8_t) + sizeof(uint8_t));
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	address	プログラムアドレス
 * @retval		true	戻り値あり
 * @retval		false	戻り値なし
 */
bool mana_actor_has_return_value(mana_actor* self, const uint32_t address)
{
	return (bool)mana_actor_get_unsigned_char_from_memory(self, address + 1 + sizeof(int32_t));
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	引数の番号
 * @return		引数の値
 */
int32_t mana_actor_get_parameter_integer(mana_actor* self, const int32_t value)
{
	uint32_t address = self->pc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t));
	size_t offset;

	MANA_ASSERT(self && mana_actor_get_argument_count(self) > value);

	offset = (size_t)mana_actor_get_unsigned_short_from_memory(self, address);
	return mana_stack_get_integer(&self->stack, offset);
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	引数の番号
 * @return		引数の値
 */
float mana_actor_get_parameter_float(mana_actor* self, const int32_t value)
{
	MANA_ASSERT(self && mana_actor_get_argument_count(self) > value);

	return mana_stack_get_real(
		&self->stack,
		mana_actor_get_short_from_memory(self, self->pc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t)))
	);
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	引数の番号
 * @return		引数の値
 */
const char* mana_actor_get_parameter_string(mana_actor* self, const int32_t value)
{
	MANA_ASSERT(self && mana_actor_get_argument_count(self) > value);

	return mana_stack_get_string(
		&self->stack,
		mana_actor_get_short_from_memory(self, self->pc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t)))
	);
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	引数の番号
 * @return		引数の値
 */
mana_actor* mana_actor_get_parameter_actor(mana_actor* self, const int32_t value)
{
	return mana_actor_get_parameter_pointer(self, value);
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	引数の番号
 * @return		引数の値
 */
void* mana_actor_get_parameter_pointer(mana_actor* self, const int32_t value)
{
	MANA_ASSERT(self && mana_actor_get_argument_count(self) > value);

	return mana_stack_get_pointer(
		&self->stack,
		mana_actor_get_short_from_memory(self, self->pc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t)))
	);
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	引数の番号
 * @return		引数の値
 */
void* mana_actor_get_parameter_address(mana_actor* self, const int32_t value)
{
	MANA_ASSERT(self && mana_actor_get_argument_count(self) > value);

	return mana_stack_get_address(
		&self->stack,
		mana_actor_get_short_from_memory(self, self->pc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))) + 1
	);
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	戻り値
 */
void mana_actor_set_return_integer(mana_actor* self, const int32_t value)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_INTEGER;
	self->return_value.values.integer_value = value;
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	戻り値
 */
void mana_actor_set_return_float(mana_actor* self, const float value)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_FLOAT;
	self->return_value.values.float_value = value;
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	戻り値
 */
void mana_actor_set_return_string(mana_actor* self, const char* string)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_STRING;
	self->return_value.values.string_value = string;
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	戻り値
 */
void mana_actor_set_return_actor(mana_actor* self, mana_actor* actor)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_ACTOR;
	self->return_value.values.actor_value = actor;
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	戻り値
 */
void mana_actor_set_return_pointer(mana_actor* self, void* pointer)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_ACTOR;
	self->return_value.values.pointer_value = pointer;
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	value	戻り値
 * @param[in]	size	戻り値のサイズ
 */
void mana_actor_set_return_data(mana_actor* self, const void* pointer, const int32_t size)
{
	MANA_ASSERT(self);

	if(size > 0)
	{
		self->return_value.values.pointer_value = mana_malloc(size);
		memcpy(self->return_value.values.pointer_value, pointer, size);
	}
	else
	{
		self->return_value.values.pointer_value = NULL;
	}
	self->return_value.size = size;
}

#if !defined(NDEBUG)
/*!
 * @param[in]	self	mana_actor オブジェクト
 * @return		実行中のアクション名
 */
const char* mana_actor_get_action_name(const mana_actor* self)
{
	const char* action;

	MANA_ASSERT(self);

	action = self->interrupt[self->interrupt_level].action_name;

	return action ? action : "SLEEP!";
}

/*!
 * @warning 	外部関数以外から呼び出し禁止
 * @param[in]	self	mana_actor オブジェクト
 * @return		外部関数名
 */
const char* mana_actor_get_function_name(const mana_actor* self)
{
	MANA_ASSERT(self);

	return mana_actor_get_string_from_memory(self, self->pc + 1);
}
#endif

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	action	アクション名
 * @param[in]	address	プログラムアドレス
 */
void mana_actor_set_action(mana_actor* self, const char* action, uint8_t* address)
{
	MANA_ASSERT(self);

	mana_hash_set(&self->actions, action, address);
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @return		mana オブジェクト
 */
mana* mana_actor_get_parent(const mana_actor* self)
{
	MANA_ASSERT(self);

	return self->parent;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @retval		false	初期化完了
 * @retval		true	初期化中
 */
bool mana_actor_is_init(const mana_actor* self)
{
	MANA_ASSERT(self);

	return (self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_INITIALIZED) ? false : true;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @retval		true	再実行が必要
 * @retval		false	再実行は不要
 */
bool mana_actor_is_repeat(const mana_actor* self)
{
	MANA_ASSERT(self);

	return (self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_REPEAT) ? true : false;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @retval		true	実行中
 * @retval		false	停止
 */
bool mana_actor_is_running(const mana_actor* self)
{
	uint8_t flag;

	MANA_ASSERT(self);

	flag = self->flag & (MANA_ACTOR_FLAG_HALT|MANA_ACTOR_FLAG_RUNNING);
	return (flag == MANA_ACTOR_FLAG_RUNNING) ? true : false;
}

/*!
 * 実行を中断して次のフレームに再実行します。
 * @param[in]	self				mana_actor オブジェクト
 * @param[in]	initial_complete	初期化完了フラグ
 */
void mana_actor_repeat(mana_actor* self, const bool initial_complete)
{
	MANA_ASSERT(self);

	if(initial_complete)
		self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_INITIALIZED;
	else
		self->interrupt[self->interrupt_level].flag &= ~MANA_INTERRUPT_FLAG_INITIALIZED;
	self->interrupt[self->interrupt_level].flag |= (MANA_INTERRUPT_FLAG_REPEAT|MANA_INTERRUPT_FLAG_SUSPEND);
}

/*!
 * 同一フレーム中に再実行します。
 * また、void mana_actor_repeat(mana_actor* self, bool initial_complete)と違い中断はしません。
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_again(mana_actor* self)
{
	MANA_ASSERT(self && self->parent);

	self->flag |= MANA_ACTOR_FLAG_REQUESTED;
	self->parent->flag |= MANA_FLAG_REQUESTED;
}

/*!
 * void mana_actor_stop(mana_actor* self)はリクエストを受け付けますが、
 * void mana_actor_halt(mana_actor* self)はリクエストも受けつめません。
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_halt(mana_actor* self)
{
	MANA_ASSERT(self);

	mana_actor_stop(self);
	mana_actor_yield(self);

	self->flag |= MANA_ACTOR_FLAG_HALT;
	self->interrupt_level = MANA_ACTOR_MAX_INTERRUPT_LEVEL - 1;
	self->interrupt[self->interrupt_level].address = (uint32_t)~0;
}

/*!
 * void mana_actor_halt(mana_actor* self)はリクエストを受け付けませんが、
 * void mana_actor_stop(mana_actor* self)は実行停止のみ行ないます。
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_stop(mana_actor* self)
{
	MANA_ASSERT(self);

	self->flag &= ~MANA_ACTOR_FLAG_RUNNING;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_yield(mana_actor* self)
{
	MANA_ASSERT(self);

	self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_SUSPEND;
}

/*!
 * リクエストを受け付けます
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_comply(mana_actor* self)
{
	MANA_ASSERT(self);

	self->flag &= ~MANA_ACTOR_FLAG_REFUSED;
}

/*!
 * リクエストを受け付けません
 * @param[in]	self	mana_actor オブジェクト
 */
void mana_actor_refuse(mana_actor* self)
{
	MANA_ASSERT(self);

	self->flag |= MANA_ACTOR_FLAG_REFUSED;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @return		現在の優先度
 */
int32_t mana_actor_get_interrupt_level(mana_actor* self)
{
	MANA_ASSERT(self);

	return self->interrupt_level;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @retval		true	同期中
 * @retval		false	非同期中
 */
bool mana_actor_is_synchronized(mana_actor* self)
{
	MANA_ASSERT(self);

	return (self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_SYNCHRONIZED) ? true : false;
}

/*!
 * @param[in]	self			mana_actor オブジェクト
 * @param[in]	synchronized
 * - trueならば同期実行
 * - falseならば非同期実行
 */
void mana_actor_set_synchronized(mana_actor* self, bool synchronized)
{
	MANA_ASSERT(self);

	if(synchronized)
		self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_SYNCHRONIZED;
	else
		self->interrupt[self->interrupt_level].flag &= ~MANA_INTERRUPT_FLAG_SYNCHRONIZED;
}

/*!
 * @param[in]	self			mana_actor オブジェクト
 * @param[in]	level			優先度
 * @param[in]	synchronized
 * - trueならば同期実行
 * - falseならば非同期実行
 */
void mana_actor_set_synchronized_with_level(mana_actor* self, int32_t level, bool synchronized)
{
	MANA_ASSERT(self);
	MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);

	if(synchronized)
		self->interrupt[level].flag |= MANA_INTERRUPT_FLAG_SYNCHRONIZED;
	else
		self->interrupt[level].flag &= ~MANA_INTERRUPT_FLAG_SYNCHRONIZED;
}

/*!
 * @param[in]	self		mana_actor オブジェクト
 * @param[in]	resouce_id	リソース番号
 * @param[out]	buffer		データアドレス
 * @param[out]	size		データサイズ
 */
void mana_actor_get_data(const mana_actor* self, const int32_t resouce_id, const void** buffer, size_t* size)
{
	MANA_ASSERT(self);

	MANA_ASSERT(resouce_id >= 0 && resouce_id < mana_datalink_get_number_of_datas(&self->parent->datalink));
	*buffer = mana_datalink_get_data(&self->parent->datalink, resouce_id);
	*size = mana_datalink_get_data_size(&self->parent->datalink, resouce_id);
}

/*!
 * @return	ファイルコールバック関数
 */
mana_actor_file_callback* mana_actor_get_file_callback(void)
{
	return mana_async_file_callback;
}

/*!
 * @param[in]	function	ファイルコールバック関数
 */
void mana_actor_set_file_callback(mana_actor_file_callback* function)
{
	mana_async_file_callback = function;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @param[in]	level	優先度
 * @return		ファイルコールバックパラメーター
 */
void* mana_actor_get_file_callback_parameter(mana_actor* self, int32_t level)
{
	MANA_ASSERT(self);
	MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);

	return self->interrupt[level].file_callback_parameter;
}

/*!
 * @param[in]	self		mana_actor オブジェクト
 * @param[in]	level		優先度
 * @param[in]	parameter	ファイルコールバックパラメーター
 */
void mana_actor_set_file_callback_parameter(mana_actor* self, int32_t level, void* parameter)
{
	MANA_ASSERT(self);
	MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);

	self->interrupt[level].file_callback_parameter = parameter;
}

/*!
 * @param[in]	function	リクエストコールバック関数
 */
void mana_actor_set_request_callback(mana_actor_callback* function)
{
	mana_actor_request_callback = function;
}

/*!
 * @param[in]	self		mana_actor オブジェクト
 * @param[in]	parameter	リクエストコールバックパラメーター
 */
void mana_actor_set_request_callback_parameter(mana_actor* self, void* parameter)
{
	MANA_ASSERT(self);

	self->request_callback_parameter = parameter;
}

/*!
 * @param[in]	function	ロールバックコールバック関数
 */
void mana_actor_set_rollback_callback(mana_actor_callback* function)
{
	mana_actor_rollback_callback = function;
}

/*!
 * @param[in]	function	ロールバックコールバック関数
 * @param[in]	parameter	ロールバックコールバックパラメーター
 */
void mana_actor_set_rollback_callback_parameter(mana_actor* self, void* parameter)
{
	MANA_ASSERT(self);

	self->rollback_callback_parameter = parameter;
}

/*!
 * @param[in]	self	mana_actor オブジェクト
 * @return		ユーザーデーター
 */
uint32_t mana_actor_get_user_data(const mana_actor* self)
{
	MANA_ASSERT(self);

	return self->user_data;
}

/*!
 * @param[in]	self		mana_actor オブジェクト
 * @param[in]	user_data	ユーザーデーター
 */
void mana_actor_set_user_data(mana_actor* self, uint32_t user_data)
{
	MANA_ASSERT(self);

	self->user_data = user_data;
}

/*!
 * @param[in]	self		mana_actor オブジェクト
 * @return		ユーザーポインター
 */
void* mana_actor_get_user_pointer(const mana_actor* self)
{
	MANA_ASSERT(self);

	return self->user_pointer;
}

/*!
 * @param[in]	self			mana_actor オブジェクト
 * @param[in]	user_pointer	ユーザーポインター
 */
void mana_actor_set_user_pointer(mana_actor* self, void* user_pointer)
{
	MANA_ASSERT(self);

	self->user_pointer = user_pointer;
}
