/*!
 * mana (library)
 *
 * @file	mana_array.h
 * @brief	mana_arrayクラスに関するヘッダーファイル
 * @detail	このファイルはmana_arrayクラスに関係するヘッダーファイルです。
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_ARRAY_H___)
#define ___MANA_ARRAY_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

/*!
 * @brief	クラス
 * mana_arrayクラスはフレームバッファを表しています。
 *
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana_array
{
	void** handle;			/*!< バッファ */
	size_t allocated_size;	/*!< 確保済みサイズ */
	size_t used_size;		/*!< 使用済みサイズ */
}mana_array;

/*! mana_array オブジェクトの生成 */
extern mana_array* mana_array_create(const size_t size);

/*! mana_array オブジェクトの破棄 */
extern void mana_array_destroy(mana_array* self);

/*! mana_array オブジェクトのシリアライズ */
extern void mana_array_serialize(const mana_array* self, mana_stream* stream);

/*! mana_array オブジェクトのデシリアライズ */
extern void mana_array_deserialize(mana_array* self, mana_stream* stream);

/*! mana_array オブジェクトの初期化 */
extern void mana_array_initialize(mana_array* self, const size_t size);

/*! mana_array オブジェクトの開放 */
extern void mana_array_finalize(mana_array* self);

/*! 配列をクリア */
extern void mana_array_clear(mana_array* self);

/*! 配列を参照 */
extern void* mana_array_get(const mana_array* self, const size_t index);

/*! 配列にポインターを設定 */
extern void mana_array_set(mana_array* self, const size_t index, const void* pointer);

/*! 配列のサイズを取得 */
extern size_t mana_array_get_size(const mana_array* self);

/*! 配列にポインターを追加 */
extern void mana_array_append(mana_array* self, const void* pointer);

/*! 配列のサイズを変更 */
extern void mana_array_resize(mana_array* self, const size_t size);

/*! 配列を比較 */
extern int32_t mana_array_compare(const mana_array* self, const mana_array* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
