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
#include <unordered_map>
#include <vector>

namespace mana
{
	class NamespaceRegistry;

	/*
	アクションや関数などローカルな範囲の文法を解析します
	*/
	class LocalSemanticAnalyzer final : public SemanticAnalyzer
	{
	public:
		LocalSemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory,
			const std::shared_ptr<StringPool>& stringPool,
			const std::shared_ptr<NamespaceRegistry>& namespaceRegistry
		);
		~LocalSemanticAnalyzer() override = default;

		void BeginNamespaceScope(const std::string_view& name);
		void EndNamespaceScope();

		void PostResolverResolve(std::shared_ptr<SyntaxNode> node);

	private:
		struct UsingScope
		{
			std::vector<std::string_view> namespacePaths;
			std::unordered_map<std::string_view, std::string_view> symbolAliases;
		};

		[[nodiscard]] std::string_view JoinQualifiedName(const std::string_view& left, const std::string_view& right) const;
		[[nodiscard]] std::string_view GetCurrentNamespace() const;
		[[nodiscard]] bool IsQualifiedName(const std::string_view& name) const;
		[[nodiscard]] std::string_view GetLastSegment(const std::string_view& name) const;
		[[nodiscard]] bool IsActorSymbol(const std::string_view& name) const;
		[[nodiscard]] std::string_view ResolveAlias(const std::string_view& name) const;

		void EnterNamespace(const std::string_view& name);
		void ExitNamespace();
		void ResolveUsingDeclaration(const std::shared_ptr<SyntaxNode>& node);
		void ResolveActionReference(const std::shared_ptr<SyntaxNode>& node, const std::string_view& actionName);
		static bool HasAction(const std::shared_ptr<Symbol>& actorSymbol, const std::string_view& actionName);

		static bool GetNodeType(TypeDescriptor::Id* t1, TypeDescriptor::Id* t2, const std::shared_ptr<const SyntaxNode>& node);
		void AutoCast(const std::shared_ptr<SyntaxNode>& node);

		std::vector<std::string_view> mNamespaceStack;
		std::vector<UsingScope> mUsingScopes;
		std::shared_ptr<NamespaceRegistry> mNamespaceRegistry;
	};
}
