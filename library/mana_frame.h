/*!
mana (library)

@file	mana_frame.h
@brief	mana_frameクラスに関するヘッダーファイル
@detail
このファイルはmana_frameクラスに関係するヘッダーファイルです。
mana_frameクラスはmana_actorクラスのローカル変数の操作を行ないます。
本来はスタックで操作したほうが良いのかもしれませんが…

@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_FRAME_H___)
#define ___MANA_FRAME_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

/*!
@brief	mana_frameクラス
mana_frameクラスはフレームバッファを表しています。

C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
操作用の関数を用意しているので、それらの関数を利用して操作してください。
*/
typedef struct mana_frame
{
	uint8_t* buffer;		//!< バッファ
	size_t allocated_size;	//!< 確保済みサイズ
	size_t used_size;		//!< 使用済みサイズ
}mana_frame;

/*!
mana_frame オブジェクトの生成
@return	mana_frame オブジェクト
@warning	void mana_frame_initialize(mana_frame* self)を呼ぶ必要はありません。
*/
extern mana_frame* mana_frame_create(void);

/*!
mana_frame オブジェクトの生成
@param[in]	size	確保サイズ
@return		mana_frame オブジェクト
@warning	void mana_frame_initialize_with_size(mana_frame* self, size_t size)を呼ぶ必要はありません。
*/
extern mana_frame* mana_frame_create_with_size(const size_t size);

/*!
mana_frame オブジェクトの破棄
@param[in]	self	mana_frame オブジェクト
@warning	void mana_frame_finalize(mana_frame* self)を呼ぶ必要はありません。
*/
extern void mana_frame_destroy(mana_frame* self);

/*!
mana_frame オブジェクトの初期化
@param[in]	self	mana_frame オブジェクト
*/
extern void mana_frame_initialize(mana_frame* self);

/*!
mana_frame オブジェクトの初期化
@param[in]	self	mana_frame オブジェクト
@param[in]	size	確保サイズ
*/
extern void mana_frame_initialize_with_size(mana_frame* self, const size_t size);

/*!
mana_frame オブジェクトのシリアライズ
@param[in]	self	mana_frame オブジェクト
@param[out]	stream	mana_stream オブジェクト
*/
extern void mana_frame_serialize(const mana_frame* self, mana_stream* stream);

/*!
mana_frame オブジェクトのデシリアライズ
@param[in]	self	mana_frame オブジェクト
@param[in]	stream	mana_stream オブジェクト
*/
extern void mana_frame_deserialize(mana_frame* self, mana_stream* stream);

/*!
mana_frame オブジェクトの開放
@param[in]	self	mana_frame オブジェクト
*/
extern void mana_frame_finalize(mana_frame* self);

/*!
mana_frame オブジェクトのクリア
@param[in]	self	mana_frame オブジェクト
*/
extern void mana_frame_clear(mana_frame* self);

/*!
フレームバッファの確保
@param[in]	self	mana_frame オブジェクト
@param[in]	size	追加サイズ
*/
extern void mana_frame_allocate(mana_frame* self, const size_t size);

/*!
フレームバッファの開放
@param[in]	self	mana_frame オブジェクト
@param[in]	size	開放サイズ
*/
extern void mana_frame_release(mana_frame* self, const size_t size);

/*!
フレームバッファのアドレスを取得
@param[in]	self	mana_frame オブジェクト
@param[in]	index	フレームポインタへのオフセット値
@return		アドレス
*/
extern void* mana_frame_get_address(const mana_frame* self, const size_t index);

/*!
フレームバッファのサイズを取得
@param[in]	self	mana_frame オブジェクト
@return		フレームバッファのサイズ/フレームポインタ
*/
extern size_t mana_frame_get_size(const mana_frame* self);

/*!
フレームバッファのサイズを設定
@param[in]	self	mana_frame オブジェクト
@param[in]	size	フレームバッファのサイズ/フレームポインタ
*/
extern void mana_frame_set_size(mana_frame* self, const size_t size);

/*!
フレームバッファを比較
@param[in]	self	mana_frame オブジェクト
@param[in]	other	mana_frame オブジェクト
@retval		== 0	同一の内容
@retval		!= 0	異なる内容
*/
extern int32_t mana_frame_compare(const mana_frame* self, const mana_frame* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
