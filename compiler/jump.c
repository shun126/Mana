/*
 * mana (compiler)
 *
 * @file	mana_datalink_generator.h
 * @brief	break,continue,goto�Ȃǂ̃W�����v�Ɋւ���\�[�X�t�@�C��
 * @detail	���̃t�@�C����break,continue,goto�ȂǃW�����v�Ɋ֌W����\�[�X�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "code.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "jump.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "linker.h"
#endif

#define MANA_JUMP_CHAIN_TABLE_SIZE			(20)	/*!< �W�����v�`�F�C���e�[�u���T�C�Y */
#define MANA_JUMP_SWITCH_ENTRY_STACK_SIZE	(256)	/*!< switch�̍ő�l�X�g���x�� */
#define MANA_JUMP_SWITCH_STACK_SIZE			(256)	/*!< switch�̍ő�l�X�g���x�� */

/*! �W�����v�`�F�C���e�[�u�� */
static struct mana_jump_chain_table
{
	mana_jump_chain_status status;				/*!< �W�����v�`�F�C���̏�� */
	int32_t break_chain;						/*!< break�`�F�C���̈ʒu */
	int32_t continue_chain;						/*!< continue�`�F�C���̈ʒu */
	int32_t start_address;						/*!< �u���b�N�J�n�ʒu */
} mana_jump_chain_table[MANA_JUMP_CHAIN_TABLE_SIZE];

static int32_t mana_jump_chain_table_pointer;	/*!< mana_jump_chain_table �̈ʒu */

/*! switch�u���b�N���̃G���g���[ */
typedef struct mana_jump_switch_entry
{
	mana_node* node;							/*!< expression��\�� mana_node */
	int32_t address;							/*!< �A�h���X */
} mana_jump_switch_entry;

/*! switch�u���b�N���̃G���g���[�X�^�b�N */
mana_jump_switch_entry mana_jump_switch_entry_stack[MANA_JUMP_SWITCH_ENTRY_STACK_SIZE];

/*! switch�u���b�N���̃G���g���[�X�^�b�N�|�C���^�[ */
mana_jump_switch_entry* mana_jump_switch_entry_stack_pointer;

/*! switch�u���b�N�X�^�b�N */
struct mana_jump_switch_stack
{
	mana_jump_switch_entry*	stack_pointer;		/*!< switch�u���b�N���̃G���g���[�X�^�b�N */
	mana_type_description* type;				/*!< mana_type_description */
	int32_t default_address;					/*!< @biref default�ւ̃A�h���X */
} mana_jump_switch_stack[MANA_JUMP_SWITCH_STACK_SIZE];

/*! switch�u���b�N�X�^�b�N�|�C���^�[ */
int32_t mana_jump_switch_stack_pointer;

/*!
 */
void mana_jump_initialize(void)
{
	mana_jump_chain_table_pointer = 0;

	mana_jump_switch_stack_pointer = 0;
	mana_jump_switch_entry_stack_pointer = mana_jump_switch_entry_stack;
}

/*!
 */
void mana_jump_finalize(void)
{
}

/*!
 * @param[in]	status	mana_jump_chain_status
 */
void mana_jump_open_chain(mana_jump_chain_status status)
{
	++mana_jump_chain_table_pointer;
	mana_jump_chain_table[mana_jump_chain_table_pointer].status = status;
	mana_jump_chain_table[mana_jump_chain_table_pointer].break_chain = -1;
	mana_jump_chain_table[mana_jump_chain_table_pointer].continue_chain = -1;
	mana_jump_chain_table[mana_jump_chain_table_pointer].start_address = mana_code_get_pc();
}

/*!
 * @param[in]	new_pc	�V�����W�����v��
 * @return		���̃W�����v��
 */
int32_t mana_jump_break(int32_t new_pc)
{
	int32_t old_pc = -1;

	if(mana_jump_chain_table_pointer > 0)
	{
		old_pc = mana_jump_chain_table[mana_jump_chain_table_pointer].break_chain;
		mana_jump_chain_table[mana_jump_chain_table_pointer].break_chain = new_pc;
	}else{
		mana_compile_error("illegal use of break statement");
	}
	return old_pc;
}

/*!
 * @param[in]	new_pc	�V�����W�����v��
 * @return		���̃W�����v��
 */
int32_t mana_jump_continue(int32_t new_pc)
{
	int32_t i;
	int32_t old_pc = -1;

	for(i = mana_jump_chain_table_pointer; i > 0 && mana_jump_chain_table[i].status == MANA_JUMP_CHAIN_STATE_SWITCH; i--)
		;
	if (i > 0)
	{
		old_pc = mana_jump_chain_table[i].continue_chain;
		mana_jump_chain_table[i].continue_chain = new_pc;
	}else{
		mana_compile_error("illegal use of the continue");
	}
	return old_pc;
}

/*!
 */
void mana_jump_close_continue_only(void)
{
	mana_code_replace_all(
		mana_jump_chain_table[mana_jump_chain_table_pointer].continue_chain,
		mana_code_get_pc()
	);
	mana_jump_chain_table[mana_jump_chain_table_pointer].continue_chain = -1;
}

/*!
 */
void mana_jump_close_chain(void)
{
	mana_code_replace_all(
		mana_jump_chain_table[mana_jump_chain_table_pointer].continue_chain,
		mana_jump_chain_table[mana_jump_chain_table_pointer].start_address
	);
	mana_code_replace_all(
		mana_jump_chain_table[mana_jump_chain_table_pointer].break_chain,
		mana_code_get_pc()
	);
	mana_jump_chain_table_pointer --;
}

/*!
 * @param[in]	type	mana_type_description
 */
void mana_jump_open_switch(mana_type_description* type)
{
	mana_jump_switch_stack_pointer++;
	mana_jump_switch_stack[mana_jump_switch_stack_pointer].stack_pointer = mana_jump_switch_entry_stack_pointer;
	mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address = -1;
	mana_jump_switch_stack[mana_jump_switch_stack_pointer].type = type;
}

/*!
 * @param[in]	node	case�Ŕ�r����expression��\��mana_node
 */
void mana_jump_switch_case(mana_node* node)
{
	if(mana_jump_switch_stack_pointer > 0)
	{
		mana_jump_switch_entry *p;
		mana_jump_switch_entry_stack_pointer->node = node;
		for(p = mana_jump_switch_stack[mana_jump_switch_stack_pointer].stack_pointer; p->node != node; p++)
			;
		if(p >= mana_jump_switch_entry_stack_pointer)
		{
			mana_jump_switch_entry_stack_pointer->address = mana_code_get_pc();
			mana_jump_switch_entry_stack_pointer++;
		}
		else
		{
			mana_compile_error("duplicated case constant");
		}
	}
}

/*!
 */
void mana_jump_switch_default(void)
{
	if(mana_jump_switch_stack_pointer > 0 && mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address < 0)
	{
		mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address = mana_code_get_pc();
	}
	else
	{
		mana_compile_error("illegal default label");
	}
}

/*!
 */
void mana_jump_switch_build(void)
{
	mana_jump_switch_entry *p;

	for(p = mana_jump_switch_stack[mana_jump_switch_stack_pointer].stack_pointer; p < mana_jump_switch_entry_stack_pointer; p++)
	{
		int32_t size;

		switch(p->node->type->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_ACTOR:
			mana_code_set_opecode(MANA_IL_DUPLICATE);
			mana_generator_expression(p->node, false);
			mana_code_set_opecode(MANA_IL_COMPARE_EQ_INTEGER);
			mana_code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		case MANA_DATA_TYPE_FLOAT:
			mana_code_set_opecode(MANA_IL_DUPLICATE);
			mana_generator_expression(p->node, false);
			mana_code_set_opecode(MANA_IL_COMPARE_EQ_FLOAT);
			mana_code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		case MANA_DATA_TYPE_STRUCT:
			size = p->node->type->memory_size;
			mana_code_set_opecode_and_operand(MANA_IL_DUPLICATE_DATA, size);
			mana_generator_expression(p->node, false);
			mana_code_set_opecode_and_operand(MANA_IL_COMPARE_EQ_DATA, size);
			mana_code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		default:
			MANA_BUG("Illigal type");
			break;
		}
	}

	if(mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address > 0)
	{
		mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address);
	}

	mana_jump_switch_entry_stack_pointer = mana_jump_switch_stack[mana_jump_switch_stack_pointer].stack_pointer;
}

/*!
 */
void mana_jump_close_switch(void)
{
	switch(mana_jump_switch_stack[mana_jump_switch_stack_pointer].type->tcons)
	{
	case MANA_DATA_TYPE_CHAR:
	case MANA_DATA_TYPE_SHORT:
	case MANA_DATA_TYPE_INT:
	case MANA_DATA_TYPE_FLOAT:
	case MANA_DATA_TYPE_ACTOR:
		mana_code_set_opecode(MANA_IL_REMOVE);
		break;

	case MANA_DATA_TYPE_STRUCT:
		mana_code_set_opecode_and_operand(MANA_IL_REMOVE_DATA, mana_jump_switch_stack[mana_jump_switch_stack_pointer].type->memory_size);
		break;

	default:
		MANA_BUG("Illigal type");
		break;
	}

	mana_jump_switch_stack_pointer--;
}