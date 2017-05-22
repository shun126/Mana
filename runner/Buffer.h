/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Setup.h"

namespace mana
{
	class Buffer : private Noncopyable
	{
	public:
		Buffer();

		explicit Buffer(const size_t size);

		~Buffer() = default;

#if 0
		void Serialize(mana_stream* stream) const;
		void Deserialize(mana_stream* stream);
#endif

		void Clear();

		void Reset();

		void Allocate(const size_t size);

		void Release(const size_t size);
	
		template<typename T>
		T* GetAddressFromTop(const size_t index) const;

		template<typename T>
		T* GetAddressFromBottom(const size_t index) const;

		size_t GetSize() const;
	
		void SetSize(const size_t size);

#if 0
		//bool mana_frame_compare(const mana_frame* other);	
#endif	

	private:
		std::unique_ptr<void, decltype(&std::free)> mBuffer;
		size_t mAllocatedSize = 0;
		size_t mUsedSize = 0;
	};
}

#include "Buffer.inl"
