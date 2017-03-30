/*!
mana (compiler)

@file	mana_generator.h
@brief	�R���p�C���Z�N�V�����Ɋւ���\�[�X�t�@�C��
@detail	���̃t�@�C���̓R���p�C���Z�N�V�����Ɋ֌W����\�[�X�t�@�C���ł��B
@author	Shun Moriya
@date	2017-
*/

#if !defined(__MANA_GENERATOR_H___)
#define __MANA_GENERATOR_H___

#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//! MANA_NODE_CALL�m�[�h���������鎞�ɌĂ΂��R�[���o�b�N�֐��̌^
typedef mana_node* (mana_generator_event_funtion_type)(mana_node*);

//! ������
extern void mana_generator_initialize(void);
//! �I��
extern void mana_generator_finalize(void);

/*!
MANA_NODE_CALL�m�[�h���������鎞�ɌĂ΂��R�[���o�b�N�֐���o�^���܂�
@param[in]	name		�֐���
@param[in]	function	�R�[���o�b�N�֐�
*/
extern void mana_generator_add_event(const char* name, mana_generator_event_funtion_type function);

/*!
�m�[�h��H��Ȃ��璆�Ԍ���ɖ|�󂵂܂�
@param	node			�m�[�h
@param	enable_load		true�Ȃ��load���߂͗L���Afalse�Ȃ��load���߂͖���
*/
extern void mana_generator_genearte_code(mana_node* node, int32_t enable_load);

extern void mana_generator_expression(mana_node* node, int32_t enable_assign);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
