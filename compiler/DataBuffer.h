/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include <memory>
#include <vector>

namespace mana
{
	class DataBuffer : private Noncopyable
	{
		typedef struct Entity
		{
			size_t mAddress;
		} Entity;

	//public:
		static const size_t nil = static_cast<size_t>(~0);

	public:
		DataBuffer() : mBuffer(nullptr, std::free)
		{
		}

		virtual ~DataBuffer()
		{
		}

		const char* GetBuffer() const
		{
			return mBuffer.get();
		}

		size_t GetSize() const
		{
			return mUsedSize;
		}

		size_t Get(const char* text) const
		{
			const Entity* entity = Find(text);
			if (entity)
				return entity->mAddress;
			else
				return nil;
		}

		size_t Set(const char* text)
		{
			const Entity* entity = Find(text);
			if (entity)
				return entity->mAddress;

			const size_t length = std::strlen(text) + 1;
			const size_t newAllocateSize = mUsedSize + length;
			if (newAllocateSize >= mAllocatedSize)
			{
				mAllocatedSize += newAllocateSize + 4096;

				void* newBuffer = std::realloc(mBuffer.get(), mAllocatedSize);
				if (newBuffer == nullptr)
					throw std::bad_alloc();
				mBuffer.release();
				mBuffer.reset(static_cast<char*>(newBuffer));
			}

			std::memcpy(mBuffer.get() + mUsedSize, text, length);

			Entity newEntity;
			newEntity.mAddress = mUsedSize;
			//newEntity.mSize = length;
			mEntities.push_back(newEntity);

			mUsedSize += length;

			return newEntity.mAddress;
		}

		bool Write(OutputStream& stream)
		{
			stream.PushData(mBuffer.get(), mUsedSize);
			return true;
		}

	private:
		const Entity* Find(const char* text) const
		{
			if (text == nullptr)
				return nullptr;

			for (const auto& entity : mEntities)
			{
				const char* data = &mBuffer.get()[entity.mAddress];
				if (std::strcmp(data, text) == 0)
					return &entity;
			}
			return nullptr;
		}

	private:
		std::vector<Entity> mEntities;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		size_t mAllocatedSize = 0;
		size_t mUsedSize = 0;
	};
}
