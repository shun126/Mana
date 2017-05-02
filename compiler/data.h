/*!
mana (compiler)

@file	data.h
@brief	データセクションに関するヘッダーファイル
@detail	このファイルはデータセクションに関係するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___SYMBOL_DATA_H___)
#define ___SYMBOL_DATA_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>

//! データセクション管理関数を初期化
extern void data_initialzie(void);

//! データセクション管理関数を終了
extern void data_finalize(void);

/*!
データセクションバッファのアドレスを取得
@return	データセクションバッファのアドレス
*/
extern char* data_get_buffer(void);

/*!
データセクションのサイズを取得
@return	データセクションのサイズ
*/
extern uint32_t data_get_size(void);

/*!
テキストデータを検索
@param[in]	text	テキスト
@return		オフセットアドレス
*/
extern uint32_t data_get(const char* text);

/*!
テキストデータを設定
@param[in]	text	テキスト
@return		オフセットアドレス
*/
extern uint32_t data_set(const char* text);

/*!
データセクションをファイルへ出力
@param[out]	file		ファイル識別子
@retval		true	出力成功
@retval		false	出力失敗
*/
extern bool data_write(mana_stream* stream);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
