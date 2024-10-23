/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Platform.h"

namespace mana
{
	/**
	 * frame heap
	 */
	class Buffer final : Noncopyable
	{
	public:
		Buffer();
		Buffer(const Buffer&) = delete;
		Buffer(Buffer&&) noexcept = delete;
		Buffer& operator=(const Buffer&) = delete;
		Buffer& operator=(Buffer&&) noexcept = delete;
		explicit Buffer(const address_t size);
		~Buffer() = default;

#if 0
		void Serialize(mana_stream* stream) const;
		void Deserialize(mana_stream* stream);
#endif

		/**
		 * Set the size used to 0. No memory is released.
		 */
		void Clear();

		/**
		 * Set the size used and the size secured to 0.
		 */
		void Reset();

		/**
		 * Extend memory block
		 * @param[in]	size	Size to expand memory
		 */
		void Allocate(const address_t size);

		/**
		 * Changes the size of the memory block used. Memory is not released.
		 * @param[in]	size	Shrinking size
		 */
		void Release(const address_t size);

		/**
		 * Get the address of the specified position from the beginning.
		 * @tparam	T	
		 * @param[in]	index	Position from the top
		 * @return		Address of the specified location
		 */
		template<typename T>
		[[nodiscard]] T* GetAddressFromTop(const address_t index) const;

		/**
		 * Get the address of the specified position from the end
		 * @tparam T 
		 * @param[in]	index	Position from the end
		 * @return		Address of the specified location
		 */
		template<typename T>
		[[nodiscard]] T* GetAddressFromBottom(const address_t index) const;

		/**
		 * Get the size in use.
		 * @return	Usage size
		 */
		[[nodiscard]] address_t GetSize() const;

		/**
		 * Set the size to be used. Do not set a value greater than the allocated memory block.
		 * @param[in]	size	Usage size
		 */
		void SetSize(const address_t size);

		/**
		 * Compare contents
		 * @param[in]	other	Compare Buffer
		 * @return		If true, same content
		 */
		bool operator==(const Buffer& other) const noexcept;

		/**
		 * Compare contents
		 * @param[in]	other	Compare Buffer
		 * @return		If true, different content
		 */
		bool operator!=(const Buffer& other) const noexcept;

	private:
		std::unique_ptr<void, decltype(&std::free)> mBuffer;
		address_t mAllocatedSize = 0;
		address_t mUsedSize = 0;
	};
}

#include "Buffer.inl"
