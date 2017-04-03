/*
mana (compiler)

@file	mana_resolver.h
@brief	�V���{���̉����Ɋ֌W����w�b�_�[�t�@�C��
@detail	�V���{���̉����Ɋ֌W����w�b�_�[�t�@�C���ł��B
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_RESOLVER_H___)
#define ___MANA_RESOLVER_H___

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void mana_resolver_set_current_file_infomation(mana_node* self);
	
/*!
mana_symbol_lookup���Ăяo��mana_symbol����������node�ɐݒ肵�܂�
@param	node	MANA_NODE_IDENTIFIER�m�[�h
*/
extern void mana_resolver_search_symbol_from_name(mana_node* node);
	
/*!
node�ɓo�^���ꂽtype��������΃V���{�����������Ă���type��node�ɐݒ肵�܂�
@param[in]	node	MANA_NODE_TYPE_DESCRIPTION�m�[�h
*/
extern void mana_resolver_resolve_type_description(mana_node* node);

/*!
mana_type_create_array���g���Ĕz��̌^��new���܂�
@param[in]	node	MANA_NODE_VARIABLE_SIZE�m�[�h
@return		�z��mana_type_description
*/
extern mana_type_description* mana_resolver_resolve_variable_size(mana_node* node);

/*!
mana_symbol_create_identification���Ăяo��
mana_symbol��new����node�ɐݒ肵�܂�
@param[in]	node	MANA_NODE_DECLARATOR�m�[�h
*/
extern void mana_resolver_resolve_declarator(mana_node* node, const bool static_variable);

/*!
���ӂ�MANA_NODE_TYPE_DESCRIPTION��MANA_NODE_DECLARATOR����������
mana_symbol_allocate_memory���g���ă����������蓖�Ă܂�
@param[in]	node	MANA_NODE_DECLARE_VARIABLE�m�[�h
*/
extern void mana_resolver_resolve_variable_description(mana_node* node, const mana_symbol_memory_type_id memory_type_id, const bool static_variable);

/*!
�q�m�[�h����^���p������
@param[in]	node	�m�[�h
*/
extern void mana_resolver_resolve_type_from_child_node(mana_node* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
