/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "Symbol.h"
#include "SymbolFactory.h"
#include "SyntaxNode.h"

namespace mana
{
	class SemanticAnalyzer : Noncopyable
	{
	public:
		explicit SemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);

		virtual ~SemanticAnalyzer() = default;

		static void SetCurrentFileInformation(const std::shared_ptr<SyntaxNode>& node);
			
		/*!
		Lookupを呼び出しSymbolを検索してNodeに設定します
		@param	node	Identifierノード
		*/
		bool SearchSymbolFromName(const std::shared_ptr<SyntaxNode>& node);
			
		/*!
		nodeに登録されたtypeが無ければシンボルを検索してそのtypeをnodeに設定します
		@param[in]	node	TypeDescriptionノード
		*/
		void ResolveTypeDescription(const std::shared_ptr<SyntaxNode>& node);

		/*!
		type_create_arrayを使って配列の型をnewします
		@param[in]	node	VariableSizeノード
		@return		配列type_description
		*/
		std::shared_ptr<TypeDescriptor> ResolveVariableSize(const std::shared_ptr<SyntaxNode>& node);

		/*!
		Symbol::CreateVariableを呼び出し
		Symbolをnewしてnodeに設定します
		@param[in]	node				Declaratorノード
		@param[in]	isStaticVariable	静的変数
		*/
		void ResolveDeclarator(const std::shared_ptr<SyntaxNode>& node, const bool isStaticVariable);

		/*!
		両辺のTypeDescriptionとDeclaratorを解決して
		SymbolFactory::AllocateMemoryを使ってメモリを割り当てます
		@param[in]	node				DeclareVariableノード
		@param[in]	memoryTypeId		メモリタイプ
		@param[in]	isStaticVariable	静的変数
		*/
		void ResolveVariableDescription(const std::shared_ptr<SyntaxNode>& node, const Symbol::MemoryTypeId memoryTypeId, const bool isStaticVariable);

		/*!
		子ノードから型を継承する
		@param[in]	node	ノード
		*/
		static void ResolveTypeFromChildNode(const std::shared_ptr<SyntaxNode>& node);

	protected:
		[[nodiscard]] std::shared_ptr<Symbol> Lookup(const std::string_view name) const;

		const std::shared_ptr<SymbolFactory>& GetSymbolFactory();
		const std::shared_ptr<TypeDescriptorFactory>& GetTypeDescriptorFactory();

	private:
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;
	};
}
