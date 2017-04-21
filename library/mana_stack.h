/*!
mana (library)

@file	mana_stack.h
@brief	mana_stackクラスに関するヘッダーファイル
@detail
このファイルはmana_stackクラスに関係するヘッダーファイルです。
mana_stackクラスはmana_actorクラスのスタック操作を行ないます。

@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_STACK_H___)
#define ___MANA_STACK_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

/*!
@brief	mana_stackクラス
mana_stackクラスはスタックを表しています。

C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
操作用の関数を用意しているので、それらの関数を利用して操作してください。
*/
typedef struct mana_stack
{
	//! バッファ
	union
	{
		const char** string_handle;	//!< 文字列ハンドル
		void** void_handle;			//!< データハンドル
#if UINTPTR_MAX == UINT64_MAX
		double* float_pointer;		//!< 実数ポインタ
		int64_t* integer_pointer;	//!< 整数ポインタ
#elif UINTPTR_MAX == UINT32_MAX
		float* float_pointer;		//!< 実数ポインタ
		int32_t* integer_pointer;	//!< 整数ポインタ
#else
#error "unsupport pointer size"
#endif
		void* void_pointer;			//!< データポインタ
	}buffer;
	size_t allocated_size;			//!< 確保済みサイズ
	size_t used_size;				//!< 使用済みサイズ(スタックポインタ)
}mana_stack;

/*!
mana_stack オブジェクトの生成
@return		mana_stack オブジェクト
@warning	void mana_stack_initialize(mana_stack* self)を呼ぶ必要はありません。
*/
extern mana_stack* mana_stack_create(void);

/*!
mana_stack オブジェクトの生成
@param[in]	size	確保サイズ
@return		mana_stack オブジェクト
@warning	void mana_stack_initialize_with_size(mana_stack* self, size_t size)を呼ぶ必要はありません。
*/
extern mana_stack* mana_stack_create_with_size(const size_t size);

/*!
mana_stack オブジェクトの破棄
@param[in]	self	mana_stack オブジェクト
@warning	void mana_stack_finalize(mana_stack* self)を呼ぶ必要はありません。
*/
extern void mana_stack_destroy(mana_stack* self);

/*!
mana_stack オブジェクトの初期化
@param[in]	self	mana_stack オブジェクト
*/
extern void mana_stack_initialize(mana_stack* self);

/*!
mana_stack オブジェクトの初期化
@param[in]	self	mana_stack オブジェクト
@param[in]	size	確保サイズ
*/
extern void mana_stack_initialize_with_size(mana_stack* self, const size_t size);

/*!
mana_stack オブジェクトの開放
@param[in]	self	mana_stack オブジェクト
*/
extern void mana_stack_finalize(mana_stack* self);

/*!
mana_stack オブジェクトのシリアライズ
@param[in]	self	mana_stack オブジェクト
@param[out]	stream	mana_stream オブジェクト
*/
extern void mana_stack_serialize(const mana_stack* self, mana_stream* stream);

/*!
mana_stack オブジェクトのデシリアライズ
@param[in]	self	mana_stack オブジェクト
@param[in]	stream	mana_stream オブジェクト
*/
extern void mana_stack_deserialize(mana_stack* self, mana_stream* stream);

/*!
mana_stack オブジェクトのクリア
@param[in]	self	mana_stack オブジェクト
*/
extern void mana_stack_clear(mana_stack* self);

/*!
mana_stack オブジェクトのコピー
@param[in]	self	mana_stack オブジェクト
*/
extern void mana_stack_duplicate(mana_stack* self);

/*!
スタックポインタを移動
@param[in]	self	mana_stack オブジェクト
@param[in]	size	削除するサイズ
*/
extern void mana_stack_remove(mana_stack* self, const size_t size);

/*!
整数をプッシュ
@param[in]	self	mana_stack オブジェクト
@param[in]	value	プッシュする値
*/
extern void mana_stack_push_integer(mana_stack* self, const int32_t value);

/*!
実数をプッシュ
@param[in]	self	mana_stack オブジェクト
@param[in]	value	プッシュする値
*/
extern void mana_stack_push_real(mana_stack* self, const float value);

/*!
文字列をプッシュ
@param[in]	self	mana_stack オブジェクト
@param[in]	string	プッシュする文字列
*/
extern void mana_stack_push_string(mana_stack* self, const char* string);

/*!
ポインタをプッシュ
@param[in]	self	mana_stack オブジェクト
@param[in]	pointer	プッシュする値
*/
extern void mana_stack_push_pointer(mana_stack* self, void* pointer);

/*!
データをプッシュ
@param[in]	self	mana_stack オブジェクト
@param[in]	buffer	プッシュするデータの先頭アドレス
@param[in]	size	プッシュするデータのサイズ
*/
extern void mana_stack_push_data(mana_stack* self, const void* buffer, const size_t size);

/*!
整数をポップ
@param[in]	self	mana_stack オブジェクト
@return		ポップした値
*/
extern int32_t mana_stack_pop_integer(mana_stack* self);

/*!
実数をポップ
@param[in]	self	mana_stack オブジェクト
@return		ポップした値
*/
extern float mana_stack_pop_real(mana_stack* self);

/*!
文字列をポップ
@param[in]	self	mana_stack オブジェクト
@return		ポップした値
*/
extern const char* mana_stack_pop_string(mana_stack* self);

/*!
ポインタをポップ
スタックに格納されたポインタをポップします。
void* mana_stack_pop_address(mana_stack* self)はスタックのアドレスをポップする点が違います。
@param[in]	self	mana_stack オブジェクト
@return		ポップした値
*/
extern void* mana_stack_pop_pointer(mana_stack* self);

/*!
アドレスをポップ
スタックのアドレスをポップします。
void* mana_stack_pop_pointer(mana_stack* self)はスタックに格納されたポインタをポップする点が違います。
@param[in]	self	mana_stack オブジェクト
@return		ポップした値
*/
extern void* mana_stack_pop_address(mana_stack* self);

/*!
データをポップ
@param[in]	self	mana_stack オブジェクト
@param[out]	buffer	ポップしたデータの先頭アドレス
@param[out]	size	ポップしたデータのサイズ
*/
extern void mana_stack_pop_data(mana_stack* self, void* buffer, const size_t size);

/*!
整数を取得
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@return		値
*/
extern int32_t mana_stack_get_integer(const mana_stack* self, const size_t index);

/*!
実数を取得
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@return		値
*/
extern float mana_stack_get_real(const mana_stack* self, const size_t index);

/*!
文字列を取得
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@return		文字列
*/
extern const char* mana_stack_get_string(const mana_stack* self, const size_t index);

/*!
ポインタを取得
スタックに格納されたポインタを取得します。
void* mana_stack_get_address(mana_stack* self, size_t index)はスタックのアドレスを取得する点が違います。
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@return		値
*/
extern void* mana_stack_get_pointer(const mana_stack* self, const size_t index);

/*!
アドレスを取得
スタックのアドレスを取得します。
void* mana_stack_get_pointer(mana_stack* self, size_t index)はスタックに格納されたポインタを取得する点が違います。
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@return		アドレス
*/
extern void* mana_stack_get_address(const mana_stack* self, const size_t index);

/*!
整数を設定
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@param[in]	integer	値
*/
extern void mana_stack_set_integer(mana_stack* self, const size_t index, const int32_t integer);

/*!
実数を設定
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@param[in]	real	値
*/
extern void mana_stack_set_real(mana_stack* self, const size_t index, const float real);

/*!
文字列を設定
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@param[in]	string	文字列
*/
extern void mana_stack_set_string(mana_stack* self, const size_t index, const char* string);

/*!
ポインタを設定
@param[in]	self	mana_stack オブジェクト
@param[in]	index	スタックポインタへのオフセット値
@param[in]	pointer	ポインタ
*/
extern void mana_stack_set_pointer(mana_stack* self, const size_t index, void* pointer);

/*!
スタックのサイズを取得
@param[in]	self	mana_stack オブジェクト
@return		スタックのサイズ/スタックポインタ
*/
extern size_t mana_stack_get_size(const mana_stack* self);

/*!
スタックのサイズを設定
@param[in]	self	mana_stack オブジェクト
@param[in]	size	スタックのサイズ/スタックポインタ
*/
extern void mana_stack_set_size(mana_stack* self, const size_t size);

/*!
スタックを比較
@param[in]	self	mana_frame オブジェクト
@param[in]	other	mana_frame オブジェクト
@retval		== 0	同一の内容
@retval		!= 0	異なる内容
*/
extern int32_t mana_stack_compare(const mana_stack* self, const mana_stack* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
