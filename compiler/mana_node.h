/*
 * mana (compiler)
 *
 * @file	mana_node.c
 * @brief	意味解析ノードに関するヘッダーファイル
 * @detail	このファイルは意味解析ノードに関係するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_NODE_H___)
#define ___MANA_NODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
#endif

typedef mana_node* (mana_node_event_funtion_type)(mana_node*);

extern void mana_node_initialize(void);
extern void mana_node_finalize(void);

extern void mana_node_auto_cast(mana_node*);
extern mana_node* mana_node_cast(mana_type_description*, mana_node*);
extern mana_node* mana_node_clone(mana_node*);
extern mana_node* mana_node_allocate(mana_node_type_id);
extern void mana_node_release(mana_node*);
extern mana_node* mana_node_create_leaf(char*);
extern mana_node* mana_node_create_node(mana_node_type_id, mana_node*, mana_node*);
extern mana_node* mana_node_create_member(mana_node*, char*);
extern mana_node* mana_node_create_call_member(mana_node*, char*, mana_node*);
extern mana_node* mana_node_create_function(char*);
extern mana_node* mana_node_create_digit(int);
extern mana_node* mana_node_create_real(float);
extern mana_node* mana_node_create_string(char*);
extern size_t mana_node_get_memory_size(mana_node*);

extern void mana_node_add_event(char* name, mana_node_event_funtion_type function);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
