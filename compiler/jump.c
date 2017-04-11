/*!
mana (compiler)

@file	jump.c
@brief	break,continue,gotoなどのジャンプに関するソースファイル
@detail	このファイルはbreak,continue,gotoなどジャンプに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "code.h"
#endif
#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#if !defined(___MANA_GENERATOR_H___)
#include "generator.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "jump.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "linker.h"
#endif

#define MANA_JUMP_CHAIN_TABLE_SIZE			(20)	/*!< ジャンプチェインテーブルサイズ */
#define MANA_JUMP_SWITCH_ENTRY_STACK_SIZE	(256)	/*!< switchの最大ネストレベル */
#define MANA_JUMP_SWITCH_STACK_SIZE			(256)	/*!< switchの最大ネストレベル */

/*! ジャンプチェインテーブル */
static struct mana_jump_chain_table
{
	mana_jump_chain_status status;				/*!< ジャンプチェインの状態 */
	int32_t break_chain;						/*!< breakチェインの位置 */
	int32_t continue_chain;						/*!< continueチェインの位置 */
	int32_t start_address;						/*!< ブロック開始位置 */
} mana_jump_chain_table[MANA_JUMP_CHAIN_TABLE_SIZE];

static int32_t mana_jump_chain_table_pointer;	/*!< mana_jump_chain_table の位置 */

/*! switchブロック内のエントリー */
typedef struct mana_jump_switch_entry
{
	node_entry* node;							/*!< expressionを表す node_entry */
	int32_t address;							/*!< アドレス */
} mana_jump_switch_entry;

/*! switchブロック内のエントリースタック */
mana_jump_switch_entry mana_jump_switch_entry_stack[MANA_JUMP_SWITCH_ENTRY_STACK_SIZE];

/*! switchブロック内のエントリースタックポインター */
mana_jump_switch_entry* mana_jump_switch_entry_stack_pointer;

/*! switchブロックスタック */
struct mana_jump_switch_stack
{
	mana_jump_switch_entry*	stack_pointer;		/*!< switchブロック内のエントリースタック */
	type_description* type;				/*!< type_description */
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
	mana_jump_chain_table[mana_jump_chain_table_pointer].start_address = code_get_pc();
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
	code_replace_all(
		mana_jump_chain_table[mana_jump_chain_table_pointer].continue_chain,
		code_get_pc()
	);
	mana_jump_chain_table[mana_jump_chain_table_pointer].continue_chain = -1;
}

/*!
 */
void mana_jump_close_chain(void)
{
	code_replace_all(
		mana_jump_chain_table[mana_jump_chain_table_pointer].continue_chain,
		mana_jump_chain_table[mana_jump_chain_table_pointer].start_address
	);
	code_replace_all(
		mana_jump_chain_table[mana_jump_chain_table_pointer].break_chain,
		code_get_pc()
	);
	mana_jump_chain_table_pointer --;
}

/*!
 * @param[in]	type	type_description
 */
void mana_jump_open_switch(type_description* type)
{
	mana_jump_switch_stack_pointer++;
	mana_jump_switch_stack[mana_jump_switch_stack_pointer].stack_pointer = mana_jump_switch_entry_stack_pointer;
	mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address = -1;
	mana_jump_switch_stack[mana_jump_switch_stack_pointer].type = type;
}

/*!
 * @param[in]	node	caseで比較するexpressionを表すnode_entry
 */
void mana_jump_switch_case(node_entry* node)
{
	if(mana_jump_switch_stack_pointer > 0)
	{
		mana_jump_switch_entry *p;
		mana_jump_switch_entry_stack_pointer->node = node;
		for(p = mana_jump_switch_stack[mana_jump_switch_stack_pointer].stack_pointer; p->node != node; p++)
			;
		if(p >= mana_jump_switch_entry_stack_pointer)
		{
			mana_jump_switch_entry_stack_pointer->address = code_get_pc();
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
		mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address = code_get_pc();
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
		case SYMBOL_DATA_TYPE_CHAR:
		case SYMBOL_DATA_TYPE_SHORT:
		case SYMBOL_DATA_TYPE_INT:
		case SYMBOL_DATA_TYPE_ACTOR:
			code_set_opecode(MANA_IL_DUPLICATE);
			generator_expression(p->node, false);
			code_set_opecode(MANA_IL_COMPARE_EQ_INTEGER);
			code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		case SYMBOL_DATA_TYPE_FLOAT:
			code_set_opecode(MANA_IL_DUPLICATE);
			generator_expression(p->node, false);
			code_set_opecode(MANA_IL_COMPARE_EQ_FLOAT);
			code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		case SYMBOL_DATA_TYPE_STRUCT:
			size = p->node->type->memory_size;
			code_set_opecode_and_operand(MANA_IL_DUPLICATE_DATA, size);
			generator_expression(p->node, false);
			code_set_opecode_and_operand(MANA_IL_COMPARE_EQ_DATA, size);
			code_set_opecode_and_operand(MANA_IL_BNE, p->address);
			break;

		default:
			MANA_BUG("Illigal type");
			break;
		}
	}

	if(mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address > 0)
	{
		code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_switch_stack[mana_jump_switch_stack_pointer].default_address);
	}

	mana_jump_switch_entry_stack_pointer = mana_jump_switch_stack[mana_jump_switch_stack_pointer].stack_pointer;
}

/*!
 */
void mana_jump_close_switch(void)
{
	switch(mana_jump_switch_stack[mana_jump_switch_stack_pointer].type->tcons)
	{
	case SYMBOL_DATA_TYPE_CHAR:
	case SYMBOL_DATA_TYPE_SHORT:
	case SYMBOL_DATA_TYPE_INT:
	case SYMBOL_DATA_TYPE_FLOAT:
	case SYMBOL_DATA_TYPE_ACTOR:
		code_set_opecode(MANA_IL_REMOVE);
		break;

	case SYMBOL_DATA_TYPE_STRUCT:
		code_set_opecode_and_operand(MANA_IL_REMOVE_DATA, mana_jump_switch_stack[mana_jump_switch_stack_pointer].type->memory_size);
		break;

	default:
		MANA_BUG("Illigal type");
		break;
	}

	mana_jump_switch_stack_pointer--;
}
