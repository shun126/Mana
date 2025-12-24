/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once

namespace mana
{
	inline Buffer::Buffer()
		: mBuffer(nullptr, std::free)
	{
	}

	inline Buffer::Buffer(const address_t size)
		: mBuffer(nullptr, std::free)
	{
		Allocate(size);
	}

	inline void Buffer::Clear()
	{
		mUsedSize = 0;
	}

	inline void Buffer::Reset()
	{
		mBuffer.reset(nullptr);
		mAllocatedSize = mUsedSize = 0;
	}

	inline void Buffer::Allocate(const address_t size)
	{
		const address_t newSize = mUsedSize + size;

                if (newSize > mAllocatedSize)
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

	inline void Buffer::Release(const address_t size)
	{
		mUsedSize -= size;
	}

	template<typename T>
	inline T* Buffer::GetAddressFromTop(const address_t index) const
	{
		MANA_ASSERT(index < mUsedSize);
		void* address = static_cast<uint8_t*>(mBuffer.get()) + (index);
		return static_cast<T*>(address);
	}

	template<typename T>
	inline T* Buffer::GetAddressFromBottom(const address_t index) const
	{
		MANA_ASSERT(0 < index && index <= mUsedSize);
		void* address = static_cast<uint8_t*>(mBuffer.get()) + (mUsedSize - index);
		return static_cast<T*>(address);
	}

	inline address_t Buffer::GetSize() const
	{
		return mUsedSize;
	}

	inline void Buffer::SetSize(const address_t size)
	{
		MANA_ASSERT((mUsedSize + size) <= mAllocatedSize);
		mUsedSize = size;
	}

	inline bool Buffer::operator==(const Buffer& other) const noexcept
	{
		return 
			(mUsedSize == other.mUsedSize) &&
			(std::memcmp(mBuffer.get(), other.mBuffer.get(), mUsedSize) == 0);
	}

	inline bool Buffer::operator!=(const Buffer& other) const noexcept
	{
		return
			(mUsedSize != other.mUsedSize) ||
			(std::memcmp(mBuffer.get(), other.mBuffer.get(), mUsedSize) != 0);
	}
}
