/*
 * mana (compiler)
 *
 * @file	mana_symbol.h
 * @brief	���W�X�^���蓖�ĂɊւ���w�b�_�[�t�@�C��
 * @detail	���̃t�@�C���̓��W�X�^���蓖�ĂɊւ���w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_SYMBOL_H___)
#define ___MANA_SYMBOL_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stdio.h>

typedef enum mana_symbol_memory_type_id
{
	MANA_MEMORY_TYPE_NORMAL,							/*!< �ϐ��^ */
	MANA_MEMORY_TYPE_PARAMETER							/*!< �����^ */
} mana_symbol_memory_type_id;

typedef enum mana_symbol_class_type_id
{
	MANA_CLASS_TYPE_NEW_SYMBOL,							/*!< �����蓖�Ċ֐� */
	MANA_CLASS_TYPE_TYPEDEF,							/*!< �^��` */
	MANA_CLASS_TYPE_PROTOTYPE_FUNCTION,					/*!< �v���g�^�C�v�֐��錾 */
	MANA_CLASS_TYPE_FUNCTION,							/*!< �֐� */
	MANA_CLASS_TYPE_NATIVE_FUNCTION,					/*!< �O���֐� */
	MANA_CLASS_TYPE_MEMBER_FUNCTION,					/*!< �����o�[�֐�(�A�N�V����) */
	MANA_CLASS_TYPE_VARIABLE_STATIC,					/*!< �X�^�e�B�b�N�O���[�o���ϐ� */
	MANA_CLASS_TYPE_VARIABLE_GLOBAL,					/*!< �O���[�o���ϐ� */
	MANA_CLASS_TYPE_VARIABLE_ACTOR,						/*!< �C���X�^���X�ϐ� */
	MANA_CLASS_TYPE_VARIABLE_LOCAL,						/*!< ���[�J���ϐ� */
	MANA_CLASS_TYPE_CONSTANT_INT,						/*!< ���� */
	MANA_CLASS_TYPE_CONSTANT_FLOAT,						/*!< ���� */
	MANA_CLASS_TYPE_CONSTANT_STRING,					/*!< ������ */
	MANA_CLASS_TYPE_ALIAS,								/*!< �f�[�^�Q�� */
	MANA_CLASS_TYPE_LABEL,								/*!< ���x�� */
	MANA_CLASS_TYPE_NUMBER_OF							/*!< mana_symbol_class_type_id���ڐ� */
} mana_symbol_class_type_id;

typedef enum mana_symbol_data_type_id
{
	MANA_DATA_TYPE_VOID,								/*!< void�^ */
	MANA_DATA_TYPE_CHAR,								/*!< char�^ */
	MANA_DATA_TYPE_SHORT,								/*!< short�^ */
	MANA_DATA_TYPE_INT,									/*!< int�^ */
	MANA_DATA_TYPE_FLOAT,								/*!< float�^ */
	MANA_DATA_TYPE_REFERENCE,							/*!< reference�^ */
	MANA_DATA_TYPE_ARRAY,								/*!< array�^ */
	MANA_DATA_TYPE_STRUCT,								/*!< struct�^ */
	MANA_DATA_TYPE_ACTOR,								/*!< actor�^ */
	MANA_DATA_TYPE_MODULE,								/*!< module�^ */
	MANA_DATA_TYPE_NIL,									/*!< nil�^ */
	MANA_DATA_TYPE_INCOMPLETE,							/*!< �錾���������̌^ */
	MANA_DATA_TYPE_NUMBER_OF							/*!< mana_symbol_data_type_id�̍��ڐ� */
} mana_symbol_data_type_id;

typedef enum mana_node_type_id
{
	MANA_NODE_TYPE_ARRAY,								/*!< variable[argument] = */
	MANA_NODE_TYPE_ASSIGN,								/*!< = */
	MANA_NODE_TYPE_MEMOP,								/*!< X.variable */
	MANA_NODE_TYPE_ARGUMENT,							/*!< �Ăяo�������� */
	MANA_NODE_TYPE_CONST,								/*!< �萔 */
	MANA_NODE_TYPE_VARIABLE,							/*!< �ϐ� */
	MANA_NODE_TYPE_INCOMPLETE,							/*!< �錾�������� */
	MANA_NODE_TYPE_FUNCTION,							/*!< �֐� */
	MANA_NODE_TYPE_CALL,								/*!< �֐��Ăяo�� */
	MANA_NODE_TYPE_ADD,									/*!< ���Z */
	MANA_NODE_TYPE_SUB,									/*!< ���Z */
	MANA_NODE_TYPE_MUL,									/*!< ��Z */
	MANA_NODE_TYPE_DIV,									/*!< ���Z */
	MANA_NODE_TYPE_REM,									/*!< �]�� */
	MANA_NODE_TYPE_NEG,									/*!< �}�������] */
	MANA_NODE_TYPE_POW,									/*!< �ׂ��� */
	MANA_NODE_TYPE_NOT,									/*!< ~ */
	MANA_NODE_TYPE_AND,									/*!< & */
	MANA_NODE_TYPE_OR,									/*!< | */
	MANA_NODE_TYPE_XOR,									/*!< ^ */
	MANA_NODE_TYPE_LSH,									/*!< << */
	MANA_NODE_TYPE_RSH,									/*!< >> */
	MANA_NODE_TYPE_LS,									/*!< < */
	MANA_NODE_TYPE_LE,									/*!< <= */
	MANA_NODE_TYPE_EQ,									/*!< == */
	MANA_NODE_TYPE_NE,									/*!< != */
	MANA_NODE_TYPE_GE,									/*!< >= */
	MANA_NODE_TYPE_GT,									/*!< > */
	MANA_NODE_TYPE_STRING,								/*!< ������ */
	MANA_NODE_TYPE_I2F,									/*!< ������������֕ϊ� */
	MANA_NODE_TYPE_F2I,									/*!< �������琮���֕ϊ� */
	MANA_NODE_TYPE_LOR,									/*!< || */
	MANA_NODE_TYPE_LAND,								/*!< && */
	MANA_NODE_TYPE_LNOT,								/*!< ! */
	MANA_NODE_TYPE_SENDER,								/*!< sender (actor) */
	MANA_NODE_TYPE_SELF,								/*!< self (actor) */
	MANA_NODE_TYPE_PRIORITY,							/*!< priority (integer) */
	MANA_NODE_TYPE_EXPRESSION_IF,						/*!< �O�����Z�q '?' */


#if 0
	MANA_NODE_TYPE_NEWLINE,								/*!<  */
	MANA_NODE_TYPE_BLOCK,								/*!<  */
	MANA_NODE_TYPE_ASSIGN,								/*!<  */
	MANA_NODE_TYPE_IF,									/*!<  */
	MANA_NODE_TYPE_SWITCH,								/*!<  */
	MANA_NODE_TYPE_CASE,								/*!<  */
	MANA_NODE_TYPE_DEFAULT,								/*!<  */
	MANA_NODE_TYPE_WHILE,								/*!<  */
	MANA_NODE_TYPE_DO,									/*!<  */
	MANA_NODE_TYPE_FOR,									/*!<  */
	MANA_NODE_TYPE_LOOP,								/*!<  */
	MANA_NODE_TYPE_LOCK,								/*!<  */
	MANA_NODE_TYPE_GOTO,								/*!<  */
	MANA_NODE_TYPE_LABEL,								/*!<  */
	MANA_NODE_TYPE_RETURN,								/*!<  */
	MANA_NODE_TYPE_ROLLBACK,							/*!<  */
	MANA_NODE_TYPE_BREAK,								/*!<  */
	MANA_NODE_TYPE_CONTINUE,							/*!<  */
	MANA_NODE_TYPE_HALT,								/*!<  */
	MANA_NODE_TYPE_YIELD,								/*!<  */
	MANA_NODE_TYPE_COMPLY,								/*!<  */
	MANA_NODE_TYPE_REFUSE,								/*!<  */
	MANA_NODE_TYPE_PRINT,								/*!<  */
	MANA_NODE_TYPE_REQ,									/*!<  */
	MANA_NODE_TYPE_JOIN,								/*!<  */
#endif
} mana_node_type_id;

typedef struct mana_symbol_entry
{
	struct mana_symbol_entry* next;						/*!< ���̃V���{���ւ̃����N */
	struct mana_symbol_entry* hash_chain;				/*!< �n�b�V���`�F�C�� */
	struct mana_symbol_entry* parameter_list;			/*!< �p�����[�^�ւ̃����N */
	struct mana_type_description* type;					/*!< mana_type_description�̃����N */
	mana_symbol_class_type_id class_type;				/*!< �V���{���̎��ID */
	mana_symbol_memory_type_id attrib;					/*!< �V���{���̃������̎�� */
	char* name;											/*!< ���� */
	int address;										/*!< �A�h���X */
	int etc;											/*!< �ėp(�p�~�\��) */
	float hloat;										/*!< ���������_�ۑ��o�b�t�@ */
	char* string;										/*!< ������ۑ��o�b�t�@ */
	int define_level;									/*!< ��`���x�� */
	int number_of_parameters;							/*!< �p�����[�^�̐� */
	unsigned used;										/*!< �Q�ƃt���O */
} mana_symbol_entry;

typedef struct mana_type_description
{
	struct mana_type_description* parent;				/*!< �e�̌^�ւ̃����N(�p�����Ȃ��Ȃ����̂Ŗ��g�p) */
	struct mana_type_description* component;			/*!< �z��A�Q�ƌ^�̌��̌^�ւ̃����N */
	mana_symbol_data_type_id tcons;						/*!< �^��ID */
	char* name;											/*!< �^�̖��� */
	int number_of_elements;								/*!< �z��̃T�C�Y */
	int memory_size;									/*!< �������T�C�Y */
	int alignment_memory_size;							/*!< �A���C�����g�T�C�Y */

	union share
	{
		struct actor
		{
			int phantom;								/*!< phantom? */
		}actor;
	}share;
} mana_type_description;

typedef struct mana_node
{
	mana_node_type_id id;								/*!< �m�[�hID */
	struct mana_symbol_entry* symbol;					/*!< �V���{���ւ̃|�C���^ */
	struct mana_type_description* type;					/*!< �^�ւ̃|�C���^ */
	struct mana_node* left;								/*!< �� */
	struct mana_node* right;							/*!< �E */
	struct mana_node* condition;						/*!< ���� */
	int etc;											/*!< ���̑� */
	int digit;											/*!< ���� */
	float real;											/*!< ���� */
	char* string;										/*!< ������ւ̃|�C���^ */
} mana_node;

/*****************************************************************************/
extern int mana_symbol_frame_size_list;					/*!< action,function�̃t���[���T�C�Y���X�g */
extern int mana_symbol_return_address_list;				/*!< action,function�̃��^�[���A�h���X���X�g */

/*****************************************************************************/
extern void mana_symbol_initialize(void);
extern void mana_symbol_finalize(void);

extern void mana_symbol_open_block(int);
extern void mana_symbol_close_block(void);

extern int mana_symbol_is_valid_variable(mana_symbol_entry*);

extern int mana_symbol_get_static_memory_address();
extern void mana_symbol_set_static_memory_address(int size);

extern int mana_symbol_get_global_memory_address();
extern void mana_symbol_set_global_memory_address(int size);

extern void mana_symbol_destroy(char*);
extern mana_symbol_entry* mana_symbol_lookup(char*);
extern mana_symbol_entry* mana_symbol_lookup_or_create_dummy(char*);
extern mana_symbol_entry* mana_symbol_create_alias(char*, char*);
extern mana_symbol_entry* mana_symbol_create_const_int(char*, int);
extern mana_symbol_entry* mana_symbol_create_const_float(char*, float);
extern mana_symbol_entry* mana_symbol_create_const_string(char*, char*);
extern mana_symbol_entry* mana_symbol_create_type(char*);
extern mana_symbol_entry* mana_symbol_create_identification(char*, mana_type_description*, int static_variable);
extern mana_symbol_entry* mana_symbol_create_label(char*);
extern mana_symbol_entry* mana_symbol_create_function(char*);
extern void mana_symbol_create_prototype(mana_symbol_entry*, mana_type_description*);

extern void mana_symbol_open_function(int, mana_symbol_entry*, mana_type_description*);
extern void mana_symbol_close_function(mana_symbol_entry*);
extern void mana_symbol_set_native_function(mana_symbol_entry*, mana_type_description*);
extern void mana_symbol_set_type(char*, mana_type_description*);

extern void mana_symbol_open_structure(void);
extern mana_type_description* mana_symbol_close_structure(char*);

extern void mana_symbol_open_actor(mana_symbol_entry*);
extern mana_type_description* mana_symbol_close_actor(char* name, char* parent, mana_type_description* td, int phantom);

extern void mana_symbol_open_actor2(mana_symbol_entry*);
extern void mana_symbol_close_actor2(void);

extern void mana_symbol_open_module(void);
extern mana_type_description* mana_symbol_close_module(char* name);
extern void mana_symbol_extend_module(char* name);

extern int mana_symbol_get_number_of_actors(void);

extern void mana_symbol_add_request(unsigned char opcode, mana_node* level, mana_node* actor, char* action);
extern void mana_symbol_add_join(mana_node* level, mana_node* actor);

extern void mana_symbol_allocate_memory(mana_symbol_entry*, mana_type_description*, mana_symbol_memory_type_id);
extern void mana_symbol_check_undefine(void);

extern void mana_symbol_print_header(void);
extern void mana_symbol_print_footer(char*, mana_type_description*);
extern void mana_symbol_print_entry(mana_symbol_entry*, mana_type_description*);
extern void mana_symbol_print_dummy_global_variable(size_t size);

extern int mana_symbol_write_actor_infomation(FILE*);

extern void mana_symbol_dump(FILE*);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
