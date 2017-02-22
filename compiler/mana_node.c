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
#include <stdarg.h>
#include <string.h>

static size_t mana_node_total_size = 0;

/*!
 * �m�[�h�̏�����
 */
void mana_node_initialize(void)
{
	mana_node_total_size = 0;
}

/*!
 * �m�[�h�̏I��
 */
void mana_node_finalize(void)
{
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
		new_node = mana_node_allocate(MANA_NODE_I2F);
		new_node->type = mana_type_get(MANA_DATA_TYPE_FLOAT);
		new_node->left = node;
	}
	else
	{
		new_node = mana_node_allocate(MANA_NODE_F2I);
		new_node->type = mana_type_get(MANA_DATA_TYPE_INT);
		new_node->left = node;
	}
	return new_node;
}

/*!
�L���X�g
@param[in]	type	�L���X�g����^
@param[in]	node	�m�[�h�I�u�W�F�N�g
@return				�m�[�h�I�u�W�F�N�g
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
			if(node->id == MANA_NODE_CONST)
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
			if(node->id == MANA_NODE_CONST)
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
�����^�ϊ��m�[�h��}�����܂�
@param[in]	node	�m�[�h�I�u�W�F�N�g
*/
void mana_node_auto_cast(mana_node* node)
{
	if(node)
	{
		if(node->id != MANA_NODE_ASSIGN && node->right && node->right->type && node->right->type->tcons == MANA_DATA_TYPE_FLOAT)
		{
			node->type = node->right->type;
		}

		assert(node->type);

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
�m�[�h�𕡐����܂�
@param[in]	org		���ɂȂ�m�[�h�I�u�W�F�N�g
@return				���������m�[�h�I�u�W�F�N�g
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
�m�[�h�쐬
@param[in]	id		�m�[�h�^�C�v�ԍ�
@return				�m�[�h�I�u�W�F�N�g
*/
mana_node* mana_node_allocate(mana_node_type_id id)
{
	mana_node* node;

	node = (mana_node*)mana_calloc(sizeof(mana_node), 1);
#if defined(_DEBUG)
	static uint32_t count = 0;
	snprintf(node->magic, sizeof(node->magic), "N%d", count);
	++count;
#endif
	node->id = id;

	node->filename = mana_lexer_get_current_filename();
	node->line = mana_lexer_get_current_line();

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

		if(node->next)
			mana_node_release(node->next);

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

	node = mana_node_allocate(MANA_NODE_FUNCTION);
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
		node = mana_node_allocate(MANA_NODE_CONST);
		node->digit = symbol->address;
		node->symbol = symbol;
		node->type = mana_type_get(MANA_DATA_TYPE_INT);
		break;

	case MANA_CLASS_TYPE_CONSTANT_INT:
		node = mana_node_allocate(MANA_NODE_CONST);
		node->digit = symbol->address;
		node->symbol = symbol;
		node->type = mana_type_get(MANA_DATA_TYPE_INT);
		break;

	case MANA_CLASS_TYPE_CONSTANT_FLOAT:
		node = mana_node_allocate(MANA_NODE_CONST);
		node->real = symbol->hloat;
		node->symbol = symbol;
		node->type = mana_type_get(MANA_DATA_TYPE_FLOAT);
		break;

	case MANA_CLASS_TYPE_CONSTANT_STRING:
		node = mana_node_allocate(MANA_NODE_CONST);
		node->string = symbol->string;
		node->symbol = symbol;
		node->type = mana_type_string;
		break;

	case MANA_CLASS_TYPE_VARIABLE_STATIC:
	case MANA_CLASS_TYPE_VARIABLE_GLOBAL:
	case MANA_CLASS_TYPE_VARIABLE_ACTOR:
	case MANA_CLASS_TYPE_VARIABLE_LOCAL:
		mana_symbol_is_valid_variable(symbol);
		node = mana_node_allocate(MANA_NODE_VARIABLE);
		node->symbol = symbol;
		node->type = symbol->type;
		break;

	case MANA_CLASS_TYPE_TYPEDEF:
		node = mana_node_allocate(MANA_NODE_VARIABLE);
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
 * @param[in]	next	���̃m�[�h�I�u�W�F�N�g
 * @return		�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_node(mana_node_type_id id, mana_node* left, mana_node* right, mana_node* next)
{
	mana_node* node;

	node = mana_node_allocate(id);
	node->type = left ? left->type : NULL;
	node->left = left;
	node->right = right;
	node->next = next;

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
						node = mana_node_allocate(MANA_NODE_MEMOP);
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

		while(leaf && leaf->left && leaf->left->id == MANA_NODE_ARGUMENT)
			leaf = leaf->left;

		leaf->left = mana_node_create_node(MANA_NODE_ARGUMENT, tree, leaf->left, NULL);

		members->etc++;

		tree = mana_node_create_node(MANA_NODE_CALL, mana_node_create_function(name), members, NULL);
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

	new_node = mana_node_allocate(MANA_NODE_CONST);
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

	new_node = mana_node_allocate(MANA_NODE_CONST);
	new_node->real = real;
	new_node->type = mana_type_get(MANA_DATA_TYPE_FLOAT);

	return new_node;
}

/*!
������m�[�h���쐬���܂�
@param[in]	string	������
@return		�m�[�h�I�u�W�F�N�g
 */
mana_node* mana_node_create_string(char* string)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_STRING);
	new_node->digit = mana_data_set(string);
	new_node->type = mana_type_string;

	return new_node;
}

/*!
�V���{���m�[�h���쐬���܂�
@param[in]	identifier	�V���{����
@return		�m�[�h�I�u�W�F�N�g
*/
mana_node* mana_node_create_identifier(char* identifier)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_IDENTIFIER);
	new_node->string = identifier;
	new_node->type = mana_type_string;

	return new_node;
}

mana_node* mana_node_create_type(mana_type_description* type, const char* identifier)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_TYPE_DESCRIPTION);
	new_node->type = type;
	new_node->string = identifier;

	return new_node;
}

mana_node* mana_node_create_declarator(const char* identifier, mana_node* left)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_VARIABLE_DECLARATION);
	new_node->string = identifier;
	new_node->left = left;

	return new_node;
}

mana_node* mana_node_create_declare_function(mana_node* left, const char* identifier, const int32_t argument_count, mana_node* right)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_DECLARE_FUNCTION);
	new_node->string = identifier;
	new_node->digit = argument_count;
	new_node->left = left;
	new_node->right = right;

	return new_node;
}

mana_node* mana_node_create_declare_native_function(mana_node* left, const char* identifier, const int32_t argument_count, mana_node* next)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_DECLARE_NATIVE_FUNCTION);
	new_node->string = identifier;
	new_node->digit = argument_count;
	new_node->left = left;
	new_node->next = next;

	return new_node;
}

mana_node* mana_node_create_allocate(int32_t size, mana_node* left)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_DECLARE_ALLOCATE);
	new_node->digit = size;

	return new_node;
}

mana_node* mana_node_create_request(mana_node* left, mana_node* right, const char* action)
{
	mana_node* new_node;

	new_node = mana_node_allocate(MANA_NODE_REQUEST);
	new_node->left = left;
	new_node->right = right;
	new_node->string = action;

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
	case MANA_NODE_CONST:			/* �萔 */
	case MANA_NODE_VARIABLE:		/* �ϐ� */
		/* �Q�Ƃ�actor���Aactor�̎��̂�����ł���悤�ɂ��� */
		return node->type->tcons == MANA_DATA_TYPE_ACTOR ? sizeof(void*) : node->type->memory_size;

	case MANA_NODE_ARRAY:			/* variable[argument] = */
	case MANA_NODE_MEMOP:			/* X.variable */
	case MANA_NODE_NEG:			/* �}�������] */
		return node->type->tcons == MANA_DATA_TYPE_ACTOR ? sizeof(void*) : node->type->memory_size;
/*		return mana_node_get_memory_size(node->left);	*/

	case MANA_NODE_SELF:			/* self (actor) */
	case MANA_NODE_PRIORITY:		/* runlevel (integer) */
		return sizeof(int32_t*);

	case MANA_NODE_ASSIGN:			/* = */
	case MANA_NODE_ARGUMENT:		/* �Ăяo�������� */
	case MANA_NODE_INCOMPLETE:		/* �錾�������� */
	case MANA_NODE_FUNCTION:		/* �֐� */
	case MANA_NODE_CALL:			/* �֐��Ăяo�� */
	case MANA_NODE_ADD:			/* ���Z */
	case MANA_NODE_SUB:			/* ���Z */
	case MANA_NODE_MUL:			/* ��Z */
	case MANA_NODE_DIV:			/* ���Z */
	case MANA_NODE_REM:			/* �]�� */
	case MANA_NODE_POW:			/* �ׂ��� */
	case MANA_NODE_NOT:			/* ~ */
	case MANA_NODE_LNOT:			/* ! */
	case MANA_NODE_AND:			/* & */
	case MANA_NODE_OR:				/* | */
	case MANA_NODE_XOR:			/* ^ */
	case MANA_NODE_LSH:			/* << */
	case MANA_NODE_RSH:			/* >> */
	case MANA_NODE_LS:				/* < */
	case MANA_NODE_LE:				/* <= */
	case MANA_NODE_EQ:				/* == */
	case MANA_NODE_NE:				/* != */
	case MANA_NODE_GE:				/* >= */
	case MANA_NODE_GT:				/* > */
	case MANA_NODE_STRING:			/* ������ */
	case MANA_NODE_I2F:			/* ������������֕ϊ� */
	case MANA_NODE_F2I:			/* �������琮���֕ϊ� */
	case MANA_NODE_LOR:			/* || */
	case MANA_NODE_LAND:			/* && */
	case MANA_NODE_SENDER:			/* sender (actor) */
	case MANA_NODE_EXPRESSION_IF:	/* �O�����Z�q '?' */
	default:
#if 1
		return node->type->memory_size;
#else
		mana_compile_error("illigal node type detect");
		return 0;
#endif
	}
}

static bool mana_node_dump_format_flag_ = false;

static void mana_node_dump_format_(FILE* file, const char* format, ...)
{
	if (mana_node_dump_format_flag_)
	{
		mana_node_dump_format_flag_ = false;
		fputc(',', file);
	}

	va_list arg;
	va_start(arg, format);
	vfprintf_s(file, format, arg);
	va_end(arg);

	mana_node_dump_format_flag_ = true;
}

static void mana_node_dump_(FILE* file, const mana_node* node)
{
	static char* name[] = {
		"NEWLINE",

		"ARRAY",								/*!< variable[argument] = */
		"ASSIGN",								/*!< = */
		"MEMOP",								/*!< X.variable */
		"ARGUMENT",							/*!< �Ăяo�������� */
		"CONST",								/*!< �萔 */
		"VARIABLE",							/*!< �ϐ� */
		"INCOMPLETE",							/*!< �錾�������� */
		"FUNCTION",							/*!< �֐� */
		"CALL",								/*!< �֐��Ăяo�� */
		"ADD",									/*!< ���Z */
		"SUB",									/*!< ���Z */
		"MUL",									/*!< ��Z */
		"DIV",									/*!< ���Z */
		"REM",									/*!< �]�� */
		"NEG",									/*!< �}�������] */
		"POW",									/*!< �ׂ��� */
		"NOT",									/*!< ~ */
		"AND",									/*!< & */
		"OR",									/*!< | */
		"XOR",									/*!< ^ */
		"LSH",									/*!< << */
		"RSH",									/*!< >> */
		"LS",									/*!< < */
		"LE",									/*!< <= */
		"EQ",									/*!< == */
		"NE",									/*!< != */
		"GE",									/*!< >= */
		"GT",									/*!< > */
		"STRING",								/*!< ������ */
		"I2F",									/*!< ������������֕ϊ� */
		"F2I",									/*!< �������琮���֕ϊ� */
		"LOR",									/*!< || */
		"LAND",								/*!< && */
		"LNOT",								/*!< ! */
		"HALT",								/*!< halt */
		"YIELD",								/*!< yield */
		"REQUEST",								/*!< req */
		"COMPLY",								/*!< comply (req����) */
		"REFUSE",								/*!< refuse (req����) */
		"JOIN",								/*!< join */
		"SENDER",								/*!< sender (actor) */
		"SELF",								/*!< self (actor) */
		"PRIORITY",							/*!< priority (integer) */
		"EXPRESSION_IF",						/*!< �O�����Z�q '?' */
		"PRINT",								/*!< print */
		"RETURN",								/*!< return */
		"ROLLBACK",							/*!< rollback */

		"BLOCK",								/*!< �u���b�N */
		"IF",									/*!< if */
		"SWITCH",								/*!< switch */
		"CASE",								/*!< case */
		"DEFAULT",								/*!< default */
		"WHILE",								/*!< while */
		"DO",									/*!< do while */
		"FOR",									/*!< for */
		"LOOP",								/*!< loop */
		"LOCK",								/*!< lock */
		"GOTO",								/*!< goto */
		"LABEL",								/*!< label */
		"BREAK",								/*!< break */
		"CONTINUE",							/*!< continue */

		"IDENTIFIER",
		"TYPE_DESCRIPTION",
		"DECLARATOR",

		"VARIABLE_DECLARATION",
		"SIZEOF",

		"DECLARE_ACTOR",						//!< �A�N�^�[�̐錾
		"DECLARE_PHANTOM",
		"DECLARE_MODULE",
		"DECLARE_STRUCT",
		"DECLARE_ACTION",
		"DECLARE_EXTEND",
		"DECLARE_ALLOCATE",
		"DECLARE_STATIC",
		"DECLARE_ALIAS",
		"DECLARE_NATIVE_FUNCTION",
		"DECLARE_VALIABLE",
		"DECLARE_FUNCTION",

		"DEFINE_CONSTANT",
		"UNDEFINE_CONSTANT",

		"MEMBER_FUNCTION",
		"MEMBER_VARIABLE",
	};

#if defined(_DEBUG)
	mana_node_dump_format_(file, "\"magic\": \"%s\"", node->magic);
#endif

	if (node->id < sizeof(name) / sizeof(name[0]))
	{
		mana_node_dump_format_(file, "\"name\": \"%s\"", name[node->id]);
	}
	else
	{
		mana_node_dump_format_(file, "\"name\": \"%d\"", node->id);
	}

	if (node->string)
		mana_node_dump_format_(file, "\"string\": \"%s\"", node->string);
	if (node->type)
		mana_node_dump_format_(file, "\"type\": \"%s\"", node->type->name);

	if (node->left)
	{
		mana_node_dump_format_(file, "\"left\": {\n");
		mana_node_dump_format_flag_ = false;
		mana_node_dump_(file, node->left);
		fprintf(file, "}\n");
	}
	if (node->right)
	{
		mana_node_dump_format_(file, "\"right\": {\n");
		mana_node_dump_format_flag_ = false;
		mana_node_dump_(file, node->right);
		fprintf(file, "}\n");
	}
	if (node->next)
	{
		mana_node_dump_format_(file, "\"next\": {\n");
		mana_node_dump_format_flag_ = false;
		mana_node_dump_(file, node->next);
		fprintf(file, "}\n");
	}
}

void mana_node_dump(const mana_node* node)
{
	FILE* file;
	if (fopen_s(&file, "mana_node.json", "wt") == 0)
	{
		if (node)
		{
			fputc('{', file);
			mana_node_dump_(file, node);
			fputc('}', file);
		}
		fclose(file);
	}
}
