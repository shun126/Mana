/*!
mana (compiler)

@file	mana_compile.c
@brief	�R���p�C���Z�N�V�����Ɋւ���\�[�X�t�@�C��
@detail	���̃t�@�C���̓R���p�C���Z�N�V�����Ɋ֌W����\�[�X�t�@�C���ł��B
@author	Shun Moriya <shun@mnu.sakura.ne.jp>
@date	2017-
*/

#if !defined(___MANA_COMPILER_H___)
#include "mana_compiler.h"
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

// TODO
static mana_symbol_entry* mana_actor_symbol_entry_pointer = NULL;
static mana_symbol_entry* mana_function_symbol_entry_pointer = NULL;

static struct mana_compiler_object
{
	mana_symbol_entry* actor_symbol_entry_pointer;
	mana_symbol_entry* function_symbol_entry_pointer;
	mana_hash* event_hash;
	bool static_block_opend;
}mana_compiler_object;

////////////////////////////////////////////////////////////////////////////////
void mana_compiler_initialize(void)
{
	mana_compiler_object.event_hash = mana_hash_create();
	mana_compiler_object.static_block_opend = false;

	{
		/* vec2 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec2", mana_symbol_close_structure("vec2"));

		/* vec3 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec3", mana_symbol_close_structure("vec3"));

		/* vec4 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("w", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec4", mana_symbol_close_structure("vec4"));

		/* color */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("r", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("g", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("b", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("a", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("color", mana_symbol_close_structure("color"));
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

void mana_compiler_finalize(void)
{
	mana_hash_destroy(mana_compiler_object.event_hash);
	mana_compiler_object.event_hash = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/*!
�֐����ɓ��ʂȏ������s�Ȃ����߂̃R�[���o�b�N�֐���o�^���܂�
@param[in]	name		�֐���
@param[in]	function	�R�[���o�b�N�֐�
*/
void mana_compiler_add_event(const char* name, mana_compiler_event_funtion_type function)
{
	mana_hash_set(mana_compiler_object.event_hash, name, function);
}

////////////////////////////////////////////////////////////////////////////////
/*!
�q�m�[�h����^���p������
@param[in]	node	�m�[�h
*/
static void mana_compiler_inherit_type_from_child_node(mana_node* node)
{
	// �����̌^���o�^����Ă��Ȃ�
	if (node->type == NULL)
	{
		// �V���{�����o�^����Ă���Ȃ�A�V���{���̌^���p������
		if (node->symbol && node->symbol->type)
		{
			node->type = node->symbol->type;
		}
		// ���ӂ̌^���o�^����Ă���Ȃ�A���ӂ̌^���p������
		else if (node->left && node->left->type)
		{
			node->type = node->left->type;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
mana_symbol_lookup���Ăяo��mana_symbol����������node�ɐݒ肵�܂�
@param	node	MANA_NODE_IDENTIFIER�m�[�h
*/
static void mana_compiler_resolve_identifier(mana_node* node)
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
			mana_compile_error(node, "incomplete type name '%s'", node->string);
			node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
	}
}

/*!
node�ɓo�^���ꂽtype��������΃V���{�����������Ă���type��node�ɐݒ肵�܂�
@param[in]	node	MANA_NODE_TYPE_DESCRIPTION�m�[�h
*/
static void mana_compiler_resolve_type_description(mana_node* node)
{
	MANA_ASSERT(node);

	if (node->type == NULL)
	{
		node->symbol = mana_symbol_lookup(node->string);
		if (node->symbol)
		{
			node->type = node->symbol->type;
		}
		else
		{
			mana_compile_error(node, "incomplete type name '%s'", node->string);
			node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
	}
}

/*!
mana_type_create_array���g���Ĕz��̌^��new���܂�
@param[in]	node	MANA_NODE_VARIABLE_SIZE�m�[�h
@return		�z��mana_type_description
*/
static mana_type_description* mana_compiler_resolve_variable_size(mana_node* node)
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
		node->type->component = mana_compiler_resolve_variable_size(node->left);
	}

	return node->type;
}

/*!
mana_symbol_create_identification���Ăяo��
mana_symbol��new����node�ɐݒ肵�܂�
@param[in]	node	MANA_NODE_DECLARATOR�m�[�h
*/
static void mana_compiler_resolve_declarator(mana_node* node)
{
	MANA_ASSERT(node);

	if (node->symbol != NULL)
		return;

	mana_type_description* type = NULL;
	if (node->left && node->left->id == MANA_NODE_VARIABLE_SIZE)
		type = mana_compiler_resolve_variable_size(node->left);

	node->symbol = mana_symbol_create_identification(node->string, type, mana_compiler_object.static_block_opend);
}

/*!
���ӂ�MANA_NODE_TYPE_DESCRIPTION��MANA_NODE_DECLARATOR����������
mana_symbol_allocate_memory���g���ă����������蓖�Ă܂�
@param[in]	node	MANA_NODE_DECLARE_VARIABLE�m�[�h
*/
static void mana_compiler_resolve_variable_description(mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(node->left && node->left->id == MANA_NODE_TYPE_DESCRIPTION);
	MANA_ASSERT(node->right && node->right->id == MANA_NODE_DECLARATOR);

	mana_compiler_resolve_type_description(node->left);	// MANA_NODE_TYPE_DESCRIPTION
	mana_compiler_resolve_declarator(node->right);		// MANA_NODE_DECLARATOR

														//if (node->right->symbol->class_type == MANA_CLASS_TYPE_VARIABLE_LOCAL)
	mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
	//mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_PARAMETER);
}

/*!
�����̐����擾���܂��i�ċA�Ăяo���j
@param	count	�����̔ԍ�
@param	param	������mana_symbol_entry
@param	arg		������mana_node
@return	�����̐�
*/
static int32_t mana_compiler_count_declaration_argument(int32_t count, mana_node* arg)
{
	if (arg)
	{
		if (arg->id == MANA_NODE_DECLARE_ARGUMENT)
		{
			count = mana_compiler_count_declaration_argument(count, arg->right);
		}
		count++;
	}
	return count;
}

////////////////////////////////////////////////////////////////////////////////
void mana_compiler_generate_symbol(mana_node* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:

	switch (node->id)
	{
		// ���ɏ������s��Ȃ��m�[�h
	case MANA_NODE_NEWLINE:
		MANA_ASSERT(node->body == NULL);
		mana_compiler_generate_symbol(node->left);
		mana_compiler_generate_symbol(node->right);
		break;

	case MANA_NODE_IDENTIFIER:
		mana_compiler_resolve_identifier(node);
		break;

		// �萔��`�Ɋւ���m�[�h									
	case MANA_NODE_DEFINE_ALIAS:
		mana_symbol_create_alias(node->string, node->string);
		break;

	case MANA_NODE_DEFINE_CONSTANT:
		mana_symbol_create_const_int(node->string, node->digit);
		break;

	case MANA_NODE_UNDEFINE_CONSTANT:
		mana_symbol_destroy(node->string);
		break;

		// ���������C�A�E�g�Ɋւ���m�[�h									
	case MANA_NODE_DECLARE_ALLOCATE:
		{
			const int32_t mana_allocated_size = mana_symbol_get_static_memory_address() + node->digit;

			mana_compiler_generate_symbol(node->left);

			const int32_t address = mana_symbol_get_static_memory_address();
			if (address >= mana_allocated_size)
			{
				mana_compile_error(node, "static variable range over");
			}
			mana_symbol_set_static_memory_address(mana_allocated_size);
		}
		break;

	case MANA_NODE_DECLARE_STATIC:
		mana_compiler_object.static_block_opend = true;
		mana_compiler_generate_symbol(node->left);
		mana_compiler_object.static_block_opend = false;
		break;

		// �\���Ɋւ���m�[�h
	case MANA_NODE_DECLARE_ACTOR:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_compiler_generate_symbol(node->left);
			mana_symbol_set_type(node->string, mana_symbol_close_actor(node->string, NULL, NULL, false));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_EXTEND:
		mana_symbol_extend_module(node->string);
		break;

	case MANA_NODE_DECLARE_MODULE:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_module(mana_actor_symbol_entry_pointer);
			mana_compiler_generate_symbol(node->left);
			mana_symbol_set_type(node->string, mana_symbol_close_module(node->string));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_compiler_generate_symbol(node->left);
			mana_symbol_set_type(node->string, mana_symbol_close_actor(node->string, NULL, NULL, true));
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_STRUCT:
		mana_symbol_open_structure();
		mana_compiler_generate_symbol(node->left);
		mana_symbol_set_type(node->string, mana_symbol_close_structure(node->string));
		break;

		// �֐��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARE_ACTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = mana_type_get(MANA_DATA_TYPE_VOID);
		}
		break;

	case MANA_NODE_DECLARE_ARGUMENT:
		MANA_ASSERT(node->body == NULL);
		mana_compiler_generate_symbol(node->left);
		mana_compiler_generate_symbol(node->right);
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);
			mana_compiler_generate_symbol(node->left);
			node->type = node->symbol->type = node->left->type;
			mana_compiler_generate_symbol(node->right);
			node->symbol->number_of_parameters = mana_compiler_count_declaration_argument(0, node->right);
			//mana_compiler_generate_symbol(node->body); �֐����̃V���{���͕]�����Ȃ�
		}
		break;

	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		{
			node->symbol = mana_symbol_create_function(node->string);
			mana_symbol_begin_native_function_registration();
			mana_compiler_generate_symbol(node->left);
			mana_compiler_generate_symbol(node->right);
			node->symbol->number_of_parameters = mana_compiler_count_declaration_argument(0, node->right);
			mana_symbol_commit_native_function_registration(node->symbol, node->left->type);
		}
		break;

		// �ϐ��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, mana_compiler_object.static_block_opend);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_compiler_resolve_variable_description(node);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_compiler_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		mana_compiler_generate_symbol(node->left);
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
	case MANA_NODE_MEMOP:
	case MANA_NODE_PRINT:
	case MANA_NODE_REFUSE:
	case MANA_NODE_REQUEST:
	case MANA_NODE_STRING:
	case MANA_NODE_VARIABLE:
	case MANA_NODE_YIELD:

		// ���Ԍ���Ƒ΂ɂȂ�m�[�h
	case MANA_NODE_SIZEOF:
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
		abort();
	}

	// �q�m�[�h����^���p������
	mana_compiler_inherit_type_from_child_node(node);

	if (node->next)
	{
		// �����ċA�Ȃ̂�goto�ɂď�������
		//mana_compiler_generate_symbol(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}



////////////////////////////////////////////////////////////////////////////////
// �ȉ��� �֐����̃V���{������������ �֐��S
////////////////////////////////////////////////////////////////////////////////
static void mana_compiler_resolve_symbol(mana_node* node)
{
	if(node == NULL)
		return;

	mana_compiler_resolve_symbol(node->left);
	mana_compiler_resolve_symbol(node->right);

	switch(node->id)
	{
	case MANA_NODE_NEWLINE:
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
		abort();

		// �ϐ��錾�Ɋւ���m�[�h
	case MANA_NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, mana_compiler_object.static_block_opend);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_compiler_resolve_variable_description(node);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_compiler_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		mana_compiler_generate_symbol(node->left);
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
	case MANA_NODE_MEMOP:
	case MANA_NODE_PRINT:
	case MANA_NODE_REFUSE:
	case MANA_NODE_REQUEST:
	case MANA_NODE_STRING:
	case MANA_NODE_VARIABLE:
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
		mana_compiler_resolve_identifier(node);
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
		abort();
	}

	// �q�m�[�h����^���p������
	mana_compiler_inherit_type_from_child_node(node);
}

////////////////////////////////////////////////////////////////////////////////
// �ȉ��� �m�[�h�̌^�ɂ��킹�Č^�ϊ�������m�[�h���������ފ֐��S
////////////////////////////////////////////////////////////////////////////////

static bool mana_compiler_get_both_node_type(mana_symbol_data_type_id* t1, mana_symbol_data_type_id* t2, const mana_node* node)
{
	MANA_ASSERT(node);
	MANA_ASSERT(t1);
	MANA_ASSERT(t2);

	if (node->left == NULL || node->left->type == NULL)
	{
		mana_compile_error(node, "illegal left-hand side expression");
		return false;
	}

	if (node->right != NULL && node->right->type == NULL)
	{
		mana_compile_error(node, "illegal right-hand side expression");
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
static void mana_compiler_automatic_cast(mana_node* node)
{
	if (node == NULL)
		return;

	mana_compiler_automatic_cast(node->left);
	mana_compiler_automatic_cast(node->right);

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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);

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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_FLOAT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_NOT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_compiler_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_NOT;
		}
		break;

	case MANA_NODE_LNOT:
		{
			mana_symbol_data_type_id t1, t2;
			mana_compiler_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			node->etc = MANA_IL_LNOT;
		}
		break;

	case MANA_NODE_I2F:
		{
			mana_symbol_data_type_id t1, t2;
			mana_compiler_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_F2I:
		{
			mana_symbol_data_type_id t1, t2;
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
			if (t1 < MANA_DATA_TYPE_CHAR || t1 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
			if (t2 < MANA_DATA_TYPE_CHAR || t2 > MANA_DATA_TYPE_INT)
				mana_compile_error(node, "imcompatible type operation in expression");
		}
		break;

	case MANA_NODE_LAND:
		{
			mana_symbol_data_type_id t1, t2;
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_get_both_node_type(&t1, &t2, node);
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
			mana_compiler_resolve_identifier(node);

			mana_compiler_event_funtion_type* function = mana_hash_get(mana_compiler_object.event_hash, node->symbol->name);
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
		mana_compiler_resolve_identifier(node);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_compiler_resolve_type_description(node);
		break;

	case MANA_NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, mana_compiler_object.static_block_opend);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_compiler_resolve_variable_description(node);
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
static void mana_compiler_resolve_load(mana_node* node)
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
static void mana_compiler_resolve_store(mana_node* node)
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
static void mana_compiler_return(mana_symbol_entry* func, mana_node* tree)
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
		tree = mana_node_cast(type, tree);

		/* �^�̌��� */
		mana_type_compatible(type, tree->type);

		/* �m�[�h�̕]�� */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_compiler_genearte_code(tree->left, true);
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
static void mana_compiler_rollback(mana_node* tree)
{
	if (tree)
	{
		/* �m�[�h�̕]�� */
		const int32_t in_depth = mana_symbol_open_block(false);
		mana_compiler_genearte_code(tree, true);
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
static int32_t mana_compiler_generate_argument(int32_t count, mana_symbol_entry* param, mana_node* arg)
{
	if (param && arg)
	{
		if (arg->id == MANA_NODE_CALL_ARGUMENT)
		{
			count = mana_compiler_generate_argument(count, param->next, arg->right);
		}
		arg = mana_node_cast(param->type, arg);
		mana_type_compatible(param->type, arg->type);
		mana_compiler_genearte_code(arg, true);
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
static int32_t mana_compiler_call_argument_size(int32_t address, mana_symbol_entry* param, mana_node* arg)
{
	if (param && arg)
	{
		address += mana_compiler_call_argument_size(address, param->next, arg->right);

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
static int32_t mana_compiler_call_argument(int32_t address, mana_symbol_entry* param, mana_node* arg)
{
	if (param && arg)
	{
		address = mana_compiler_call_argument(address, param->next, arg->right);

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
static void mana_compiler_call(mana_node* node)
{
	mana_node* argument = node->right;
	int32_t argument_counter = mana_compiler_generate_argument(0, (node->symbol)->parameter_list, argument);

	/* �G���[�`�F�b�N */
	if ((node->symbol)->number_of_parameters != argument_counter)
	{
		/* �����̐�����v���Ȃ� */
		mana_compile_error(node, "unmatched argument.");
	}
	else if ((node->symbol)->class_type == MANA_CLASS_TYPE_NATIVE_FUNCTION)
	{
		/* �O���֐��̏��� */
		int32_t argument_size = mana_compiler_call_argument_size(0, (node->symbol)->parameter_list, argument);

		node->symbol->address = mana_data_set(node->symbol->name);

		mana_code_set_opecode_and_operand((uint8_t)MANA_IL_CALL, (node->symbol)->address);
		mana_code_set_unsigned_char((uint8_t)((node->symbol)->type->tcons != MANA_DATA_TYPE_VOID));
		mana_code_set_unsigned_char((uint8_t)argument_counter);
		mana_code_set_unsigned_short((uint16_t)argument_size);
		mana_compiler_call_argument(argument_size - 1, (node->symbol)->parameter_list, argument);
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
static void mana_compiler_call_print(mana_node* tree)
{
	mana_compiler_resolve_symbol(tree);

	int32_t argument_counter = 0;

	for (mana_node* node = tree; node; node = node->left)
	{
		mana_compiler_genearte_code((node->id == MANA_NODE_CALL_ARGUMENT) ? node->right : node, true);
		argument_counter++;
	}

	mana_code_set_opecode_and_operand((uint8_t)MANA_IL_PRINT, argument_counter);
}

/*!
* ���ʎ����ɑ���������邩���ׂ܂�
* @param	tree	�]������mana_node
*/
static void mana_compiler_condition_check(mana_node* tree)
{
	if (tree)
	{
		if (tree->id == MANA_NODE_ASSIGN)
		{
			/* ��������ɑ�����͎g�p�ł��Ȃ� */
			mana_compile_error(tree, "can't assign expression in condition");
		}

		mana_compiler_condition_check(tree->left);
		mana_compiler_condition_check(tree->right);
	}
}

/*!
* ���ʎ��̕]��
* @param	tree	�]������mana_node
* @param	pc		�v���O�����J�E���^
* @return	���݂̃v���O�����A�h���X
*/
static int32_t mana_compiler_condition_core(mana_node* tree)
{
	/* ���ʎ����ɑ���������邩���ׂ܂� */
	mana_compiler_condition_check(tree);

	if (tree)
	{
		if ((tree->type)->tcons == MANA_DATA_TYPE_VOID || (tree->type)->tcons > MANA_DATA_TYPE_REFERENCE)
			mana_compile_error(tree, "illegal type of expression in condition");

		mana_compiler_genearte_code(tree, true);
	}
	return mana_code_get_pc() - 5;
}

/*!
* ���ʎ��̕]��
* @param	tree	�]������mana_node
* @return	���݂̃v���O�����A�h���X
*/
static int32_t mana_compiler_condition(mana_node* tree, int32_t match)
{
	mana_compiler_resolve_symbol(tree);
	//mana_compiler_automatic_cast(tree);

	/* ���ʎ��̕]�� */
	const int32_t in_depth = mana_symbol_open_block(false);
	mana_compiler_condition_core(tree);
	const int32_t out_depth = mana_symbol_close_block();
	MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);

	return mana_code_set_opecode_and_operand(match ? MANA_IL_BEQ : MANA_IL_BNE, -1);
}




void mana_compiler_genearte_code(mana_node* node, int32_t enable_load)
{
	if (!node)
		return;

DO_RECURSIVE:

	switch (node->id)
	{
		// ���ɏ������s��Ȃ��m�[�h
	case MANA_NODE_NEWLINE:
		MANA_ASSERT(node->body == NULL);
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_IDENTIFIER:
		mana_compiler_resolve_identifier(node);
		break;

		// �萔��`�Ɋւ���m�[�h									
	case MANA_NODE_DEFINE_ALIAS:
	case MANA_NODE_DEFINE_CONSTANT:
	case MANA_NODE_UNDEFINE_CONSTANT:

		// ���������C�A�E�g�Ɋւ���m�[�h
	case MANA_NODE_DECLARE_ALLOCATE:
	case MANA_NODE_DECLARE_STATIC:
		break;

		// �\���Ɋւ���m�[�h									
	case MANA_NODE_DECLARE_ACTOR:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_compiler_genearte_code(node->left, enable_load);
			mana_symbol_close_actor(node->string, NULL, NULL, false);
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_EXTEND:
		break;

	case MANA_NODE_DECLARE_MODULE:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_module(mana_actor_symbol_entry_pointer);
			mana_compiler_genearte_code(node->left, enable_load);
			mana_symbol_close_module(node->string);
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		{
			mana_actor_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
			mana_compiler_genearte_code(node->left, enable_load);
			mana_symbol_close_actor(node->string, NULL, NULL, true);
			mana_actor_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_STRUCT:
		break;

		// �֐��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARE_ACTION:
		{
			mana_function_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_symbol_begin_function_registration(true, mana_function_symbol_entry_pointer, mana_type_get(MANA_DATA_TYPE_VOID));

			mana_compiler_genearte_code(node->left, enable_load);

			mana_symbol_commit_function_registration(mana_function_symbol_entry_pointer);
			mana_function_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_ARGUMENT:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		{
			mana_function_symbol_entry_pointer = mana_symbol_lookup(node->string);
			mana_compiler_genearte_code(node->left, enable_load);

			mana_symbol_begin_function_registration(false, mana_function_symbol_entry_pointer, node->left->type);

			mana_compiler_genearte_code(node->right, enable_load);
			mana_compiler_genearte_code(node->body, enable_load);

			mana_symbol_commit_function_registration(mana_function_symbol_entry_pointer);
			mana_function_symbol_entry_pointer = NULL;
		}
		break;

	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		break;

		// �ϐ��錾�Ɋւ���m�[�h									
	case MANA_NODE_DECLARATOR:
		//| tIDENTIFIER variable_sizes
		if (node->symbol == NULL)
			node->symbol = mana_symbol_create_identification(node->string, NULL, mana_compiler_object.static_block_opend);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_compiler_resolve_variable_description(node);
		/*
		mana_compiler_genearte_code(node->left, enable_load); // MANA_NODE_TYPE_DESCRIPTION
		mana_compiler_genearte_code(node->right, enable_load);// MANA_NODE_DECLARATOR
		if(node->right->symbol->class_type == MANA_CLASS_TYPE_VARIABLE_LOCAL)
		mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_NORMAL);
		*/
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_compiler_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		mana_compiler_genearte_code(node->left, enable_load);
		break;

		// �u���b�N�𔺂�����Ɋւ���m�[�h
	case MANA_NODE_BLOCK:
		{
			const int32_t in_depth = mana_symbol_open_block(false);
			mana_compiler_genearte_code(node->left, enable_load);
			mana_compiler_genearte_code(node->right, enable_load);
			const int32_t out_depth = mana_symbol_close_block();
			MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
		}
		break;

	case MANA_NODE_BREAK:
		mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc()));
		break;

	case MANA_NODE_CASE:
		mana_jump_switch_case(node->left);
		if(node->left && node->left->type) node->type = node->left->type;
		mana_compiler_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_CONTINUE:
		mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
		break;

	case MANA_NODE_DEFAULT:
		mana_jump_switch_default();
		break;

	case MANA_NODE_DO:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_DO);
			const int address = mana_code_get_pc();
			mana_compiler_genearte_code(node->left, enable_load);
			mana_jump_close_continue_only();

			mana_code_replace_all(mana_compiler_condition(node->right, false), address);
			mana_jump_close_chain();
		}
		break;

	case MANA_NODE_FOR:
		/* 'for(type variable = expression' �̌`�� */
		{
			//mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL);
			//mana_compiler_expression(mana_node_create_node(MANA_NODE_TYPE_ASSIGN, mana_node_create_leaf($2->name), $4), true);
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
			//$$ = mana_code_get_pc();

			mana_compiler_genearte_code(node->left, enable_load);
			mana_jump_break(mana_compiler_condition(node->left, true));


			mana_jump_close_continue_only();
			mana_compiler_expression(node->right, true);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
			mana_jump_close_chain();
		}
		break;

	case MANA_NODE_GOTO:
		{
			mana_symbol_entry* symbol = mana_symbol_create_label(node->string);
			symbol->etc = mana_code_set_opecode_and_operand(MANA_IL_BRA, symbol->etc);
		}
		break;

	case MANA_NODE_HALT:
		mana_code_set_opecode(MANA_IL_HALT);
		break;

	case MANA_NODE_IF:
		{
			int address = mana_compiler_condition(node->left, true);
			mana_compiler_genearte_code(node->body, enable_load);
			if(node->right)
			{
				const int else_begin_address = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
				mana_code_replace_all(address, mana_code_get_pc());
				mana_compiler_genearte_code(node->right, enable_load);
				address = else_begin_address;
			}
			mana_code_replace_all(address, mana_code_get_pc());
		}
		break;

	case MANA_NODE_LABEL:
		{
			mana_symbol_entry* symbol = mana_symbol_create_label(node->string);
			symbol->address = mana_code_get_pc();
		}
		break;

	case MANA_NODE_LOCK:
		mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOCK);
		mana_code_set_opecode(MANA_IL_NONPREEMPTIVE);
		mana_compiler_genearte_code(node->left, enable_load);
		mana_jump_close_chain();
		mana_code_set_opecode(MANA_IL_PREEMPTIVE);
		break;

	case MANA_NODE_LOOP:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOOP);
			const int address = mana_code_get_pc();
			mana_compiler_genearte_code(node->left, enable_load);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, address);
			mana_jump_close_chain();
		}
		break;

	case MANA_NODE_RETURN:
		mana_compiler_genearte_code(node->left, enable_load);
		if (node->left) node->type = node->left->type;
		mana_compiler_return(mana_function_symbol_entry_pointer, node);
		break;

	case MANA_NODE_ROLLBACK:
		mana_compiler_genearte_code(node->left, enable_load);
		if (node->left) node->type = node->left->type;
		mana_compiler_rollback(node);
		break;

	case MANA_NODE_SWITCH:
		{
			mana_compiler_expression(node->left, false);
			const int address = mana_code_get_pc();
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_SWITCH);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
			mana_jump_open_switch(node->left->type);

			mana_compiler_genearte_code(node->right, enable_load);

			mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc()));
			mana_code_replace_all(address, mana_code_get_pc());
			mana_jump_switch_build();
			mana_jump_close_chain();
			mana_jump_close_switch();
		}
		break;

	case MANA_NODE_WHILE:
		{
			mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_WHILE);
			mana_jump_break(mana_compiler_condition(node->left, true));
			mana_compiler_genearte_code(node->right, enable_load);
			mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
			mana_jump_close_chain();
		}
		break;

		// ����Ɋւ���m�[�h
	case MANA_NODE_COMPLY:
		mana_code_set_opecode(MANA_IL_COMPLY);
		break;

	case MANA_NODE_JOIN:
		mana_compiler_resolve_symbol(node);
		mana_symbol_add_join(node->left, node->right);
		break;

	case MANA_NODE_PRINT:
		mana_compiler_call_print(node->left); 
		break;

	case MANA_NODE_REFUSE:
		mana_code_set_opecode(MANA_IL_REFUSE);
		break;

	case MANA_NODE_REQUEST:
		mana_symbol_add_request(MANA_IL_REQ, node->left, node->right, node->string);
		break;

	case MANA_NODE_STRING:
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_STRING, node->digit);
		break;

	case MANA_NODE_VARIABLE:
		/* variable */
		mana_compiler_genearte_code(node->right, enable_load);
		mana_compiler_genearte_code(node->left, enable_load);

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
			mana_compiler_resolve_load(node);
		}

		break;

	case MANA_NODE_YIELD:
		mana_code_set_opecode(MANA_IL_YIELD);
		break;

		// ���Z�Ɋւ���m�[�h
	case MANA_NODE_ARRAY:
		/* variable[index] */
		mana_compiler_genearte_code(node->right, true);
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, (node->type)->memory_size);
		mana_code_set_opecode(MANA_IL_MUL_INTEGER);
		mana_compiler_genearte_code(node->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if (enable_load)
		{
			mana_compiler_resolve_load(node);
		}
		break;

	case MANA_NODE_ASSIGN:
		//mana_compiler_automatic_cast(node);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_compiler_genearte_code(node->left, false);
		mana_compiler_resolve_store(node->left);
		break;

	case MANA_NODE_CALL:
		// �֐��A�A�N�V�������Ăт܂�
		mana_compiler_resolve_symbol(node);
		mana_compiler_resolve_identifier(node);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_compiler_call(node);
		break;

	case MANA_NODE_CALL_ARGUMENT:
		mana_compiler_genearte_code(node->left, enable_load);
		if (node->left && node->left->type) node->type = node->left->type;
		mana_compiler_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_CONST:
		switch (node->type->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
			if (node->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}
			else {
				mana_code_set_opecode(MANA_IL_PUSH_CHAR);
				mana_code_set_char((int8_t)node->digit);
			}
			break;

		case MANA_DATA_TYPE_SHORT:
			if (node->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}
			else {
				mana_code_set_opecode(MANA_IL_PUSH_SHORT);
				mana_code_set_short((int16_t)node->digit);
			}
			break;

		case MANA_DATA_TYPE_INT:
			if (node->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}
			else {
				mana_code_set_opecode(MANA_IL_PUSH_INTEGER);
				mana_code_set_integer(node->digit);
			}
			break;

		case MANA_DATA_TYPE_FLOAT:
			if (node->real == 0.0f)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_FLOAT);
			}
			else {
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

	case MANA_NODE_EXPRESSION_IF:
		// �O�����Z�q
		{
			int32_t pc1, pc2;
			mana_compiler_condition_core(node->next);
			pc1 = mana_code_set_opecode_and_operand(MANA_IL_BEQ, -1);
			mana_compiler_genearte_code(node->left, enable_load);
			pc2 = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
			mana_code_replace_all(pc1, mana_code_get_pc());
			mana_compiler_genearte_code(node->right, enable_load);
			mana_code_replace_all(pc2, mana_code_get_pc());
		}
		break;

	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
		// TODO:�������Ă�������
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		break;

	case MANA_NODE_MEMOP:
		/* variable.member */
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, node->etc);
		mana_compiler_genearte_code(node->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if (enable_load)
		{
			mana_compiler_resolve_load(node);
		}
		break;

	case MANA_NODE_SIZEOF:
		{
			int max_char = (1 << (8 * CBSZ - 1)) - 1;
			int min_char = -1 << (8 * CBSZ - 1);
			int max_short = (1 << (8 * SBSZ - 1)) - 1;
			int min_short = -1 << (8 * SBSZ - 1);

			node->digit = node->left->type->memory_size;

			if(node->digit <= max_char && node->digit >= min_char)
				node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
			else if(node->digit <= max_short && node->digit >= min_short)
				node->type = mana_type_get(MANA_DATA_TYPE_SHORT);
			else
				node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
		break;

	case MANA_NODE_ADD:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_ADD_FLOAT : MANA_IL_ADD_INTEGER);
		break;

	case MANA_NODE_SUB:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		break;

	case MANA_NODE_MUL:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MUL_FLOAT : MANA_IL_MUL_INTEGER);
		break;

	case MANA_NODE_DIV:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_DIV_FLOAT : MANA_IL_DIV_INTEGER);
		break;

	case MANA_NODE_REM:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MOD_FLOAT : MANA_IL_MOD_INTEGER);
		break;

	case MANA_NODE_POW:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		break;

	case MANA_NODE_AND:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_AND);
		break;

	case MANA_NODE_OR:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_OR);
		break;

	case MANA_NODE_XOR:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_EOR);
		break;

	case MANA_NODE_LSH:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHL);
		break;

	case MANA_NODE_RSH:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHR);
		break;

	case MANA_NODE_NEG:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_code_set_opecode((node->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MINUS_FLOAT : MANA_IL_MINUS_INTEGER);
		break;

	case MANA_NODE_I2F:
		mana_compiler_genearte_code(node->left, enable_load);
		mana_code_set_opecode(MANA_IL_INT2FLOAT);
		break;

	case MANA_NODE_F2I:
		mana_compiler_genearte_code(node->left, enable_load);
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
		// ��r�A�_�����Z�q
		//mana_compiler_automatic_cast(node);
		mana_compiler_genearte_code(node->left, enable_load);
		mana_compiler_genearte_code(node->right, enable_load);
		// TODO:�I�y�R�[�h��ݒ肵�Ă�������
		mana_code_set_opecode((uint8_t)node->etc);
		break;

	case MANA_NODE_SENDER:
		// sender��push���܂�
		mana_code_set_opecode(MANA_IL_PUSH_SENDER);
		break;

	case MANA_NODE_SELF:
		// self��push���܂�
		mana_code_set_opecode(MANA_IL_PUSH_SELF);
		break;

	case MANA_NODE_PRIORITY:
		// priority��push���܂�
		mana_code_set_opecode(MANA_IL_PUSH_PRIORITY);
		break;

	default:
		abort();
		mana_compile_error(node, "illegal right-hand side value");
		break;
	}

	// �q�m�[�h����^���p������
	mana_compiler_inherit_type_from_child_node(node);

	if (node->next)
	{
		// �����ċA�Ȃ̂�goto�ɂď�������
		//mana_compiler_generate_symbol(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
���̕]��
@param	tree			����mana_node
@param	enable_assign	true�Ȃ�Α�����Afalse�Ȃ�΂���ȊO
*/
void mana_compiler_expression(mana_node* tree, int32_t enable_assign)
{
	if (!tree)
		return;

	mana_compiler_resolve_symbol(tree);

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

	mana_compiler_genearte_code(tree, true);

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
			MANA_BUG("illigal type detect");
			break;
		}
	}

	const int32_t out_depth = mana_symbol_close_block();
	MANA_VERIFY(in_depth == out_depth, "�u���b�N�̐[������v���܂��� in:%d out:%d", in_depth, out_depth);
}
