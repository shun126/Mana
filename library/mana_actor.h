/*!
 * mana (library)
 *
 * @file	mana_actor.h
 * @brief	mana_actor�N���X�Ɋւ���w�b�_�[�t�@�C��
 * @detail
 * ���̃t�@�C����mana_actor�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * mana_actor�N���X�͍X�V�⃊�N�G�X�g�Ȃǂ̐���A�O���֐��̌Ăяo���Ȃǂ�
 * ���s����\���Ă��܂��B
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_ACTOR_H___)
#define ___MANA_ACTOR_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_FRAME_H___)
#include "mana_frame.h"
#endif
#if !defined(___MANA_HASH_H___)
#include "mana_hash.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif
#if !defined(___MANA_STACK_H___)
#include "mana_stack.h"
#endif
#if !defined(___MANA_STREAM_H___)
#include "mana_stream.h"
#endif

#define MANA_RETURN_VALUE_TYPE_INVALID	( 0)	/*!< �߂�l�F���� */
#define MANA_RETURN_VALUE_TYPE_ACTOR	(-1)	/*!< �߂�l�F�A�N�^�[�ւ̎Q�� */
#define MANA_RETURN_VALUE_TYPE_INTEGER	(-2)	/*!< �߂�l�F���� */
#define MANA_RETURN_VALUE_TYPE_FLOAT	(-3)	/*!< �߂�l�F���� */
#define MANA_RETURN_VALUE_TYPE_STRING	(-4)	/*!< �߂�l�F������ւ̎Q�� */

/*! mana_actor_callback�֐��ւ̖��߃^�C�v */
typedef enum mana_file_callback_command
{
	MANA_FILE_COMMAND_CLOSE,					/*!< �t�@�C������܂� */
	MANA_FILE_COMMAND_IS_READING				/*!< �t�@�C�����ǂݍ��ݒ����擾���܂� */
}mana_file_callback_command;

/*! �t�@�C������R�[���o�b�N */
typedef void (mana_actor_file_callback)(mana_file_callback_command, void* parameter);

/*! ���荞�݃e�[�u�� */
typedef struct mana_interrupt
{
	struct mana_actor* sender;				/*!< �v������mana_actor �I�u�W�F�N�g */
	uint32_t address;						/*!< ���荞�ݐ�A�h���X */
	uint32_t return_address;				/*!< ���^�[���A�h���X */
	int32_t counter;								/*!< �ėp�J�E���^ */
	int32_t frame_pointer;							/*!< �t���[���|�C���^ */
	int32_t stack_pointer;							/*!< �X�^�b�N�|�C���^ */
	void* file_callback_parameter;			/*!< �t�@�C���I���R�[���o�b�N�p�����[�^ */
#if !defined(NDEBUG)
	const char* action_name;					/*!< ���s���̃A�N�V������ */
#endif
	uint8_t flag;
	/*
	 * mana_actor :: flags
	 *
	 *| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 *	|	|	|	|	|	|	|	|
	 *	|	|	|	|	|	|	|	+-- MANA_INTERRUPT_FLAG_INITIALIZED
	 *	|	|	|	|	|	|	+------ MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL
	 *	|	|	|	|	|	+---------- MANA_INTERRUPT_FLAG_REPEAT
	 *	|	|	|	|	+-------------- MANA_INTERRUPT_FLAG_SUSPEND
	 *	|	|	|	+------------------ MANA_INTERRUPT_FLAG_SYNCHRONIZED
	 *	|	|	+----------------------
	 *	|	+--------------------------
	 *	+------------------------------
	 */
}mana_interrupt;

#define MANA_INTERRUPT_FLAG_INITIALIZED		(0x01)/*!< �������ς݃t���O */
#define MANA_INTERRUPT_FLAG_IS_IN_SYNC_CALL	(0x02)/*!< �������s�t���O */
#define MANA_INTERRUPT_FLAG_REPEAT			(0x04)/*!< ���s�[�g�t���O */
#define MANA_INTERRUPT_FLAG_SUSPEND			(0x08)/*!< �T�X�y���h�t���O */
#define MANA_INTERRUPT_FLAG_SYNCHRONIZED	(0x10)/*!< �����t���O */

/*! �߂�l */
typedef struct mana_return_value
{
	union
	{
		int32_t integer_value;						/*!< �����l */
		float float_value;						/*!< �����l */
		const char* string_value;				/*!< ������ */
		void* pointer_value;					/*!< �\���� */
		struct mana_actor* actor_value;		/*!< �A�N�^�[ */
	} values;
	int32_t size;									/*!< �T�C�Y(pointer_value) */
}mana_return_value;

/*!
 * @brief	mana_actor�N���X
 * mana_actor�N���X�̓A�N�^�[��\���Ă��܂��B
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana_actor
{
#if !defined(NDEBUG)
	char* name;													/*!< �A�N�^�[�� */
#endif
	struct mana* parent;										/*!< ��������Mana�I�u�W�F�N�g�ւ̃|�C���^ */
	mana_frame frame;											/*!< �t���[�������� */
	mana_stack stack;											/*!< �X�^�b�N */
	mana_hash actions;											/*!< �A�N�V����Hash */
	mana_interrupt interrupt[MANA_ACTOR_MAX_INTERRUPT_LEVEL];	/*!< ���荞�݃e�[�u�� */
	mana_return_value return_value;								/*!< �߂�l */
	void* request_callback_parameter;							/*!< ���N�G�X�g�R�[���o�b�N */
	void* rollback_callback_parameter;							/*!< ���[���o�b�N�R�[���o�b�N */
	void* variable;												/*!< �C���X�^���X�ϐ� */
	size_t variable_size;										/*!< �C���X�^���X�ϐ��̃T�C�Y */
	uint32_t pc;											/*!< �v���O�����J�E���^ */
	uint8_t interrupt_level;								/*!< ���荞�݃��x�� */
	uint8_t flag;
	/*
	 * mana_actor :: flags
	 *
	 *| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 *	|	|	|	|	|	|	|	|
	 *	|	|	|	|	|	|	|	+-- MANA_ACTOR_FLAG_BATTLER
	 *	|	|	|	|	|	|	+------ MANA_ACTOR_FLAG_CHARACTER
	 *	|	|	|	|	|	+---------- MANA_ACTOR_FLAG_HALT
	 *	|	|	|	|	+-------------- MANA_ACTOR_FLAG_RUNNING
	 *	|	|	|	+------------------ MANA_ACTOR_FLAG_TOUCHED
	 *	|	|	+---------------------- MANA_ACTOR_FLAG_HIDED
	 *	|	+-------------------------- MANA_ACTOR_FLAG_REQUESTED
	 *	+------------------------------ MANA_ACTOR_FLAG_REFUSED
	 */
	uint16_t padding;
	uint32_t user_data;										/*!< ���[�U�[�f�[�^ */
	void* user_pointer;											/*!< ���[�U�[�|�C���^�[ */
}mana_actor;

#define MANA_ACTOR_FLAG_BATTLER		(0x01)		/*!< �퓬�Q���t���O */
#define MANA_ACTOR_FLAG_CHARACTER	(0x02)		/*!< �L�����N�^�[�t���O */
#define MANA_ACTOR_FLAG_HALT			(0x04)		/*!< ��~�t���O */
#define MANA_ACTOR_FLAG_RUNNING		(0x08)		/*!< ���s�t���O */
#define MANA_ACTOR_FLAG_TOUCHED		(0x10)		/*!< �ڐG�t���O */
#define MANA_ACTOR_FLAG_HIDED			(0x20)		/*!< ��\���t���O */
#define MANA_ACTOR_FLAG_REQUESTED	(0x40)		/*!< ���N�G�X�g�t���O */
#define MANA_ACTOR_FLAG_REFUSED		(0x80)		/*!< ���N�G�X�g�֎~�t���O */

/*! request / rollback�R�[���o�b�N */
typedef int32_t (mana_actor_callback)(mana_actor* actor, void* parameter);

/*! �O���֐��錾�p�^ */
typedef void (mana_external_funtion_type)(mana_actor* actor);

/*! mana_actor �I�u�W�F�N�g�̐��� */
extern mana_actor* mana_actor_create(struct mana* parent, const size_t variable_size);

/*! mana_actor �I�u�W�F�N�g�̃R�s�[ */
extern mana_actor* mana_actor_clone(mana_actor* actor);

/*! mana_actor �I�u�W�F�N�g�̔j�� */
extern void mana_actor_destroy(mana_actor* self);

/*! mana_actor �I�u�W�F�N�g�̃V���A���C�Y */
extern void mana_actor_serialize(mana_actor* self, mana_stream* stream);

/*! mana_actor �I�u�W�F�N�g�̃f�V���A���C�Y */
extern void mana_actor_deserialize(mana_actor* self, mana_stream* stream);

/*! �o�ߎ��Ԃ��擾 */
extern float mana_actor_get_delta_time(void);

/*! �o�ߎ��Ԃ�ݒ� */
extern void mana_actor_set_delta_time(float second);

/*! �X�V */
extern int32_t mana_actor_run(mana_actor* self);

/*! �������s */
extern bool mana_sync_call(mana_actor* self, const int32_t level, const char* action, mana_actor* sender);

/*! �񓯊����s */
extern bool mana_async_call(mana_actor* self, const int32_t level, const char* action, mana_actor* sender);

/*! ���N�G�X�g */
extern bool mana_actor_request(mana_actor* self, const int32_t level, const char* name, mana_actor* sender);

/*! ���[���o�b�N */
extern void mana_actor_rollback(mana_actor* self, int32_t level);

/*! �ċN�� */
extern void mana_actor_restart(mana_actor* self);

/*! ���O���擾 */
extern const char* mana_actor_get_name(mana_actor* self);

/*! �A�N�V�����̃v���O�����J�n�A�h���X���擾 */
extern uintptr_t mana_actor_get_action(mana_actor* self, const char* name);

/*! �ėp�J�E���^�[���擾 */
extern int32_t mana_actor_get_counter(mana_actor* self);

/*! �ėp�J�E���^�[��ݒ� */
extern void mana_actor_set_counter(mana_actor* self, const int32_t counter);

/*! �����̐����擾 */
extern int32_t mana_actor_get_argument_count(mana_actor* self);

/*! �����̐����擾 */
extern int32_t mana_actor_get_argument_count_by_address(mana_actor* self, const uint32_t address);

/*! �����̃T�C�Y���擾 */
extern int32_t mana_actor_get_argument_size(mana_actor* self, const uint32_t address);

/*! �߂�l�����݂��邩���ׂ� */
extern bool mana_actor_has_return_value(mana_actor* self, const uint32_t address);

/*! �����^�̈������擾 */
extern int32_t mana_actor_get_parameter_integer(mana_actor* self, const int32_t value);

/*! �����^�̈������擾 */
extern float mana_actor_get_parameter_float(mana_actor* self, const int32_t value);

/*! ������^�̈������擾 */
extern const char* mana_actor_get_parameter_string(mana_actor* self, const int32_t value);

/*! �A�N�^�[�^�̈������擾 */
extern mana_actor* mana_actor_get_parameter_actor(mana_actor* self, const int32_t value);

/*! �|�C���^�[�^�̈������擾 */
extern void* mana_actor_get_parameter_pointer(mana_actor* self, const int32_t value);

/*! �|�C���^�[�^�̈������擾 */
extern void* mana_actor_get_parameter_address(mana_actor* self, const int32_t value);

/*! �����^�̖߂�l��ݒ� */
extern void mana_actor_set_return_integer(mana_actor* self, const int32_t value);

/*! �����^�̖߂�l��ݒ� */
extern void mana_actor_set_return_float(mana_actor* self, const float value);

/*! ������^�̖߂�l��ݒ� */
extern void mana_actor_set_return_string(mana_actor* self, const char* string);

/*! �A�N�^�[�^�̖߂�l��ݒ� */
extern void mana_actor_set_return_actor(mana_actor* self, mana_actor* actor);

/*! �|�C���^�[�^�̖߂�l��ݒ� */
extern void mana_actor_set_return_pointer(mana_actor* self, void* pointer);

/*! �f�[�^�̖߂�l��ݒ� */
extern void mana_actor_set_return_data(mana_actor* self, const void* pointer, const int32_t size);

#if !defined(NDEBUG)
/*! ���s���̃A�N�V���������擾 */
const char* mana_actor_get_action_name(const mana_actor* self);

/*! ���s���̊O���֐������擾 */
const char* mana_actor_get_function_name(const mana_actor* self);
#endif

/*! mana_actor �I�u�W�F�N�g�ɃA�N�V������o�^(protected) */
extern void mana_actor_set_action(mana_actor* self, const char* name, uint8_t* address);

/*! mana �I�u�W�F�N�g���擾���܂� */
extern struct mana* mana_actor_get_parent(const mana_actor* self);

/*! �������t���O���擾 */
extern bool mana_actor_is_init(const mana_actor* self);

/*! ���s�[�g�t���O���擾 */
extern bool mana_actor_is_repeat(const mana_actor* self);

/*! ���s�t���O���擾 */
extern bool mana_actor_is_running(const mana_actor* self);

/*! ���s���̖��߂𒆒f���Ď��̃t���[���ɍĎ��s���܂� */
extern void mana_actor_repeat(mana_actor* self, const bool initial_complete);

/*! ���s���̖��߂𒆒f���ē����t���[�����ɍĎ��s���܂� */
extern void mana_actor_again(mana_actor* self);

/*! ���s��~ */
extern void mana_actor_halt(mana_actor* self);

/*! ���s��~ */
extern void mana_actor_stop(mana_actor* self);

/*! ���̃t���[���܂Ŏ��s���f */
extern void mana_actor_yield(mana_actor* self);

/*! ���N�G�X�g������ */
extern void mana_actor_comply(mana_actor* self);

/*! ���N�G�X�g������ */
extern void mana_actor_refuse(mana_actor* self);

/*! �D��x���擾 */
extern int32_t mana_actor_get_interrupt_level(mana_actor* self);

/*! �����t���O���擾 */
extern bool mana_actor_is_synchronized(mana_actor* self);

/*! �����t���O��ݒ� */
extern void mana_actor_set_synchronized(mana_actor* self, bool synchronized);

/*! �D��x�t�������t���O�̐ݒ� */
extern void mana_actor_set_synchronized_with_level(mana_actor* self, int32_t level, bool synchronized);

/*! ���\�[�X�f�[�^�̎擾 */
extern void mana_actor_get_data(const mana_actor* self, const int32_t resouce_id, const void** buffer, size_t* size);

/*! �t�@�C���ǂݍ��݃R�[���o�b�N�̎擾 */
extern mana_actor_file_callback* mana_actor_get_file_callback(void);

/*! �t�@�C���ǂݍ��݃R�[���o�b�N�̐ݒ� */
extern void mana_actor_set_file_callback(mana_actor_file_callback* function);

/*! �t�@�C���ǂݍ��݃R�[���o�b�N�p�����[�^�[�̎擾 */
extern void* mana_actor_get_file_callback_parameter(mana_actor* self, int32_t level);

/*! �t�@�C���ǂݍ��݃R�[���o�b�N�p�����[�^�[�̐ݒ� */
extern void mana_actor_set_file_callback_parameter(mana_actor* self, int32_t level, void* parameter);

/*! ���N�G�X�g�R�[���o�b�N�̐ݒ� */
extern void mana_actor_set_request_callback(mana_actor_callback* function);

/*! ���N�G�X�g�R�[���o�b�N�p�����[�^�[�̐ݒ� */
extern void mana_actor_set_request_callback_parameter(mana_actor* self, void* parameter);

/*! ���[���o�b�N�R�[���o�b�N�̐ݒ� */
extern void mana_actor_set_rollback_callback(mana_actor_callback* function);

/*! ���[���o�b�N�R�[���o�b�N�p�����[�^�[�̐ݒ� */
extern void mana_actor_set_rollback_callback_parameter(mana_actor* self, void* parameter);

/*!< ���[�U�[�f�[�^�̎擾 */
extern uint32_t mana_actor_get_user_data(const mana_actor* self);

/*!< ���[�U�[�f�[�^�̐ݒ� */
extern void mana_actor_set_user_data(mana_actor* self, uint32_t user_data);

/*!< ���[�U�[�|�C���^�[�̎擾 */
extern void* mana_actor_get_user_pointer(const mana_actor* self);

/*!< ���[�U�[�|�C���^�[�̐ݒ� */
extern void mana_actor_set_user_pointer(mana_actor* self, void* user_pointer);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
