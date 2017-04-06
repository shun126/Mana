/*!
 * mana (library)
 *
 * @file	mana_lzss.h
 * @brief	LZSS�Ɋւ���w�b�_�t�@�C��
 * @detail
 * LZSS - Lempel-Ziv sliding window compression
 * �Z�p�]�_�� C����ɂ��ŐV�A���S���Y�����T ����
 * �E�B���h�E�o�b�t�@���g�p���Ȃ����@�ɉ��ς��܂���
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_LZSS_H___)
#define ___MANA_LZSS_H___

#include "mana_info.h"

extern const char* mana_lzss_get_magic_word();
extern size_t mana_lzss_get_magic_word_size();
extern unsigned char mana_lzss_get_version();
extern unsigned char mana_lzss_get_dictonary();

extern bool mana_lzss_is_valid(const void* compressed_data_pointer);

extern bool mana_lzss_is_compressed(const void* compressed_data_pointer);

extern size_t mana_lzss_get_extract_data_size(const void* compressed_data_pointer);

extern size_t mana_lzss_get_compressed_data_size(const void* compressed_data_pointer);

extern void mana_lzss_extract(void* extract_data_pointer, const void* compressed_data_pointer);

#endif
