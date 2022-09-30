/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "SemanticAnalyzer.h"
#include "SymbolFactory.h"
#include "SyntaxNode.h"
#include "TypeDescriptor.h"

namespace mana
{
	class LocalSemanticAnalyzer final : public SemanticAnalyzer
	{
	public:
		LocalSemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory
		);
		virtual ~LocalSemanticAnalyzer();

		void PostResolverResolve(std::shared_ptr<SyntaxNode> node);

	private:
		bool GetNodeType(TypeDescriptor::Id* t1, TypeDescriptor::Id* t2, const std::shared_ptr<const SyntaxNode>& node);
		void AutoCast(const std::shared_ptr<SyntaxNode>& node);
	};
}
