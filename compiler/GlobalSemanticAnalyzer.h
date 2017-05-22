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

	class GlobalSemanticAnalyzer : public SemanticAnalyzer
	{
	public:
		GlobalSemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<SymbolTable>& symbolTable,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);
		virtual ~GlobalSemanticAnalyzer();

		/*!
		ノードを辿りながらシンボル情報を登録します（ただし中間言語に対して評価しません）
		@param	node	ノード
		*/
		void Resolve(std::shared_ptr<SyntaxNode> node);

	private:
		size_t CalcArgumentCount(const size_t count, const std::shared_ptr<const SyntaxNode>& node) const;

	private:
		bool mStaticBlockOpend = false;
	};	
}
