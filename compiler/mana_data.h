/*!
 * mana (compiler)
 *
 * @file	mana_data.h
 * @brief	�f�[�^�Z�N�V�����Ɋւ���w�b�_�[�t�@�C��
 * @detail	���̃t�@�C���̓f�[�^�Z�N�V�����Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_DATA_H___)
#define ___MANA_DATA_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stdio.h>
#include <libmana.h>

/*! �f�[�^�Z�N�V�����Ǘ��֐��������� */
extern void mana_data_initialzie(void);

/*! �f�[�^�Z�N�V�����Ǘ��֐����I�� */
extern void mana_data_finalize(void);

/*! �f�[�^�Z�N�V�����o�b�t�@�̃A�h���X���擾 */
extern char* mana_data_get_buffer(void);

/*! �f�[�^�Z�N�V�����̃T�C�Y���擾 */
extern uint32_t mana_data_get_size(void);

/*! �e�L�X�g�f�[�^������ */
extern uint32_t mana_data_get(char* text);

/*! �e�L�X�g�f�[�^��ݒ� */
extern uint32_t mana_data_set(char* text);

/*! �f�[�^�Z�N�V�������t�@�C���֏o�� */
extern uint32_t mana_data_write(mana_stream* stream);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
