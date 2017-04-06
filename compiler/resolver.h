/*!
mana (compiler)

@file	resolver.h
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

extern void mana_resolver_set_current_file_infomation(node_entry* self);
	
/*!
mana_symbol_lookup���Ăяo��mana_symbol����������self�ɐݒ肵�܂�
@param	self	NODE_IDENTIFIER�m�[�h
*/
extern void mana_resolver_search_symbol_from_name(node_entry* self);
	
/*!
self�ɓo�^���ꂽtype��������΃V���{�����������Ă���type��self�ɐݒ肵�܂�
@param[in]	self	NODE_TYPE_DESCRIPTION�m�[�h
*/
extern void mana_resolver_resolve_type_description(node_entry* self);

/*!
mana_type_create_array���g���Ĕz��̌^��new���܂�
@param[in]	self	NODE_VARIABLE_SIZE�m�[�h
@return		�z��type_description
*/
extern type_description* mana_resolver_resolve_variable_size(node_entry* self);

/*!
mana_symbol_create_variable���Ăяo��
mana_symbol��new����self�ɐݒ肵�܂�
@param[in]	self	NODE_DECLARATOR�m�[�h
*/
extern void mana_resolver_resolve_declarator(node_entry* self, const bool static_variable);

/*!
���ӂ�NODE_TYPE_DESCRIPTION��NODE_DECLARATOR����������
mana_symbol_allocate_memory���g���ă����������蓖�Ă܂�
@param[in]	self	NODE_DECLARE_VARIABLE�m�[�h
*/
extern void mana_resolver_resolve_variable_description(node_entry* self, const symbol_memory_type_id memory_type_id, const bool static_variable);

/*!
�q�m�[�h����^���p������
@param[in]	self	�m�[�h
*/
extern void mana_resolver_resolve_type_from_child_node(node_entry* self);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
