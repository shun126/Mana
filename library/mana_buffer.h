/*!
 * mana (library)
 *
 * @file	mana_buffer.h
 * @brief	mana_buffer�N���X�Ɋւ���w�b�_�[�t�@�C��
 * @detail
 * ���̃t�@�C����mana_buffer�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * mana_stream�N���X�͉ϒ��o�b�t�@�Ɋւ��鑀����s�Ȃ��܂��B
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_BUFFER_H___)
#define ___MANA_BUFFER_H___

#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*!
 * @brief	mana_buffer�N���X
 *
 * mana_buffer�N���X�͉ϒ��o�b�t�@��\���Ă��܂��B
 *
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana_buffer
{
	char* buffer;
	size_t allocated_size;
	size_t used_size;
}mana_buffer;

/*! mana_stream �I�u�W�F�N�g�̐��� */
extern mana_buffer* mana_buffer_create(void);

/*! mana_stream �I�u�W�F�N�g�̔j�� */
extern void mana_buffer_destroy(mana_buffer* self);

/*! �o�b�t�@�̐擪�A�h���X���擾 */
extern void* mana_buffer_get_buffer(mana_buffer* self);

/*! �m�ۍς݃o�b�t�@�T�C�Y���擾 */
extern size_t mana_buffer_get_allocated_size(mana_buffer* self);

/*! �g�p���Ă���o�b�t�@�T�C�Y���擾 */
extern size_t mana_buffer_get_used_size(mana_buffer* self);

/*! �f�[�^���v�b�V�� */
extern int32_t mana_buffer_push(mana_buffer* self, const void* pointer, const size_t size);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
