/*!
 * mana (library)
 *
 * @file	mana_define.h
 * @brief	manaライブラリ内で共通利用されるヘッダーファイル
 * @detail	このファイルはmanaライブラリ内で共通利用されるヘッダーファイルです。
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_DEFINE_H___)
#define ___MANA_DEFINE_H___

#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(NDEBUG)
/*! 引数の数を調べ、一致しない場合はreturnします */
#define MANA_ASSERT_PARAMETER(P, I) {									\
	if(mana_actor_get_argument_count(P) != I)							\
		return;															\
}
/*! initアクション中ならばreturnします */
#define MANA_ASSERT_ILLIGAL_CALL_IN_INIT_ACTION(P) {					\
	if(mana_is_in_init_action(P->parent)){								\
		return;															\
}
#else
/*! 引数の数を調べ、一致しない場合は警告を表示してreturnします */
#if 1
#define MANA_ASSERT_PARAMETER(P, I)										\
	if(mana_actor_get_argument_count(P) != I){							\
		 MANA_PRINT("ERROR: %s: function %s number of arguments %d correct%d\n", mana_actor_get_name(P), mana_actor_get_function_name(P), mana_actor_get_argument_count(P), I);\
		 return;														\
	}
/*! initアクション中ならば警告を表示してreturnします */
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)							\
	if(mana_is_in_init_action(P->parent)){								\
		MANA_PRINT("ERROR: %s: init action %s can not call\n", mana_actor_get_name(P), mana_actor_get_function_name(P));\
		return;															\
	}
#else
#define MANA_ASSERT_PARAMETER(P, I)										\
	if(mana_actor_get_argument_count(P) != I){							\
		MANA_PRINT("ERROR: %s: 関数%sの引数の数が%dです。正しい引数の数は%dです。\n", mana_actor_get_name(P), mana_actor_get_function_name(P), mana_actor_get_argument_count(P), I);\
		return;														\
	}
/*! initアクション中ならば警告を表示してreturnします */
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)							\
	if(mana_is_in_init_action(P->parent)){								\
		MANA_PRINT("ERROR: %s: initアクション内で %s は使えません。\n", mana_actor_get_name(P), mana_actor_get_function_name(P));\
		return;															\
	}
#endif
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
