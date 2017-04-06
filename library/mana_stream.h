/*!
 * mana (library)
 *
 * @file	mana_stream.h
 * @brief	mana_stream�N���X�Ɋւ���w�b�_�[�t�@�C��
 * @detail
 * ���̃t�@�C����mana_stream�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * mana_stream�N���X�̓������X�g���[�~���O�Ɋւ��鑀����s�Ȃ��܂��B
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_STREAM_H___)
#define ___MANA_STREAM_H___

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*!
 * @brief	mana_stream�N���X
 *
 * mana_stream�N���X�̓������X�g���[�~���O��\���Ă��܂��B
 *
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana_stream
{
	char* buffer;			/*!< �o�b�t�@ */
	size_t allocated_size;	/*!< �m�ۍς݃T�C�Y */
	size_t used_size;		/*!< �g�p�ς݃T�C�Y */
	size_t pointer;			/*!< �|�b�v��I�t�Z�b�g�A�h���X */
}mana_stream;

/*! mana_stream �I�u�W�F�N�g�̐��� */
extern mana_stream* mana_stream_create(void);

/*! mana_stream �I�u�W�F�N�g�̔j�� */
extern void mana_stream_destroy(mana_stream* self);

/* �t�@�C������f�[�^��ǂݍ��� */
int32_t mana_stream_load(mana_stream* self, const char* filename);

/* �t�@�C���փf�[�^���������� */
int32_t mana_stream_save(mana_stream* self, const char* filename);

/*! �o�b�t�@�̐擪�A�h���X���擾 */
extern void* mana_stream_get_buffer(const mana_stream* self);

/*! �o�b�t�@�̃A�h���X���擾 */
extern void* mana_stream_index_at(const mana_stream* self, const size_t index);

/*! �m�ۍς݃o�b�t�@�T�C�Y���擾 */
extern size_t mana_stream_get_allocated_size(const mana_stream* self);

/*! �g�p���Ă���o�b�t�@�T�C�Y���擾 */
extern size_t mana_stream_get_used_size(const mana_stream* self);

/*! int8_t�T�C�Y�f�[�^���v�b�V�� */
extern void mana_stream_push_char(mana_stream* self, const int8_t value);

/*! int16_t�T�C�Y�f�[�^���v�b�V�� */
extern void mana_stream_push_short(mana_stream* self, const int16_t value);

/*! int32_t�T�C�Y�f�[�^���v�b�V�� */
extern void mana_stream_push_integer(mana_stream* self, const int32_t value);

/*! uint8_t�T�C�Y�f�[�^���v�b�V�� */
extern void mana_stream_push_unsigned_char(mana_stream* self, const uint8_t value);

/*! uint16_t�T�C�Y�f�[�^���v�b�V�� */
extern void mana_stream_push_unsigned_short(mana_stream* self, const uint16_t value);

/*! uint32_t�T�C�Y�f�[�^���v�b�V�� */
extern void mana_stream_push_unsigned_integer(mana_stream* self, const uint32_t value);

/*! float�T�C�Y�f�[�^���v�b�V�� */
extern void mana_stream_push_float(mana_stream* self, const float value);

/*! ��������v�b�V�� */
extern void mana_stream_push_string(mana_stream* self, const char* text);

/*! �f�[�^���v�b�V�� */
extern void mana_stream_push_data(mana_stream* self, const void* pointer, const size_t size);

/*! pop�|�C���^�������߂� */
extern void mana_stream_rewind(mana_stream* self);

/*! int8_t�T�C�Y�f�[�^���|�b�v */
extern int8_t mana_stream_pop_char(mana_stream* self);

/*! int16_t�T�C�Y�f�[�^���|�b�v */
extern int16_t mana_stream_pop_short(mana_stream* self);

/*! int32_t�T�C�Y�f�[�^���|�b�v */
extern int32_t mana_stream_pop_integer(mana_stream* self);

/*! uint8_t�T�C�Y�f�[�^���|�b�v */
extern uint8_t mana_stream_pop_unsigned_char(mana_stream* self);

/*! uint16_t�T�C�Y�f�[�^���|�b�v */
extern uint16_t mana_stream_pop_unsigned_short(mana_stream* self);

/*! uint32_t�T�C�Y�f�[�^���|�b�v */
extern uint32_t mana_stream_pop_unsigned_integer(mana_stream* self);

/*! float�T�C�Y�f�[�^���|�b�v */
extern float mana_stream_pop_float(mana_stream* self);

/*! ��������|�b�v */
extern void mana_stream_pop_string(mana_stream* self, char* pointer, const size_t size);

/*! �f�[�^���|�b�v */
extern void mana_stream_pop_data(mana_stream* self, void* pointer, const size_t size);

/*! int8_t�T�C�Y�f�[�^���擾 */
extern int8_t mana_stream_get_char(const mana_stream* self);

/*! int16_t�T�C�Y�f�[�^���擾 */
extern int16_t mana_stream_get_short(const mana_stream* self);

/*! int32_t�T�C�Y�f�[�^���擾 */
extern int32_t mana_stream_get_integer(const mana_stream* self);

/*! uint8_t�T�C�Y�f�[�^���擾 */
extern uint8_t mana_stream_get_unsigned_char(const mana_stream* self);

/*! uint16_t�T�C�Y�f�[�^���擾 */
extern uint16_t mana_stream_get_unsigned_short(const mana_stream* self);

/*! uint32_t�T�C�Y�f�[�^���擾 */
extern uint32_t mana_stream_get_unsigned_integer(const mana_stream* self);

/*! float�T�C�Y�f�[�^���擾 */
extern float mana_stream_get_float(const mana_stream* self);

/*! ��������擾 */
extern void mana_stream_get_string(const mana_stream* self, char* pointer, const size_t size);

/*! ������̃|�C���^�[���擾 */
extern const char* mana_stream_get_string_pointer(const mana_stream* self);

/*! ������̒������擾 */
extern size_t mana_stream_get_string_length(const mana_stream* self);

/*! �f�[�^���擾 */
extern void mana_stream_get_data(const mana_stream* self, void* pointer, const size_t size);

/*! �Q�ƃ|�C���^���ړ����܂� */
extern void mana_steram_seek(mana_stream* self, const int32_t offset);

/*! ���݂̃|�C���^�[���L�^���܂� */
extern void mana_stream_mark(mana_stream* self);

/*! ���݂̃|�C���^�[�� mana_stream_mark �����ʒu�����ׂ܂� */
extern void mana_stream_check(mana_stream* self);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
