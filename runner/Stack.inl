/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "Stack.h"

namespace mana
{
	inline Stack::Stack()
		: mBuffer(nullptr, std::free)
	{
	}

	inline void Stack::Serialize(OutputStream* stream) const
	{
		stream->Push(mUsedSize);
		stream->PushData(mBuffer.get(), mUsedSize);
	}

	inline void Stack::Deserialize(OutputStream* stream)
	{
		mUsedSize = stream->Pop<address_t>();
		mAllocatedSize = mUsedSize + 1;
		//mBuffer.mVoidPointer = mana_realloc(mBuffer.mVoidPointer, mAllocatedSize);
		//mana_stream_pop_data(stream, mBuffer.mVoidPointer, mUsedSize);
	}

	inline void Stack::Clear()
	{
		mUsedSize = 0;
	}

	inline void Stack::Duplicate()
	{
		Push(Get<void*>(0));
	}

	inline void Stack::Remove(const address_t size)
	{
		mUsedSize -= size;
		MANA_ASSERT(mAllocatedSize == 0 || mUsedSize < mAllocatedSize);
	}

	template<typename T>
	inline void Stack::Push(T value)
	{
		AllocateBegin(sizeof(T));
		mBuffer.get()[mUsedSize].Set(value);
		AllocateEnd(sizeof(T));
	}

	inline void Stack::Push(const void* buffer, const address_t size)
	{
		AllocateBegin(size);
		std::memcpy(&mBuffer.get()[mUsedSize], buffer, size);
		AllocateEnd(size);
	}

	template<typename T>
	inline T Stack::Pop()
	{
		Deallocate(sizeof(T));
		return static_cast<T>(mBuffer.get()[mUsedSize]);
	}

	inline void Stack::PopData(void* buffer, const address_t size)
	{
		Deallocate(size);
		std::memcpy(buffer, &mBuffer.get()[mUsedSize], size);
	}

	inline void* Stack::PopAddress()
	{
		Deallocate(sizeof(void*));
		return &mBuffer.get()[mUsedSize];
	}

	template<typename T>
	inline T Stack::Get(const address_t index) const
	{
		const address_t pointer = mUsedSize - index - 1;
		MANA_ASSERT(pointer < mAllocatedSize);
		return static_cast<T>(mBuffer.get()[pointer]);
	}

	inline void* Stack::GetAddress(const address_t index) const
	{
		const address_t pointer = mUsedSize - index;
		MANA_ASSERT(pointer < mAllocatedSize);
		return &mBuffer.get()[pointer];
	}

	template<typename T>
	inline void Stack::Set(const address_t index, T value)
	{
		const address_t pointer = mUsedSize - index - 1;
		MANA_ASSERT(pointer < mAllocatedSize);
		mBuffer.get()[pointer].Set(value);
	}

	inline address_t Stack::GetSize() const
	{
		return mUsedSize;
	}

	inline void Stack::SetSize(const address_t size)
	{
		mUsedSize = size;
		MANA_ASSERT(mUsedSize < mAllocatedSize);
	}

	inline bool Stack::operator==(const Stack& other) const
	{
		if (mUsedSize != other.mUsedSize)
			return false;
		return std::memcmp(mBuffer.get(), other.mBuffer.get(), mUsedSize) == 0;
	}

	inline address_t Stack::GetAlignmentSize(const address_t size)
	{
		static const address_t PageSize = 8;
		return (size + (PageSize - 1)) / PageSize * PageSize;
	}

	inline void Stack::AllocateBegin(const address_t gainSize)
	{
		const address_t allocateSize = GetAlignmentSize(mUsedSize + gainSize);
		if (allocateSize >= mAllocatedSize)
		{
			mAllocatedSize = allocateSize;

			Buffer* newBuffer = static_cast<Buffer*>(std::realloc(mBuffer.get(), mAllocatedSize * sizeof(mBuffer)));
			if (newBuffer == nullptr)
				std::bad_alloc();
			mBuffer.release();
			mBuffer.reset(newBuffer);
		}
	}

	inline void Stack::AllocateEnd(const address_t gainSize)
	{
		mUsedSize += ((gainSize + sizeof(mBuffer) - 1) / sizeof(mBuffer));
		MANA_ASSERT(mUsedSize < mAllocatedSize);
	}

	inline void Stack::Deallocate(const address_t releaseSize)
	{
		mUsedSize -= ((releaseSize + sizeof(mBuffer) - 1) / sizeof(mBuffer));
		MANA_ASSERT(mUsedSize < mAllocatedSize);
	}
}
