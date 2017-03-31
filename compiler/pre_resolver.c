/*
mana (compiler)

@file	mana_pre_resolver.c
@brief	構文木評価に関するソースファイル
@detail	このファイルは構文木評価に関係するソースファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_PRE_RESOLVER_H___)
#include "pre_resolver.h"
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
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec2");

		/* vec3 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec3");

		/* vec4 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("w", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec4");

		/* color */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("r", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("g", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("b", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("a", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("color");
	}

	{
		mana_symbol_entry* symbol;

		/* int getUerData() */
		symbol = mana_symbol_create_function("getUserData");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setUserData(int data) */
		symbol = mana_symbol_create_function("setUserData");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("data", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* pointer getUserPointer() */
		symbol = mana_symbol_create_function("getUserPointer");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_pointer);

		/* void setUserPointer(pointer address) */
		symbol = mana_symbol_create_function("setUserPointer");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("address", NULL, false), mana_type_pointer, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int debug() */
		symbol = mana_symbol_create_function("debug");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setTickCount(int count) */
		symbol = mana_symbol_create_function("setTickCount");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void wait(float) */
		symbol = mana_symbol_create_function("wait");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("second", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void waitFrame(int) */
		symbol = mana_symbol_create_function("waitFrame");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("frame", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void srand(int count) */
		symbol = mana_symbol_create_function("srand");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int rand(int, int) */
		symbol = mana_symbol_create_function("rand");
		symbol->number_of_parameters = 2;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("min", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("max", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* float frand() */
		symbol = mana_symbol_create_function("frand");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sin(float) */
		symbol = mana_symbol_create_function("sin");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float cos(float) */
		symbol = mana_symbol_create_function("cos");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float atan2(float, float) */
		symbol = mana_symbol_create_function("atan2");
		symbol->number_of_parameters = 2;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float tan(float) */
		symbol = mana_symbol_create_function("tan");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float angleMod(float) */
		symbol = mana_symbol_create_function("angleMod");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sqrt(float) */
		symbol = mana_symbol_create_function("sqrt");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* actor getActor(string) */
		symbol = mana_symbol_create_function("getActor");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("name", NULL, false), mana_type_string, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_ACTOR));
	}
}

void mana_pre_resolver_finalize(void)
{
}

/*!
引数の数を取得します（再帰呼び出し）
@param	count	引数の番号
@param	param	引数のmana_symbol_entry
@param	node	引数のmana_node
@return	引数の数
*/
static int32_t get_argument_count(const int32_t count, const mana_node* node)
{
	int32_t _count = count;
	if (node)
	{
		MANA_ASSERT(node->id == MANA_NODE_DECLARE_ARGUMENT);
		_count = get_argument_count(_count, node->right) + 1;
	}
	return _count;
}


void mana_pre_resolver_resolve(mana_node* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:

	switch (node->id)
	{
		// 特に処理を行わないノード
	case MANA_NODE_IDENTIFIER:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		// TODO:post_resolverと確認して下さい
		mana_resolver_search_symbol_from_name(node);
		break;

		// 定数定義に関するノード									
	case MANA_NODE_DEFINE_ALIAS:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_create_alias(node->string, node->string);
		break;

	case MANA_NODE_DEFINE_CONSTANT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_create_const_int(node->string, node->digit);
		break;

	case MANA_NODE_UNDEFINE_CONSTANT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_destroy(node->string);
		break;

		// メモリレイアウトに関するノード									
	case MANA_NODE_DECLARE_ALLOCATE:
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

	case MANA_NODE_DECLARE_STATIC:
		mana_pre_resolver_object.static_block_opend = true;
		mana_pre_resolver_resolve(node->left);
		mana_pre_resolver_object.static_block_opend = false;
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		// 構造に関するノード
	case MANA_NODE_DECLARE_ACTOR:
		{
			mana_symbol_begin_registration_actor(mana_symbol_lookup(node->string));
			mana_pre_resolver_resolve(node->left);
			mana_symbol_commit_registration_actor(node->string, NULL, NULL, false);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_EXTEND:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_symbol_extend_module(node->string);
		break;

	case MANA_NODE_DECLARE_MODULE:
		{
			mana_symbol_begin_registration_module(mana_symbol_lookup(node->string));
			mana_pre_resolver_resolve(node->left);
			mana_symbol_commit_registration_module(node->string);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		{
			mana_symbol_begin_registration_actor(mana_symbol_lookup(node->string));
			mana_pre_resolver_resolve(node->left);
			mana_symbol_commit_registration_actor(node->string, NULL, NULL, true);
		}
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_STRUCT:
		mana_symbol_open_structure();
		mana_pre_resolver_resolve(node->left);
		mana_symbol_close_structure(node->string);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		// 関数宣言に関するノード									
	case MANA_NODE_DECLARE_ACTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = mana_type_get(MANA_DATA_TYPE_VOID);
		}
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_ARGUMENT:
		mana_resolver_resolve_variable_description(node->left, MANA_MEMORY_TYPE_PARAMETER, mana_pre_resolver_object.static_block_opend);
		mana_pre_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_FUNCTION:
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

	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
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
	case MANA_NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, mana_pre_resolver_object.static_block_opend);
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_resolver_resolve_variable_description(node, MANA_MEMORY_TYPE_NORMAL, mana_pre_resolver_object.static_block_opend);
		MANA_ASSERT(node->left && node->left->id == MANA_NODE_TYPE_DESCRIPTION);
		MANA_ASSERT(node->right && node->right->id == MANA_NODE_DECLARATOR);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_resolver_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		if (node->string)
		{
			mana_symbol_entry* symbol = mana_symbol_lookup(node->string);
			if (symbol)
				node->digit = symbol->address;
			else
				mana_generator_error("undefined symbol '%s'", node->string);
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// 以下はシンボル生成を行わないノード
		///////////////////////////////////////////////////////////////////////

		// ブロックを伴う制御に関するノード
	case MANA_NODE_BLOCK:
	case MANA_NODE_BREAK:
	case MANA_NODE_CASE:
	case MANA_NODE_CONTINUE:
	case MANA_NODE_DEFAULT:
	case MANA_NODE_DO:
	case MANA_NODE_FOR:
	case MANA_NODE_GOTO:
	case MANA_NODE_HALT:
	case MANA_NODE_IF:
	case MANA_NODE_LABEL:
	case MANA_NODE_LOCK:
	case MANA_NODE_LOOP:
	case MANA_NODE_RETURN:
	case MANA_NODE_ROLLBACK:
	case MANA_NODE_SWITCH:
	case MANA_NODE_WHILE:

		// 複数の命令に展開されるノード									
	case MANA_NODE_COMPLY:
	case MANA_NODE_JOIN:
	case MANA_NODE_PRINT:
	case MANA_NODE_REFUSE:
	case MANA_NODE_REQUEST:
	case MANA_NODE_STRING:
	case MANA_NODE_YIELD:
	case MANA_NODE_SIZEOF:

		// 中間言語と対になるノード
	case MANA_NODE_ARRAY:
	case MANA_NODE_ASSIGN:
	case MANA_NODE_CONST:
	case MANA_NODE_EXPRESSION_IF:
	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
	case MANA_NODE_CALL:
	case MANA_NODE_CALL_ARGUMENT:
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
	case MANA_NODE_I2F:
	case MANA_NODE_F2I:
	case MANA_NODE_LOR:
	case MANA_NODE_LAND:
	case MANA_NODE_LNOT:
	case MANA_NODE_SENDER:
	case MANA_NODE_SELF:
	case MANA_NODE_PRIORITY:
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
