/*
 * mana (compiler)
 *
 * @file	mana_link.c
 * @brief	�����N�Ɋւ���\�[�X�t�@�C��
 * @detail	���̃t�@�C���̓����N�Ɋ֌W����\�[�X�t�@�C���ł��B
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
#if !defined(___MANA_JUMP_H___)
#include "mana_jump.h"
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

 // TODO
static mana_symbol_entry* mana_actor_symbol_entry_pointer = NULL;
static mana_symbol_entry* mana_function_symbol_entry_pointer = NULL;
static bool mana_static_block_opend = false;


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
�֐����ɓ��ʂȏ������s�Ȃ����߂̃R�[���o�b�N�֐���o�^���܂�
@param[in]	name		�֐���
@param[in]	function	�R�[���o�b�N�֐�
*/
void mana_node_add_event(char* name, mana_node_event_funtion_type function)
{
	mana_hash_set(mana_node_event_hash, name, function);
}


/*!
�X�^�b�N�ɂ���A�h���X����f�[�^��ǂݍ��ޖ��߂��o�͂���
@param[in]	�ǂݍ���mana_type_description
*/
static void mana_linker_generate_load(mana_node* node)
{
//	mana_register_entity* register_entity;

	MANA_ASSERT(node);
	MANA_ASSERT(node->type);
#if 0
	/* TODO:�e�X�g */
	register_entity = mana_register_allocate(node->symbol, node->symbol->address);
	if(register_entity == NULL)
	{
		mana_register_entity* register_entity = mana_register_allocate(node->symbol, node->symbol->address);
		if(register_entity == NULL)
		{
			/* ���蓖�Ă郌�W�X�^�������ꍇ��spill���s�� */
		}
		/* ���W�X�^�ɋ󂫂�����A�܂��͊��Ɋ��蓖�čς݂̏ꍇ */
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
�X�^�b�N�ɂ���A�h���X�փf�[�^���������ޖ��߂��o�͂���
@param[in]	�ǂݍ���mana_type_description
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
		mana_compile_error(node, "illegal type stored");
		break;
	}
}

static bool mana_linker_get_both_node_type(mana_symbol_data_type_id* t1, mana_symbol_data_type_id* t2, const mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(t1);
	MANA_ASSERT(t2);

	if(node->left == NULL || node->left->type == NULL)
	{
		mana_compile_error(node, "illegal left-hand side expression");
		return false;
	}

	if(node->right != NULL && node->right->type == NULL)
	{
		mana_compile_error(node, "illegal right-hand side expression");
		return false;
	}

	*t1 = (node->left->type)->tcons;
	*t2 = node->right ? (node->right->type)->tcons : MANA_DATA_TYPE_INT;

	return true;
}

/*!
�V���{���̎Q�Ƃ��������܂�
@param	node	�m�[�h�|�C���^
*/
static void mana_linker_resolve_identifier(mana_node* node)
{
	MANA_ASSERT(node);

	if (node->symbol == NULL)
	{
		node->symbol = mana_symbol_lookup(node->string);
		if (node->symbol)
		{
			if (node->type == NULL)
				node->type = node->symbol->type;
		}
		else
		{
			mana_compile_error(node, "incomplete type name %s", node->string);
			node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
	}
}

static void mana_linker_resolve_type_description(mana_node* node)
{
	MANA_ASSERT(node);

	if(node->type == NULL)
	{
		node->symbol = mana_symbol_lookup(node->string);
		if (node->symbol)
		{
			node->type = node->symbol->type;
		}
		else
		{
			mana_compile_error(node, "incomplete type name %s", node->string);
			node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
	}
}

static mana_type_description* mana_linker_resolve_variable_size(mana_node* node)
{
	if (node == NULL)
		return NULL;

	MANA_ASSERT(node->left == NULL);
	MANA_ASSERT(node->right == NULL);

	if (node->string)
	{
		mana_symbol_entry* symbol = mana_symbol_lookup(node->string);
		if (symbol)
		{
			if (symbol->class_type == MANA_CLASS_TYPE_CONSTANT_INT)
			{
				node->type = mana_type_create_array(symbol->address);
			}
			else
			{
				mana_compile_error(node, "invalid size information on parameter");
			}
		}
		else
		{
			mana_compile_error(node, "identifier %s is not defined", node->string);
		}
	}
	else
	{
		if (node->digit > 0)
			node->type = mana_type_create_array(node->digit);
		else
			mana_compile_error(node, "invalid size information on parameter");
	}

	if (node->type)
	{
		node->type->component = mana_linker_resolve_variable_size(node->next);
	}

	return node->type;
}

static void mana_linker_resolve_declarator(mana_node* node)
{
	MANA_ASSERT(node);

	if (node->symbol != NULL)
		return;
	
	mana_type_description* type = NULL;
	if (node->left && node->left->id == MANA_NODE_VARIABLE_SIZE)
		type = mana_linker_resolve_variable_size(node->left);

	node->symbol = mana_symbol_create_identification(node->string, type, mana_static_block_opend);
}

static void mana_linker_resolve_variable_description(mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(node->left && node->left->id == MANA_NODE_TYPE_DESCRIPTION);
	MANA_ASSERT(node->right && node->right->id == MANA_NODE_DECLARATOR);

	mana_linker_resolve_type_description(node->left);	// MANA_NODE_TYPE_DESCRIPTION
	mana_linker_resolve_declarator(node->right);		// MANA_NODE_DECLARATOR

														//if (node->right->symbol->class_type == MANA_CLASS_TYPE_VARIABLE_LOCAL)
	mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
}


/*!
�m�[�h��H��Ȃ���V���{������o�^���܂�
�������֐���A�N�V�������̃m�[�h�͕]�����܂���
@param	node	�m�[�h�|�C���^
*/
void mana_linker_generate_symbol(mana_node* node, mana_node* parent_node)
{
	bool call_next_node = true;

	if (node == NULL)
		return;

	switch (node->id)
	{
	case MANA_NODE_IDENTIFIER:
		mana_linker_resolve_identifier(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		mana_linker_generate_symbol(node->left, node);
		break;

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
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_module();
			mana_linker_generate_symbol(node->left, node);
			mana_symbol_set_type(node->string, mana_symbol_close_module(node->string));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_ACTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = mana_type_get(MANA_DATA_TYPE_VOID);

			/*
			mana_function_symbol_entry_pointer = node->symbol;
			mana_symbol_open_function(true, mana_function_symbol_entry_pointer, mana_type_get(MANA_DATA_TYPE_VOID));

			// node->left�� mana_linker_generate_code �ŏ������܂�

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
				mana_compile_error(node, "static variable range over");
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
			// node->right�� mana_linker_generate_code �ŏ������܂�
			//mana_symbol_close_function(symbol);
		}
		break;

	case MANA_NODE_VARIABLE_DECLARATION:
		mana_linker_resolve_variable_description(node);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_linker_resolve_type_description(node);
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

		// ���ɏ������s��Ȃ��m�[�h
	case MANA_NODE_NEWLINE:
		mana_linker_generate_symbol(node->left, node);
		mana_linker_generate_symbol(node->right, node);
		break;

		// �錾���s��Ȃ��m�[�h
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
	default:
		abort();
	}

	if (call_next_node)
		mana_linker_generate_symbol(node->next, node);
}

/*
���[�J���ϐ����������ϐ��̃A�h���X����������
@param[in]	node	�\����
*/
void mana_linker_resolve_symbol(mana_node* node)
{
	if (node == NULL)
		return;

	switch (node->id)
	{
	case MANA_NODE_IDENTIFIER:
		mana_linker_resolve_identifier(node);
		break;

	case MANA_NODE_SIZEOF:
	case MANA_NODE_DECLARE_VALIABLE:
	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
	case MANA_NODE_INCOMPLETE:
	case MANA_NODE_FUNCTION:
	case MANA_NODE_CONST:
	case MANA_NODE_VARIABLE:
		abort();
		break;

		// ���ɏ������s��Ȃ��m�[�h
	case MANA_NODE_NEWLINE:
		mana_linker_resolve_symbol(node->left);
		mana_linker_resolve_symbol(node->right);
		break;
	case MANA_NODE_VARIABLE_DECLARATION:
		mana_linker_resolve_variable_description(node);
		break;

	case MANA_NODE_DECLARE_ALIAS:
	case MANA_NODE_DECLARE_ACTION:
	case MANA_NODE_DECLARE_ACTOR:
	case MANA_NODE_DECLARE_ALLOCATE:
	case MANA_NODE_DECLARE_EXTEND:
	case MANA_NODE_DECLARE_FUNCTION:
	case MANA_NODE_DECLARE_MODULE:
	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
	case MANA_NODE_DECLARE_PHANTOM:
	case MANA_NODE_DECLARE_STATIC:
	case MANA_NODE_DECLARE_STRUCT:
	case MANA_NODE_DEFINE_CONSTANT:
	case MANA_NODE_UNDEFINE_CONSTANT:
		break;

		// �ϐ��̃A�h���X��T�C�Y�̉����Ɋ֌W���Ȃ��m�[�h
	case MANA_NODE_TYPE_DESCRIPTION:
	case MANA_NODE_DECLARATOR:
	case MANA_NODE_ARRAY:
	case MANA_NODE_ASSIGN:
	case MANA_NODE_MEMOP:
	case MANA_NODE_ARGUMENT:
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
	default:
		abort();
	}

	mana_linker_resolve_symbol(node->next);
}

/*
�^�ϊ����K�v�Ȃ�΁A�^�ϊ��̃m�[�h��}�����܂�
@param[in]	node	�m�[�h
*/
static void mana_linker_automatic_cast(mana_node* node)
{
	if (node == NULL)
		return;

	mana_linker_automatic_cast(node->left);
	mana_linker_automatic_cast(node->right);

	if (node->left)
		node->type = node->left->type;

	switch (node->id)
	{
	case MANA_NODE_ASSIGN:
		if (node->left->id == MANA_NODE_CONST)
		{
			mana_compile_error(node, "already initialized constant '%s'", node->left->symbol->name);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
			mana_node_auto_cast(node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
				mana_compile_error(node, "imcompatible type operation in expression");
			if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_FLOAT)
				mana_compile_error(node, "imcompatible type operation in expression");
			mana_type_compatible(node->left->type, node->right->type);
		}
		break;

	case MANA_NODE_GT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);

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
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_NOT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_linker_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_NOT;
		}
		break;

	case MANA_NODE_LNOT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_linker_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_LNOT;
		}
		break;

	case MANA_NODE_I2F:
		{
			mana_symbol_data_type_id t1, t2;
			mana_linker_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_F2I:
		{
			mana_symbol_data_type_id t1, t2;
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_LAND:
		{
			mana_symbol_data_type_id t1, t2;
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_get_both_node_type(&t1, &t2, node);
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
			mana_linker_resolve_identifier(node);

			mana_node_event_funtion_type* function = mana_hash_get(mana_node_event_hash, node->symbol->name);
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

	case MANA_NODE_ARGUMENT:
		node->etc = (node->right->id == MANA_NODE_ARGUMENT) ? node->right->etc + 1 : 2;
		break;

	case MANA_NODE_IDENTIFIER:
		mana_linker_resolve_identifier(node);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_linker_resolve_type_description(node);
		break;

	case MANA_NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, /*mana_static_block_opend*/false);
		break;

	case MANA_NODE_VARIABLE_DECLARATION:
		mana_linker_resolve_variable_description(node);
		break;

	default:
		break;
	}
}

/*!
�m�[�h��H��Ȃ��璆�Ԍ���ɖ|�󂵂܂�
@param	tree			�m�[�h
@param	enable_load		true�Ȃ��load���߂͗L���Afalse�Ȃ��load���߂͖���
*/
void mana_linker_generate_code(mana_node* node, int32_t enable_load)
{
	bool call_next_node = true;

	if(!node)
		return;

	switch(node->id)
	{
	case MANA_NODE_IDENTIFIER:
		mana_linker_resolve_identifier(node);
		break;

	case MANA_NODE_ASSIGN:
		mana_linker_automatic_cast(node);
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
			mana_compile_error(node, "illegal type of expression in statement");
			break;
		}

		if(enable_load)
		{
			mana_linker_generate_load(node);
		}

		break;

	case MANA_NODE_VARIABLE_SIZE:
		mana_linker_generate_code(node->left, enable_load);
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
			mana_compile_error(node, "illegal type of expression in statement");
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
		/* ��r�A�_�����Z�q */
		mana_linker_automatic_cast(node);
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		mana_code_set_opecode((uint8_t)node->etc);
		break;

	case MANA_NODE_CALL:
		// �֐��A�A�N�V�������Ăт܂�
		if(node->symbol == NULL)
		{
			node->symbol = mana_symbol_lookup(node->string);
			if(node->symbol)
				node->type = node->symbol->type;
		}
		mana_linker_call(node);
		break;

	case MANA_NODE_SENDER:
		/* sender��push���܂� */
		mana_code_set_opecode(MANA_IL_PUSH_SENDER);
		break;

	case MANA_NODE_SELF:
		/* self��push���܂� */
		mana_code_set_opecode(MANA_IL_PUSH_SELF);
		break;

	case MANA_NODE_PRIORITY:
		/* priority��push���܂� */
		mana_code_set_opecode(MANA_IL_PUSH_PRIORITY);
		break;

	case MANA_NODE_EXPRESSION_IF:
		/* �O�����Z�q */
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

	case MANA_NODE_VARIABLE_DECLARATION:
		mana_linker_resolve_variable_description(node);
		/*
		mana_linker_generate_code(node->left, enable_load); // MANA_NODE_TYPE_DESCRIPTION
		mana_linker_generate_code(node->right, enable_load);// MANA_NODE_DECLARATOR
		if(node->right->symbol->class_type == MANA_CLASS_TYPE_VARIABLE_LOCAL)
			mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
		*/
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_linker_resolve_type_description(node);
		break;

	case MANA_NODE_DECLARATOR:
		//| tIDENTIFIER variable_sizes
		if(node->symbol == NULL)
			node->symbol = mana_symbol_create_identification(node->string, NULL, /*mana_static_block_opend*/false);
		break;

	case MANA_NODE_DECLARE_ACTOR:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_linker_generate_code(node->left, enable_load);
			mana_symbol_set_type(node->string, mana_symbol_close_actor(node->string, NULL, NULL, false));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_linker_generate_code(node->left, enable_load);
			mana_symbol_set_type(node->string, mana_symbol_close_actor(node->string, NULL, NULL, true));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_MODULE:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_module();
			mana_linker_generate_code(node->left, enable_load);
			mana_symbol_set_type(node->string, mana_symbol_close_module(node->string));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_ACTION:
		{
			mana_function_symbol_entry_pointer = mana_symbol_create_function(node->string);
			mana_symbol_open_function(true, mana_function_symbol_entry_pointer, mana_type_get(MANA_DATA_TYPE_VOID));

			mana_linker_generate_code(node->left, enable_load);
			
			mana_symbol_close_function(mana_function_symbol_entry_pointer);
			mana_function_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		{
			mana_function_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_linker_generate_code(node->left, enable_load);

			mana_symbol_open_function(false, mana_function_symbol_entry_pointer, node->left->type);

			mana_linker_generate_code(node->right, enable_load);

			mana_symbol_close_function(mana_function_symbol_entry_pointer);
			mana_function_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_BLOCK:
		{
			const int32_t in_depth = mana_symbol_open_block(false);
			mana_linker_generate_code(node->left, enable_load);
			mana_linker_generate_code(node->right, enable_load);
			const int32_t out_depth = mana_symbol_close_block();
			MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
		}
		break;

	case MANA_NODE_FOR:
		/* 'for(type variable = expression' �̌`�� */
		{
			//mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL);
			//mana_linker_expression(mana_node_create_node(MANA_NODE_TYPE_ASSIGN, mana_node_create_leaf($2->name), $4), true);
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
			//$$ = mana_code_get_pc();

			mana_linker_generate_code(node->left, enable_load);
			mana_jump_break(mana_linker_condition(node->left, true));


			mana_jump_close_continue_only();
			mana_linker_expression(node->right, true);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
			mana_jump_close_chain();
		}
		break;

	case MANA_NODE_IF:
	case MANA_NODE_SWITCH:
	case MANA_NODE_CASE:
	case MANA_NODE_DEFAULT:
	case MANA_NODE_WHILE:
	case MANA_NODE_DO:
	case MANA_NODE_LOOP:
	case MANA_NODE_LOCK:
	case MANA_NODE_GOTO:
	case MANA_NODE_LABEL:
	case MANA_NODE_BREAK:
	case MANA_NODE_CONTINUE:
	case MANA_NODE_SIZEOF:
	case MANA_NODE_DECLARE_EXTEND:
	case MANA_NODE_DECLARE_VALIABLE:

	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		break;

		// ���ɏ������s��Ȃ��m�[�h
	case MANA_NODE_NEWLINE:
		mana_linker_generate_code(node->left, enable_load);
		mana_linker_generate_code(node->right, enable_load);
		break;

		// ��`���s��Ȃ��m�[�h
	case MANA_NODE_DECLARE_ALIAS:
	case MANA_NODE_DECLARE_STRUCT:
	case MANA_NODE_DECLARE_ALLOCATE:
	case MANA_NODE_DECLARE_STATIC:
	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
	case MANA_NODE_DEFINE_CONSTANT:
	case MANA_NODE_UNDEFINE_CONSTANT:
		break;

	default:
		mana_compile_error(node, "illegal right-hand side value");
		break;
	}

	if (call_next_node)
		mana_linker_generate_code(node->next, enable_load);
}

/*!
 * return�̏���
 * @param	func	return����������֐���mana_symbol_entry
 * @param	tree	return����mana_node
 */
void mana_linker_return(mana_symbol_entry* func, mana_node* tree)
{
	mana_type_description* type;

	type = func->type;

	/* �G���[�`�F�b�N */
	if(type->tcons == MANA_DATA_TYPE_VOID)
	{
		if(tree != NULL)
			mana_compile_error(func, "meaningless return value specification");
	}
	else if(tree == NULL)
	{
		mana_compile_error(func, "missing return value");
	}
	else
	{
		/* �����L���X�g */
		tree = mana_node_cast(type, tree);

		/* �^�̌��� */
		mana_type_compatible(type, tree->type);

		/* �m�[�h�̕]�� */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_linker_generate_code(tree, true);
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
void mana_linker_rollback(mana_node* tree)
{
	if(tree)
	{
		/* �m�[�h�̕]�� */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_linker_generate_code(tree, true);
		const int32_t out_depth = mana_symbol_close_block();
		MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
	}
	mana_code_set_opecode(MANA_IL_ROLLBACK);
}

/*!
 * �֐��̈����̃T�C�Y�𒲂ׂ܂�
 * @param	address	�A�h���X
 * @param	param	������mana_symbol_entry
 * @param	arg		������mana_node
 * @return	�����̃T�C�Y
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
 * �֐��̈�����]�����܂�
 * @param	address	�����̃A�h���X
 * @param	param	������mana_symbol_entry
 * @param	arg		������mana_node
 * @return	�����̃A�h���X
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
 * �֐��Ăяo���̃m�[�h��]�����܂�
 * @param	�֐��Ăяo����mana_node
 */
static void mana_linker_call(mana_node* node)
{
	mana_node* argument = node->right;
	int32_t argument_counter = mana_linker_argument((node->symbol)->parameter_list, argument);

	/* �G���[�`�F�b�N */
	if((node->symbol)->number_of_parameters != argument_counter)
	{
		/* �����̐�����v���Ȃ� */
		mana_compile_error(node, "unmatched argument.");
	}
	else if((node->symbol)->class_type == MANA_CLASS_TYPE_NATIVE_FUNCTION)
	{
		/* �O���֐��̏��� */
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
		/* �����֐��̏��� */
		mana_code_set_opecode_and_operand((uint8_t)MANA_IL_BSR, (node->symbol)->address);
	}
}

/*!
 * print���̏���
 * @param	print����mana_node
 */
void mana_linker_call_print(mana_node* tree)
{
	int32_t argument_counter = 0;

	for(mana_node* node = tree; node; node = node->left)
	{
		mana_linker_generate_code((node->id == MANA_NODE_ARGUMENT) ? node->right : node, true);
		argument_counter ++;
	}

	mana_code_set_opecode_and_operand((uint8_t)MANA_IL_PRINT, argument_counter);
}

/*!
 * �����̏o�́i�ċA�Ăяo���j
 * @param	count	�����̔ԍ�
 * @param	param	������mana_symbol_entry
 * @param	arg		������mana_node
 * @return	�����̐�
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
 * �����̏o��
 * @param	param	������mana_symbol_entry
 * @param	arg		������mana_node
 * @return	�����̐�
 */
int32_t mana_linker_argument(mana_symbol_entry* param, mana_node* arg)
{
	return mana_linker_generate_argument(0, param, arg);
}

/*!
 * ���̕]��
 * @param	tree			����mana_node
 * @param	enable_assign	true�Ȃ�Α�����Afalse�Ȃ�΂���ȊO
 */
void mana_linker_expression(mana_node* tree, int32_t enable_assign)
{
	if(!tree)
		return;

	const int32_t in_depth = mana_symbol_open_block(false);

	if(enable_assign)
	{
		if(tree->id != MANA_NODE_ASSIGN && tree->id != MANA_NODE_CALL && tree->id != MANA_NODE_F2I)
			mana_compile_error(tree, "illegal expression in write-statement");
	}else{
		if(tree->id == MANA_NODE_ASSIGN)
			mana_compile_error(tree, "illegal expression in write-statement");
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

	const int32_t out_depth = mana_symbol_close_block();
	MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
}

/*!
 * ���ʎ����ɑ���������邩���ׂ܂�
 * @param	tree	�]������mana_node
 */
static void mana_linker_condition_check(mana_node* tree)
{
	if(tree)
	{
		if(tree->id == MANA_NODE_ASSIGN)
		{
			/* ��������ɑ�����͎g�p�ł��Ȃ� */
			mana_compile_error(tree, "can't assign expression in condition");
		}

		mana_linker_condition_check(tree->left);
		mana_linker_condition_check(tree->right);
	}
}

/*!
 * ���ʎ��̕]��
 * @param	tree	�]������mana_node
 * @param	pc		�v���O�����J�E���^
 * @return	���݂̃v���O�����A�h���X
 */
static int32_t mana_linker_condition_core(mana_node* tree)
{
	/* ���ʎ����ɑ���������邩���ׂ܂� */
	mana_linker_condition_check(tree);

	if(tree)
	{
		if((tree->type)->tcons == MANA_DATA_TYPE_VOID || (tree->type)->tcons > MANA_DATA_TYPE_REFERENCE)
			mana_compile_error(tree, "illegal type of expression in condition");

		mana_linker_generate_code(tree, true);
	}
	return mana_code_get_pc() - 5;
}

/*!
 * ���ʎ��̕]��
 * @param	tree	�]������mana_node
 * @return	���݂̃v���O�����A�h���X
 */
int32_t mana_linker_condition(mana_node* tree, int32_t match)
{
	/* ���ʎ��̕]�� */
	const int32_t in_depth = mana_symbol_open_block(false);
	mana_linker_condition_core(tree);
	const int32_t out_depth = mana_symbol_close_block();
	MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);

	return mana_code_set_opecode_and_operand(match ? MANA_IL_BEQ : MANA_IL_BNE, -1);
}
