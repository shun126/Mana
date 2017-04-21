/*!
mana (library)

@file	mana_stack.h
@brief	mana_stack�N���X�Ɋւ���w�b�_�[�t�@�C��
@detail
���̃t�@�C����mana_stack�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
mana_stack�N���X��mana_actor�N���X�̃X�^�b�N������s�Ȃ��܂��B

@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_STACK_H___)
#define ___MANA_STACK_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

/*!
@brief	mana_stack�N���X
mana_stack�N���X�̓X�^�b�N��\���Ă��܂��B

C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
*/
typedef struct mana_stack
{
	//! �o�b�t�@
	union
	{
		const char** string_handle;	//!< ������n���h��
		void** void_handle;			//!< �f�[�^�n���h��
#if UINTPTR_MAX == UINT64_MAX
		double* float_pointer;		//!< �����|�C���^
		int64_t* integer_pointer;	//!< �����|�C���^
#elif UINTPTR_MAX == UINT32_MAX
		float* float_pointer;		//!< �����|�C���^
		int32_t* integer_pointer;	//!< �����|�C���^
#else
#error "unsupport pointer size"
#endif
		void* void_pointer;			//!< �f�[�^�|�C���^
	}buffer;
	size_t allocated_size;			//!< �m�ۍς݃T�C�Y
	size_t used_size;				//!< �g�p�ς݃T�C�Y(�X�^�b�N�|�C���^)
}mana_stack;

/*!
mana_stack �I�u�W�F�N�g�̐���
@return		mana_stack �I�u�W�F�N�g
@warning	void mana_stack_initialize(mana_stack* self)���ĂԕK�v�͂���܂���B
*/
extern mana_stack* mana_stack_create(void);

/*!
mana_stack �I�u�W�F�N�g�̐���
@param[in]	size	�m�ۃT�C�Y
@return		mana_stack �I�u�W�F�N�g
@warning	void mana_stack_initialize_with_size(mana_stack* self, size_t size)���ĂԕK�v�͂���܂���B
*/
extern mana_stack* mana_stack_create_with_size(const size_t size);

/*!
mana_stack �I�u�W�F�N�g�̔j��
@param[in]	self	mana_stack �I�u�W�F�N�g
@warning	void mana_stack_finalize(mana_stack* self)���ĂԕK�v�͂���܂���B
*/
extern void mana_stack_destroy(mana_stack* self);

/*!
mana_stack �I�u�W�F�N�g�̏�����
@param[in]	self	mana_stack �I�u�W�F�N�g
*/
extern void mana_stack_initialize(mana_stack* self);

/*!
mana_stack �I�u�W�F�N�g�̏�����
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	size	�m�ۃT�C�Y
*/
extern void mana_stack_initialize_with_size(mana_stack* self, const size_t size);

/*!
mana_stack �I�u�W�F�N�g�̊J��
@param[in]	self	mana_stack �I�u�W�F�N�g
*/
extern void mana_stack_finalize(mana_stack* self);

/*!
mana_stack �I�u�W�F�N�g�̃V���A���C�Y
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[out]	stream	mana_stream �I�u�W�F�N�g
*/
extern void mana_stack_serialize(const mana_stack* self, mana_stream* stream);

/*!
mana_stack �I�u�W�F�N�g�̃f�V���A���C�Y
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	stream	mana_stream �I�u�W�F�N�g
*/
extern void mana_stack_deserialize(mana_stack* self, mana_stream* stream);

/*!
mana_stack �I�u�W�F�N�g�̃N���A
@param[in]	self	mana_stack �I�u�W�F�N�g
*/
extern void mana_stack_clear(mana_stack* self);

/*!
mana_stack �I�u�W�F�N�g�̃R�s�[
@param[in]	self	mana_stack �I�u�W�F�N�g
*/
extern void mana_stack_duplicate(mana_stack* self);

/*!
�X�^�b�N�|�C���^���ړ�
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	size	�폜����T�C�Y
*/
extern void mana_stack_remove(mana_stack* self, const size_t size);

/*!
�������v�b�V��
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	value	�v�b�V������l
*/
extern void mana_stack_push_integer(mana_stack* self, const int32_t value);

/*!
�������v�b�V��
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	value	�v�b�V������l
*/
extern void mana_stack_push_real(mana_stack* self, const float value);

/*!
��������v�b�V��
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	string	�v�b�V�����镶����
*/
extern void mana_stack_push_string(mana_stack* self, const char* string);

/*!
�|�C���^���v�b�V��
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	pointer	�v�b�V������l
*/
extern void mana_stack_push_pointer(mana_stack* self, void* pointer);

/*!
�f�[�^���v�b�V��
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	buffer	�v�b�V������f�[�^�̐擪�A�h���X
@param[in]	size	�v�b�V������f�[�^�̃T�C�Y
*/
extern void mana_stack_push_data(mana_stack* self, const void* buffer, const size_t size);

/*!
�������|�b�v
@param[in]	self	mana_stack �I�u�W�F�N�g
@return		�|�b�v�����l
*/
extern int32_t mana_stack_pop_integer(mana_stack* self);

/*!
�������|�b�v
@param[in]	self	mana_stack �I�u�W�F�N�g
@return		�|�b�v�����l
*/
extern float mana_stack_pop_real(mana_stack* self);

/*!
��������|�b�v
@param[in]	self	mana_stack �I�u�W�F�N�g
@return		�|�b�v�����l
*/
extern const char* mana_stack_pop_string(mana_stack* self);

/*!
�|�C���^���|�b�v
�X�^�b�N�Ɋi�[���ꂽ�|�C���^���|�b�v���܂��B
void* mana_stack_pop_address(mana_stack* self)�̓X�^�b�N�̃A�h���X���|�b�v����_���Ⴂ�܂��B
@param[in]	self	mana_stack �I�u�W�F�N�g
@return		�|�b�v�����l
*/
extern void* mana_stack_pop_pointer(mana_stack* self);

/*!
�A�h���X���|�b�v
�X�^�b�N�̃A�h���X���|�b�v���܂��B
void* mana_stack_pop_pointer(mana_stack* self)�̓X�^�b�N�Ɋi�[���ꂽ�|�C���^���|�b�v����_���Ⴂ�܂��B
@param[in]	self	mana_stack �I�u�W�F�N�g
@return		�|�b�v�����l
*/
extern void* mana_stack_pop_address(mana_stack* self);

/*!
�f�[�^���|�b�v
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[out]	buffer	�|�b�v�����f�[�^�̐擪�A�h���X
@param[out]	size	�|�b�v�����f�[�^�̃T�C�Y
*/
extern void mana_stack_pop_data(mana_stack* self, void* buffer, const size_t size);

/*!
�������擾
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@return		�l
*/
extern int32_t mana_stack_get_integer(const mana_stack* self, const size_t index);

/*!
�������擾
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@return		�l
*/
extern float mana_stack_get_real(const mana_stack* self, const size_t index);

/*!
��������擾
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@return		������
*/
extern const char* mana_stack_get_string(const mana_stack* self, const size_t index);

/*!
�|�C���^���擾
�X�^�b�N�Ɋi�[���ꂽ�|�C���^���擾���܂��B
void* mana_stack_get_address(mana_stack* self, size_t index)�̓X�^�b�N�̃A�h���X���擾����_���Ⴂ�܂��B
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@return		�l
*/
extern void* mana_stack_get_pointer(const mana_stack* self, const size_t index);

/*!
�A�h���X���擾
�X�^�b�N�̃A�h���X���擾���܂��B
void* mana_stack_get_pointer(mana_stack* self, size_t index)�̓X�^�b�N�Ɋi�[���ꂽ�|�C���^���擾����_���Ⴂ�܂��B
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@return		�A�h���X
*/
extern void* mana_stack_get_address(const mana_stack* self, const size_t index);

/*!
������ݒ�
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@param[in]	integer	�l
*/
extern void mana_stack_set_integer(mana_stack* self, const size_t index, const int32_t integer);

/*!
������ݒ�
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@param[in]	real	�l
*/
extern void mana_stack_set_real(mana_stack* self, const size_t index, const float real);

/*!
�������ݒ�
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@param[in]	string	������
*/
extern void mana_stack_set_string(mana_stack* self, const size_t index, const char* string);

/*!
�|�C���^��ݒ�
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
@param[in]	pointer	�|�C���^
*/
extern void mana_stack_set_pointer(mana_stack* self, const size_t index, void* pointer);

/*!
�X�^�b�N�̃T�C�Y���擾
@param[in]	self	mana_stack �I�u�W�F�N�g
@return		�X�^�b�N�̃T�C�Y/�X�^�b�N�|�C���^
*/
extern size_t mana_stack_get_size(const mana_stack* self);

/*!
�X�^�b�N�̃T�C�Y��ݒ�
@param[in]	self	mana_stack �I�u�W�F�N�g
@param[in]	size	�X�^�b�N�̃T�C�Y/�X�^�b�N�|�C���^
*/
extern void mana_stack_set_size(mana_stack* self, const size_t size);

/*!
�X�^�b�N���r
@param[in]	self	mana_frame �I�u�W�F�N�g
@param[in]	other	mana_frame �I�u�W�F�N�g
@retval		== 0	����̓��e
@retval		!= 0	�قȂ���e
*/
extern int32_t mana_stack_compare(const mana_stack* self, const mana_stack* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
