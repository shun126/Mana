/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string_view>

namespace mana
{
	class StringPool final : private Noncopyable
	{
	public:
		StringPool();
		~StringPool() = default;

		const std::string_view Get(const char* text) const;
		const std::string_view Set(const char* text);

	private:
		size_t Find(const char* text) const noexcept;
		const std::string_view Text(const size_t address) const;

	private:
		std::vector<size_t> mAddress;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		size_t mUsedSize = 0;
		size_t mAllocatedSize = 0;
	};
}
