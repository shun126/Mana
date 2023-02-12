/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Platform.h"

namespace mana
{
	class Buffer : private Noncopyable
	{
	public:
		Buffer();

		explicit Buffer(const address_t size);

		~Buffer() = default;

#if 0
		void Serialize(mana_stream* stream) const;
		void Deserialize(mana_stream* stream);
#endif

		void Clear();

		void Reset();

		void Allocate(const address_t size);

		void Release(const address_t size);
	
		template<typename T>
		T* GetAddressFromTop(const address_t index) const;

		template<typename T>
		T* GetAddressFromBottom(const address_t index) const;

		address_t GetSize() const;
	
		void SetSize(const address_t size);

		bool operator==(const Buffer& other) const noexcept;
		bool operator!=(const Buffer& other) const noexcept;

	private:
		std::unique_ptr<void, decltype(&std::free)> mBuffer;
		address_t mAllocatedSize = 0;
		address_t mUsedSize = 0;
	};
}

#include "Buffer.inl"
