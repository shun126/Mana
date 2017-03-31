/*
 * mana (compiler)
 *
 * @file	mana_node.c
 * @brief	�Ӗ���̓m�[�h�Ɋւ���w�b�_�[�t�@�C��
 * @detail	���̃t�@�C���͈Ӗ���̓m�[�h�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_NODE_H___)
#define ___MANA_NODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif

#if defined(_DEBUG)
#define MANA_NODE_CHECK(NODE, MAGIC)	(strcmp(NODE->magic, MAGIC) == 0)
#else
#define MANA_NODE_CHECK(NODE, MAGIC)	(false)
#endif

/*!
�m�[�h�̏�����
*/
extern void mana_node_initialize(void);

/*!
�m�[�h�̏I��
*/
extern void mana_node_finalize(void);

/*!
�m�[�h�𕡐����܂�
@param[in]	org		���ɂȂ�m�[�h�I�u�W�F�N�g
@return				���������m�[�h�I�u�W�F�N�g
*/
extern mana_node* mana_node_clone(mana_node*);

/*!
�m�[�h�쐬
@param[in]	id		�m�[�h�^�C�v�ԍ�
@return				�m�[�h�I�u�W�F�N�g
*/
extern mana_node* mana_node_allocate(const mana_node_type_id id);

/*!
�m�[�h���쐬���܂�
@param[in]	id		�m�[�h�^�C�v�ԍ�
@param[in]	left	���Ӄm�[�h�I�u�W�F�N�g
@param[in]	right	�E�Ӄm�[�h�I�u�W�F�N�g
@param[in]	body	�m�[�h�I�u�W�F�N�g
@return		�m�[�h�I�u�W�F�N�g
*/
extern mana_node* mana_node_create_node(const mana_node_type_id id, mana_node* left, mana_node* right, mana_node* body);

extern mana_node* mana_node_create_digit(int32_t);
extern mana_node* mana_node_create_real(float);
extern mana_node* mana_node_create_string(char* string);
extern mana_node* mana_node_create_type(mana_type_description* type, const char* identifier);

extern mana_node* mana_node_create_declare_function(mana_node* left, const char* identifier, mana_node* argument_count, mana_node* body);
extern mana_node* mana_node_create_declare_native_function(mana_node* left, const char* identifier, mana_node* argument_count, mana_node* body);

extern mana_node* mana_node_create_allocate(int32_t size, mana_node* left);
extern mana_node* mana_node_create_request(mana_node* left, mana_node* right, const char* action);

extern mana_node* mana_node_create_declarator(const char* identifier, mana_node* left);

/*!
�^�L���X�g�m�[�h��}�����܂�
@param[in]	type	�L���X�g����^
@param[in]	node	�m�[�h�I�u�W�F�N�g
@return				�m�[�h�I�u�W�F�N�g
*/
extern mana_node* mana_node_cast(mana_type_description* type, mana_node* node);

/*!
�������T�C�Y���擾���܂�
@param[in]	node	�m�[�h�I�u�W�F�N�g
@return				�������T�C�Y
*/
extern size_t mana_node_get_memory_size(mana_node* node);




extern void mana_node_dump(const mana_node* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif