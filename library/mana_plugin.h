/*!
 * mana (library)
 *
 * @file	mana_plugin.h
 * @brief	�v���O�C���֌W�̃w�b�_�[�t�@�C��
 * @detail	���̃t�@�C���̓v���O�C���֌W�̃w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_PLUGIN_H___)
#define ___MANA_PLUGIN_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! �v���O�C���V�X�e���̏����� */
extern void mana_plugin_initialize(void);

/*! �v���O�C���V�X�e���̏I�� */
extern void mana_plugin_finalize(void);

/*! �v���O�C���t�@�C���̓ǂݍ��� */
extern mana_bool mana_plugin_load(const char* file_name);

/*! �f�B���N�g�����̃v���O�C���t�@�C����ǂݍ��� */
extern mana_bool mana_plugin_regist(const char* directory_name);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
