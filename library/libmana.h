/*!
 * mana (library)
 *
 * @file	libmana.h
 * @brief	システム全体に関するヘッダファイル
 * @detail	このファイルはライブラリ全体に関係するヘッダファイルです。
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___LIBMANA_H___)
#define ___LIBMANA_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_DEFINE_H___)
#include "mana_define.h"
#endif
#if !defined(___MANA_H___)
#include "mana.h"
#endif
#if !defined(___MANA_ACTOR_H___)
#include "mana_actor.h"
#endif
#if !defined(___MANA_ARRAY_H___)
#include "mana_array.h"
#endif
#if !defined(___MANA_BUFFER_H___)
#include "mana_buffer.h"
#endif
#if !defined(___MANA_DATALINK_H___)
#include "mana_datalink.h"
#endif
#if !defined(___MANA_FRAME_H___)
#include "mana_frame.h"
#endif
#if !defined(___MANA_FUNCTION_H___)
#include "mana_function.h"
#endif
#if !defined(___MANA_HASH_H___)
#include "mana_hash.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif
#if !defined(___MANA_LZSS_H___)
#include "mana_lzss.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#if !defined(___MANA_PLUGIN_H___)
#include "mana_plugin.h"
#endif
#if !defined(___MANA_STACK_H___)
#include "mana_stack.h"
#endif
#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

/*! ユニークな名前の最大サイズ */
#define MANA_UNIQUE_NAME_LENGTH	(16)

/*! 外部関数登録マクロ */
#define MANA_REGIST_FUNCTION(a)	mana_regist_function((const char*)#a, a)

/*! ライブラリの初期化 */
extern void mana_initialize(void);

/*! ライブラリの終了 */
extern void mana_finalize(void);

/*! 外部関数の登録 */
extern bool mana_regist_function(const char* name, mana_external_funtion_type* function);

/*! 外部関数の登録解除 */
extern void mana_unregist_function(const char* name);

/*! デバックモードの設定 */
extern void mana_set_debug_mode(const bool enable);

/*! デバックモードの取得 */
extern bool mana_is_debug_mode(void);

/*! 角度調整 */
extern float mana_angle_mod(const float x, const float div);

/*! スタティック変数領域を確保します。最初の確保は内容を０で初期化します。 */
extern void mana_reallocate_static_variables(const size_t allocate_size);

/*! スタティック変数領域のシリアライズ */
extern void mana_serialize_static_variables(mana_stream* stream);

/*! スタティック変数領域のデシリアイズ */
extern void mana_deserialize_static_variables(mana_stream* stream);

/*! スタティック変数領域のアドレスの取得 */
extern uint8_t* mana_get_static_variables(void);

/*! スタティック変数領域のサイズの取得 */
extern size_t mana_get_static_variables_size(void);

/*! ユニークな名前を生成 */
extern void mana_generate_unique_name(char* name, const size_t size);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
