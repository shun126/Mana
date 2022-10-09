/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "common/Platform.h"
#include "common/Assert.h"
#include "common/Noncopyable.h"
#include "common/OutputStream.h"
#include <memory>

namespace mana
{
	class Actor;

	class Stack : private Noncopyable
	{
	public:
		Stack();
		virtual ~Stack() = default;

		void Serialize(OutputStream* stream) const;
		void Deserialize(OutputStream* stream);
		void Clear();
		void Duplicate();
		void Remove(const address_t size);

		template<typename T>
		void Push(T value);
	
		void Push(const void* buffer, const address_t size);

		template<typename T>
		T Pop();

		void PopData(void* buffer, const address_t size);
	
		void* PopAddress();
	
		template<typename T>
		T Get(const address_t index) const;

		void* GetAddress(const address_t index) const;
	
		template<typename T>
		void Set(const address_t index, T value);

		address_t GetSize() const;

		void SetSize(const address_t size);

		bool operator==(const Stack& other) const;
	
	private:
		static address_t GetAlignmentSize(const address_t size);

		void AllocateBegin(const address_t gainSize);

		void AllocateEnd(const address_t gainSize);

		void Deallocate(const address_t releaseSize);

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
		address_t mAllocatedSize = 0;
		address_t mUsedSize = 0;
	};
}

#include "Stack.inl"
