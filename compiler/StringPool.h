/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include "../runner/common/Noncopyable.h"
#include <stdexcept>
#include <memory>
#include <string_view>
#include <vector>

namespace mana
{
	class StringPool final : private Noncopyable
	{
	public:
		StringPool();
		~StringPool() = default;

		const std::string_view Get(const std::string_view text) const;
		const std::string_view Set(const std::string_view text);

	private:
		address_t Find(const std::string_view& text) const noexcept;
		const std::string_view Text(const address_t address) const;

	private:
		std::vector<address_t> mAddress;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		address_t mUsedSize = 0;
		address_t mAllocatedSize = 0;
	};
}
