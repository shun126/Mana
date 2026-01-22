/*!
mana (compiler)

@author	Shun Moriya
@date	2025-
*/

#include "NamespaceRegistry.h"
#include "StringPool.h"

namespace mana
{
	NamespaceRegistry::NamespaceRegistry(const std::shared_ptr<StringPool>& stringPool)
		: mStringPool(stringPool)
	{
	}

	std::string_view NamespaceRegistry::Intern(const std::string_view& name)
	{
		return mStringPool ? mStringPool->Set(name) : name;
	}

	void NamespaceRegistry::RegisterNamespace(const std::string_view& name)
	{
		mNamespaces.insert(Intern(name));
	}

	bool NamespaceRegistry::IsNamespace(const std::string_view& name) const
	{
		return mNamespaces.find(name) != mNamespaces.end();
	}
}
