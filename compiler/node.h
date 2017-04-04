/*!
mana (compiler)

@file	node.h
@brief	�Ӗ���̓m�[�h�Ɋւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���͈Ӗ���̓m�[�h�Ɋ֌W����w�b�_�[�t�@�C���ł��B
@author	Shun Moriya
@date	2003-
*/

#if !defined(___NODE_H___)
#define ___NODE_H___

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
#define NODE_CHECK(NODE, MAGIC)	(strcmp(NODE->magic, MAGIC) == 0)
#else
#define NODE_CHECK(NODE, MAGIC)	(false)
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
extern node_entry* mana_node_clone(const node_entry* org);

/*!
�m�[�h�쐬
@param[in]	id		�m�[�h�^�C�v�ԍ�
@return				�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_allocate(const node_id id);

/*!
�m�[�h���쐬���܂�
@param[in]	id		�m�[�h�^�C�v�ԍ�
@param[in]	left	���Ӄm�[�h�I�u�W�F�N�g
@param[in]	right	�E�Ӄm�[�h�I�u�W�F�N�g
@param[in]	body	�m�[�h�I�u�W�F�N�g
@return		�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_create_node(const node_id id, node_entry* left, node_entry* right, node_entry* body);

/*!
�����m�[�h���쐬���܂�
@param[in]	digit	����
@return				�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_create_digit(const int32_t digit);

/*!
�����m�[�h���쐬���܂�
@param[in]	real	����
@return				�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_create_real(const float real);

/*!
������m�[�h���쐬���܂�
@param[in]	string	������
@return		�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_create_string(const char* string);

/*!
�֐��錾�m�[�h���쐬���܂�
@param[in]	left
@param[in]	identifier, 
@param[in]	argument_count
@param[in]	body
@return		�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_create_declare_function(node_entry* left, const char* identifier, node_entry* argument_count, node_entry* body);

/*!
�l�C�e�B�u�֐��錾�m�[�h���쐬���܂�
@param[in]	left
@param[in]	identifier,
@param[in]	argument_count
@param[in]	body
@return		�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_create_declare_native_function(node_entry* left, const char* identifier, node_entry* argument_count, node_entry* body);

/*!
�^�L���X�g�m�[�h��}�����܂�
@param[in]	type	�L���X�g����^
@param[in]	node	�m�[�h�I�u�W�F�N�g
@return				�m�[�h�I�u�W�F�N�g
*/
extern node_entry* mana_node_cast(type_description* type, node_entry* node);

/*!
�������T�C�Y���擾���܂�
@param[in]	node	�m�[�h�I�u�W�F�N�g
@return				�������T�C�Y
*/
extern size_t mana_node_get_memory_size(node_entry* node);

/*!
�m�[�h���_���v���܂�
@param[-in]	node	�e�m�[�h�I�u�W�F�N�g
*/
extern void mana_node_dump(const node_entry* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
