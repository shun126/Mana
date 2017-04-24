/*!
mana (compiler)

@file	type.h
@brief	型情報に関するヘッダーファイル
@detail	このファイルは型情報に関係するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_TYPE_H___)
#define ___MANA_TYPE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stdio.h>
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

#define CBSZ sizeof(int8_t)
#define BBSZ sizeof(uint8_t)
#define SBSZ sizeof(int16_t)
#define IBSZ sizeof(int32_t)
#define LBSZ sizeof(int64_t)
#define FBSZ sizeof(float)
#define DBSZ sizeof(double)

extern void type_initialize(void);
extern void type_finalize(void);

extern type_description* type_get(symbol_data_type_id);
extern type_description* type_create(symbol_data_type_id, type_description*, type_description*);
extern type_description* type_create_reference(type_description*);
extern type_description* type_create_array(int32_t);

extern void type_set_array(type_description*, type_description*);
extern int32_t type_compare(type_description*, type_description*);
extern int32_t type_compatible(type_description*, type_description*);
extern void type_print(FILE*, type_description*);

extern type_description* type_actor;
extern type_description* type_string;
extern type_description* type_pointer;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
