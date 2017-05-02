 /*!
  * mana (library)
  *
  * @file	mana_malloc.h
  * @brief	manaライブラリ内で共通利用されるメモリ管理に関するヘッダーファイル
  * @detail	このファイルはmanaライブラリ内で共通利用されるメモリ管理に関するヘッダーファイルです。
  * @author	Shun Moriya
  * @date	2003-
  */

#if !defined(___MANA_MALLOC_H___)
#define ___MANA_MALLOC_H___

#include <stddef.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! メモリブロックを確保する関数 */
typedef void* (*mana_malloc_function)(const size_t);

/*! メモリブロックを確保＋初期化する関数 */
typedef void* (*mana_calloc_function)(const size_t, const size_t);

/*! メモリブロックを再確保する関数 */
typedef void* (*mana_realloc_function)(void*, const size_t);

/*! メモリブロックを開放する関数 */
typedef void (*mana_free_function)(void*);

/*! メモリブロックの確保関数の登録 */
extern void mana_set_malloc_function(mana_malloc_function function);

/*! メモリブロックを確保後初期化関数の登録 */
extern void mana_set_calloc_function(mana_calloc_function function);

/*! メモリブロックの再確保関数の登録 */
extern void mana_set_realloc_function(mana_realloc_function function);

/*! メモリブロックの開放関数の登録 */
extern void mana_set_free_function(mana_free_function function);

/*! メモリブロックの確保 */
extern void* mana_malloc(const size_t size);

/*! メモリブロックを確保後初期化 */
extern void* mana_calloc(const size_t size, const size_t count);

/*! メモリブロックの再確保 */
extern void* mana_realloc(void* buffer, const size_t size);

/*! メモリブロックの開放 */
extern void mana_free(void* buffer);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
