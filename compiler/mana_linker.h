/*!
mana (compiler)

@file	mana_linker.h
@brief	�����N�Ɋւ���w�b�_�t�@�C��
@detail	���̃t�@�C���̓����N�Ɋ֌W����w�b�_�t�@�C���ł��B
@author	Shun Moriya <shun@mnu.sakura.ne.jp>
@date	2003-
*/

#if !defined(___MANA_LINKER_H___)
#define ___MANA_LINKER_H___

#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void mana_linker_initialize(void);
extern void mana_linker_finalize(void);

extern void mana_linker_add_call_list(const int32_t code_address, mana_symbol_entry* symbol);
extern void mana_linker_resolve_address();

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
