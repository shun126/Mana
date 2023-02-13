/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "SemanticAnalyzer.h"

namespace mana
{
	class SyntaxNode;

	class GlobalSemanticAnalyzer final : public SemanticAnalyzer
	{
	public:
		GlobalSemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);
		~GlobalSemanticAnalyzer() = default;

		/*!
		Registers symbol information while traversing nodes
		(but does not evaluate to intermediate languages)
		*/
		void Resolve(std::shared_ptr<SyntaxNode> node);

	private:
		size_t CalcArgumentCount(const size_t count, const std::shared_ptr<const SyntaxNode>& node) const;

	private:
		bool mStaticBlockOpend = false;
	};	
}
