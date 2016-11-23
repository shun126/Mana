 /*!
  * mana (library)
  *
  * @file	mana_malloc.h
  * @brief	mana���C�u�������ŋ��ʗ��p����郁�����Ǘ��Ɋւ���w�b�_�[�t�@�C��
  * @detail	���̃t�@�C����mana���C�u�������ŋ��ʗ��p����郁�����Ǘ��Ɋւ���w�b�_�[�t�@�C���ł��B
  * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
  * @date	2003-
  */

#if !defined(___MANA_MALLOC_H___)
#define ___MANA_MALLOC_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <stddef.h>

/*! �������u���b�N���m�ۂ���֐� */
typedef void* (*mana_malloc_function)(const size_t);

/*! �������u���b�N���m�ہ{����������֐� */
typedef void* (*mana_calloc_function)(const size_t, const size_t);

/*! �������u���b�N���Ċm�ۂ���֐� */
typedef void* (*mana_realloc_function)(void*, const size_t);

/*! �������u���b�N���J������֐� */
typedef void (*mana_free_function)(void*);

/*! �������u���b�N�̊m�ۊ֐��̓o�^ */
extern void mana_set_malloc_function(mana_malloc_function function);

/*! �������u���b�N���m�ی㏉�����֐��̓o�^ */
extern void mana_set_calloc_function(mana_calloc_function function);

/*! �������u���b�N�̍Ċm�ۊ֐��̓o�^ */
extern void mana_set_realloc_function(mana_realloc_function function);

/*! �������u���b�N�̊J���֐��̓o�^ */
extern void mana_set_free_function(mana_free_function function);

/*! �������u���b�N�̊m�� */
extern void* mana_malloc(const size_t size);

/*! �������u���b�N���m�ی㏉���� */
extern void* mana_calloc(const size_t size, const size_t count);

/*! �������u���b�N�̍Ċm�� */
extern void* mana_realloc(void* buffer, const size_t size);

/*! �������u���b�N�̊J�� */
extern void mana_free(void* buffer);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
