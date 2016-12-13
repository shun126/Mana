/*!
 * mana (library)
 *
 * @file	mana.h
 * @brief	mana�N���X�Ɋւ���w�b�_�[�t�@�C��
 * @detail
 * ���̃t�@�C����mana�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * mana�N���X�͍X�V�⃊�N�G�X�g�Ȃǂ̐���Amana_actor �I�u�W�F�N�g�̌����Ȃǂ�
 * ���s����\���Ă��܂��B
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_H___)
#define ___MANA_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_DEFINE_H___)
#include "mana_define.h"
#endif
#if !defined(___MANA_ACTOR_H___)
#include "mana_actor.h"
#endif
#if !defined(___MANA_ARRAY_H___)
#include "mana_array.h"
#endif
#if !defined(___MANA_DATALINK_H___)
#include "mana_datalink.h"
#endif
#if !defined(___MANA_HASH_H___)
#include "mana_hash.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif
#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*!
 * @brief	mana�N���X
 * mana�N���X�͎��s����\���Ă��܂��B
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana
{
#if 0
	struct mana_actor* map_actor;		/*!< �}�b�v��\��mana_actor �I�u�W�F�N�g */
	struct mana_actor* player_actor;	/*!< �v���C���[��\��mana_actor �I�u�W�F�N�g */
#endif
	mana_array actor_array;				/*!< mana_actor �I�u�W�F�N�g�ւ̔z�� */
	mana_hash actor_hash;				/*!< mana_actor �I�u�W�F�N�g�ւ̘A�z�z�� */
	mana_hash phantom_hash;				/*!< phantom��\��mana_actor �I�u�W�F�N�g�ւ̘A�z�z�� */
	mana_datalink datalink;				/*!< mana_datalink �I�u�W�F�N�g */
	uint8_t* global_memory;		/*!< �O���[�o���ϐ��i�[�G���A */
	mana_file_header* file_header;		/*!< ���[�h���ꂽ�t�@�C���ւ̃|�C���^ */
	uint8_t* instruction_pool;	/*!< ���[�h���ꂽ�v���O�����ւ̃|�C���^ */
	char* constant_pool;				/*!< ���[�h���ꂽ�f�[�^�ւ̃|�C���^ */
	uint32_t frame_counter;			/*!< �t���[���J�E���^ */

/*
	CManaIntersectionManager m_IntersectionManager;
	CManaResource m_Resource;
*/

	uint8_t flag;					/*!< �t���O */
	/*
	 * mana :: flags
	 *
	 *| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 *	|	|	|	|	|	|	|	|
	 *	|	|	|	|	|	|	|	+-- MANA_FLAG_AUTO_RELEASE
	 *	|	|	|	|	|	|	+------ MANA_FLAG_DEBUG_MODE
	 *	|	|	|	|	|	+---------- MANA_FLAG_FRAME_CHANGED
	 *	|	|	|	|	+-------------- MANA_FLAG_INIT_ACTION_RUNNING
	 *	|	|	|	+------------------ MANA_FLAG_INIT_ACTION_FINISHED
	 *	|	|	+---------------------- MANA_FLAG_INITIALIZED
	 *	|	+-------------------------- MANA_FLAG_REQUESTED
	 *	+------------------------------ MANA_FLAG_SYSTEM_REQUEST
	 */
}mana;

#define MANA_FLAG_AUTO_RELEASE			(0x01)	/*!< �v���O�����̎����J���t���O */
#define MANA_FLAG_DEBUG_MODE			(0x02)	/*!< �f�o�b�N���[�h�t���O */
#define MANA_FLAG_FRAME_CHANGED			(0x04)	/*!< �t���[���ύX�t���O */
#define MANA_FLAG_INIT_ACTION_RUNNING	(0x08)	/*!< init�A�N�V�������s���t���O */
#define MANA_FLAG_INIT_ACTION_FINISHED	(0x10)	/*!< init�A�N�V�����I���t���O */
#define MANA_FLAG_INITIALIZED			(0x20)	/*!< �������ς݃t���O */
#define MANA_FLAG_REQUESTED				(0x40)	/*!< ���N�G�X�g�����ς݃t���O */
#define MANA_FLAG_SYSTEM_REQUEST		(0x80)	/*!< �V�X�e�����N�G�X�g���t���O */

/*! mana �I�u�W�F�N�g�̐��� */
extern mana* mana_create(void);

/*! mana �I�u�W�F�N�g�̔j�� */
extern void mana_destroy(mana* self);

/*! mana �I�u�W�F�N�g�̃V���A���C�Y */
extern void mana_serialize(mana* self, mana_stream* stream);

/*! mana �I�u�W�F�N�g�̃f�V���A���C�Y */
extern void mana_deserialize(mana* self, mana_stream* stream);

/*! mana�v���O�����̓ǂݍ��� */
extern bool mana_load_program(mana* self, void* program, int32_t auto_release);

/*! mana�v���O�����̊J�� */
extern void mana_unload_program(mana* self);

/*! mana�v���O�����A�h���X�̎擾 */
extern void* mana_get_program_buffer(mana* self);

/*! �ċN�� */
extern void mana_restart(mana* self);

/*! �����X�V */
extern bool mana_run(mana* self, const float second);

/*! �񓯊��X�V */
extern void mana_execute(mana* self);

/*! ���s�������ׂ� */
extern bool mana_is_running(mana* self);

/*! �S�A�N�^�[�Ƀ��N�G�X�g��v�� */
extern void mana_request_all(mana* self, int32_t level, const char* action_name, mana_actor* sender);

/*! �A�N�^�[�Ƀ��N�G�X�g��v�� */
extern bool mana_request(mana* self, int32_t level, const char* actor_name, const char* action_name, mana_actor* sender);

/*! ���������� */
extern void mana_yield(mana* self);

extern int8_t mana_get_char(const mana* self, const uint8_t* address);
extern int16_t mana_get_short(const mana* self, const uint8_t* address);
extern int32_t mana_get_integer(const mana* self, const uint8_t* address);
extern uint8_t mana_get_unsigned_char(const mana* self, const uint8_t* address);
extern uint16_t mana_get_unsigned_short(const mana* self, const uint8_t* address);
extern uint32_t mana_get_unsigned_integer(const mana* self, const uint8_t* address);
extern float mana_get_float(const mana* self, const uint8_t* address);
extern const char* mana_get_string(const mana* self, const uint8_t* address);
extern uint8_t* mana_get_address(const mana* self, const uint8_t* address);

/*! �A�N�^�[�I�u�W�F�N�g���擾 */
extern struct mana_actor* mana_get_actor(mana* self, const char* name);

/*! �A�N�^�[�̖��O���擾 */
extern const char* mana_get_actor_name(mana* self, struct mana_actor* actor);

/*! �A�N�^�[�̃R�s�[ */
extern struct mana_actor* mana_create_actor(mana* self, struct mana_actor* actor, const char* new_name);

/*! �A�N�^�[�̃R�s�[ */
extern struct mana_actor* mana_create_actor_name(mana* self, const char* name, const char* new_name);

/*! phantom�����ɃA�N�^�[�𐶐� */
extern struct mana_actor* mana_create_actor_from_phantom(mana* self, const char* name, const char* new_name);

/*! init�A�N�V�������s�����擾 */
extern bool mana_is_in_init_action(mana* self);

/*! init�A�N�V�����I�����擾 */
extern bool mana_is_finish_init_action(mana* self);

/*! �V�X�e�����N�G�X�g�̋��t���O��ݒ� */
extern void mana_enable_system_request(mana* self, const bool enable);

/*! �V�X�e�����N�G�X�g�̋��t���O���擾 */
extern bool mana_is_system_request_enabled(mana* self);

/*! �t���[���J�E���^�[���擾 */
extern uint32_t mana_get_frame_counter(mana* self);

/*! �t���[�����ύX���ꂽ���擾 */
extern bool mana_is_frame_changed(mana* self);

/*! ���\�[�X�f�[�^���擾 */
extern void mana_get_data(const mana* self, const int32_t resouce_id, const void** buffer, size_t* size);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
