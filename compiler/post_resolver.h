/*!
mana (compiler)

@file	post_resolver.h
@brief	�֐��܂��̓A�N�V�������̃V���{���̉����Ɋւ���w�b�_�[�t�@�C��
@detail	�֐��܂��̓A�N�V�������̃V���{���̉����Ɋ֌W����w�b�_�[�t�@�C���ł��B
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_POST_RESOLVER_H___)
#define ___MANA_POST_RESOLVER_H___

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*!
�m�[�h��H��Ȃ���V���{������o�^���܂�
@param	node	�m�[�h
@note	mana_generator_genearte_code����action�܂���function�m�[�h����̂݌Ăяo�����
*/
extern void mana_post_resolver_resolve(node_entry* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
