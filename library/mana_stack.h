/*!
 * mana (library)
 *
 * @file	mana_stack.h
 * @brief	mana_stack�N���X�Ɋւ���w�b�_�[�t�@�C��
 * @detail
 * ���̃t�@�C����mana_stack�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * mana_stack�N���X��mana_actor�N���X�̃X�^�b�N������s�Ȃ��܂��B
 *
 * @author	Shun Moriya
 * @date	2003-
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
 * @brief	mana_stack�N���X
 * mana_stack�N���X�̓X�^�b�N��\���Ă��܂��B
 *
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana_stack
{
	/*! �o�b�t�@ */
	union
	{
		const char** string_handle;	/*!< ������n���h�� */
		void** void_handle;			/*!< �f�[�^�n���h�� */
		float* float_pointer;		/*!< �����|�C���^ */
		int32_t* integer_pointer;	/*!< �����|�C���^ */
		void* void_pointer;			/*!< �f�[�^�|�C���^ */
	}buffer;
	size_t allocated_size;			/*!< �m�ۍς݃T�C�Y */
	size_t used_size;				/*!< �g�p�ς݃T�C�Y(�X�^�b�N�|�C���^) */
}mana_stack;

/*! mana_stack �I�u�W�F�N�g�̐��� */
extern mana_stack* mana_stack_create(void);

/*! mana_stack �I�u�W�F�N�g�̐��� */
extern mana_stack* mana_stack_create_with_size(const size_t size);

/*! mana_stack �I�u�W�F�N�g�̔j�� */
extern void mana_stack_destroy(mana_stack* self);

/*! mana_stack �I�u�W�F�N�g�̃V���A���C�Y */
extern void mana_stack_serialize(const mana_stack* self, mana_stream* stream);

/*! mana_stack �I�u�W�F�N�g�̃f�V���A���C�Y */
extern void mana_stack_deserialize(mana_stack* self, mana_stream* stream);

/*! mana_stack �I�u�W�F�N�g�̏����� */
extern void mana_stack_initialize(mana_stack* self);

/*! mana_stack �I�u�W�F�N�g�̏����� */
extern void mana_stack_initialize_with_size(mana_stack* self, const size_t size);

/*! mana_stack �I�u�W�F�N�g�̊J�� */
extern void mana_stack_finalize(mana_stack* self);

/*! mana_stack �I�u�W�F�N�g�̃N���A */
extern void mana_stack_clear(mana_stack* self);

/*! mana_stack �I�u�W�F�N�g�̃R�s�[ */
extern void mana_stack_duplicate(mana_stack* self);

/*! �X�^�b�N�|�C���^���ړ� */
extern void mana_stack_remove(mana_stack* self, const size_t size);

/*! �������v�b�V�� */
extern void mana_stack_push_integer(mana_stack* self, const int32_t value);

/*! �������v�b�V�� */
extern void mana_stack_push_real(mana_stack* self, const float value);

/*! ��������v�b�V�� */
extern void mana_stack_push_string(mana_stack* self, const char* string);

/*! �|�C���^���v�b�V�� */
extern void mana_stack_push_pointer(mana_stack* self, void* pointer);

/*! �f�[�^���v�b�V�� */
extern void mana_stack_push_data(mana_stack* self, const void* buffer, const size_t size);

/*! �������|�b�v */
extern int32_t mana_stack_pop_integer(mana_stack* self);

/*! �������|�b�v */
extern float mana_stack_pop_real(mana_stack* self);

/*! ��������|�b�v */
extern const char* mana_stack_pop_string(mana_stack* self);

/*! �|�C���^���|�b�v */
extern void* mana_stack_pop_pointer(mana_stack* self);

/*! �A�h���X���|�b�v */
extern void* mana_stack_pop_address(mana_stack* self);

/*! �f�[�^���|�b�v */
extern void mana_stack_pop_data(mana_stack* self, void* buffer, const size_t size);

/*! �������擾 */
extern int32_t mana_stack_get_integer(const mana_stack* self, const size_t index);

/*! �������擾 */
extern float mana_stack_get_real(const mana_stack* self, const size_t index);

/*! ��������擾 */
extern const char* mana_stack_get_string(const mana_stack* self, const size_t index);

/*! �|�C���^���擾 */
extern void* mana_stack_get_pointer(const mana_stack* self, const size_t index);

/*! �A�h���X���擾 */
extern void* mana_stack_get_address(const mana_stack* self, const size_t index);

/*! ������ݒ� */
extern void mana_stack_set_integer(mana_stack* self, const size_t index, const int32_t integer);

/*! ������ݒ� */
extern void mana_stack_set_real(mana_stack* self, const size_t index, const float real);

/*! �������ݒ� */
extern void mana_stack_set_string(mana_stack* self, const size_t index, const char* string);

/*! �|�C���^��ݒ� */
extern void mana_stack_set_pointer(mana_stack* self, const size_t index, void* pointer);

/*! �X�^�b�N�̃T�C�Y���擾 */
extern size_t mana_stack_get_size(const mana_stack* self);

/*! �X�^�b�N�̃T�C�Y��ݒ� */
extern void mana_stack_set_size(mana_stack* self, const size_t size);

/*! �X�^�b�N���r */
extern int32_t mana_stack_compare(const mana_stack* self, const mana_stack* other);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
