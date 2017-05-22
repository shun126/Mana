/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "CodeBuffer.h"
#include "DataBuffer.h"
#include "GlobalAddressResolver.h"
#include "GlobalSemanticAnalyzer.h"
#include "LocalAddressResolver.h"
#include "LocalSemanticAnalyzer.h"
#include "StringPool.h"
#include "SymbolFactory.h"
#include "SymbolTable.h"
#include "SyntaxNode.h"
#include "TypeDescriptorFactory.h"
#include <fstream>
#include <memory>

namespace mana
{
	/*!
	コード生成クラス
	*/
	class CodeGenerator : private Noncopyable, public std::enable_shared_from_this<CodeGenerator>
	{
	public:
		//! Callノードを解決する時に呼ばれるコールバック関数の型
		typedef std::shared_ptr<SyntaxNode> (generator_event_funtion_type)(std::shared_ptr<SyntaxNode>);

	public:
		explicit CodeGenerator(
			const std::shared_ptr<CodeBuffer>& codeBuffer,
			const std::shared_ptr<DataBuffer>& dataBuffer,
			const std::shared_ptr<GlobalSemanticAnalyzer>& globalSemanticAnalyzer,
			const std::shared_ptr<LocalSemanticAnalyzer>& localSemanticAnalyzer,
			const std::shared_ptr<StringPool>& stringPool,
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<SymbolTable>& symbolTable,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);

		virtual ~CodeGenerator() = default;

		/*!
		Callノードを解決する時に呼ばれるコールバック関数を登録します
		@param[in]	name		関数名
		@param[in]	function	コールバック関数
		*/
		void generator_add_event(const char* name, generator_event_funtion_type function);

		/*!
		ノードを辿りながら中間言語に翻訳します
		@param	self			ノード
		@param	enable_load		trueならばload命令は有効、falseならばload命令は無効
		*/
		void generator_genearte_code(std::shared_ptr<SyntaxNode> self, int32_t enable_load);

		/*!
		ノードを巡りながら指揮を中間言語に翻訳します
		*/
		void generator_expression(std::shared_ptr<SyntaxNode> self, int32_t enable_assign);

		/*!
		グローバルアドレスを解決する
		*/
		const std::shared_ptr<GlobalAddressResolver>& GetGlobalAddressResolver() { return mGlobalAddressResolver; }

		/*!
		ローカルアドレスを解決する
		*/
		const std::shared_ptr<LocalAddressResolver>& GetLocalAddressResolver() { return mLocalAddressResolver; }

		void Dump(std::ofstream& output);

	private:
		void generator_resolve_load(std::shared_ptr<SyntaxNode> node);
		void generator_resolve_store(std::shared_ptr<SyntaxNode> node);
		void generator_return(std::shared_ptr<SymbolEntry> func, std::shared_ptr<SyntaxNode> tree);
		void generator_rollback(std::shared_ptr<SyntaxNode> tree);
		int32_t generate_argument(int32_t count, std::shared_ptr<SymbolEntry> param, std::shared_ptr<SyntaxNode> arg);
		int32_t generator_call_argument_size(int32_t address, std::shared_ptr<SymbolEntry> param, std::shared_ptr<SyntaxNode> arg);
		int32_t generator_call_argument(int32_t address, std::shared_ptr<SymbolEntry> param, std::shared_ptr<SyntaxNode> arg);
		void generator_call(std::shared_ptr<SyntaxNode> node);
		int32_t generator_call_print_generate_argument(int32_t argc, std::shared_ptr<SyntaxNode> node);
		void generator_call_print(std::shared_ptr<SyntaxNode> node);
		void generator_condition_check(std::shared_ptr<SyntaxNode> tree);
		int32_t generator_condition_core(std::shared_ptr<SyntaxNode> tree);
		int32_t generator_condition(std::shared_ptr<SyntaxNode> tree, int32_t match);
		void generator_generate_const_int(const TypeDescriptor::Id type_id, const int32_t value);
		void generator_generate_const_float(const TypeDescriptor::Id type_id, const float value);

	private:
		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<DataBuffer> mDataBuffer;
		std::shared_ptr<GlobalAddressResolver> mGlobalAddressResolver;
		std::shared_ptr<GlobalSemanticAnalyzer> mGlobalSemanticAnalyzer;
		std::shared_ptr<LocalAddressResolver> mLocalAddressResolver;
		std::shared_ptr<LocalSemanticAnalyzer> mLocalSemanticAnalyzer;
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<SymbolTable> mSymbolTable;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;

		std::shared_ptr<SymbolEntry> actor_symbol_entry_pointer;
		std::shared_ptr<SymbolEntry> function_symbol_entry_pointer;
		//mana_hash* event_hash;
		int32_t symbol_return_address_list = -1;
		bool static_block_opend;
	};
}
