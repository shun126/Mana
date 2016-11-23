/*
 * mana (compiler)
 *
 * @file	mana_code.h
 * @brief	���߃Z�N�V�����Ɋւ���w�b�_�[�t�@�C��
 * @detail	���̃t�@�C���͖��߃Z�N�V�����Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#define ___MANA_CODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stdio.h>

/*! ���߃Z�N�V�����Ǘ��֐��������� */
extern void mana_code_initialize(void);

/*! ���߃Z�N�V�����Ǘ��֐����I�� */
extern void mana_code_finalize(void);

/*! ���߃Z�N�V�������k�� */
extern void mana_code_reduce(size_t size);

/*! char�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_char(char value);

/*! short�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_short(short value);

/*! int�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_integer(int value);

/*! unsigned char�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_unsigned_char(unsigned char value);

/*! unsigned short�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_unsigned_short(unsigned short value);

/*! unsigned int�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_unsigned_integer(unsigned int value);

/*! float�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_float(float value);

/*! �I�y�R�[�h�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_opecode(unsigned char code);

/*! �I�y�R�[�h�ƃI�y�����h�𖽗߃Z�N�V�����ɐݒ� */
extern int mana_code_set_opecode_and_operand(unsigned char code, int address);

/*! �I�y�R�[�h�Ɠ�̃I�y�����h�𖽗߃Z�N�V�����ɐݒ� */
extern int mana_code_set_opecode_and_two_operands(unsigned char code, int address, int size);

/*! �I�y�R�[�h��u�� */
extern void mana_code_replace_opecode(int address, unsigned char code);

/*! �I�y�R�[�h�Ɋ֌W�t����ꂽ�A�h���X��u�� */
extern void mana_code_replace_address(int address, int new_address);

/*! �I�y�R�[�h�Ɋ֌W�t����ꂽ�A�h���X��H���Ēu�� */
extern void mana_code_replace_all(int address, int new_address);

/*! ���߃Z�N�V�������t�@�C���֏o�� */
extern int mana_code_write(FILE* file);

/*! ���߃Z�N�V�������������֏o�� */
extern int mana_code_copy(void*);

/*! ���߃Z�N�V�����̃T�C�Y���擾 */
extern int mana_code_get_pc(void);

/*! ���߃Z�N�V�����̃T�C�Y���擾 */
extern int mana_code_get_size(void);

/*! ���߃Z�N�V�����o�b�t�@�̃A�h���X���擾 */
extern void* mana_code_get_buffer(void);

/*! �I�y�R�[�h���擾 */
extern unsigned char mana_code_getcode(int address);

/*! int�^���l�𖽗߃Z�N�V��������擾 */
extern int mana_code_get_integer(int address);

/*! float�^���l�𖽗߃Z�N�V��������擾 */
extern float mana_code_get_float(int address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
