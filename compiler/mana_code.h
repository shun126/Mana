/*
mana (compiler)

@file	mana_code.h
@brief	���߃Z�N�V�����Ɋւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���͖��߃Z�N�V�����Ɋ֌W����w�b�_�[�t�@�C���ł��B
@author	Shun Moriya <shun@mnu.sakura.ne.jp>
@date	2003-
*/

#if !defined(___MANA_CODE_H___)
#define ___MANA_CODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>
#include <stdio.h>

/*! ���߃Z�N�V�����Ǘ��֐��������� */
extern void mana_code_initialize(void);

/*! ���߃Z�N�V�����Ǘ��֐����I�� */
extern void mana_code_finalize(void);

/*! ���߃Z�N�V�������k�� */
extern void mana_code_reduce(size_t size);

/*! int8_t�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_char(int8_t value);

/*! int16_t�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_short(int16_t value);

/*! int32_t�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_integer(int32_t value);

/*! uint8_t�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_unsigned_char(uint8_t value);

/*! uint16_t�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_unsigned_short(uint16_t value);

/*! uint32_t�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_unsigned_integer(uint32_t value);

/*! float�^���l�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_float(float value);

/*! �I�y�R�[�h�𖽗߃Z�N�V�����ɐݒ� */
extern void mana_code_set_opecode(uint8_t code);

/*! �I�y�R�[�h�ƃI�y�����h�𖽗߃Z�N�V�����ɐݒ� */
extern int32_t mana_code_set_opecode_and_operand(uint8_t code, int32_t address);

/*! �I�y�R�[�h�Ɠ�̃I�y�����h�𖽗߃Z�N�V�����ɐݒ� */
extern int32_t mana_code_set_opecode_and_two_operands(uint8_t code, int32_t address, int32_t size);

/*! �I�y�R�[�h��u�� */
extern void mana_code_replace_opecode(int32_t address, uint8_t code);

/*! �I�y�R�[�h�Ɋ֌W�t����ꂽ�A�h���X��u�� */
extern void mana_code_replace_address(int32_t address, int32_t new_address);

/*! �I�y�R�[�h�Ɋ֌W�t����ꂽ�A�h���X��H���Ēu�� */
extern void mana_code_replace_all(int32_t address, int32_t new_address);

/*! ���߃Z�N�V�������t�@�C���֏o�� */
extern int32_t mana_code_write(mana_stream* stream);

/*! ���߃Z�N�V�������������֏o�� */
extern int32_t mana_code_copy(void*);

/*! ���߃Z�N�V�����̃T�C�Y���擾 */
extern int32_t mana_code_get_pc(void);

/*! ���߃Z�N�V�����̃T�C�Y���擾 */
extern int32_t mana_code_get_size(void);

/*! ���߃Z�N�V�����o�b�t�@�̃A�h���X���擾 */
extern void* mana_code_get_buffer(void);

/*! �I�y�R�[�h���擾 */
extern uint8_t mana_code_getcode(int32_t address);

/*! int32_t�^���l�𖽗߃Z�N�V��������擾 */
extern int32_t mana_code_get_integer(int32_t address);

/*! float�^���l�𖽗߃Z�N�V��������擾 */
extern float mana_code_get_float(int32_t address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
