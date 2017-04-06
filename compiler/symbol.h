/*!
mana (compiler)

@file	symbol.h
@brief	���W�X�^���蓖�ĂɊւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���̓��W�X�^���蓖�ĂɊւ���w�b�_�[�t�@�C���ł��B
@author	Shun Moriya
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

typedef enum symbol_memory_type_id
{
	MEMORY_TYPE_NORMAL,						//!< �ϐ��^
	MEMORY_TYPE_PARAMETER					//!< �����^
} symbol_memory_type_id;

typedef enum symbol_class_type_id
{
	SYMBOL_CLASS_TYPE_NEW_SYMBOL,			//!< �����蓖�Ċ֐�
	SYMBOL_CLASS_TYPE_TYPEDEF,				//!< �^��`
	SYMBOL_CLASS_TYPE_FUNCTION,				//!< �֐�
	SYMBOL_CLASS_TYPE_NATIVE_FUNCTION,		//!< �O���֐�
	SYMBOL_CLASS_TYPE_MEMBER_FUNCTION,		//!< �����o�[�֐�(�A�N�V����)
	SYMBOL_CLASS_TYPE_VARIABLE_STATIC,		//!< �X�^�e�B�b�N�O���[�o���ϐ�
	SYMBOL_CLASS_TYPE_VARIABLE_GLOBAL,		//!< �O���[�o���ϐ�
	SYMBOL_CLASS_TYPE_VARIABLE_ACTOR,		//!< �C���X�^���X�ϐ�
	SYMBOL_CLASS_TYPE_VARIABLE_LOCAL,		//!< ���[�J���ϐ�
	SYMBOL_CLASS_TYPE_CONSTANT_INT,			//!< ����
	SYMBOL_CLASS_TYPE_CONSTANT_FLOAT,		//!< ����
	SYMBOL_CLASS_TYPE_CONSTANT_STRING,		//!< ������
	SYMBOL_CLASS_TYPE_ALIAS,				//!< �f�[�^�Q��
	SYMBOL_CLASS_TYPE_LABEL,				//!< ���x��
	SYMBOL_CLASS_TYPE_NUMBER_OF				//!< symbol_class_type_id���ڐ�
} symbol_class_type_id;

typedef enum symbol_data_type_id
{
	SYMBOL_DATA_TYPE_VOID,					//!< void�^
	SYMBOL_DATA_TYPE_CHAR,					//!< int8_t�^
	SYMBOL_DATA_TYPE_SHORT,					//!< int16_t�^
	SYMBOL_DATA_TYPE_INT,					//!< int32_t�^
	SYMBOL_DATA_TYPE_FLOAT,					//!< float�^
	SYMBOL_DATA_TYPE_REFERENCE,				//!< reference�^
	SYMBOL_DATA_TYPE_ARRAY,					//!< array�^
	SYMBOL_DATA_TYPE_STRUCT,				//!< struct�^
	SYMBOL_DATA_TYPE_ACTOR,					//!< actor�^
	SYMBOL_DATA_TYPE_MODULE,				//!< module�^
	SYMBOL_DATA_TYPE_NIL,					//!< nil�^
	SYMBOL_DATA_TYPE_INCOMPLETE,			//!< �錾���������̌^
	SYMBOL_DATA_TYPE_NUMBER_OF				//!< symbol_data_type_id�̍��ڐ�
} symbol_data_type_id;

typedef enum node_id
{
	NODE_ARRAY,								//!< variable[argument] =
	NODE_ASSIGN,							//!< =
	NODE_CALL_ARGUMENT,						//!< �����i�Ăяo���j
	NODE_DECLARE_ARGUMENT,					//!< �����i�錾�j
	NODE_CONST,								//!< �萔
	NODE_CALL,								//!< �֐��Ăяo��
	NODE_ADD,								//!< ���Z
	NODE_SUB,								//!< ���Z
	NODE_MUL,								//!< ��Z
	NODE_DIV,								//!< ���Z
	NODE_REM,								//!< �]��
	NODE_NEG,								//!< �}�������]
	NODE_POW,								//!< �ׂ���
	NODE_NOT,								//!< ~
	NODE_AND,								//!< &
	NODE_OR,								//!< |
	NODE_XOR,								//!< ^
	NODE_LSH,								//!< <<
	NODE_RSH,								//!< >>
	NODE_LS,								//!< <
	NODE_LE,								//!< <=
	NODE_EQ,								//!< ==
	NODE_NE,								//!< !=
	NODE_GE,								//!< >=
	NODE_GT,								//!< >
	NODE_STRING,							//!< ������
	NODE_I2F,								//!< ������������֕ϊ�
	NODE_F2I,								//!< �������琮���֕ϊ�
	NODE_LOR,								//!< ||
	NODE_LAND,								//!< &&
	NODE_LNOT,								//!< !
	NODE_HALT,								//!< halt
	NODE_YIELD,								//!< yield
	NODE_REQUEST,							//!< req
	NODE_COMPLY,							//!< comply (req����)
	NODE_REFUSE,							//!< refuse (req����)
	NODE_JOIN,								//!< join
	NODE_SENDER,							//!< sender (actor)
	NODE_SELF,								//!< self (actor)
	NODE_PRIORITY,							//!< priority (integer)
	NODE_EXPRESSION_IF,						//!< �O�����Z�q '?'
	NODE_PRINT,								//!< print
	NODE_RETURN,							//!< return
	NODE_ROLLBACK,							//!< rollback

	NODE_BLOCK,								//!< �u���b�N
	NODE_IF,								//!< if
	NODE_SWITCH,							//!< switch
	NODE_CASE,								//!< case
	NODE_DEFAULT,							//!< default
	NODE_WHILE,								//!< while
	NODE_DO,								//!< do while
	NODE_FOR,								//!< for
	NODE_LOOP,								//!< loop
	NODE_LOCK,								//!< lock
	NODE_GOTO,								//!< goto
	NODE_LABEL,								//!< label
	NODE_BREAK,								//!< break
	NODE_CONTINUE,							//!< continue

	NODE_IDENTIFIER,
	NODE_TYPE_DESCRIPTION,
	NODE_DECLARATOR,

	NODE_DECLARE_VARIABLE,
	NODE_SIZEOF,

	NODE_DECLARE_ACTOR,						//!< �A�N�^�[�̐錾
	NODE_DECLARE_PHANTOM,
	NODE_DECLARE_MODULE,
	NODE_DECLARE_STRUCT,
	NODE_DECLARE_ACTION,
	NODE_DECLARE_EXTEND,
	NODE_DECLARE_ALLOCATE,
	NODE_DECLARE_STATIC,
	NODE_DEFINE_ALIAS,
	NODE_DECLARE_NATIVE_FUNCTION,
	NODE_DECLARE_FUNCTION,

	NODE_DEFINE_CONSTANT,
	NODE_UNDEFINE_CONSTANT,

	NODE_MEMBER_FUNCTION,
	NODE_MEMBER_VARIABLE,

	NODE_VARIABLE_SIZE,
} node_id;

typedef struct symbol_entry
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct symbol_entry* next;				//!< ���̃V���{���ւ̃����N
	struct symbol_entry* hash_chain;		//!< �n�b�V���`�F�C��
	struct symbol_entry* parameter_list;	//!< �p�����[�^�ւ̃����N
	struct type_description* type;			//!< type_description�̃����N
	symbol_class_type_id class_type;		//!< �V���{���̎��ID
	symbol_memory_type_id attrib;			//!< �V���{���̃������̎��
	const char* name;						//!< ����
	int32_t address;						//!< �A�h���X
	int32_t etc;							//!< �ėp(�p�~�\��)
	float hloat;							//!< ���������_�ۑ��o�b�t�@
	const char* string;						//!< ������ۑ��o�b�t�@
	int32_t define_level;					//!< ��`���x��
	int32_t number_of_parameters;			//!< �p�����[�^�̐�
	bool override;							//!< �㏑������
	bool used;								//!< �Q�ƃt���O

	struct symbol_entry* link;				//!< ����p�|�C���^
} symbol_entry;

typedef struct type_description
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct type_description* parent;		//!< �e�̌^�ւ̃����N(�p�����Ȃ��Ȃ����̂Ŗ��g�p)
	struct type_description* component;		//!< �z��A�Q�ƌ^�̌��̌^�ւ̃����N
	symbol_data_type_id tcons;				//!< �^��ID
	const char* name;						//!< �^�̖���
	int32_t number_of_elements;				//!< �z��̃T�C�Y
	int32_t memory_size;					//!< �������T�C�Y
	int32_t alignment_memory_size;			//!< �A���C�����g�T�C�Y

	union share
	{
		struct actor
		{
			int32_t phantom;				//!< phantom?
		}actor;
	}share;

	struct type_description* link;			//!< ����p�|�C���^
} type_description;

typedef struct node_entry
{
#if defined(_DEBUG)
	char magic[8];
#endif
	node_id id;								//!< �m�[�hID
	struct symbol_entry* symbol;			//!< �V���{���ւ̃|�C���^
	struct type_description* type;			//!< �^�ւ̃|�C���^
	struct node_entry* left;				//!< ��
	struct node_entry* right;				//!< �E
	struct node_entry* body;				//!< �E
	struct node_entry* next;				//!< ��
	int32_t etc;							//!< ���̑�
	int32_t digit;							//!< ����
	float real;								//!< ����
	const char* string;						//!< ������ւ̃|�C���^

	const char* filename;					//!< �t�@�C����
	int32_t line;							//!< �s�ԍ�
	struct node_entry* link;				//!< ����p�|�C���^
} node_entry;

/*****************************************************************************/
extern int32_t mana_symbol_frame_size_list;				/*!< action,function�̃t���[���T�C�Y���X�g */
extern int32_t mana_symbol_return_address_list;			/*!< action,function�̃��^�[���A�h���X���X�g */

/*****************************************************************************/
extern void mana_symbol_initialize(void);
extern void mana_symbol_finalize(void);

extern int32_t mana_symbol_open_block(const bool reset_max_frame_memory_address);
extern int32_t mana_symbol_close_block(void);

extern symbol_entry* mana_symbol_get_head_symbol(void);

extern int32_t mana_symbol_is_valid_variable(symbol_entry*);

extern int32_t mana_symbol_get_static_memory_address();
extern void mana_symbol_set_static_memory_address(const int32_t size);

extern int32_t mana_symbol_get_global_memory_address();
extern void mana_symbol_set_global_memory_address(const int32_t size);

extern symbol_entry* mana_symbol_lookup(const char* name);
extern symbol_entry* mana_symbol_lookup_or_create_dummy(const char* name);
extern symbol_entry* mana_symbol_create_alias(const char*, const char*);
extern symbol_entry* mana_symbol_create_const_int(const char*, const int32_t);
extern symbol_entry* mana_symbol_create_const_float(const char*, const float);
extern symbol_entry* mana_symbol_create_const_string(const char*, const char*);
extern symbol_entry* mana_symbol_create_variable(const char* name, type_description* type, const bool static_variable);
extern symbol_entry* mana_symbol_create_label(const char*);

extern void mana_symbol_destroy(const char* name);

// function
extern symbol_entry* mana_symbol_create_function(const char* name);
extern void mana_symbol_open_function(node_entry* node, const bool is_action);
extern void mana_symbol_close_function(node_entry* node, const bool is_action);

extern void mana_symbol_begin_native_function_registration();
extern void mana_symbol_commit_native_function_registration(symbol_entry* function, type_description* type);

////////////////////////////////////////////////////////////////////////////////////////////////////
// struct
extern void mana_symbol_open_structure(void);
extern void mana_symbol_close_structure(const char* name);

////////////////////////////////////////////////////////////////////////////////////////////////////
// actor
/*!
�A�N�^�[�V���{���̓o�^���J�n���܂�
@param[in]	symbol	NULL�Ȃ�ΐV�K�쐬
*/
extern void mana_symbol_begin_registration_actor(symbol_entry* symbol);

/*!
�A�N�^�[�V���{���̓o�^���m�肵�܂�
*/
extern void mana_symbol_commit_registration_actor(const char* name, const char* parent, type_description* td, const bool phantom);

/*!
�A�N�^�[�̃V���{���o�^���J�n���܂�
@param[in]	symbol	
*/
extern void mana_symbol_open_actor(const char* name);
extern void mana_symbol_close_actor();

////////////////////////////////////////////////////////////////////////////////////////////////////
// module
extern void mana_symbol_begin_registration_module(symbol_entry* symbol);
extern void mana_symbol_commit_registration_module(const char* name);

extern void mana_symbol_open_module(symbol_entry* symbol);
extern void mana_symbol_close_module(const char* name);

extern void mana_symbol_extend_module(const char* name);

////////////////////////////////////////////////////////////////////////////////////////////////////
extern void mana_symbol_set_type(const char* name, type_description* type);

extern int32_t mana_symbol_get_number_of_actors(void);

extern void mana_symbol_add_request(uint8_t opcode, node_entry* level, node_entry* actor, const char* action);
extern void mana_symbol_add_join(node_entry* level, node_entry* actor);

extern void mana_symbol_allocate_memory(symbol_entry*, type_description*, symbol_memory_type_id);
extern void mana_symbol_check_undefine(void);

extern void mana_symbol_print_header(void);
extern void mana_symbol_print_footer(const char*, type_description*);
extern void mana_symbol_print_entry(symbol_entry*, type_description*);
extern void mana_symbol_print_dummy_global_variable(size_t size);

extern bool mana_symbol_write_actor_infomation(mana_stream* stream);

extern void mana_symbol_dump(FILE*);
extern void mana_symbol_dump_function_symbol_from_address(FILE* log, const int32_t address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
