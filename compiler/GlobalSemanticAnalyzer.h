/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "SemanticAnalyzer.h"
#include <vector>

namespace mana
{
	class NamespaceRegistry;
	class SyntaxNode;

	/*
	構造体、アクター、モジュール、関数などグローバルな範囲の文法を解析します
	*/
	class GlobalSemanticAnalyzer final : public SemanticAnalyzer
	{
	public:
		GlobalSemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory,
			const std::shared_ptr<StringPool>& stringPool,
			const std::shared_ptr<NamespaceRegistry>& namespaceRegistry);
		~GlobalSemanticAnalyzer() override = default;

		/*!
		Registers symbol information while traversing nodes
		(but does not evaluate to intermediate languages)
		*/
		void Resolve(std::shared_ptr<SyntaxNode> node);

	private:
		static size_t CalcArgumentCount(const size_t count, const std::shared_ptr<const SyntaxNode>& node);
		std::string_view QualifyName(const std::string_view& name);
		std::string_view QualifyNameIfUnqualified(const std::string_view& name);
		void EnterNamespace(const std::string_view& name);
		void ExitNamespace();
		void RegisterNamespaceHierarchy(const std::string_view& fullName);

	private:
		bool mStaticBlockOpened = false;
		std::vector<std::string_view> mNamespaceStack;
		std::shared_ptr<NamespaceRegistry> mNamespaceRegistry;
	};	
}
