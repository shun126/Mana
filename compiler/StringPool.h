/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include <memory>
#include <string_view>
#include <vector>

namespace mana
{
	/**
	 * Pool strings while removing duplicate strings
	 */
	class StringPool final
	{
	public:
		/**
		 * Creates StringPool object
		 */
		StringPool();
		StringPool(const StringPool& other) = delete;
		StringPool(StringPool&& other) noexcept = delete;
		StringPool& operator=(const StringPool& other) = delete;
		StringPool& operator=(StringPool&& other) noexcept = delete;

		/**
		 * Destroy StringPool object
		 */
		~StringPool() = default;

		/**
		 * Searches for pooled strings
		 * @param[in]	text	String to search
		 * @return				Registered string. Note that the address is not the same as the searched string.
		 */
		[[nodiscard]] std::string_view Get(const std::string_view& text) const;

		/**
		 * Register strings in the string pool.
		 * @param[in]	text String to be registered.
		 * @return		Registered string. Note that the address is different from the string to be registered.
		 */
		std::string_view Set(const std::string_view& text);

	private:
		[[nodiscard]] address_t Find(const std::string_view& text) const noexcept;
		[[nodiscard]] std::string_view Text(const address_t address) const;

	private:
		std::vector<address_t> mAddress;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		address_t mUsedSize = 0;
		address_t mAllocatedSize = 0;
	};
}
