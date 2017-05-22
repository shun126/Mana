/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "Symbol.h"
#include "SymbolFactory.h"
#include "SymbolTable.h"
#include "SyntaxNode.h"

namespace mana
{
	class SemanticAnalyzer : private Noncopyable
	{
	public:
		explicit SemanticAnalyzer(
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<SymbolTable>& symbolTable,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);

		virtual ~SemanticAnalyzer() = default;

		void SetCurrentFileInfomation(const std::shared_ptr<SyntaxNode>& node);
			
		/*!
		mana_symbol_lookupを呼び出しmana_symbolを検索してnodeに設定します
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
		mana_symbol_create_variableを呼び出し
		mana_symbolをnewしてnodeに設定します
		@param[in]	node	Declaratorノード
		*/
		void ResolveDeclarator(const std::shared_ptr<SyntaxNode>& node, const bool isStaticVariable);

		/*!
		両辺のTypeDescriptionとDeclaratorを解決して
		mana_symbol_allocate_memoryを使ってメモリを割り当てます
		@param[in]	node	DeclareVariableノード
		*/
		void ResolveVariableDescription(const std::shared_ptr<SyntaxNode>& node, const Symbol::MemoryTypeId memoryTypeId, const bool isStaticVariable);

		/*!
		子ノードから型を継承する
		@param[in]	node	ノード
		*/
		void ResolveTypeFromChildNode(const std::shared_ptr<SyntaxNode>& node);

	protected:
		std::shared_ptr<Symbol> Lookup(const std::string_view name) const;

		const std::shared_ptr<SymbolFactory>& GetSymbolFactory();
		const std::shared_ptr<SymbolTable>& GetSymbolTable();
		const std::shared_ptr<TypeDescriptorFactory>& GetTypeDescriptorFactory();

	private:
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<SymbolTable> mSymbolTable;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;
	};
}
