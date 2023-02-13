/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "DataBuffer.h"
#include "../runner/common/OutputStream.h"

namespace mana
{
	static constexpr address_t allocationPageSize = 4096;

	DataBuffer::DataBuffer() noexcept
		: mBuffer(nullptr, std::free)
	{
	}

	const char* DataBuffer::GetBuffer() const noexcept
	{
		return mBuffer.get();
	}

	address_t DataBuffer::GetSize() const noexcept
	{
		return mUsedSize;
	}

	address_t DataBuffer::Get(const std::string_view& text) const noexcept
	{
		return Find(text);
	}

	address_t DataBuffer::Set(const std::string_view& text)
	{
		const address_t entity = Find(text);
		if (entity != InvalidAddress)
			return entity;

		const address_t length = ToAddress(text.size() + 1);
		const address_t newAllocateSize = mUsedSize + length;
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

		const address_t result = mUsedSize;
		mEntities.push_back(mUsedSize);
		mUsedSize += length;
		return result;
	}

	bool DataBuffer::Write(OutputStream& stream) const
	{
		stream.PushData(mBuffer.get(), mUsedSize);
		return true;
	}

	address_t DataBuffer::Find(const std::string_view& text) const noexcept
	{
		for (const auto& address : mEntities)
		{
			const std::string_view data = &mBuffer.get()[address];
			if (data == text)
				return address;
		}
		return InvalidAddress;
	}
}
