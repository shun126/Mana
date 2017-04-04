/*!
mana (compiler)

@file	jump.h
@brief	break,continue,goto�Ȃǂ̃W�����v�Ɋւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C����break,continue,goto�ȂǃW�����v�Ɋ֌W����w�b�_�[�t�@�C���ł��B
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

/*! �W�����v�`�F�C���̏�� */
typedef enum mana_jump_chain_status
{
	MANA_JUMP_CHAIN_STATE_SWITCH,	/*!< switch�u���b�N */
	MANA_JUMP_CHAIN_STATE_WHILE,	/*!< while�u���b�N */
	MANA_JUMP_CHAIN_STATE_DO,		/*!< do�u���b�N */
	MANA_JUMP_CHAIN_STATE_FOR,		/*!< for�u���b�N */
	MANA_JUMP_CHAIN_STATE_LOOP,		/*!< loop�u���b�N */
	MANA_JUMP_CHAIN_STATE_LOCK,		/*!< lock�u���b�N */
} mana_jump_chain_status;

/*! �W�����v�Ǘ��̏����� */
extern void mana_jump_initialize(void);

/*! �W�����v�Ǘ��̏I������ */
extern void mana_jump_finalize(void);

/*! �W�����v�u���b�N�̊J�n */
extern void mana_jump_open_chain(mana_jump_chain_status);

/*! break�̐ݒ� */
extern int32_t mana_jump_break(int32_t);

/*! continue�̐ݒ� */
extern int32_t mana_jump_continue(int32_t);

/*! continue�W�����v�u���b�N�̏I�� */
extern void mana_jump_close_continue_only(void);

/*! �W�����v�u���b�N�̏I�� */
extern void mana_jump_close_chain(void);

/*! switch�u���b�N�̊J�n */
extern void mana_jump_open_switch(type_description*);

/*! case�̓o�^ */
extern void mana_jump_switch_case(node_entry*);

/*! default�̓o�^ */
extern void mana_jump_switch_default(void);

/*! switch�u���b�N���o�C�i���[�R�[�h�ɕϊ� */
extern void mana_jump_switch_build(void);

/*! switch�u���b�N�̏I�� */
extern void mana_jump_close_switch(void);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
