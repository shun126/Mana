/*
 * mana (compiler)
 *
 * @file	mana_node.c
 * @brief	�Ӗ���̓m�[�h�Ɋւ���\�[�X�t�@�C��
 * @detail	���̃t�@�C���͈Ӗ���̓m�[�h�Ɋ֌W����\�[�X�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
#endif
#if !defined(___MANA_DATALINK_GENERATOR_H___)
#include "mana_datalink_generator.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_NODE_H___)
#include "mana_node.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif
#include <string.h>

static mana_hash* mana_node_event_hash = NULL;
static size_t mana_node_total_size = 0;

/*!
 * �m�[�h�̏�����
 */
void mana_node_initialize(void)
{
	mana_node_event_hash = mana_hash_create();

	mana_node_total_size = 0;
}

/*!
 * �m�[�h�̏I��
 */
void mana_node_finalize(void)
{
	mana_hash_destroy(mana_node_event_hash);
	mana_node_event_hash = NULL;
}

/*!
 * �L���X�g�m�[�h���쐬���܂�
 * @param[in]	type	�L���X�g����^
 * @param[in]	node	�m�[�h�I�u�W�F�N�g
 * @return				�m�[�h�I�u�W�F�N�g
 */
static mana_node* mana_node_create_cast(mana_type_description* type, mana_node* node)
{
	mana_node* new_node;

	if(type->tcons == node->type->tcons)
		return node;

	if(type->tcons == MANA_DATA_TYPE_FLOAT)
	{
		new_node = mana_node_allocate(MANA_NODE_TYPE_I2F);
		new_node->type = mana_type_get(MANA_DATA_TYPE_FLOAT);
		new_node->left = node;
	}
	else
	{
		new_node = mana_node_allocate(MANA_NODE_TYPE_F2I);
		new_node->type = mana_type_get(MANA_DATA_TYPE_INT);
		new_node->left = node;
	}
	return new_node;
}

/*!
 * �L���X�g
 * @param[in]	type	�L���X�g����^
 * @param[in]	node	�m�[�h�I�u�W�F�N�g
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_cast(mana_type_description* type, mana_node* node)
{
	switch((node->type)->tcons)
	{
	case MANA_DATA_TYPE_CHAR:
	case MANA_DATA_TYPE_SHORT:
	case MANA_DATA_TYPE_INT:
		if(type->tcons == MANA_DATA_TYPE_FLOAT)
		{
			if(node->id == MANA_NODE_TYPE_CONST)
			{
				node->real = (float)node->digit;
				node->type = mana_type_get(MANA_DATA_TYPE_FLOAT);
			}else{
				node = mana_node_create_cast(mana_type_get(MANA_DATA_TYPE_FLOAT), node);
			}
		}
		break;

	case MANA_DATA_TYPE_FLOAT:
		switch(type->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
			if(node->id == MANA_NODE_TYPE_CONST)
			{
				node->digit = (int32_t)node->real;
				node->type = mana_type_get(MANA_DATA_TYPE_INT);
			}else{
				node = mana_node_create_cast(mana_type_get(MANA_DATA_TYPE_INT), node);
			}
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
	return node;
}

/*!
 * �����L���X�g
 * @param[in]	node	�m�[�h�I�u�W�F�N�g
 */
void mana_node_auto_cast(mana_node* node)
{
	if(node)
	{
		if(node->id != MANA_NODE_TYPE_ASSIGN && node->right && node->right->type && node->right->type->tcons == MANA_DATA_TYPE_FLOAT)
		{
			node->type = node->right->type;
		}
		if(node->left)
		{
			if(node->type->tcons != node->left->type->tcons)
				node->left = mana_node_cast(node->type, node->left);
		}
		if(node->right)
		{
			if(node->type->tcons != node->right->type->tcons)
				node->right = mana_node_cast(node->type, node->right);
		}
	}
}

/*!
 * �m�[�h�𕡐����܂�
 * @param[in]	org		���ɂȂ�m�[�h�I�u�W�F�N�g
 * @return				���������m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_clone(mana_node* org)
{
	mana_node* node;

	node = NULL;
	if(org)
	{
		node = (mana_node*)mana_malloc(sizeof(mana_node));
		memcpy(node, org, sizeof(mana_node));

		if(org->left)
			node->left = mana_node_clone(org->left);

		if(org->right)
			node->right = mana_node_clone(org->right);
	}
	return node;
}

/*!
 * �m�[�h�쐬
 * @param[in]	id		�m�[�h�^�C�v�ԍ�
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_allocate(mana_node_type_id id)
{
	mana_node* node;

	node = (mana_node*)mana_calloc(1, sizeof(mana_node));
#if defined(_DEBUG)
	static uint32_t count = 0;
	snprintf(node->magic, sizeof(node->magic), "N%d", count);
	++count;
#endif
	node->id = id;

	return node;
}

/*!
 * �m�[�h�J��
 * @param[in]	node	�m�[�h�I�u�W�F�N�g
 */
void mana_node_release(mana_node* node)
{
	if(node)
	{
		if(node->left)
			mana_node_release(node->left);

		if(node->right)
			mana_node_release(node->right);

		if(node->condition)
			mana_node_release(node->condition);

		mana_free(node);
		node = NULL;
	}
}

/*!
 * �֐��m�[�h���쐬���܂�
 * @param[in]	name	�֐���
 * @return				�֐��m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_function(char* name)
{
	mana_node* node;
	mana_symbol_entry* symbol;

	symbol = mana_symbol_lookup_or_create_dummy(name);

	node = mana_node_allocate(MANA_NODE_TYPE_FUNCTION);
	node->symbol = symbol;
	node->type = symbol->type;

	return node;
}

/*!
 * ���[�t���쐬���܂�
 * @param[in]	name	����
 * @return				���[�t�I�u�W�F�N�g
 */
mana_node* mana_node_create_leaf(char* name)
{
	mana_node* node;
	mana_symbol_entry* symbol;

	node = NULL;
	symbol = mana_symbol_lookup_or_create_dummy(name);
	switch(symbol->class_type)
	{
	case MANA_CLASS_TYPE_ALIAS:
		if(!symbol->used)
		{
			symbol->address = mana_datalink_generator_append(symbol->string);
			symbol->used = 1;
		}
		node = mana_node_allocate(MANA_NODE_TYPE_CONST);
		node->digit = symbol->address;
		node->symbol = symbol;
		node->type = mana_type_get(MANA_DATA_TYPE_INT);
		break;

	case MANA_CLASS_TYPE_CONSTANT_INT:
		node = mana_node_allocate(MANA_NODE_TYPE_CONST);
		node->digit = symbol->address;
		node->symbol = symbol;
		node->type = mana_type_get(MANA_DATA_TYPE_INT);
		break;

	case MANA_CLASS_TYPE_CONSTANT_FLOAT:
		node = mana_node_allocate(MANA_NODE_TYPE_CONST);
		node->real = symbol->hloat;
		node->symbol = symbol;
		node->type = mana_type_get(MANA_DATA_TYPE_FLOAT);
		break;

	case MANA_CLASS_TYPE_CONSTANT_STRING:
		node = mana_node_allocate(MANA_NODE_TYPE_CONST);
		node->string = symbol->string;
		node->symbol = symbol;
		node->type = mana_type_string;
		break;

	case MANA_CLASS_TYPE_VARIABLE_STATIC:
	case MANA_CLASS_TYPE_VARIABLE_GLOBAL:
	case MANA_CLASS_TYPE_VARIABLE_ACTOR:
	case MANA_CLASS_TYPE_VARIABLE_LOCAL:
		mana_symbol_is_valid_variable(symbol);
		node = mana_node_allocate(MANA_NODE_TYPE_VARIABLE);
		node->symbol = symbol;
		node->type = symbol->type;
		break;

	case MANA_CLASS_TYPE_TYPEDEF:
		node = mana_node_allocate(MANA_NODE_TYPE_VARIABLE);
		node->symbol = symbol;
		node->type = symbol->type;
		break;

	case MANA_CLASS_TYPE_NEW_SYMBOL:
		break;

	default:
		mana_compile_error("illigal data type");
		break;
	}

	return node;
}

/*!
 * �m�[�h���쐬���܂�
 * @param[in]	id		�m�[�h�^�C�v�ԍ�
 * @param[in]	left	���Ӄm�[�h�I�u�W�F�N�g
 * @param[in]	right	�E�Ӄm�[�h�I�u�W�F�N�g
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_node(mana_node_type_id id, mana_node* left, mana_node* right)
{
	mana_node* node;
	mana_symbol_data_type_id t1, t2;

	if(left == NULL || left->type == NULL)
	{
		mana_compile_error("illegal left-hand side expression");
		return NULL;
	}

	if(right != NULL && right->type == NULL)
	{
		mana_compile_error("illegal right-hand side expression");
		return NULL;
	}

	node = mana_node_allocate(id);
	node->type = left->type;
	node->left = left;
	node->right = right;

	t1 = (node->left->type)->tcons;
	t2 = node->right ? (node->right->type)->tcons : MANA_DATA_TYPE_INT;

	switch(id)
	{
	case MANA_NODE_TYPE_ASSIGN:
		if(left->id == MANA_NODE_TYPE_CONST)
		{
			mana_compile_error("already initialized constant '%s'", left->symbol->name);
			return NULL;
		}
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		break;

	case MANA_NODE_TYPE_ADD:
	case MANA_NODE_TYPE_SUB:
	case MANA_NODE_TYPE_MUL:
	case MANA_NODE_TYPE_DIV:
	case MANA_NODE_TYPE_REM:
	case MANA_NODE_TYPE_POW:
		mana_node_auto_cast(node);
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		if(t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		mana_type_compatible(node->left->type, node->right->type);
		break;

	case MANA_NODE_TYPE_GT:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch(t1)
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

	case MANA_NODE_TYPE_GE:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch(t1)
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

	case MANA_NODE_TYPE_EQ:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch(t1)
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

	case MANA_NODE_TYPE_NE:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch(t1)
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

	case MANA_NODE_TYPE_LS:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch(t1)
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

	case MANA_NODE_TYPE_LE:
		mana_node_auto_cast(node);
		mana_type_compatible(node->left->type, node->right->type);
		switch(t1)
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

	case MANA_NODE_TYPE_NEG:
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_TYPE_NOT:
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_NOT;
		break;

	case MANA_NODE_TYPE_LNOT:
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_LNOT;
		break;

	case MANA_NODE_TYPE_I2F:
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_TYPE_F2I:
		if(t1 != MANA_DATA_TYPE_FLOAT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_TYPE_SENDER:
	case MANA_NODE_TYPE_SELF:
	case MANA_NODE_TYPE_PRIORITY:
	case MANA_NODE_TYPE_EXPRESSION_IF:
		break;

	case MANA_NODE_TYPE_AND:
	case MANA_NODE_TYPE_OR:
	case MANA_NODE_TYPE_XOR:
	case MANA_NODE_TYPE_LSH:
	case MANA_NODE_TYPE_RSH:
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		if(t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		break;

	case MANA_NODE_TYPE_LAND:
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		if(t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_LAND;
		break;

	case MANA_NODE_TYPE_LOR:
		if(t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		if(t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
			mana_compile_error("imcompatible type operation in expression");
		node->etc = MANA_IL_LOR;
		break;

	case MANA_NODE_TYPE_ARRAY:
		if(t2 == MANA_DATA_TYPE_VOID || t2 > MANA_DATA_TYPE_FLOAT)
			mana_compile_error("non-integer expression used as subscript");
		if(t1 != MANA_DATA_TYPE_ARRAY)
			mana_compile_error("subscript specified to non-array");

		if(right->id == MANA_NODE_TYPE_CONST)
		{
			assert(
				right->type == mana_type_get(MANA_DATA_TYPE_CHAR) ||
				right->type == mana_type_get(MANA_DATA_TYPE_SHORT) ||
				right->type == mana_type_get(MANA_DATA_TYPE_INT));

			if(right->digit >= (left->type)->number_of_elements)
				mana_compile_error("subscript range over");
		}

		node->type = (left->type)->component;
		break;

	case MANA_NODE_TYPE_CALL:
		{
			mana_symbol_entry* sp = left->symbol;

			mana_node_event_funtion_type* function = mana_hash_get(mana_node_event_hash, sp->name);
			if(function)
			{
				node = (*function)(node);
			}

			if(	sp->class_type != MANA_CLASS_TYPE_PROTOTYPE_FUNCTION &&
				sp->class_type != MANA_CLASS_TYPE_FUNCTION &&
				sp->class_type != MANA_CLASS_TYPE_NATIVE_FUNCTION &&
				sp->class_type != MANA_CLASS_TYPE_MEMBER_FUNCTION
			)	mana_compile_error("trying to call non-funcation");
		}
		break;

	case MANA_NODE_TYPE_ARGUMENT:
		node->etc = (left->id == MANA_NODE_TYPE_ARGUMENT) ? left->etc + 1 : 2;
		break;

	case MANA_NODE_TYPE_STRING:
		break;

	default:
		MANA_BUG("Illgal node detect\n");
		break;
	}
	return node;
}

/*!
 * �����o�[�m�[�h���쐬���܂�
 * @param[in]	tree	�m�[�h�I�u�W�F�N�g
 * @param[in]	name	�����o�[��
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_member(mana_node* tree, char* name)
{
	mana_type_description* type;

	type = tree->type;

	if(type)
	{
		if(type->tcons == MANA_DATA_TYPE_STRUCT)
		{
			mana_symbol_entry* symbol;

			while(type)
			{
				for(symbol = (mana_symbol_entry*)type->component; symbol != NULL; symbol = symbol->next)
				{
					if(strcmp(symbol->name, name) == 0 && symbol->class_type == MANA_CLASS_TYPE_VARIABLE_ACTOR)
					{
						mana_node* node;
						node = mana_node_allocate(MANA_NODE_TYPE_MEMOP);
						node->etc = symbol->address;
						node->type = symbol->type;
						node->left = tree;
						return node;
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

	return tree;
}

/*!
 * �����o�[�m�[�h���쐬���܂�
 * @param[in]	tree	�m�[�h�I�u�W�F�N�g
 * @param[in]	name	�����o�[��
 * @param[in]	members	�����o�[�m�[�h�I�u�W�F�N�g
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_call_member(mana_node* tree, char* name, mana_node* members)
{
	mana_type_description* type;

	type = tree->type;

	if(type->tcons == MANA_DATA_TYPE_ACTOR)
	{
		mana_node* leaf = members;

		while(leaf && leaf->left && leaf->left->id == MANA_NODE_TYPE_ARGUMENT)
			leaf = leaf->left;

		leaf->left = mana_node_create_node(MANA_NODE_TYPE_ARGUMENT, tree, leaf->left);

		members->etc++;

		tree = mana_node_create_node(MANA_NODE_TYPE_CALL, mana_node_create_function(name), members);
	}
	else
	{
		mana_compile_error("reference to non-actor type");
	}

	return tree;
}

/*!
 * �����m�[�h���쐬���܂�
 * @param[in]	digit	����
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_digit(int32_t digit)
{
	int32_t max_char = (1 << (8 * CBSZ - 1)) - 1;
	int32_t min_char = -1 << (8 * CBSZ - 1);
	int32_t max_short = (1 << (8 * SBSZ - 1)) - 1;
	int32_t min_short = -1 << (8 * SBSZ - 1);
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_TYPE_CONST);
	new_node->digit = digit;
	if(digit <= max_char && digit >= min_char)
		new_node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
	else if(digit <= max_short && digit >= min_short)
		new_node->type = mana_type_get(MANA_DATA_TYPE_SHORT);
	else
		new_node->type = mana_type_get(MANA_DATA_TYPE_INT);

	return new_node;
}

/*!
 * �����m�[�h���쐬���܂�
 * @param[in]	real	����
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_real(float real)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_TYPE_CONST);
	new_node->real = real;
	new_node->type = mana_type_get(MANA_DATA_TYPE_FLOAT);

	return new_node;
}

/*!
 * ������m�[�h���쐬���܂�
 * @param[in]	string	������
 * @return				�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_string(char* string)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_TYPE_STRING);
	new_node->digit = mana_data_set(string);
	new_node->type = mana_type_string;

	return new_node;
}

/*!
 * �������T�C�Y���擾���܂�
 * @param[in]	node	�m�[�h�I�u�W�F�N�g
 * @return				�������T�C�Y
 */
size_t mana_node_get_memory_size(mana_node* node)
{
	switch(node->id)
	{
	case MANA_NODE_TYPE_CONST:			/* �萔 */
	case MANA_NODE_TYPE_VARIABLE:		/* �ϐ� */
		/* �Q�Ƃ�actor���Aactor�̎��̂�����ł���悤�ɂ��� */
		return node->type->tcons == MANA_DATA_TYPE_ACTOR ? sizeof(void*) : node->type->memory_size;

	case MANA_NODE_TYPE_ARRAY:			/* variable[argument] = */
	case MANA_NODE_TYPE_MEMOP:			/* X.variable */
	case MANA_NODE_TYPE_NEG:			/* �}�������] */
		return node->type->tcons == MANA_DATA_TYPE_ACTOR ? sizeof(void*) : node->type->memory_size;
/*		return mana_node_get_memory_size(node->left);	*/

	case MANA_NODE_TYPE_SELF:			/* self (actor) */
	case MANA_NODE_TYPE_PRIORITY:		/* runlevel (integer) */
		return sizeof(int32_t*);

	case MANA_NODE_TYPE_ASSIGN:			/* = */
	case MANA_NODE_TYPE_ARGUMENT:		/* �Ăяo�������� */
	case MANA_NODE_TYPE_INCOMPLETE:		/* �錾�������� */
	case MANA_NODE_TYPE_FUNCTION:		/* �֐� */
	case MANA_NODE_TYPE_CALL:			/* �֐��Ăяo�� */
	case MANA_NODE_TYPE_ADD:			/* ���Z */
	case MANA_NODE_TYPE_SUB:			/* ���Z */
	case MANA_NODE_TYPE_MUL:			/* ��Z */
	case MANA_NODE_TYPE_DIV:			/* ���Z */
	case MANA_NODE_TYPE_REM:			/* �]�� */
	case MANA_NODE_TYPE_POW:			/* �ׂ��� */
	case MANA_NODE_TYPE_NOT:			/* ~ */
	case MANA_NODE_TYPE_LNOT:			/* ! */
	case MANA_NODE_TYPE_AND:			/* & */
	case MANA_NODE_TYPE_OR:				/* | */
	case MANA_NODE_TYPE_XOR:			/* ^ */
	case MANA_NODE_TYPE_LSH:			/* << */
	case MANA_NODE_TYPE_RSH:			/* >> */
	case MANA_NODE_TYPE_LS:				/* < */
	case MANA_NODE_TYPE_LE:				/* <= */
	case MANA_NODE_TYPE_EQ:				/* == */
	case MANA_NODE_TYPE_NE:				/* != */
	case MANA_NODE_TYPE_GE:				/* >= */
	case MANA_NODE_TYPE_GT:				/* > */
	case MANA_NODE_TYPE_STRING:			/* ������ */
	case MANA_NODE_TYPE_I2F:			/* ������������֕ϊ� */
	case MANA_NODE_TYPE_F2I:			/* �������琮���֕ϊ� */
	case MANA_NODE_TYPE_LOR:			/* || */
	case MANA_NODE_TYPE_LAND:			/* && */
	case MANA_NODE_TYPE_SENDER:			/* sender (actor) */
	case MANA_NODE_TYPE_EXPRESSION_IF:	/* �O�����Z�q '?' */
	default:
#if 1
		return node->type->memory_size;
#else
		mana_compile_error("illigal node type detect");
		return 0;
#endif
	}
}

/*!
 * �֐����ɓ��ʂȏ������s�Ȃ����߂̃R�[���o�b�N�֐���o�^���܂�
 * @param[in]	name		�֐���
 * @param[in]	function	�R�[���o�b�N�֐�
 */
void mana_node_add_event(char* name, mana_node_event_funtion_type function)
{
	mana_hash_set(mana_node_event_hash, name, function);
}
