/*!
 * mana (library)
 *
 * @file	mana_stream.h
 * @brief	mana_streamクラスに関するヘッダーファイル
 * @detail
 * このファイルはmana_streamクラスに関係するヘッダーファイルです。
 * mana_streamクラスはメモリストリーミングに関する操作を行ないます。
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_STREAM_H___)
#define ___MANA_STREAM_H___

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*!
 * @brief	mana_streamクラス
 *
 * mana_streamクラスはメモリストリーミングを表しています。
 *
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana_stream
{
	char* buffer;			/*!< バッファ */
	size_t allocated_size;	/*!< 確保済みサイズ */
	size_t used_size;		/*!< 使用済みサイズ */
	size_t pointer;			/*!< ポップ先オフセットアドレス */
}mana_stream;

/*! mana_stream オブジェクトの生成 */
extern mana_stream* mana_stream_create(void);

/*! mana_stream オブジェクトの破棄 */
extern void mana_stream_destroy(mana_stream* self);

/* ファイルからデータを読み込む */
int32_t mana_stream_load(mana_stream* self, const char* filename);

/* ファイルへデータを書き込む */
int32_t mana_stream_save(mana_stream* self, const char* filename);

/*! バッファの先頭アドレスを取得 */
extern void* mana_stream_get_buffer(const mana_stream* self);

/*! バッファのアドレスを取得 */
extern void* mana_stream_index_at(const mana_stream* self, const size_t index);

/*! 確保済みバッファサイズを取得 */
extern size_t mana_stream_get_allocated_size(const mana_stream* self);

/*! 使用しているバッファサイズを取得 */
extern size_t mana_stream_get_used_size(const mana_stream* self);

/*! int8_tサイズデータをプッシュ */
extern void mana_stream_push_char(mana_stream* self, const int8_t value);

/*! int16_tサイズデータをプッシュ */
extern void mana_stream_push_short(mana_stream* self, const int16_t value);

/*! int32_tサイズデータをプッシュ */
extern void mana_stream_push_integer(mana_stream* self, const int32_t value);

/*! uint8_tサイズデータをプッシュ */
extern void mana_stream_push_unsigned_char(mana_stream* self, const uint8_t value);

/*! uint16_tサイズデータをプッシュ */
extern void mana_stream_push_unsigned_short(mana_stream* self, const uint16_t value);

/*! uint32_tサイズデータをプッシュ */
extern void mana_stream_push_unsigned_integer(mana_stream* self, const uint32_t value);

/*! floatサイズデータをプッシュ */
extern void mana_stream_push_float(mana_stream* self, const float value);

/*! 文字列をプッシュ */
extern void mana_stream_push_string(mana_stream* self, const char* text);

/*! データをプッシュ */
extern void mana_stream_push_data(mana_stream* self, const void* pointer, const size_t size);

/*! popポインタを巻き戻す */
extern void mana_stream_rewind(mana_stream* self);

/*! int8_tサイズデータをポップ */
extern int8_t mana_stream_pop_char(mana_stream* self);

/*! int16_tサイズデータをポップ */
extern int16_t mana_stream_pop_short(mana_stream* self);

/*! int32_tサイズデータをポップ */
extern int32_t mana_stream_pop_integer(mana_stream* self);

/*! uint8_tサイズデータをポップ */
extern uint8_t mana_stream_pop_unsigned_char(mana_stream* self);

/*! uint16_tサイズデータをポップ */
extern uint16_t mana_stream_pop_unsigned_short(mana_stream* self);

/*! uint32_tサイズデータをポップ */
extern uint32_t mana_stream_pop_unsigned_integer(mana_stream* self);

/*! floatサイズデータをポップ */
extern float mana_stream_pop_float(mana_stream* self);

/*! 文字列をポップ */
extern void mana_stream_pop_string(mana_stream* self, char* pointer, const size_t size);

/*! データをポップ */
extern void mana_stream_pop_data(mana_stream* self, void* pointer, const size_t size);

/*! int8_tサイズデータを取得 */
extern int8_t mana_stream_get_char(const mana_stream* self);

/*! int16_tサイズデータを取得 */
extern int16_t mana_stream_get_short(const mana_stream* self);

/*! int32_tサイズデータを取得 */
extern int32_t mana_stream_get_integer(const mana_stream* self);

/*! uint8_tサイズデータを取得 */
extern uint8_t mana_stream_get_unsigned_char(const mana_stream* self);

/*! uint16_tサイズデータを取得 */
extern uint16_t mana_stream_get_unsigned_short(const mana_stream* self);

/*! uint32_tサイズデータを取得 */
extern uint32_t mana_stream_get_unsigned_integer(const mana_stream* self);

/*! floatサイズデータを取得 */
extern float mana_stream_get_float(const mana_stream* self);

/*! 文字列を取得 */
extern void mana_stream_get_string(const mana_stream* self, char* pointer, const size_t size);

/*! 文字列のポインターを取得 */
extern const char* mana_stream_get_string_pointer(const mana_stream* self);

/*! 文字列の長さを取得 */
extern size_t mana_stream_get_string_length(const mana_stream* self);

/*! データを取得 */
extern void mana_stream_get_data(const mana_stream* self, void* pointer, const size_t size);

/*! 参照ポインタを移動します */
extern void mana_steram_seek(mana_stream* self, const int32_t offset);

/*! 現在のポインターを記録します */
extern void mana_stream_mark(mana_stream* self);

/*! 現在のポインターが mana_stream_mark した位置か調べます */
extern void mana_stream_check(mana_stream* self);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
