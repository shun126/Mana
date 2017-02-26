/*!
mana (compiler)

@file	mana_symbol.h
@brief	���W�X�^���蓖�ĂɊւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���̓��W�X�^���蓖�ĂɊւ���w�b�_�[�t�@�C���ł��B
@author	Shun Moriya <shun@mnu.sakura.ne.jp>
@date	2003-
*/

#if !defined(___MANA_SYMBOL_H___)
#define ___MANA_SYMBOL_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum mana_symbol_memory_type_id
{
	MANA_MEMORY_TYPE_NORMAL,							/*!< �ϐ��^ */
	MANA_MEMORY_TYPE_PARAMETER							/*!< �����^ */
} mana_symbol_memory_type_id;

typedef enum mana_symbol_class_type_id
{
	MANA_CLASS_TYPE_NEW_SYMBOL,							/*!< �����蓖�Ċ֐� */
	MANA_CLASS_TYPE_TYPEDEF,							/*!< �^��` */
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
	MANA_DATA_TYPE_CHAR,								/*!< int8_t�^ */
	MANA_DATA_TYPE_SHORT,								/*!< int16_t�^ */
	MANA_DATA_TYPE_INT,									/*!< int32_t�^ */
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
	MANA_NODE_NEWLINE,								/*!< ���s */

	MANA_NODE_ARRAY,								/*!< variable[argument] = */
	MANA_NODE_ASSIGN,								/*!< = */
	MANA_NODE_MEMOP,								/*!< X.variable */
	MANA_NODE_CALL_ARGUMENT,							//!< �����i�Ăяo���j
	MANA_NODE_DECLARE_ARGUMENT,							//!< �����i�錾�j
	MANA_NODE_CONST,								/*!< �萔 */
	MANA_NODE_VARIABLE,							/*!< �ϐ� */
	MANA_NODE_CALL,								/*!< �֐��Ăяo�� */
	MANA_NODE_ADD,									/*!< ���Z */
	MANA_NODE_SUB,									/*!< ���Z */
	MANA_NODE_MUL,									/*!< ��Z */
	MANA_NODE_DIV,									/*!< ���Z */
	MANA_NODE_REM,									/*!< �]�� */
	MANA_NODE_NEG,									/*!< �}�������] */
	MANA_NODE_POW,									/*!< �ׂ��� */
	MANA_NODE_NOT,									/*!< ~ */
	MANA_NODE_AND,									/*!< & */
	MANA_NODE_OR,									/*!< | */
	MANA_NODE_XOR,									/*!< ^ */
	MANA_NODE_LSH,									/*!< << */
	MANA_NODE_RSH,									/*!< >> */
	MANA_NODE_LS,									/*!< < */
	MANA_NODE_LE,									/*!< <= */
	MANA_NODE_EQ,									/*!< == */
	MANA_NODE_NE,									/*!< != */
	MANA_NODE_GE,									/*!< >= */
	MANA_NODE_GT,									/*!< > */
	MANA_NODE_STRING,								/*!< ������ */
	MANA_NODE_I2F,									/*!< ������������֕ϊ� */
	MANA_NODE_F2I,									/*!< �������琮���֕ϊ� */
	MANA_NODE_LOR,									/*!< || */
	MANA_NODE_LAND,								/*!< && */
	MANA_NODE_LNOT,								/*!< ! */
	MANA_NODE_HALT,								/*!< halt */
	MANA_NODE_YIELD,								/*!< yield */
	MANA_NODE_REQUEST,								/*!< req */
	MANA_NODE_COMPLY,								/*!< comply (req����) */
	MANA_NODE_REFUSE,								/*!< refuse (req����) */
	MANA_NODE_JOIN,								/*!< join */
	MANA_NODE_SENDER,								/*!< sender (actor) */
	MANA_NODE_SELF,								/*!< self (actor) */
	MANA_NODE_PRIORITY,							/*!< priority (integer) */
	MANA_NODE_EXPRESSION_IF,						/*!< �O�����Z�q '?' */
	MANA_NODE_PRINT,								/*!< print */
	MANA_NODE_RETURN,								/*!< return */
	MANA_NODE_ROLLBACK,							/*!< rollback */

	MANA_NODE_BLOCK,								/*!< �u���b�N */
	MANA_NODE_IF,									/*!< if */
	MANA_NODE_SWITCH,								/*!< switch */
	MANA_NODE_CASE,								/*!< case */
	MANA_NODE_DEFAULT,								/*!< default */
	MANA_NODE_WHILE,								/*!< while */
	MANA_NODE_DO,									/*!< do while */
	MANA_NODE_FOR,									/*!< for */
	MANA_NODE_LOOP,								/*!< loop */
	MANA_NODE_LOCK,								/*!< lock */
	MANA_NODE_GOTO,								/*!< goto */
	MANA_NODE_LABEL,								/*!< label */
	MANA_NODE_BREAK,								/*!< break */
	MANA_NODE_CONTINUE,							/*!< continue */

	MANA_NODE_IDENTIFIER,
	MANA_NODE_TYPE_DESCRIPTION,
	MANA_NODE_DECLARATOR,

	MANA_NODE_DECLARE_VARIABLE,
	MANA_NODE_SIZEOF,

	MANA_NODE_DECLARE_ACTOR,						//!< �A�N�^�[�̐錾
	MANA_NODE_DECLARE_PHANTOM,
	MANA_NODE_DECLARE_MODULE,
	MANA_NODE_DECLARE_STRUCT,
	MANA_NODE_DECLARE_ACTION,
	MANA_NODE_DECLARE_EXTEND,
	MANA_NODE_DECLARE_ALLOCATE,
	MANA_NODE_DECLARE_STATIC,
	MANA_NODE_DEFINE_ALIAS,
	MANA_NODE_DECLARE_NATIVE_FUNCTION,
	MANA_NODE_DECLARE_FUNCTION,

	MANA_NODE_DEFINE_CONSTANT,
	MANA_NODE_UNDEFINE_CONSTANT,

	MANA_NODE_MEMBER_FUNCTION,
	MANA_NODE_MEMBER_VARIABLE,

	MANA_NODE_VARIABLE_SIZE,
} mana_node_type_id;

typedef struct mana_symbol_entry
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct mana_symbol_entry* next;						/*!< ���̃V���{���ւ̃����N */
	struct mana_symbol_entry* hash_chain;				/*!< �n�b�V���`�F�C�� */
	struct mana_symbol_entry* parameter_list;			/*!< �p�����[�^�ւ̃����N */
	struct mana_type_description* type;					/*!< mana_type_description�̃����N */
	mana_symbol_class_type_id class_type;				/*!< �V���{���̎��ID */
	mana_symbol_memory_type_id attrib;					/*!< �V���{���̃������̎�� */
	char* name;											/*!< ���� */
	int32_t address;									/*!< �A�h���X */
	int32_t etc;										/*!< �ėp(�p�~�\��) */
	float hloat;										/*!< ���������_�ۑ��o�b�t�@ */
	char* string;										/*!< ������ۑ��o�b�t�@ */
	int32_t define_level;								/*!< ��`���x�� */
	int32_t number_of_parameters;						/*!< �p�����[�^�̐� */
	bool override;										//!< �㏑������
	bool used;											/*!< �Q�ƃt���O */
} mana_symbol_entry;

typedef struct mana_type_description
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct mana_type_description* parent;				/*!< �e�̌^�ւ̃����N(�p�����Ȃ��Ȃ����̂Ŗ��g�p) */
	struct mana_type_description* next;					/*!< ���̌^�ւ̃����N */
	struct mana_type_description* component;			/*!< �z��A�Q�ƌ^�̌��̌^�ւ̃����N */
	mana_symbol_data_type_id tcons;						/*!< �^��ID */
	char* name;											/*!< �^�̖��� */
	int32_t number_of_elements;							/*!< �z��̃T�C�Y */
	int32_t memory_size;								/*!< �������T�C�Y */
	int32_t alignment_memory_size;						/*!< �A���C�����g�T�C�Y */

	union share
	{
		struct actor
		{
			int32_t phantom;							/*!< phantom? */
		}actor;
	}share;
} mana_type_description;

typedef struct mana_node
{
#if defined(_DEBUG)
	char magic[8];
#endif
	mana_node_type_id id;								/*!< �m�[�hID */
	struct mana_symbol_entry* symbol;					/*!< �V���{���ւ̃|�C���^ */
	struct mana_type_description* type;					/*!< �^�ւ̃|�C���^ */
	struct mana_node* left;								/*!< �� */
	struct mana_node* right;							/*!< �E */
	struct mana_node* body;								/*!< �E */
	struct mana_node* next;								/*!< �� */
	int32_t etc;										/*!< ���̑� */
	int32_t digit;										/*!< ���� */
	float real;											/*!< ���� */
	char* string;										/*!< ������ւ̃|�C���^ */

	char* filename;
	int32_t line;
	struct mana_node* link;								/*!< ����p�|�C���^ */
} mana_node;

/*****************************************************************************/
extern int32_t mana_symbol_frame_size_list;				/*!< action,function�̃t���[���T�C�Y���X�g */
extern int32_t mana_symbol_return_address_list;			/*!< action,function�̃��^�[���A�h���X���X�g */

/*****************************************************************************/
extern void mana_symbol_initialize(void);
extern void mana_symbol_finalize(void);

extern int32_t mana_symbol_open_block(const bool reset_max_frame_memory_address);
extern int32_t mana_symbol_close_block(void);

extern int32_t mana_symbol_is_valid_variable(mana_symbol_entry*);

extern int32_t mana_symbol_get_static_memory_address();
extern void mana_symbol_set_static_memory_address(int32_t size);

extern int32_t mana_symbol_get_global_memory_address();
extern void mana_symbol_set_global_memory_address(int32_t size);

extern mana_symbol_entry* mana_symbol_lookup(char*);
extern mana_symbol_entry* mana_symbol_lookup_or_create_dummy(char*);
extern mana_symbol_entry* mana_symbol_create_alias(char*, char*);
extern mana_symbol_entry* mana_symbol_create_const_int(char*, int32_t);
extern mana_symbol_entry* mana_symbol_create_const_float(char*, float);
extern mana_symbol_entry* mana_symbol_create_const_string(char*, char*);
//extern mana_symbol_entry* mana_symbol_create_type(char*);
extern mana_symbol_entry* mana_symbol_create_identification(char*, mana_type_description*, int32_t static_variable);
extern mana_symbol_entry* mana_symbol_create_label(char*);

extern void mana_symbol_destroy(char* name);



// function
extern mana_symbol_entry* mana_symbol_create_function(const char* name);
extern void mana_symbol_begin_function_registration(bool is_action, mana_symbol_entry* function, mana_type_description* type);
extern void mana_symbol_commit_function_registration(mana_symbol_entry*);
extern void mana_symbol_begin_native_function_registration();
extern void mana_symbol_commit_native_function_registration(mana_symbol_entry* function, mana_type_description* type);



// struct
extern void mana_symbol_open_structure(void);
extern mana_type_description* mana_symbol_close_structure(const char* name);

// actor
/*!
�A�N�^�[�̃V���{���o�^���J�n���܂�
@param[in]	symbol	NULL�Ȃ�ΐV�K�쐬�A
*/
extern void mana_symbol_open_actor(mana_symbol_entry* symbol);
extern mana_type_description* mana_symbol_close_actor(char* name, char* parent, mana_type_description* td, int32_t phantom);

// module
extern void mana_symbol_open_module(mana_symbol_entry* symbol);
extern mana_type_description* mana_symbol_close_module(const char* name);
extern void mana_symbol_extend_module(const char* name);

extern void mana_symbol_set_type(const char* name, mana_type_description* type);

extern int32_t mana_symbol_get_number_of_actors(void);

extern void mana_symbol_add_request(uint8_t opcode, mana_node* level, mana_node* actor, char* action);
extern void mana_symbol_add_join(mana_node* level, mana_node* actor);

extern void mana_symbol_allocate_memory(mana_symbol_entry*, mana_type_description*, mana_symbol_memory_type_id);
extern void mana_symbol_check_undefine(void);

extern void mana_symbol_print_header(void);
extern void mana_symbol_print_footer(char*, mana_type_description*);
extern void mana_symbol_print_entry(mana_symbol_entry*, mana_type_description*);
extern void mana_symbol_print_dummy_global_variable(size_t size);

extern bool mana_symbol_write_actor_infomation(mana_stream* stream);

extern void mana_symbol_dump(FILE*);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
