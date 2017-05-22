/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "StringPool.h"

namespace mana
{

	StringPool::StringPool()
		: mBuffer(nullptr, std::free)
	{
	}

	size_t StringPool::Find(const char* text) const noexcept
	{
		if (text)
		{
			for (const auto& address : mAddress)
			{
				if (strcmp(static_cast<const char*>(&mBuffer.get()[address]), text) == 0)
					return address;
			}
		}
		return static_cast<size_t>(~0);
	}

	const std::string_view StringPool::Text(const size_t address) const
	{
		if (address == ~0)
			return std::string_view();

		if (address >= mUsedSize)
			throw std::range_error("out of range");

		return std::string_view(static_cast<const char*>(&mBuffer.get()[address]));
	}

	const std::string_view StringPool::Get(const char* text) const
	{
		return Text(Find(text));
	}

	const std::string_view StringPool::Set(const char* text)
	{
		auto address = Find(text);
		if (address == ~0)
		{
			size_t length = strlen(text) + 1;
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
			std::memcpy(mBuffer.get() + mUsedSize, text, length);

			address = mUsedSize;
			mUsedSize += length;

			mAddress.push_back(address);
		}
		return Text(address);
	}
}
