/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "StringPool.h"
#include <cstring>

namespace mana
{
	StringPool::StringPool()
		: mBuffer(nullptr, std::free)
	{
	}

	address_t StringPool::Find(const std::string_view& text) const noexcept
	{
		if (!text.empty())
		{
			for (const auto& address : mAddress)
			{
				if (std::strcmp(static_cast<const char*>(&mBuffer.get()[address]), text.data()) == 0)
					return address;
			}
		}
		return InvalidAddress;
	}

	const std::string_view StringPool::Text(const address_t address) const
	{
		if (address == InvalidAddress)
			return std::string_view();

		if (address >= mUsedSize)
			throw std::range_error("out of range");

		return std::string_view(static_cast<const char*>(&mBuffer.get()[address]));
	}

	const std::string_view StringPool::Get(const std::string_view text) const
	{
		return Text(Find(text));
	}

	const std::string_view StringPool::Set(const std::string_view text)
	{
		address_t address = Find(text);
		if (address == InvalidAddress)
		{
			address_t length = ToAddress(text.length() + 1);
			if (mUsedSize + length >= mAllocatedSize)
			{
				mAllocatedSize += (mUsedSize + length + 4096);

				auto* newBuffer = static_cast<char*>(std::realloc(mBuffer.release(), mAllocatedSize));
				if (newBuffer == nullptr)
				{
					throw std::bad_alloc();
				}
				mBuffer.reset(newBuffer);
			}
			std::memcpy(mBuffer.get() + mUsedSize, text.data(), length);

			address = mUsedSize;
			mUsedSize += length;

			mAddress.push_back(address);
		}
		return Text(address);
	}
}
