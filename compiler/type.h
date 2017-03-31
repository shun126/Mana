/*
 * mana (compiler)
 *
 * @file	mana_type.h
 * @brief	�^���Ɋւ���w�b�_�[�t�@�C��
 * @detail	���̃t�@�C���͌^���Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2003-
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

extern void mana_type_initialize(void);
extern void mana_type_finalize(void);

extern mana_type_description* mana_type_get(mana_symbol_data_type_id);
extern mana_type_description* mana_type_create(mana_symbol_data_type_id, mana_type_description*, mana_type_description*);
extern mana_type_description* mana_type_create_reference(mana_type_description*);
extern mana_type_description* mana_type_create_array(int32_t);

extern void mana_type_set_array(mana_type_description*, mana_type_description*);
extern int32_t mana_type_compare(mana_type_description*, mana_type_description*);
extern int32_t mana_type_compatible(mana_type_description*, mana_type_description*);
extern void mana_type_print(FILE*, mana_type_description*);

extern mana_type_description* mana_type_actor;
extern mana_type_description* mana_type_string;
extern mana_type_description* mana_type_pointer;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif