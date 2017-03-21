/*
 * mana (compiler)
 *
 * @file	mana_evaluator.h
 * @brief	構文木評価に関するヘッダファイル
 * @detail	このファイルは構文木評価に関係するヘッダファイルです。
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

//! 初期化
extern void mana_evaluator_initialize(void);

//! 終了
extern void mana_evaluator_finalize(void);

/*!
子ノードから型を継承する
@param[in]	node	ノード
*/
extern void mana_evaluator_inherit_type_from_child_node(mana_node* node);

/*!
ノードを辿りながらシンボル情報を登録します（ただし中間言語に対して評価しません）
@param	node	ノード
*/
extern void mana_evaluator_evaluate(mana_node* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
