/*!
 * mana (library)
 *
 * @file	mana_buffer.h
 * @brief	mana_bufferクラスに関するヘッダーファイル
 * @detail
 * このファイルはmana_bufferクラスに関係するヘッダーファイルです。
 * mana_streamクラスは可変長バッファに関する操作を行ないます。
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_BUFFER_H___)
#define ___MANA_BUFFER_H___

#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*!
 * @brief	mana_bufferクラス
 *
 * mana_bufferクラスは可変長バッファを表しています。
 *
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana_buffer
{
	char* buffer;
	size_t allocated_size;
	size_t used_size;
}mana_buffer;

/*! mana_stream オブジェクトの生成 */
extern mana_buffer* mana_buffer_create(void);

/*! mana_stream オブジェクトの破棄 */
extern void mana_buffer_destroy(mana_buffer* self);

/*! バッファの先頭アドレスを取得 */
extern void* mana_buffer_get_buffer(mana_buffer* self);

/*! 確保済みバッファサイズを取得 */
extern size_t mana_buffer_get_allocated_size(mana_buffer* self);

/*! 使用しているバッファサイズを取得 */
extern size_t mana_buffer_get_used_size(mana_buffer* self);

/*! データをプッシュ */
extern int32_t mana_buffer_push(mana_buffer* self, const void* pointer, const size_t size);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
