/*
 * mana (compiler)
 *
 * @file	mana_datalink_generator.h
 * @brief	break,continue,gotoなどのジャンプに関するソースファイル
 * @detail	このファイルはbreak,continue,gotoなどジャンプに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "mana_jump.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "mana_linker.h"
#endif

#define MANA_JUMP_CHAIN_TABLE_SIZE			(20)	/*!< ジャンプチェインテーブルサイズ */
#define MANA_JUMP_SWITCH_ENTRY_STACK_SIZE	(256)	/*!< switchの最大ネストレベル */
#define MANA_JUMP_SWITCH_STACK_SIZE			(256)	/*!< switchの最大ネストレベル */

/*! ジャンプチェインテーブル */
static struct mana_jump_chain_table
{
	mana_jump_chain_status status;			/*!< ジャンプチェインの状態 */
	int32_t break_chain;						/*!< breakチェインの位置 */
	int32_t continue_chain;						/*!< continueチェインの位置 */
	int32_t start_address;						/*!< ブロック開始位置 */
} mana_jump_chain_table[MANA_JUMP_CHAIN_TABLE_SIZE];

static int32_t mana_jump_chain_table_pointer;	/*!< mana_jump_chain_table の位置 */

/*! switchブロック内のエントリー */
typedef struct mana_jump_switch_entry
{
	mana_node* node;						/*!< expressionを表す mana_node */
	int32_t address;							/*!< アドレス */
} mana_jump_switch_entry;

/*! switchブロック内のエントリースタック */
mana_jump_switch_entry mana_jump_switch_entry_stack[MANA_JUMP_SWITCH_ENTRY_STACK_SIZE];

/*! switchブロック内のエントリースタックポインター */
mana_jump_switch_entry* mana_jump_switch_entry_stack_pointer;

/*! switchブロックスタック */
struct mana_jump_switch_stack
{
	mana_jump_switch_entry*	stack_pointer;	/*!< switchブロック内のエントリースタック */
	mana_type_description* type;			/*!< mana_type_description */
	int32_t default_address;					/*!< @biref defaultへのアドレス */
} mana_jump_switch_stack[MANA_JUMP_SWITCH_STACK_SIZE];

/*! switchブロックスタックポインター */
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
 * @param[in]	new_pc	新しいジャンプ先
 * @return		元のジャンプ先
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
 * @param[in]	new_pc	新しいジャンプ先
 * @return		元のジャンプ先
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
 * @param[in]	node	caseで比較するexpressionを表すmana_node
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
			mana_linker_expression(p->node, false);
			mana_code_set_opecode(MANA_IL_COMPARE_EQ_INTEGER);
			mana_code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		case MANA_DATA_TYPE_FLOAT:
			mana_code_set_opecode(MANA_IL_DUPLICATE);
			mana_linker_expression(p->node, false);
			mana_code_set_opecode(MANA_IL_COMPARE_EQ_FLOAT);
			mana_code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		case MANA_DATA_TYPE_STRUCT:
			size = p->node->type->memory_size;
			mana_code_set_opecode_and_operand(MANA_IL_DUPLICATE_DATA, size);
			mana_linker_expression(p->node, false);
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
