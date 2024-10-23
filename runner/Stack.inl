/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once

namespace mana
{
	inline Stack::Stack()
		: mBuffer(nullptr, std::free)
	{
	}

	inline void Stack::Serialize(const std::shared_ptr<OutputStream>& stream) const
	{
		stream->Push(mUsedSize);
		stream->PushData(mBuffer.get(), mUsedSize);
	}

	inline void Stack::Deserialize(const std::shared_ptr<OutputStream>& stream)
	{
		mBuffer.reset();
		mAllocatedSize = 0;

		mUsedSize = stream->Pop<address_t>();
		AllocateBegin(mUsedSize);
		std::memcpy(mBuffer.get(), stream->GetBuffer(), mUsedSize);
		AllocateEnd(mUsedSize);
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
		static_assert(std::is_arithmetic_v<T> == true || std::is_pointer_v<T> == true, "Specify the arithmetic or pointer type");
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
		static_assert(std::is_arithmetic_v<T> == true || std::is_pointer_v<T> == true, "Specify the arithmetic or pointer type");
		Deallocate(sizeof(T));
		return static_cast<T>(mBuffer.get()[mUsedSize]);
	}

	inline void Stack::PopData(void* buffer, const address_t size)
	{
		Deallocate(size);
		std::memcpy(buffer, &mBuffer.get()[mUsedSize], size);
	}

	template<typename T>
	inline T Stack::Get(const address_t index) const
	{
		static_assert(std::is_arithmetic_v<T> == true || std::is_pointer_v<T> == true, "Specify the arithmetic or pointer type");
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
		static_assert(std::is_arithmetic_v<T> == true || std::is_pointer_v<T> == true, "Specify the arithmetic or pointer type");
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
		MANA_ASSERT(mUsedSize <= mAllocatedSize);
	}

	inline bool Stack::operator==(const Stack& other) const
	{
		if (mUsedSize != other.mUsedSize)
			return false;
		return std::memcmp(mBuffer.get(), other.mBuffer.get(), mUsedSize) == 0;
	}

	/**
	 * Get the size to match the memory alignment size.
	 * @param[in]	size	Size.
	 * @return		Size to fit memory alignment.
	 */
	inline address_t Stack::GetAlignmentSize(const address_t size)
	{
		constexpr address_t PageSize = sizeof(void*);
		return (size + (PageSize - 1)) / PageSize * PageSize;
	}

	/**
	 * Start of memory allocation
	 * @param[in]	gainSize	Size to increase memory
	 */
	inline void Stack::AllocateBegin(const address_t gainSize)
	{
		const address_t allocateSize = GetAlignmentSize(mUsedSize + gainSize);
		if (allocateSize >= mAllocatedSize)
		{
			mAllocatedSize = allocateSize;

			Buffer* newBuffer = static_cast<Buffer*>(std::realloc(mBuffer.get(), mAllocatedSize * sizeof(mBuffer)));
			if (newBuffer == nullptr)
				throw std::bad_alloc();
			mBuffer.release();
			mBuffer.reset(newBuffer);
		}
	}

	/**
	 * End of memory allocation
	 * @param[in]	gainSize	Size to increase memory
	 */
	inline void Stack::AllocateEnd(const address_t gainSize)
	{
		mUsedSize += ((gainSize + sizeof(mBuffer) - 1) / sizeof(mBuffer));
		MANA_ASSERT(mUsedSize < mAllocatedSize);
	}

	/**
	 * Release memory
	 * @param[in]	releaseSize		Size to release
	 */
	inline void Stack::Deallocate(const address_t releaseSize)
	{
		mUsedSize -= ((releaseSize + sizeof(mBuffer) - 1) / sizeof(mBuffer));
		MANA_ASSERT(mUsedSize < mAllocatedSize);
	}
}
