/*!
mana (compiler)

@file	mana_generator.h
@brief	コンパイルセクションに関するソースファイル
@detail	このファイルはコンパイルセクションに関係するソースファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(__MANA_GENERATOR_H___)
#define __MANA_GENERATOR_H___

#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//! MANA_NODE_CALLノードを解決する時に呼ばれるコールバック関数の型
typedef mana_node* (mana_generator_event_funtion_type)(mana_node*);

//! 初期化
extern void mana_generator_initialize(void);
//! 終了
extern void mana_generator_finalize(void);

/*!
MANA_NODE_CALLノードを解決する時に呼ばれるコールバック関数を登録します
@param[in]	name		関数名
@param[in]	function	コールバック関数
*/
extern void mana_generator_add_event(const char* name, mana_generator_event_funtion_type function);

/*!
ノードを辿りながら中間言語に翻訳します
@param	node			ノード
@param	enable_load		trueならばload命令は有効、falseならばload命令は無効
*/
extern void mana_generator_genearte_code(mana_node* node, int32_t enable_load);

extern void mana_generator_expression(mana_node* node, int32_t enable_assign);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
