/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "StringPool.h"
#include <cstring>

namespace mana
{
	static constexpr size_t AllocatePageSize = 4096;

	StringPool::StringPool()
		: mBuffer(nullptr, std::free)
	{
	}

	/**
	 * Searches for a string and returns a registered address. Returns InvalidAddress when searching for an unregistered string.
	 * @param[in]	text	String to search for
	 * @return				Registered address
	 */
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

	/**
	 * Get the string pointed to by the address. If an out-of-range address is specified, a range_error exception is thrown.
	 * @param[in]	address	Registered address
	 * @return		String pointed to by address
	 */
	std::string_view StringPool::Text(const address_t address) const
	{
		if (address == InvalidAddress)
			return "";

		if (address >= mUsedSize)
			throw std::range_error("out of range");

		return { static_cast<const char*>(&mBuffer.get()[address]) };
	}

	std::string_view StringPool::Get(const std::string_view& text) const
	{
		return Text(Find(text));
	}

	std::string_view StringPool::Set(const std::string_view& text)
	{
		address_t address = Find(text);
		if (address == InvalidAddress)
		{
			const address_t length = ToAddress(text.length() + 1);
			if (mUsedSize + length >= mAllocatedSize)
			{
				mAllocatedSize += (mUsedSize + length + AllocatePageSize);

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
