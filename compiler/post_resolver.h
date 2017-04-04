/*!
mana (compiler)

@file	post_resolver.h
@brief	関数またはアクション内のシンボルの解決に関するヘッダーファイル
@detail	関数またはアクション内のシンボルの解決に関係するヘッダーファイルです。
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
ノードを辿りながらシンボル情報を登録します
@param	node	ノード
@note	mana_generator_genearte_code内のactionまたはfunctionノードからのみ呼び出される
*/
extern void mana_post_resolver_resolve(node_entry* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
