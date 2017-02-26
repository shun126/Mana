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

#if defined(_DEBUG)
#define MANA_NODE_CHECK(NODE, MAGIC)	(strcmp(NODE->magic, MAGIC) == 0)
#else
#define MANA_NODE_CHECK(NODE, MAGIC)	(false)
#endif

extern void mana_node_initialize(void);
extern void mana_node_finalize(void);

extern void mana_node_auto_cast(mana_node*);
extern mana_node* mana_node_cast(mana_type_description*, mana_node*);

extern mana_node* mana_node_clone(mana_node*);
extern mana_node* mana_node_allocate(const mana_node_type_id id);
extern mana_node* mana_node_create_node(const mana_node_type_id id, mana_node* left, mana_node* right, mana_node* body);

extern mana_node* mana_node_create_leaf(char*);
extern mana_node* mana_node_create_member(mana_node*, char*);
extern mana_node* mana_node_create_call_member(mana_node*, char*, mana_node*);
extern mana_node* mana_node_create_digit(int32_t);
extern mana_node* mana_node_create_real(float);
extern mana_node* mana_node_create_string(char* string);
extern mana_node* mana_node_create_identifier(char* identifier);
extern mana_node* mana_node_create_type(mana_type_description* type, const char* identifier);

extern mana_node* mana_node_create_declare_function(mana_node* left, const char* identifier, mana_node* argument_count, mana_node* body);
extern mana_node* mana_node_create_declare_native_function(mana_node* left, const char* identifier, mana_node* argument_count, mana_node* body);

extern mana_node* mana_node_create_allocate(int32_t size, mana_node* left);
extern mana_node* mana_node_create_request(mana_node* left, mana_node* right, const char* action);

extern size_t mana_node_get_memory_size(mana_node*);

extern mana_node* mana_node_create_declarator(const char* identifier, mana_node* left);





extern void mana_node_dump(const mana_node* node);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
