/*!
mana (compiler)

@file	pre_resolver.h
@brief	�\���ؕ]���Ɋւ���w�b�_�t�@�C��
@detail	���̃t�@�C���͍\���ؕ]���Ɋ֌W����w�b�_�t�@�C���ł��B
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_PRE_RESOLVER_H___)
#define ___MANA_PRE_RESOLVER_H___

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//! ������
extern void mana_pre_resolver_initialize(void);

//! �I��
extern void mana_pre_resolver_finalize(void);

/*!
�q�m�[�h����^���p������
@param[in]	node	�m�[�h
*/
extern void mana_resolver_resolve_type_from_child_node(node_entry* node);

/*!
�m�[�h��H��Ȃ���V���{������o�^���܂��i���������Ԍ���ɑ΂��ĕ]�����܂���j
@param	node	�m�[�h
*/
extern void mana_pre_resolver_resolve(node_entry* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
