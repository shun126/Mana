/*!
mana (library)

@file	mana_stack.c
@brief	mana_stackクラスに関するソースファイル
@detail
このファイルはmana_stackクラスに関係するソースファイルです。
mana_stackクラスはmana_actorクラスのスタック操作を行ないます。

@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#if !defined(___MANA_STACK_H___)
#include "mana_stack.h"
#endif
#include <assert.h>
#include <string.h>

// メモリ確保時のページサイズ
#define MANA_STACK_ALLOCATE_PAGE_SIZE (8)

#define MANA_STACK_ALLOCATE_BEGIN(s) {																						\
	size_t allocate_size = self->used_size + ((s + sizeof(self->buffer) - 1) / sizeof(self->buffer));						\
	if(allocate_size >= self->allocated_size){																				\
		self->allocated_size = allocate_size + MANA_STACK_ALLOCATE_PAGE_SIZE;												\
		self->buffer.void_pointer = mana_realloc(self->buffer.void_pointer, self->allocated_size * sizeof(self->buffer));	\
	}

#define MANA_STACK_ALLOCATE_END(s)																							\
	self->used_size += ((s + sizeof(self->buffer) - 1) / sizeof(self->buffer));												\
	assert(self->used_size < self->allocated_size);																			\
}

#define MANA_STACK_RELEASE(s) {																								\
	self->used_size -= ((s + sizeof(self->buffer) - 1) / sizeof(self->buffer));												\
	assert(self->used_size < self->allocated_size);																			\
}

mana_stack* mana_stack_create(void)
{
	mana_stack* self = mana_malloc(sizeof(mana_stack));
	mana_stack_initialize(self);
	return self;
}

mana_stack* mana_stack_create_with_size(const size_t size)
{
	mana_stack* self = mana_malloc(sizeof(mana_stack));
	mana_stack_initialize_with_size(self, size);
	return self;
}

void mana_stack_destroy(mana_stack* self)
{
	if(self)
	{
		mana_stack_finalize(self);
		mana_free(self);
	}
}

void mana_stack_initialize(mana_stack* self)
{
	assert(self);
	//assert(sizeof(int32_t) == sizeof(void*));
	//assert(sizeof(float) == sizeof(void*));
	//assert(sizeof(char*) == sizeof(void*));

	self->allocated_size = self->used_size = 0;
	self->buffer.void_pointer = NULL;
}

void mana_stack_initialize_with_size(mana_stack* self, const size_t size)
{
	assert(self);
	//assert(sizeof(int32_t) == sizeof(void*));
	//assert(sizeof(float) == sizeof(void*));
	//assert(sizeof(char*) == sizeof(void*));

	self->allocated_size = size;
	self->used_size = 0;
	self->buffer.void_pointer = mana_malloc(size);
}

void mana_stack_finalize(mana_stack* self)
{
	assert(self);
	mana_stack_clear(self);
	mana_free(self->buffer.void_pointer);
	self->buffer.void_pointer = NULL;
}

void mana_stack_serialize(const mana_stack* self, mana_stream* stream)
{
	assert(self && stream);

	mana_stream_push_size(stream, self->used_size);
	mana_stream_push_data(stream, self->buffer.void_pointer, self->used_size);
}

void mana_stack_deserialize(mana_stack* self, mana_stream* stream)
{
	assert(self && stream);

	self->used_size = mana_stream_pop_size(stream);
	self->allocated_size = self->used_size + 1;
	self->buffer.void_pointer = mana_realloc(self->buffer.void_pointer, self->allocated_size);

	mana_stream_pop_data(stream, self->buffer.void_pointer, self->used_size);
}

void mana_stack_clear(mana_stack* self)
{
	if(self)
	{
		self->used_size = 0;
	}
}

void mana_stack_duplicate(mana_stack* self)
{
	mana_stack_push_pointer(self, mana_stack_get_pointer(self, 0));
}

void mana_stack_remove(mana_stack* self, const size_t size)
{
	if(self)
	{
		self->used_size -= size;
		assert(self->allocated_size == 0 || self->used_size < self->allocated_size);
	}
}

void mana_stack_push_integer(mana_stack* self, const int32_t value)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(int32_t));
		self->buffer.integer_pointer[self->used_size] = value;
		MANA_STACK_ALLOCATE_END(sizeof(int32_t));
	}
}

void mana_stack_push_real(mana_stack* self, const float value)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(float));
		self->buffer.float_pointer[self->used_size] = value;
		MANA_STACK_ALLOCATE_END(sizeof(float));
	}
}

void mana_stack_push_string(mana_stack* self, const char* string)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(const char*));
		self->buffer.string_handle[self->used_size] = string;
		MANA_STACK_ALLOCATE_END(sizeof(const char*));
	}
}

void mana_stack_push_pointer(mana_stack* self, void* pointer)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(sizeof(void*));
		self->buffer.void_handle[self->used_size] = pointer;
		MANA_STACK_ALLOCATE_END(sizeof(void*));
	}
}

void mana_stack_push_data(mana_stack* self, const void* buffer, const size_t size)
{
	if(self)
	{
		MANA_STACK_ALLOCATE_BEGIN(size);
		memcpy(&self->buffer.integer_pointer[self->used_size], buffer, size);
		MANA_STACK_ALLOCATE_END(size);
	}
}

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

void mana_stack_pop_data(mana_stack* self, void* buffer, const size_t size)
{
	if(self)
	{
		MANA_STACK_RELEASE(size);
		memcpy(buffer, &self->buffer.integer_pointer[self->used_size], size);
	}
}

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

void mana_stack_set_integer(mana_stack* self, const size_t index, const int32_t integer)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		self->buffer.integer_pointer[pointer] = integer;
	}
}

void mana_stack_set_real(mana_stack* self, const size_t index, const float real)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		self->buffer.float_pointer[pointer] = real;
	}
}

void mana_stack_set_string(mana_stack* self, const size_t index, const char* string)
{
	if(self)
	{
		size_t pointer = self->used_size - index - 1;
		assert(pointer < self->allocated_size);
		self->buffer.string_handle[pointer] = string;
	}
}

void mana_stack_set_pointer(mana_stack* self, const size_t index, void* pointer)
{
	if(self)
	{
		size_t idx = self->used_size - index - 1;
		assert(idx < self->allocated_size);
		self->buffer.void_handle[idx] = pointer;
	}
}

size_t mana_stack_get_size(const mana_stack* self)
{
	return self ? self->used_size : 0;
}

void mana_stack_set_size(mana_stack* self, const size_t size)
{
	if(self)
	{
		self->used_size = size;
	}
}

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
