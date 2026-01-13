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
#include "SymbolFactory.h"
#include "SyntaxNode.h"
#include "TypeDescriptorFactory.h"
#include <memory>
#include <vector>

namespace mana
{
	/*!
	コード生成クラス
	*/
	class CodeGenerator final : Noncopyable, public std::enable_shared_from_this<CodeGenerator>
	{
	public:
		CodeGenerator() = delete;
		explicit CodeGenerator(
			const std::shared_ptr<CodeBuffer>& codeBuffer,
			const std::shared_ptr<DataBuffer>& dataBuffer,
			const std::shared_ptr<GlobalSemanticAnalyzer>& globalSemanticAnalyzer,
			const std::shared_ptr<LocalSemanticAnalyzer>& localSemanticAnalyzer,
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);

		/*!
		ノードを辿りながら中間言語に翻訳します
		@param	node			ノード
		@param	enableLoad		trueならばload命令は有効、falseならばload命令は無効
		*/
		void GenerateCode(std::shared_ptr<SyntaxNode> node, const int32_t enableLoad);

		/*!
		ノードを巡りながら指揮を中間言語に翻訳します
		*/
		void Expression(const std::shared_ptr<SyntaxNode>& tree, const int32_t enableAssign);

		/*!
		グローバルアドレスを解決する
		*/
		const std::shared_ptr<GlobalAddressResolver>& GetGlobalAddressResolver();

		/*!
		ローカルアドレスを解決する
		*/
		const std::shared_ptr<LocalAddressResolver>& GetLocalAddressResolver();

		void Dump(std::ofstream& output) const;

		void FinalizeGlobalInitializers();

		[[nodiscard]] const std::vector<uint8_t>& GetGlobalInitData() const noexcept;

	private:
		struct ConstantValue final
		{
			bool mIsFloat = false;
			int32_t mInt = 0;
			float mFloat = 0.0f;
		};

		bool TryEvaluateConstantExpression(const std::shared_ptr<SyntaxNode>& node, ConstantValue& value) const;
		bool TryHandleGlobalConstInitializer(const std::shared_ptr<Symbol>& symbol, const std::shared_ptr<SyntaxNode>& expression);
		void StoreGlobalInitValue(const std::shared_ptr<Symbol>& symbol, const ConstantValue& value);
		void EnsureGlobalInitDataSize(const size_t size);
		void QueueGlobalRuntimeInitializer(const std::shared_ptr<SyntaxNode>& expression);
		void EmitGlobalInitActor();

		void ResolveLoad(const std::shared_ptr<SyntaxNode>& node) const;
		void ResolveStore(const std::shared_ptr<SyntaxNode>& node) const;
		void Return(const std::shared_ptr<Symbol>& function, const std::shared_ptr<SyntaxNode>& tree);
		void Rollback(const std::shared_ptr<SyntaxNode>& tree);
		int32_t Argument(int32_t count, const std::shared_ptr<Symbol>& param, std::shared_ptr<SyntaxNode> arg);
		static int32_t CallArgumentSize(int32_t address, const std::shared_ptr<Symbol>& param, std::shared_ptr<SyntaxNode> arg);
		int32_t CallArgument(int32_t address, const std::shared_ptr<Symbol>& param, std::shared_ptr<SyntaxNode> arg);
		void Call(const std::shared_ptr<SyntaxNode>& node);
		int32_t CallPrintGenerateArgument(int32_t argc, const std::shared_ptr<SyntaxNode>& node);
		void CallPrint(const std::shared_ptr<SyntaxNode>& node);
		void ConditionCheck(const std::shared_ptr<SyntaxNode>& tree);
		address_t ConditionCore(const std::shared_ptr<SyntaxNode>& tree);
		address_t Condition(const std::shared_ptr<SyntaxNode>& tree, const bool match);
		void GenerateConstantInteger(const TypeDescriptor::Id typeId, const int32_t value) const;
		void GenerateConstantFloat(const TypeDescriptor::Id typeId, const float value) const;

	private:
		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<DataBuffer> mDataBuffer;
		std::shared_ptr<GlobalAddressResolver> mGlobalAddressResolver;
		std::shared_ptr<GlobalSemanticAnalyzer> mGlobalSemanticAnalyzer;
		std::shared_ptr<LocalAddressResolver> mLocalAddressResolver;
		std::shared_ptr<LocalSemanticAnalyzer> mLocalSemanticAnalyzer;
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;

		std::shared_ptr<Symbol> mActorSymbolEntryPointer;
		std::shared_ptr<Symbol> mFunctionSymbolEntryPointer;

		std::vector<uint8_t> mGlobalInitData;
		std::vector<std::shared_ptr<SyntaxNode>> mGlobalRuntimeInitializers;
		bool mHasGlobalInitData = false;
	};
}
