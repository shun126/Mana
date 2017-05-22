/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Buffer.h"

namespace mana
{
	inline Buffer::Buffer()
		: mBuffer(nullptr, std::free)
	{
	}

	inline Buffer::Buffer(const size_t size)
		: mBuffer(nullptr, std::free)
	{
		Allocate(size);
	}

#if 0
	inline void Buffer::Serialize(mana_stream* stream) const
	{
		assert(stream);
		mana_stream_push_size(stream, self->mUsedSize);
		mana_stream_push_data(stream, self->mBuffer, self->mUsedSize);
	}

	inline void Buffer::Deserialize(mana_stream* stream)
	{
		assert(stream);

		mAllocatedSize = mUsedSize = mana_stream_pop_size(stream);
		mBuffer = mana_realloc(mBuffer, mAllocatedSize);

		mana_stream_pop_data(stream, mBuffer, mAllocatedSize);
	}
#endif

	inline void Buffer::Clear()
	{
		mUsedSize = 0;
	}

	inline void Buffer::Reset()
	{
		mBuffer.reset(nullptr);
		mAllocatedSize = mUsedSize = 0;
	}

	inline void Buffer::Allocate(const size_t size)
	{
		const size_t newSize = mUsedSize + size;

		if (mUsedSize >= mAllocatedSize)
		{
			void* newBuffer = realloc(mBuffer.get(), newSize);
			if (newBuffer == nullptr)
				throw std::bad_alloc();
			mAllocatedSize = newSize;
			mBuffer.release();
			mBuffer.reset(newBuffer);
		}

		// zero clear
		std::memset(
			static_cast<uint8_t*>(mBuffer.get()) + mUsedSize,
			0,
			newSize - mUsedSize
		);

		// apply new size
		mUsedSize = newSize;
	}

	inline void Buffer::Release(const size_t size)
	{
		mUsedSize -= size;
	}

	template<typename T>
	inline T* Buffer::GetAddressFromTop(const size_t index) const
	{
		MANA_ASSERT(index < mUsedSize);
		void* address = static_cast<uint8_t*>(mBuffer.get()) + (index);
		return reinterpret_cast<T*>(address);
	}

	template<typename T>
	inline T* Buffer::GetAddressFromBottom(const size_t index) const
	{
		MANA_ASSERT(0 < index && index <= mUsedSize);
		void* address = static_cast<uint8_t*>(mBuffer.get()) + (mUsedSize - index);
		return reinterpret_cast<T*>(address);
	}

	inline size_t Buffer::GetSize() const
	{
		return mUsedSize;
	}

	inline void Buffer::SetSize(const size_t size)
	{
		MANA_ASSERT((mUsedSize + size) <= mAllocatedSize);
		mUsedSize = size;
	}

#if 0
	//inline bool mana_frame_compare(const mana_frame* other);	
	{
		if (self->mUsedSize != other->mUsedSize)
			return 1;
		return memcmp(self->mBuffer, other->mBuffer, self->mUsedSize);
	}
#endif	
}
