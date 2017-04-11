/*!
mana (compiler)

@file	code.h
@brief	命令セクションに関するヘッダーファイル
@detail	このファイルは命令セクションに関係するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_CODE_H___)
#define ___MANA_CODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>

//! 命令セクション管理関数を初期化
extern void code_initialize(void);

//! 命令セクション管理関数を終了
extern void code_finalize(void);

/*!
命令セクションを縮小
@param[in]	size	縮小するサイズ
*/
extern void code_reduce(const size_t size);

/*!
int8_t型数値を命令セクションに設定
@param[in]	value	値
*/
extern void code_set_char(const int8_t value);

/*!
int16_t型数値を命令セクションに設定
@param[in]	value	値
*/
extern void code_set_short(const int16_t value);

/*!
int32_t型数値を命令セクションに設定
@param[in]	value	値
*/
extern void code_set_integer(const int32_t value);

/*!
uint8_t型数値を命令セクションに設定
@param[in]	value	値
*/
extern void code_set_unsigned_char(const uint8_t value);

/*!
uint16_t型数値を命令セクションに設定
@param[in]	value	値
*/
extern void code_set_unsigned_short(const uint16_t value);

/*!
uint32_t型数値を命令セクションに設定
@param[in]	value	値
*/
extern void code_set_unsigned_integer(const uint32_t value);

/*!
float型数値を命令セクションに設定
@param[in]	value	値
*/
extern void code_set_float(const float value);

/*!
オペコードを命令セクションに設定
@param[in]	code	オペコード
*/
extern void code_set_opecode(const uint8_t code);

/*!
オペコードとオペランドを命令セクションに設定
@param[in]	code	オペコード
@param[in]	address	オペランド
*/
extern int32_t code_set_opecode_and_operand(const uint8_t code, const int32_t address);

/*!
* @param[in]	code	オペコード
* @param[in]	address	第一オペランド
* @param[in]	size	第二オペランド
オペコードと二つのオペランドを命令セクションに設定
*/
extern int32_t code_set_opecode_and_two_operands(const uint8_t code, const int32_t address, const int32_t size);

/*!
オペコードを置換
@param[in]	address	アドレス
@param[in]	code	オペコード
*/
extern void code_replace_opecode(const int32_t address, const uint8_t code);

/*!
オペコードに関係付けられたアドレスを置換
@param[in]	address		アドレス
@param[in]	new_address	置き換えるアドレス
*/
extern void code_replace_address(const int32_t address, const int32_t new_address);

/*!
オペコードに関係付けられたアドレスを辿って置換
@param[in]	base_address	アドレス
@param[in]	new_address		置き換えるアドレス
*/
extern void code_replace_all(const int32_t base_address, const int32_t new_address);

/*!
命令セクションをファイルへ出力
@param[out]	file	ファイル識別子
@retval		true	出力成功
@retval		false	出力失敗
*/
extern bool code_write(mana_stream* stream);

/*!
命令セクションをメモリへ出力
@param[out]	file	ファイル識別子
@return		出力したサイズ
*/
extern int32_t code_copy(void* buffer);

/*!
命令セクションのサイズを取得
@return	命令セクションのサイズ
*/
extern int32_t code_get_pc(void);

/*!
命令セクションのサイズを取得
@return	命令セクションのサイズ
*/
extern int32_t code_get_size(void);

/*!
命令セクションバッファのアドレスを取得
@return	命令セクションバッファのアドレス
*/
extern void* code_get_buffer(void);

/*!
オペコードを取得
@param[in]	address		アドレス
@return		オペコード
*/
extern uint8_t code_getcode(const int32_t address);

/*!
int32_t型数値を命令セクションから取得
@param[in]	address		アドレス
@return		int32_t型数値
*/
extern int32_t code_get_integer(const int32_t address);

/*!
float型数値を命令セクションから取得
@param[in]	address		アドレス
@return		float型数値
*/
extern float code_get_float(const int32_t address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
