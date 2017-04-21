/*!
mana (library)

@file	mana_stream.h
@brief	mana_stream�N���X�Ɋւ���w�b�_�[�t�@�C��
@detail
���̃t�@�C����mana_stream�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
mana_stream�N���X�̓������X�g���[�~���O�Ɋւ��鑀����s�Ȃ��܂��B

@author	Shun Moriya
@date	2003-
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
@brief	mana_stream�N���X

mana_stream�N���X�̓������X�g���[�~���O��\���Ă��܂��B

C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
*/
typedef struct mana_stream
{
	char* buffer;			//!< �o�b�t�@
	size_t allocated_size;	//!< �m�ۍς݃T�C�Y
	size_t used_size;		//!< �g�p�ς݃T�C�Y
	size_t pointer;			//!< �|�b�v��I�t�Z�b�g�A�h���X
}mana_stream;

/*!
mana_stream �I�u�W�F�N�g�̐���
@return	mana_stream �I�u�W�F�N�g
*/
extern mana_stream* mana_stream_create(void);

/*!
mana_stream �I�u�W�F�N�g�̔j��
@param[in]	self	mana_stream �I�u�W�F�N�g
*/
extern void mana_stream_destroy(mana_stream* self);

/*!
�t�@�C������f�[�^��ǂݍ���
@param[in]	self		mana_stream �I�u�W�F�N�g
@param[in]	filename	�t�@�C����
*/
bool mana_stream_load(mana_stream* self, const char* filename);

/*
�t�@�C���փf�[�^����������
@param[in]	self		mana_stream �I�u�W�F�N�g
@param[in]	filename	�t�@�C����
*/
bool mana_stream_save(const mana_stream* self, const char* filename);

/*!
�o�b�t�@�̐擪�A�h���X���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�o�b�t�@�[�̐擪�A�h���X
*/
extern void* mana_stream_get_buffer(const mana_stream* self);

/*!
�o�b�t�@�̃A�h���X���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	index	�C���f�b�N�X�ԍ�
@return		�o�b�t�@�[�̃A�h���X
*/
extern void* mana_stream_index_at(const mana_stream* self, const size_t index);

/*!
�m�ۍς݃o�b�t�@�T�C�Y���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�o�b�t�@�[�̊m�ۍς݃T�C�Y
*/
extern size_t mana_stream_get_allocated_size(const mana_stream* self);

/*!
�g�p���Ă���o�b�t�@�T�C�Y���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�o�b�t�@�[�̎g�p���T�C�Y
*/
extern size_t mana_stream_get_used_size(const mana_stream* self);

/*!
int8_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_char(mana_stream* self, const int8_t value);

/*!
int16_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_short(mana_stream* self, const int16_t value);

/*!
int32_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_integer(mana_stream* self, const int32_t value);

/*!
int64_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_long(mana_stream* self, const int64_t value);

/*!
uint8_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_unsigned_char(mana_stream* self, const uint8_t value);

/*!
uint16_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_unsigned_short(mana_stream* self, const uint16_t value);

/*!
uint32_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_unsigned_integer(mana_stream* self, const uint32_t value);

/*!
uint64_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_unsigned_long(mana_stream* self, const uint64_t value);

/*!
size_t�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_size(mana_stream* self, const size_t value);

/*!
float�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_float(mana_stream* self, const float value);

/*!
double�T�C�Y�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	value	�l
*/
extern void mana_stream_push_double(mana_stream* self, const double value);

/*!
��������v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	text	������
*/
extern void mana_stream_push_string(mana_stream* self, const char* text);

/*!
�f�[�^���v�b�V��
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	pointer	�f�[�^�̐擪�A�h���X
@param[in]	size	�f�[�^�̃T�C�Y
*/
extern void mana_stream_push_data(mana_stream* self, const void* pointer, const size_t size);

/*!
pop�|�C���^�������߂�
@param[in]	self	mana_stream �I�u�W�F�N�g
*/
extern void mana_stream_rewind(mana_stream* self);

/*!
int8_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int8_t mana_stream_pop_char(mana_stream* self);

/*!
int16_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int16_t mana_stream_pop_short(mana_stream* self);

/*!
int32_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int32_t mana_stream_pop_integer(mana_stream* self);

/*!
int64_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int64_t mana_stream_pop_long(mana_stream* self);

/*!
uint8_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint8_t mana_stream_pop_unsigned_char(mana_stream* self);

/*!
uint16_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint16_t mana_stream_pop_unsigned_short(mana_stream* self);

/*!
uint32_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint32_t mana_stream_pop_unsigned_integer(mana_stream* self);

/*!
uint64_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint64_t mana_stream_pop_unsigned_long(mana_stream* self);

/*!
size_t�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern size_t mana_stream_pop_size(mana_stream* self);

/*!
float�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern float mana_stream_pop_float(mana_stream* self);

/*!
double�T�C�Y�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern double mana_stream_pop_double(mana_stream* self);

/*!
��������|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[out]	pointer	�|�b�v��̃o�b�t�@�擪�A�h���X
@param[out]	size	�|�b�v��̃o�b�t�@�̃T�C�Y
*/
extern void mana_stream_pop_string(mana_stream* self, char* pointer, const size_t size);

/*!
�f�[�^���|�b�v
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[out]	pointer	�|�b�v��̃o�b�t�@�擪�A�h���X
@param[out]	size	�|�b�v����f�[�^�̃T�C�Y
*/
extern void mana_stream_pop_data(mana_stream* self, void* pointer, const size_t size);

/*!
int8_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int8_t mana_stream_get_char(const mana_stream* self);

/*!
int16_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int16_t mana_stream_get_short(const mana_stream* self);

/*!
int32_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int32_t mana_stream_get_integer(const mana_stream* self);

/*!
int64_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern int64_t mana_stream_get_long(const mana_stream* self);

/*!
uint8_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint8_t mana_stream_get_unsigned_char(const mana_stream* self);

/*!
uint16_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint16_t mana_stream_get_unsigned_short(const mana_stream* self);

/*!
uint32_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint32_t mana_stream_get_unsigned_integer(const mana_stream* self);

/*!
uint64_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern uint64_t mana_stream_get_unsigned_long(const mana_stream* self);

/*!
size_t�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern size_t mana_stream_get_size(const mana_stream* self);

/*!
float�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern float mana_stream_get_float(const mana_stream* self);

/*!
double�T�C�Y�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		�l
*/
extern double mana_stream_get_double(const mana_stream* self);

/*!
��������擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[out]	pointer	�擾��̃o�b�t�@�擪�A�h���X
@param[out]	size	�擾��̃o�b�t�@�̃T�C�Y
*/
extern void mana_stream_get_string(const mana_stream* self, char* pointer, const size_t size);

/*!
������̃|�C���^�[���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		������̃A�h���X
*/
extern const char* mana_stream_get_string_pointer(const mana_stream* self);

/*!
������̒������擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@return		������̒���
*/
extern size_t mana_stream_get_string_length(const mana_stream* self);

/*!
�f�[�^���擾
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[out]	pointer	�擾��̃o�b�t�@�擪�A�h���X
@param[out]	size	�擾����f�[�^�̃T�C�Y
*/
extern void mana_stream_get_data(const mana_stream* self, void* pointer, const size_t size);

/*!
�Q�ƃ|�C���^���ړ����܂�
@param[in]	self	mana_stream �I�u�W�F�N�g
@param[in]	offset	�I�t�Z�b�g
*/
extern void mana_steram_seek(mana_stream* self, const int32_t offset);

/*!
���݂̃|�C���^�[���L�^���܂�
@param[in]	self	mana_stream �I�u�W�F�N�g
*/
extern void mana_stream_mark(mana_stream* self);

/*!
���݂̃|�C���^�[�� mana_stream_mark �����ʒu�����ׂ܂�
@param[in]	self	mana_stream �I�u�W�F�N�g
*/
extern void mana_stream_check(mana_stream* self);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
