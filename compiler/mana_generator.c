/*!
mana (compiler)

@file	mana_generator.c
@brief	�R���p�C���Z�N�V�����Ɋւ���\�[�X�t�@�C��
@detail	���̃t�@�C���̓R���p�C���Z�N�V�����Ɋ֌W����\�[�X�t�@�C���ł��B
@author	Shun Moriya
@date	2017-
*/

#if !defined(__MANA_GENERATOR_H___)
#include "mana_generator.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "mana_jump.h"
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
#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
#endif
#if !defined(___MANA_RESOLVER_H___)
#include "mana_resolver.h"
#endif
#if !defined(___MANA_POST_RESOLVER_H___)
#include "mana_post_resolver.h"
#endif

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
�֐����ɓ��ʂȏ������s�Ȃ����߂̃R�[���o�b�N�֐���o�^���܂�
@param[in]	name		�֐���
@param[in]	function	�R�[���o�b�N�֐�
*/
void mana_generator_add_event(const char* name, mana_generator_event_funtion_type function)
{
	mana_hash_set(mana_generator_object.event_hash, name, function);
}


/*!
�����̐����擾���܂��i�ċA�Ăяo���j
@param	count	�����̔ԍ�
@param	param	������mana_symbol_entry
@param	arg		������mana_node
@return	�����̐�
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



////////////////////////////////////////////////////////////////////////////////
// �ȉ��� �֐����̃V���{������������ �֐��S
////////////////////////////////////////////////////////////////////////////////
#if 0
static void mana_generator_resolve_symbol(mana_node* node)
{
	if(node == NULL)
		return;

	mana_generator_resolve_symbol(node->left);
	mana_generator_resolve_symbol(node->right);

	switch(node->id)
	{
		// �萔��`�Ɋւ���m�[�h
	case MANA_NODE_DEFINE_ALIAS:
	case MANA_NODE_DEFINE_CONSTANT:
	case MANA_NODE_UNDEFINE_CONSTANT:
		// ���������C�A�E�g�Ɋւ���m�[�h
	case MANA_NODE_DECLARE_ALLOCATE:
	case MANA_NODE_DECLARE_STATIC:
		// �\���Ɋւ���m�[�h
	case MANA_NODE_DECLARE_ACTOR:
	case MANA_NODE_DECLARE_EXTEND:
	case MANA_NODE_DECLARE_MODULE:
	case MANA_NODE_DECLARE_PHANTOM:
	case MANA_NODE_DECLARE_STRUCT:
		// �֐��錾�Ɋւ���m�[�h
	case MANA_NODE_DECLARE_ACTION:
	case MANA_NODE_DECLARE_ARGUMENT:
	case MANA_NODE_DECLARE_FUNCTION:
	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		MANA_BUG("illigal node detect");

		// �ϐ��錾�Ɋւ���m�[�h
	case MANA_NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, mana_generator_object.static_block_opend);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_resolver_resolve_variable_description(node, MANA_MEMORY_TYPE_NORMAL);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_resolver_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		mana_pre_resolver_resolve(node->left);
		break;

		// �u���b�N�𔺂�����Ɋւ���m�[�h
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
		// ����Ɋւ���m�[�h
	case MANA_NODE_COMPLY:
	case MANA_NODE_JOIN:
	case MANA_NODE_PRINT:
	case MANA_NODE_REFUSE:
	case MANA_NODE_REQUEST:
	case MANA_NODE_STRING:
	case MANA_NODE_YIELD:
		// ���Z�Ɋւ���m�[�h
	case MANA_NODE_ADD:
	case MANA_NODE_AND:
	case MANA_NODE_ARRAY:
	case MANA_NODE_ASSIGN:
	case MANA_NODE_CALL:
	case MANA_NODE_CALL_ARGUMENT:
	case MANA_NODE_CONST:
	case MANA_NODE_DIV:
	case MANA_NODE_EQ:
	case MANA_NODE_EXPRESSION_IF:
	case MANA_NODE_F2I:
	case MANA_NODE_GE:
	case MANA_NODE_GT:
	case MANA_NODE_I2F:
		break;
	case MANA_NODE_IDENTIFIER:
		mana_resolver_search_symbol_from_name(node);
		break;
	case MANA_NODE_LAND:
	case MANA_NODE_LE:
	case MANA_NODE_LNOT:
	case MANA_NODE_LOR:
	case MANA_NODE_LS:
	case MANA_NODE_LSH:
	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
	case MANA_NODE_MUL:
	case MANA_NODE_NE:
	case MANA_NODE_NEG:
	case MANA_NODE_NOT:
	case MANA_NODE_OR:
	case MANA_NODE_POW:
	case MANA_NODE_PRIORITY:
	case MANA_NODE_REM:
	case MANA_NODE_RSH:
	case MANA_NODE_SELF:
	case MANA_NODE_SENDER:
	case MANA_NODE_SIZEOF:
	case MANA_NODE_SUB:
	case MANA_NODE_XOR:
		break;

	default:
		MANA_BUG("illigal node detect");
	}

	// �q�m�[�h����^���p������
	mana_resolver_resolve_type_from_child_node(node);
}
#else
/*!
�m�[�h�����f���Ȃ���V���{���ƃ^�C�v�����������܂�
@param[in]	node	�e�m�[�h
*/
static void mana_generator_resolve_symbol(mana_node* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:

	mana_generator_resolve_symbol(node->left);
	mana_generator_resolve_symbol(node->right);

	switch (node->id)
	{
		// ���ɏ������s��Ȃ��m�[�h
	case MANA_NODE_IDENTIFIER:
		mana_resolver_search_symbol_from_name(node);
		break;

		// �萔��`�Ɋւ���m�[�h
	case MANA_NODE_DEFINE_ALIAS:
	case MANA_NODE_DEFINE_CONSTANT:
	case MANA_NODE_UNDEFINE_CONSTANT:

		// ���������C�A�E�g�Ɋւ���m�[�h									
	case MANA_NODE_DECLARE_ALLOCATE:
	case MANA_NODE_DECLARE_STATIC:

		// �\���Ɋւ���m�[�h
	case MANA_NODE_DECLARE_ACTOR:
	case MANA_NODE_DECLARE_EXTEND:
	case MANA_NODE_DECLARE_MODULE:
	case MANA_NODE_DECLARE_PHANTOM:
	case MANA_NODE_DECLARE_STRUCT:

		// �֐��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARE_ACTION:
	case MANA_NODE_DECLARE_ARGUMENT:
	case MANA_NODE_DECLARE_FUNCTION:
	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		MANA_BUG("illigal node detect");
		break;

		// �ϐ��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARATOR:
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		break;

	case MANA_NODE_VARIABLE_SIZE:
		MANA_ASSERT(node->left);
		MANA_ASSERT(node->right);
		abort();
		break;

		///////////////////////////////////////////////////////////////////////
		// �ȉ��̓V���{���������s��Ȃ��m�[�h
		///////////////////////////////////////////////////////////////////////

		// �u���b�N�𔺂�����Ɋւ���m�[�h
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

		// �����̖��߂ɓW�J�����m�[�h									
	case MANA_NODE_COMPLY:
	case MANA_NODE_JOIN:
	case MANA_NODE_PRINT:
	case MANA_NODE_REFUSE:
	case MANA_NODE_REQUEST:
	case MANA_NODE_STRING:
	case MANA_NODE_YIELD:

		// ���Ԍ���Ƒ΂ɂȂ�m�[�h
	case MANA_NODE_SIZEOF:
	case MANA_NODE_ARRAY:
	case MANA_NODE_ASSIGN:
	case MANA_NODE_CONST:
	case MANA_NODE_EXPRESSION_IF:
	case MANA_NODE_MEMBER_FUNCTION:
		break;

	case MANA_NODE_MEMBER_VARIABLE:
		{
			MANA_ASSERT(node->left);
			MANA_ASSERT(node->right == NULL);

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
		break;

	default:
		MANA_BUG("illigal node detect");
	}

	// �q�m�[�h����^���p������
	mana_resolver_resolve_type_from_child_node(node);

	if (node->next)
	{
		// �����ċA�Ȃ̂�goto�ɂď�������
		//mana_generator_resolve_symbol(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}
#endif

////////////////////////////////////////////////////////////////////////////////
// �ȉ��� �m�[�h�̌^�ɂ��킹�Č^�ϊ�������m�[�h���������ފ֐��S
////////////////////////////////////////////////////////////////////////////////

static bool mana_generator_get_both_node_type(mana_symbol_data_type_id* t1, mana_symbol_data_type_id* t2, const mana_node* node)
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


/*
�^�ϊ����K�v�Ȃ�΁A�^�ϊ��̃m�[�h��}�����܂�
@param[in]	node	�m�[�h
*/
static void mana_generator_automatic_cast(mana_node* node)
{
	if (node == NULL)
		return;

	mana_generator_automatic_cast(node->left);
	mana_generator_automatic_cast(node->right);

	if (node->left)
		node->type = node->left->type;

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
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			mana_node_auto_cast(node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
				mana_compile_error("imcompatible type operation in expression");
			if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_FLOAT)
				mana_compile_error(node, "imcompatible type operation in expression");
			mana_type_compatible(node->left->type, node->right->type);
		}
		break;

	case MANA_NODE_GT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
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
		}
		break;

	case MANA_NODE_GE:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
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
		}
		break;

	case MANA_NODE_EQ:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
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
		}
		break;

	case MANA_NODE_NE:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
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
		}
		break;

	case MANA_NODE_LS:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);

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
		}
		break;

	case MANA_NODE_LE:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
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
		}
		break;

	case MANA_NODE_NEG:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_NOT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_NOT;
		}
		break;

	case MANA_NODE_LNOT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_LNOT;
		}
		break;

	case MANA_NODE_I2F:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_F2I:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 != MANA_DATA_TYPE_FLOAT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_AND:
	case MANA_NODE_OR:
	case MANA_NODE_XOR:
	case MANA_NODE_LSH:
	case MANA_NODE_RSH:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_LAND:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_LAND;
		}
		break;

	case MANA_NODE_LOR:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_LOR;
		}
		break;

	case MANA_NODE_ARRAY:
		{
			mana_symbol_data_type_id t1, t2;
			mana_generator_get_both_node_type(&t1, &t2, node);
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
		break;

	case MANA_NODE_CALL:
		{
			mana_resolver_search_symbol_from_name(node);

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
		}
		break;

	case MANA_NODE_CALL_ARGUMENT:
		node->etc = (node->right->id == MANA_NODE_CALL_ARGUMENT) ? node->right->etc + 1 : 2;
		break;

	case MANA_NODE_IDENTIFIER:
		mana_resolver_search_symbol_from_name(node);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_resolver_resolve_type_description(node);
		break;

	case MANA_NODE_DECLARATOR:
		//MANA_ASSERT(node->symbol == NULL);
		//node->symbol = mana_symbol_create_identification(node->string, NULL, mana_generator_object.static_block_opend);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
		//mana_resolver_resolve_variable_description(node, MANA_MEMORY_TYPE_NORMAL);
		break;

	case MANA_NODE_MEMBER_VARIABLE:
#if 0
		mana_resolver_search_symbol_from_name(node);
		node->type = node->symbol->type;
#endif
		break;

	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// �ȉ��� �V���{�����������R�[�h���o�͂��� �֐��S
////////////////////////////////////////////////////////////////////////////////
/*!
�X�^�b�N�ɂ���A�h���X����f�[�^��ǂݍ��ޖ��߂��o�͂���
@param[in]	�ǂݍ���mana_type_description
*/
static void mana_generator_resolve_load(mana_node* node)
{
	//	mana_register_entity* register_entity;

	MANA_ASSERT(node);
	MANA_ASSERT(node->type);
#if 0
	/* TODO:�e�X�g */
	register_entity = mana_register_allocate(node->symbol, node->symbol->address);
	if (register_entity == NULL)
	{
		mana_register_entity* register_entity = mana_register_allocate(node->symbol, node->symbol->address);
		if (register_entity == NULL)
		{
			/* ���蓖�Ă郌�W�X�^�������ꍇ��spill���s�� */
		}
		/* ���W�X�^�ɋ󂫂�����A�܂��͊��Ɋ��蓖�čς݂̏ꍇ */
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
�X�^�b�N�ɂ���A�h���X�փf�[�^���������ޖ��߂��o�͂���
@param[in]	�ǂݍ���mana_type_description
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
* return�̏���
* @param	func	return����������֐���mana_symbol_entry
* @param	tree	return����mana_node
*/
static void mana_generator_return(mana_symbol_entry* func, mana_node* tree)
{
	mana_type_description* type = func->type;

	/* �G���[�`�F�b�N */
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
		/* �����L���X�g */
		tree->left = mana_node_cast(type, tree->left);

		/* �^�̌��� */
		mana_type_compatible(type, tree->left->type);

		/* �m�[�h�̕]�� */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_generator_genearte_code(tree->left, true);
		const int32_t out_depth = mana_symbol_close_block();
		MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
	}

	/* �֐��̍Ō�ɃW�����v */
	mana_symbol_return_address_list = mana_code_set_opecode_and_operand(
		MANA_IL_BRA, mana_symbol_return_address_list);

	/* �֐����g�p�����t���O�𗧂Ă� */
	func->used = true;
}

/*!
* rollback�̏���
* @param	tree	rollback����mana_node
*/
static void mana_generator_rollback(mana_node* tree)
{
	if (tree)
	{
		/* �m�[�h�̕]�� */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_generator_genearte_code(tree, true);
		const int32_t out_depth = mana_symbol_close_block();
		MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
	}
	mana_code_set_opecode(MANA_IL_ROLLBACK);
}

/*!
* �����̏o�́i�ċA�Ăяo���j
* @param	count	�����̔ԍ�
* @param	param	������mana_symbol_entry
* @param	arg		������mana_node
* @return	�����̐�
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

				// �q�m�[�h����^���p������
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
* �֐��̈����̃T�C�Y�𒲂ׂ܂�
* @param	address	�A�h���X
* @param	param	������mana_symbol_entry
* @param	arg		������mana_node
* @return	�����̃T�C�Y
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
* �֐��̈�����]�����܂�
* @param	address	�����̃A�h���X
* @param	param	������mana_symbol_entry
* @param	arg		������mana_node
* @return	�����̃A�h���X
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
* �֐��Ăяo���̃m�[�h��]�����܂�
* @param	�֐��Ăяo����mana_node
*/
static void mana_generator_call(mana_node* node)
{
	mana_node* argument = node->right;
	int32_t argument_counter = mana_generator_generate_argument(0, (node->symbol)->parameter_list, argument);

	/* �G���[�`�F�b�N */
	if ((node->symbol)->number_of_parameters != argument_counter)
	{
		/* �����̐�����v���Ȃ� */
		mana_compile_error(node, "unmatched argument.");
	}
	else if ((node->symbol)->class_type == MANA_CLASS_TYPE_NATIVE_FUNCTION)
	{
		/* �O���֐��̏��� */
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
		// �����֐��̏���
		mana_linker_add_call_list(
			mana_code_set_opecode_and_operand((uint8_t)MANA_IL_BSR, (node->symbol)->address) + 1,
			node->symbol
		);
	}
}

/*!
* print���̏���
* @param	print����mana_node
*/
static void mana_generator_call_print(mana_node* tree)
{
	mana_generator_resolve_symbol(tree);

	int32_t argument_counter = 0;

	for (mana_node* node = tree; node; node = node->left)
	{
		mana_generator_genearte_code((node->id == MANA_NODE_CALL_ARGUMENT) ? node->right : node, true);
		argument_counter++;
	}

	mana_code_set_opecode_and_operand((uint8_t)MANA_IL_PRINT, argument_counter);
}

/*!
* ���ʎ����ɑ���������邩���ׂ܂�
* @param	tree	�]������mana_node
*/
static void mana_generator_condition_check(mana_node* tree)
{
	if (tree)
	{
		if (tree->id == MANA_NODE_ASSIGN)
		{
			/* ��������ɑ�����͎g�p�ł��Ȃ� */
			mana_compile_error(tree, "can't assign expression in condition");
		}

		mana_generator_condition_check(tree->left);
		mana_generator_condition_check(tree->right);
	}
}

/*!
* ���ʎ��̕]��
* @param	tree	�]������mana_node
* @param	pc		�v���O�����J�E���^
* @return	���݂̃v���O�����A�h���X
*/
static int32_t mana_generator_condition_core(mana_node* tree)
{
	/* ���ʎ����ɑ���������邩���ׂ܂� */
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
* ���ʎ��̕]��
* @param	tree	�]������mana_node
* @return	���݂̃v���O�����A�h���X
*/
static int32_t mana_generator_condition(mana_node* tree, int32_t match)
{
	mana_generator_resolve_symbol(tree);
	//mana_generator_automatic_cast(tree);

	/* ���ʎ��̕]�� */
	const int32_t in_depth = mana_symbol_open_block(false);
	mana_generator_condition_core(tree);
	const int32_t out_depth = mana_symbol_close_block();
	MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);

	return mana_code_set_opecode_and_operand(match ? MANA_IL_BEQ : MANA_IL_BNE, -1);
}

static void mana_generator_generate_variable_value(mana_node* node)
{
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

static void mana_compier_generate_hogehoge(mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(node->symbol);

}


////////////////////////////////////////////////////////////////////////////////
/*!
���̕]��
@param	tree			����mana_node
@param	enable_assign	true�Ȃ�Α�����Afalse�Ȃ�΂���ȊO
*/
void mana_generator_expression(mana_node* tree, int32_t enable_assign)
{
	if (!tree)
		return;

	mana_generator_resolve_symbol(tree);

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
	MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
}


////////////////////////////////////////////////////////////////////////////////
void mana_generator_genearte_code(mana_node* node, int32_t enable_load)
{
	if (!node)
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
			mana_symbol_open_module(node->string);
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
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
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
		// �֐��錾�Ɋւ���m�[�h									
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
		MANA_BUG("illegal type of expression");
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		{
			MANA_ASSERT(node->symbol);
			// �֐��̖߂�l��]��
			mana_generator_genearte_code(node->left, enable_load);
			// �V���{���̌����ƌ^�̒�`
			node->symbol = mana_function_symbol_entry_pointer = mana_symbol_lookup(node->string);

			mana_symbol_open_block(false);

			// �֐��̈�����]��
			mana_pre_resolver_resolve(node->right, enable_load);
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
		// �ϐ��錾�Ɋւ���m�[�h									
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
		// �u���b�N�𔺂�����Ɋւ���m�[�h
	case MANA_NODE_BLOCK:
		{
			const int32_t in_depth = mana_symbol_open_block(false);

			mana_post_resolver_resolve(node->left);
			mana_generator_genearte_code(node->left, enable_load);

			mana_post_resolver_resolve(node->right);
			mana_generator_genearte_code(node->right, enable_load);

			const int32_t out_depth = mana_symbol_close_block();
			MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
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
		/* 'for(type variable = expression' �̌`�� */
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
		// ����Ɋւ���m�[�h
	case MANA_NODE_COMPLY:
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_COMPLY);
		break;

	case MANA_NODE_JOIN:
		mana_generator_resolve_symbol(node);
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
		// �񍀉��Z�q�Ɋւ���m�[�h
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
		// ��r�A�_�����Z�q
		mana_generator_automatic_cast(node);
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		MANA_ASSERT(node->body == NULL);
		// TODO:�I�y�R�[�h��ݒ肵�Ă�������
		mana_code_set_opecode((uint8_t)node->etc);
		break;

		///////////////////////////////////////////////////////////////////////
		// �P�����Z�q�Ɋւ���m�[�h
	case MANA_NODE_LNOT:
	case MANA_NODE_NOT:
		mana_generator_automatic_cast(node);
		mana_generator_genearte_code(node->left, enable_load);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		// TODO:�I�y�R�[�h��ݒ肵�Ă�������
		mana_code_set_opecode((uint8_t)node->etc);
		break;

	case MANA_NODE_NEG:
		mana_generator_genearte_code(node->left, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MINUS_FLOAT : MANA_IL_MINUS_INTEGER);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

		///////////////////////////////////////////////////////////////////////
		// ���Z�Ɋւ���m�[�h
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
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_ASSIGN:
		mana_generator_resolve_symbol(node->left);
		mana_generator_resolve_symbol(node->right);

		mana_generator_automatic_cast(node);
		mana_generator_genearte_code(node->right, enable_load);
		mana_generator_genearte_code(node->left, false);
		mana_generator_resolve_store(node->left);
		MANA_ASSERT(node->body == NULL);
		break;

	case MANA_NODE_CALL:
		// �֐��A�A�N�V�������Ăт܂�
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
		// �O�����Z�q
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
		// TODO:�������Ă�������
		mana_generator_genearte_code(node->left, enable_load);
		mana_generator_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_MEMBER_VARIABLE:
		MANA_ASSERT(node->left);
		MANA_ASSERT(node->right == NULL);
		{
			mana_generator_resolve_symbol(node->left);
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
		// sender��push���܂�
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_PUSH_SENDER);
		break;

	case MANA_NODE_SELF:
		// self��push���܂�
		MANA_ASSERT(node->left == NULL);
		MANA_ASSERT(node->right == NULL);
		MANA_ASSERT(node->body == NULL);
		mana_code_set_opecode(MANA_IL_PUSH_SELF);
		break;

	case MANA_NODE_PRIORITY:
		// priority��push���܂�
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
		mana_compile_error(node, "illegal right-hand side value");
		break;
	}

	// �q�m�[�h����^���p������
	mana_resolver_resolve_type_from_child_node(node);

	if (node->next)
	{
		// �����ċA�Ȃ̂�goto�ɂď�������
		//mana_pre_resolver_resolve(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}