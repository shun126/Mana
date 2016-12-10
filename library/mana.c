/*!
 * mana (library)
 *
 * @file	mana.c
 * @brief	mana�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana�N���X�͍X�V�⃊�N�G�X�g�Ȃǂ̐���Amana_actor �I�u�W�F�N�g�̌����Ȃǂ�
 * ���s����\���Ă��܂��B
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___LIBMANA_H___)
#include "libmana.h"
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>

#if defined(_MSC_VER)
#include <windows.h>
#define alloca(size)	_alloca(size)
#else
#include <alloca.h>
#include <sched.h>
#endif

extern mana_hash mana_external_function_hash;
extern mana_bool mana_big_endian;

static void mana_yield_thread(void)
{
#if defined(_MSC_VER)
	SwitchToThread();
#else
	sched_yield();
/*
	pthread_yield();
	pthread_yield_np();
*/
#endif
}

/*!
 * @return	mana �I�u�W�F�N�g
 */
mana* mana_create(void)
{
	mana* self = mana_malloc(sizeof(mana));
#if 0
	self->map_actor = NULL;
	self->player_actor = NULL;
#endif
	mana_array_initialize(&self->actor_array, 0);
	mana_hash_initialize(&self->actor_hash);
	mana_hash_initialize(&self->phantom_hash);
	mana_datalink_initialize(&self->datalink);
	self->global_memory = NULL;
	self->file_header = NULL;
	self->frame_counter = 0;
	self->instruction_pool = NULL;
	self->constant_pool = NULL;

	self->flag = MANA_FLAG_FRAME_CHANGED;

	return self;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 */
void mana_destroy(mana* self)
{
	if(self)
	{
		mana_unload_program(self);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[out]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_serialize(mana* self, mana_stream* stream)
{
	mana_hash_iterator iterator;

	assert(self && stream);

	{
		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor_serialize(mana_hash_iterator_get_value(&iterator), stream);
		}
/*
actor_array
 */
	}
#if 0
	mana_stream_push_string(stream, mana_get_actor_name(self, self->map_actor));
	mana_stream_push_string(stream, mana_get_actor_name(self, self->player_actor));
#endif
	mana_stream_push_integer(stream, self->frame_counter);

	mana_stream_push_unsigned_char(stream, self->flag);
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_deserialize(mana* self, mana_stream* stream)
{
	mana_hash_iterator iterator;

	assert(self && stream);

	{
		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor_deserialize(mana_hash_iterator_get_value(&iterator), stream);
		}
	}
	/*
actor_array
	 */
#if 0
	{
		size_t size = mana_stream_get_string_length(stream) + 1;
		void* string = alloca(size);

		mana_stream_pop_string(stream, string, size);

		self->map_actor = mana_get_actor(self, string);
	}

	{
		size_t size = mana_stream_get_string_length(stream) + 1;
		void* string = alloca(size);

		mana_stream_pop_string(stream, string, size);

		self->player_actor = mana_get_actor(self, string);
	}
#endif
	self->frame_counter = mana_stream_pop_integer(stream);

	self->flag = mana_stream_pop_unsigned_char(stream);
}

/*!
 * �v���O�������s���̓v���O�����̈���Q�Ƃ��Ă���̂ŁA
 * void mana_unload_program(mana* self)�������
 * �v���O�����̈���J�����Ȃ��ł��������B
 * @param[in]	program			mana�v���O�����A�h���X
 * @param[in]	auto_release	MANA_TRUE�Ȃ�Ύ����I�ɃR�[�h�̈���J�����܂�
 * @retval		MANA_TRUE			�ǂݍ��ݐ���
 * @retval		MANA_FALSE			�ǂݍ��ݎ��s
 */
mana_bool mana_load_program(mana* self, void* program, int auto_release)
{
	mana_actor_info_header* actor_info;
	unsigned int i;

	assert(self);
	assert(program);

	/* �v���O�������J�����܂� */
	mana_unload_program(self);

	if((int)program % MANA_DATALINK_STANDARD_ALIGNMENT_SIZE)
	{
		MANA_ERROR("The program address is NOT aligned on %d-byte boundaries.\n", MANA_DATALINK_STANDARD_ALIGNMENT_SIZE);
		goto ABORT;
	}

	self->file_header = (mana_file_header*)program;
	if(!self->file_header || memcmp(MANA_SIGNATURE, self->file_header->header, sizeof(self->file_header->header)) != 0)
	{
		MANA_ERROR("abnormal mana program loaded.\n");
		goto ABORT;
	}
	if(self->file_header->major_version != MANA_MAJOR_VERSION || self->file_header->minor_version != MANA_MINOR_VERSION)
	{
		MANA_ERROR("file version error.\n");
		goto ABORT;
	}

	/* �X�^�e�B�b�N�ϐ��̈���m�ۂ��܂� */
	mana_reallocate_static_variables(self->file_header->size_of_static_memory);

	/* �O���[�o���ϐ��̈���m�ۂ��܂� */
	self->global_memory = (unsigned char*)mana_malloc(self->file_header->size_of_global_memory);

	actor_info = (mana_actor_info_header*)(self->file_header + 1);

	for(i = 0; i < self->file_header->number_of_actors; i++)
	{
		mana_action_info_header* action_info = (mana_action_info_header*)(actor_info + 1);
		int j;

		for(j = 0; j < actor_info->number_of_actions; j++)
		{
			action_info++;
		}

		actor_info = (mana_actor_info_header*)action_info;
	}

	self->constant_pool = (char*)actor_info;

	self->instruction_pool = (unsigned char*)self->constant_pool + self->file_header->size_of_constant_pool;

	if(!(self->file_header->flag & MANA_HEADER_FLAG_COMPILED))
	{
		unsigned char* constant_pool;
		unsigned int program_counter;
		unsigned int i;

		constant_pool = (unsigned char*)self->constant_pool;

		program_counter = 0;
		while(program_counter < self->file_header->size_of_instruction_pool)
		{
			unsigned char opecode = self->instruction_pool[program_counter];
			switch(opecode)
			{
			case MANA_IL_BEQ:
			case MANA_IL_BNE:
			case MANA_IL_BRA:
			case MANA_IL_BSR:
			case MANA_IL_COMPARE_EQ_INTEGER:
			case MANA_IL_COMPARE_NE_INTEGER:
			case MANA_IL_COMPARE_GE_INTEGER:
			case MANA_IL_COMPARE_GT_INTEGER:
			case MANA_IL_COMPARE_LE_INTEGER:
			case MANA_IL_COMPARE_LS_INTEGER:
			case MANA_IL_COMPARE_EQ_FLOAT:
			case MANA_IL_COMPARE_NE_FLOAT:
			case MANA_IL_COMPARE_GE_FLOAT:
			case MANA_IL_COMPARE_GT_FLOAT:
			case MANA_IL_COMPARE_LE_FLOAT:
			case MANA_IL_COMPARE_LS_FLOAT:
			case MANA_IL_COMPARE_EQ_DATA:
			case MANA_IL_COMPARE_NE_DATA:
			case MANA_IL_COMPARE_GE_DATA:
			case MANA_IL_COMPARE_GT_DATA:
			case MANA_IL_COMPARE_LE_DATA:
			case MANA_IL_COMPARE_LS_DATA:
				{
#if 0
					unsigned char* address = self->instruction_pool + mana_get_integer(self, &self->instruction_pool[program_counter + 1]);
					self->instruction_pool[program_counter+4] = (unsigned char)(((unsigned int)address) >> 24);
					self->instruction_pool[program_counter+3] = (unsigned char)(((unsigned int)address) >> 16);
					self->instruction_pool[program_counter+2] = (unsigned char)(((unsigned int)address) >> 8);
					self->instruction_pool[program_counter+1] = (unsigned char)(((unsigned int)address));
#endif
				}
				break;

			case MANA_IL_CALL:
				{
					const char* name = mana_get_string(self, &self->instruction_pool[program_counter + 1]);

					mana_external_funtion_type* function = (mana_external_funtion_type*)mana_hash_get(&mana_external_function_hash, name);
					if(function)
					{
#if defined(NDEBUG)
						self->instruction_pool[program_counter+4] = ((unsigned int)function) >> 24;
						self->instruction_pool[program_counter+3] = ((unsigned int)function) >> 16;
						self->instruction_pool[program_counter+2] = ((unsigned int)function) >> 8;
						self->instruction_pool[program_counter+1] = ((unsigned int)function);
#endif
					}
					else
					{
						MANA_ERROR("An external function called %s is not found.\n", name);
						goto ABORT;
					}
				}
				break;
			}
			program_counter += mana_get_instruction_size(&self->instruction_pool[program_counter]);
		}

		self->file_header->flag |= MANA_HEADER_FLAG_COMPILED;
	}

	{
		int counter = self->file_header->flag >> 2;
		counter++;
		self->file_header->flag &= 0x03;
		self->file_header->flag |= (counter << 2);
	}

	if(self->file_header->flag & MANA_HEADER_FLAG_RESOURCE)
	{
		unsigned char* p = self->instruction_pool + self->file_header->size_of_instruction_pool;
		p = (unsigned char*)(((int)p + (MANA_DATALINK_STANDARD_ALIGNMENT_SIZE - 1))
			/ MANA_DATALINK_STANDARD_ALIGNMENT_SIZE * MANA_DATALINK_STANDARD_ALIGNMENT_SIZE);
		mana_datalink_load(&self->datalink, (void*)p);
	}

	actor_info = (mana_actor_info_header*)(self->file_header + 1);

	for(i = 0; i < self->file_header->number_of_actors; i++)
	{
		mana_actor* actor;
		char* actor_name = &self->constant_pool[actor_info->name];

		assert(actor_info->number_of_instances == 1);

		if(actor_info->flag & MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM)
		{
			actor = NULL;
			mana_hash_set(&self->phantom_hash, actor_name, actor_info);
		}
		else
		{
			actor = mana_actor_create(self, actor_info->variable_size);
			mana_hash_set(&self->actor_hash, actor_name, actor);
			mana_array_set(&self->actor_array, i, actor);
		}
		{
			mana_action_info_header* action_info;
			int j;

			action_info = (mana_action_info_header*)(actor_info + 1);

			for(j = 0; j < actor_info->number_of_actions; j++)
			{
				if(!(actor_info->flag & MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM))
				{
					const char* acton_name = &self->constant_pool[action_info->name];
					mana_hash_set(&actor->actions, acton_name, (void*)action_info->address);
				}

				action_info++;
			}

			actor_info = (mana_actor_info_header*)action_info;
		}
	}

	/* �t���O�������� */
	self->flag = MANA_FLAG_INIT_ACTION_RUNNING|MANA_FLAG_INITIALIZED|MANA_FLAG_SYSTEM_REQUEST;
	if(auto_release)
		self->flag |= MANA_FLAG_AUTO_RELEASE;

	/* �S�A�N�^�[�� init��main �A�N�V���������s */
	mana_restart(self);
	mana_request_all(self, 0, "main", NULL);
	mana_request_all(self, 1, "init", NULL);

	return MANA_TRUE;

ABORT:
	mana_unload_program(self);

	return MANA_FALSE;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 */
void mana_unload_program(mana* self)
{
	mana_hash_iterator iterator;

	assert(self);

	/* �X�N���v�g�Ŋm�ۂ������\�[�X�̊J�� */
	/*
	GetResource().Clear();
	*/

	/* �C�x���g�{�b�N�X�̊J�� */
	/*
	DestroyIntersections();
	*/

	/* release actors */
	{
		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor_destroy(mana_hash_iterator_get_value(&iterator));
		}

		mana_hash_clear(&self->actor_hash);

		mana_array_clear(&self->actor_array);
	}
	/* release phantoms */
	{
		mana_hash_clear(&self->phantom_hash);
	}

	/* �ϐ��̏����� */
#if 0
	self->map_actor = NULL;
	self->player_actor = NULL;
#endif
	self->flag &= ~(MANA_FLAG_INIT_ACTION_RUNNING|MANA_FLAG_INIT_ACTION_FINISHED|MANA_FLAG_INITIALIZED|MANA_FLAG_SYSTEM_REQUEST);

	/* �O���[�o���ϐ��̈��������܂� */
	mana_free(self->global_memory);
	self->global_memory = NULL;

	/* �f�[�^�̊J�� */
	mana_datalink_release(&self->datalink);

	/* �v���O�����̊J�� */
	if(self->file_header)
	{
		int counter = self->file_header->flag >> 2;
		counter--;
		self->file_header->flag &= 0x03;
		self->file_header->flag |= (counter << 2);
		if(counter == 0 && (self->flag & MANA_FLAG_AUTO_RELEASE))
		{
			mana_free(self->file_header);
			self->file_header = NULL;

			self->constant_pool = NULL;
			self->instruction_pool = NULL;
		}
	}
}

/*!
 * @return	mana�v���O�����A�h���X
 */
void* mana_get_program_buffer(mana* self)
{
	assert(self);

	return self->file_header;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 */
void mana_restart(mana* self)
{
	mana_hash_iterator iterator;

	assert(self);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		mana_actor_restart(actor);
	}

	self->flag &= ~(MANA_FLAG_INIT_ACTION_RUNNING|MANA_FLAG_INIT_ACTION_FINISHED);
	self->frame_counter = 0;
}

/*!
 * yield���߂ȂǁA���̃t���[���҂��ɂȂ閽�߂��t�F�b�`����܂Ŏ��s���܂��B
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	second	�o�߂�������(�P�ʂ͕b)
 * @retval		MANA_TRUE	�X�N���v�g���s��
 * @retval		MANA_FALSE	�X�N���v�g��~
 */
mana_bool mana_run(mana* self, const float second)
{
	mana_hash_iterator iterator;
	mana_bool running = MANA_FALSE;

	assert(self);

	self->flag &= ~MANA_FLAG_REQUESTED;
	self->flag |= MANA_FLAG_FRAME_CHANGED;

	mana_actor_set_delta_time(second);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		running |= mana_actor_run(actor);
	}

	self->flag &= ~MANA_FLAG_FRAME_CHANGED;
	while(self->flag & MANA_FLAG_REQUESTED)
	{
		self->flag &= ~MANA_FLAG_REQUESTED;

		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor* actor = mana_hash_iterator_get_value(&iterator);

			assert(actor);

			if(actor->flag & MANA_ACTOR_FLAG_REQUESTED)
			{
				running |= mana_actor_run(actor);
			}
		}
	}
	self->flag |= MANA_FLAG_FRAME_CHANGED;

	if((self->flag & MANA_FLAG_INIT_ACTION_RUNNING) && !running)
	{
		self->flag &= MANA_FLAG_INIT_ACTION_RUNNING;
		self->flag |= MANA_FLAG_INIT_ACTION_FINISHED;
	}

	self->frame_counter++;

	return running;
}

/*!
 * ���s�I���܂Ńu���b�N���܂��B
 * �X�N���v�g�ɂ���Ă̓f�b�h���b�N���������鎖������̂Œ��ӂ��Ă��������B
 * @param[in]	self	mana �I�u�W�F�N�g
 */
void mana_execute(mana* self)
{
	while(mana_run(self, 1.0f / 60.0f))
	{
		mana_yield_thread();
	}
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @retval		MANA_TRUE	�X�N���v�g���s��
 * @retval		MANA_FALSE	�X�N���v�g��~��
 */
mana_bool mana_is_running(mana* self)
{
	assert(self);

	if(self->flag & MANA_FLAG_INITIALIZED)
	{
		mana_hash_iterator iterator;

		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			mana_actor* actor = mana_hash_iterator_get_value(&iterator);

			assert(actor);

			if(mana_actor_is_running(actor))
				return MANA_TRUE;
		}
	}

	return MANA_FALSE;
}

/*!
 * @param[in]	self		mana �I�u�W�F�N�g
 * @param[in]	level		�D��x(�����قǗD��)
 * @param[in]	action_name	�A�N�V������
 * @param[in]	sender		���N�G�X�g���A�N�^�[
 */
void mana_request_all(mana* self, int level, const char* action_name, mana_actor* sender)
{
	mana_hash_iterator iterator;

	assert(self);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		mana_actor_request(actor, level, action_name, sender);
	}
}

/*!
 * @param[in]	self		mana �I�u�W�F�N�g
 * @param[in]	level		�D��x(�����قǗD��)
 * @param[in]	actor_name	�A�N�^�[��
 * @param[in]	action_name	�A�N�V������
 * @param[in]	sender		���N�G�X�g���A�N�^�[
 * @retval		MANA_TRUE		���N�G�X�g����
 * @retval		MANA_FALSE		���N�G�X�g���s
 */
mana_bool mana_request(mana* self, int level, const char* actor_name, const char* action_name, mana_actor* sender)
{
	mana_actor* actor;

	assert(self);

	actor = mana_get_actor(self, actor_name);
	if(actor == NULL)
		return MANA_FALSE;

	return mana_actor_request(actor, level, action_name, sender);
}

/*!
 * @param[in]	self		mana �I�u�W�F�N�g
 */
void mana_yield(mana* self)
{
	mana_hash_iterator iterator;

	assert(self);

	mana_hash_iterator_rewind(&iterator);

	while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
	{
		mana_actor* actor = mana_hash_iterator_get_value(&iterator);

		assert(actor);

		mana_actor_yield(actor);
	}
}

/*!
 * �v���O�����̈悩��P�o�C�g���̐��l���擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̂P�o�C�g���̐��l
 */
char mana_get_char(const mana* self, const unsigned char* address)
{
	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	return *address;
}

/*!
 * �v���O�����̈悩��Q�o�C�g���̐��l���擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̂Q�o�C�g���̐��l
 */
short mana_get_short(const mana* self, const unsigned char* address)
{
	short value;
	unsigned char* pointer;

	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	pointer = (unsigned char*)&value;

	if(mana_big_endian)
	{
		pointer[0] = address[0];
		pointer[1] = address[1];
		pointer[2] = address[2];
		pointer[3] = address[3];
	}
	else
	{
		pointer[3] = address[0];
		pointer[2] = address[1];
		pointer[1] = address[2];
		pointer[0] = address[3];
	}

	return value;
}

/*!
 * �v���O�����̈悩��S�o�C�g���̐��l���擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̂S�o�C�g���̐��l
 */
int mana_get_integer(const mana* self, const unsigned char* address)
{
	int value;
	unsigned char* pointer;

	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	pointer = (unsigned char*)&value;

	if(mana_big_endian)
	{
		pointer[0] = address[0];
		pointer[1] = address[1];
		pointer[2] = address[2];
		pointer[3] = address[3];
	}
	else
	{
		pointer[3] = address[0];
		pointer[2] = address[1];
		pointer[1] = address[2];
		pointer[0] = address[3];
	}

	return value;
}

/*!
 * �v���O�����̈悩��P�o�C�g���̐��l���擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̂P�o�C�g���̐��l
 */
unsigned char mana_get_unsigned_char(const mana* self, const unsigned char* address)
{
	return (unsigned char)mana_get_char(self, address);
}

/*!
 * �v���O�����̈悩��Q�o�C�g���̐��l���擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̂Q�o�C�g���̐��l
 */
unsigned short mana_get_unsigned_short(const mana* self, const unsigned char* address)
{
	return (unsigned short)mana_get_short(self, address);
}

/*!
 * �v���O�����̈悩��S�o�C�g���̐��l���擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̂S�o�C�g���̐��l
 */
unsigned int mana_get_unsigned_integer(const mana* self, const unsigned char* address)
{
	return (unsigned int)mana_get_integer(self, address);
}

/*!
 * �v���O�����̈悩��P���x�����������擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̒P���x��������
 */
float mana_get_float(const mana* self, const unsigned char* address)
{
	float value;
	unsigned char* pointer;

	assert(self);
	assert(address >= self->instruction_pool && address < &self->instruction_pool[self->file_header->size_of_instruction_pool]);

	pointer = (unsigned char*)&value;

	if(mana_big_endian)
	{
		pointer[0] = address[0];
		pointer[1] = address[1];
		pointer[2] = address[2];
		pointer[3] = address[3];
	}
	else
	{
		pointer[3] = address[0];
		pointer[2] = address[1];
		pointer[1] = address[2];
		pointer[0] = address[3];
	}

	return value;
}

/*!
 * ������̈悩�當�����擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		������ւ̃|�C���^
 */
const char* mana_get_string(const mana* self, const unsigned char* address)
{
	assert(self);

	return self->constant_pool + mana_get_integer(self, address);
}

/*!
 * �v���O�����̈悩��|�C���^���擾���܂�
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	address	�v���O�����A�h���X
 * @return		�v���O�����̈��̃|�C���^
 */
unsigned char* mana_get_address(const mana* self, const unsigned char* address)
{
	assert(self);

	return self->instruction_pool + mana_get_integer(self, address);
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	name	�A�N�^�[��
 * @retval		!= NULL	�A�N�^�[�I�u�W�F�N�g�|�C���^
 * @retval		== NULL	�������s
 */
mana_actor* mana_get_actor(mana* self, const char* name)
{
	assert(self);

	return mana_hash_get(&self->actor_hash, name);
}

/*!
 * ���s���x���x���̂Œ��ӂ��K�v�ł��B
 *
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	actor	mana_actor �I�u�W�F�N�g
 * @retval		!= NULL	�A�N�^�[�̖��O
 * @retval		== NULL	�������s
 */
const char* mana_get_actor_name(mana* self, mana_actor* actor)
{
	assert(self);

	if((self->flag & MANA_FLAG_INITIALIZED) && actor)
	{
		mana_hash_iterator iterator;

		mana_hash_iterator_rewind(&iterator);

		while(mana_hash_iterator_hash_next(&self->actor_hash, &iterator))
		{
			if(actor == mana_hash_iterator_get_value(&iterator))
				return mana_hash_iterator_get_name(&iterator);
		}
	}

	return NULL;
}

/*!
 * @param[in]	self		mana �I�u�W�F�N�g
 * @param[in]	actor		�R�s�[���ɂȂ�mana_actor �I�u�W�F�N�g
 * @param[in]	new_name	�V�����A�N�^�[�̖��O
 * @retval		!= NULL		�R�s�[���ꂽmana_actor �I�u�W�F�N�g
 * @retval		== NULL		�R�s�[���s
 */
struct mana_actor* mana_create_actor(mana* self, struct mana_actor* actor, const char* new_name)
{
	mana_actor* new_actor = NULL;

	if(actor && actor->parent == self)
	{
		if(mana_get_actor(self, new_name) == NULL)
		{
			new_actor = mana_actor_clone(actor);
			mana_hash_set(&self->actor_hash, new_name, new_actor);
			mana_array_append(&self->actor_array, new_actor);
		}
	}

	return new_actor;
}

/*!
 * @param[in]	self		mana �I�u�W�F�N�g
 * @param[in]	name		�R�s�[���ɂȂ�A�N�^�[�̖��O
 * @param[in]	new_name	�V�����A�N�^�[�̖��O
 * @retval		!= NULL		�R�s�[���ꂽmana_actor �I�u�W�F�N�g
 * @retval		== NULL		�R�s�[���s
 */
struct mana_actor* mana_create_actor_name(mana* self, const char* name, const char* new_name)
{
	return mana_create_actor(self, mana_get_actor(self, name), new_name);
}

/*!
 * @param[in]	self		mana �I�u�W�F�N�g
 * @param[in]	name		�R�s�[���ɂȂ�t�@���g���̖��O
 * @param[in]	new_name	�V�����A�N�^�[�̖��O
 * @retval		!= NULL		�R�s�[���ꂽmana_actor �I�u�W�F�N�g
 * @retval		== NULL		�R�s�[���s
 */
struct mana_actor* mana_create_actor_from_phantom(mana* self, const char* name, const char* new_name)
{
	mana_actor* actor = NULL;
	mana_actor_info_header* actor_info = mana_hash_get(&self->phantom_hash, name);
	if(actor_info && (actor_info->flag & MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM))
	{
		actor = mana_actor_create(self, actor_info->variable_size);
		mana_hash_set(&self->actor_hash, new_name, actor);
		mana_array_append(&self->actor_array, actor);

		{
			mana_action_info_header* action_info;
			int j;

			action_info = (mana_action_info_header*)(actor_info + 1);

			for(j = 0; j < actor_info->number_of_actions; j++)
			{
				char* action_name = &self->constant_pool[action_info->name];
				mana_hash_set(&actor->actions, action_name, &self->instruction_pool[action_info->address]);

				action_info++;
			}
		}
	}
	return actor;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @retval		MANA_TRUE	init�A�N�V������
 * @retval		MANA_FALSE	init�A�N�V�����I��
 */
mana_bool mana_is_in_init_action(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_INIT_ACTION_RUNNING) ? MANA_TRUE : MANA_FALSE;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @retval		MANA_TRUE	init�A�N�V�����I��
 * @retval		MANA_FALSE	init�A�N�V�����͎��s���Ă��Ȃ�
 */
mana_bool mana_is_finish_init_action(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_INIT_ACTION_FINISHED) ? MANA_TRUE : MANA_FALSE;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @param[in]	enable
 * - MANA_TRUE�Ȃ�΃��N�G�X�g����
 * - MANA_FALSE�Ȃ�΃��N�G�X�g�֎~
 */
void mana_enable_system_request(mana* self, const mana_bool enable)
{
	assert(self);

	if(enable)
		self->flag |= MANA_FLAG_SYSTEM_REQUEST;
	else
		self->flag &= ~MANA_FLAG_SYSTEM_REQUEST;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @retval		MANA_TRUE	���N�G�X�g����
 * @retval		MANA_FALSE	���N�G�X�g�֎~
 */
mana_bool mana_is_system_request_enabled(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_SYSTEM_REQUEST) ? MANA_TRUE : MANA_FALSE;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @return		�t���[���J�E���^
 */
unsigned int mana_get_frame_counter(mana* self)
{
	assert(self);

	return self->frame_counter;
}

/*!
 * @param[in]	self	mana �I�u�W�F�N�g
 * @retval		MANA_TRUE	�t���[�����ύX���ꂽ
 * @retval		MANA_FALSE	�����t���[��
 */
mana_bool mana_is_frame_changed(mana* self)
{
	assert(self);

	return (self->flag & MANA_FLAG_FRAME_CHANGED) ? MANA_TRUE : MANA_FALSE;
}

/*!
 * @param[in]	self		mana �I�u�W�F�N�g
 * @param[in]	resouce_id	���\�[�X�ԍ�
 * @param[out]	buffer		�f�[�^�A�h���X
 * @param[out]	size		�f�[�^�T�C�Y
 */
void mana_get_data(const mana* self, const int resouce_id, const void** buffer, size_t* size)
{
	assert(self);
	assert(resouce_id >= 0 && resouce_id < mana_datalink_get_number_of_datas(&self->datalink));

	*buffer = mana_datalink_get_data(&self->datalink, resouce_id);
	*size = mana_datalink_get_data_size(&self->datalink, resouce_id);
}
