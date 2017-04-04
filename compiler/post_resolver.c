/*!
mana (compiler)

@file	post_resolver.c
@brief	関数またはアクション内のシンボルの解決に関するソースファイル
@detail	関数またはアクション内のシンボルの解決に関係するソースファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_POST_RESOLVER_H___)
#include "post_resolver.h"
#endif
#if !defined(___MANA_DATALINK_GENERATOR_H___)
#include "datalink_generator.h"
#endif
#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_RESOLVER_H___)
#include "resolver.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif
#include <string.h>

static bool get_node_type(symbol_data_type_id* t1, symbol_data_type_id* t2, const node_entry* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(t1);
	MANA_ASSERT(t2);

	if (node->left == NULL || node->left->type == NULL)
	{
		mana_compile_error("illegal left-hand side expression");
		return false;
	}

	if (node->right != NULL && node->right->type == NULL)
	{
		mana_compile_error("illegal right-hand side expression");
		return false;
	}

	*t1 = (node->left->type)->tcons;
	*t2 = node->right ? (node->right->type)->tcons : SYMBOL_DATA_TYPE_INT;

	return true;
}

/*!
自動型変換ノードを挿入します
@param[in]	node	ノードオブジェクト
*/
static void auto_cast(node_entry* node)
{
	if (node == NULL)
		return;

	mana_resolver_resolve_type_from_child_node(node);

	if (node->id != NODE_ASSIGN && node->right && node->right->type && node->right->type->tcons == SYMBOL_DATA_TYPE_FLOAT)
	{
		node->type = node->right->type;
	}

	if (node->left)
	{
		if (node->type->tcons != node->left->type->tcons)
			node->left = mana_node_cast(node->type, node->left);
	}
	if (node->right)
	{
		if (node->type->tcons != node->right->type->tcons)
			node->right = mana_node_cast(node->type, node->right);
	}
}

void mana_post_resolver_resolve(node_entry* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:
	mana_resolver_set_current_file_infomation(node);

	switch (node->id)
	{
		///////////////////////////////////////////////////////////////////////
		// 定数定義に関するノード
	case NODE_DEFINE_ALIAS:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DEFINE_CONSTANT:
		// TODO:将来的には対応して下さい
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_UNDEFINE_CONSTANT:
		// TODO:将来的には対応して下さい
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// メモリレイアウトに関するノード									
	case NODE_DECLARE_ALLOCATE:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_STATIC:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 構造に関するノード
	case NODE_DECLARE_ACTOR:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_EXTEND:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_MODULE:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_PHANTOM:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_STRUCT:
		/*
		TODO:ローカルスコープ内で宣言された時のみ有効にして下さい
		mana_symbol_open_structure();
		mana_post_resolver_resolve(node->left);
		mana_symbol_close_structure(node->string);
		*/
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 関数宣言に関するノード									
	case NODE_DECLARE_ACTION:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_ARGUMENT:
		// node->left
		// node->right
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_FUNCTION:
		// node->left
		// node->right
		// node->body
		break;

	case NODE_DECLARE_NATIVE_FUNCTION:
		// node->left
		// node->right
		// node->body
		break;

		///////////////////////////////////////////////////////////////////////
		// 変数宣言に関するノード									
	case NODE_DECLARATOR:
		if(node->symbol == NULL)
			node->symbol = mana_symbol_create_variable(node->string, NULL, false);
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_VARIABLE:
		// node->left
		// node->right
		mana_resolver_resolve_variable_description(node, MEMORY_TYPE_NORMAL, false);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_TYPE_DESCRIPTION:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_resolver_resolve_type_description(node);
		break;

	case NODE_VARIABLE_SIZE:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ブロックを伴う制御に関するノード
	case NODE_BLOCK:
		{
			const int32_t in_depth = mana_symbol_open_block(false);
			mana_post_resolver_resolve(node->left);
			mana_post_resolver_resolve(node->right);
			const int32_t out_depth = mana_symbol_close_block();
			MANA_VERIFY(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_BREAK:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_CASE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_CONTINUE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DEFAULT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		mana_post_resolver_resolve(node->body);
		break;

	case NODE_DO:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_FOR:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		mana_post_resolver_resolve(node->body);
		break;

	case NODE_GOTO:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_HALT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_IF:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		mana_post_resolver_resolve(node->body);
		break;

	case NODE_LABEL:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_LOCK:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_LOOP:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_RETURN:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_ROLLBACK:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_SWITCH:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_WHILE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 制御に関するノード
	case NODE_COMPLY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_JOIN:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_PRINT:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_REFUSE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_REQUEST:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_YIELD:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// 二項演算子に関するノード
	case NODE_ADD:
	case NODE_SUB:
	case NODE_MUL:
	case NODE_DIV:
	case NODE_REM:
	case NODE_POW:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_FLOAT || t2 < SYMBOL_DATA_TYPE_CHAR || t2 > SYMBOL_DATA_TYPE_FLOAT)
				{
					mana_compile_error("imcompatible type operation in expression");
				}
				else
				{
					auto_cast(node);
					mana_type_compatible(node->left->type, node->right->type);
				}
			}
		}
		break;

	case NODE_AND:
	case NODE_OR:
	case NODE_XOR:
	case NODE_LSH:
	case NODE_RSH:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_INT || t2 < SYMBOL_DATA_TYPE_CHAR || t2 > SYMBOL_DATA_TYPE_INT)
					mana_compile_error("imcompatible type operation in expression");
			}
		}
		break;

	case NODE_LAND:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_INT || t2 < SYMBOL_DATA_TYPE_CHAR || t2 > SYMBOL_DATA_TYPE_INT)
					mana_compile_error("imcompatible type operation in expression");
				node->etc = MANA_IL_LAND;
			}
		}
		break;

	case NODE_LOR:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_INT || t2 < SYMBOL_DATA_TYPE_CHAR || t2 > SYMBOL_DATA_TYPE_INT)
					mana_compile_error("imcompatible type operation in expression");
				node->etc = MANA_IL_LOR;
			}
		}
		break;

	case NODE_EQ:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				mana_type_compatible(node->left->type, node->right->type);
				switch (t1)
				{
				case SYMBOL_DATA_TYPE_CHAR:
				case SYMBOL_DATA_TYPE_SHORT:
				case SYMBOL_DATA_TYPE_INT:
				case SYMBOL_DATA_TYPE_ACTOR:
					node->etc = MANA_IL_COMPARE_EQ_INTEGER;
					break;
				case SYMBOL_DATA_TYPE_FLOAT:
					node->etc = MANA_IL_COMPARE_EQ_FLOAT;
					break;
				case SYMBOL_DATA_TYPE_STRUCT:
					node->etc = MANA_IL_COMPARE_EQ_DATA;
					break;
				default:
					MANA_BUG("illigal data type");
				}
				node->type = mana_type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_GE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				mana_type_compatible(node->left->type, node->right->type);
				switch (t1)
				{
				case SYMBOL_DATA_TYPE_CHAR:
				case SYMBOL_DATA_TYPE_SHORT:
				case SYMBOL_DATA_TYPE_INT:
				case SYMBOL_DATA_TYPE_ACTOR:
					node->etc = MANA_IL_COMPARE_GE_INTEGER;
					break;
				case SYMBOL_DATA_TYPE_FLOAT:
					node->etc = MANA_IL_COMPARE_GE_FLOAT;
					break;
				case SYMBOL_DATA_TYPE_STRUCT:
					node->etc = MANA_IL_COMPARE_GE_DATA;
					break;
				default:
					MANA_BUG("illigal data type");
				}
				node->type = mana_type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_GT:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				mana_type_compatible(node->left->type, node->right->type);
				switch (t1)
				{
				case SYMBOL_DATA_TYPE_CHAR:
				case SYMBOL_DATA_TYPE_SHORT:
				case SYMBOL_DATA_TYPE_INT:
				case SYMBOL_DATA_TYPE_ACTOR:
					node->etc = MANA_IL_COMPARE_GT_INTEGER;
					break;
				case SYMBOL_DATA_TYPE_FLOAT:
					node->etc = MANA_IL_COMPARE_GT_FLOAT;
					break;
				case SYMBOL_DATA_TYPE_STRUCT:
					node->etc = MANA_IL_COMPARE_GT_DATA;
					break;
				default:
					MANA_BUG("illigal data type");
				}
				node->type = mana_type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_LE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				mana_type_compatible(node->left->type, node->right->type);
				switch (t1)
				{
				case SYMBOL_DATA_TYPE_CHAR:
				case SYMBOL_DATA_TYPE_SHORT:
				case SYMBOL_DATA_TYPE_INT:
				case SYMBOL_DATA_TYPE_ACTOR:
					node->etc = MANA_IL_COMPARE_LE_INTEGER;
					break;
				case SYMBOL_DATA_TYPE_FLOAT:
					node->etc = MANA_IL_COMPARE_LE_FLOAT;
					break;
				case SYMBOL_DATA_TYPE_STRUCT:
					node->etc = MANA_IL_COMPARE_LE_DATA;
					break;
				default:
					MANA_BUG("illigal data type");
				}
				node->type = mana_type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_LS:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				mana_type_compatible(node->left->type, node->right->type);
				switch (t1)
				{
				case SYMBOL_DATA_TYPE_CHAR:
				case SYMBOL_DATA_TYPE_SHORT:
				case SYMBOL_DATA_TYPE_INT:
				case SYMBOL_DATA_TYPE_ACTOR:
					node->etc = MANA_IL_COMPARE_LS_INTEGER;
					break;
				case SYMBOL_DATA_TYPE_FLOAT:
					node->etc = MANA_IL_COMPARE_LS_FLOAT;
					break;
				case SYMBOL_DATA_TYPE_STRUCT:
					node->etc = MANA_IL_COMPARE_LS_DATA;
					break;
				default:
					MANA_BUG("illigal data type");
				}
				node->type = mana_type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_NE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				mana_type_compatible(node->left->type, node->right->type);
				switch (t1)
				{
				case SYMBOL_DATA_TYPE_CHAR:
				case SYMBOL_DATA_TYPE_SHORT:
				case SYMBOL_DATA_TYPE_INT:
				case SYMBOL_DATA_TYPE_ACTOR:
					node->etc = MANA_IL_COMPARE_NE_INTEGER;
					break;
				case SYMBOL_DATA_TYPE_FLOAT:
					node->etc = MANA_IL_COMPARE_NE_FLOAT;
					break;
				case SYMBOL_DATA_TYPE_STRUCT:
					node->etc = MANA_IL_COMPARE_NE_DATA;
					break;
				default:
					MANA_BUG("illigal data type");
				}
				node->type = mana_type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// 単項演算子に関するノード
	case NODE_LNOT:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_INT)
					mana_compile_error("imcompatible type operation in expression");
				node->etc = MANA_IL_LNOT;
			}
		}
		break;

	case NODE_NOT:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_INT)
					mana_compile_error("imcompatible type operation in expression");
				node->etc = MANA_IL_NOT;
			}
		}
		break;

	case NODE_NEG:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_FLOAT)
					mana_compile_error("imcompatible type operation in expression");
			}
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// 演算に関するノード
	case NODE_ARRAY:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t2 == SYMBOL_DATA_TYPE_VOID || t2 > SYMBOL_DATA_TYPE_FLOAT)
					mana_compile_error("non-integer expression used as subscript");
				if (t1 != SYMBOL_DATA_TYPE_ARRAY)
					mana_compile_error("subscript specified to non-array");
				else
				{
					if (node->right->id == NODE_CONST)
					{
						assert(
							node->right->type == mana_type_get(SYMBOL_DATA_TYPE_CHAR) ||
							node->right->type == mana_type_get(SYMBOL_DATA_TYPE_SHORT) ||
							node->right->type == mana_type_get(SYMBOL_DATA_TYPE_INT));

						if (node->right->digit >= (node->left->type)->number_of_elements)
							mana_compile_error("subscript range over");
					}

					node->type = (node->left->type)->component;
				}
			}
		}
		break;

	case NODE_ASSIGN:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);

		if (node->left->id == NODE_CONST)
		{
			mana_compile_error("already initialized constant '%s'", node->left->symbol->name);
			return;
		}
		auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		break;

		break;

	case NODE_CALL:
		MANA_ASSERT(node->left == NULL);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_resolver_search_symbol_from_name(node);
#if 0
			mana_generator_event_funtion_type* function = mana_hash_get(mana_generator_object.event_hash, node->symbol->name);
			if (function)
			{
				node = (*function)(node);
			}

			if (node->symbol->class_type != SYMBOL_CLASS_TYPE_FUNCTION &&
				node->symbol->class_type != SYMBOL_CLASS_TYPE_NATIVE_FUNCTION &&
				node->symbol->class_type != SYMBOL_CLASS_TYPE_MEMBER_FUNCTION)
			{
				mana_compile_error("trying to call non-funcation");
			}
#endif
		}
		break;

	case NODE_CALL_ARGUMENT:
		mana_post_resolver_resolve(node->left);
		if (node->right)
		{
			mana_post_resolver_resolve(node->right);
			node->etc = (node->right->id == NODE_CALL_ARGUMENT) ? node->right->etc + 1 : 2;
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_CONST:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_EXPRESSION_IF:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		mana_post_resolver_resolve(node->body);
		break;

	case NODE_I2F:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < SYMBOL_DATA_TYPE_CHAR || t1 > SYMBOL_DATA_TYPE_INT)
					mana_compile_error("imcompatible type operation in expression");
			}
		}
		break;

	case NODE_F2I:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 != SYMBOL_DATA_TYPE_FLOAT)
					mana_compile_error("imcompatible type operation in expression");
			}
		}
		break;

	case NODE_IDENTIFIER:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);

		mana_resolver_search_symbol_from_name(node);

		if (node->symbol)
			node->type = node->symbol->type;

		switch (node->symbol->class_type)
		{
		case SYMBOL_CLASS_TYPE_ALIAS:
			if (!node->symbol->used)
			{
				node->symbol->address = mana_datalink_generator_append(node->symbol->string);
				node->symbol->type = mana_type_get(SYMBOL_DATA_TYPE_INT);
				node->symbol->used = 1;
			}
			break;

		case SYMBOL_CLASS_TYPE_CONSTANT_INT:
			node->symbol->type = mana_type_get(SYMBOL_DATA_TYPE_INT);
			break;

		case SYMBOL_CLASS_TYPE_CONSTANT_FLOAT:
			node->symbol->type = mana_type_get(SYMBOL_DATA_TYPE_FLOAT);
			break;

		case SYMBOL_CLASS_TYPE_CONSTANT_STRING:
			node->symbol->type = mana_type_string;
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_STATIC:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_GLOBAL:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_ACTOR:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_LOCAL:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case SYMBOL_CLASS_TYPE_TYPEDEF:
			break;

		case SYMBOL_CLASS_TYPE_NEW_SYMBOL:
		default:
			mana_compile_error("illigal data type");
			break;
		}
		break;

	case NODE_MEMBER_FUNCTION:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		abort();
		break;

	case NODE_MEMBER_VARIABLE:
		{
			mana_post_resolver_resolve(node->left);
			MANA_ASSERT(node->right == NULL);
			MANA_ASSERT(node->body == NULL);

			type_description* type = node->left->type;
			if (type)
			{
				while (type->tcons == SYMBOL_DATA_TYPE_ARRAY)
				{
					type = type->component;
				}
				if (type->tcons == SYMBOL_DATA_TYPE_STRUCT)
				{
					while (type)
					{
						for (symbol_entry* symbol = (symbol_entry*)type->component; symbol; symbol = symbol->next)
						{
							if (strcmp(symbol->name, node->string) == 0 && symbol->class_type == SYMBOL_CLASS_TYPE_VARIABLE_ACTOR)
							{
								// variable.member
								node->type = symbol->type;
								goto ESCAPE;
							}
						}
						type = type->parent;
					}
					mana_compile_error("reference to undefined field '%s'", node->string);
				}
				else
				{
					mana_compile_error("reference to non-struct type");
				}
			}
		}
	ESCAPE:
		break;

	case NODE_SENDER:
	case NODE_SELF:
	case NODE_PRIORITY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_SIZEOF:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_STRING:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	default:
		MANA_BUG("illigal node detect");
	}

	// 子ノードから型を継承する
	mana_resolver_resolve_type_from_child_node(node);

	if (node->next)
	{
		// 末尾再帰なのでgotoにて処理する
		//mana_post_resolver_resolve(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}
