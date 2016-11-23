/*!
 * mana (library)
 *
 * @file	mana_array.h
 * @brief	mana_array�N���X�Ɋւ���w�b�_�[�t�@�C��
 * @detail	���̃t�@�C����mana_array�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_ARRAY_H___)
#define ___MANA_ARRAY_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif
#include <stddef.h>

/*!
 * @brief	�N���X
 * mana_array�N���X�̓t���[���o�b�t�@��\���Ă��܂��B
 *
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana_array
{
	void** handle;			/*!< �o�b�t�@ */
	size_t allocated_size;	/*!< �m�ۍς݃T�C�Y */
	size_t used_size;		/*!< �g�p�ς݃T�C�Y */
}mana_array;

/*! mana_array �I�u�W�F�N�g�̐��� */
extern mana_array* mana_array_create(const size_t size);

/*! mana_array �I�u�W�F�N�g�̔j�� */
extern void mana_array_destroy(mana_array* self);

/*! mana_array �I�u�W�F�N�g�̃V���A���C�Y */
extern void mana_array_serialize(const mana_array* self, mana_stream* stream);

/*! mana_array �I�u�W�F�N�g�̃f�V���A���C�Y */
extern void mana_array_deserialize(mana_array* self, mana_stream* stream);

/*! mana_array �I�u�W�F�N�g�̏����� */
extern void mana_array_initialize(mana_array* self, const size_t size);

/*! mana_array �I�u�W�F�N�g�̊J�� */
extern void mana_array_finalize(mana_array* self);

/*! �z����N���A */
extern void mana_array_clear(mana_array* self);

/*! �z����Q�� */
extern void* mana_array_get(const mana_array* self, const size_t index);

/*! �z��Ƀ|�C���^�[��ݒ� */
extern void mana_array_set(mana_array* self, const size_t index, const void* pointer);

/*! �z��̃T�C�Y���擾 */
extern size_t mana_array_get_size(const mana_array* self);

/*! �z��Ƀ|�C���^�[��ǉ� */
extern void mana_array_append(mana_array* self, const void* pointer);

/*! �z��̃T�C�Y��ύX */
extern void mana_array_resize(mana_array* self, const size_t size);

/*! �z����r */
extern int mana_array_compare(const mana_array* self, const mana_array* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
