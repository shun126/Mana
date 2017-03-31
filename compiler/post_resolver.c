/*
mana (compiler)

@file	mana_post_resolver.c
@brief	�֐��܂��̓A�N�V�������̃V���{���̉����Ɋւ���\�[�X�t�@�C��
@detail	�֐��܂��̓A�N�V�������̃V���{���̉����Ɋ֌W����\�[�X�t�@�C���ł��B
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_POST_RESOLVER_H___)
#include "post_resolver.h"
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
#include <string.h>

static bool get_node_type(mana_symbol_data_type_id* t1, mana_symbol_data_type_id* t2, const mana_node* node)
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
	*t2 = node->right ? (node->right->type)->tcons : MANA_DATA_TYPE_INT;

	return true;
}

/*!
�����^�ϊ��m�[�h��}�����܂�
@param[in]	node	�m�[�h�I�u�W�F�N�g
*/
static void auto_cast(mana_node* node)
{
	if (node == NULL)
		return;

	mana_resolver_resolve_type_from_child_node(node);

	if (node->id != MANA_NODE_ASSIGN && node->right && node->right->type && node->right->type->tcons == MANA_DATA_TYPE_FLOAT)
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

void mana_post_resolver_resolve(mana_node* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:

	switch (node->id)
	{
		///////////////////////////////////////////////////////////////////////
		// �萔��`�Ɋւ���m�[�h
	case MANA_NODE_DEFINE_ALIAS:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DEFINE_CONSTANT:
		// TODO:�����I�ɂ͑Ή����ĉ�����
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_UNDEFINE_CONSTANT:
		// TODO:�����I�ɂ͑Ή����ĉ�����
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ���������C�A�E�g�Ɋւ���m�[�h									
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
		// �\���Ɋւ���m�[�h
	case MANA_NODE_DECLARE_ACTOR:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_EXTEND:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_MODULE:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_STRUCT:
		/*
		TODO:���[�J���X�R�[�v���Ő錾���ꂽ���̂ݗL���ɂ��ĉ�����
		mana_symbol_open_structure();
		mana_post_resolver_resolve(node->left);
		mana_symbol_close_structure(node->string);
		*/
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// �֐��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARE_ACTION:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_ARGUMENT:
		// node->left
		// node->right
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		// node->left
		// node->right
		// node->body
		break;

	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		// node->left
		// node->right
		// node->body
		break;

		///////////////////////////////////////////////////////////////////////
		// �ϐ��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARATOR:
		if(node->symbol == NULL)
			node->symbol = mana_symbol_create_identification(node->string, NULL, false);
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		// node->left
		// node->right
		mana_resolver_resolve_variable_description(node, MANA_MEMORY_TYPE_NORMAL, false);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_resolver_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		// node->left
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// �u���b�N�𔺂�����Ɋւ���m�[�h
	case MANA_NODE_BLOCK:
		{
			const int32_t in_depth = mana_symbol_open_block(false);
			mana_post_resolver_resolve(node->left);
			mana_post_resolver_resolve(node->right);
			const int32_t out_depth = mana_symbol_close_block();
			MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_BREAK:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_CASE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_CONTINUE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DEFAULT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		mana_post_resolver_resolve(node->body);
		break;

	case MANA_NODE_DO:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_FOR:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		mana_post_resolver_resolve(node->body);
		break;

	case MANA_NODE_GOTO:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_HALT:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_IF:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		mana_post_resolver_resolve(node->body);
		break;

	case MANA_NODE_LABEL:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_LOCK:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_LOOP:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_RETURN:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_ROLLBACK:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_SWITCH:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_WHILE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ����Ɋւ���m�[�h
	case MANA_NODE_COMPLY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_JOIN:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_PRINT:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_REFUSE:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_REQUEST:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_YIELD:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// �񍀉��Z�q�Ɋւ���m�[�h
	case MANA_NODE_ADD:
	case MANA_NODE_SUB:
	case MANA_NODE_MUL:
	case MANA_NODE_DIV:
	case MANA_NODE_REM:
	case MANA_NODE_POW:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT || t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_FLOAT)
				{
					mana_compile_error(node, "imcompatible type operation in expression");
				}
				else
				{
					auto_cast(node);
					mana_type_compatible(node->left->type, node->right->type);
				}
			}
		}
		break;

	case MANA_NODE_AND:
	case MANA_NODE_OR:
	case MANA_NODE_XOR:
	case MANA_NODE_LSH:
	case MANA_NODE_RSH:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT || t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
					mana_compile_error(node, "imcompatible type operation in expression");
			}
		}
		break;

	case MANA_NODE_LAND:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT || t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
					mana_compile_error(node, "imcompatible type operation in expression");
				node->etc = MANA_IL_LAND;
			}
		}
		break;

	case MANA_NODE_LOR:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT || t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
					mana_compile_error(node, "imcompatible type operation in expression");
				node->etc = MANA_IL_LOR;
			}
		}
		break;

	case MANA_NODE_EQ:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
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
			}
		}
		break;

	case MANA_NODE_GE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
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
			}
		}
		break;

	case MANA_NODE_GT:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
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
			}
		}
		break;

	case MANA_NODE_LE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
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
			}
		}
		break;

	case MANA_NODE_LS:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
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
			}
		}
		break;

	case MANA_NODE_NE:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				auto_cast(node);
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
			}
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// �P�����Z�q�Ɋւ���m�[�h
	case MANA_NODE_LNOT:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
					mana_compile_error(node, "imcompatible type operation in expression");
				node->etc = MANA_IL_LNOT;
			}
		}
		break;

	case MANA_NODE_NOT:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
					mana_compile_error(node, "imcompatible type operation in expression");
				node->etc = MANA_IL_NOT;
			}
		}
		break;

	case MANA_NODE_NEG:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
					mana_compile_error(node, "imcompatible type operation in expression");
			}
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// ���Z�Ɋւ���m�[�h
	case MANA_NODE_ARRAY:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t2 == MANA_DATA_TYPE_VOID || t2 > MANA_DATA_TYPE_FLOAT)
					mana_compile_error(node, "non-integer expression used as subscript");
				if (t1 != MANA_DATA_TYPE_ARRAY)
					mana_compile_error(node, "subscript specified to non-array");
				else
				{
					if (node->right->id == MANA_NODE_CONST)
					{
						assert(
							node->right->type == mana_type_get(MANA_DATA_TYPE_CHAR) ||
							node->right->type == mana_type_get(MANA_DATA_TYPE_SHORT) ||
							node->right->type == mana_type_get(MANA_DATA_TYPE_INT));

						if (node->right->digit >= (node->left->type)->number_of_elements)
							mana_compile_error(node, "subscript range over");
					}

					node->type = (node->left->type)->component;
				}
			}
		}
		break;

	case MANA_NODE_ASSIGN:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);

		if (node->left->id == MANA_NODE_CONST)
		{
			mana_compile_error("already initialized constant '%s'", node->left->symbol->name);
			return;
		}
		auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		break;

		break;

	case MANA_NODE_CALL:
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

			if (node->symbol->class_type != MANA_CLASS_TYPE_FUNCTION &&
				node->symbol->class_type != MANA_CLASS_TYPE_NATIVE_FUNCTION &&
				node->symbol->class_type != MANA_CLASS_TYPE_MEMBER_FUNCTION)
			{
				mana_compile_error(node, "trying to call non-funcation");
			}
#endif
		}
		break;

	case MANA_NODE_CALL_ARGUMENT:
		mana_post_resolver_resolve(node->left);
		if (node->right)
		{
			mana_post_resolver_resolve(node->right);
			node->etc = (node->right->id == MANA_NODE_CALL_ARGUMENT) ? node->right->etc + 1 : 2;
		}
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_CONST:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_EXPRESSION_IF:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		mana_post_resolver_resolve(node->body);
		break;

	case MANA_NODE_I2F:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
					mana_compile_error(node, "imcompatible type operation in expression");
			}
		}
		break;

	case MANA_NODE_F2I:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		{
			mana_symbol_data_type_id t1, t2;
			if (get_node_type(&t1, &t2, node))
			{
				if (t1 != MANA_DATA_TYPE_FLOAT)
					mana_compile_error(node, "imcompatible type operation in expression");
			}
		}
		break;

	case MANA_NODE_IDENTIFIER:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);

		mana_resolver_search_symbol_from_name(node);

		if (node->symbol)
			node->type = node->symbol->type;

		switch (node->symbol->class_type)
		{
		case MANA_CLASS_TYPE_ALIAS:
			if (!node->symbol->used)
			{
				node->symbol->address = mana_datalink_generator_append(node->symbol->string);
				node->symbol->type = mana_type_get(MANA_DATA_TYPE_INT);
				node->symbol->used = 1;
			}
			break;

		case MANA_CLASS_TYPE_CONSTANT_INT:
			node->symbol->type = mana_type_get(MANA_DATA_TYPE_INT);
			break;

		case MANA_CLASS_TYPE_CONSTANT_FLOAT:
			node->symbol->type = mana_type_get(MANA_DATA_TYPE_FLOAT);
			break;

		case MANA_CLASS_TYPE_CONSTANT_STRING:
			node->symbol->type = mana_type_string;
			break;

		case MANA_CLASS_TYPE_VARIABLE_STATIC:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case MANA_CLASS_TYPE_VARIABLE_GLOBAL:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case MANA_CLASS_TYPE_VARIABLE_ACTOR:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case MANA_CLASS_TYPE_VARIABLE_LOCAL:
			mana_symbol_is_valid_variable(node->symbol);
			break;

		case MANA_CLASS_TYPE_TYPEDEF:
			break;

		case MANA_CLASS_TYPE_NEW_SYMBOL:
		default:
			mana_compile_error("illigal data type");
			break;
		}
		break;

	case MANA_NODE_MEMBER_FUNCTION:
		mana_post_resolver_resolve(node->left);
		mana_post_resolver_resolve(node->right);
		MANA_ASSERT(node->body == NULL);
		abort();
		break;

	case MANA_NODE_MEMBER_VARIABLE:
		{
			mana_post_resolver_resolve(node->left);
			MANA_ASSERT(node->right == NULL);
			MANA_ASSERT(node->body == NULL);

			mana_type_description* type = node->left->type;
			if (type)
			{
				while (type->tcons == MANA_DATA_TYPE_ARRAY)
				{
					type = type->component;
				}
				if (type->tcons == MANA_DATA_TYPE_STRUCT)
				{
					while (type)
					{
						for (mana_symbol_entry* symbol = (mana_symbol_entry*)type->component; symbol; symbol = symbol->next)
						{
							if (strcmp(symbol->name, node->string) == 0 && symbol->class_type == MANA_CLASS_TYPE_VARIABLE_ACTOR)
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

	case MANA_NODE_SENDER:
	case MANA_NODE_SELF:
	case MANA_NODE_PRIORITY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_SIZEOF:
		mana_post_resolver_resolve(node->left);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_STRING:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	default:
		MANA_BUG("illigal node detect");
	}

	// �q�m�[�h����^���p������
	mana_resolver_resolve_type_from_child_node(node);

	if (node->next)
	{
		// �����ċA�Ȃ̂�goto�ɂď�������
		//mana_post_resolver_resolve(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}
