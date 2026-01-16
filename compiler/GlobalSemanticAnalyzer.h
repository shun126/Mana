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

	/*
	構造体、アクター、モジュール、関数などグローバルな範囲の文法を解析します
	*/
	class GlobalSemanticAnalyzer final : public SemanticAnalyzer
	{
	public:
		GlobalSemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);
		~GlobalSemanticAnalyzer() override = default;

		/*!
		Registers symbol information while traversing nodes
		(but does not evaluate to intermediate languages)
		*/
		void Resolve(std::shared_ptr<SyntaxNode> node);

	private:
		static size_t CalcArgumentCount(const size_t count, const std::shared_ptr<const SyntaxNode>& node);

	private:
		bool mStaticBlockOpened = false;
	};	
}
