/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include "../runner/common/Noncopyable.h"
#include <cstdlib>
#include <memory>
#include <string_view>
#include <vector>

namespace mana
{
	class OutputStream;

	/*
	Data Section
	データセクション
	*/
	class DataBuffer final : Noncopyable
	{
	public:
		DataBuffer() noexcept;
		~DataBuffer() = default;

		[[nodiscard]] const char* GetBuffer() const noexcept;

		[[nodiscard]] address_t GetSize() const noexcept;

		[[nodiscard]] address_t Get(const std::string_view& text) const noexcept;

		address_t Set(const std::string_view& text);

		bool Write(OutputStream& stream) const;

	private:
		[[nodiscard]] address_t Find(const std::string_view& text) const noexcept;

	private:
		std::vector<address_t> mEntities;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		address_t mAllocatedSize = 0;
		address_t mUsedSize = 0;
	};
}
