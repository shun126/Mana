/*!
mana (compiler)

@file	data.h
@brief	�f�[�^�Z�N�V�����Ɋւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���̓f�[�^�Z�N�V�����Ɋ֌W����w�b�_�[�t�@�C���ł��B
@author	Shun Moriya
@date	2003-
*/

#if !defined(___SYMBOL_DATA_H___)
#define ___SYMBOL_DATA_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>

//! �f�[�^�Z�N�V�����Ǘ��֐���������
extern void data_initialzie(void);

//! �f�[�^�Z�N�V�����Ǘ��֐����I��
extern void data_finalize(void);

/*!
�f�[�^�Z�N�V�����o�b�t�@�̃A�h���X���擾
@return	�f�[�^�Z�N�V�����o�b�t�@�̃A�h���X
*/
extern char* data_get_buffer(void);

/*!
�f�[�^�Z�N�V�����̃T�C�Y���擾
@return	�f�[�^�Z�N�V�����̃T�C�Y
*/
extern uint32_t data_get_size(void);

/*!
�e�L�X�g�f�[�^������
@param[in]	text	�e�L�X�g
@return		�I�t�Z�b�g�A�h���X
*/
extern uint32_t data_get(const char* text);

/*!
�e�L�X�g�f�[�^��ݒ�
@param[in]	text	�e�L�X�g
@return		�I�t�Z�b�g�A�h���X
*/
extern uint32_t data_set(const char* text);

/*!
�f�[�^�Z�N�V�������t�@�C���֏o��
@param[out]	file		�t�@�C�����ʎq
@retval		true	�o�͐���
@retval		false	�o�͎��s
*/
extern bool data_write(mana_stream* stream);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
