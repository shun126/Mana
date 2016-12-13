/*!
 * mana (compiler/library)
 *
 * @file	mana_datalink.h
 * @brief	mana_datalinkクラスに関するヘッダーファイル
 * @detail
 * このファイルはmana_datalinkクラスに関係するヘッダーファイルです。
 * manaコンパイラはスクリプトでalias指定され、かつ参照されているリソースを
 * 結合します。
 * mana_datalinkクラスは結合されたデータを参照する機能を持っています。
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_DATALINK_H___)
#define ___MANA_DATALINK_H___

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MANA_DATALINK_STANDARD_ALIGNMENT_SIZE	(sizeof(void*))

/*! datalinkファイルのヘッダー */
typedef struct mana_datalink_file_header
{
	uint32_t total_data_size;	/*!< 全体サイズ */
	int32_t number_of_datas;			/*!< データの個数 */
} mana_datalink_file_header;

/*! datalinkファイル内のデータの情報 */
typedef struct mana_datalink_info_header
{
	uint32_t offset;			/*!< 先頭からのオフセット */
	uint32_t size;				/*!< データのサイズ */
} mana_datalink_info_header;

/*! mana_datalink */
/*!
 * @brief	mana_datalinkクラス
 * mana_datalinkクラスは結合されたデータを参照します。
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana_datalink
{
	/*! datalinkファイルのヘッダー */
	mana_datalink_info_header* datalink_info_header_pointer;
	uint8_t* data_pointer;	/*!< データセクション先頭アドレス */
	int32_t number_of_entries;			/*!< データの個数 */
} mana_datalink;

/*! mana_datalink オブジェクトの生成 */
extern mana_datalink* mana_datalink_create(void);

/*! mana_datalink オブジェクトの破棄 */
extern void mana_datalink_destroy(mana_datalink* self);

/*! mana_datalink オブジェクトの初期化 */
extern void mana_datalink_initialize(mana_datalink* self);

/*! mana_datalink オブジェクトの終了化 */
extern void mana_datalink_finalize(mana_datalink* self);

/*! datalinkファイルの読み込み */
extern void mana_datalink_load(mana_datalink* self, const void* buffer);

/*! datalinkファイルの開放 */
extern void mana_datalink_release(mana_datalink* self);

/*! datalinkファイル内のデータ数を取得 */
extern int32_t mana_datalink_get_number_of_datas(const mana_datalink* self);

/*! データのサイズを取得 */
extern int32_t mana_datalink_get_data_size(const mana_datalink* self, const int32_t index);

/*! データのアドレスを取得 */
extern const void* mana_datalink_get_data(const mana_datalink* self, const int32_t index);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
