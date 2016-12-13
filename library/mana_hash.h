/*!
 * mana (compiler/library)
 *
 * @file	mana_hash.h
 * @brief	�n�b�V���Ɋւ���w�b�_�[�t�@�C��
 * @detail
 * ���̃t�@�C����mana_hash�N���X�Ɋ֌W����w�b�_�[�t�@�C���ł��B
 * mana_hash�N���X�͕�������L�[�ɂ����A�z�z��Ɋ֐�������s�Ȃ��܂��B
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_HASH_H___)
#define ___MANA_HASH_H___

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MANA_HASH_TABLE_SIZE	(31)	/*!< �n�b�V���e�[�u���T�C�Y(�f�����D�܂���) */

/*! mana_hash_table �̑o���������N */
typedef struct mana_hash_table_link
{
	struct mana_hash_table_link* previousChain;	/*!< �O�� mana_hash_table �ւ̃|�C���^ */
	struct mana_hash_table_link* nextChain;		/*!< ���� mana_hash_table �ւ̃|�C���^ */
}mana_hash_table_link;

/*! mana_hash�N���X�̃n�b�V���e�[�u�� */
typedef struct mana_hash_table
{
	mana_hash_table_link chain;		/*!< mana_hash_table �ւ̑o���������N */
	struct mana_hash_table* next;	/*!< ���̃n�b�V���e�[�u���ւ̃|�C���^ */
	const char* name;				/*!< �L�[ */
	void* data;						/*!< �l */
} mana_hash_table;

/*!
 * @brief	mana_hash�N���X
 * mana_hash�N���X�͕�������L�[�ɂ����A�z�z���\���Ă��܂��B
 *
 * C++�ł����Ƃ���̃����o�[�ϐ��ł����AC����Ȃ̂őS��public�ɂȂ��Ă��܂��B
 * ����p�̊֐���p�ӂ��Ă���̂ŁA�����̊֐��𗘗p���đ��삵�Ă��������B
 */
typedef struct mana_hash
{
	/*! �n�b�V���`�F�[��(�����p�L���b�V��) */
	mana_hash_table_link chain;
	/*! �n�b�V���`�F�[��(�Ō�̃A�N�Z�X) */
	mana_hash_table* last;
	/*! �n�b�V���e�[�u�� */
	mana_hash_table* table[MANA_HASH_TABLE_SIZE];
}mana_hash;

/*! mana_hash�N���X�̔����q */
typedef struct mana_hash_iterator
{
	mana_hash_table* iterator;			/*!< �Q�ƒ���mana_hash_table */
}mana_hash_iterator;

/*! mana_hash �I�u�W�F�N�g�̐��� */
extern mana_hash* mana_hash_create(void);

/*! mana_hash �I�u�W�F�N�g�̔j�� */
extern void mana_hash_destroy(mana_hash* self);

/*! mana_hash �I�u�W�F�N�g�̏����� */
extern void mana_hash_initialize(mana_hash* self);

/*! mana_hash �I�u�W�F�N�g�̊J�� */
extern void mana_hash_finalize(mana_hash* self);

/*! mana_hash �I�u�W�F�N�g�̔�r */
extern int32_t mana_hash_compare(const mana_hash* self, const mana_hash* other);

/*! mana_hash �I�u�W�F�N�g�̃R�s�[ */
extern void mana_hash_duplicate(mana_hash* self, const mana_hash* source);

/*! mana_hash �I�u�W�F�N�g�̃N���A */
extern void mana_hash_clear(mana_hash* self);

/*! �f�[�^������ */
extern void mana_hash_erase(mana_hash* self, const char* name);

/*! �f�[�^�̑��݃e�X�g */
extern int32_t mana_hash_test(mana_hash* self, const char* name);

/*! �f�[�^���擾 */
extern void* mana_hash_get(mana_hash* self, const char* name);

/*! �f�[�^��ݒ� */
extern void* mana_hash_set(mana_hash* self, const char* name, void* data);

/*! �C�e���[�^�i�����q�j�̊����߂� */
extern void mana_hash_iterator_rewind(mana_hash_iterator* iterator);

/*! �C�e���[�^�i�����q�j�����ֈړ� */
extern int32_t mana_hash_iterator_hash_next(const mana_hash* self, mana_hash_iterator* iterator);

/*! �C�e���[�^�i�����q�j���w���n�b�V���̃L�[���擾 */
extern const char* mana_hash_iterator_get_name(const mana_hash_iterator* iterator);

/*! �C�e���[�^�i�����q�j���w���n�b�V���̒l���擾 */
extern void* mana_hash_iterator_get_value(const mana_hash_iterator* iterator);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
