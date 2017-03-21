/*!
 * mana (library)
 *
 * @file	mana_stack.h
 * @brief	mana_stackクラスに関するヘッダーファイル
 * @detail
 * このファイルはmana_stackクラスに関係するヘッダーファイルです。
 * mana_stackクラスはmana_actorクラスのスタック操作を行ないます。
 *
 * @author	Shun Moriya
 * @date	2003-
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
 * @brief	mana_stackクラス
 * mana_stackクラスはスタックを表しています。
 *
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana_stack
{
	/*! バッファ */
	union
	{
		const char** string_handle;	/*!< 文字列ハンドル */
		void** void_handle;			/*!< データハンドル */
		float* float_pointer;		/*!< 実数ポインタ */
		int32_t* integer_pointer;	/*!< 整数ポインタ */
		void* void_pointer;			/*!< データポインタ */
	}buffer;
	size_t allocated_size;			/*!< 確保済みサイズ */
	size_t used_size;				/*!< 使用済みサイズ(スタックポインタ) */
}mana_stack;

/*! mana_stack オブジェクトの生成 */
extern mana_stack* mana_stack_create(void);

/*! mana_stack オブジェクトの生成 */
extern mana_stack* mana_stack_create_with_size(const size_t size);

/*! mana_stack オブジェクトの破棄 */
extern void mana_stack_destroy(mana_stack* self);

/*! mana_stack オブジェクトのシリアライズ */
extern void mana_stack_serialize(const mana_stack* self, mana_stream* stream);

/*! mana_stack オブジェクトのデシリアライズ */
extern void mana_stack_deserialize(mana_stack* self, mana_stream* stream);

/*! mana_stack オブジェクトの初期化 */
extern void mana_stack_initialize(mana_stack* self);

/*! mana_stack オブジェクトの初期化 */
extern void mana_stack_initialize_with_size(mana_stack* self, const size_t size);

/*! mana_stack オブジェクトの開放 */
extern void mana_stack_finalize(mana_stack* self);

/*! mana_stack オブジェクトのクリア */
extern void mana_stack_clear(mana_stack* self);

/*! mana_stack オブジェクトのコピー */
extern void mana_stack_duplicate(mana_stack* self);

/*! スタックポインタを移動 */
extern void mana_stack_remove(mana_stack* self, const size_t size);

/*! 整数をプッシュ */
extern void mana_stack_push_integer(mana_stack* self, const int32_t value);

/*! 実数をプッシュ */
extern void mana_stack_push_real(mana_stack* self, const float value);

/*! 文字列をプッシュ */
extern void mana_stack_push_string(mana_stack* self, const char* string);

/*! ポインタをプッシュ */
extern void mana_stack_push_pointer(mana_stack* self, void* pointer);

/*! データをプッシュ */
extern void mana_stack_push_data(mana_stack* self, const void* buffer, const size_t size);

/*! 整数をポップ */
extern int32_t mana_stack_pop_integer(mana_stack* self);

/*! 実数をポップ */
extern float mana_stack_pop_real(mana_stack* self);

/*! 文字列をポップ */
extern const char* mana_stack_pop_string(mana_stack* self);

/*! ポインタをポップ */
extern void* mana_stack_pop_pointer(mana_stack* self);

/*! アドレスをポップ */
extern void* mana_stack_pop_address(mana_stack* self);

/*! データをポップ */
extern void mana_stack_pop_data(mana_stack* self, void* buffer, const size_t size);

/*! 整数を取得 */
extern int32_t mana_stack_get_integer(const mana_stack* self, const size_t index);

/*! 実数を取得 */
extern float mana_stack_get_real(const mana_stack* self, const size_t index);

/*! 文字列を取得 */
extern const char* mana_stack_get_string(const mana_stack* self, const size_t index);

/*! ポインタを取得 */
extern void* mana_stack_get_pointer(const mana_stack* self, const size_t index);

/*! アドレスを取得 */
extern void* mana_stack_get_address(const mana_stack* self, const size_t index);

/*! 整数を設定 */
extern void mana_stack_set_integer(mana_stack* self, const size_t index, const int32_t integer);

/*! 実数を設定 */
extern void mana_stack_set_real(mana_stack* self, const size_t index, const float real);

/*! 文字列を設定 */
extern void mana_stack_set_string(mana_stack* self, const size_t index, const char* string);

/*! ポインタを設定 */
extern void mana_stack_set_pointer(mana_stack* self, const size_t index, void* pointer);

/*! スタックのサイズを取得 */
extern size_t mana_stack_get_size(const mana_stack* self);

/*! スタックのサイズを設定 */
extern void mana_stack_set_size(mana_stack* self, const size_t size);

/*! スタックを比較 */
extern int32_t mana_stack_compare(const mana_stack* self, const mana_stack* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
