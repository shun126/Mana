/*!
mana (compiler)

@file	mana_compile.h
@brief	コンパイルセクションに関するソースファイル
@detail	このファイルはコンパイルセクションに関係するソースファイルです。
@author	Shun Moriya <shun@mnu.sakura.ne.jp>
@date	2017-
*/

#if !defined(___MANA_COMPILER_H___)
#define ___MANA_COMPILER_H___

#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//! MANA_NODE_CALLノードを解決する時に呼ばれるコールバック関数の型
typedef mana_node* (mana_compiler_event_funtion_type)(mana_node*);

//! 初期化
extern void mana_compiler_initialize(void);
//! 終了
extern void mana_compiler_finalize(void);

/*!
MANA_NODE_CALLノードを解決する時に呼ばれるコールバック関数を登録します
@param[in]	name		関数名
@param[in]	function	コールバック関数
*/
extern void mana_compiler_add_event(const char* name, mana_compiler_event_funtion_type function);

/*!
ノードを辿りながらシンボル情報を登録します（ただし中間言語に対して評価しません）
@param	node	ノード
*/
extern void mana_compiler_generate_symbol(mana_node* node);

/*!
ノードを辿りながら中間言語に翻訳します
@param	node			ノード
@param	enable_load		trueならばload命令は有効、falseならばload命令は無効
*/
extern void mana_compiler_genearte_symbol(mana_node* node, int32_t enable_load);

extern void mana_compiler_expression(mana_node* node, int32_t enable_assign);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
