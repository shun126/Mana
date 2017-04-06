/*!
mana (compiler)

@file	resolver.h
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

extern void mana_resolver_set_current_file_infomation(node_entry* self);
	
/*!
mana_symbol_lookupを呼び出しmana_symbolを検索してselfに設定します
@param	self	NODE_IDENTIFIERノード
*/
extern void mana_resolver_search_symbol_from_name(node_entry* self);
	
/*!
selfに登録されたtypeが無ければシンボルを検索してそのtypeをselfに設定します
@param[in]	self	NODE_TYPE_DESCRIPTIONノード
*/
extern void mana_resolver_resolve_type_description(node_entry* self);

/*!
mana_type_create_arrayを使って配列の型をnewします
@param[in]	self	NODE_VARIABLE_SIZEノード
@return		配列type_description
*/
extern type_description* mana_resolver_resolve_variable_size(node_entry* self);

/*!
mana_symbol_create_variableを呼び出し
mana_symbolをnewしてselfに設定します
@param[in]	self	NODE_DECLARATORノード
*/
extern void mana_resolver_resolve_declarator(node_entry* self, const bool static_variable);

/*!
両辺のNODE_TYPE_DESCRIPTIONとNODE_DECLARATORを解決して
mana_symbol_allocate_memoryを使ってメモリを割り当てます
@param[in]	self	NODE_DECLARE_VARIABLEノード
*/
extern void mana_resolver_resolve_variable_description(node_entry* self, const symbol_memory_type_id memory_type_id, const bool static_variable);

/*!
子ノードから型を継承する
@param[in]	self	ノード
*/
extern void mana_resolver_resolve_type_from_child_node(node_entry* self);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
