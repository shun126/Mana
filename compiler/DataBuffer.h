/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include "../runner/common/Noncopyable.h"
#include <memory>
#include <string_view>
#include <vector>

namespace mana
{
	class OutputStream;

	/*
	Data Section Class
	*/
	class DataBuffer final : private Noncopyable
	{
	public:
		DataBuffer() noexcept;
		~DataBuffer() = default;

		const char* GetBuffer() const noexcept;

		address_t GetSize() const noexcept;

		address_t Get(const std::string_view& text) const noexcept;

		address_t Set(const std::string_view& text);

		bool Write(OutputStream& stream) const;

	private:
		address_t Find(const std::string_view& text) const noexcept;

	private:
		std::vector<address_t> mEntities;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		address_t mAllocatedSize = 0;
		address_t mUsedSize = 0;
	};
}
