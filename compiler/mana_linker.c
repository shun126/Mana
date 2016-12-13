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

static void mana_linker_call(mana_node* node);
static int32_t mana_linker_condition_core(mana_node* node);

/*!
 * �X�^�b�N�ɂ���A�h���X����f�[�^��ǂݍ��ޖ��߂��o�͂���
 * @param[in]	�ǂݍ���mana_type_description
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
 * �X�^�b�N�ɂ���A�h���X�փf�[�^���������ޖ��߂��o�͂���
 * @param[in]	�ǂݍ���mana_type_description
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
		mana_compile_error("illegal type stored");
		break;
	}
}

/*!
 * �m�[�h�ɂ�H��Ȃ���A�������Ԍ���ɖ|�󂵂܂�
 * @param	tree			�m�[�h�|�C���^
 * @param	enable_load		true�Ȃ��load���߂͗L���Afalse�Ȃ��load���߂͖���
 */
static void mana_linker_generate(mana_node* tree, int32_t enable_load)
{
	if(!tree)
		return;

	switch(tree->id)
	{
	case MANA_NODE_TYPE_ASSIGN:
		mana_linker_generate(tree->right, enable_load);
		mana_linker_generate(tree->left, false);
		mana_linker_generate_store(tree->left);
		break;

	case MANA_NODE_TYPE_INCOMPLETE:
	case MANA_NODE_TYPE_FUNCTION:
		mana_linker_generate(tree->right, enable_load);
		mana_linker_generate(tree->left, enable_load);
		break;

	case MANA_NODE_TYPE_VARIABLE:
		/* variable */
		mana_linker_generate(tree->right, enable_load);
		mana_linker_generate(tree->left, enable_load);

		switch((tree->symbol)->class_type)
		{
		case MANA_CLASS_TYPE_VARIABLE_STATIC:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_STATIC_ADDRESS, (tree->symbol)->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_GLOBAL:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_GLOBAL_ADDRESS, (tree->symbol)->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_ACTOR:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_SELF_ADDRESS, (tree->symbol)->address);
			break;

		case MANA_CLASS_TYPE_VARIABLE_LOCAL:
			mana_code_set_opecode_and_operand(MANA_IL_LOAD_FRAME_ADDRESS, (tree->symbol)->address);
			break;

		case MANA_CLASS_TYPE_TYPEDEF:
			break;

		default:
			mana_compile_error("illegal type of expression in statement");
			break;
		}

		if(enable_load)
		{
			mana_linker_generate_load(tree);
		}

		break;

	case MANA_NODE_TYPE_ARRAY:
		/* variable[index] */
		mana_linker_generate(tree->right, true);
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, (tree->type)->memory_size);
		mana_code_set_opecode(MANA_IL_MUL_INTEGER);
		mana_linker_generate(tree->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if(enable_load)
		{
			mana_linker_generate_load(tree);
		}
		break;

	case MANA_NODE_TYPE_MEMOP:
		/* variable.member */
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_INTEGER, tree->etc);
		mana_linker_generate(tree->left, false);
		mana_code_set_opecode(MANA_IL_ADD_INTEGER);
		if(enable_load)
		{
			mana_linker_generate_load(tree);
		}
		break;

	case MANA_NODE_TYPE_CONST:
		switch(tree->type->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
			if(tree->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_CHAR);
				mana_code_set_char((int8_t)tree->digit);
			}
			break;

		case MANA_DATA_TYPE_SHORT:
			if(tree->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_SHORT);
				mana_code_set_short((int16_t)tree->digit);
			}
			break;

		case MANA_DATA_TYPE_INT:
			if(tree->digit == 0)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_INTEGER);
				mana_code_set_integer(tree->digit);
			}
			break;

		case MANA_DATA_TYPE_FLOAT:
			if(tree->real == 0.0f)
			{
				mana_code_set_opecode(MANA_IL_PUSH_ZERO_FLOAT);
			}else{
				mana_code_set_opecode(MANA_IL_PUSH_FLOAT);
				mana_code_set_float(tree->real);
			}
			break;

		case MANA_DATA_TYPE_NIL:
			mana_code_set_opecode(MANA_IL_PUSH_ZERO_INTEGER);
			break;

		default:
			mana_compile_error("illegal type of expression in statement");
			break;
		}
		break;

	case MANA_NODE_TYPE_STRING:
		mana_code_set_opecode_and_operand(MANA_IL_PUSH_STRING, tree->digit);
		break;

	case MANA_NODE_TYPE_ADD:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode((tree->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_ADD_FLOAT : MANA_IL_ADD_INTEGER);
		break;

	case MANA_NODE_TYPE_SUB:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode((tree->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		break;

	case MANA_NODE_TYPE_MUL:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode((tree->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MUL_FLOAT : MANA_IL_MUL_INTEGER);
		break;

	case MANA_NODE_TYPE_DIV:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode((tree->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_DIV_FLOAT : MANA_IL_DIV_INTEGER);
		break;

	case MANA_NODE_TYPE_REM:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode((tree->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MOD_FLOAT : MANA_IL_MOD_INTEGER);
		break;

	case MANA_NODE_TYPE_POW:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode((tree->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
		break;

	case MANA_NODE_TYPE_AND:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode(MANA_IL_AND);
		break;

	case MANA_NODE_TYPE_OR:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode(MANA_IL_OR);
		break;

	case MANA_NODE_TYPE_XOR:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode(MANA_IL_EOR);
		break;

	case MANA_NODE_TYPE_LSH:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHL);
		break;

	case MANA_NODE_TYPE_RSH:
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode(MANA_IL_SHR);
		break;

	case MANA_NODE_TYPE_NEG:
		mana_linker_generate(tree->left, enable_load);
		mana_code_set_opecode((tree->left->type)->tcons == MANA_DATA_TYPE_FLOAT ? MANA_IL_MINUS_FLOAT : MANA_IL_MINUS_INTEGER);
		break;

	case MANA_NODE_TYPE_I2F:
		mana_linker_generate(tree->left, enable_load);
		mana_code_set_opecode(MANA_IL_INT2FLOAT);
		break;

	case MANA_NODE_TYPE_F2I:
		mana_linker_generate(tree->left, enable_load);
		mana_code_set_opecode(MANA_IL_FLOAT2INT);
		break;

	case MANA_NODE_TYPE_LS:
	case MANA_NODE_TYPE_LE:
	case MANA_NODE_TYPE_EQ:
	case MANA_NODE_TYPE_NE:
	case MANA_NODE_TYPE_GE:
	case MANA_NODE_TYPE_GT:
	case MANA_NODE_TYPE_LAND:
	case MANA_NODE_TYPE_LOR:
	case MANA_NODE_TYPE_LNOT:
	case MANA_NODE_TYPE_NOT:
		/* ��r�A�_�����Z�q */
		mana_linker_generate(tree->left, enable_load);
		mana_linker_generate(tree->right, enable_load);
		mana_code_set_opecode((uint8_t)tree->etc);
		break;

	case MANA_NODE_TYPE_CALL:
		/* �֐��A�A�N�V�������Ăт܂� */
		mana_linker_call(tree);
		break;

	case MANA_NODE_TYPE_SENDER:
		/* sender��push���܂� */
		mana_code_set_opecode(MANA_IL_PUSH_SENDER);
		break;

	case MANA_NODE_TYPE_SELF:
		/* self��push���܂� */
		mana_code_set_opecode(MANA_IL_PUSH_SELF);
		break;

	case MANA_NODE_TYPE_PRIORITY:
		/* priority��push���܂� */
		mana_code_set_opecode(MANA_IL_PUSH_PRIORITY);
		break;

	case MANA_NODE_TYPE_EXPRESSION_IF:
		/* �O�����Z�q */
		{
			int32_t pc1, pc2;
			mana_linker_condition_core(tree->condition);
			pc1 = mana_code_set_opecode_and_operand(MANA_IL_BEQ, -1);
			mana_linker_generate(tree->left, enable_load);
			pc2 = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
			mana_code_replace_all(pc1, mana_code_get_pc());
			mana_linker_generate(tree->right, enable_load);
			mana_code_replace_all(pc2, mana_code_get_pc());
		}
		break;

	default:
		mana_compile_error("illegal right-hand side value");
		break;
	}
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
			mana_compile_error("meaningless return value specification");
	}
	else if(tree == NULL)
	{
		mana_compile_error("missing return value");
	}
	else
	{
		/* �����L���X�g */
		tree = mana_node_cast(type, tree);

		/* �^�̌��� */
		mana_type_compatible(type, tree->type);

		/* �m�[�h�̕]�� */
		mana_symbol_open_block(false);
		mana_linker_generate(tree, true);
		mana_symbol_close_block();

		/* �m�[�h�̉�� */
		mana_node_release(tree);
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
		mana_symbol_open_block(false);
		mana_linker_generate(tree, true);
		mana_symbol_close_block();

		/* �m�[�h�̉�� */
		mana_node_release(tree);
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

		if(arg->id == MANA_NODE_TYPE_ARGUMENT)
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

		if(arg->id == MANA_NODE_TYPE_ARGUMENT)
			arg = arg->right;
		address -= (mana_node_get_memory_size(arg) + (sizeof(int32_t) - 1)) / sizeof(int32_t);
	}
	return address;
}

/*!
 * �֐��Ăяo���̃m�[�h��]�����܂�
 * @param	�֐��Ăяo����mana_node
 */
static void mana_linker_call(mana_node* tree)
{
	mana_node* node;
	mana_node* argument;
	mana_type_description* tp;
	int32_t argument_counter;

	node = tree->left;
	argument = tree->right;
	tp = (node->symbol)->type;
	argument_counter = mana_linker_argument((node->symbol)->parameter_list, argument);

	/* �G���[�`�F�b�N */
	if((node->symbol)->number_of_parameters != argument_counter)
	{
		/* �����̐�����v���Ȃ� */
		mana_compile_error("unmatched argument.");
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

		/* �������֐��̏ꍇ�A�����N����A�h���X���L�^���܂� */
		if((node->symbol)->class_type == MANA_CLASS_TYPE_PROTOTYPE_FUNCTION)
			(node->symbol)->address = mana_code_get_pc() - 5;
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
		mana_linker_generate((node->id == MANA_NODE_TYPE_ARGUMENT) ? node->right : node, true);
		argument_counter ++;
	}

	mana_node_release(tree);

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
		if(arg->id == MANA_NODE_TYPE_ARGUMENT)
		{
			count = mana_linker_generate_argument(count, param->next, arg->left);
			arg = arg->right;
		}
		arg = mana_node_cast(param->type, arg);
		mana_type_compatible(param->type, arg->type);
		mana_linker_generate(arg, true);
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

	mana_symbol_open_block(false);

	if(enable_assign)
	{
		if(tree->id != MANA_NODE_TYPE_ASSIGN && tree->id != MANA_NODE_TYPE_CALL && tree->id != MANA_NODE_TYPE_F2I)
			mana_compile_error("illegal expression in write-statement");
	}else{
		if(tree->id == MANA_NODE_TYPE_ASSIGN)
			mana_compile_error("illegal expression in write-statement");
	}

	mana_linker_generate(tree, true);

	if(enable_assign && tree->id == MANA_NODE_TYPE_CALL && tree->type)
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

	mana_symbol_close_block();

	/* node�̉�� */
	mana_node_release(tree);
}

/*!
 * ���ʎ����ɑ���������邩���ׂ܂�
 * @param	tree	�]������mana_node
 */
static void mana_linker_condition_check(mana_node* tree)
{
	if(tree)
	{
		if(tree->id == MANA_NODE_TYPE_ASSIGN)
		{
			/* ��������ɑ�����͎g�p�ł��Ȃ� */
			mana_compile_error("can't assign expression in condition");
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
			mana_compile_error("illegal type of expression in condition");

		mana_linker_generate(tree, true);
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
	mana_symbol_open_block(false);
	mana_linker_condition_core(tree);
	mana_symbol_close_block();

	/* node�̉�� */
	mana_node_release(tree);

	return mana_code_set_opecode_and_operand(match ? MANA_IL_BEQ : MANA_IL_BNE, -1);
}
