/*!
mana (compiler)

@file	node.h
@brief	意味解析ノードに関するヘッダーファイル
@detail	このファイルは意味解析ノードに関係するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___NODE_H___)
#define ___NODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif

#if defined(_DEBUG)
#define NODE_CHECK(NODE, MAGIC)	(strcmp(NODE->magic, MAGIC) == 0)
#else
#define NODE_CHECK(NODE, MAGIC)	(false)
#endif

/*!
ノードの初期化
*/
extern void mana_node_initialize(void);

/*!
ノードの終了
*/
extern void mana_node_finalize(void);

/*!
ノードを複製します
@param[in]	org		元になるノードオブジェクト
@return				複製したノードオブジェクト
*/
extern node_entry* mana_node_clone(const node_entry* org);

/*!
ノード作成
@param[in]	id		ノードタイプ番号
@return				ノードオブジェクト
*/
extern node_entry* mana_node_allocate(const node_id id);

/*!
ノードを作成します
@param[in]	id		ノードタイプ番号
@param[in]	left	左辺ノードオブジェクト
@param[in]	right	右辺ノードオブジェクト
@param[in]	body	ノードオブジェクト
@return		ノードオブジェクト
*/
extern node_entry* mana_node_create_node(const node_id id, node_entry* left, node_entry* right, node_entry* body);

/*!
整数ノードを作成します
@param[in]	digit	整数
@return				ノードオブジェクト
*/
extern node_entry* mana_node_create_digit(const int32_t digit);

/*!
実数ノードを作成します
@param[in]	real	実数
@return				ノードオブジェクト
*/
extern node_entry* mana_node_create_real(const float real);

/*!
文字列ノードを作成します
@param[in]	string	文字列
@return		ノードオブジェクト
*/
extern node_entry* mana_node_create_string(const char* string);

/*!
関数宣言ノードを作成します
@param[in]	left
@param[in]	identifier, 
@param[in]	argument_count
@param[in]	body
@return		ノードオブジェクト
*/
extern node_entry* mana_node_create_declare_function(node_entry* left, const char* identifier, node_entry* argument_count, node_entry* body);

/*!
ネイティブ関数宣言ノードを作成します
@param[in]	left
@param[in]	identifier,
@param[in]	argument_count
@param[in]	body
@return		ノードオブジェクト
*/
extern node_entry* mana_node_create_declare_native_function(node_entry* left, const char* identifier, node_entry* argument_count, node_entry* body);

/*!
型キャストノードを挿入します
@param[in]	type	キャストする型
@param[in]	node	ノードオブジェクト
@return				ノードオブジェクト
*/
extern node_entry* mana_node_cast(type_description* type, node_entry* node);

/*!
メモリサイズを取得します
@param[in]	node	ノードオブジェクト
@return				メモリサイズ
*/
extern size_t mana_node_get_memory_size(node_entry* node);

/*!
ノードをダンプします
@param[-in]	node	親ノードオブジェクト
*/
extern void mana_node_dump(const node_entry* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
