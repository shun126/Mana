/*!
 * mana (compiler/library)
 *
 * @file	mana_datalink.h
 * @brief	mana_datalink�N���X�Ɋւ���w�b�_�[�t�@�C��
 * @detail
 * ���̃t�@�C����mana_datalink�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * mana�R���p�C���̓X�N���v�g��alias�w�肳��A���Q�Ƃ���Ă��郊�\�[�X��
 * �������܂��B
 * mana_datalink�N���X�͌������ꂽ�f�[�^���Q�Ƃ���@�\�������Ă��܂��B
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_DATALINK_H___)
#define ___MANA_DATALINK_H___

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MANA_DATALINK_STANDARD_ALIGNMENT_SIZE	(sizeof(void*))

/*! datalink�t�@�C���̃w�b�_�[ */
typedef struct mana_datalink_file_header
{
	uint32_t total_data_size;	/*!< �S�̃T�C�Y */
	int32_t number_of_datas;			/*!< �f�[�^�̌� */
} mana_datalink_file_header;

/*! datalink�t�@�C�����̃f�[�^�̏�� */
typedef struct mana_datalink_info_header
{
	uint32_t offset;			/*!< �擪����̃I�t�Z�b�g */
	uint32_t size;				/*!< �f�[�^�̃T�C�Y */
} mana_datalink_info_header;

/*! mana_datalink */
/*!
 * @brief	mana_datalink�N���X
 * mana_datalink�N���X�͌������ꂽ�f�[�^���Q�Ƃ��܂��B
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana_datalink
{
	/*! datalink�t�@�C���̃w�b�_�[ */
	mana_datalink_info_header* datalink_info_header_pointer;
	uint8_t* data_pointer;	/*!< �f�[�^�Z�N�V�����擪�A�h���X */
	int32_t number_of_entries;			/*!< �f�[�^�̌� */
} mana_datalink;

/*! mana_datalink �I�u�W�F�N�g�̐��� */
extern mana_datalink* mana_datalink_create(void);

/*! mana_datalink �I�u�W�F�N�g�̔j�� */
extern void mana_datalink_destroy(mana_datalink* self);

/*! mana_datalink �I�u�W�F�N�g�̏����� */
extern void mana_datalink_initialize(mana_datalink* self);

/*! mana_datalink �I�u�W�F�N�g�̏I���� */
extern void mana_datalink_finalize(mana_datalink* self);

/*! datalink�t�@�C���̓ǂݍ��� */
extern void mana_datalink_load(mana_datalink* self, const void* buffer);

/*! datalink�t�@�C���̊J�� */
extern void mana_datalink_release(mana_datalink* self);

/*! datalink�t�@�C�����̃f�[�^�����擾 */
extern int32_t mana_datalink_get_number_of_datas(const mana_datalink* self);

/*! �f�[�^�̃T�C�Y���擾 */
extern int32_t mana_datalink_get_data_size(const mana_datalink* self, const int32_t index);

/*! �f�[�^�̃A�h���X���擾 */
extern const void* mana_datalink_get_data(const mana_datalink* self, const int32_t index);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
