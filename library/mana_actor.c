/*!
 * mana (library)
 *
 * @file	mana_actor.c
 * @brief	mana_actor�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana_actor�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana_actor�N���X�͍X�V�⃊�N�G�X�g�Ȃǂ̐���A�O���֐��̌Ăяo���Ȃǂ�
 * ���s����\���Ă��܂��B
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
 * @param[in]	�v���O�����̈悩��P�o�C�g���̐��l���擾���܂�
 * @param[in]	�v���O�����A�h���X
 * @return		�v���O�����̈��̂P�o�C�g���̐��l
 */
static int8_t mana_actor_get_char_from_memory(const mana_actor* self, const uint32_t address)
{
	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_instruction_pool);

	return self->parent->instruction_pool[address];
}

/*!
 * @param[in]	�v���O�����̈悩��P�o�C�g���̐��l���擾���܂�
 * @param[in]	�v���O�����A�h���X
 * @return		�v���O�����̈��̂P�o�C�g���̐��l
 */
static uint8_t mana_actor_get_unsigned_char_from_memory(const mana_actor* self, const uint32_t address)
{
	return (uint8_t )mana_actor_get_char_from_memory(self, address);
}

/*!
 * @param[in]	�v���O�����̈悩��Q�o�C�g���̐��l���擾���܂�
 * @param[in]	�v���O�����A�h���X
 * @return		�v���O�����̈��̂Q�o�C�g���̐��l
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
 * @param[in]	�v���O�����̈悩��Q�o�C�g���̐��l���擾���܂�
 * @param[in]	�v���O�����A�h���X
 * @return		�v���O�����̈��̂Q�o�C�g���̐��l
 */
static int16_t mana_actor_get_short_from_memory(const mana_actor* self, const uint32_t address)
{
	return (int16_t)mana_actor_get_unsigned_short_from_memory(self, address);
}

/*!
 * @param[in]	�v���O�����̈悩��S�o�C�g���̐��l���擾���܂�
 * @param[in]	�v���O�����A�h���X
 * @return		�v���O�����̈��̂S�o�C�g���̐��l
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
 * @param[in]	�v���O�����̈悩��S�o�C�g���̐��l���擾���܂�
 * @param[in]	�v���O�����A�h���X
 * @return		�v���O�����̈��̂S�o�C�g���̐��l
 */
static int32_t mana_actor_get_integer_from_memory(const mana_actor* self, const uint32_t address)
{
	return (int32_t)mana_actor_get_unsigned_integer_from_memory(self, address);
}

/*!
 * @param[in]	�v���O�����̈悩��P���x�����������擾���܂�
 * @param[in]	�v���O�����A�h���X
 * @return		�v���O�����̈��̒P���x��������
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
 * �v���O�����̈悩��I�t�Z�b�g���擾��A������̈悩�當�����擾���܂�
 * @param[in]	mana_actor �I�u�W�F�N�g
 * @param[in]	�v���O�����A�h���X
 * @return	������ւ̃|�C���^
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
 * ������̈悩�當�����擾���܂�
 * @param[in]	mana_actor �I�u�W�F�N�g
 * @param[in]	�v���O�����A�h���X
 * @return	������ւ̃|�C���^
 */
static char* mana_actor_get_string_from_data(const mana_actor* self, const uint32_t address)
{
	MANA_ASSERT(self);
	MANA_ASSERT(address < self->parent->file_header->size_of_constant_pool);

	return self->parent->constant_pool + address;
}
#endif

/*!
 * �I�y�R�[�h���擾
 * @param	address		�v���O�����A�h���X
 * @return	�I�y�R�[�h
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
	mana_stack_push_integer(&actor->stack, mana_actor_get_integer_from_memory(actor, actor->pc + 1));
}

static void mana_actor_cmd_push_float(mana_actor* actor)
{
	mana_stack_push_real(&actor->stack, mana_actor_get_float_from_memory(actor, actor->pc + 1));
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
	mana_frame_allocate(&actor->frame, mana_actor_get_integer_from_memory(actor, actor->pc + 1));
}

static void mana_actor_cmd_free(mana_actor* actor)
{
	mana_frame_release(&actor->frame, mana_actor_get_integer_from_memory(actor, actor->pc + 1));
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
	size_t offset = (size_t)mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	MANA_ASSERT(offset < mana_static_memory_size);
	mana_stack_push_pointer(&actor->stack, &mana_static_memory[offset]);
}

static void mana_actor_cmd_load_global_address(mana_actor* actor)
{
	size_t offset = (size_t)mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	MANA_ASSERT(offset < actor->parent->file_header->size_of_global_memory);
	mana_stack_push_pointer(&actor->stack, &actor->parent->global_memory[offset]);
}

static void mana_actor_cmd_load_frame_address(mana_actor* actor)
{
	int32_t offset = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
	void* address = mana_frame_get_address(&actor->frame, offset);
	mana_stack_push_pointer(&actor->stack, address);
}

static void mana_actor_cmd_load_self_address(mana_actor* actor)
{
	int32_t offset = mana_actor_get_integer_from_memory(actor, actor->pc + 1);
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
	char* pointer = (char*)mana_stack_get_pointer(&actor->stack, 0);
	int32_t value = mana_stack_get_integer(&actor->stack, 1);
	*pointer = (int8_t)value;
	mana_stack_remove(&actor->stack, 2);
}

static void mana_actor_cmd_store_short(mana_actor* actor)
{
	int16_t* pointer = (int16_t*)mana_stack_get_pointer(&actor->stack, 0);
	int32_t value = mana_stack_get_integer(&actor->stack, 1);
	*pointer = (int16_t)value;
	mana_stack_remove(&actor->stack, 2);
}

static void mana_actor_cmd_store_integer(mana_actor* actor)
{
	int32_t* pointer = (int32_t*)mana_stack_get_pointer(&actor->stack, 0);
	int32_t value = mana_stack_get_integer(&actor->stack, 1);
	*pointer = value;
	mana_stack_remove(&actor->stack, 2);
}

static void mana_actor_cmd_store_float(mana_actor* actor)
{
	float* pointer = (float*)mana_stack_get_pointer(&actor->stack, 0);
	float value = mana_stack_get_real(&actor->stack, 1);
	*pointer = value;
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

	/* �O���֐��̎��s */
	const char* name = mana_actor_get_string_from_memory(actor, actor->pc + 1);
	mana_external_funtion_type* function = mana_hash_get(&mana_external_function_hash, name);
	if(!function)
	{
		MANA_ERROR("An external function called %s was not found.\n", name);
	}
	MANA_ASSERT(function);

	function(actor);

	/* �O���֐��̏I�� */
	if(mana_actor_is_running(actor))
	{
		if(!mana_actor_is_repeat(actor))
		{
			bool bHasReturnValue = mana_actor_has_return_value(actor, last_pc);
			int32_t nNumberOfArguments = mana_actor_get_argument_count_by_address(actor, last_pc);
			int32_t nSizeOfArguments   = mana_actor_get_argument_size(actor, last_pc);

			/* �X�^�b�N�ɓ����Ă���p�����[�^��pop���� */
			mana_stack_remove(&actor->stack, nSizeOfArguments);

			/* �߂�l������Ȃ�A�߂�l��push */
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

			/* �O���֐�����req�n�̖��߂����s���ꂽ�ꍇ�A�X�^�b�N�̏�Ԃ��C������ */
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
	int32_t level = mana_stack_get_integer(&actor->stack, 1);
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
	int32_t level = mana_stack_get_integer(&actor->stack, 1);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left & right);
}

static void mana_actor_cmd_exclusive_or(mana_actor* actor)
{
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left ^ right);
}

static void mana_actor_cmd_logical_and(mana_actor* actor)
{
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left && right);
}

static void mana_actor_cmd_logical_or(mana_actor* actor)
{
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left | right);
}

static void mana_actor_cmd_shift_left(mana_actor* actor)
{
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left << right);
}

static void mana_actor_cmd_shift_right(mana_actor* actor)
{
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
	mana_stack_remove(&actor->stack, 1);
	mana_stack_set_integer(&actor->stack, 0, left >> right);
}

static void mana_actor_cmd_compare_equal_integer(mana_actor* actor)
{
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
	int32_t left  = mana_stack_get_integer(&actor->stack, 1);
	int32_t right = mana_stack_get_integer(&actor->stack, 0);
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
				if(counter >= number_of_arguments)
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
					message_pointer += sprintf(&message[message_pointer], "%d", mana_stack_get_integer(&actor->stack, counter++));
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

#if !defined(NDEBUG) && (defined(DEBUG) || defined(_DEBUG) || defined(_DEBUG_))
#if defined(_WIN32) || defined(_WIN64)
	OutputDebugStringA(message);
#else
	fprintf(stdout, "%s", message);
#endif
#endif

	mana_stack_remove(&actor->stack, number_of_arguments);
}

/*! �������߃W�����v�e�[�u�� */
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
 * @param[in]	parent			mana �I�u�W�F�N�g
 * @param[in]	variable_size	�A�N�^�[�ϐ��̃T�C�Y
 * @return		mana_actor �I�u�W�F�N�g
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
 * @param[in]	actor	�R�s�[����mana_actor �I�u�W�F�N�g
 * @return		mana_actor �I�u�W�F�N�g
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
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 */
void mana_actor_destroy(mana_actor* self)
{
	if(self)
	{
		int32_t i;

		/* �ǂݍ��ݒ���delete���ꂽ�H */
		for(i = 0 ; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			mana_interrupt* interrupt = &self->interrupt[i];

			/* �t�@�C���G���g���̍폜 */
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
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[out]	stream	mana_stream �I�u�W�F�N�g
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

	mana_stream_push_unsigned_integer(stream, self->variable_size);
	mana_stream_push_data(stream, self->variable, self->variable_size);
	mana_stream_push_integer(stream, self->pc);

	mana_stream_push_char(stream, self->interrupt_level);
	mana_stream_push_unsigned_char(stream, self->flag);

	mana_stream_mark(stream);
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	stream	mana_stream �I�u�W�F�N�g
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

	self->variable_size = mana_stream_pop_unsigned_integer(stream);
	self->variable = mana_malloc(self->variable_size);
	mana_stream_pop_data(stream, self->variable, self->variable_size);

	self->pc = mana_stream_pop_integer(stream);

	self->interrupt_level = mana_stream_pop_char(stream);
	self->flag = mana_stream_pop_unsigned_char(stream);

	mana_stream_check(stream);
}

/*!
 * �O���֐�������Ă΂�鎖��z�肵�Ă��܂��B
 * bool mana_run(mana* self, float second)��second�̒l���擾�ł���̂ŁA
 * �e�O���֐��Ōo�߂������Ԃ��K�v�Ȏ��Ɏg���Ă��������B
 *
 * @return		�o�ߎ���
 */
float mana_actor_get_delta_time(void)
{
	return mana_actor_delta_time;
}

/*!
 * bool mana_run(mana* self, float second)��second��ݒ肵�Ă��܂��B
 * @param[in]	second	�o�ߎ���(�b)
 */
void mana_actor_set_delta_time(float second)
{
	mana_actor_delta_time = second;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @retval		true	���s��
 * @retval		false	��~
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
 * �A�N�V�����I���܂Ŏ��s���܂��B
 * bool mana_async_call(mana_actor* self, int32_t level, char* action, mana_actor* sender)
 * �ƈႢ���̃A�N�^�[�����񓮍삵�܂��B
 * �A�N�V�������I�����Ȃ���dead lock����̂Œ��ӂ��Ă��������B
 *
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	level	�D��x(�����قǗD��)
 * @param[in]	action	�A�N�V������
 * @param[in]	sender	���N�G�X�g���A�N�^�[
 * @retval		true	���s����
 * @retval		false	���s���s
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
 * �A�N�V�����I���܂Ŏ��s���܂��B
 * bool mana_sync_call(mana_actor* self, int32_t level, char* action, mana_actor* sender)
 * �ƈႢ���̃A�N�^�[�͒�~���܂��B
 * �A�N�V�������I�����Ȃ���dead lock����̂Œ��ӂ��Ă��������B
 * �܂��A���̃A�N�^�[�ɏ������ڂ�Ȃ��̂�reqws�Ȃǂ̖��߂�dead lock����̂Œ��ӂ��Ă��������B
 *
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	level	�D��x(�����قǗD��)
 * @param[in]	action	�A�N�V������
 * @param[in]	sender	���N�G�X�g���A�N�^�[
 * @retval		true	���s����
 * @retval		false	���s���s
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
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	level	�D��x(�����قǗD��)
 * @param[in]	action	�A�N�V������
 * @param[in]	sender	���N�G�X�g���A�N�^�[
 * @retval		true	���N�G�X�g����
 * @retval		false	���N�G�X�g���s
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

	uintptr_t address = mana_actor_get_action(self, action);
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

	/* ���s����A�N�V���������L�^ */
	self->interrupt[level].action_name = action;
#endif

	if(level >= self->interrupt_level)
	{
		/* �R�[���o�b�N�֐����Ăт܂� */
		if(mana_actor_request_callback)
			mana_actor_request_callback(self, self->request_callback_parameter);

		/* ���݂��������D��x(�����قǗD��)�̏ꍇ�A�����Ɋ��荞�� */
		mana_actor_again(self);

		/* ���݂�FP��SP��ۑ����܂� */
		self->interrupt[level].frame_pointer = mana_frame_get_size(&self->frame);
		self->interrupt[level].stack_pointer = mana_stack_get_size(&self->stack);

		/* ���݂̃v���O�����J�E���^��ۑ����܂� */
		self->interrupt[self->interrupt_level].address = self->pc;

		/* �V�����D��x(�����قǗD��)�ƃv���O�����J�E���^��ݒ肵�܂� */
		self->interrupt_level = (uint8_t)level;
		self->pc = address;

		/* ����Tick�Ńv���O�����J�E���^��i�߂Ȃ����� */
		self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;
	}
	else
	{
		/* ���݂����Ⴂ�D��x(�����قǗD��)�̏ꍇ�A���݂̃A�N�V�����I����Ɏ��s */

		/* ���݂̗D��x(�����قǗD��)�ɓ������Ƃ���FP��SP��ۑ����܂� */
		self->interrupt[level].frame_pointer = self->interrupt[self->interrupt_level].frame_pointer;
		self->interrupt[level].stack_pointer = self->interrupt[self->interrupt_level].stack_pointer;

		/* ����Tick�Ńv���O�����J�E���^��i�߂鏈�� */
		self->interrupt[level].flag &= ~MANA_INTERRUPT_FLAG_REPEAT;
	}

	MANA_ASSERT(self->pc != ~0);

	return true;
}

/*!
 * �A�N�V�������I�����Ďw��D��x�܂ŊҌ����܂�
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	level	�Ҍ�����D��x
 */
void mana_actor_rollback(mana_actor* self, int32_t level)
{
	size_t frame_pointer, stack_pointer;
	int32_t current_level;
	bool in_sync_call;

	MANA_ASSERT(self);

	/* �񓯊����[�h�ɕύX */
	mana_actor_set_synchronized(self, false);

	/* ���N�G�X�g���I���������Ƃ�sender�ɒʒm���� */
	if(self->interrupt[self->interrupt_level].sender)
	{
		mana_actor_again(self->interrupt[self->interrupt_level].sender);
		self->interrupt[self->interrupt_level].sender = NULL;
	}

	/* �t�@�C���ǂݍ��ݒ��Ȃ�Ή�� */
	if(mana_async_file_callback && self->interrupt[self->interrupt_level].file_callback_parameter)
	{
		mana_async_file_callback(MANA_FILE_COMMAND_CLOSE, self->interrupt[self->interrupt_level].file_callback_parameter);
		self->interrupt[self->interrupt_level].file_callback_parameter = NULL;
	}

	/* �D��x�J�� */
	self->interrupt[self->interrupt_level].address = (uint32_t)~0;

#if !defined(NDEBUG)
	/* ���s���̃A�N�V������������ */
	self->interrupt[self->interrupt_level].action_name = NULL;
#endif

	/* SyncCall�����擾 */
	in_sync_call = self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL;
	if(in_sync_call)
	{
		self->interrupt[self->interrupt_level].flag &= ~MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL;
	}

	/* FP��SP���擾 */
	frame_pointer = self->interrupt[self->interrupt_level].frame_pointer;
	stack_pointer = self->interrupt[self->interrupt_level].stack_pointer;

	/* ���O�o�� */
	/*
	MANA_TRACE("mana_actor::Rollback: %d, %s\n", interrupt_level, GetName());
	*/

	/* �D��x(�����قǗD��)���w�肳��Ă���Ȃ�΁A�����I�Ɏw��D��x�܂Ŗ߂� */
	current_level = self->interrupt_level - 1;
	if(current_level > level)
	{
		while(current_level > level)
		{
			mana_interrupt* interrupt = &self->interrupt[current_level];

			/* �D��x�J�� */
			interrupt->address = (uint32_t)~0;

			/* �t�@�C���G���g���̍폜 */
			if(mana_async_file_callback && interrupt->file_callback_parameter)
			{
				MANA_TRACE("mana_actor_rollback: %d: waitting for file loading\n", current_level);

				mana_async_file_callback(MANA_FILE_COMMAND_CLOSE, interrupt->file_callback_parameter);
				interrupt->file_callback_parameter = NULL;
			}

			/* �t���[���|�C���^�ƃX�^�b�N�|�C���^���擾 */
			frame_pointer = interrupt->frame_pointer;
			stack_pointer = interrupt->stack_pointer;

			/* �R�[���o�b�N�֐����Ăт܂� */
			if(mana_actor_rollback_callback)
				mana_actor_rollback_callback(self, self->rollback_callback_parameter);

			current_level--;
		}
	}

	/* FP��SP�𕜌����܂� */
	mana_frame_set_size(&self->frame, frame_pointer);
	mana_stack_set_size(&self->stack, stack_pointer);

	/* ���f���ꂽ�A�N�V�����̍ĊJ */
	while(current_level >= 0)
	{
		mana_interrupt* interrupt = &self->interrupt[current_level];
		if(interrupt->address != ~0)
		{
			/* ���f���Ă����ꏊ���畜�A�����܂� */
			self->pc = interrupt->address;

			/* �R�[���o�b�N�֐����Ăт܂� */
			if(mana_actor_rollback_callback)
				mana_actor_rollback_callback(self, self->rollback_callback_parameter);

			/* �D��x(�����قǗD��)�ύX */
			self->interrupt_level = (uint8_t)current_level;

			/* �����Tick�Ńv���O�����J�E���^�[�����Z���Ȃ� */
			self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_REPEAT;

			/* SyncCall���Ȃ��Tick�𔲂��� */
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

	/* ���s�\�ȃA�N�V�����������̂ŁA�A�N�^�[�͒�~���� */
	mana_actor_restart(self);
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
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
 * ���s���x�͒x���̂Œ��ӂ��K�v
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @retval		!= NULL			�A�N�^�[�̖��O
 * @retval		== NULL			�������s
 */
const char* mana_actor_get_name(mana_actor* self)
{
	MANA_ASSERT(self);

	return mana_get_actor_name(self->parent, self);
}

/*!
 * ���s���x�͒x���̂Œ��ӂ��K�v
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	action	�A�N�V�����̖��O
 * @return		�A�N�V�����̃v���O�����A�h���X�B0�Ȃ玸�s
 */
uintptr_t mana_actor_get_action(mana_actor* self, const char* action)
{
	MANA_ASSERT(self);

	if(!mana_hash_test(&self->actions, action))
		return (uint32_t)(~0);

	const void* address = mana_hash_get(&self->actions, action);
	if(address == NULL)
		return (uint32_t)(~0);

	return (uintptr_t)(address);
}

/*!
 * �ėp�J�E���^�[�͗D��x���ɐݒ�ł��܂�
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @return		�ėp�J�E���^�[
 */
int32_t mana_actor_get_counter(mana_actor* self)
{
	MANA_ASSERT(self);

	return self->interrupt[self->interrupt_level].counter;
}

/*!
 * �ėp�J�E���^�[�͗D��x���ɐݒ�ł��܂�
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	counter	�ėp�J�E���^�[
 */
void mana_actor_set_counter(mana_actor* self, const int32_t counter)
{
	MANA_ASSERT(self);

	self->interrupt[self->interrupt_level].counter = counter;
}

/*!
 * �O���֐��ɐς܂ꂽ�����̐����擾���܂�
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @return		�����̐�
 */
int32_t mana_actor_get_argument_count(mana_actor* self)
{
	MANA_ASSERT(self);

	return mana_actor_get_argument_count_by_address(self, self->pc);
}

/*!
 * �O���֐��ɐς܂ꂽ�����̐����擾���܂�
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�����̐�
 */
int32_t mana_actor_get_argument_count_by_address(mana_actor* self, const uint32_t address)
{
	return (int32_t)mana_actor_get_unsigned_char_from_memory(self, address + 1 + sizeof(int32_t) + sizeof(uint8_t));
}

/*!
 * �O���֐��ɐς܂ꂽ�����̃��[�h�T�C�Y���擾���܂�
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�O���֐��ɐς܂ꂽ�����̃��[�h�T�C�Y
 */
int32_t mana_actor_get_argument_size(mana_actor* self, const uint32_t address)
{
	return (int32_t)mana_actor_get_unsigned_short_from_memory(self, address + 1 + sizeof(int32_t) + sizeof(uint8_t) + sizeof(uint8_t));
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @retval		true	�߂�l����
 * @retval		false	�߂�l�Ȃ�
 */
bool mana_actor_has_return_value(mana_actor* self, const uint32_t address)
{
	return (bool)mana_actor_get_unsigned_char_from_memory(self, address + 1 + sizeof(int32_t));
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�����̔ԍ�
 * @return		�����̒l
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
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�����̔ԍ�
 * @return		�����̒l
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
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�����̔ԍ�
 * @return		�����̒l
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
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�����̔ԍ�
 * @return		�����̒l
 */
mana_actor* mana_actor_get_parameter_actor(mana_actor* self, const int32_t value)
{
	return mana_actor_get_parameter_pointer(self, value);
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�����̔ԍ�
 * @return		�����̒l
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
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�����̔ԍ�
 * @return		�����̒l
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
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�߂�l
 */
void mana_actor_set_return_integer(mana_actor* self, const int32_t value)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_INTEGER;
	self->return_value.values.integer_value = value;
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�߂�l
 */
void mana_actor_set_return_float(mana_actor* self, const float value)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_FLOAT;
	self->return_value.values.float_value = value;
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�߂�l
 */
void mana_actor_set_return_string(mana_actor* self, const char* string)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_STRING;
	self->return_value.values.string_value = string;
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�߂�l
 */
void mana_actor_set_return_actor(mana_actor* self, mana_actor* actor)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_ACTOR;
	self->return_value.values.actor_value = actor;
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�߂�l
 */
void mana_actor_set_return_pointer(mana_actor* self, void* pointer)
{
	MANA_ASSERT(self);

	self->return_value.size = MANA_RETURN_VALUE_TYPE_ACTOR;
	self->return_value.values.pointer_value = pointer;
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	value	�߂�l
 * @param[in]	size	�߂�l�̃T�C�Y
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
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @return		���s���̃A�N�V������
 */
const char* mana_actor_get_action_name(const mana_actor* self)
{
	const char* action;

	MANA_ASSERT(self);

	action = self->interrupt[self->interrupt_level].action_name;

	return action ? action : "SLEEP!";
}

/*!
 * @warning 	�O���֐��ȊO����Ăяo���֎~
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @return		�O���֐���
 */
const char* mana_actor_get_function_name(const mana_actor* self)
{
	MANA_ASSERT(self);

	return mana_actor_get_string_from_memory(self, self->pc + 1);
}
#endif

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	action	�A�N�V������
 * @param[in]	address	�v���O�����A�h���X
 */
void mana_actor_set_action(mana_actor* self, const char* action, uint8_t* address)
{
	MANA_ASSERT(self);

	mana_hash_set(&self->actions, action, address);
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @return		mana �I�u�W�F�N�g
 */
mana* mana_actor_get_parent(const mana_actor* self)
{
	MANA_ASSERT(self);

	return self->parent;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @retval		false	����������
 * @retval		true	��������
 */
bool mana_actor_is_init(const mana_actor* self)
{
	MANA_ASSERT(self);

	return (self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_INITIALIZED) ? false : true;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @retval		true	�Ď��s���K�v
 * @retval		false	�Ď��s�͕s�v
 */
bool mana_actor_is_repeat(const mana_actor* self)
{
	MANA_ASSERT(self);

	return (self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_REPEAT) ? true : false;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @retval		true	���s��
 * @retval		false	��~
 */
bool mana_actor_is_running(const mana_actor* self)
{
	uint8_t flag;

	MANA_ASSERT(self);

	flag = self->flag & (MANA_ACTOR_FLAG_HALT|MANA_ACTOR_FLAG_RUNNING);
	return (flag == MANA_ACTOR_FLAG_RUNNING) ? true : false;
}

/*!
 * ���s�𒆒f���Ď��̃t���[���ɍĎ��s���܂��B
 * @param[in]	self				mana_actor �I�u�W�F�N�g
 * @param[in]	initial_complete	�����������t���O
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
 * ����t���[�����ɍĎ��s���܂��B
 * �܂��Avoid mana_actor_repeat(mana_actor* self, bool initial_complete)�ƈႢ���f�͂��܂���B
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 */
void mana_actor_again(mana_actor* self)
{
	MANA_ASSERT(self && self->parent);

	self->flag |= MANA_ACTOR_FLAG_REQUESTED;
	self->parent->flag |= MANA_FLAG_REQUESTED;
}

/*!
 * void mana_actor_stop(mana_actor* self)�̓��N�G�X�g���󂯕t���܂����A
 * void mana_actor_halt(mana_actor* self)�̓��N�G�X�g���󂯂߂܂���B
 * @param[in]	self	mana_actor �I�u�W�F�N�g
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
 * void mana_actor_halt(mana_actor* self)�̓��N�G�X�g���󂯕t���܂��񂪁A
 * void mana_actor_stop(mana_actor* self)�͎��s��~�̂ݍs�Ȃ��܂��B
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 */
void mana_actor_stop(mana_actor* self)
{
	MANA_ASSERT(self);

	self->flag &= ~MANA_ACTOR_FLAG_RUNNING;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 */
void mana_actor_yield(mana_actor* self)
{
	MANA_ASSERT(self);

	self->interrupt[self->interrupt_level].flag |= MANA_INTERRUPT_FLAG_SUSPEND;
}

/*!
 * ���N�G�X�g���󂯕t���܂�
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 */
void mana_actor_comply(mana_actor* self)
{
	MANA_ASSERT(self);

	self->flag &= ~MANA_ACTOR_FLAG_REFUSED;
}

/*!
 * ���N�G�X�g���󂯕t���܂���
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 */
void mana_actor_refuse(mana_actor* self)
{
	MANA_ASSERT(self);

	self->flag |= MANA_ACTOR_FLAG_REFUSED;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @return		���݂̗D��x
 */
int32_t mana_actor_get_interrupt_level(mana_actor* self)
{
	MANA_ASSERT(self);

	return self->interrupt_level;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @retval		true	������
 * @retval		false	�񓯊���
 */
bool mana_actor_is_synchronized(mana_actor* self)
{
	MANA_ASSERT(self);

	return (self->interrupt[self->interrupt_level].flag & MANA_INTERRUPT_FLAG_SYNCHRONIZED) ? true : false;
}

/*!
 * @param[in]	self			mana_actor �I�u�W�F�N�g
 * @param[in]	synchronized
 * - true�Ȃ�Γ������s
 * - false�Ȃ�Δ񓯊����s
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
 * @param[in]	self			mana_actor �I�u�W�F�N�g
 * @param[in]	level			�D��x
 * @param[in]	synchronized
 * - true�Ȃ�Γ������s
 * - false�Ȃ�Δ񓯊����s
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
 * @param[in]	self		mana_actor �I�u�W�F�N�g
 * @param[in]	resouce_id	���\�[�X�ԍ�
 * @param[out]	buffer		�f�[�^�A�h���X
 * @param[out]	size		�f�[�^�T�C�Y
 */
void mana_actor_get_data(const mana_actor* self, const int32_t resouce_id, const void** buffer, size_t* size)
{
	MANA_ASSERT(self);

	MANA_ASSERT(resouce_id >= 0 && resouce_id < mana_datalink_get_number_of_datas(&self->parent->datalink));
	*buffer = mana_datalink_get_data(&self->parent->datalink, resouce_id);
	*size = mana_datalink_get_data_size(&self->parent->datalink, resouce_id);
}

/*!
 * @return	�t�@�C���R�[���o�b�N�֐�
 */
mana_actor_file_callback* mana_actor_get_file_callback(void)
{
	return mana_async_file_callback;
}

/*!
 * @param[in]	function	�t�@�C���R�[���o�b�N�֐�
 */
void mana_actor_set_file_callback(mana_actor_file_callback* function)
{
	mana_async_file_callback = function;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @param[in]	level	�D��x
 * @return		�t�@�C���R�[���o�b�N�p�����[�^�[
 */
void* mana_actor_get_file_callback_parameter(mana_actor* self, int32_t level)
{
	MANA_ASSERT(self);
	MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);

	return self->interrupt[level].file_callback_parameter;
}

/*!
 * @param[in]	self		mana_actor �I�u�W�F�N�g
 * @param[in]	level		�D��x
 * @param[in]	parameter	�t�@�C���R�[���o�b�N�p�����[�^�[
 */
void mana_actor_set_file_callback_parameter(mana_actor* self, int32_t level, void* parameter)
{
	MANA_ASSERT(self);
	MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);

	self->interrupt[level].file_callback_parameter = parameter;
}

/*!
 * @param[in]	function	���N�G�X�g�R�[���o�b�N�֐�
 */
void mana_actor_set_request_callback(mana_actor_callback* function)
{
	mana_actor_request_callback = function;
}

/*!
 * @param[in]	self		mana_actor �I�u�W�F�N�g
 * @param[in]	parameter	���N�G�X�g�R�[���o�b�N�p�����[�^�[
 */
void mana_actor_set_request_callback_parameter(mana_actor* self, void* parameter)
{
	MANA_ASSERT(self);

	self->request_callback_parameter = parameter;
}

/*!
 * @param[in]	function	���[���o�b�N�R�[���o�b�N�֐�
 */
void mana_actor_set_rollback_callback(mana_actor_callback* function)
{
	mana_actor_rollback_callback = function;
}

/*!
 * @param[in]	function	���[���o�b�N�R�[���o�b�N�֐�
 * @param[in]	parameter	���[���o�b�N�R�[���o�b�N�p�����[�^�[
 */
void mana_actor_set_rollback_callback_parameter(mana_actor* self, void* parameter)
{
	MANA_ASSERT(self);

	self->rollback_callback_parameter = parameter;
}

/*!
 * @param[in]	self	mana_actor �I�u�W�F�N�g
 * @return		���[�U�[�f�[�^�[
 */
uint32_t mana_actor_get_user_data(const mana_actor* self)
{
	MANA_ASSERT(self);

	return self->user_data;
}

/*!
 * @param[in]	self		mana_actor �I�u�W�F�N�g
 * @param[in]	user_data	���[�U�[�f�[�^�[
 */
void mana_actor_set_user_data(mana_actor* self, uint32_t user_data)
{
	MANA_ASSERT(self);

	self->user_data = user_data;
}

/*!
 * @param[in]	self		mana_actor �I�u�W�F�N�g
 * @return		���[�U�[�|�C���^�[
 */
void* mana_actor_get_user_pointer(const mana_actor* self)
{
	MANA_ASSERT(self);

	return self->user_pointer;
}

/*!
 * @param[in]	self			mana_actor �I�u�W�F�N�g
 * @param[in]	user_pointer	���[�U�[�|�C���^�[
 */
void mana_actor_set_user_pointer(mana_actor* self, void* user_pointer)
{
	MANA_ASSERT(self);

	self->user_pointer = user_pointer;
}
