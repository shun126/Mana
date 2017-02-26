/*
mana (compiler)

@file	mana_code.h
@brief	命令セクションに関するヘッダーファイル
@detail	このファイルは命令セクションに関係するヘッダーファイルです。
@author	Shun Moriya <shun@mnu.sakura.ne.jp>
@date	2003-
*/

#if !defined(___MANA_CODE_H___)
#define ___MANA_CODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>
#include <stdio.h>

/*! 命令セクション管理関数を初期化 */
extern void mana_code_initialize(void);

/*! 命令セクション管理関数を終了 */
extern void mana_code_finalize(void);

/*! 命令セクションを縮小 */
extern void mana_code_reduce(size_t size);

/*! int8_t型数値を命令セクションに設定 */
extern void mana_code_set_char(int8_t value);

/*! int16_t型数値を命令セクションに設定 */
extern void mana_code_set_short(int16_t value);

/*! int32_t型数値を命令セクションに設定 */
extern void mana_code_set_integer(int32_t value);

/*! uint8_t型数値を命令セクションに設定 */
extern void mana_code_set_unsigned_char(uint8_t value);

/*! uint16_t型数値を命令セクションに設定 */
extern void mana_code_set_unsigned_short(uint16_t value);

/*! uint32_t型数値を命令セクションに設定 */
extern void mana_code_set_unsigned_integer(uint32_t value);

/*! float型数値を命令セクションに設定 */
extern void mana_code_set_float(float value);

/*! オペコードを命令セクションに設定 */
extern void mana_code_set_opecode(uint8_t code);

/*! オペコードとオペランドを命令セクションに設定 */
extern int32_t mana_code_set_opecode_and_operand(uint8_t code, int32_t address);

/*! オペコードと二つのオペランドを命令セクションに設定 */
extern int32_t mana_code_set_opecode_and_two_operands(uint8_t code, int32_t address, int32_t size);

/*! オペコードを置換 */
extern void mana_code_replace_opecode(int32_t address, uint8_t code);

/*! オペコードに関係付けられたアドレスを置換 */
extern void mana_code_replace_address(int32_t address, int32_t new_address);

/*! オペコードに関係付けられたアドレスを辿って置換 */
extern void mana_code_replace_all(int32_t address, int32_t new_address);

/*! 命令セクションをファイルへ出力 */
extern int32_t mana_code_write(mana_stream* stream);

/*! 命令セクションをメモリへ出力 */
extern int32_t mana_code_copy(void*);

/*! 命令セクションのサイズを取得 */
extern int32_t mana_code_get_pc(void);

/*! 命令セクションのサイズを取得 */
extern int32_t mana_code_get_size(void);

/*! 命令セクションバッファのアドレスを取得 */
extern void* mana_code_get_buffer(void);

/*! オペコードを取得 */
extern uint8_t mana_code_getcode(int32_t address);

/*! int32_t型数値を命令セクションから取得 */
extern int32_t mana_code_get_integer(int32_t address);

/*! float型数値を命令セクションから取得 */
extern float mana_code_get_float(int32_t address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
