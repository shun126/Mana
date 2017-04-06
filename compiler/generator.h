/*!
mana (compiler)

@file	generator.h
@brief	コンパイルセクションに関するソースファイル
@detail	このファイルはコンパイルセクションに関係するソースファイルです。
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_GENERATOR_H___)
#define ___MANA_GENERATOR_H___

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//! NODE_CALLノードを解決する時に呼ばれるコールバック関数の型
typedef node_entry* (mana_generator_event_funtion_type)(node_entry*);

//! 初期化
extern void mana_generator_initialize(void);
//! 終了
extern void mana_generator_finalize(void);

/*!
NODE_CALLノードを解決する時に呼ばれるコールバック関数を登録します
@param[in]	name		関数名
@param[in]	function	コールバック関数
*/
extern void mana_generator_add_event(const char* name, mana_generator_event_funtion_type function);

/*!
ノードを辿りながら中間言語に翻訳します
@param	self			ノード
@param	enable_load		trueならばload命令は有効、falseならばload命令は無効
*/
extern void mana_generator_genearte_code(node_entry* self, int32_t enable_load);

extern void mana_generator_expression(node_entry* self, int32_t enable_assign);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
