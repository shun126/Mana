/*!
 * mana (library)
 *
 * @file	mana_info.h
 * @brief	mana�R���p�C����mana���C�u�����ŋ��ʗ��p����w�b�_�[�t�@�C��
 * @detail	���̃t�@�C����mana�R���p�C����mana���C�u�����ŋ��ʗ��p����w�b�_�[�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_INFO_H___)
#define ___MANA_INFO_H___

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MANA_SIGNATURE					"MaNa"		/*!< mana�t�@�C������ */
#define MANA_MAJOR_VERSION				(0)			/*!< mana�t�@�C���̃��W���[�o�[�W�����ԍ� */
#define MANA_MINOR_VERSION				(12)		/*!< mana�t�@�C���̃}�C�i�[�o�[�W�����ԍ� */
#define MANA_ACTOR_MAX_INTERRUPT_LEVEL	(32)		/*!< �D��x�� */
#define MANA_FUNCTION_HASH_SIZE			(127)		/*!< �O���֐��n�b�V���e�[�u���T�C�Y(�f�����D�܂���) */
#define MANA_REGISTER_COUNT				(8)			/*!< ���W�X�^�̐� */

/*! @brief	�� */
#define MANA_PI (3.1415926535897932384626433832795028841971693993751f)

/*! mana�t�@�C���w�b�_�[ */
typedef struct mana_file_header
{
	int8_t header[4];									/*!< ���� */
	uint8_t major_version;					/*!< ���W���[�o�[�W���� */
	uint8_t minor_version;					/*!< �}�C�i�[�o�[�W���� */
	uint8_t pad;								/*!< �p�f�B���O */
	uint8_t flag;								/*!< �t���O */
/*
mana_file_header :: flag

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  |   |   |   |   |   |   |   |
  |   |   |   |   |   |   |   +---	MANA_HEADER_FLAG_RESOURCE
  |   |   |   |   |   |   +-------	MANA_HEADER_FLAG_64BIT
  |   |   |   |   |   +-----------  MANA_HEADER_FLAG_BIG_ENDIAN
  |   |   |   |   +---------------
  |   |   |   +-------------------
  |   |   +-----------------------
  |   +---------------------------
  +-------------------------------	MANA_HEADER_FLAG_COMPILED
*/
#define MANA_HEADER_FLAG_RESOURCE	0x01			/*!< �t���O�F���\�[�X�Z�N�V�������� */
#define MANA_HEADER_FLAG_64BIT		0x02			/*!< �t���O�F64�r�b�g�ŃR���p�C�����ꂽ */
#define MANA_HEADER_FLAG_BIG_ENDIAN	0x04			/*!< �t���O�F�r�b�N�G���f�B�A���ŃR���p�C�����ꂽ */
#define MANA_HEADER_FLAG_COMPILED	0x80			/*!< �t���O�F�R���p�C���ς� */

	uint32_t number_of_actors;					/*!< �A�N�^�[�� */
	uint32_t size_of_constant_pool;				/*!< �萔�Z�N�V�����T�C�Y */
	uint32_t size_of_instruction_pool;			/*!< ���߃Z�N�V�����T�C�Y */
	uint32_t size_of_static_memory;				/*!< �X�^�e�B�b�N�������T�C�Y */
	uint32_t size_of_global_memory;				/*!< �O���[�o���������T�C�Y */
	uint32_t random_seed_number;				/*!< �����̎� */
}mana_file_header;

/*! mana�t�@�C�����̃A�N�^�[���w�b�_�[ */
typedef struct mana_actor_info_header
{
	uint8_t flag;								/*!< �t���O */
/*
mana_actor_info_header :: flag

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  |   |   |   |   |   |   |   |
  |   |   |   |   |   |   |   +---	MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM
  |   |   |   |   |   |   +-------
  |   |   |   |   |   +-----------
  |   |   |   |   +---------------
  |   |   |   +-------------------
  |   |   +-----------------------
  |   +---------------------------
  +-------------------------------
*/
#define MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM	0x01	/*!< �t���O�F�t�@���g�� */

	uint8_t number_of_instances;				/*!< �C���X�^���X���i���g�p�j */
	uint16_t number_of_actions;				/*!< �A�N�V������ */
	uint32_t name;								/*!< �A�N�^�[�� */
	uint32_t variable_size;						/*!< �C���X�^���X�ϐ��T�C�Y */
}mana_actor_info_header;

/*! mana�t�@�C�����̃A�N�V�������w�b�_�[ */
typedef struct mana_action_info_header
{
	uint32_t name;								/*!< �A�N�V������ */
	intptr_t address;							/*!< �A�h���X */
}mana_action_info_header;

/*!  */
enum mana_intermediate_language
{
	/* thread */
	MANA_IL_RESTART = 0,							/* 00 */
	MANA_IL_HALT,									/* 01 */
	MANA_IL_YIELD,									/* 02 */
	MANA_IL_NONPREEMPTIVE,							/* 03 */
	MANA_IL_PREEMPTIVE,								/* 04 */

	/* jump */
	MANA_IL_BEQ,									/* 05 */
	MANA_IL_BNE,									/* 06 */
	MANA_IL_BRA,									/* 07 */
	MANA_IL_BSR,									/* 08 */
	MANA_IL_CALL,									/* 09 */
	MANA_IL_REQ,									/* 0A */
	MANA_IL_REQWS,									/* 0B */
	MANA_IL_REQWE,									/* 0C */
	MANA_IL_JOIN,									/* 0D */
	MANA_IL_COMPLY,									/* 0E */
	MANA_IL_REFUSE,									/* 0F */
	MANA_IL_LOAD_RETURN_ADDRESS,					/* 10 */
	MANA_IL_SAVE_RETURN_ADDRESS,					/* 11 */
	MANA_IL_RETURN_FROM_FUNCTION,					/* 12 */
	MANA_IL_RETURN_FROM_ACTION,						/* 13 */
	MANA_IL_ROLLBACK,								/* 14 */

	/* constant */
	MANA_IL_PUSH_ZERO_INTEGER,						/* 15 */
	MANA_IL_PUSH_ZERO_FLOAT,						/* 16 */
	MANA_IL_PUSH_CHAR,								/* 17 */
	MANA_IL_PUSH_SHORT,								/* 18 */
	MANA_IL_PUSH_INTEGER,							/* 19 */
	MANA_IL_PUSH_FLOAT,								/* 1A */
	MANA_IL_PUSH_STRING,							/* 1B */
	MANA_IL_PUSH_PRIORITY,							/* 1C */
	MANA_IL_PUSH_ACTOR,								/* 1D */
	MANA_IL_PUSH_SELF,								/* 1E */
	MANA_IL_PUSH_SENDER,							/* 1F */

	/* stack */
	MANA_IL_ALLOCATE,								/* 20 */
	MANA_IL_FREE,									/* 21 */
	MANA_IL_DUPLICATE,								/* 22 */
	MANA_IL_DUPLICATE_DATA,							/* 23 */
	MANA_IL_REMOVE,									/* 24 */
	MANA_IL_REMOVE_DATA,							/* 25 */
	MANA_IL_LOAD_STATIC_ADDRESS,					/* 26 */
	MANA_IL_LOAD_GLOBAL_ADDRESS,					/* 27 */
	MANA_IL_LOAD_FRAME_ADDRESS,						/* 28 */
	MANA_IL_LOAD_SELF_ADDRESS,						/* 29 */

	/* memory operation */
	MANA_IL_LOAD_CHAR,								/* 2A */
	MANA_IL_LOAD_SHORT,								/* 2B */
	MANA_IL_LOAD_INTEGER,							/* 2C */
	MANA_IL_LOAD_FLOAT,								/* 2D */
	MANA_IL_LOAD_REFFRENCE,							/* 2E */
	MANA_IL_LOAD_DATA,								/* 2F */
	MANA_IL_STORE_CHAR,								/* 30 */
	MANA_IL_STORE_SHORT,							/* 31 */
	MANA_IL_STORE_INTEGER,							/* 32 */
	MANA_IL_STORE_FLOAT,							/* 33 */
	MANA_IL_STORE_REFFRENCE,						/* 34 */
	MANA_IL_STORE_DATA,								/* 35 */

	/* caluclation */
	MANA_IL_ADD_INTEGER,							/* 36 */
	MANA_IL_DIV_INTEGER,							/* 37 */
	MANA_IL_MINUS_INTEGER,							/* 38 */
	MANA_IL_MOD_INTEGER,							/* 39 */
	MANA_IL_MUL_INTEGER,							/* 3A */
	MANA_IL_SUB_INTEGER,							/* 3B */

	MANA_IL_ADD_FLOAT,								/* 3C */
	MANA_IL_DIV_FLOAT,								/* 3D */
	MANA_IL_MINUS_FLOAT,							/* 3E */
	MANA_IL_MOD_FLOAT,								/* 3F */
	MANA_IL_MUL_FLOAT,								/* 40 */
	MANA_IL_SUB_FLOAT,								/* 41 */

	MANA_IL_AND,									/* 42 */
	MANA_IL_EOR,									/* 43 */
	MANA_IL_LAND,									/* 44 */
	MANA_IL_LOR,									/* 45 */
	MANA_IL_LNOT,									/* 46 */
	MANA_IL_NOT,									/* 47 */
	MANA_IL_OR,										/* 48 */
	MANA_IL_SHL,									/* 49 */
	MANA_IL_SHR,									/* 4A */

	MANA_IL_INT2FLOAT,								/* 4B */
	MANA_IL_FLOAT2INT,								/* 4C */

	/* compare and branch */
	MANA_IL_COMPARE_EQ_INTEGER,						/* 4D */
	MANA_IL_COMPARE_NE_INTEGER,						/* 4E */
	MANA_IL_COMPARE_GE_INTEGER,						/* 4F */
	MANA_IL_COMPARE_GT_INTEGER,						/* 50 */
	MANA_IL_COMPARE_LE_INTEGER,						/* 51 */
	MANA_IL_COMPARE_LS_INTEGER,						/* 52 */

	MANA_IL_COMPARE_EQ_FLOAT,						/* 53 */
	MANA_IL_COMPARE_NE_FLOAT,						/* 54 */
	MANA_IL_COMPARE_GE_FLOAT,						/* 55 */
	MANA_IL_COMPARE_GT_FLOAT,						/* 56 */
	MANA_IL_COMPARE_LE_FLOAT,						/* 57 */
	MANA_IL_COMPARE_LS_FLOAT,						/* 58 */

	MANA_IL_COMPARE_EQ_DATA,						/* 59 */
	MANA_IL_COMPARE_NE_DATA,						/* 5A */
	MANA_IL_COMPARE_GE_DATA,						/* 5B */
	MANA_IL_COMPARE_GT_DATA,						/* 5C */
	MANA_IL_COMPARE_LE_DATA,						/* 5D */
	MANA_IL_COMPARE_LS_DATA,						/* 5E */

	/* utility */
	MANA_IL_PRINT,									/* 5F */

	/* under discussion */
	MANA_IL_DYNAMIC_REQ,							/* 60 */
	MANA_IL_DYNAMIC_REQWS,							/* 61 */
	MANA_IL_DYNAMIC_REQWE,							/* 62 */

	MANA_IL_MAX_SIZE
};

/*! ������������ */
extern void mana_srand(const uint32_t seed);

/*! �������擾 */
extern int32_t mana_rand(void);

/*! �O����P�܂ł̗��� */
#define mana_frand()	(((float)rand() & 0x7FFF) / 32767.0f)

/*! �p�x�����W�A���ɕϊ� */
#define mana_to_radian(degree)	(degree * (MANA_PI / 180.f))

/*! ���W�A�����p�x�ɕϊ� */
#define mana_to_degree(radian)	(radian * (180.0f / MANA_PI))

/*! �������̏�Ԏ擾 */
extern int32_t mana_is_big_endian(void);

/*! int16_t�^�̃G���f�B�A����ϊ����܂� */
extern int16_t mana_swap_endian_short(const int16_t value);

/*! uint16_t�^�̃G���f�B�A����ϊ����܂� */
extern uint16_t mana_swap_endian_unsigned_short(const uint16_t value);

/*! int32_t�^�̃G���f�B�A����ϊ����܂� */
extern int32_t mana_swap_endian_integer(const int32_t value);

/*! uint32_t�^�̃G���f�B�A����ϊ����܂� */
extern uint32_t mana_swap_endian_unsigned_integer(const uint32_t value);

/*! float�^�̃G���f�B�A����ϊ����܂� */
extern float mana_swap_endian_float(const float value);

/*! �R���\�[���ɕ�������o�͂��܂� */
extern void mana_print_debug(const char* format, ...);

/*! ������̒����當��������� */
extern int32_t mana_string_find(const int8_t text[], const int8_t pattern[]);

/*! �p�t�H�[�}���X�����}�C�N���b�Ŏ擾���܂� */
extern uint64_t mana_get_micro_secound(void);

/*! �t�@�C���T�C�Y���擾 */
extern long mana_get_file_size(const char* filename);

/*! �t�@�C����ǂݍ��݂܂�(�G���f�B�A���ϊ��ɂ͑Ή����Ă��܂���) */
extern int32_t mana_read(void** buffer, size_t* size, const char* filename);

/*! ���߂̃T�C�Y���擾 */
extern int32_t mana_get_instruction_size(const uint8_t* program);

/*! �I�y�R�[�h�ƃI�y�����h�̕�������擾 */
extern const char* mana_get_instruction_text(const char* data, const void* program, const int32_t address);

#if defined(_DEBUG)
/*! �R���\�[���ɕ�������o�� */
#define MANA_PRINT(...)		mana_print_debug(__VA_ARGS__)
/*! �R���\�[���ɕ�������o��(�f�o�b�O�r���h�̂�) */
#define MANA_TRACE(...)		mana_print_debug(__VA_ARGS__)
/*! �R���\�[���ɕ�������o�� */
#define MANA_WARNING(...)	{ mana_print_debug(__VA_ARGS__); }
/*! �R���\�[���ɕ�������o�͌㋭���I�� */
#define MANA_ERROR(...)		{ mana_print_debug(__VA_ARGS__); abort(); }
/*! �l�̐^�U�𒲂ׁA�U�Ȃ�x���\��(�f�o�b�O�r���h�̂�) */
#define MANA_CHECK(EXP)		if(!(EXP)){ mana_print_debug("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); }
/*! �l�̐^�U�𒲂ׁA�U�Ȃ狭���I��(�f�o�b�O�r���h�̂�) */
#define MANA_ASSERT(EXP)	if(!(EXP)){ mana_print_debug("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); abort(); }
/*! �l�̐^�U�𒲂ׁA�U�Ȃ狭���I��(�f�o�b�O�r���h�̂�) */
#define MANA_ASSERT_MESSAGE(EXP, m) if(!(EXP)){ mana_print_debug("%s: %s(%d): %s\n", #EXP, __FILE__, __LINE__, m); abort(); }
//! �����I��
#define MANA_BUG(m) { mana_print_debug("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#else
/*! �R���\�[���ɕ�������o�� */
#define MANA_PRINT(...)		printf(__VA_ARGS__)
/*! �R���\�[���ɕ�������o��(�f�o�b�O�r���h�̂�) */
#define MANA_TRACE(...)		((void)0)
/*! �R���\�[���ɕ�������o�� */
#define MANA_WARNING(...)	{ printf(__VA_ARGS__); }
/*! �R���\�[���ɕ�������o�͌㋭���I�� */
#define MANA_ERROR(...)		{ printf(__VA_ARGS__); abort(); }
/*! �l�̐^�U�𒲂ׁA�U�Ȃ�x���\��(�f�o�b�O�r���h�̂�) */
#define MANA_CHECK(EXP)		((void)0)
/*! �l�̐^�U�𒲂ׁA�U�Ȃ狭���I��(�f�o�b�O�r���h�̂�) */
#define MANA_ASSERT(EXP)	((void)0)
/*! �l�̐^�U�𒲂ׁA�U�Ȃ狭���I��(�f�o�b�O�r���h�̂�) */
#define MANA_ASSERT_MESSAGE(EXP, m) ((void)0)
//! �����I��
#define MANA_BUG(m) { printf("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#endif
/*! �l�̐^�U�𒲂ׁA�U�Ȃ狭���I�� */
#define MANA_VERIFY(EXP) \
if(!(EXP)){ \
	MANA_PRINT("%s(%d): (%s)\n", __FILE__, __LINE__, #EXP); \
	abort(); \
}
#define MANA_VERIFY_MESSAGE(EXP, ...) \
if(!(EXP)){ \
	MANA_PRINT("%s(%d): ", __FILE__, __LINE__); \
	MANA_PRINT(__VA_ARGS__); \
	MANA_PRINT(" (%s)\n", #EXP); \
	abort(); \
}
#define MANA_UNUSED_VAR(var)	((void)&var);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
