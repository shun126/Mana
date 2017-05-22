/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Setup.h"

namespace mana
{
	class Buffer : private Noncopyable
	{
	public:
		Buffer() : mBuffer(nullptr, std::free) {}

		explicit Buffer(const size_t size) : mBuffer(nullptr, std::free)
		{
			Allocate(size);
		}

		~Buffer() {}

#if 0
		void Serialize(mana_stream* stream) const
		{
			assert(stream);
			mana_stream_push_size(stream, self->mUsedSize);
			mana_stream_push_data(stream, self->mBuffer, self->mUsedSize);
		}

		void Deserialize(mana_stream* stream)
		{
			assert(stream);

			mAllocatedSize = mUsedSize = mana_stream_pop_size(stream);
			mBuffer = mana_realloc(mBuffer, mAllocatedSize);

			mana_stream_pop_data(stream, mBuffer, mAllocatedSize);
		}
#endif

		void Clear()
		{
			mUsedSize = 0;
		}

		void Reset()
		{
			mBuffer.reset(nullptr);
			mAllocatedSize = mUsedSize = 0;
		}

		void Allocate(const size_t size)
		{
			const size_t newSize = mUsedSize + size;

			if(mUsedSize >= mAllocatedSize)
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

		void Release(const size_t size)
		{
			mUsedSize -= size;
		}
	
		template<typename T>
		T* GetAddressFromTop(const size_t index) const
		{
			MANA_ASSERT(index < mUsedSize);
			void* address = static_cast<uint8_t*>(mBuffer.get()) + (index);
			return reinterpret_cast<T*>(address);
		}

		template<typename T>
		T* GetAddressFromBottom(const size_t index) const
		{
			MANA_ASSERT(0 < index && index <= mUsedSize);
			void* address = static_cast<uint8_t*>(mBuffer.get()) + (mUsedSize - index);
			return reinterpret_cast<T*>(address);
		}

		size_t GetSize() const
		{
			return mUsedSize;
		}
	
		void SetSize(const size_t size)
		{
			MANA_ASSERT((mUsedSize + size) <= mAllocatedSize);
			mUsedSize = size;
		}

#if 0
		//bool mana_frame_compare(const mana_frame* other);	
		{
			if(self->mUsedSize != other->mUsedSize)
				return 1;
			return memcmp(self->mBuffer, other->mBuffer, self->mUsedSize);
		}	
#endif	

	private:
		std::unique_ptr<void, decltype(&std::free)> mBuffer;
		size_t mAllocatedSize = 0;
		size_t mUsedSize = 0;
	};
}
