/*
mana (compiler)

@file	mana_resolver.h
@brief	シンボルの解決に関係するヘッダーファイル
@detail	シンボルの解決に関係するヘッダーファイルです。
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

extern void mana_resolver_set_current_file_infomation(mana_node* self);
	
/*!
mana_symbol_lookupを呼び出しmana_symbolを検索してnodeに設定します
@param	node	MANA_NODE_IDENTIFIERノード
*/
extern void mana_resolver_search_symbol_from_name(mana_node* node);
	
/*!
nodeに登録されたtypeが無ければシンボルを検索してそのtypeをnodeに設定します
@param[in]	node	MANA_NODE_TYPE_DESCRIPTIONノード
*/
extern void mana_resolver_resolve_type_description(mana_node* node);

/*!
mana_type_create_arrayを使って配列の型をnewします
@param[in]	node	MANA_NODE_VARIABLE_SIZEノード
@return		配列mana_type_description
*/
extern mana_type_description* mana_resolver_resolve_variable_size(mana_node* node);

/*!
mana_symbol_create_identificationを呼び出し
mana_symbolをnewしてnodeに設定します
@param[in]	node	MANA_NODE_DECLARATORノード
*/
extern void mana_resolver_resolve_declarator(mana_node* node, const bool static_variable);

/*!
両辺のMANA_NODE_TYPE_DESCRIPTIONとMANA_NODE_DECLARATORを解決して
mana_symbol_allocate_memoryを使ってメモリを割り当てます
@param[in]	node	MANA_NODE_DECLARE_VARIABLEノード
*/
extern void mana_resolver_resolve_variable_description(mana_node* node, const mana_symbol_memory_type_id memory_type_id, const bool static_variable);

/*!
子ノードから型を継承する
@param[in]	node	ノード
*/
extern void mana_resolver_resolve_type_from_child_node(mana_node* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
