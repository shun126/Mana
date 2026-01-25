/*!
mana (compiler)

@author	Shun Moriya
@date	2025-
*/

#pragma once
#include <memory>
#include <string_view>
#include <unordered_set>

namespace mana
{
	class StringPool;

	class NamespaceRegistry final
	{
	public:
		explicit NamespaceRegistry(const std::shared_ptr<StringPool>& stringPool);

		std::string_view Intern(const std::string_view& name);
		void RegisterNamespace(const std::string_view& name);
		[[nodiscard]] bool IsNamespace(const std::string_view& name) const;

	private:
		std::shared_ptr<StringPool> mStringPool;
		std::unordered_set<std::string_view> mNamespaces;
	};
}
