/*!
 * mana (compiler)
 *
 * @file	mana_data.h
 * @brief	データセクションに関するヘッダーファイル
 * @detail	このファイルはデータセクションに関係するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_DATA_H___)
#define ___MANA_DATA_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stdio.h>
#include <libmana.h>

/*! データセクション管理関数を初期化 */
extern void mana_data_initialzie(void);

/*! データセクション管理関数を終了 */
extern void mana_data_finalize(void);

/*! データセクションバッファのアドレスを取得 */
extern char* mana_data_get_buffer(void);

/*! データセクションのサイズを取得 */
extern uint32_t mana_data_get_size(void);

/*! テキストデータを検索 */
extern uint32_t mana_data_get(char* text);

/*! テキストデータを設定 */
extern uint32_t mana_data_set(char* text);

/*! データセクションをファイルへ出力 */
extern uint32_t mana_data_write(mana_stream* stream);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
