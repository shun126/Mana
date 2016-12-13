/*!
 * mana (library)
 *
 * @file	mana.h
 * @brief	manaクラスに関するヘッダーファイル
 * @detail
 * このファイルはmanaクラスに関係するヘッダーファイルです。
 * manaクラスは更新やリクエストなどの制御、mana_actor オブジェクトの検索などの
 * 実行環境を表しています。
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_H___)
#define ___MANA_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_DEFINE_H___)
#include "mana_define.h"
#endif
#if !defined(___MANA_ACTOR_H___)
#include "mana_actor.h"
#endif
#if !defined(___MANA_ARRAY_H___)
#include "mana_array.h"
#endif
#if !defined(___MANA_DATALINK_H___)
#include "mana_datalink.h"
#endif
#if !defined(___MANA_HASH_H___)
#include "mana_hash.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif
#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*!
 * @brief	manaクラス
 * manaクラスは実行環境を表しています。
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana
{
#if 0
	struct mana_actor* map_actor;		/*!< マップを表すmana_actor オブジェクト */
	struct mana_actor* player_actor;	/*!< プレイヤーを表すmana_actor オブジェクト */
#endif
	mana_array actor_array;				/*!< mana_actor オブジェクトへの配列 */
	mana_hash actor_hash;				/*!< mana_actor オブジェクトへの連想配列 */
	mana_hash phantom_hash;				/*!< phantomを表すmana_actor オブジェクトへの連想配列 */
	mana_datalink datalink;				/*!< mana_datalink オブジェクト */
	uint8_t* global_memory;		/*!< グローバル変数格納エリア */
	mana_file_header* file_header;		/*!< ロードされたファイルへのポインタ */
	uint8_t* instruction_pool;	/*!< ロードされたプログラムへのポインタ */
	char* constant_pool;				/*!< ロードされたデータへのポインタ */
	uint32_t frame_counter;			/*!< フレームカウンタ */

/*
	CManaIntersectionManager m_IntersectionManager;
	CManaResource m_Resource;
*/

	uint8_t flag;					/*!< フラグ */
	/*
	 * mana :: flags
	 *
	 *| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 *	|	|	|	|	|	|	|	|
	 *	|	|	|	|	|	|	|	+-- MANA_FLAG_AUTO_RELEASE
	 *	|	|	|	|	|	|	+------ MANA_FLAG_DEBUG_MODE
	 *	|	|	|	|	|	+---------- MANA_FLAG_FRAME_CHANGED
	 *	|	|	|	|	+-------------- MANA_FLAG_INIT_ACTION_RUNNING
	 *	|	|	|	+------------------ MANA_FLAG_INIT_ACTION_FINISHED
	 *	|	|	+---------------------- MANA_FLAG_INITIALIZED
	 *	|	+-------------------------- MANA_FLAG_REQUESTED
	 *	+------------------------------ MANA_FLAG_SYSTEM_REQUEST
	 */
}mana;

#define MANA_FLAG_AUTO_RELEASE			(0x01)	/*!< プログラムの自動開放フラグ */
#define MANA_FLAG_DEBUG_MODE			(0x02)	/*!< デバックモードフラグ */
#define MANA_FLAG_FRAME_CHANGED			(0x04)	/*!< フレーム変更フラグ */
#define MANA_FLAG_INIT_ACTION_RUNNING	(0x08)	/*!< initアクション実行中フラグ */
#define MANA_FLAG_INIT_ACTION_FINISHED	(0x10)	/*!< initアクション終了フラグ */
#define MANA_FLAG_INITIALIZED			(0x20)	/*!< 初期化済みフラグ */
#define MANA_FLAG_REQUESTED				(0x40)	/*!< リクエスト発生済みフラグ */
#define MANA_FLAG_SYSTEM_REQUEST		(0x80)	/*!< システムリクエスト許可フラグ */

/*! mana オブジェクトの生成 */
extern mana* mana_create(void);

/*! mana オブジェクトの破棄 */
extern void mana_destroy(mana* self);

/*! mana オブジェクトのシリアライズ */
extern void mana_serialize(mana* self, mana_stream* stream);

/*! mana オブジェクトのデシリアライズ */
extern void mana_deserialize(mana* self, mana_stream* stream);

/*! manaプログラムの読み込み */
extern bool mana_load_program(mana* self, void* program, int32_t auto_release);

/*! manaプログラムの開放 */
extern void mana_unload_program(mana* self);

/*! manaプログラムアドレスの取得 */
extern void* mana_get_program_buffer(mana* self);

/*! 再起動 */
extern void mana_restart(mana* self);

/*! 同期更新 */
extern bool mana_run(mana* self, const float second);

/*! 非同期更新 */
extern void mana_execute(mana* self);

/*! 実行中か調べる */
extern bool mana_is_running(mana* self);

/*! 全アクターにリクエストを要求 */
extern void mana_request_all(mana* self, int32_t level, const char* action_name, mana_actor* sender);

/*! アクターにリクエストを要求 */
extern bool mana_request(mana* self, int32_t level, const char* actor_name, const char* action_name, mana_actor* sender);

/*! 処理を譲る */
extern void mana_yield(mana* self);

extern int8_t mana_get_char(const mana* self, const uint8_t* address);
extern int16_t mana_get_short(const mana* self, const uint8_t* address);
extern int32_t mana_get_integer(const mana* self, const uint8_t* address);
extern uint8_t mana_get_unsigned_char(const mana* self, const uint8_t* address);
extern uint16_t mana_get_unsigned_short(const mana* self, const uint8_t* address);
extern uint32_t mana_get_unsigned_integer(const mana* self, const uint8_t* address);
extern float mana_get_float(const mana* self, const uint8_t* address);
extern const char* mana_get_string(const mana* self, const uint8_t* address);
extern uint8_t* mana_get_address(const mana* self, const uint8_t* address);

/*! アクターオブジェクトを取得 */
extern struct mana_actor* mana_get_actor(mana* self, const char* name);

/*! アクターの名前を取得 */
extern const char* mana_get_actor_name(mana* self, struct mana_actor* actor);

/*! アクターのコピー */
extern struct mana_actor* mana_create_actor(mana* self, struct mana_actor* actor, const char* new_name);

/*! アクターのコピー */
extern struct mana_actor* mana_create_actor_name(mana* self, const char* name, const char* new_name);

/*! phantomを元にアクターを生成 */
extern struct mana_actor* mana_create_actor_from_phantom(mana* self, const char* name, const char* new_name);

/*! initアクション実行中か取得 */
extern bool mana_is_in_init_action(mana* self);

/*! initアクション終了か取得 */
extern bool mana_is_finish_init_action(mana* self);

/*! システムリクエストの許可フラグを設定 */
extern void mana_enable_system_request(mana* self, const bool enable);

/*! システムリクエストの許可フラグを取得 */
extern bool mana_is_system_request_enabled(mana* self);

/*! フレームカウンターを取得 */
extern uint32_t mana_get_frame_counter(mana* self);

/*! フレームが変更されたか取得 */
extern bool mana_is_frame_changed(mana* self);

/*! リソースデータを取得 */
extern void mana_get_data(const mana* self, const int32_t resouce_id, const void** buffer, size_t* size);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
