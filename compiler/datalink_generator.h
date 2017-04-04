/*!
mana (compiler)

@file	datalink_generator.h
@brief	�f�[�^�Z�N�V�����Ɋւ���w�b�_�t�@�C��
@detail	���̃t�@�C���̓f�[�^�Z�N�V�����Ɋ֌W����w�b�_�t�@�C���ł��B
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_DATALINK_GENERATOR_H___)
#define ___MANA_DATALINK_GENERATOR_H___

#include <libmana.h>
#include <stdio.h>

#define MANA_DATALINK_ALIGNMENT_SIZE	0x10

extern void mana_datalink_generator_initialize(void);
extern void mana_datalink_generator_finalize(void);
extern size_t mana_datalink_generator_get_number_of_files(void);
extern int32_t mana_datalink_generator_append(const char* file_name);
extern int32_t mana_datalink_generator_write_data(mana_stream* stream);

#endif
