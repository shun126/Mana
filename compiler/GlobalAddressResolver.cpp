/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "GlobalAddressResolver.h"
#include "CodeBuffer.h"
#include "Symbol.h"

namespace mana
{
	GlobalAddressResolver::GlobalAddressResolver(const std::shared_ptr<CodeBuffer>& codeBuffer)
		: mCodeBuffer(codeBuffer)
	{
	}

	void GlobalAddressResolver::AddCallList(const int32_t address, const std::shared_ptr<Symbol>& symbol)
	{
		mCallList.emplace_back(std::make_unique<CallList>(address, symbol));
	}

	void GlobalAddressResolver::ResolveAddress() const
	{
		for (const auto& callList : mCallList)
		{
			mCodeBuffer->ReplaceAddress(
				callList->mAddress,
				callList->mSymbol->GetAddress()
			);
		}
	}
}
