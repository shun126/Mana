/*!
mana (compiler)

@file	pre_resolver.c
@brief	構文木評価に関するソースファイル
@detail	このファイルは構文木評価に関係するソースファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_PRE_RESOLVER_H___)
#include "pre_resolver.h"
#endif
#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#if !defined(___MANA_RESOLVER_H___)
#include "resolver.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif

static struct mana_pre_resolver_object
{
	bool static_block_opend;
}mana_pre_resolver_object;

void mana_pre_resolver_initialize(void)
{
	{
		/* vec2 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_variable("x", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("y", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec2");

		/* vec3 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_variable("x", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("y", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("z", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec3");

		/* vec4 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_variable("x", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("y", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("z", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("w", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec4");

		/* color */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_variable("r", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("g", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("b", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_variable("a", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("color");
	}

	{
		symbol_entry* symbol;

		/* int getUerData() */
		symbol = mana_symbol_create_function("getUserData");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_INT));

		/* void setUserData(int data) */
		symbol = mana_symbol_create_function("setUserData");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("data", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_INT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_VOID));

		/* pointer getUserPointer() */
		symbol = mana_symbol_create_function("getUserPointer");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_pointer);

		/* void setUserPointer(pointer address) */
		symbol = mana_symbol_create_function("setUserPointer");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("address", NULL, false), mana_type_pointer, MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_VOID));

		/* int debug() */
		symbol = mana_symbol_create_function("debug");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_INT));

		/* void setTickCount(int count) */
		symbol = mana_symbol_create_function("setTickCount");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("count", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_INT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_VOID));

		/* void wait(float) */
		symbol = mana_symbol_create_function("wait");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("second", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_VOID));

		/* void waitFrame(int) */
		symbol = mana_symbol_create_function("waitFrame");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("frame", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_INT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_VOID));

		/* void srand(int count) */
		symbol = mana_symbol_create_function("srand");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("count", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_INT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_VOID));

		/* int rand(int, int) */
		symbol = mana_symbol_create_function("rand");
		symbol->number_of_parameters = 2;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("min", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_INT), MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_variable("max", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_INT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_INT));

		/* float frand() */
		symbol = mana_symbol_create_function("frand");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_FLOAT));

		/* float sin(float) */
		symbol = mana_symbol_create_function("sin");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("degree", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_FLOAT));

		/* float cos(float) */
		symbol = mana_symbol_create_function("cos");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("degree", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_FLOAT));

		/* float atan2(float, float) */
		symbol = mana_symbol_create_function("atan2");
		symbol->number_of_parameters = 2;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("y", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_variable("x", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_FLOAT));

		/* float tan(float) */
		symbol = mana_symbol_create_function("tan");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("degree", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_FLOAT));

		/* float angleMod(float) */
		symbol = mana_symbol_create_function("angleMod");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("degree", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_FLOAT));

		/* float sqrt(float) */
		symbol = mana_symbol_create_function("sqrt");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("degree", NULL, false), mana_type_get(SYMBOL_DATA_TYPE_FLOAT), MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_FLOAT));

		/* actor getActor(string) */
		symbol = mana_symbol_create_function("getActor");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_variable("name", NULL, false), mana_type_string, MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(SYMBOL_DATA_TYPE_ACTOR));
	}
}

void mana_pre_resolver_finalize(void)
{
}

/*!
引数の数を取得します（再帰呼び出し）
@param	count	引数の番号
@param	param	引数のsymbol_entry
@param	node	引数のnode_entry
@return	引数の数
*/
static int32_t get_argument_count(const int32_t count, const node_entry* node)
{
	int32_t _count = count;
	if (node)
	{
		MANA_ASSERT(node->id == NODE_DECLARE_ARGUMENT);
		_count = get_argument_count(_count, node->right) + 1;
	}
	return _count;
}


void mana_pre_resolver_resolve(node_entry* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:
	mana_resolver_set_current_file_infomation(node);

	switch (node->id)
	{
		// 特に処理を行わないノード
	case NODE_IDENTIFIER:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		// TODO:post_resolverと確認して下さい
		mana_resolver_search_symbol_from_name(node);
		break;

		// 定数定義に関するノード									
	case NODE_DEFINE_ALIAS:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_create_alias(node->string, node->string);
		break;

	case NODE_DEFINE_CONSTANT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_create_const_int(node->string, node->digit);
		break;

	case NODE_UNDEFINE_CONSTANT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_destroy(node->string);
		break;

		// メモリレイアウトに関するノード									
	case NODE_DECLARE_ALLOCATE:
		{
			const int32_t mana_allocated_size = mana_symbol_get_static_memory_address() + node->digit;

			mana_pre_resolver_resolve(node->left);

			const int32_t address = mana_symbol_get_static_memory_address();
			if (address >= mana_allocated_size)
				mana_compile_error("static variable range over");

			mana_symbol_set_static_memory_address(mana_allocated_size);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_STATIC:
		mana_pre_resolver_object.static_block_opend = true;
		mana_pre_resolver_resolve(node->left);
		mana_pre_resolver_object.static_block_opend = false;
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		// 構造に関するノード
	case NODE_DECLARE_ACTOR:
		{
			mana_symbol_begin_registration_actor(mana_symbol_lookup(node->string));
			mana_pre_resolver_resolve(node->left);
			mana_symbol_commit_registration_actor(node->string, NULL, NULL, false);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_EXTEND:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_extend_module(node->string);
		break;

	case NODE_DECLARE_MODULE:
		{
			mana_symbol_begin_registration_module(mana_symbol_lookup(node->string));
			mana_pre_resolver_resolve(node->left);
			mana_symbol_commit_registration_module(node->string);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_PHANTOM:
		{
			mana_symbol_begin_registration_actor(mana_symbol_lookup(node->string));
			mana_pre_resolver_resolve(node->left);
			mana_symbol_commit_registration_actor(node->string, NULL, NULL, true);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_STRUCT:
		mana_symbol_open_structure();
		mana_pre_resolver_resolve(node->left);
		mana_symbol_close_structure(node->string);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		// 関数宣言に関するノード									
	case NODE_DECLARE_ACTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = mana_type_get(SYMBOL_DATA_TYPE_VOID);
		}
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_ARGUMENT:
		mana_resolver_resolve_variable_description(node->left, MEMORY_TYPE_PARAMETER, mana_pre_resolver_object.static_block_opend);
		mana_pre_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_FUNCTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			// 関数の戻り値を評価
			mana_pre_resolver_resolve(node->left);
			// シンボルの作成と型の定義
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = node->left->type;
			// シンボルに引数の数を登録
			node->symbol->number_of_parameters = get_argument_count(0, node->right);
		}
		// node->body
		break;

	case NODE_DECLARE_NATIVE_FUNCTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			// シンボルの作成と型の定義
			node->symbol = mana_symbol_create_function(node->string);
			mana_symbol_begin_native_function_registration();
			mana_pre_resolver_resolve(node->left);
			mana_pre_resolver_resolve(node->right);
			node->symbol->number_of_parameters = get_argument_count(0, node->right);
			mana_symbol_commit_native_function_registration(node->symbol, node->left->type);
		}
		break;

		// 変数宣言に関するノード									
	case NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_variable(node->string, NULL, mana_pre_resolver_object.static_block_opend);
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_VARIABLE:
		mana_resolver_resolve_variable_description(node, MEMORY_TYPE_NORMAL, mana_pre_resolver_object.static_block_opend);
		MANA_ASSERT(node->left && node->left->id == NODE_TYPE_DESCRIPTION);
		MANA_ASSERT(node->right && node->right->id == NODE_DECLARATOR);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_TYPE_DESCRIPTION:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_resolver_resolve_type_description(node);
		break;

	case NODE_VARIABLE_SIZE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		if (node->string)
		{
			symbol_entry* symbol = mana_symbol_lookup(node->string);
			if (symbol)
				node->digit = symbol->address;
			else
				mana_linker_error("undefined symbol '%s'", node->string);
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// 以下はシンボル生成を行わないノード
		///////////////////////////////////////////////////////////////////////

		// ブロックを伴う制御に関するノード
	case NODE_BLOCK:
	case NODE_BREAK:
	case NODE_CASE:
	case NODE_CONTINUE:
	case NODE_DEFAULT:
	case NODE_DO:
	case NODE_FOR:
	case NODE_GOTO:
	case NODE_HALT:
	case NODE_IF:
	case NODE_LABEL:
	case NODE_LOCK:
	case NODE_LOOP:
	case NODE_RETURN:
	case NODE_ROLLBACK:
	case NODE_SWITCH:
	case NODE_WHILE:

		// 複数の命令に展開されるノード									
	case NODE_COMPLY:
	case NODE_JOIN:
	case NODE_PRINT:
	case NODE_REFUSE:
	case NODE_REQUEST:
	case NODE_STRING:
	case NODE_YIELD:
	case NODE_SIZEOF:

		// 中間言語と対になるノード
	case NODE_ARRAY:
	case NODE_ASSIGN:
	case NODE_CONST:
	case NODE_EXPRESSION_IF:
	case NODE_MEMBER_FUNCTION:
	case NODE_MEMBER_VARIABLE:
	case NODE_CALL:
	case NODE_CALL_ARGUMENT:
	case NODE_ADD:
	case NODE_SUB:
	case NODE_MUL:
	case NODE_DIV:
	case NODE_REM:
	case NODE_NEG:
	case NODE_POW:
	case NODE_NOT:
	case NODE_AND:
	case NODE_OR:
	case NODE_XOR:
	case NODE_LSH:
	case NODE_RSH:
	case NODE_LS:
	case NODE_LE:
	case NODE_EQ:
	case NODE_NE:
	case NODE_GE:
	case NODE_GT:
	case NODE_I2F:
	case NODE_F2I:
	case NODE_LOR:
	case NODE_LAND:
	case NODE_LNOT:
	case NODE_SENDER:
	case NODE_SELF:
	case NODE_PRIORITY:
	default:
		MANA_BUG("illigal node detect");
	}

	// 子ノードから型を継承する
	mana_resolver_resolve_type_from_child_node(node);

	if (node->next)
	{
		// 末尾再帰なのでgotoにて処理する
		//mana_pre_resolver_resolve(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}
