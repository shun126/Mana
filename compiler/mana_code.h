/*
 * mana (compiler)
 *
 * @file	mana_code.h
 * @brief	命令セクションに関するヘッダーファイル
 * @detail	このファイルは命令セクションに関係するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#define ___MANA_CODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stdio.h>

/*! 命令セクション管理関数を初期化 */
extern void mana_code_initialize(void);

/*! 命令セクション管理関数を終了 */
extern void mana_code_finalize(void);

/*! 命令セクションを縮小 */
extern void mana_code_reduce(size_t size);

/*! char型数値を命令セクションに設定 */
extern void mana_code_set_char(char value);

/*! short型数値を命令セクションに設定 */
extern void mana_code_set_short(short value);

/*! int型数値を命令セクションに設定 */
extern void mana_code_set_integer(int value);

/*! unsigned char型数値を命令セクションに設定 */
extern void mana_code_set_unsigned_char(unsigned char value);

/*! unsigned short型数値を命令セクションに設定 */
extern void mana_code_set_unsigned_short(unsigned short value);

/*! unsigned int型数値を命令セクションに設定 */
extern void mana_code_set_unsigned_integer(unsigned int value);

/*! float型数値を命令セクションに設定 */
extern void mana_code_set_float(float value);

/*! オペコードを命令セクションに設定 */
extern void mana_code_set_opecode(unsigned char code);

/*! オペコードとオペランドを命令セクションに設定 */
extern int mana_code_set_opecode_and_operand(unsigned char code, int address);

/*! オペコードと二つのオペランドを命令セクションに設定 */
extern int mana_code_set_opecode_and_two_operands(unsigned char code, int address, int size);

/*! オペコードを置換 */
extern void mana_code_replace_opecode(int address, unsigned char code);

/*! オペコードに関係付けられたアドレスを置換 */
extern void mana_code_replace_address(int address, int new_address);

/*! オペコードに関係付けられたアドレスを辿って置換 */
extern void mana_code_replace_all(int address, int new_address);

/*! 命令セクションをファイルへ出力 */
extern int mana_code_write(FILE* file);

/*! 命令セクションをメモリへ出力 */
extern int mana_code_copy(void*);

/*! 命令セクションのサイズを取得 */
extern int mana_code_get_pc(void);

/*! 命令セクションのサイズを取得 */
extern int mana_code_get_size(void);

/*! 命令セクションバッファのアドレスを取得 */
extern void* mana_code_get_buffer(void);

/*! オペコードを取得 */
extern unsigned char mana_code_getcode(int address);

/*! int型数値を命令セクションから取得 */
extern int mana_code_get_integer(int address);

/*! float型数値を命令セクションから取得 */
extern float mana_code_get_float(int address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
