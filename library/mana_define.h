/*!
 * mana (library)
 *
 * @file	mana_define.h
 * @brief	mana���C�u�������ŋ��ʗ��p�����w�b�_�[�t�@�C��
 * @detail	���̃t�@�C����mana���C�u�������ŋ��ʗ��p�����w�b�_�[�t�@�C���ł��B
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
/*! �����̐��𒲂ׁA��v���Ȃ��ꍇ��return���܂� */
#define MANA_ASSERT_PARAMETER(P, I) {									\
	if(mana_actor_get_argument_count(P) != I)							\
		return;															\
}
/*! init�A�N�V�������Ȃ��return���܂� */
#define MANA_ASSERT_ILLIGAL_CALL_IN_INIT_ACTION(P) {					\
	if(mana_is_in_init_action(P->parent)){								\
		return;															\
}
#else
/*! �����̐��𒲂ׁA��v���Ȃ��ꍇ�͌x����\������return���܂� */
#if 1
#define MANA_ASSERT_PARAMETER(P, I)										\
	if(mana_actor_get_argument_count(P) != I){							\
		 MANA_PRINT("ERROR: %s: function %s number of arguments %d correct%d\n", mana_actor_get_name(P), mana_actor_get_function_name(P), mana_actor_get_argument_count(P), I);\
		 return;														\
	}
/*! init�A�N�V�������Ȃ�Όx����\������return���܂� */
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)							\
	if(mana_is_in_init_action(P->parent)){								\
		MANA_PRINT("ERROR: %s: init action %s can not call\n", mana_actor_get_name(P), mana_actor_get_function_name(P));\
		return;															\
	}
#else
#define MANA_ASSERT_PARAMETER(P, I)										\
	if(mana_actor_get_argument_count(P) != I){							\
		MANA_PRINT("ERROR: %s: �֐�%s�̈����̐���%d�ł��B�����������̐���%d�ł��B\n", mana_actor_get_name(P), mana_actor_get_function_name(P), mana_actor_get_argument_count(P), I);\
		return;														\
	}
/*! init�A�N�V�������Ȃ�Όx����\������return���܂� */
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)							\
	if(mana_is_in_init_action(P->parent)){								\
		MANA_PRINT("ERROR: %s: init�A�N�V�������� %s �͎g���܂���B\n", mana_actor_get_name(P), mana_actor_get_function_name(P));\
		return;															\
	}
#endif
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
