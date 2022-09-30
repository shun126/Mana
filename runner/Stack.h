/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "common/Setup.h"
#include <memory>

namespace mana
{
	class Actor;

	class Stack : private Noncopyable
	{
	public:
		Stack() : mBuffer(nullptr, std::free)
		{
		}

		virtual ~Stack() = default;

		void Serialize(OutputStream* stream) const
		{
			stream->Push(mUsedSize);
			stream->PushData(mBuffer.get(), mUsedSize);
		}

		void Deserialize(OutputStream* stream)
		{
			mUsedSize = stream->Pop<size_t>();
			mAllocatedSize = mUsedSize + 1;
			//mBuffer.mVoidPointer = mana_realloc(mBuffer.mVoidPointer, mAllocatedSize);

			//mana_stream_pop_data(stream, mBuffer.mVoidPointer, mUsedSize);
		}

		void Clear()
		{
			mUsedSize = 0;
		}
	
		void Duplicate()
		{
			Push(Get<void*>(0));
		}
	
		void Remove(const size_t size)
		{
			mUsedSize -= size;
			MANA_ASSERT(mAllocatedSize == 0 || mUsedSize < mAllocatedSize);
		}

		template<typename T> void Push(T value)
		{
			AllocateBegin(sizeof(T));
			mBuffer.get()[mUsedSize].Set(value);
			AllocateEnd(sizeof(T));
		}
	
		void Push(const void* buffer, const size_t size)
		{
			AllocateBegin(size);
			std::memcpy(&mBuffer.get()[mUsedSize], buffer, size);
			AllocateEnd(size);
		}

		template<typename T> T Pop()
		{
			Deallocate(sizeof(T));
			return static_cast<T>(mBuffer.get()[mUsedSize]);
		}

		void PopData(void* buffer, const size_t size)
		{
			Deallocate(size);
			std::memcpy(buffer, &mBuffer.get()[mUsedSize], size);
		}
	
		void* PopAddress()
		{
			Deallocate(sizeof(void*));
			return &mBuffer.get()[mUsedSize];
		}
	
		template<typename T> T Get(const size_t index) const
		{
			const size_t pointer = mUsedSize - index - 1;
			MANA_ASSERT(pointer < mAllocatedSize);
			return static_cast<T>(mBuffer.get()[pointer]);
		}

		void* GetAddress(const size_t index) const
		{
			const size_t pointer = mUsedSize - index;
			MANA_ASSERT(pointer < mAllocatedSize);
			return &mBuffer.get()[pointer];
		}
	
		template<typename T> void Set(const size_t index, T value)
		{
			const size_t pointer = mUsedSize - index - 1;
			MANA_ASSERT(pointer < mAllocatedSize);
			mBuffer.get()[pointer].Set(value);
		}

		size_t GetSize() const
		{
			return mUsedSize;
		}

		void SetSize(const size_t size)
		{
			mUsedSize = size;
			MANA_ASSERT(mUsedSize < mAllocatedSize);
		}

		bool operator==(const Stack& other) const
		{
			if(mUsedSize != other.mUsedSize)
				return false;
			return std::memcmp(mBuffer.get(), other.mBuffer.get(), mUsedSize) == 0;
		}
	
	private:
		static size_t GetAlignmentSize(const size_t size)
		{
			static const size_t PageSize = 8;
			return (size + (PageSize - 1)) / PageSize * PageSize;
		}

		void AllocateBegin(const size_t gainSize)
		{
			const size_t allocateSize = GetAlignmentSize(mUsedSize + gainSize);
			if (allocateSize >= mAllocatedSize)
			{
				mAllocatedSize = allocateSize;

				Buffer* newBuffer = static_cast<Buffer*>(std::realloc(mBuffer.get(), mAllocatedSize * sizeof(mBuffer)));
				if (newBuffer == nullptr)
					std::bad_alloc();
				mBuffer.release();
				mBuffer.reset(newBuffer);
			}
		}

		void AllocateEnd(const size_t gainSize)
		{
			mUsedSize += ((gainSize + sizeof(mBuffer) - 1) / sizeof(mBuffer));
			MANA_ASSERT(mUsedSize < mAllocatedSize);
		}

		void Deallocate(const size_t releaseSize)
		{
			mUsedSize -= ((releaseSize + sizeof(mBuffer) - 1) / sizeof(mBuffer));
			MANA_ASSERT(mUsedSize < mAllocatedSize);
		}

	private:
		union Buffer
		{
			Actor* mActorPointer;
			const char* mStringPointer;
			void* mVoidPointer;
			int_t mInteger;
			float_t mFloat;
			bool mBool;

			operator Actor*() const { return mActorPointer; }
			operator const char*() const { return mStringPointer; }
			operator void*() const { return mVoidPointer; }
			operator int32_t() const { return static_cast<int32_t>(mInteger); }
			operator int64_t() const { return static_cast<int64_t>(mInteger); }
			operator float() const { return static_cast<float>(mFloat); }
			operator double() const { return static_cast<double>(mFloat); }
			operator bool() const { return mBool; }

			void Set(Actor* actorPointer) { mActorPointer = actorPointer; }
			void Set(const char* stringPointer) { mStringPointer = stringPointer; }
			void Set(void* voidPointer) { mVoidPointer = voidPointer; }
			void Set(int32_t value) { mInteger = static_cast<int_t>(value); }
			void Set(int64_t value) { mInteger = static_cast<int_t>(value); }
			void Set(float value) { mFloat = static_cast<float_t>(value); }
			void Set(double value) { mFloat = static_cast<float_t>(value); }
			void Set(bool value) { mBool = value; }
		};
		std::unique_ptr<Buffer, decltype(&std::free)> mBuffer;
		size_t mAllocatedSize = 0;
		size_t mUsedSize = 0;
	};
}
