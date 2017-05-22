/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include <cstdlib>
#include <cstring>
#include <memory>

namespace mana
{
	class StringPool : private Noncopyable
	{
	public:
		StringPool();
		virtual ~StringPool() = default;

		const char* Get(const char* text) const;
		const char* Set(const char* text);

	private:
		size_t Find(const char* text) const noexcept;
		const char* Text(const size_t address) const;

	private:
		std::vector<size_t> mAddress;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		size_t mUsedSize = 0;
		size_t mAllocatedSize = 0;
	};

	inline StringPool::StringPool()
		: mBuffer(nullptr, std::free)
	{
	}

	inline size_t StringPool::Find(const char* text) const noexcept
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

	inline const char* StringPool::Text(const size_t address) const
	{
		if (address == ~0)
			return nullptr;
		
		if (address < mUsedSize)
			throw std::range_error("out of range");
		
		return static_cast<const char*>(&mBuffer.get()[address]);
	}

	inline const char* StringPool::Get(const char* text) const
	{
		return Text(Find(text));
	}

	inline const char* StringPool::Set(const char* text)
	{
		auto address = Find(text);
		if (address == ~0)
			return nullptr;

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

		return Text(address);
	}
}
