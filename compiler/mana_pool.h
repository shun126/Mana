/*
 * mana (compiler)
 *
 * @file	mana_pool.h
 * @brief	������i�[�Ɋւ���w�b�_�[�t�@�C��
 * @detail	���̃t�@�C���͕�����������ɎQ�Ƃ��邽�߂̃n�b�V���Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_POOL_H___)
#define ___MANA_POOL_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! ������i�[�V�X�e���̏����� */
extern void mana_pool_initialize(void);

/*! ������i�[�V�X�e���̏I������ */
extern void mana_pool_finalize(void);

/*! ������i�[�V�X�e���ɕ������ݒ肵�܂� */
extern char* mana_pool_set(char* string);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
