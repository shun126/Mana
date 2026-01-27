/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "SemanticAnalyzer.h"
#include <unordered_map>
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
		struct UsingScope
		{
			std::vector<std::string_view> namespacePaths;
			std::unordered_map<std::string_view, std::string_view> symbolAliases;
		};

		static size_t CalcArgumentCount(const size_t count, const std::shared_ptr<const SyntaxNode>& node);
		[[nodiscard]] std::string_view JoinQualifiedName(const std::string_view& left, const std::string_view& right) const;
		[[nodiscard]] std::string_view GetCurrentNamespace() const;
		[[nodiscard]] bool IsQualifiedName(const std::string_view& name) const;
		[[nodiscard]] std::string_view GetLastSegment(const std::string_view& name) const;
		[[nodiscard]] bool IsActorSymbol(const std::string_view& name) const;
		[[nodiscard]] bool IsTypeSymbol(const std::string_view& name) const;
		[[nodiscard]] std::string_view ResolveAlias(const std::string_view& name) const;
		std::string_view QualifyName(const std::string_view& name);
		std::string_view QualifyNameIfUnqualified(const std::string_view& name);
		std::string_view ResolveTypeName(const std::string_view& name);
		std::string_view ResolveExtendName(const std::string_view& name) const;
		void ResolveTypeDescriptionScoped(const std::shared_ptr<SyntaxNode>& node);
		void RejectActorTypeName(const std::shared_ptr<SyntaxNode>& node);
		void ResolveVariableDescription(const std::shared_ptr<SyntaxNode>& node, const Symbol::MemoryTypeId memoryTypeId, const bool isStaticVariable);
		void EnterNamespace(const std::string_view& name);
		void ExitNamespace();
		void RegisterNamespaceHierarchy(const std::string_view& fullName);
		void ResolveUsingDeclaration(const std::shared_ptr<SyntaxNode>& node);

	private:
		bool mStaticBlockOpened = false;
		std::vector<std::string_view> mNamespaceStack;
		std::vector<UsingScope> mUsingScopes;
		std::shared_ptr<NamespaceRegistry> mNamespaceRegistry;
	};	
}
