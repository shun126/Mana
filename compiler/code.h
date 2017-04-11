/*!
mana (compiler)

@file	code.h
@brief	���߃Z�N�V�����Ɋւ���w�b�_�[�t�@�C��
@detail	���̃t�@�C���͖��߃Z�N�V�����Ɋ֌W����w�b�_�[�t�@�C���ł��B
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_CODE_H___)
#define ___MANA_CODE_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>

//! ���߃Z�N�V�����Ǘ��֐���������
extern void code_initialize(void);

//! ���߃Z�N�V�����Ǘ��֐����I��
extern void code_finalize(void);

/*!
���߃Z�N�V�������k��
@param[in]	size	�k������T�C�Y
*/
extern void code_reduce(const size_t size);

/*!
int8_t�^���l�𖽗߃Z�N�V�����ɐݒ�
@param[in]	value	�l
*/
extern void code_set_char(const int8_t value);

/*!
int16_t�^���l�𖽗߃Z�N�V�����ɐݒ�
@param[in]	value	�l
*/
extern void code_set_short(const int16_t value);

/*!
int32_t�^���l�𖽗߃Z�N�V�����ɐݒ�
@param[in]	value	�l
*/
extern void code_set_integer(const int32_t value);

/*!
uint8_t�^���l�𖽗߃Z�N�V�����ɐݒ�
@param[in]	value	�l
*/
extern void code_set_unsigned_char(const uint8_t value);

/*!
uint16_t�^���l�𖽗߃Z�N�V�����ɐݒ�
@param[in]	value	�l
*/
extern void code_set_unsigned_short(const uint16_t value);

/*!
uint32_t�^���l�𖽗߃Z�N�V�����ɐݒ�
@param[in]	value	�l
*/
extern void code_set_unsigned_integer(const uint32_t value);

/*!
float�^���l�𖽗߃Z�N�V�����ɐݒ�
@param[in]	value	�l
*/
extern void code_set_float(const float value);

/*!
�I�y�R�[�h�𖽗߃Z�N�V�����ɐݒ�
@param[in]	code	�I�y�R�[�h
*/
extern void code_set_opecode(const uint8_t code);

/*!
�I�y�R�[�h�ƃI�y�����h�𖽗߃Z�N�V�����ɐݒ�
@param[in]	code	�I�y�R�[�h
@param[in]	address	�I�y�����h
*/
extern int32_t code_set_opecode_and_operand(const uint8_t code, const int32_t address);

/*!
* @param[in]	code	�I�y�R�[�h
* @param[in]	address	���I�y�����h
* @param[in]	size	���I�y�����h
�I�y�R�[�h�Ɠ�̃I�y�����h�𖽗߃Z�N�V�����ɐݒ�
*/
extern int32_t code_set_opecode_and_two_operands(const uint8_t code, const int32_t address, const int32_t size);

/*!
�I�y�R�[�h��u��
@param[in]	address	�A�h���X
@param[in]	code	�I�y�R�[�h
*/
extern void code_replace_opecode(const int32_t address, const uint8_t code);

/*!
�I�y�R�[�h�Ɋ֌W�t����ꂽ�A�h���X��u��
@param[in]	address		�A�h���X
@param[in]	new_address	�u��������A�h���X
*/
extern void code_replace_address(const int32_t address, const int32_t new_address);

/*!
�I�y�R�[�h�Ɋ֌W�t����ꂽ�A�h���X��H���Ēu��
@param[in]	base_address	�A�h���X
@param[in]	new_address		�u��������A�h���X
*/
extern void code_replace_all(const int32_t base_address, const int32_t new_address);

/*!
���߃Z�N�V�������t�@�C���֏o��
@param[out]	file	�t�@�C�����ʎq
@retval		true	�o�͐���
@retval		false	�o�͎��s
*/
extern bool code_write(mana_stream* stream);

/*!
���߃Z�N�V�������������֏o��
@param[out]	file	�t�@�C�����ʎq
@return		�o�͂����T�C�Y
*/
extern int32_t code_copy(void* buffer);

/*!
���߃Z�N�V�����̃T�C�Y���擾
@return	���߃Z�N�V�����̃T�C�Y
*/
extern int32_t code_get_pc(void);

/*!
���߃Z�N�V�����̃T�C�Y���擾
@return	���߃Z�N�V�����̃T�C�Y
*/
extern int32_t code_get_size(void);

/*!
���߃Z�N�V�����o�b�t�@�̃A�h���X���擾
@return	���߃Z�N�V�����o�b�t�@�̃A�h���X
*/
extern void* code_get_buffer(void);

/*!
�I�y�R�[�h���擾
@param[in]	address		�A�h���X
@return		�I�y�R�[�h
*/
extern uint8_t code_getcode(const int32_t address);

/*!
int32_t�^���l�𖽗߃Z�N�V��������擾
@param[in]	address		�A�h���X
@return		int32_t�^���l
*/
extern int32_t code_get_integer(const int32_t address);

/*!
float�^���l�𖽗߃Z�N�V��������擾
@param[in]	address		�A�h���X
@return		float�^���l
*/
extern float code_get_float(const int32_t address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
