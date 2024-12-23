/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include <memory>
#include <vector>

namespace mana
{
	class CodeBuffer;
	class Symbol;

	/*
	グローバルアドレスを解決します
	*/
	class GlobalAddressResolver final : Noncopyable
	{
	public:
		explicit GlobalAddressResolver(const std::shared_ptr<CodeBuffer>& codeBuffer);
		~GlobalAddressResolver() = default;

		void AddCallList(const int32_t address, const std::shared_ptr<Symbol>& symbol);
		void ResolveAddress() const;

	private:
		struct CallList final
		{
			std::shared_ptr<Symbol> mSymbol;
			int32_t mAddress;

			CallList(const int32_t address, const std::shared_ptr<Symbol>& symbol)
				: mSymbol(symbol)
				, mAddress(address)
			{}
		};

		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::vector<std::unique_ptr<CallList>> mCallList;
	};
}
