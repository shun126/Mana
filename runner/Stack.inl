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
		stream->PushData(mBuffer.get(), mUsedSize * SlotSize);
	}

	inline void Stack::Deserialize(const std::shared_ptr<OutputStream>& stream)
	{
		mBuffer.reset();
		mAllocatedSize = 0;

		mUsedSize = 0;

		const address_t slotCount = stream->Pop<address_t>();
		AllocateBegin(slotCount * SlotSize);
		std::memcpy(mBuffer.get(), stream->GetBuffer(), slotCount * SlotSize);
		AllocateEnd(slotCount * SlotSize);
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
		MANA_ASSERT(size <= mUsedSize);
		mUsedSize -= size;
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
		MANA_ASSERT(size <= mAllocatedSize);
		mUsedSize = size;
	}

	inline bool Stack::operator==(const Stack& other) const
	{
		if (mUsedSize != other.mUsedSize)
			return false;
		return std::memcmp(mBuffer.get(), other.mBuffer.get(), mUsedSize) == 0;
	}

	//! バイト数を、それを収めるのに必要なスロット数へ切り上げます
	inline address_t Stack::ToSlotCount(const address_t byteSize)
	{
		return (byteSize + SlotSize - 1) / SlotSize;
	}

	/**
	 * Get the size to match the memory alignment size.
	 * @param[in]	slotCount	Number of slots.
	 * @return		Number of slots to fit memory alignment.
	 */
	inline address_t Stack::GetAlignmentSize(const address_t slotCount)
	{
		// 確保はこのスロット数単位で行います
		constexpr address_t PageSize = 8;
		return (slotCount + (PageSize - 1)) / PageSize * PageSize;
	}

	/**
	 * Start of memory allocation
	 * @param[in]	gainSize	Size to increase memory
	 */
	inline void Stack::AllocateBegin(const address_t gainSize)
	{
		const address_t requiredSize = mUsedSize + ToSlotCount(gainSize);
		if (requiredSize > mAllocatedSize)
		{
			const address_t allocateSize = GetAlignmentSize(requiredSize);

			Buffer* newBuffer = static_cast<Buffer*>(std::realloc(mBuffer.get(), allocateSize * SlotSize));
			if (newBuffer == nullptr)
				throw std::bad_alloc();
			mAllocatedSize = allocateSize;
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
		mUsedSize += ToSlotCount(gainSize);
		MANA_ASSERT(mUsedSize <= mAllocatedSize);
	}

	/**
	 * Release memory
	 * @param[in]	releaseSize		Size to release
	 */
	inline void Stack::Deallocate(const address_t releaseSize)
	{
		const address_t slotCount = ToSlotCount(releaseSize);
		MANA_ASSERT(slotCount <= mUsedSize);
		mUsedSize -= slotCount;
	}
}
