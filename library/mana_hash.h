/*!
 * mana (compiler/library)
 *
 * @file	mana_hash.h
 * @brief	ハッシュに関するヘッダーファイル
 * @detail
 * このファイルはmana_hashクラスに関係するヘッダーファイルです。
 * mana_hashクラスは文字列をキーにした連想配列に関数操作を行ないます。
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_HASH_H___)
#define ___MANA_HASH_H___

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MANA_HASH_TABLE_SIZE	(31)	/*!< ハッシュテーブルサイズ(素数が好ましい) */

/*! mana_hash_table の双方向リンク */
typedef struct mana_hash_table_link
{
	struct mana_hash_table_link* previousChain;	/*!< 前の mana_hash_table へのポインタ */
	struct mana_hash_table_link* nextChain;		/*!< 次の mana_hash_table へのポインタ */
}mana_hash_table_link;

/*! mana_hashクラスのハッシュテーブル */
typedef struct mana_hash_table
{
	mana_hash_table_link chain;		/*!< mana_hash_table への双方向リンク */
	struct mana_hash_table* next;	/*!< 次のハッシュテーブルへのポインタ */
	const char* name;				/*!< キー */
	void* data;						/*!< 値 */
} mana_hash_table;

/*!
 * @brief	mana_hashクラス
 * mana_hashクラスは文字列をキーにした連想配列を表しています。
 *
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana_hash
{
	/*! ハッシュチェーン(反復用キャッシュ) */
	mana_hash_table_link chain;
	/*! ハッシュチェーン(最後のアクセス) */
	mana_hash_table* last;
	/*! ハッシュテーブル */
	mana_hash_table* table[MANA_HASH_TABLE_SIZE];
}mana_hash;

/*! mana_hashクラスの反復子 */
typedef struct mana_hash_iterator
{
	mana_hash_table* iterator;			/*!< 参照中のmana_hash_table */
}mana_hash_iterator;

/*! mana_hash オブジェクトの生成 */
extern mana_hash* mana_hash_create(void);

/*! mana_hash オブジェクトの破棄 */
extern void mana_hash_destroy(mana_hash* self);

/*! mana_hash オブジェクトの初期化 */
extern void mana_hash_initialize(mana_hash* self);

/*! mana_hash オブジェクトの開放 */
extern void mana_hash_finalize(mana_hash* self);

/*! mana_hash オブジェクトの比較 */
extern int32_t mana_hash_compare(const mana_hash* self, const mana_hash* other);

/*! mana_hash オブジェクトのコピー */
extern void mana_hash_duplicate(mana_hash* self, const mana_hash* source);

/*! mana_hash オブジェクトのクリア */
extern void mana_hash_clear(mana_hash* self);

/*! データを消去 */
extern void mana_hash_erase(mana_hash* self, const char* name);

/*! データの存在テスト */
extern int32_t mana_hash_test(mana_hash* self, const char* name);

/*! データを取得 */
extern void* mana_hash_get(mana_hash* self, const char* name);

/*! データを設定 */
extern void* mana_hash_set(mana_hash* self, const char* name, void* data);

/*! イテレータ（反復子）の巻き戻し */
extern void mana_hash_iterator_rewind(mana_hash_iterator* iterator);

/*! イテレータ（反復子）を次へ移動 */
extern int32_t mana_hash_iterator_hash_next(const mana_hash* self, mana_hash_iterator* iterator);

/*! イテレータ（反復子）が指すハッシュのキーを取得 */
extern const char* mana_hash_iterator_get_name(const mana_hash_iterator* iterator);

/*! イテレータ（反復子）が指すハッシュの値を取得 */
extern void* mana_hash_iterator_get_value(const mana_hash_iterator* iterator);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
