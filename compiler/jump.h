/*!
mana (compiler)

@file	jump.h
@brief	break,continue,gotoなどのジャンプに関するヘッダーファイル
@detail	このファイルはbreak,continue,gotoなどジャンプに関係するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_JUMP_H___)
#define ___MANA_JUMP_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___NODE_H___)
#include "node.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif

/*! ジャンプチェインの状態 */
typedef enum mana_jump_chain_status
{
	MANA_JUMP_CHAIN_STATE_SWITCH,	/*!< switchブロック */
	MANA_JUMP_CHAIN_STATE_WHILE,	/*!< whileブロック */
	MANA_JUMP_CHAIN_STATE_DO,		/*!< doブロック */
	MANA_JUMP_CHAIN_STATE_FOR,		/*!< forブロック */
	MANA_JUMP_CHAIN_STATE_LOOP,		/*!< loopブロック */
	MANA_JUMP_CHAIN_STATE_LOCK,		/*!< lockブロック */
} mana_jump_chain_status;

/*! ジャンプ管理の初期化 */
extern void mana_jump_initialize(void);

/*! ジャンプ管理の終了処理 */
extern void mana_jump_finalize(void);

/*! ジャンプブロックの開始 */
extern void mana_jump_open_chain(mana_jump_chain_status);

/*! breakの設定 */
extern int32_t mana_jump_break(int32_t);

/*! continueの設定 */
extern int32_t mana_jump_continue(int32_t);

/*! continueジャンプブロックの終了 */
extern void mana_jump_close_continue_only(void);

/*! ジャンプブロックの終了 */
extern void mana_jump_close_chain(void);

/*! switchブロックの開始 */
extern void mana_jump_open_switch(type_description*);

/*! caseの登録 */
extern void mana_jump_switch_case(node_entry*);

/*! defaultの登録 */
extern void mana_jump_switch_default(void);

/*! switchブロックをバイナリーコードに変換 */
extern void mana_jump_switch_build(void);

/*! switchブロックの終了 */
extern void mana_jump_close_switch(void);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
