/*!
mana (compiler)

@file	generator.c
@brief	コンパイルセクションに関するソースファイル
@detail	このファイルはコンパイルセクションに関係するソースファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_GENERATOR_H___)
#include "generator.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "code.h"
#endif
#if !defined(___SYMBOL_DATA_H___)
#include "data.h"
#endif
#if !defined(___MANA_ERROR_H___)
#include "error.h"
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
#if !defined(___NODE_H___)
#include "self.h"
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
static symbol_entry* mana_actor_symbol_entry_pointer = NULL;
static symbol_entry* mana_function_symbol_entry_pointer = NULL;

static struct generator_entry
{
	symbol_entry* actor_symbol_entry_pointer;
	symbol_entry* function_symbol_entry_pointer;
	mana_hash* event_hash;
	bool static_block_opend;
}generator_entry;

////////////////////////////////////////////////////////////////////////////////
void generator_initialize(void)
{
	generator_entry.event_hash = mana_hash_create();
	generator_entry.static_block_opend = false;
}

void generator_finalize(void)
{
	mana_hash_destroy(generator_entry.event_hash);
	generator_entry.event_hash = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/*!
関数毎に特別な処理を行なうためのコールバック関数を登録します
@param[in]	name		関数名
@param[in]	function	コールバック関数
*/
void generator_add_event(const char* name, generator_event_funtion_type function)
{
	mana_hash_set(generator_entry.event_hash, name, function);
}

////////////////////////////////////////////////////////////////////////////////
// 以下は シンボルを解決しつつコードを出力しる 関数郡
////////////////////////////////////////////////////////////////////////////////
/*!
スタックにあるアドレスからデータを読み込む命令を出力する
@param[in]	読み込むtype_description
*/
static void generator_resolve_load(node_entry* self)
{
	//	register_entity* register_entity;

	MANA_ASSERT(self);
	MANA_ASSERT(self->type);
#if 0
	/* TODO:テスト */
	register_entity = register_allocate(self->symbol, self->symbol->address);
	if (register_entity == NULL)
	{
		register_entity* register_entity = register_allocate(self->symbol, self->symbol->address);
		if (register_entity == NULL)
		{
			/* 割り当てるレジスタが無い場合はspillを行う */
		}
		/* レジスタに空きがある、または既に割り当て済みの場合 */
	}
#endif
	switch (self->type->tcons)
	{
	case SYMBOL_DATA_TYPE_CHAR:
		code_set_opecode(MANA_IL_LOAD_CHAR);
		break;

	case SYMBOL_DATA_TYPE_SHORT:
		code_set_opecode(MANA_IL_LOAD_SHORT);
		break;

	case SYMBOL_DATA_TYPE_INT:
		code_set_opecode(MANA_IL_LOAD_INTEGER);
		break;

	case SYMBOL_DATA_TYPE_FLOAT:
		code_set_opecode(MANA_IL_LOAD_FLOAT);
		break;

	case SYMBOL_DATA_TYPE_REFERENCE:
		code_set_opecode(MANA_IL_LOAD_REFFRENCE);
		break;

	case SYMBOL_DATA_TYPE_ACTOR:
		if (self->type == type_get(SYMBOL_DATA_TYPE_ACTOR))
			code_set_opecode(MANA_IL_LOAD_REFFRENCE);
		else
			code_set_opecode_and_operand(MANA_IL_PUSH_ACTOR, data_set(self->type->name));
		break;

	case SYMBOL_DATA_TYPE_ARRAY:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);

	case SYMBOL_DATA_TYPE_STRUCT:
		code_set_opecode_and_operand(MANA_IL_LOAD_DATA, self->type->memory_size);
		break;

	default:
		MANA_BUG("illigal data type");
	}
}

/*!
スタックにあるアドレスへデータを書き込む命令を出力する
@param[in]	読み込むtype_description
*/
static void generator_resolve_store(node_entry* self)
{
	MANA_ASSERT(self);
	MANA_ASSERT(self->type);

	switch (self->type->tcons)
	{
	case SYMBOL_DATA_TYPE_CHAR:
		code_set_opecode(MANA_IL_STORE_CHAR);
		break;

	case SYMBOL_DATA_TYPE_SHORT:
		code_set_opecode(MANA_IL_STORE_SHORT);
		break;

	case SYMBOL_DATA_TYPE_INT:
		code_set_opecode(MANA_IL_STORE_INTEGER);
		break;

	case SYMBOL_DATA_TYPE_FLOAT:
		code_set_opecode(MANA_IL_STORE_FLOAT);
		break;

	case SYMBOL_DATA_TYPE_REFERENCE:
	case SYMBOL_DATA_TYPE_ACTOR:
		code_set_opecode(MANA_IL_STORE_REFFRENCE);
		break;

	case SYMBOL_DATA_TYPE_ARRAY:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);

	case SYMBOL_DATA_TYPE_STRUCT:
		code_set_opecode_and_operand(MANA_IL_STORE_DATA, self->type->memory_size);
		break;

	default:
		mana_compile_error("illegal type stored");
		break;
	}
}

/*!
* returnの処理
* @param	func	returnが所属する関数のsymbol_entry
* @param	tree	return文のnode_entry
*/
static void generator_return(symbol_entry* func, node_entry* tree)
{
	type_description* type = func->type;

	/* エラーチェック */
	if (type->tcons == SYMBOL_DATA_TYPE_VOID)
	{
		if (tree != NULL)
			mana_compile_error("meaningless return value specification");
	}
	else if (tree == NULL)
	{
		mana_compile_error("missing return value");
	}
	else
	{
		/* 自動キャスト */
		tree->left = node_cast(type, tree->left);

		/* 型の検証 */
		type_compatible(type, tree->left->type);

		/* ノードの評価 */
		const int32_t in_depth = symbol_open_block(false);
		generator_genearte_code(tree->left, true);
		const int32_t out_depth = symbol_close_block();
		MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
	}

	/* 関数の最後にジャンプ */
	symbol_return_address_list = code_set_opecode_and_operand(MANA_IL_BRA, symbol_return_address_list);

	/* 関数を使用したフラグを立てる */
	func->used = true;
}

/*!
* rollbackの処理
* @param	tree	rollback文のnode_entry
*/
static void generator_rollback(node_entry* tree)
{
	if (tree)
	{
		/* ノードの評価 */
		const int32_t in_depth = symbol_open_block(false);
		generator_genearte_code(tree, true);
		const int32_t out_depth = symbol_close_block();
		MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
	}
	code_set_opecode(MANA_IL_ROLLBACK);
}

/*!
* 関数呼び出し時の引数を出力（再帰呼び出し）
* @param	count	引数の番号
* @param	param	引数のsymbol_entry
* @param	arg		引数のnode_entry
* @return	引数の数
*/
static int32_t generate_argument(int32_t count, symbol_entry* param, node_entry* arg)
{
	if (param && arg)
	{
		if (arg->id == NODE_CALL_ARGUMENT)
		{
			count = generate_argument(count, param->next, arg->right);

			if (arg->type == NULL)
			{
				MANA_ASSERT(arg->left);
				if (arg->left->id == NODE_IDENTIFIER)
					resolver_search_symbol_from_name(arg->left);

				// 子ノードから型を継承する
				resolver_resolve_type_from_child_node(arg);
			}
		}
		arg = node_cast(param->type, arg);
		type_compatible(param->type, arg->type);
		//generator_genearte_code(arg, true);
		generator_genearte_code((arg->id == NODE_CALL_ARGUMENT) ? arg->left : arg, true);
	}
	if (arg)
		count++;
	return count;
}

/*!
* 関数の引数のサイズを調べます
* @param	address	アドレス
* @param	param	引数のsymbol_entry
* @param	arg		引数のnode_entry
* @return	引数のサイズ
*/
static int32_t generator_call_argument_size(int32_t address, symbol_entry* param, node_entry* arg)
{
	if (param && arg)
	{
		address += generator_call_argument_size(address, param->next, arg->right);

		if (arg->id == NODE_CALL_ARGUMENT)
			arg = arg->left;

		address += (node_get_memory_size(arg) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
	}
	return address;
}

/*!
* 関数の引数を評価します
* @param	address	引数のアドレス
* @param	param	引数のsymbol_entry
* @param	arg		引数のnode_entry
* @return	引数のアドレス
*/
static int32_t generator_call_argument(int32_t address, symbol_entry* param, node_entry* arg)
{
	if (param && arg)
	{
		address = generator_call_argument(address, param->next, arg->right);

		code_set_short((int16_t)address);

		if (arg->id == NODE_CALL_ARGUMENT)
			arg = arg->left;
		address -= (node_get_memory_size(arg) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
	}
	return address;
}

/*!
* 関数呼び出しのノードを評価します
* @param	関数呼び出しのnode_entry
*/
static void generator_call(node_entry* self)
{
	node_entry* argument = self->right;
	int32_t argument_counter = generate_argument(0, (self->symbol)->parameter_list, argument);

	/* エラーチェック */
	if ((self->symbol)->number_of_parameters != argument_counter)
	{
		/* 引数の数が一致しない */
		mana_compile_error("unmatched argument.");
	}
	else if ((self->symbol)->class_type == SYMBOL_CLASS_TYPE_NATIVE_FUNCTION)
	{
		/* 外部関数の処理 */
		int32_t argument_size = generator_call_argument_size(0, (self->symbol)->parameter_list, argument);

		self->symbol->address = data_set(self->symbol->name);

		code_set_opecode_and_operand((uint8_t)MANA_IL_CALL, (self->symbol)->address);
		code_set_unsigned_char((uint8_t)((self->symbol)->type->tcons != SYMBOL_DATA_TYPE_VOID));
		code_set_unsigned_char((uint8_t)argument_counter);
		code_set_unsigned_short((uint16_t)argument_size);
		generator_call_argument(argument_size - 1, (self->symbol)->parameter_list, argument);
	}
	else
	{
		// 内部関数の処理
		mana_linker_add_call_list(
			code_set_opecode_and_operand((uint8_t)MANA_IL_BSR, (self->symbol)->address) + 1,
			self->symbol
		);
	}
}

static int32_t generator_call_print_generate_argument(int32_t argc, node_entry* self)
{
	if (self)
	{
		argc = generator_call_print_generate_argument(argc, self->right);
		
		generator_genearte_code((self->id == NODE_CALL_ARGUMENT) ? self->left : self, true);
		
		++argc;
	}
	return argc;
}

/*!
* print文の処理
* @param[in]	self	print文のnode_entry
*/
static void generator_call_print(node_entry* self)
{
	code_set_opecode_and_operand(
		(uint8_t)MANA_IL_PRINT,
		generator_call_print_generate_argument(0, self)
	);
}

/*!
* 判別式内に代入文があるか調べます
* @param	tree	評価式のnode_entry
*/
static void generator_condition_check(node_entry* tree)
{
	if (tree)
	{
		if (tree->id == NODE_ASSIGN)
		{
			/* 条件判定に代入文は使用できない */
			mana_compile_error("can't assign expression in condition");
		}

		generator_condition_check(tree->left);
		generator_condition_check(tree->right);
	}
}

/*!
* 判別式の評価
* @param	tree	評価式のnode_entry
* @param	pc		プログラムカウンタ
* @return	現在のプログラムアドレス
*/
static int32_t generator_condition_core(node_entry* tree)
{
	/* 判別式内に代入式があるか調べます */
	generator_condition_check(tree);

	if (tree)
	{
		if ((tree->type)->tcons == SYMBOL_DATA_TYPE_VOID || (tree->type)->tcons > SYMBOL_DATA_TYPE_REFERENCE)
			mana_compile_error("illegal type of expression in condition");

		generator_genearte_code(tree, true);
	}
	return code_get_pc() - 5;
}

/*!
* 判別式の評価
* @param	tree	評価式のnode_entry
* @return	現在のプログラムアドレス
*/
static int32_t generator_condition(node_entry* tree, int32_t match)
{
	//generator_resolve_symbol(tree);
	//generator_automatic_cast(tree);

	/* 判別式の評価 */
	const int32_t in_depth = symbol_open_block(false);
	generator_condition_core(tree);
	const int32_t out_depth = symbol_close_block();
	MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);

	return code_set_opecode_and_operand(match ? MANA_IL_BEQ : MANA_IL_BNE, -1);
}

static void generator_generate_const_int(const symbol_data_type_id type_id, const int32_t value)
{
	switch (type_id)
	{
	case SYMBOL_DATA_TYPE_CHAR:
		if (value == 0)
		{
			code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
		}
		else {
			code_set_opecode(MANA_IL_PUSH_CHAR);
			code_set_char((int8_t)value);
		}
		break;

	case SYMBOL_DATA_TYPE_SHORT:
		if (value == 0)
		{
			code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
		}
		else {
			code_set_opecode(MANA_IL_PUSH_SHORT);
			code_set_short((int16_t)value);
		}
		break;

	case SYMBOL_DATA_TYPE_INT:
		if (value == 0)
		{
			code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
		}
		else {
			code_set_opecode(MANA_IL_PUSH_INTEGER);
			code_set_integer(value);
		}
		break;

	default:
		//mana_compile_error("illegal type of expression in statement");
		MANA_BUG("illegal type of expression");
		break;
	}
}

static void generator_generate_const_float(const symbol_data_type_id type_id, const float value)
{
	switch (type_id)
	{
	case SYMBOL_DATA_TYPE_FLOAT:
		if (value == 0.0f)
		{
			code_set_opecode(MANA_IL_PUSH_ZERO_FLOAT);
		}
		else {
			code_set_opecode(MANA_IL_PUSH_FLOAT);
			code_set_float(value);
		}
		break;

	default:
		//mana_compile_error("illegal type of expression in statement");
		MANA_BUG("illegal type of expression");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
式の評価
@param	tree			式のnode_entry
@param	enable_assign	trueならば代入式、falseならばそれ以外
*/
void generator_expression(node_entry* tree, int32_t enable_assign)
{
	if (!tree)
		return;

	//generator_resolve_symbol(tree);

	const int32_t in_depth = symbol_open_block(false);

	if (enable_assign)
	{
		if (tree->id != NODE_ASSIGN && tree->id != NODE_CALL && tree->id != NODE_F2I)
			mana_compile_error("illegal expression in write-statement");
	}
	else {
		if (tree->id == NODE_ASSIGN)
			mana_compile_error("illegal expression in write-statement");
	}

	generator_genearte_code(tree, true);

	if (enable_assign && tree->id == NODE_CALL && tree->type)
	{
		switch (tree->type->tcons)
		{
		case SYMBOL_DATA_TYPE_VOID:
			break;

		case SYMBOL_DATA_TYPE_CHAR:
		case SYMBOL_DATA_TYPE_SHORT:
		case SYMBOL_DATA_TYPE_INT:
		case SYMBOL_DATA_TYPE_FLOAT:
		case SYMBOL_DATA_TYPE_ACTOR:
			code_set_opecode(MANA_IL_REMOVE);
			break;

		case SYMBOL_DATA_TYPE_STRUCT:
			code_set_opecode_and_operand(MANA_IL_REMOVE_DATA, tree->type->memory_size);
			break;

		default:
			MANA_BUG("illegal type of expression");
			break;
		}
	}

	const int32_t out_depth = symbol_close_block();
	MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
}


////////////////////////////////////////////////////////////////////////////////
void generator_genearte_code(node_entry* self, int32_t enable_load)
{
	if (!self)
		return;

DO_RECURSIVE:
	resolver_set_current_file_infomation(self);

	switch (self->id)
	{
		///////////////////////////////////////////////////////////////////////
		// 定数定義に関するノード									
	case NODE_DEFINE_ALIAS:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DEFINE_CONSTANT:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_UNDEFINE_CONSTANT:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// メモリレイアウトに関するノード
	case NODE_DECLARE_ALLOCATE:
		// self->left
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DECLARE_STATIC:
		// self->left
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 構造に関するノード									
	case NODE_DECLARE_ACTOR:
		{
			mana_actor_symbol_entry_pointer = symbol_lookup(self->string);
			symbol_open_actor(self->string);
			generator_genearte_code(self->left, enable_load);
			symbol_close_actor();
			mana_actor_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DECLARE_EXTEND:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DECLARE_MODULE:
		{
			mana_actor_symbol_entry_pointer = symbol_lookup(self->string);
			symbol_open_module(mana_actor_symbol_entry_pointer);
			generator_genearte_code(self->left, enable_load);
			symbol_close_module(self->string);
			mana_actor_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DECLARE_PHANTOM:
		{
			mana_actor_symbol_entry_pointer = symbol_lookup(self->string);
			symbol_open_actor(self->string);
			generator_genearte_code(self->left, enable_load);
			symbol_close_actor();
			mana_actor_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DECLARE_STRUCT:
		// self->left
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		post_resolver_resolve(self);
		break;

		///////////////////////////////////////////////////////////////////////
		// 関数宣言に関するノード									
	case NODE_DECLARE_ACTION:
		{
			self->type = type_get(SYMBOL_DATA_TYPE_VOID);
			mana_function_symbol_entry_pointer = symbol_lookup(self->string);
			{
				symbol_open_function(self, true);
				generator_genearte_code(self->left, enable_load);
				symbol_close_function(self, true);
			}
			mana_function_symbol_entry_pointer = NULL;
		}
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DECLARE_ARGUMENT:
		MANA_ASSERT(self->body == NULL);
		MANA_BUG("illegal type of expression");
		break;

	case NODE_DECLARE_FUNCTION:
		{
			MANA_ASSERT(self->symbol);
			// 関数の戻り値を評価
			generator_genearte_code(self->left, enable_load);
			// シンボルの検索と型の定義
			self->symbol = mana_function_symbol_entry_pointer = symbol_lookup(self->string);

			// 引数の為にスコープを分ける
			symbol_open_block(false);
			{
				// 引数を登録
				symbol_open_function(self, false);

				pre_resolver_resolve(self->right);
				self->symbol->parameter_list = symbol_get_head_symbol();

				symbol_open_function2(self->symbol);

				generator_genearte_code(self->body, enable_load);
				symbol_close_function(self, false);
			}
			symbol_close_block();

			mana_function_symbol_entry_pointer = NULL;
		}
		break;

	case NODE_DECLARE_NATIVE_FUNCTION:
		// self->left
		// self->right
		// self->body
		break;

		///////////////////////////////////////////////////////////////////////
		// 変数宣言に関するノード									
	case NODE_DECLARATOR:
		// self->left
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DECLARE_VARIABLE:
		//symbol_allocate_memory(self->right->symbol, self->left->type, MEMORY_TYPE_NORMAL);
		// self->left
		// self->right
		MANA_ASSERT(self->body == NULL);
		//resolver_resolve_variable_description(self, MEMORY_TYPE_NORMAL);
		/*
		generator_genearte_code(self->left, enable_load); // NODE_TYPE_DESCRIPTION
		generator_genearte_code(self->right, enable_load);// NODE_DECLARATOR
		if(self->right->symbol->class_type == SYMBOL_CLASS_TYPE_VARIABLE_LOCAL)
		symbol_allocate_memory(self->right->symbol, self->left->type, MEMORY_TYPE_NORMAL);
		*/
		break;

	case NODE_TYPE_DESCRIPTION:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		//resolver_resolve_type_description(self);
		break;

	case NODE_VARIABLE_SIZE:
		generator_genearte_code(self->left, enable_load);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ブロックを伴う制御に関するノード
	case NODE_BLOCK:
		{
			const int32_t in_depth = symbol_open_block(false);

			post_resolver_resolve(self->left);
			generator_genearte_code(self->left, enable_load);

			post_resolver_resolve(self->right);
			generator_genearte_code(self->right, enable_load);

			const int32_t out_depth = symbol_close_block();
			MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
		}
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_BREAK:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(code_get_pc()));
		break;

	case NODE_CASE:
		mana_jump_switch_case(self->left);
		if (self->left && self->left->type) self->type = self->left->type;
		generator_genearte_code(self->right, enable_load);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_CONTINUE:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(code_get_pc()));
		break;

	case NODE_DEFAULT:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		mana_jump_switch_default();
		break;

	case NODE_DO:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_DO);
			const int32_t address = code_get_pc();
			generator_genearte_code(self->left, enable_load);
			mana_jump_close_continue_only();

			code_replace_all(generator_condition(self->right, false), address);
			mana_jump_close_chain();
		}
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_FOR:
		/* 'for(type variable = expression' の形式 */
		{
			//symbol_allocate_memory($2, $1, MEMORY_TYPE_NORMAL);
			//generator_expression(node_create_node(NODE_TYPE_ASSIGN, node_create_leaf($2->name), $4), true);
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
			//$$ = code_get_pc();

			//generator_genearte_code(self->left, enable_load);
			mana_jump_break(generator_condition(self->left, true));

			generator_genearte_code(self->body, enable_load);

			mana_jump_close_continue_only();
			generator_expression(self->right, true);
			code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(code_get_pc()));
			mana_jump_close_chain();
		}
		break;

	case NODE_GOTO:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		{
			symbol_entry* symbol = symbol_create_label(self->string);
			symbol->etc = code_set_opecode_and_operand(MANA_IL_BRA, symbol->etc);
		}
		break;

	case NODE_HALT:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode(MANA_IL_HALT);
		break;

	case NODE_IF:
		{
			int32_t address = generator_condition(self->left, true);
			generator_genearte_code(self->right, enable_load);
			if (self->body)
			{
				const int32_t else_begin_address = code_set_opecode_and_operand(MANA_IL_BRA, -1);
				code_replace_all(address, code_get_pc());
				generator_genearte_code(self->body, enable_load);
				address = else_begin_address;
			}
			code_replace_all(address, code_get_pc());
		}
		break;

	case NODE_LABEL:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		{
			symbol_entry* symbol = symbol_create_label(self->string);
			symbol->address = code_get_pc();
		}
		break;

	case NODE_LOCK:
		mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOCK);
		code_set_opecode(MANA_IL_NONPREEMPTIVE);
		generator_genearte_code(self->left, enable_load);
		mana_jump_close_chain();
		code_set_opecode(MANA_IL_PREEMPTIVE);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_LOOP:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOOP);
			const int32_t address = code_get_pc();
			generator_genearte_code(self->left, enable_load);
			code_set_opecode_and_operand(MANA_IL_BRA, address);
			mana_jump_close_chain();
		}
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_RETURN:
		//generator_genearte_code(self->left, enable_load);
		if (self->left) self->type = self->left->type;
		generator_return(mana_function_symbol_entry_pointer, self);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_ROLLBACK:
		generator_genearte_code(self->left, enable_load);
		if (self->left) self->type = self->left->type;
		generator_rollback(self);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_SWITCH:
		{
			generator_expression(self->left, false);
			const int32_t address = code_get_pc();
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_SWITCH);
			code_set_opecode_and_operand(MANA_IL_BRA, -1);
			mana_jump_open_switch(self->left->type);

			generator_genearte_code(self->right, enable_load);

			code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(code_get_pc()));
			code_replace_all(address, code_get_pc());
			mana_jump_switch_build();
			mana_jump_close_chain();
			mana_jump_close_switch();
		}
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_WHILE:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_WHILE);
			mana_jump_break(generator_condition(self->left, true));
			generator_genearte_code(self->right, enable_load);
			code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(code_get_pc()));
			mana_jump_close_chain();
		}
		MANA_ASSERT(self->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 制御に関するノード
	case NODE_COMPLY:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode(MANA_IL_COMPLY);
		break;

	case NODE_JOIN:
		//generator_resolve_symbol(self);
		symbol_add_join(self->left, self->right);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_PRINT:
		generator_call_print(self->left);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_REFUSE:
		code_set_opecode(MANA_IL_REFUSE);
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_REQUEST:
		symbol_add_request(MANA_IL_REQ, self->left, self->right, self->string);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_YIELD:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode(MANA_IL_YIELD);
		break;

		///////////////////////////////////////////////////////////////////////
		// 二項演算子に関するノード
	case NODE_ADD:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode((self->left->type)->tcons == SYMBOL_DATA_TYPE_FLOAT ? MANA_IL_ADD_FLOAT : MANA_IL_ADD_INTEGER);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_SUB:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode((self->left->type)->tcons == SYMBOL_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_MUL:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode((self->left->type)->tcons == SYMBOL_DATA_TYPE_FLOAT ? MANA_IL_MUL_FLOAT : MANA_IL_MUL_INTEGER);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_DIV:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode((self->left->type)->tcons == SYMBOL_DATA_TYPE_FLOAT ? MANA_IL_DIV_FLOAT : MANA_IL_DIV_INTEGER);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_REM:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode((self->left->type)->tcons == SYMBOL_DATA_TYPE_FLOAT ? MANA_IL_MOD_FLOAT : MANA_IL_MOD_INTEGER);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_POW:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode((self->left->type)->tcons == SYMBOL_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_AND:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode(MANA_IL_AND);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_OR:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode(MANA_IL_OR);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_XOR:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode(MANA_IL_EOR);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_LSH:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode(MANA_IL_SHL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_RSH:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		code_set_opecode(MANA_IL_SHR);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_LAND:
	case NODE_LOR:
	case NODE_EQ:
	case NODE_GE:
	case NODE_GT:
	case NODE_LS:
	case NODE_LE:
	case NODE_NE:
		// 比較、論理演算子
		//generator_automatic_cast(self);
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		MANA_ASSERT(self->body == NULL);
		// TODO:オペコードを設定してください
		code_set_opecode((uint8_t)self->etc);
		break;

		///////////////////////////////////////////////////////////////////////
		// 単項演算子に関するノード
	case NODE_LNOT:
	case NODE_NOT:
		//generator_automatic_cast(self);
		generator_genearte_code(self->left, enable_load);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		// TODO:オペコードを設定してください
		code_set_opecode((uint8_t)self->etc);
		break;

	case NODE_NEG:
		generator_genearte_code(self->left, enable_load);
		code_set_opecode((self->left->type)->tcons == SYMBOL_DATA_TYPE_FLOAT ? MANA_IL_MINUS_FLOAT : MANA_IL_MINUS_INTEGER);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 演算に関するノード
	case NODE_ARRAY:
		/* variable[index] */
		generator_genearte_code(self->right, true);
		code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, (self->type)->memory_size);
		code_set_opecode(MANA_IL_MUL_INTEGER);
		generator_genearte_code(self->left, false);
		code_set_opecode(MANA_IL_ADD_INTEGER);
		if (enable_load)
		{
			generator_resolve_load(self);
		}
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_ASSIGN:
		//generator_resolve_symbol(self->left);
		//generator_resolve_symbol(self->right);

		//generator_automatic_cast(self);
		generator_genearte_code(self->right, enable_load);
		generator_genearte_code(self->left, false);
		generator_resolve_store(self->left);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_CALL:
		// 関数、アクションを呼びます
		//generator_resolve_symbol(self);
		// resolver_search_symbol_from_name(self)
		if (self->symbol)
		{
			//generator_genearte_code(self->right, enable_load);
			generator_call(self);
		}
		break;

	case NODE_CALL_ARGUMENT:
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		break;

	case NODE_CONST:
		switch (self->type->tcons)
		{
		case SYMBOL_DATA_TYPE_CHAR:
		case SYMBOL_DATA_TYPE_SHORT:
		case SYMBOL_DATA_TYPE_INT:
			generator_generate_const_int(self->type->tcons, self->digit);
			break;

		case SYMBOL_DATA_TYPE_FLOAT:
			generator_generate_const_float(self->type->tcons, self->real);
			break;

		case SYMBOL_DATA_TYPE_NIL:
			code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			break;

		default:
			mana_compile_error("illegal type of expression in statement");
			break;
		}
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_EXPRESSION_IF:
		// 三項演算子
		{
			int32_t pc1, pc2;
			generator_condition_core(self->next);
			pc1 = code_set_opecode_and_operand(MANA_IL_BEQ, -1);
			generator_genearte_code(self->left, enable_load);
			pc2 = code_set_opecode_and_operand(MANA_IL_BRA, -1);
			code_replace_all(pc1, code_get_pc());
			generator_genearte_code(self->right, enable_load);
			code_replace_all(pc2, code_get_pc());
		}
		break;

	case NODE_I2F:
		generator_genearte_code(self->left, enable_load);
		code_set_opecode(MANA_IL_INT2FLOAT);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_F2I:
		generator_genearte_code(self->left, enable_load);
		code_set_opecode(MANA_IL_FLOAT2INT);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_IDENTIFIER:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		if (self->symbol)
		{
			switch (self->symbol->class_type)
			{
			case SYMBOL_CLASS_TYPE_ALIAS:
				generator_generate_const_int(self->symbol->type->tcons, self->symbol->address);
				break;

			case SYMBOL_CLASS_TYPE_CONSTANT_INT:
				generator_generate_const_int(self->symbol->type->tcons, self->symbol->address);
				break;

			case SYMBOL_CLASS_TYPE_CONSTANT_FLOAT:
				generator_generate_const_float(self->symbol->type->tcons, self->symbol->hloat);
				break;

			case SYMBOL_CLASS_TYPE_CONSTANT_STRING:
				generator_generate_const_int(self->symbol->type->tcons, self->symbol->address);
				break;

			case SYMBOL_CLASS_TYPE_VARIABLE_STATIC:
				code_set_opecode_and_operand(MANA_IL_LOAD_STATIC_ADDRESS, self->symbol->address);
				break;

			case SYMBOL_CLASS_TYPE_VARIABLE_GLOBAL:
				code_set_opecode_and_operand(MANA_IL_LOAD_GLOBAL_ADDRESS, self->symbol->address);
				break;

			case SYMBOL_CLASS_TYPE_VARIABLE_ACTOR:
				code_set_opecode_and_operand(MANA_IL_LOAD_SELF_ADDRESS, self->symbol->address);
				break;

			case SYMBOL_CLASS_TYPE_VARIABLE_LOCAL:
				code_set_opecode_and_operand(MANA_IL_LOAD_FRAME_ADDRESS, self->symbol->address);
				break;

			case SYMBOL_CLASS_TYPE_TYPEDEF:
				break;

			case SYMBOL_CLASS_TYPE_NEW_SYMBOL:

			default:
				mana_compile_error("illigal data type");
				break;
			}

			if (enable_load)
				generator_resolve_load(self);
		}
		break;

	case NODE_MEMBER_FUNCTION:
		// TODO:実装してください
		generator_genearte_code(self->left, enable_load);
		generator_genearte_code(self->right, enable_load);
		break;

	case NODE_MEMBER_VARIABLE:
		MANA_ASSERT(self->left);
		MANA_ASSERT(self->right == NULL);
		{
			//generator_resolve_symbol(self->left);
			/////resolver_search_symbol_from_name(self);

			type_description* type = self->left->type;
			if (type)
			{
				if (type->tcons == SYMBOL_DATA_TYPE_STRUCT)
				{
					while (type)
					{
						for (symbol_entry* symbol = (symbol_entry*)type->component; symbol; symbol = symbol->next)
						{
							if (strcmp(symbol->name, self->string) == 0 && symbol->class_type == SYMBOL_CLASS_TYPE_VARIABLE_ACTOR)
							{
								// variable.member
								code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, symbol->address);
								generator_genearte_code(self->left, false);
								code_set_opecode(MANA_IL_ADD_INTEGER);
								if (enable_load)
									generator_resolve_load(self);
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
	case NODE_MEMOP:
		/* variable.member */
		code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, self->etc);
		generator_genearte_code(self->left, false);
		code_set_opecode(MANA_IL_ADD_INTEGER);
		if (enable_load)
		{
			generator_resolve_load(self);
		}
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;
#endif
	case NODE_SENDER:
		// senderをpushします
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode(MANA_IL_PUSH_SENDER);
		break;

	case NODE_SELF:
		// selfをpushします
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode(MANA_IL_PUSH_SELF);
		break;

	case NODE_PRIORITY:
		// priorityをpushします
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode(MANA_IL_PUSH_PRIORITY);
		break;

	case NODE_SIZEOF:
		{
			int32_t max_char = (1 << (8 * CBSZ - 1)) - 1;
			int32_t min_char = -1 << (8 * CBSZ - 1);
			int32_t max_short = (1 << (8 * SBSZ - 1)) - 1;
			int32_t min_short = -1 << (8 * SBSZ - 1);

			self->digit = self->left->type->memory_size;

			if (self->digit <= max_char && self->digit >= min_char)
				self->type = type_get(SYMBOL_DATA_TYPE_CHAR);
			else if (self->digit <= max_short && self->digit >= min_short)
				self->type = type_get(SYMBOL_DATA_TYPE_SHORT);
			else
				self->type = type_get(SYMBOL_DATA_TYPE_INT);
		}
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		break;

	case NODE_STRING:
		MANA_ASSERT(self->left == NULL);
		MANA_ASSERT(self->right == NULL);
		MANA_ASSERT(self->body == NULL);
		code_set_opecode_and_operand(MANA_IL_PUSH_STRING, self->digit);
		break;
#if 0
	case NODE_VARIABLE:
		/* variable */
		generator_genearte_code(self->right, enable_load);
		generator_genearte_code(self->left, enable_load);

		//generator_generate_variable_value(self);
		switch ((self->symbol)->class_type)
		{
		case SYMBOL_CLASS_TYPE_VARIABLE_STATIC:
			code_set_opecode_and_operand(MANA_IL_LOAD_STATIC_ADDRESS, (self->symbol)->address);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_GLOBAL:
			code_set_opecode_and_operand(MANA_IL_LOAD_GLOBAL_ADDRESS, (self->symbol)->address);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_ACTOR:
			code_set_opecode_and_operand(MANA_IL_LOAD_SELF_ADDRESS, (self->symbol)->address);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_LOCAL:
			code_set_opecode_and_operand(MANA_IL_LOAD_FRAME_ADDRESS, (self->symbol)->address);
			break;

		case SYMBOL_CLASS_TYPE_TYPEDEF:
			break;

		default:
			mana_compile_error(self, "illegal type of expression in statement");
			break;
		}

		if (enable_load)
		{
			generator_resolve_load(self);
		}

		break;
#endif
	default:
		MANA_BUG("illegal right-hand side value");
		break;
	}

	// 子ノードから型を継承する
	resolver_resolve_type_from_child_node(self);

	if (self->next)
	{
		// 末尾再帰なのでgotoにて処理する
		//generator_genearte_code(self->next);
		self = self->next;
		goto DO_RECURSIVE;
	}
}
