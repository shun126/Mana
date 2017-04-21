/*!
mana (library)

@file	mana_frame.h
@brief	mana_frame�N���X�Ɋւ���w�b�_�[�t�@�C��
@detail
���̃t�@�C����mana_frame�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
mana_frame�N���X��mana_actor�N���X�̃��[�J���ϐ��̑�����s�Ȃ��܂��B
�{���̓X�^�b�N�ő��삵���ق����ǂ��̂�������܂��񂪁c

@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_FRAME_H___)
#define ___MANA_FRAME_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

/*!
@brief	mana_frame�N���X
mana_frame�N���X�̓t���[���o�b�t�@��\���Ă��܂��B

C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
*/
typedef struct mana_frame
{
	uint8_t* buffer;		//!< �o�b�t�@
	size_t allocated_size;	//!< �m�ۍς݃T�C�Y
	size_t used_size;		//!< �g�p�ς݃T�C�Y
}mana_frame;

/*!
mana_frame �I�u�W�F�N�g�̐���
@return	mana_frame �I�u�W�F�N�g
@warning	void mana_frame_initialize(mana_frame* self)���ĂԕK�v�͂���܂���B
*/
extern mana_frame* mana_frame_create(void);

/*!
mana_frame �I�u�W�F�N�g�̐���
@param[in]	size	�m�ۃT�C�Y
@return		mana_frame �I�u�W�F�N�g
@warning	void mana_frame_initialize_with_size(mana_frame* self, size_t size)���ĂԕK�v�͂���܂���B
*/
extern mana_frame* mana_frame_create_with_size(const size_t size);

/*!
mana_frame �I�u�W�F�N�g�̔j��
@param[in]	self	mana_frame �I�u�W�F�N�g
@warning	void mana_frame_finalize(mana_frame* self)���ĂԕK�v�͂���܂���B
*/
extern void mana_frame_destroy(mana_frame* self);

/*!
mana_frame �I�u�W�F�N�g�̏�����
@param[in]	self	mana_frame �I�u�W�F�N�g
*/
extern void mana_frame_initialize(mana_frame* self);

/*!
mana_frame �I�u�W�F�N�g�̏�����
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	size	�m�ۃT�C�Y
*/
extern void mana_frame_initialize_with_size(mana_frame* self, const size_t size);

/*!
mana_frame �I�u�W�F�N�g�̃V���A���C�Y
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[out]	stream	mana_stream �I�u�W�F�N�g
*/
extern void mana_frame_serialize(const mana_frame* self, mana_stream* stream);

/*!
mana_frame �I�u�W�F�N�g�̃f�V���A���C�Y
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	stream	mana_stream �I�u�W�F�N�g
*/
extern void mana_frame_deserialize(mana_frame* self, mana_stream* stream);

/*!
mana_frame �I�u�W�F�N�g�̊J��
@param[in]	self	mana_frame �I�u�W�F�N�g
*/
extern void mana_frame_finalize(mana_frame* self);

/*!
mana_frame �I�u�W�F�N�g�̃N���A
@param[in]	self	mana_frame �I�u�W�F�N�g
*/
extern void mana_frame_clear(mana_frame* self);

/*!
�t���[���o�b�t�@�̊m��
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	size	�ǉ��T�C�Y
*/
extern void mana_frame_allocate(mana_frame* self, const size_t size);

/*!
�t���[���o�b�t�@�̊J��
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	size	�J���T�C�Y
*/
extern void mana_frame_release(mana_frame* self, const size_t size);

/*!
�t���[���o�b�t�@�̃A�h���X���擾
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	index	�t���[���|�C���^�ւ̃I�t�Z�b�g�l
@return		�A�h���X
*/
extern void* mana_frame_get_address(const mana_frame* self, const size_t index);

/*!
�t���[���o�b�t�@�̃T�C�Y���擾
@param[in]	self	mana_frame �I�u�W�F�N�g
@return		�t���[���o�b�t�@�̃T�C�Y/�t���[���|�C���^
*/
extern size_t mana_frame_get_size(const mana_frame* self);

/*!
�t���[���o�b�t�@�̃T�C�Y��ݒ�
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	size	�t���[���o�b�t�@�̃T�C�Y/�t���[���|�C���^
*/
extern void mana_frame_set_size(mana_frame* self, const size_t size);

/*!
�t���[���o�b�t�@���r
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	other	mana_frame �I�u�W�F�N�g
@retval		== 0	����̓��e
@retval		!= 0	�قȂ���e
*/
extern int32_t mana_frame_compare(const mana_frame* self, const mana_frame* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
