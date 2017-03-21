/*
 * mana (compiler)
 *
 * @file	mana_evaluator.h
 * @brief	�\���ؕ]���Ɋւ���w�b�_�t�@�C��
 * @detail	���̃t�@�C���͍\���ؕ]���Ɋ֌W����w�b�_�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2017-
 */

#if !defined(___MANA_EVALUATOR_H___)
#define ___MANA_EVALUATOR_H___

#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//! ������
extern void mana_evaluator_initialize(void);

//! �I��
extern void mana_evaluator_finalize(void);

/*!
�q�m�[�h����^���p������
@param[in]	node	�m�[�h
*/
extern void mana_evaluator_inherit_type_from_child_node(mana_node* node);

/*!
�m�[�h��H��Ȃ���V���{������o�^���܂��i���������Ԍ���ɑ΂��ĕ]�����܂���j
@param	node	�m�[�h
*/
extern void mana_evaluator_evaluate(mana_node* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
