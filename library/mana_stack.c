/*!
 * mana (library)
 *
 * @file	mana_stack.c
 * @brief	mana_stack�N���X�Ɋւ���\�[�X�t�@�C��
 * @detail
 * ���̃t�@�C����mana_stack�N���X�Ɋ֌W����\�[�X�t�@�C���ł��B
 * mana_stack�N���X��mana_actor�N���X�̃X�^�b�N������s�Ȃ��܂��B
 *
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#if !defined(___MANA_STACK_H___)
#include "mana_stack.h"
#endif
#include <assert.h>
#include <string.h>

#define MANA_STACK_ALLOCATE_BEGIN(s) {																						\
	size_t allocate_size = self->used_size + (s / sizeof(self->buffer));													\
	assert(s % sizeof(self->buffer) == 0);																					\
	if(allocate_size >= self->allocated_size){																				\
		self->allocated_size = allocate_size + 32;																			\
		self->buffer.void_pointer = mana_realloc(self->buffer.void_pointer, self->allocated_size * sizeof(self->buffer));	\
	}

#define MANA_STACK_ALLOCATE_END(s)																							\
	self->used_size += (s / sizeof(self->buffer));																			\
	assert(s % sizeof(self->buffer) == 0);																					\
	assert(self->used_size < self->allocated_size);																			\
}

#define MANA_STACK_RELEASE(s) {																								\
	self->used_size -= (s / sizeof(self->buffer));																			\
	assert(s % sizeof(self->buffer) == 0);																					\
	assert(self->used_size < self->allocated_size);																			\
}

/*!
 * @return	mana_stack �I�u�W�F�N�g
 * @warning	void mana_stack_initialize(mana_stack* self)���ĂԕK�v�͂���܂���B
 */
mana_stack* mana_stack_create(void)
{
	mana_stack* self = mana_malloc(sizeof(mana_stack));
	mana_stack_initialize(self);
	return self;
}

/*!
 * @param[in]	size	�m�ۃT�C�Y
 * @return		mana_stack �I�u�W�F�N�g
 * @warning		void mana_stack_initialize_with_size(mana_stack* self, size_t size)���ĂԕK�v�͂���܂���B
 */
mana_stack* mana_stack_create_with_size(const size_t size)
{
	mana_stack* self = mana_malloc(sizeof(mana_stack));
	mana_stack_initialize_with_size(self, size);
	return self;
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @warning		void mana_stack_finalize(mana_stack* self)���ĂԕK�v�͂���܂���B
 */
void mana_stack_destroy(mana_stack* self)
{
	if(self)
	{
		mana_stack_finalize(self);
		mana_free(self);
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 */
void mana_stack_initialize(mana_stack* self)
{
	assert(self);
	assert(sizeof(int32_t) == sizeof(void*));
	assert(sizeof(float) == sizeof(void*));
	assert(sizeof(char*) == sizeof(void*));

	self->allocated_size = self->used_size = 0;
	self->buffer.void_pointer = NULL;
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	size	�m�ۃT�C�Y
 */
void mana_stack_initialize_with_size(mana_stack* self, const size_t size)
{
	assert(self);
	assert(sizeof(int32_t) == sizeof(void*));
	assert(sizeof(float) == sizeof(void*));
	assert(sizeof(char*) == sizeof(void*));

	self->allocated_size = size;
	self->used_size = 0;
	self->buffer.void_pointer = mana_malloc(size);
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 */
void mana_stack_finalize(mana_stack* self)
{
	assert(self);
	mana_stack_clear(self);
	mana_free(self->buffer.void_pointer);
	self->buffer.void_pointer = NULL;
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[out]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_stack_serialize(const mana_stack* self, mana_stream* stream)
{
	assert(self && stream);

	mana_stream_push_integer(stream, self->used_size);
	mana_stream_push_data(stream, self->buffer.void_pointer, self->used_size);
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	stream	mana_stream �I�u�W�F�N�g
 */
void mana_stack_deserialize(mana_stack* self, mana_stream* stream)
{
	assert(self && stream);

	self->used_size = (size_t)mana_stream_pop_integer(stream);
	self->allocated_size = self->used_size + 1;
	self->buffer.void_pointer = mana_realloc(self->buffer.void_pointer, self->allocated_size);

	mana_stream_pop_data(stream, self->buffer.void_pointer, self->used_size);
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 */
void mana_stack_clear(mana_stack* self)
{
	if(self)
	{
		self->used_size = 0;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 */
void mana_stack_duplicate(mana_stack* self)
{
	mana_stack_push_pointer(self, mana_stack_get_pointer(self, 0));
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	size	�폜����T�C�Y
 */
void mana_stack_remove(mana_stack* self, const size_t size)
{
	if(self)
	{
		self->used_size -= size;
		assert(self->allocated_size == 0 || self->used_size < self->allocated_size);
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	value	�v�b�V������l
 */
void mana_stack_push_integer(mana_stack* self, const int32_t value)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(int32_t));
		self->buffer.integer_pointer[self->used_size] = value;
		MANA_STACK_ALLOCATE_END(sizeof(int32_t));
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	value	�v�b�V������l
 */
void mana_stack_push_real(mana_stack* self, const float value)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(float));
		self->buffer.float_pointer[self->used_size] = value;
		MANA_STACK_ALLOCATE_END(sizeof(float));
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	string	�v�b�V�����镶����
 */
void mana_stack_push_string(mana_stack* self, const char* string)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(const char*));
		self->buffer.string_handle[self->used_size] = string;
		MANA_STACK_ALLOCATE_END(sizeof(const char*));
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	pointer	�v�b�V������l
 */
void mana_stack_push_pointer(mana_stack* self, void* pointer)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(void*));
		self->buffer.void_handle[self->used_size] = pointer;
		MANA_STACK_ALLOCATE_END(sizeof(void*));
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	buffer	�v�b�V������f�[�^�̐擪�A�h���X
 * @param[in]	size	�v�b�V������f�[�^�̃T�C�Y
 */
void mana_stack_push_data(mana_stack* self, const void* buffer, const size_t size)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(size);
		memcpy(&self->buffer.integer_pointer[self->used_size], buffer, size);
		MANA_STACK_ALLOCATE_END(size);
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @return		�|�b�v�����l
 */
int32_t mana_stack_pop_integer(mana_stack* self)
{
	if(self)
	{
		MANA_STACK_RELEASE(sizeof(int32_t));
		return self->buffer.integer_pointer[self->used_size];
	}
	else
	{
		return 0;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @return		�|�b�v�����l
 */
float mana_stack_pop_real(mana_stack* self)
{
	if(self)
	{
		MANA_STACK_RELEASE(sizeof(float));
		return self->buffer.float_pointer[self->used_size];
	}
	else
	{
		return 0;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @return		�|�b�v�����l
 */
const char* mana_stack_pop_string(mana_stack* self)
{
	if(self)
	{
		MANA_STACK_RELEASE(sizeof(char*));
		return self->buffer.string_handle[self->used_size];
	}
	else
	{
		return 0;
	}
}

/*!
 * �X�^�b�N�Ɋi�[���ꂽ�|�C���^���|�b�v���܂��B
 * void* mana_stack_pop_address(mana_stack* self)
 * �̓X�^�b�N�̃A�h���X���|�b�v����_���Ⴂ�܂��B
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @return		�|�b�v�����l
 */
void* mana_stack_pop_pointer(mana_stack* self)
{
	if(self)
	{
		MANA_STACK_RELEASE(sizeof(void*));
		return self->buffer.void_handle[self->used_size];
	}
	else
	{
		return NULL;
	}
}

/*!
 * �X�^�b�N�̃A�h���X���|�b�v���܂��B
 * void* mana_stack_pop_pointer(mana_stack* self)
 * �̓X�^�b�N�Ɋi�[���ꂽ�|�C���^���|�b�v����_���Ⴂ�܂��B
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @return		�|�b�v�����l
 */
void* mana_stack_pop_address(mana_stack* self)
{
	if(self)
	{
		MANA_STACK_RELEASE(sizeof(void*));
		return &self->buffer.integer_pointer[self->used_size];
	}
	else
	{
		return NULL;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[out]	buffer	�|�b�v�����f�[�^�̐擪�A�h���X
 * @param[out]	size	�|�b�v�����f�[�^�̃T�C�Y
 */
void mana_stack_pop_data(mana_stack* self, void* buffer, const size_t size)
{
	if(self)
	{
		MANA_STACK_RELEASE(size);
		memcpy(buffer, &self->buffer.integer_pointer[self->used_size], size);
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @return		�l
 */
int32_t mana_stack_get_integer(const mana_stack* self, const size_t index)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		return self->buffer.integer_pointer[pointer];
	}
	else
	{
		return 0;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @return		�l
 */
float mana_stack_get_real(const mana_stack* self, const size_t index)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		return self->buffer.float_pointer[pointer];
	}
	else
	{
		return 0;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @return		������
 */
const char* mana_stack_get_string(const mana_stack* self, const size_t index)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		return self->buffer.string_handle[pointer];
	}
	else
	{
		return NULL;
	}
}

/*!
 * �X�^�b�N�Ɋi�[���ꂽ�|�C���^���擾���܂��B
 * void* mana_stack_get_address(mana_stack* self, size_t index)
 * �̓X�^�b�N�̃A�h���X���擾����_���Ⴂ�܂��B
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @return		�l
 */
void* mana_stack_get_pointer(const mana_stack* self, const size_t index)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		return self->buffer.void_handle[pointer];
	}
	else
	{
		return NULL;
	}
}

/*!
 * �X�^�b�N�̃A�h���X���擾���܂��B
 * void* mana_stack_get_pointer(mana_stack* self, size_t index)
 * �̓X�^�b�N�Ɋi�[���ꂽ�|�C���^���擾����_���Ⴂ�܂��B
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @return		�A�h���X
 */
void* mana_stack_get_address(const mana_stack* self, const size_t index)
{
	if(self)
	{
		size_t pointer = self->used_size - index;
		assert(pointer < self->allocated_size);
		return &self->buffer.integer_pointer[pointer];
	}
	else
	{
		return NULL;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @param[in]	integer	�l
 */
void mana_stack_set_integer(mana_stack* self, const size_t index, const int32_t integer)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		self->buffer.integer_pointer[pointer] = integer;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @param[in]	real	�l
 */
void mana_stack_set_real(mana_stack* self, const size_t index, const float real)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		self->buffer.float_pointer[pointer] = real;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @param[in]	string	������
 */
void mana_stack_set_string(mana_stack* self, const size_t index, const char* string)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		self->buffer.string_handle[pointer] = string;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	index	�X�^�b�N�|�C���^�ւ̃I�t�Z�b�g�l
 * @param[in]	pointer	�|�C���^
 */
void mana_stack_set_pointer(mana_stack* self, const size_t index, void* pointer)
{
	if(self)
	{
		size_t idx = self->used_size - index - 1;
		assert(idx < self->allocated_size);
		self->buffer.void_handle[idx] = pointer;
	}
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @return		�X�^�b�N�̃T�C�Y/�X�^�b�N�|�C���^
 */
size_t mana_stack_get_size(const mana_stack* self)
{
	return self ? self->used_size : 0;
}

/*!
 * @param[in]	self	mana_stack �I�u�W�F�N�g
 * @param[in]	size	�X�^�b�N�̃T�C�Y/�X�^�b�N�|�C���^
 */
void mana_stack_set_size(mana_stack* self, const size_t size)
{
	if(self)
	{
		self->used_size = size;
	}
}

/*!
 * @param[in]	self	mana_frame �I�u�W�F�N�g
 * @param[in]	other	mana_frame �I�u�W�F�N�g
 * @retval		== 0	����̓��e
 * @retval		!= 0	�قȂ���e
 */
int32_t mana_stack_compare(const mana_stack* self, const mana_stack* other)
{
	if(self == NULL)
		return 1;
	if(other == NULL)
		return 1;
	if(self->used_size != other->used_size)
		return 1;
	return memcmp(self->buffer.void_pointer, other->buffer.void_pointer, self->used_size);
}
