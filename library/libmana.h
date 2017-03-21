/*!
 * mana (library)
 *
 * @file	libmana.h
 * @brief	�V�X�e���S�̂Ɋւ���w�b�_�t�@�C��
 * @detail	���̃t�@�C���̓��C�u�����S�̂Ɋ֌W����w�b�_�t�@�C���ł��B
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

/*! ���j�[�N�Ȗ��O�̍ő�T�C�Y */
#define MANA_UNIQUE_NAME_LENGTH	(16)

/*! �O���֐��o�^�}�N�� */
#define MANA_REGIST_FUNCTION(a)	mana_regist_function((const char*)#a, a)

/*! ���C�u�����̏����� */
extern void mana_initialize(void);

/*! ���C�u�����̏I�� */
extern void mana_finalize(void);

/*! �O���֐��̓o�^ */
extern bool mana_regist_function(const char* name, mana_external_funtion_type* function);

/*! �O���֐��̓o�^���� */
extern void mana_unregist_function(const char* name);

/*! �f�o�b�N���[�h�̐ݒ� */
extern void mana_set_debug_mode(const bool enable);

/*! �f�o�b�N���[�h�̎擾 */
extern bool mana_is_debug_mode(void);

/*! �p�x���� */
extern float mana_angle_mod(const float x, const float div);

/*! �X�^�e�B�b�N�ϐ��̈���m�ۂ��܂��B�ŏ��̊m�ۂ͓��e���O�ŏ��������܂��B */
extern void mana_reallocate_static_variables(const size_t allocate_size);

/*! �X�^�e�B�b�N�ϐ��̈�̃V���A���C�Y */
extern void mana_serialize_static_variables(mana_stream* stream);

/*! �X�^�e�B�b�N�ϐ��̈�̃f�V���A�C�Y */
extern void mana_deserialize_static_variables(mana_stream* stream);

/*! �X�^�e�B�b�N�ϐ��̈�̃A�h���X�̎擾 */
extern uint8_t* mana_get_static_variables(void);

/*! �X�^�e�B�b�N�ϐ��̈�̃T�C�Y�̎擾 */
extern size_t mana_get_static_variables_size(void);

/*! ���j�[�N�Ȗ��O�𐶐� */
extern void mana_generate_unique_name(char* name, const size_t size);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
