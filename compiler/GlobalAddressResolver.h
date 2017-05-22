/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "SymbolEntry.h"
#include <memory>
#include <vector>

namespace mana
{
	class GlobalAddressResolver : private Noncopyable
	{
		struct CallList
		{
			std::shared_ptr<SymbolEntry> mSymbol;
			int32_t mAddress;

			CallList(int32_t address, const std::shared_ptr<SymbolEntry>& symbol)
				: mAddress(address)
				, mSymbol(symbol)
			{}
		};

	public:
		explicit GlobalAddressResolver(const std::shared_ptr<CodeBuffer>& codeBuffer);
		virtual ~GlobalAddressResolver();

		void AddCallList(const int32_t code_address, const std::shared_ptr<SymbolEntry>& symbol);
		void ResolveAddress();

	private:
		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::vector<std::unique_ptr<CallList>> mCallList;
	};

	inline GlobalAddressResolver::GlobalAddressResolver(const std::shared_ptr<CodeBuffer>& codeBuffer)
		: mCodeBuffer(codeBuffer)
	{
	}

	inline GlobalAddressResolver::~GlobalAddressResolver()
	{
	}

	inline void GlobalAddressResolver::AddCallList(const int32_t codeAddress, const std::shared_ptr<SymbolEntry>& symbol)
	{
		mCallList.push_back(std::make_unique<CallList>(codeAddress, symbol));
	}

	inline void GlobalAddressResolver::ResolveAddress()
	{
		for(const auto& callList : mCallList)
		{
			mCodeBuffer->ReplaceAddress(
				callList->mAddress,
				callList->mSymbol->GetAddress()
			);
		}
	}
}
