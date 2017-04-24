/*!
mana (compiler)

@file	pool.h
@brief	������i�[�Ɋւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���͕�����������ɎQ�Ƃ��邽�߂̃n�b�V���Ɋ֌W����w�b�_�[�t�@�C���ł��B
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_POOL_H___)
#define ___MANA_POOL_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! ������i�[�V�X�e���̏����� */
extern void pool_initialize(void);

/*! ������i�[�V�X�e���̏I������ */
extern void pool_finalize(void);

/*! ������i�[�V�X�e���ɕ������ݒ肵�܂� */
extern char* pool_set(char* string);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
