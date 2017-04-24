/*!
mana (compiler)

@file	pool.h
@brief	文字列格納に関するヘッダーファイル
@detail	このファイルは文字列を高速に参照するためのハッシュに関係するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_POOL_H___)
#define ___MANA_POOL_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! 文字列格納システムの初期化 */
extern void pool_initialize(void);

/*! 文字列格納システムの終了処理 */
extern void pool_finalize(void);

/*! 文字列格納システムに文字列を設定します */
extern char* pool_set(char* string);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
