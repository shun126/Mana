/*
 * mana (compiler)
 *
 * @file	mana_link.c
 * @brief	リンクに関するソースファイル
 * @detail	このファイルはリンクに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "mana_linker.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_NODE_H___)
#include "mana_node.h"
#endif
#if !defined(___MANA_REGISTER_H___)
#include "mana_register.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
#endif

static void mana_linker_call(mana_node* node);
static int32_t mana_linker_condition_core(mana_node* node);
static mana_hash* mana_node_event_hash = NULL;

void mana_linker_initialize(void)
{
	mana_node_event_hash = mana_hash_create();
}

void mana_linker_finalize(void)
{
	mana_hash_destroy(mana_node_event_hash);
	mana_node_event_hash = NULL;
}

/*!
* 関数毎に特別な処理を行なうためのコールバック関数を登録します
* @param[in]	name		関数名
* @param[in]	function	コールバック関数
*/
void mana_node_add_event(char* name, mana_node_event_funtion_type function)
{
	mana_hash_set(mana_node_event_hash, name, function);
}


/*!
 * スタックにあるアドレスからデータを読み込む命令を出力する
 * @param[in]	読み込むmana_type_description
 */
static void mana_linker_generate_load(mana_node* node)
{
//	mana_register_entity* register_entity;

	MANA_ASSERT(node);
	MANA_ASSERT(node->type);
#if 0
	/* TODO:テスト */
	register_entity = mana_register_allocate(node->symbol, node->symbol->address);
	if(register_entity == NULL)
	{
		mana_register_entity* register_entity = mana_register_allocate(node->symbol, node->symbol->address);
		if(register_entity == NULL)
		{
			/* 割り当てるレジスタが無い場合はspillを行う */
		}
		/* レジスタに空きがある、または既に割り当て済みの場合 */
	}
#endif
	switch(node->type->tcons)
	{
	case MANA_DATA_TYPE_CHAR:
		mana_code_set_opecode(MANA_IL_LOAD_CHAR);
		break;

	case MANA_DATA_TYPE_SHORT:
		mana_code_set_opecode(MANA_IL_LOAD_SHORT);
		break;

	case MANA_DATA_TYPE_INT:
		mana_code_set_opecode(MANA_IL_LOAD_INTEGER);
		break;

	case MANA_DATA_TYPE_FLOAT:
		mana_code_set_opecode(MANA_IL_LOAD_FLOAT);
		break;

	case MANA_DATA_TYPE_REFERENCE:
		mana_code_set_opecode(MANA_IL_LOAD_REFFRENCE);
		break;

	case MANA_DATA_TYPE_ACTOR:
		if(node->type == mana_type_get(MANA_DATA_TYPE_ACTOR))
			mana_code_set_opecode(MANA_IL_LOAD_REFFRENCE);
		else
			mana_code_set_opecode_and_operand(MANA_IL_PUSH_ACTOR, mana_data_set(node->type->name));
		break;

	case MANA_DATA_TYPE_ARRAY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);

	case MANA_DATA_TYPE_STRUCT:
		mana_code_set_opecode_and_operand(MANA_IL_LOAD_DATA, node->type->memory_size);
		break;

	default:
		MANA_BUG("illigal data type");
	}
}

/*!
 * スタックにあるアドレスへデータを書き込む命令を出力する
 * @param[in]	読み込むmana_type_description
 */
static void mana_linker_generate_store(mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(node->type);

	switch(node->type->tcons)
	{
	case MANA_DATA_TYPE_CHAR:
		mana_code_set_opecode(MANA_IL_STORE_CHAR);
		break;

	case MANA_DATA_TYPE_SHORT:
		mana_code_set_opecode(MANA_IL_STORE_SHORT);
		break;

	case MANA_DATA_TYPE_INT:
		mana_code_set_opecode(MANA_IL_STORE_INTEGER);
		break;

	case MANA_DATA_TYPE_FLOAT:
		mana_code_set_opecode(MANA_IL_STORE_FLOAT);
		break;

	case MANA_DATA_TYPE_REFERENCE:
	case MANA_DATA_TYPE_ACTOR:
		mana_code_set_opecode(MANA_IL_STORE_REFFRENCE);
		break;

	case MANA_DATA_TYPE_ARRAY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);

	case MANA_DATA_TYPE_STRUCT:
		mana_code_set_opecode_and_operand(MANA_IL_STORE_DATA, node->type->memory_size);
		break;

	default:
		mana_compile_error("illegal type stored");
		break;
	}
}

static void mana_linker_auto_cast(mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(node->left);
	MANA_ASSERT(node->right);

	if(node->left == NULL || node->left->type == NULL)
	{
	mana_compile_error("illegal left-hand side expression");
	return;
	}

	if(node->right != NULL && node->right->type == NULL)
	{
	mana_compile_error("illegal right-hand side expression");
	return;
	}

	mana_symbol_data_type_id t1 = (node->left->type)->tcons;
	mana_symbol_data_type_id t2 = node->right ? (node->right->type)->tcons : MANA_DATA_TYPE_INT;

	switch (node->id)
	{
	case MANA_NODE_ASSIGN:
		if (node->left->id == MANA_NODE_CONST)
		{
			mana_compile_error("already initialized constant '%s'", node->left->symbol->name);
			return;
		}
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		break;

	case MANA_NODE_ADD:
	case MANA_NODE_SUB:
	case MANA_NODE_MUL:
	case MANA_NODE_DIV:
	case MANA_NODE_REM:
	case MANA_NODE_POW:
		mana_node_auto_cast(node);
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		mana_type_compatible(node->left->type, node->right->type);
		break;

	case MANA_NODE_GT:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch (t1)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_ACTOR:
			node->etc = MANA_IL_COMPARE_GT_INTEGER;
			break;
		case MANA_DATA_TYPE_FLOAT:
			node->etc = MANA_IL_COMPARE_GT_FLOAT;
			break;
		case MANA_DATA_TYPE_STRUCT:
			node->etc = MANA_IL_COMPARE_GT_DATA;
			break;
		default:
			MANA_BUG("illigal data type");
		}
		node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
		break;

	case MANA_NODE_GE:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch (t1)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_ACTOR:
			node->etc = MANA_IL_COMPARE_GE_INTEGER;
			break;
		case MANA_DATA_TYPE_FLOAT:
			node->etc = MANA_IL_COMPARE_GE_FLOAT;
			break;
		case MANA_DATA_TYPE_STRUCT:
			node->etc = MANA_IL_COMPARE_GE_DATA;
			break;
		default:
			MANA_BUG("illigal data type");
		}
		node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
		break;

	case MANA_NODE_EQ:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch (t1)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_ACTOR:
			node->etc = MANA_IL_COMPARE_EQ_INTEGER;
			break;
		case MANA_DATA_TYPE_FLOAT:
			node->etc = MANA_IL_COMPARE_EQ_FLOAT;
			break;
		case MANA_DATA_TYPE_STRUCT:
			node->etc = MANA_IL_COMPARE_EQ_DATA;
			break;
		default:
			MANA_BUG("illigal data type");
		}
		node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
		break;

	case MANA_NODE_NE:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch (t1)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_ACTOR:
			node->etc = MANA_IL_COMPARE_NE_INTEGER;
			break;
		case MANA_DATA_TYPE_FLOAT:
			node->etc = MANA_IL_COMPARE_NE_FLOAT;
			break;
		case MANA_DATA_TYPE_STRUCT:
			node->etc = MANA_IL_COMPARE_NE_DATA;
			break;
		default:
			MANA_BUG("illigal data type");
		}
		node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
		break;

	case MANA_NODE_LS:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch (t1)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_ACTOR:
			node->etc = MANA_IL_COMPARE_LS_INTEGER;
			break;
		case MANA_DATA_TYPE_FLOAT:
			node->etc = MANA_IL_COMPARE_LS_FLOAT;
			break;
		case MANA_DATA_TYPE_STRUCT:
			node->etc = MANA_IL_COMPARE_LS_DATA;
			break;
		default:
			MANA_BUG("illigal data type");
		}
		node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
		break;

	case MANA_NODE_LE:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch (t1)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_ACTOR:
			node->etc = MANA_IL_COMPARE_LE_INTEGER;
			break;
		case MANA_DATA_TYPE_FLOAT:
			node->etc = MANA_IL_COMPARE_LE_FLOAT;
			break;
		case MANA_DATA_TYPE_STRUCT:
			node->etc = MANA_IL_COMPARE_LE_DATA;
			break;
		default:
			MANA_BUG("illigal data type");
		}
		node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
		break;

	case MANA_NODE_NEG:
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_NOT:
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_NOT;
		break;

	case MANA_NODE_LNOT:
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_LNOT;
		break;

	case MANA_NODE_I2F:
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_F2I:
		if (t1 != MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_SENDER:
	case MANA_NODE_SELF:
	case MANA_NODE_PRIORITY:
	case MANA_NODE_EXPRESSION_IF:
		break;

	case MANA_NODE_AND:
	case MANA_NODE_OR:
	case MANA_NODE_XOR:
	case MANA_NODE_LSH:
	case MANA_NODE_RSH:
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_LAND:
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_LAND;
		break;

	case MANA_NODE_LOR:
		if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_LOR;
		break;

	case MANA_NODE_ARRAY:
		if (t2 == MANA_DATA_TYPE_VOID || t2 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("non-integer expression used as subscript");
		if (t1 != MANA_DATA_TYPE_ARRAY)
			mana_compile_error("subscript specified to non-array");

		if (node->right->id == MANA_NODE_CONST)
		{
			assert(
				node->right->type == mana_type_get(MANA_DATA_TYPE_CHAR) ||
				node->right->type == mana_type_get(MANA_DATA_TYPE_SHORT) ||
				node->right->type == mana_type_get(MANA_DATA_TYPE_INT));

			if (node->right->digit >= (node->left->type)->number_of_elements)
				mana_compile_error("subscript range over");
		}

		node->type = (node->left->type)->component;
		break;

	case MANA_NODE_CALL:
		{
			mana_symbol_entry* sp = node->left->symbol;

			mana_node_event_funtion_type* function = mana_hash_get(mana_node_event_hash, sp->name);
			if (function)
			{
				node = (*function)(node);
			}

			if (sp->class_type != MANA_CLASS_TYPE_FUNCTION &&
				sp->class_type != MANA_CLASS_TYPE_NATIVE_FUNCTION &&
				sp->class_type != MANA_CLASS_TYPE_MEMBER_FUNCTION
				)	mana_compile_error("trying to call non-funcation");
		}
		break;

	case MANA_NODE_ARGUMENT:
		node->etc = (node->left->id == MANA_NODE_ARGUMENT) ? node->left->etc + 1 : 2;
		break;

	case MANA_NODE_STRING:
		break;

	default:
		//MANA_BUG("Illgal node detect\n");
		break;
	}
}

/*!
ノードを辿りながら、内部中間言語に翻訳します
@param	tree			ノードポインタ
@param	enable_load		trueならばload命令は有効、falseならばload命令は無効
*/
void mana_linker_generate_code(mana_node* node, int32_t enable_load)
{
	bool call_next_node = true;

	if(!node)
		return;

	switch(node->id)
	{
	case MANA_NODE_ASSIGN:
		mana_linker_generate_code(node->right, enable_load);
		mana_linker_generate_code(node->left, false);
		mana_linker_generate_store(node->left);
		break;

	case MANA_NODE_INCOMPLETE:
	case MANA_NODE_FUNCTION:
		mana_linker_generate_code(node->right, enable_load);
		mana_linker_generate_code(node->left, enable_load);
		break;

	case MANA_NODE_VARIABLE:
		/* variable */
		mana_linker_generate_code(node->right, enable_load);
		mana_linker_generate_code(node->left, enable_load);

		switch((node->symbol)->class_type)
		{
		case MANA_CLASS_TYPE_VARIABLE_STATIC:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_STATIC_ADDRESS, (node->symbol)->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_GLOBAL:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_GLOBAL_ADDRESS, (node->symbol)->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_ACTOR:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_SELF_ADDRESS, (node->symbol)->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_LOCAL:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_FRAME_ADDRESS, (node->symbol)->address);
			break;

		case MANA_CLASS_TYPE_TYPEDEF:
			break;

		default:
			mana_compile_error("illegal type of expression in statement");
			break;
		}

		if(enable_load)
		{
			mana_linker_generate_load(node);
		}

		break;

	case MANA_NODE_ARRAY:
		/* variable[index] */
		mana_linker_generate_code(node->right, true);
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, (node->type)->memory_size);
		mana_code_set_opecode(MANA_IL_MUL_INTEGER);
		mana_linker_generate_code(node->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if(enable_load)
		{
			mana_linker_generate_load(node);
		}
		break;

	case MANA_NODE_MEMOP:
		/* variable.member */
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, node->etc);
		mana_linker_generate_code(node->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if(enable_load)
		{
			mana_linker_generate_load(node);
		}
		break;

	case MANA_NODE_CONST:
		switch(node->type->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
			if(node->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_CHAR);
				mana_code_set_char((int8_t)node->digit);
			}
			break;

		case MANA_DATA_TYPE_SHORT:
			if(node->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_SHORT);
				mana_code_set_short((int16_t)node->digit);
			}
			break;

		case MANA_DATA_TYPE_INT:
			if(node->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_INTEGER);
				mana_code_set_integer(node->digit);
			}
			break;

		case MANA_DATA_TYPE_FLOAT:
			if(node->real == 0.0f)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_FLOAT);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_FLOAT);
				mana_code_set_float(node->real);
			}
			break;

		case MANA_DATA_TYPE_NIL:
			mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			break;

		default:
			mana_compile_error("illegal type of expression in statement");
			break;
		}
		break;

	case MANA_NODE_STRING:
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_STRING, node->digit);
		break;

	case MANA_NODE_ADD:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_ADD_FLOAT : MANA_IL_ADD_INTEGER);
		break;

	case MANA_NODE_SUB:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		break;

	case MANA_NODE_MUL:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MUL_FLOAT : MANA_IL_MUL_INTEGER);
		break;

	case MANA_NODE_DIV:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_DIV_FLOAT : MANA_IL_DIV_INTEGER);
		break;

	case MANA_NODE_REM:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MOD_FLOAT : MANA_IL_MOD_INTEGER);
		break;

	case MANA_NODE_POW:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		break;

	case MANA_NODE_AND:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_AND);
		break;

	case MANA_NODE_OR:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_OR);
		break;

	case MANA_NODE_XOR:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_EOR);
		break;

	case MANA_NODE_LSH:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHL);
		break;

	case MANA_NODE_RSH:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHR);
		break;

	case MANA_NODE_NEG:
		mana_linker_generate_code(node->left, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MINUS_FLOAT : MANA_IL_MINUS_INTEGER);
		break;

	case MANA_NODE_I2F:
		mana_linker_generate_code(node->left, enable_load);
		mana_code_set_opecode(MANA_IL_INT2FLOAT);
		break;

	case MANA_NODE_F2I:
		mana_linker_generate_code(node->left, enable_load);
		mana_code_set_opecode(MANA_IL_FLOAT2INT);
		break;

	case MANA_NODE_LS:
	case MANA_NODE_LE:
	case MANA_NODE_EQ:
	case MANA_NODE_NE:
	case MANA_NODE_GE:
	case MANA_NODE_GT:
	case MANA_NODE_LAND:
	case MANA_NODE_LOR:
	case MANA_NODE_LNOT:
	case MANA_NODE_NOT:
		/* 比較、論理演算子 */
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((uint8_t)node->etc);
		break;

	case MANA_NODE_CALL:
		/* 関数、アクションを呼びます */
		mana_linker_call(node);
		break;

	case MANA_NODE_SENDER:
		/* senderをpushします */
		mana_code_set_opecode(MANA_IL_PUSH_SENDER);
		break;

	case MANA_NODE_SELF:
		/* selfをpushします */
		mana_code_set_opecode(MANA_IL_PUSH_SELF);
		break;

	case MANA_NODE_PRIORITY:
		/* priorityをpushします */
		mana_code_set_opecode(MANA_IL_PUSH_PRIORITY);
		break;

	case MANA_NODE_EXPRESSION_IF:
		/* 三項演算子 */
		{
			int32_t pc1, pc2;
			mana_linker_condition_core(node->next);
			pc1 = mana_code_set_opecode_and_operand(MANA_IL_BEQ, -1);
			mana_linker_generate_code(node->left, enable_load);
			pc2 = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
			mana_code_replace_all(pc1, mana_code_get_pc());
			mana_linker_generate_code(node->right, enable_load);
			mana_code_replace_all(pc2, mana_code_get_pc());
		}
		break;

	case MANA_NODE_BLOCK:
	case MANA_NODE_IF:
	case MANA_NODE_SWITCH:
	case MANA_NODE_CASE:
	case MANA_NODE_DEFAULT:
	case MANA_NODE_WHILE:
	case MANA_NODE_DO:
	case MANA_NODE_FOR:
	case MANA_NODE_LOOP:
	case MANA_NODE_LOCK:
	case MANA_NODE_GOTO:
	case MANA_NODE_LABEL:
	case MANA_NODE_BREAK:
	case MANA_NODE_CONTINUE:

	case MANA_NODE_IDENTIFIER:
	case MANA_NODE_TYPE_DESCRIPTION:
	case MANA_NODE_DECLARATOR:

	case MANA_NODE_VARIABLE_DECLARATION:
	case MANA_NODE_SIZEOF:

	case MANA_NODE_DECLARE_ACTOR:
	case MANA_NODE_DECLARE_PHANTOM:
	case MANA_NODE_DECLARE_MODULE:
	case MANA_NODE_DECLARE_ACTION:
	case MANA_NODE_DECLARE_EXTEND:
	case MANA_NODE_DECLARE_ALLOCATE:
	case MANA_NODE_DECLARE_STATIC:
	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
	case MANA_NODE_DECLARE_VALIABLE:
	case MANA_NODE_DECLARE_FUNCTION:

	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		break;

		// 特に処理を行わないノード
	case MANA_NODE_NEWLINE:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		break;

		// 定義を行わないノード
	case MANA_NODE_DECLARE_ALIAS:
	case MANA_NODE_DECLARE_STRUCT:
	case MANA_NODE_DEFINE_CONSTANT:
	case MANA_NODE_UNDEFINE_CONSTANT:
		break;

	default:
		mana_compile_error("illegal right-hand side value");
		break;
	}

	if (call_next_node)
		mana_linker_generate_code(node->next, enable_load);
}

// TODO
static mana_symbol_entry* mana_actor_symbol_entry_pointer = NULL;
static mana_symbol_entry* mana_function_symbol_entry_pointer = NULL;
static bool mana_static_block_opend = false;

/*!
ノードを辿りながら、シンボル情報を登録します
@param	node			ノードポインタ
*/
void mana_linker_generate_symbol(mana_node* node, mana_node* parent_node)
{
	bool call_next_node = true;

	if (node == NULL)
		return;

	switch (node->id)
	{
	case MANA_NODE_IDENTIFIER:
	case MANA_NODE_SIZEOF:
	case MANA_NODE_DECLARE_VALIABLE:
	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
		mana_linker_generate_symbol(node->left, node);
		mana_linker_generate_symbol(node->right, node);
		break;

	case MANA_NODE_DECLARE_ACTOR:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_linker_generate_symbol(node->left, node);
			mana_symbol_set_type(node->string, mana_symbol_close_actor(node->string, NULL, NULL, false));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_linker_generate_symbol(node->left, node);
			mana_symbol_set_type(node->string, mana_symbol_close_actor(node->string, NULL, NULL, true));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_MODULE:
		{
			mana_symbol_open_module();
			mana_linker_generate_symbol(node->left, node);
			mana_symbol_set_type(node->string, mana_symbol_close_module(node->string));
		}
		break;

	case MANA_NODE_DECLARE_ACTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = mana_type_get(MANA_DATA_TYPE_VOID);
			mana_function_symbol_entry_pointer = node->symbol;

			/*
			mana_symbol_open_function(true, mana_function_symbol_entry_pointer, mana_type_get(MANA_DATA_TYPE_VOID));

			// node->leftは mana_linker_generate_code で処理します

			mana_symbol_close_function(mana_function_symbol_entry_pointer);
			mana_function_symbol_entry_pointer = NULL;
			*/
		}
		break;

	case MANA_NODE_DECLARE_EXTEND:
		mana_symbol_extend_module(node->string);
		break;

	case MANA_NODE_DECLARE_STRUCT:
		mana_symbol_open_structure();
		mana_linker_generate_symbol(node->left, node);
		mana_symbol_set_type(node->string, mana_symbol_close_structure(node->string));
		break;

	case MANA_NODE_DECLARE_ALLOCATE:
		{
			const int32_t mana_allocated_size = mana_symbol_get_static_memory_address() + node->digit;

			mana_linker_generate_symbol(node->left, node);

			const int32_t address = mana_symbol_get_static_memory_address();
			if (address >= mana_allocated_size)
			{
				mana_compile_error("static variable range over");
			}
			mana_symbol_set_static_memory_address(mana_allocated_size);
		}
		break;

	case MANA_NODE_DECLARE_STATIC:
		mana_static_block_opend = true;
		mana_linker_generate_symbol(node->left, node);
		mana_static_block_opend = false;
		break;

	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		{
			mana_symbol_entry* symbol = mana_symbol_create_function(node->string);
			symbol->number_of_parameters = node->digit;
			mana_symbol_open_native_function();
			mana_linker_generate_symbol(node->left, node);
			mana_symbol_close_native_function(symbol, node->left->type);
		}
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);

			mana_linker_generate_symbol(node->left, node);
			node->symbol->type = node->left->type;

			//mana_linker_generate_symbol(node->left, node);
			//mana_function_symbol_entry_pointer = $2;

			//mana_symbol_open_function(false, symbol, node->left->type);
			// node->rightは mana_linker_generate_code で処理します
			//mana_symbol_close_function(symbol);
		}
		break;

	case MANA_NODE_VARIABLE_DECLARATION:
		mana_linker_generate_symbol(node->left, node); // MANA_NODE_TYPE_DESCRIPTION
		mana_linker_generate_symbol(node->right, node);// MANA_NODE_DECLARATOR
		mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		if (node->type == NULL)
		{
			mana_symbol_entry* symbol = mana_symbol_lookup(node->string);
			if (symbol)
			{
				node->type =symbol->type;
			}
			else
			{
				mana_compile_error("incomplete type name %s", node->string);
				node->type = mana_type_get(MANA_DATA_TYPE_INT);
			}
		}
		break;

	case MANA_NODE_DECLARATOR:
		//| tIDENTIFIER variable_sizes
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, /*mana_static_block_opend*/false);
		//mana_symbol_create_identification(node->string, $2, /*mana_static_block_opend*/false);
		break;

	case MANA_NODE_DECLARE_ALIAS:
		mana_symbol_create_alias(node->string, node->string);
		break;

	case MANA_NODE_DEFINE_CONSTANT:
		mana_symbol_create_const_int(node->string, node->digit);
		break;

	case MANA_NODE_UNDEFINE_CONSTANT:
		mana_symbol_destroy(node->string);
		break;

		// 特に処理を行わないノード
	case MANA_NODE_NEWLINE:
		mana_linker_generate_symbol(node->left, node);
		mana_linker_generate_symbol(node->right, node);
		break;

		// 宣言を行わないノード
	case MANA_NODE_ARRAY:
	case MANA_NODE_ASSIGN:
	case MANA_NODE_MEMOP:
	case MANA_NODE_ARGUMENT:
	case MANA_NODE_CONST:
	case MANA_NODE_VARIABLE:
	case MANA_NODE_INCOMPLETE:
	case MANA_NODE_FUNCTION:
	case MANA_NODE_CALL:
	case MANA_NODE_ADD:
	case MANA_NODE_SUB:
	case MANA_NODE_MUL:
	case MANA_NODE_DIV:
	case MANA_NODE_REM:
	case MANA_NODE_NEG:
	case MANA_NODE_POW:
	case MANA_NODE_NOT:
	case MANA_NODE_AND:
	case MANA_NODE_OR:
	case MANA_NODE_XOR:
	case MANA_NODE_LSH:
	case MANA_NODE_RSH:
	case MANA_NODE_LS:
	case MANA_NODE_LE:
	case MANA_NODE_EQ:
	case MANA_NODE_NE:
	case MANA_NODE_GE:
	case MANA_NODE_GT:
	case MANA_NODE_STRING:
	case MANA_NODE_I2F:
	case MANA_NODE_F2I:
	case MANA_NODE_LOR:
	case MANA_NODE_LAND:
	case MANA_NODE_LNOT:
	case MANA_NODE_HALT:
	case MANA_NODE_YIELD:
	case MANA_NODE_REQUEST:
	case MANA_NODE_COMPLY:
	case MANA_NODE_REFUSE:
	case MANA_NODE_JOIN:
	case MANA_NODE_SENDER:
	case MANA_NODE_SELF:
	case MANA_NODE_PRIORITY:
	case MANA_NODE_EXPRESSION_IF:
	case MANA_NODE_PRINT:
	case MANA_NODE_RETURN:
	case MANA_NODE_ROLLBACK:
	case MANA_NODE_BLOCK:
	case MANA_NODE_IF:
	case MANA_NODE_SWITCH:
	case MANA_NODE_CASE:
	case MANA_NODE_DEFAULT:
	case MANA_NODE_WHILE:
	case MANA_NODE_DO:
	case MANA_NODE_FOR:
	case MANA_NODE_LOOP:
	case MANA_NODE_LOCK:
	case MANA_NODE_GOTO:
	case MANA_NODE_LABEL:
	case MANA_NODE_BREAK:
	case MANA_NODE_CONTINUE:
		break;

	default:
		abort();
	}

	if (call_next_node)
		mana_linker_generate_symbol(node->next, node);
}


/*!
 * returnの処理
 * @param	func	returnが所属する関数のmana_symbol_entry
 * @param	tree	return文のmana_node
 */
void mana_linker_return(mana_symbol_entry* func, mana_node* tree)
{
	mana_type_description* type;

	type = func->type;

	/* エラーチェック */
	if(type->tcons == MANA_DATA_TYPE_VOID)
	{
		if(tree != NULL)
			mana_compile_error("meaningless return value specification");
	}
	else if(tree == NULL)
	{
		mana_compile_error("missing return value");
	}
	else
	{
		/* 自動キャスト */
		tree = mana_node_cast(type, tree);

		/* 型の検証 */
		mana_type_compatible(type, tree->type);

		/* ノードの評価 */
		mana_symbol_open_block(false);
		mana_linker_generate_code(tree, true);
		mana_symbol_close_block();

		/* ノードの解放 */
		mana_node_release(tree);
	}

	/* 関数の最後にジャンプ */
	mana_symbol_return_address_list = mana_code_set_opecode_and_operand(
		MANA_IL_BRA, mana_symbol_return_address_list);

	/* 関数を使用したフラグを立てる */
	func->used = true;
}

/*!
 * rollbackの処理
 * @param	tree	rollback文のmana_node
 */
void mana_linker_rollback(mana_node* tree)
{
	if(tree)
	{
		/* ノードの評価 */
		mana_symbol_open_block(false);
		mana_linker_generate_code(tree, true);
		mana_symbol_close_block();

		/* ノードの解放 */
		mana_node_release(tree);
	}
	mana_code_set_opecode(MANA_IL_ROLLBACK);
}

/*!
 * 関数の引数のサイズを調べます
 * @param	address	アドレス
 * @param	param	引数のmana_symbol_entry
 * @param	arg		引数のmana_node
 * @return	引数のサイズ
 */
static int32_t mana_linker_call_argument_size(int32_t address, mana_symbol_entry* param, mana_node* arg)
{
	if(param && arg)
	{
		address += mana_linker_call_argument_size(address, param->next, arg->left);

		if(arg->id == MANA_NODE_ARGUMENT)
			arg = arg->right;

		address += (mana_node_get_memory_size(arg) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
	}
	return address;
}

/*!
 * 関数の引数を評価します
 * @param	address	引数のアドレス
 * @param	param	引数のmana_symbol_entry
 * @param	arg		引数のmana_node
 * @return	引数のアドレス
 */
static int32_t mana_linker_call_argument(int32_t address, mana_symbol_entry* param, mana_node* arg)
{
	if(param && arg)
	{
		address = mana_linker_call_argument(address, param->next, arg->left);

		mana_code_set_short((int16_t)address);

		if(arg->id == MANA_NODE_ARGUMENT)
			arg = arg->right;
		address -= (mana_node_get_memory_size(arg) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
	}
	return address;
}

/*!
 * 関数呼び出しのノードを評価します
 * @param	関数呼び出しのmana_node
 */
static void mana_linker_call(mana_node* tree)
{
	mana_linker_auto_cast(tree);

	mana_node* node = tree->left;
	mana_node* argument = tree->right;
	int32_t argument_counter = mana_linker_argument((node->symbol)->parameter_list, argument);

	/* エラーチェック */
	if((node->symbol)->number_of_parameters != argument_counter)
	{
		/* 引数の数が一致しない */
		mana_compile_error("unmatched argument.");
	}
	else if((node->symbol)->class_type == MANA_CLASS_TYPE_NATIVE_FUNCTION)
	{
		/* 外部関数の処理 */
		int32_t argument_size = mana_linker_call_argument_size(0, (node->symbol)->parameter_list, argument);

		node->symbol->address = mana_data_set(node->symbol->name);

		mana_code_set_opecode_and_operand((uint8_t)MANA_IL_CALL, (node->symbol)->address);
		mana_code_set_unsigned_char((uint8_t)((node->symbol)->type->tcons != MANA_DATA_TYPE_VOID));
		mana_code_set_unsigned_char((uint8_t)argument_counter);
		mana_code_set_unsigned_short((uint16_t)argument_size);
		mana_linker_call_argument(argument_size - 1, (node->symbol)->parameter_list, argument);
	}
	else
	{
		/* 内部関数の処理 */
		mana_code_set_opecode_and_operand((uint8_t)MANA_IL_BSR, (node->symbol)->address);
	}
}

/*!
 * print文の処理
 * @param	print文のmana_node
 */
void mana_linker_call_print(mana_node* tree)
{
	int32_t argument_counter = 0;

	for(mana_node* node = tree; node; node = node->left)
	{
		mana_linker_generate_code((node->id == MANA_NODE_ARGUMENT) ? node->right : node, true);
		argument_counter ++;
	}

	mana_node_release(tree);

	mana_code_set_opecode_and_operand((uint8_t)MANA_IL_PRINT, argument_counter);
}

/*!
 * 引数の出力（再帰呼び出し）
 * @param	count	引数の番号
 * @param	param	引数のmana_symbol_entry
 * @param	arg		引数のmana_node
 * @return	引数の数
 */
static int32_t mana_linker_generate_argument(int32_t count, mana_symbol_entry* param, mana_node* arg)
{
	if(param && arg)
	{
		if(arg->id == MANA_NODE_ARGUMENT)
		{
			count = mana_linker_generate_argument(count, param->next, arg->left);
			arg = arg->right;
		}
		arg = mana_node_cast(param->type, arg);
		mana_type_compatible(param->type, arg->type);
		mana_linker_generate_code(arg, true);
	}
	if(arg)
		count ++;
	return count;
}

/*!
 * 引数の出力
 * @param	param	引数のmana_symbol_entry
 * @param	arg		引数のmana_node
 * @return	引数の数
 */
int32_t mana_linker_argument(mana_symbol_entry* param, mana_node* arg)
{
	return mana_linker_generate_argument(0, param, arg);
}

/*!
 * 式の評価
 * @param	tree			式のmana_node
 * @param	enable_assign	trueならば代入式、falseならばそれ以外
 */
void mana_linker_expression(mana_node* tree, int32_t enable_assign)
{
	if(!tree)
		return;

	mana_symbol_open_block(false);

	if(enable_assign)
	{
		if(tree->id != MANA_NODE_ASSIGN && tree->id != MANA_NODE_CALL && tree->id != MANA_NODE_F2I)
			mana_compile_error("illegal expression in write-statement");
	}else{
		if(tree->id == MANA_NODE_ASSIGN)
			mana_compile_error("illegal expression in write-statement");
	}

	mana_linker_generate_code(tree, true);

	if(enable_assign && tree->id == MANA_NODE_CALL && tree->type)
	{
		switch(tree->type->tcons)
		{
		case MANA_DATA_TYPE_VOID:
			break;

		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
		case MANA_DATA_TYPE_FLOAT:
		case MANA_DATA_TYPE_ACTOR:
			mana_code_set_opecode(MANA_IL_REMOVE);
			break;

		case MANA_DATA_TYPE_STRUCT:
			mana_code_set_opecode_and_operand(MANA_IL_REMOVE_DATA, tree->type->memory_size);
			break;

		default:
			MANA_BUG("illigal type detect");
			break;
		}
	}

	mana_symbol_close_block();

	/* nodeの解放 */
	mana_node_release(tree);
}

/*!
 * 判別式内に代入文があるか調べます
 * @param	tree	評価式のmana_node
 */
static void mana_linker_condition_check(mana_node* tree)
{
	if(tree)
	{
		if(tree->id == MANA_NODE_ASSIGN)
		{
			/* 条件判定に代入文は使用できない */
			mana_compile_error("can't assign expression in condition");
		}

		mana_linker_condition_check(tree->left);
		mana_linker_condition_check(tree->right);
	}
}

/*!
 * 判別式の評価
 * @param	tree	評価式のmana_node
 * @param	pc		プログラムカウンタ
 * @return	現在のプログラムアドレス
 */
static int32_t mana_linker_condition_core(mana_node* tree)
{
	/* 判別式内に代入式があるか調べます */
	mana_linker_condition_check(tree);

	if(tree)
	{
		if((tree->type)->tcons == MANA_DATA_TYPE_VOID || (tree->type)->tcons > MANA_DATA_TYPE_REFERENCE)
			mana_compile_error("illegal type of expression in condition");

		mana_linker_generate_code(tree, true);
	}
	return mana_code_get_pc() - 5;
}

/*!
 * 判別式の評価
 * @param	tree	評価式のmana_node
 * @return	現在のプログラムアドレス
 */
int32_t mana_linker_condition(mana_node* tree, int32_t match)
{
	/* 判別式の評価 */
	mana_symbol_open_block(false);
	mana_linker_condition_core(tree);
	mana_symbol_close_block();

	/* nodeの解放 */
	mana_node_release(tree);

	return mana_code_set_opecode_and_operand(match ? MANA_IL_BEQ : MANA_IL_BNE, -1);
}
