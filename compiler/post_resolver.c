/*!
mana (compiler)

@file	post_resolver.c
@brief	�֐��܂��̓A�N�V�������̃V���{���̉����Ɋւ���\�[�X�t�@�C��
@detail	�֐��܂��̓A�N�V�������̃V���{���̉����Ɋ֌W����\�[�X�t�@�C���ł��B
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
�����^�ϊ��m�[�h��}�����܂�
@param[in]	node	�m�[�h�I�u�W�F�N�g
*/
static void auto_cast(node_entry* node)
{
	if (node == NULL)
		return;

	resolver_resolve_type_from_child_node(node);

	if (node->id != NODE_ASSIGN && node->right && node->right->type && node->right->type->tcons == SYMBOL_DATA_TYPE_FLOAT)
	{
		node->type = node->right->type;
	}

	if (node->left)
	{
		if (node->type->tcons != node->left->type->tcons)
			node->left = node_cast(node->type, node->left);
	}
	if (node->right)
	{
		if (node->type->tcons != node->right->type->tcons)
			node->right = node_cast(node->type, node->right);
	}
}

void post_resolver_resolve(node_entry* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:
	resolver_set_current_file_infomation(node);

	switch (node->id)
	{
		///////////////////////////////////////////////////////////////////////
		// �萔��`�Ɋւ���m�[�h
	case NODE_DEFINE_ALIAS:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DEFINE_CONSTANT:
		// TODO:�����I�ɂ͑Ή����ĉ�����
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_UNDEFINE_CONSTANT:
		// TODO:�����I�ɂ͑Ή����ĉ�����
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ���������C�A�E�g�Ɋւ���m�[�h									
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
		// �\���Ɋւ���m�[�h
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
		TODO:���[�J���X�R�[�v���Ő錾���ꂽ���̂ݗL���ɂ��ĉ�����
		symbol_open_structure();
		post_resolver_resolve(node->left);
		symbol_close_structure(node->string);
		*/
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// �֐��錾�Ɋւ���m�[�h									
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
		// �ϐ��錾�Ɋւ���m�[�h									
	case NODE_DECLARATOR:
		if(node->symbol == NULL)
			node->symbol = symbol_create_variable(node->string, NULL, false);
		post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_DECLARE_VARIABLE:
		// node->left
		// node->right
		resolver_resolve_variable_description(node, MEMORY_TYPE_NORMAL, false);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_TYPE_DESCRIPTION:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		resolver_resolve_type_description(node);
		break;

	case NODE_VARIABLE_SIZE:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// �u���b�N�𔺂�����Ɋւ���m�[�h
	case NODE_BLOCK:
		{
			const int32_t in_depth = symbol_open_block(false);
			post_resolver_resolve(node->left);
			post_resolver_resolve(node->right);
			const int32_t out_depth = symbol_close_block();
			MANA_VERIFY_MESSAGE(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_BREAK:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_CASE:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
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
		post_resolver_resolve(node->body);
		break;

	case NODE_DO:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_FOR:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		post_resolver_resolve(node->body);
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
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		post_resolver_resolve(node->body);
		break;

	case NODE_LABEL:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_LOCK:
		post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_LOOP:
		post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_RETURN:
		post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_ROLLBACK:
		post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_SWITCH:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_WHILE:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ����Ɋւ���m�[�h
	case NODE_COMPLY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_JOIN:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_PRINT:
		post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_REFUSE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_REQUEST:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case NODE_YIELD:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// �񍀉��Z�q�Ɋւ���m�[�h
	case NODE_ADD:
	case NODE_SUB:
	case NODE_MUL:
	case NODE_DIV:
	case NODE_REM:
	case NODE_POW:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
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
					type_compatible(node->left->type, node->right->type);
				}
			}
		}
		break;

	case NODE_AND:
	case NODE_OR:
	case NODE_XOR:
	case NODE_LSH:
	case NODE_RSH:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
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
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
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
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
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
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				type_compatible(node->left->type, node->right->type);
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
				node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_GE:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				type_compatible(node->left->type, node->right->type);
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
				node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_GT:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				type_compatible(node->left->type, node->right->type);
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
				node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_LE:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				type_compatible(node->left->type, node->right->type);
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
				node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_LS:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				type_compatible(node->left->type, node->right->type);
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
				node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

	case NODE_NE:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
				type_compatible(node->left->type, node->right->type);
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
				node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
			}
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// �P�����Z�q�Ɋւ���m�[�h
	case NODE_LNOT:
		post_resolver_resolve(node->left);
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
		post_resolver_resolve(node->left);
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
		post_resolver_resolve(node->left);
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
		// ���Z�Ɋւ���m�[�h
	case NODE_ARRAY:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
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
							node->right->type == type_get(SYMBOL_DATA_TYPE_CHAR) ||
							node->right->type == type_get(SYMBOL_DATA_TYPE_SHORT) ||
							node->right->type == type_get(SYMBOL_DATA_TYPE_INT));

						if (node->right->digit >= (node->left->type)->number_of_elements)
							mana_compile_error("subscript range over");
					}

					node->type = (node->left->type)->component;
				}
			}
		}
		break;

	case NODE_ASSIGN:
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);

		if (node->left->id == NODE_CONST)
		{
			mana_compile_error("already initialized constant '%s'", node->left->symbol->name);
			return;
		}
		auto_cast(node);
		type_compatible(node->left->type, node->right->type);
		break;

		break;

	case NODE_CALL:
		MANA_ASSERT(node->left == NULL);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			resolver_search_symbol_from_name(node);
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
		post_resolver_resolve(node->left);
		if (node->right)
		{
			post_resolver_resolve(node->right);
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
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		post_resolver_resolve(node->body);
		break;

	case NODE_I2F:
		post_resolver_resolve(node->left);
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
		post_resolver_resolve(node->left);
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

		resolver_search_symbol_from_name(node);

		if (node->symbol)
			node->type = node->symbol->type;

		switch (node->symbol->class_type)
		{
		case SYMBOL_CLASS_TYPE_ALIAS:
			if (!node->symbol->used)
			{
				node->symbol->address = datalink_generator_append(node->symbol->string);
				node->symbol->type = type_get(SYMBOL_DATA_TYPE_INT);
				node->symbol->used = 1;
			}
			break;

		case SYMBOL_CLASS_TYPE_CONSTANT_INT:
			node->symbol->type = type_get(SYMBOL_DATA_TYPE_INT);
			break;

		case SYMBOL_CLASS_TYPE_CONSTANT_FLOAT:
			node->symbol->type = type_get(SYMBOL_DATA_TYPE_FLOAT);
			break;

		case SYMBOL_CLASS_TYPE_CONSTANT_STRING:
			node->symbol->type = type_string;
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_STATIC:
			symbol_is_valid_variable(node->symbol);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_GLOBAL:
			symbol_is_valid_variable(node->symbol);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_ACTOR:
			symbol_is_valid_variable(node->symbol);
			break;

		case SYMBOL_CLASS_TYPE_VARIABLE_LOCAL:
			symbol_is_valid_variable(node->symbol);
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
		post_resolver_resolve(node->left);
		post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		abort();
		break;

	case NODE_MEMBER_VARIABLE:
		{
			post_resolver_resolve(node->left);
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
		post_resolver_resolve(node->left);
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

	// �q�m�[�h����^���p������
	resolver_resolve_type_from_child_node(node);

	if (node->next)
	{
		// �����ċA�Ȃ̂�goto�ɂď�������
		//post_resolver_resolve(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}
