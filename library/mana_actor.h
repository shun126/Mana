/*!
 * mana (library)
 *
 * @file	mana_actor.h
 * @brief	mana_actorクラスに関するヘッダーファイル
 * @detail
 * このファイルはmana_actorクラスに関係するヘッダーファイルです。
 * mana_actorクラスは更新やリクエストなどの制御、外部関数の呼び出しなどの
 * 実行環境を表しています。
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_ACTOR_H___)
#define ___MANA_ACTOR_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_FRAME_H___)
#include "mana_frame.h"
#endif
#if !defined(___MANA_HASH_H___)
#include "mana_hash.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif
#if !defined(___MANA_STACK_H___)
#include "mana_stack.h"
#endif
#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

#define MANA_RETURN_VALUE_TYPE_INVALID	( 0)	/*!< 戻り値：無効 */
#define MANA_RETURN_VALUE_TYPE_ACTOR	(-1)	/*!< 戻り値：アクターへの参照 */
#define MANA_RETURN_VALUE_TYPE_INTEGER	(-2)	/*!< 戻り値：整数 */
#define MANA_RETURN_VALUE_TYPE_FLOAT	(-3)	/*!< 戻り値：小数 */
#define MANA_RETURN_VALUE_TYPE_STRING	(-4)	/*!< 戻り値：文字列への参照 */

/*! mana_actor_callback関数への命令タイプ */
typedef enum mana_file_callback_command
{
	MANA_FILE_COMMAND_CLOSE,					/*!< ファイルを閉じます */
	MANA_FILE_COMMAND_IS_READING				/*!< ファイルが読み込み中か取得します */
}mana_file_callback_command;

/*! ファイル制御コールバック */
typedef void (mana_actor_file_callback)(mana_file_callback_command, void* parameter);

/*! 割り込みテーブル */
typedef struct mana_interrupt
{
	struct mana_actor* sender;				/*!< 要求したmana_actor オブジェクト */
	uint32_t address;						/*!< 割り込み先アドレス */
	uint32_t return_address;				/*!< リターンアドレス */
	int32_t counter;								/*!< 汎用カウンタ */
	int32_t frame_pointer;							/*!< フレームポインタ */
	int32_t stack_pointer;							/*!< スタックポインタ */
	void* file_callback_parameter;			/*!< ファイル終了コールバックパラメータ */
#if !defined(NDEBUG)
	const char* action_name;					/*!< 実行中のアクション名 */
#endif
	uint8_t flag;
	/*
	 * mana_actor :: flags
	 *
	 *| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 *	|	|	|	|	|	|	|	|
	 *	|	|	|	|	|	|	|	+-- MANA_INTERRUPT_FLAG_INITIALIZED
	 *	|	|	|	|	|	|	+------ MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL
	 *	|	|	|	|	|	+---------- MANA_INTERRUPT_FLAG_REPEAT
	 *	|	|	|	|	+-------------- MANA_INTERRUPT_FLAG_SUSPEND
	 *	|	|	|	+------------------ MANA_INTERRUPT_FLAG_SYNCHRONIZED
	 *	|	|	+----------------------
	 *	|	+--------------------------
	 *	+------------------------------
	 */
}mana_interrupt;

#define MANA_INTERRUPT_FLAG_INITIALIZED		(0x01)/*!< 初期化済みフラグ */
#define MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL	(0x02)/*!< 同期実行フラグ */
#define MANA_INTERRUPT_FLAG_REPEAT			(0x04)/*!< リピートフラグ */
#define MANA_INTERRUPT_FLAG_SUSPEND			(0x08)/*!< サスペンドフラグ */
#define MANA_INTERRUPT_FLAG_SYNCHRONIZED	(0x10)/*!< 同期フラグ */

/*! 戻り値 */
typedef struct mana_return_value
{
	union
	{
		int32_t integer_value;						/*!< 整数値 */
		float float_value;						/*!< 実数値 */
		const char* string_value;				/*!< 文字列 */
		void* pointer_value;					/*!< 構造体 */
		struct mana_actor* actor_value;		/*!< アクター */
	} values;
	int32_t size;									/*!< サイズ(pointer_value) */
}mana_return_value;

/*!
 * @brief	mana_actorクラス
 * mana_actorクラスはアクターを表しています。
 * C++でいうところのメンバー変数ですが、C言語なので全てpublicになっています。
 * 操作用の関数を用意しているので、それらの関数を利用して操作してください。
 */
typedef struct mana_actor
{
#if !defined(NDEBUG)
	char* name;													/*!< アクター名 */
#endif
	struct mana* parent;										/*!< 所属するManaオブジェクトへのポインタ */
	mana_frame frame;											/*!< フレームメモリ */
	mana_stack stack;											/*!< スタック */
	mana_hash actions;											/*!< アクションHash */
	mana_interrupt interrupt[MANA_ACTOR_MAX_INTERRUPT_LEVEL];	/*!< 割り込みテーブル */
	mana_return_value return_value;								/*!< 戻り値 */
	void* request_callback_parameter;							/*!< リクエストコールバック */
	void* rollback_callback_parameter;							/*!< ロールバックコールバック */
	void* variable;												/*!< インスタンス変数 */
	size_t variable_size;										/*!< インスタンス変数のサイズ */
	uint32_t pc;											/*!< プログラムカウンタ */
	uint8_t interrupt_level;								/*!< 割り込みレベル */
	uint8_t flag;
	/*
	 * mana_actor :: flags
	 *
	 *| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 *	|	|	|	|	|	|	|	|
	 *	|	|	|	|	|	|	|	+-- MANA_ACTOR_FLAG_BATTLER
	 *	|	|	|	|	|	|	+------ MANA_ACTOR_FLAG_CHARACTER
	 *	|	|	|	|	|	+---------- MANA_ACTOR_FLAG_HALT
	 *	|	|	|	|	+-------------- MANA_ACTOR_FLAG_RUNNING
	 *	|	|	|	+------------------ MANA_ACTOR_FLAG_TOUCHED
	 *	|	|	+---------------------- MANA_ACTOR_FLAG_HIDED
	 *	|	+-------------------------- MANA_ACTOR_FLAG_REQUESTED
	 *	+------------------------------ MANA_ACTOR_FLAG_REFUSED
	 */
	uint16_t padding;
	uint32_t user_data;										/*!< ユーザーデータ */
	void* user_pointer;											/*!< ユーザーポインター */
}mana_actor;

#define MANA_ACTOR_FLAG_BATTLER		(0x01)		/*!< 戦闘参加フラグ */
#define MANA_ACTOR_FLAG_CHARACTER	(0x02)		/*!< キャラクターフラグ */
#define MANA_ACTOR_FLAG_HALT			(0x04)		/*!< 停止フラグ */
#define MANA_ACTOR_FLAG_RUNNING		(0x08)		/*!< 実行フラグ */
#define MANA_ACTOR_FLAG_TOUCHED		(0x10)		/*!< 接触フラグ */
#define MANA_ACTOR_FLAG_HIDED			(0x20)		/*!< 非表示フラグ */
#define MANA_ACTOR_FLAG_REQUESTED	(0x40)		/*!< リクエストフラグ */
#define MANA_ACTOR_FLAG_REFUSED		(0x80)		/*!< リクエスト禁止フラグ */

/*! request / rollbackコールバック */
typedef int32_t (mana_actor_callback)(mana_actor* actor, void* parameter);

/*! 外部関数宣言用型 */
typedef void (mana_external_funtion_type)(mana_actor* actor);

/*! mana_actor オブジェクトの生成 */
extern mana_actor* mana_actor_create(struct mana* parent, const size_t variable_size);

/*! mana_actor オブジェクトのコピー */
extern mana_actor* mana_actor_clone(mana_actor* actor);

/*! mana_actor オブジェクトの破棄 */
extern void mana_actor_destroy(mana_actor* self);

/*! mana_actor オブジェクトのシリアライズ */
extern void mana_actor_serialize(mana_actor* self, mana_stream* stream);

/*! mana_actor オブジェクトのデシリアライズ */
extern void mana_actor_deserialize(mana_actor* self, mana_stream* stream);

/*! 経過時間を取得 */
extern float mana_actor_get_delta_time(void);

/*! 経過時間を設定 */
extern void mana_actor_set_delta_time(float second);

/*! 更新 */
extern int32_t mana_actor_run(mana_actor* self);

/*! 同期実行 */
extern bool mana_sync_call(mana_actor* self, const int32_t level, const char* action, mana_actor* sender);

/*! 非同期実行 */
extern bool mana_async_call(mana_actor* self, const int32_t level, const char* action, mana_actor* sender);

/*! リクエスト */
extern bool mana_actor_request(mana_actor* self, const int32_t level, const char* name, mana_actor* sender);

/*! ロールバック */
extern void mana_actor_rollback(mana_actor* self, int32_t level);

/*! 再起動 */
extern void mana_actor_restart(mana_actor* self);

/*! 名前を取得 */
extern const char* mana_actor_get_name(mana_actor* self);

/*! アクションのプログラム開始アドレスを取得 */
extern uintptr_t mana_actor_get_action(mana_actor* self, const char* name);

/*! 汎用カウンターを取得 */
extern int32_t mana_actor_get_counter(mana_actor* self);

/*! 汎用カウンターを設定 */
extern void mana_actor_set_counter(mana_actor* self, const int32_t counter);

/*! 引数の数を取得 */
extern int32_t mana_actor_get_argument_count(mana_actor* self);

/*! 引数の数を取得 */
extern int32_t mana_actor_get_argument_count_by_address(mana_actor* self, const uint32_t address);

/*! 引数のサイズを取得 */
extern int32_t mana_actor_get_argument_size(mana_actor* self, const uint32_t address);

/*! 戻り値が存在するか調べる */
extern bool mana_actor_has_return_value(mana_actor* self, const uint32_t address);

/*! 整数型の引数を取得 */
extern int32_t mana_actor_get_parameter_integer(mana_actor* self, const int32_t value);

/*! 実数型の引数を取得 */
extern float mana_actor_get_parameter_float(mana_actor* self, const int32_t value);

/*! 文字列型の引数を取得 */
extern const char* mana_actor_get_parameter_string(mana_actor* self, const int32_t value);

/*! アクター型の引数を取得 */
extern mana_actor* mana_actor_get_parameter_actor(mana_actor* self, const int32_t value);

/*! ポインター型の引数を取得 */
extern void* mana_actor_get_parameter_pointer(mana_actor* self, const int32_t value);

/*! ポインター型の引数を取得 */
extern void* mana_actor_get_parameter_address(mana_actor* self, const int32_t value);

/*! 整数型の戻り値を設定 */
extern void mana_actor_set_return_integer(mana_actor* self, const int32_t value);

/*! 実数型の戻り値を設定 */
extern void mana_actor_set_return_float(mana_actor* self, const float value);

/*! 文字列型の戻り値を設定 */
extern void mana_actor_set_return_string(mana_actor* self, const char* string);

/*! アクター型の戻り値を設定 */
extern void mana_actor_set_return_actor(mana_actor* self, mana_actor* actor);

/*! ポインター型の戻り値を設定 */
extern void mana_actor_set_return_pointer(mana_actor* self, void* pointer);

/*! データの戻り値を設定 */
extern void mana_actor_set_return_data(mana_actor* self, const void* pointer, const int32_t size);

#if !defined(NDEBUG)
/*! 実行中のアクション名を取得 */
const char* mana_actor_get_action_name(const mana_actor* self);

/*! 実行中の外部関数名を取得 */
const char* mana_actor_get_function_name(const mana_actor* self);
#endif

/*! mana_actor オブジェクトにアクションを登録(protected) */
extern void mana_actor_set_action(mana_actor* self, const char* name, uint8_t* address);

/*! mana オブジェクトを取得します */
extern struct mana* mana_actor_get_parent(const mana_actor* self);

/*! 初期化フラグを取得 */
extern bool mana_actor_is_init(const mana_actor* self);

/*! リピートフラグを取得 */
extern bool mana_actor_is_repeat(const mana_actor* self);

/*! 実行フラグを取得 */
extern bool mana_actor_is_running(const mana_actor* self);

/*! 実行中の命令を中断して次のフレームに再実行します */
extern void mana_actor_repeat(mana_actor* self, const bool initial_complete);

/*! 実行中の命令を中断して同じフレーム中に再実行します */
extern void mana_actor_again(mana_actor* self);

/*! 実行停止 */
extern void mana_actor_halt(mana_actor* self);

/*! 実行停止 */
extern void mana_actor_stop(mana_actor* self);

/*! 次のフレームまで実行中断 */
extern void mana_actor_yield(mana_actor* self);

/*! リクエストを許可 */
extern void mana_actor_comply(mana_actor* self);

/*! リクエストを拒否 */
extern void mana_actor_refuse(mana_actor* self);

/*! 優先度を取得 */
extern int32_t mana_actor_get_interrupt_level(mana_actor* self);

/*! 同期フラグを取得 */
extern bool mana_actor_is_synchronized(mana_actor* self);

/*! 同期フラグを設定 */
extern void mana_actor_set_synchronized(mana_actor* self, bool synchronized);

/*! 優先度付き同期フラグの設定 */
extern void mana_actor_set_synchronized_with_level(mana_actor* self, int32_t level, bool synchronized);

/*! リソースデータの取得 */
extern void mana_actor_get_data(const mana_actor* self, const int32_t resouce_id, const void** buffer, size_t* size);

/*! ファイル読み込みコールバックの取得 */
extern mana_actor_file_callback* mana_actor_get_file_callback(void);

/*! ファイル読み込みコールバックの設定 */
extern void mana_actor_set_file_callback(mana_actor_file_callback* function);

/*! ファイル読み込みコールバックパラメーターの取得 */
extern void* mana_actor_get_file_callback_parameter(mana_actor* self, int32_t level);

/*! ファイル読み込みコールバックパラメーターの設定 */
extern void mana_actor_set_file_callback_parameter(mana_actor* self, int32_t level, void* parameter);

/*! リクエストコールバックの設定 */
extern void mana_actor_set_request_callback(mana_actor_callback* function);

/*! リクエストコールバックパラメーターの設定 */
extern void mana_actor_set_request_callback_parameter(mana_actor* self, void* parameter);

/*! ロールバックコールバックの設定 */
extern void mana_actor_set_rollback_callback(mana_actor_callback* function);

/*! ロールバックコールバックパラメーターの設定 */
extern void mana_actor_set_rollback_callback_parameter(mana_actor* self, void* parameter);

/*!< ユーザーデータの取得 */
extern uint32_t mana_actor_get_user_data(const mana_actor* self);

/*!< ユーザーデータの設定 */
extern void mana_actor_set_user_data(mana_actor* self, uint32_t user_data);

/*!< ユーザーポインターの取得 */
extern void* mana_actor_get_user_pointer(const mana_actor* self);

/*!< ユーザーポインターの設定 */
extern void mana_actor_set_user_pointer(mana_actor* self, void* user_pointer);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
