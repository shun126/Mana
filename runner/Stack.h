/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "common/Platform.h"
#include "common/Noncopyable.h"
#include "common/OutputStream.h"
#include <memory>

namespace mana
{
	class Actor;

	/**
	 * Stack memory for VM, allocating memory from the heap on each push;
	 * pop does not shrink the memory used.
	 */
	class Stack final : Noncopyable
	{
	public:
		Stack();
		Stack(const Stack&) = delete;
		Stack(Stack&&) noexcept = delete;
		Stack& operator=(const Stack&) = delete;
		Stack& operator=(Stack&&) noexcept = delete;
		~Stack() = default;

		void Serialize(const std::shared_ptr<OutputStream>& stream) const;
		void Deserialize(const std::shared_ptr<OutputStream>& stream);

		/**
		 * Set the size used to 0. No memory is released.
		 */
		void Clear();

		/**
		 * Duplicate the pushed value;
		 * if Data was pushed, the first 8 bytes (4 bytes in 32-bit environment) will be duplicated.
		 */
		void Duplicate();

		/**
		 * Shrink stack by specified size
		 * @param[in]	size	size to shrink
		 */
		void Remove(const address_t size);

		/**
		 * Push value
		 * @tparam		T		Specify the arithmetic or pointer type
		 * @param[in]	value	Value to push
		 */
		template<typename T>
		void Push(T value);

		/**
		 * Push memory contents
		 * @param[in]	buffer	Address to push
		 * @param[in]	size	Size to push
		 */
		void Push(const void* buffer, const address_t size);

		/**
		 * Pop value
		 * @tparam		T		Specify the arithmetic or pointer type
		 * @return		Value to pop
		 */
		template<typename T>
		T Pop();

		/**
		 * Pop memory contents
		 * @param[in]	buffer	Address to pop
		 * @param[in]	size	Size to pop
		 */
		void PopData(void* buffer, const address_t size);

		/**
		 * Get the value of the specified position.
		 * @tparam		T		Specify the arithmetic or pointer type
		 * @param[in]	index	Position to get value
		 * @return		Value to pop
		 */
		template<typename T>
		T Get(const address_t index) const;

		/**
		 * Get the address of the specified location.
		 * @param[in]	index	Position to get address
		 * @return		Address of stack
		 */
		void* GetAddress(const address_t index) const;

		/**
		 * Sets a value at the specified position
		 * @tparam		T		Specify the arithmetic or pointer type
		 * @param[in]	index	Position to set address
		 * @param[in]	value	Value to set
		 */
		template<typename T>
		void Set(const address_t index, T value);

		/**
		 * Get the size of the stack.
		 * @return	Stack Size
		 */
		address_t GetSize() const;

		/**
		 * Set the stack size.
		 * Do not specify a size larger than you have allocated.
		 * @param[in]	size	Stack Size
		 */
		void SetSize(const address_t size);

		/**
		 * Compare stacks.
		 * @param[in]	other	Compare Stacks
		 * @return		If true, same content
		 */
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

			explicit operator Actor*() const { return mActorPointer; }
			explicit operator const char*() const { return mStringPointer; }
			explicit operator void*() const { return mVoidPointer; }
			explicit operator int32_t() const { return static_cast<int32_t>(mInteger); }
			explicit operator int64_t() const { return static_cast<int64_t>(mInteger); }
			explicit operator float() const { return static_cast<float>(mFloat); }
			explicit operator double() const { return static_cast<double>(mFloat); }
			explicit operator bool() const { return mBool; }

			void Set(Actor* actorPointer) { mActorPointer = actorPointer; }
			void Set(const char* stringPointer) { mStringPointer = stringPointer; }
			void Set(void* voidPointer) { mVoidPointer = voidPointer; }
			void Set(const int32_t value) { mInteger = static_cast<int_t>(value); }
			void Set(const int64_t value) { mInteger = static_cast<int_t>(value); }
			void Set(const float value) { mFloat = static_cast<float_t>(value); }
			void Set(const double value) { mFloat = static_cast<float_t>(value); }
			void Set(bool const value) { mBool = value; }
		};
		std::unique_ptr<Buffer, decltype(&std::free)> mBuffer;
		address_t mAllocatedSize = 0;
		address_t mUsedSize = 0;
	};
}

#include "Stack.inl"
