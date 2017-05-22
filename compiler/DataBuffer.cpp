/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "DataBuffer.h"

namespace mana
{
	static constexpr size_t allocationPageSize = 4096;

	DataBuffer::DataBuffer() noexcept
		: mBuffer(nullptr, std::free)
	{
	}

	const char* DataBuffer::GetBuffer() const noexcept
	{
		return mBuffer.get();
	}

	size_t DataBuffer::GetSize() const noexcept
	{
		return mUsedSize;
	}

	size_t DataBuffer::Get(const std::string_view& text) const noexcept
	{
		return Find(text);
	}

	size_t DataBuffer::Set(const std::string_view& text)
	{
		const size_t entity = Find(text);
		if (entity != nil)
			return entity;

		const size_t length = text.size() + 1;
		const size_t newAllocateSize = mUsedSize + length;
		if (newAllocateSize >= mAllocatedSize)
		{
			mAllocatedSize += newAllocateSize + allocationPageSize;

			void* newBuffer = std::realloc(mBuffer.get(), mAllocatedSize);
			if (newBuffer == nullptr)
				throw std::bad_alloc();
			mBuffer.release();
			mBuffer.reset(static_cast<char*>(newBuffer));
		}

		std::memcpy(mBuffer.get() + mUsedSize, text.data(), length);

		const size_t result = mUsedSize;
		mEntities.push_back(mUsedSize);
		mUsedSize += length;
		return result;
	}

	bool DataBuffer::Write(OutputStream& stream) const
	{
		stream.PushData(mBuffer.get(), mUsedSize);
		return true;
	}

	size_t DataBuffer::Find(const std::string_view& text) const noexcept
	{
		for (const auto& address : mEntities)
		{
			const std::string_view data = &mBuffer.get()[address];
			if (data == text)
				return address;
		}
		return nil;
	}
}
