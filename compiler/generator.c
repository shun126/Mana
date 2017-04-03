/*!
mana (compiler)

@file	mana_generator.c
@brief	コンパイルセクションに関するソースファイル
@detail	このファイルはコンパイルセクションに関係するソースファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(__MANA_GENERATOR_H___)
#include "generator.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "code.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "data.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "jump.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "linker.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_NODE_H___)
#include "node.h"
#endif
#if !defined(___MANA_REGISTER_H___)
#include "register.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif
#if !defined(___MANA_RESOLVER_H___)
#include "resolver.h"
#endif
#if !defined(___MANA_PRE_RESOLVER_H___)
#include "pre_resolver.h"
#endif
#if !defined(___MANA_POST_RESOLVER_H___)
#include "post_resolver.h"
#endif
#include <string.h>

// TODO
static mana_symbol_entry* mana_actor_symbol_entry_pointer = NULL;
static mana_symbol_entry* mana_function_symbol_entry_pointer = NULL;

static struct mana_generator_object
{
	mana_symbol_entry* actor_symbol_entry_pointer;
	mana_symbol_entry* function_symbol_entry_pointer;
	mana_hash* event_hash;
	bool static_block_opend;
}mana_generator_object;

////////////////////////////////////////////////////////////////////////////////
void mana_generator_initialize(void)
{
	mana_generator_object.event_hash = mana_hash_create();
	mana_generator_object.static_block_opend = false;
}

void mana_generator_finalize(void)
{
	mana_hash_destroy(mana_generator_object.event_hash);
	mana_generator_object.event_hash = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/*!
関数毎に特別な処理を行なうためのコールバック関数を登録します
@param[in]	name		関数名
@param[in]	function	コールバック関数
*/
void mana_generator_add_event(const char* name, mana_generator_event_funtion_type function)
{
	mana_hash_set(mana_generator_object.event_hash, name, function);
}


/*!
引数の数を取得します（再帰呼び出し）
@param	count	引数の番号
@param	param	引数のmana_symbol_entry
@param	arg		引数のmana_node
@return	引数の数
*/
static int32_t mana_generator_count_declaration_argument(int32_t count, mana_node* arg)
{
	if (arg)
	{
		if (arg->id == MANA_NODE_DECLARE_ARGUMENT)
		{
			count = mana_generator_count_declaration_argument(count, arg->right);
		}
		count++;
	}
	return count;
}

////////////////////////////////////////////////////////////////////////////////
// 以下は シンボルを解決しつつコードを出力しる 関数郡
////////////////////////////////////////////////////////////////////////////////
/*!
スタックにあるアドレスからデータを読み込む命令を出力する
@param[in]	読み込むmana_type_description
*/
static void mana_generator_resolve_load(mana_node* node)
{
	//	mana_register_entity* register_entity;

	MANA_ASSERT(node);
	MANA_ASSERT(node->type);
#if 0
	/* TODO:テスト */
	register_entity = mana_register_allocate(node->symbol, node->symbol->address);
	if (register_entity == NULL)
	{
		mana_register_entity* register_entity = mana_register_allocate(node->symbol, node->symbol->address);
		if (register_entity == NULL)
		{
			/* 割り当てるレジスタが無い場合はspillを行う */
		}
		/* レジスタに空きがある、または既に割り当て済みの場合 */
	}
#endif
	switch (node->type->tcons)
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
		if (node->type == mana_type_get(MANA_DATA_TYPE_ACTOR))
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
スタックにあるアドレスへデータを書き込む命令を出力する
@param[in]	読み込むmana_type_description
*/
static void mana_generator_resolve_store(mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(node->type);

	switch (node->type->tcons)
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
		mana_compile_error(node, "illegal type stored");
		break;
	}
}

/*!
* returnの処理
* @param	func	returnが所属する関数のmana_symbol_entry
* @param	tree	return文のmana_node
*/
static void mana_generator_return(mana_symbol_entry* func, mana_node* tree)
{
	mana_type_description* type = func->type;

	/* エラーチェック */
	if (type->tcons == MANA_DATA_TYPE_VOID)
	{
		if (tree != NULL)
			mana_compile_error(func, "meaningless return value specification");
	}
	else if (tree == NULL)
	{
		mana_compile_error(func, "missing return value");
	}
	else
	{
		/* 自動キャスト */
		tree->left = mana_node_cast(type, tree->left);

		/* 型の検証 */
		mana_type_compatible(type, tree->left->type);

		/* ノードの評価 */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_generator_genearte_code(tree->left, true);
		const int32_t out_depth = mana_symbol_close_block();
		MANA_VERIFY(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
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
static void mana_generator_rollback(mana_node* tree)
{
	if (tree)
	{
		/* ノードの評価 */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_generator_genearte_code(tree, true);
		const int32_t out_depth = mana_symbol_close_block();
		MANA_VERIFY(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
	}
	mana_code_set_opecode(MANA_IL_ROLLBACK);
}

/*!
* 引数の出力（再帰呼び出し）
* @param	count	引数の番号
* @param	param	引数のmana_symbol_entry
* @param	arg		引数のmana_node
* @return	引数の数
*/
static int32_t mana_generator_generate_argument(int32_t count, mana_symbol_entry* param, mana_node* arg)
{
	if (param && arg)
	{
		if (arg->id == MANA_NODE_CALL_ARGUMENT)
		{
			count = mana_generator_generate_argument(count, param->next, arg->right);

			if (arg->type == NULL)
			{
				MANA_ASSERT(arg->left);
				if (arg->left->id == MANA_NODE_IDENTIFIER)
					mana_resolver_search_symbol_from_name(arg->left);

				// 子ノードから型を継承する
				mana_resolver_resolve_type_from_child_node(arg);
			}
		}
		arg = mana_node_cast(param->type, arg);
		mana_type_compatible(param->type, arg->type);
		mana_generator_genearte_code(arg, true);
	}
	if (arg)
		count++;
	return count;
}

/*!
* 関数の引数のサイズを調べます
* @param	address	アドレス
* @param	param	引数のmana_symbol_entry
* @param	arg		引数のmana_node
* @return	引数のサイズ
*/
static int32_t mana_generator_call_argument_size(int32_t address, mana_symbol_entry* param, mana_node* arg)
{
	if (param && arg)
	{
		address += mana_generator_call_argument_size(address, param->next, arg->right);

		if (arg->id == MANA_NODE_CALL_ARGUMENT)
			arg = arg->left;

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
static int32_t mana_generator_call_argument(int32_t address, mana_symbol_entry* param, mana_node* arg)
{
	if (param && arg)
	{
		address = mana_generator_call_argument(address, param->next, arg->right);

		mana_code_set_short((int16_t)address);

		if (arg->id == MANA_NODE_CALL_ARGUMENT)
			arg = arg->left;
		address -= (mana_node_get_memory_size(arg) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
	}
	return address;
}

/*!
* 関数呼び出しのノードを評価します
* @param	関数呼び出しのmana_node
*/
static void mana_generator_call(mana_node* node)
{
	mana_node* argument = node->right;
	int32_t argument_counter = mana_generator_generate_argument(0, (node->symbol)->parameter_list, argument);

	/* エラーチェック */
	if ((node->symbol)->number_of_parameters != argument_counter)
	{
		/* 引数の数が一致しない */
		mana_compile_error(node, "unmatched argument.");
	}
	else if ((node->symbol)->class_type == MANA_CLASS_TYPE_NATIVE_FUNCTION)
	{
		/* 外部関数の処理 */
		int32_t argument_size = mana_generator_call_argument_size(0, (node->symbol)->parameter_list, argument);

		node->symbol->address = mana_data_set(node->symbol->name);

		mana_code_set_opecode_and_operand((uint8_t)MANA_IL_CALL, (node->symbol)->address);
		mana_code_set_unsigned_char((uint8_t)((node->symbol)->type->tcons != MANA_DATA_TYPE_VOID));
		mana_code_set_unsigned_char((uint8_t)argument_counter);
		mana_code_set_unsigned_short((uint16_t)argument_size);
		mana_generator_call_argument(argument_size - 1, (node->symbol)->parameter_list, argument);
	}
	else
	{
		// 内部関数の処理
		mana_linker_add_call_list(
			mana_code_set_opecode_and_operand((uint8_t)MANA_IL_BSR, (node->symbol)->address) + 1,
			node->symbol
		);
	}
}

/*!
* print文の処理
* @param	print文のmana_node
*/
static void mana_generator_call_print(mana_node* tree)
{
	//mana_generator_resolve_symbol(tree);

	int32_t argument_counter = 0;

	for (mana_node* node = tree; node; node = node->left)
	{
		mana_generator_genearte_code((node->id == MANA_NODE_CALL_ARGUMENT) ? node->right : node, true);
		argument_counter++;
	}

	mana_code_set_opecode_and_operand((uint8_t)MANA_IL_PRINT, argument_counter);
}

/*!
* 判別式内に代入文があるか調べます
* @param	tree	評価式のmana_node
*/
static void mana_generator_condition_check(mana_node* tree)
{
	if (tree)
	{
		if (tree->id == MANA_NODE_ASSIGN)
		{
			/* 条件判定に代入文は使用できない */
			mana_compile_error(tree, "can't assign expression in condition");
		}

		mana_generator_condition_check(tree->left);
		mana_generator_condition_check(tree->right);
	}
}

/*!
* 判別式の評価
* @param	tree	評価式のmana_node
* @param	pc		プログラムカウンタ
* @return	現在のプログラムアドレス
*/
static int32_t mana_generator_condition_core(mana_node* tree)
{
	/* 判別式内に代入式があるか調べます */
	mana_generator_condition_check(tree);

	if (tree)
	{
		if ((tree->type)->tcons == MANA_DATA_TYPE_VOID || (tree->type)->tcons > MANA_DATA_TYPE_REFERENCE)
			mana_compile_error(tree, "illegal type of expression in condition");

		mana_generator_genearte_code(tree, true);
	}
	return mana_code_get_pc() - 5;
}

/*!
* 判別式の評価
* @param	tree	評価式のmana_node
* @return	現在のプログラムアドレス
*/
static int32_t mana_generator_condition(mana_node* tree, int32_t match)
{
	//mana_generator_resolve_symbol(tree);
	//mana_generator_automatic_cast(tree);

	/* 判別式の評価 */
	const int32_t in_depth = mana_symbol_open_block(false);
	mana_generator_condition_core(tree);
	const int32_t out_depth = mana_symbol_close_block();
	MANA_VERIFY(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);

	return mana_code_set_opecode_and_operand(match ? MANA_IL_BEQ : MANA_IL_BNE, -1);
}

static void mana_generator_generate_const_int(const mana_symbol_data_type_id type_id, const int32_t value)
{
	switch (type_id)
	{
	case MANA_DATA_TYPE_CHAR:
		if (value == 0)
		{
			mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
		}
		else {
			mana_code_set_opecode(MANA_IL_PUSH_CHAR);
			mana_code_set_char((int8_t)value);
		}
		break;

	case MANA_DATA_TYPE_SHORT:
		if (value == 0)
		{
			mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
		}
		else {
			mana_code_set_opecode(MANA_IL_PUSH_SHORT);
			mana_code_set_short((int16_t)value);
		}
		break;

	case MANA_DATA_TYPE_INT:
		if (value == 0)
		{
			mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
		}
		else {
			mana_code_set_opecode(MANA_IL_PUSH_INTEGER);
			mana_code_set_integer(value);
		}
		break;

	default:
		//mana_compile_error(node, "illegal type of expression in statement");
		MANA_BUG("illegal type of expression");
		break;
	}
}

static void mana_generator_generate_const_float(const mana_symbol_data_type_id type_id, const float value)
{
	switch (type_id)
	{
	case MANA_DATA_TYPE_FLOAT:
		if (value == 0.0f)
		{
			mana_code_set_opecode(MANA_IL_PUSH_ZERO_FLOAT);
		}
		else {
			mana_code_set_opecode(MANA_IL_PUSH_FLOAT);
			mana_code_set_float(value);
		}
		break;

	default:
		//mana_compile_error(node, "illegal type of expression in statement");
		MANA_BUG("illegal type of expression");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
式の評価
@param	tree			式のmana_node
@param	enable_assign	trueならば代入式、falseならばそれ以外
*/
void mana_generator_expression(mana_node* tree, int32_t enable_assign)
{
	if (!tree)
		return;

	//mana_generator_resolve_symbol(tree);

	const int32_t in_depth = mana_symbol_open_block(false);

	if (enable_assign)
	{
		if (tree->id != MANA_NODE_ASSIGN && tree->id != MANA_NODE_CALL && tree->id != MANA_NODE_F2I)
			mana_compile_error(tree, "illegal expression in write-statement");
	}
	else {
		if (tree->id == MANA_NODE_ASSIGN)
			mana_compile_error(tree, "illegal expression in write-statement");
	}

	mana_generator_genearte_code(tree, true);

	if (enable_assign && tree->id == MANA_NODE_CALL && tree->type)
	{
		switch (tree->type->tcons)
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
			MANA_BUG("illegal type of expression");
			break;
		}
	}

	const int32_t out_depth = mana_symbol_close_block();
	MANA_VERIFY(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
}


////////////////////////////////////////////////////////////////////////////////
void mana_generator_genearte_code(mana_node* node, int32_t enable_load)
{
	if (!node)
		return;

DO_RECURSIVE:
	mana_resolver_set_current_file_infomation(node);

	switch (node->id)
	{
		///////////////////////////////////////////////////////////////////////
		// 定数定義に関するノード									
	case MANA_NODE_DEFINE_ALIAS:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DEFINE_CONSTANT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_UNDEFINE_CONSTANT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// メモリレイアウトに関するノード
	case MANA_NODE_DECLARE_ALLOCATE:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_STATIC:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 構造に関するノード									
	case MANA_NODE_DECLARE_ACTOR:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(node->string);
			mana_generator_genearte_code(node->left, enable_load);
			mana_symbol_close_actor();
			mana_actor_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_EXTEND:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_MODULE:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_module(mana_actor_symbol_entry_pointer);
			mana_generator_genearte_code(node->left, enable_load);
			mana_symbol_close_module(node->string);
			mana_actor_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(node->string);
			mana_generator_genearte_code(node->left, enable_load);
			mana_symbol_close_actor();
			mana_actor_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_STRUCT:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_post_resolver_resolve(node);
		break;

		///////////////////////////////////////////////////////////////////////
		// 関数宣言に関するノード									
	case MANA_NODE_DECLARE_ACTION:
		{
			node->type = mana_type_get(MANA_DATA_TYPE_VOID);
			mana_function_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_function(node, true);
		
			mana_generator_genearte_code(node->left, enable_load);
			
			mana_symbol_close_function(node, true);
			mana_function_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_ARGUMENT:
		MANA_ASSERT(node->body == NULL);
		MANA_BUG("illegal type of expression");
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		{
			MANA_ASSERT(node->symbol);
			// 関数の戻り値を評価
			mana_generator_genearte_code(node->left, enable_load);
			// シンボルの検索と型の定義
			node->symbol = mana_function_symbol_entry_pointer = mana_symbol_lookup(node->string);

			mana_symbol_open_block(false);

			// 関数の引数を登録
			mana_pre_resolver_resolve(node->right);
			node->symbol->parameter_list = mana_symbol_get_head_symbol();

			// 
			mana_symbol_open_function(node, false);
			mana_generator_genearte_code(node->body, enable_load);
			mana_symbol_close_function(node, false);

			mana_symbol_close_block();

			mana_function_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		// node->left
		// node->right
		// node->body
		break;

		///////////////////////////////////////////////////////////////////////
		// 変数宣言に関するノード									
	case MANA_NODE_DECLARATOR:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		//mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
		// node->left
		// node->right
		MANA_ASSERT(node->body == NULL);
		//mana_resolver_resolve_variable_description(node, MANA_MEMORY_TYPE_NORMAL);
		/*
		mana_generator_genearte_code(node->left, enable_load); // MANA_NODE_TYPE_DESCRIPTION
		mana_generator_genearte_code(node->right, enable_load);// MANA_NODE_DECLARATOR
		if(node->right->symbol->class_type == MANA_CLASS_TYPE_VARIABLE_LOCAL)
		mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
		*/
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		//mana_resolver_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		mana_generator_genearte_code(node->left, enable_load);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ブロックを伴う制御に関するノード
	case MANA_NODE_BLOCK:
		{
			const int32_t in_depth = mana_symbol_open_block(false);

			mana_post_resolver_resolve(node->left);
			mana_generator_genearte_code(node->left, enable_load);

			mana_post_resolver_resolve(node->right);
			mana_generator_genearte_code(node->right, enable_load);

			const int32_t out_depth = mana_symbol_close_block();
			MANA_VERIFY(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_BREAK:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc()));
		break;

	case MANA_NODE_CASE:
		mana_jump_switch_case(node->left);
		if (node->left && node->left->type) node->type = node->left->type;
		mana_generator_genearte_code(node->right, enable_load);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_CONTINUE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
		break;

	case MANA_NODE_DEFAULT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		mana_jump_switch_default();
		break;

	case MANA_NODE_DO:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_DO);
			const int32_t address = mana_code_get_pc();
			mana_generator_genearte_code(node->left, enable_load);
			mana_jump_close_continue_only();

			mana_code_replace_all(mana_generator_condition(node->right, false), address);
			mana_jump_close_chain();
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_FOR:
		/* 'for(type variable = expression' の形式 */
		{
			//mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL);
			//mana_generator_expression(mana_node_create_node(MANA_NODE_TYPE_ASSIGN, mana_node_create_leaf($2->name), $4), true);
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
			//$$ = mana_code_get_pc();

			//mana_generator_genearte_code(node->left, enable_load);
			mana_jump_break(mana_generator_condition(node->left, true));

			mana_generator_genearte_code(node->body, enable_load);

			mana_jump_close_continue_only();
			mana_generator_expression(node->right, true);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
			mana_jump_close_chain();
		}
		break;

	case MANA_NODE_GOTO:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_entry* symbol = mana_symbol_create_label(node->string);
			symbol->etc = mana_code_set_opecode_and_operand(MANA_IL_BRA, symbol->etc);
		}
		break;

	case MANA_NODE_HALT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_HALT);
		break;

	case MANA_NODE_IF:
		{
			int32_t address = mana_generator_condition(node->left, true);
			mana_generator_genearte_code(node->right, enable_load);
			if (node->body)
			{
				const int32_t else_begin_address = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
				mana_code_replace_all(address, mana_code_get_pc());
				mana_generator_genearte_code(node->body, enable_load);
				address = else_begin_address;
			}
			mana_code_replace_all(address, mana_code_get_pc());
		}
		break;

	case MANA_NODE_LABEL:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_entry* symbol = mana_symbol_create_label(node->string);
			symbol->address = mana_code_get_pc();
		}
		break;

	case MANA_NODE_LOCK:
		mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOCK);
		mana_code_set_opecode(MANA_IL_NONPREEMPTIVE);
		mana_generator_genearte_code(node->left, enable_load);
		mana_jump_close_chain();
		mana_code_set_opecode(MANA_IL_PREEMPTIVE);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_LOOP:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOOP);
			const int32_t address = mana_code_get_pc();
			mana_generator_genearte_code(node->left, enable_load);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, address);
			mana_jump_close_chain();
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_RETURN:
		mana_generator_genearte_code(node->left, enable_load);
		if (node->left) node->type = node->left->type;
		mana_generator_return(mana_function_symbol_entry_pointer, node);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_ROLLBACK:
		mana_generator_genearte_code(node->left, enable_load);
		if (node->left) node->type = node->left->type;
		mana_generator_rollback(node);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_SWITCH:
		{
			mana_generator_expression(node->left, false);
			const int32_t address = mana_code_get_pc();
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_SWITCH);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
			mana_jump_open_switch(node->left->type);

			mana_generator_genearte_code(node->right, enable_load);

			mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc()));
			mana_code_replace_all(address, mana_code_get_pc());
			mana_jump_switch_build();
			mana_jump_close_chain();
			mana_jump_close_switch();
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_WHILE:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_WHILE);
			mana_jump_break(mana_generator_condition(node->left, true));
			mana_generator_genearte_code(node->right, enable_load);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
			mana_jump_close_chain();
		}
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 制御に関するノード
	case MANA_NODE_COMPLY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_COMPLY);
		break;

	case MANA_NODE_JOIN:
		//mana_generator_resolve_symbol(node);
		mana_symbol_add_join(node->left, node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_PRINT:
		mana_generator_call_print(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_REFUSE:
		mana_code_set_opecode(MANA_IL_REFUSE);
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_REQUEST:
		mana_symbol_add_request(MANA_IL_REQ, node->left, node->right, node->string);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_YIELD:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_YIELD);
		break;

		///////////////////////////////////////////////////////////////////////
		// 二項演算子に関するノード
	case MANA_NODE_ADD:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_ADD_FLOAT : MANA_IL_ADD_INTEGER);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_SUB:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_MUL:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MUL_FLOAT : MANA_IL_MUL_INTEGER);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DIV:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_DIV_FLOAT : MANA_IL_DIV_INTEGER);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_REM:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MOD_FLOAT : MANA_IL_MOD_INTEGER);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_POW:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_AND:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_AND);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_OR:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_OR);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_XOR:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_EOR);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_LSH:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_RSH:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHR);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_LAND:
	case MANA_NODE_LOR:
	case MANA_NODE_EQ:
	case MANA_NODE_GE:
	case MANA_NODE_GT:
	case MANA_NODE_LS:
	case MANA_NODE_LE:
	case MANA_NODE_NE:
		// 比較、論理演算子
		//mana_generator_automatic_cast(node);
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		MANA_ASSERT(node->body == NULL);
		// TODO:オペコードを設定してください
		mana_code_set_opecode((uint8_t)node->etc);
		break;

		///////////////////////////////////////////////////////////////////////
		// 単項演算子に関するノード
	case MANA_NODE_LNOT:
	case MANA_NODE_NOT:
		//mana_generator_automatic_cast(node);
		mana_generator_genearte_code(node->left, enable_load);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		// TODO:オペコードを設定してください
		mana_code_set_opecode((uint8_t)node->etc);
		break;

	case MANA_NODE_NEG:
		mana_generator_genearte_code(node->left, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MINUS_FLOAT : MANA_IL_MINUS_INTEGER);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 演算に関するノード
	case MANA_NODE_ARRAY:
		/* variable[index] */
		mana_generator_genearte_code(node->right, true);
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, (node->type)->memory_size);
		mana_code_set_opecode(MANA_IL_MUL_INTEGER);
		mana_generator_genearte_code(node->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if (enable_load)
		{
			mana_generator_resolve_load(node);
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_ASSIGN:
		//mana_generator_resolve_symbol(node->left);
		//mana_generator_resolve_symbol(node->right);

		//mana_generator_automatic_cast(node);
		mana_generator_genearte_code(node->right, enable_load);
		mana_generator_genearte_code(node->left, false);
		mana_generator_resolve_store(node->left);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_CALL:
		// 関数、アクションを呼びます
		//mana_generator_resolve_symbol(node);
		mana_resolver_search_symbol_from_name(node);
		//mana_generator_genearte_code(node->right, enable_load);
		mana_generator_call(node);
		break;

	case MANA_NODE_CALL_ARGUMENT:
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_CONST:
		switch (node->type->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
			mana_generator_generate_const_int(node->type->tcons, node->digit);
			break;

		case MANA_DATA_TYPE_FLOAT:
			mana_generator_generate_const_float(node->type->tcons, node->real);
			break;

		case MANA_DATA_TYPE_NIL:
			mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			break;

		default:
			mana_compile_error(node, "illegal type of expression in statement");
			break;
		}
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_EXPRESSION_IF:
		// 三項演算子
		{
			int32_t pc1, pc2;
			mana_generator_condition_core(node->next);
			pc1 = mana_code_set_opecode_and_operand(MANA_IL_BEQ, -1);
			mana_generator_genearte_code(node->left, enable_load);
			pc2 = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
			mana_code_replace_all(pc1, mana_code_get_pc());
			mana_generator_genearte_code(node->right, enable_load);
			mana_code_replace_all(pc2, mana_code_get_pc());
		}
		break;

	case MANA_NODE_I2F:
		mana_generator_genearte_code(node->left, enable_load);
		mana_code_set_opecode(MANA_IL_INT2FLOAT);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_F2I:
		mana_generator_genearte_code(node->left, enable_load);
		mana_code_set_opecode(MANA_IL_FLOAT2INT);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_IDENTIFIER:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		MANA_ASSERT(node->symbol);
		switch (node->symbol->class_type)
		{
		case MANA_CLASS_TYPE_ALIAS:
			mana_generator_generate_const_int(node->symbol->type->tcons, node->symbol->address);
			break;

		case MANA_CLASS_TYPE_CONSTANT_INT:
			mana_generator_generate_const_int(node->symbol->type->tcons, node->symbol->address);
			break;

		case MANA_CLASS_TYPE_CONSTANT_FLOAT:
			mana_generator_generate_const_float(node->symbol->type->tcons, node->symbol->hloat);
			break;

		case MANA_CLASS_TYPE_CONSTANT_STRING:
			mana_generator_generate_const_int(node->symbol->type->tcons, node->symbol->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_STATIC:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_STATIC_ADDRESS, node->symbol->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_GLOBAL:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_GLOBAL_ADDRESS, node->symbol->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_ACTOR:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_SELF_ADDRESS, node->symbol->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_LOCAL:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_FRAME_ADDRESS, node->symbol->address);
			break;

		case MANA_CLASS_TYPE_TYPEDEF:
			break;

		case MANA_CLASS_TYPE_NEW_SYMBOL:

		default:
			mana_compile_error("illigal data type");
			break;
		}

		if (enable_load)
			mana_generator_resolve_load(node);
		break;

	case MANA_NODE_MEMBER_FUNCTION:
		// TODO:実装してください
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_MEMBER_VARIABLE:
		MANA_ASSERT(node->left);
		MANA_ASSERT(node->right == NULL);
		{
			//mana_generator_resolve_symbol(node->left);
			/////mana_resolver_search_symbol_from_name(node);

			mana_type_description* type = node->left->type;
			if (type)
			{
				if (type->tcons == MANA_DATA_TYPE_STRUCT)
				{
					while (type)
					{
						for (mana_symbol_entry* symbol = (mana_symbol_entry*)type->component; symbol; symbol = symbol->next)
						{
							if (strcmp(symbol->name, node->string) == 0 && symbol->class_type == MANA_CLASS_TYPE_VARIABLE_ACTOR)
							{
								// variable.member
								mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, symbol->address);
								mana_generator_genearte_code(node->left, false);
								mana_code_set_opecode(MANA_IL_ADD_INTEGER);
								if (enable_load)
									mana_generator_resolve_load(node);
								goto ESCAPE;
							}
						}
						type = type->parent;
					}
					mana_compile_error("reference to undefined field");
				}
				else
				{
					mana_compile_error("reference to non-struct type");
				}
			}
		}
		ESCAPE:
		break;
#if 0
	case MANA_NODE_MEMOP:
		/* variable.member */
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, node->etc);
		mana_generator_genearte_code(node->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if (enable_load)
		{
			mana_generator_resolve_load(node);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;
#endif
	case MANA_NODE_SENDER:
		// senderをpushします
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_PUSH_SENDER);
		break;

	case MANA_NODE_SELF:
		// selfをpushします
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_PUSH_SELF);
		break;

	case MANA_NODE_PRIORITY:
		// priorityをpushします
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_PUSH_PRIORITY);
		break;

	case MANA_NODE_SIZEOF:
		{
			int32_t max_char = (1 << (8 * CBSZ - 1)) - 1;
			int32_t min_char = -1 << (8 * CBSZ - 1);
			int32_t max_short = (1 << (8 * SBSZ - 1)) - 1;
			int32_t min_short = -1 << (8 * SBSZ - 1);

			node->digit = node->left->type->memory_size;

			if (node->digit <= max_char && node->digit >= min_char)
				node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
			else if (node->digit <= max_short && node->digit >= min_short)
				node->type = mana_type_get(MANA_DATA_TYPE_SHORT);
			else
				node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_STRING:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_STRING, node->digit);
		break;
#if 0
	case MANA_NODE_VARIABLE:
		/* variable */
		mana_generator_genearte_code(node->right, enable_load);
		mana_generator_genearte_code(node->left, enable_load);

		//mana_generator_generate_variable_value(node);
		switch ((node->symbol)->class_type)
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
			mana_compile_error(node, "illegal type of expression in statement");
			break;
		}

		if (enable_load)
		{
			mana_generator_resolve_load(node);
		}

		break;
#endif
	default:
		MANA_BUG("illegal right-hand side value");
		break;
	}

	// 子ノードから型を継承する
	mana_resolver_resolve_type_from_child_node(node);

	if (node->next)
	{
		// 末尾再帰なのでgotoにて処理する
		//mana_generator_genearte_code(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}
