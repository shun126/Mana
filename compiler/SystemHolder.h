/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "CodeBuffer.h"
#include "CodeGenerator.h"
#include "DataBuffer.h"
#include "GlobalAddressResolver.h"
#include "GlobalSemanticAnalyzer.h"
#include "LocalAddressResolver.h"
#include "LocalSemanticAnalyzer.h"
#include "SymbolFactory.h"
#include "SymbolTable.h"
#include "SyntaxNodeHolder.h"
#include "StringPool.h"
#include "TypeDescriptorFactory.h"
#include <memory>

namespace mana
{
	class SystemHolder : private Noncopyable
	{
	public:
		SystemHolder(){}
		virtual ~SystemHolder() { Finalize(); }

		void Initialize()
		{
			mCodeBuffer = std::make_shared<CodeBuffer>();
			mDataBuffer = std::make_shared<DataBuffer>();
			mSyntaxNodeHolder = std::make_shared<SyntaxNodeHolder>();
			mStringPool = std::make_shared<StringPool>();
			mTypeDescriptorFactory = std::make_shared<TypeDescriptorFactory>();
			mSymbolFactory = std::make_shared<SymbolFactory>(mStringPool, mTypeDescriptorFactory);
			mSymbolTable = std::make_shared<SymbolTable>(mCodeBuffer, mDataBuffer, mStringPool, mSymbolFactory, mTypeDescriptorFactory);
			mLocalSemanticAnalyzer = std::make_shared<LocalSemanticAnalyzer>(mSymbolFactory, mSymbolTable, mTypeDescriptorFactory);
			mGlobalSemanticAnalyzer = std::make_shared<GlobalSemanticAnalyzer>(mSymbolFactory, mSymbolTable, mTypeDescriptorFactory);
			mCodeGenerator = std::make_shared<CodeGenerator>(
				mCodeBuffer,
				mDataBuffer,
				mGlobalSemanticAnalyzer,
				mLocalSemanticAnalyzer,
				mStringPool,
				mSymbolFactory,
				mSymbolTable,
				mTypeDescriptorFactory
			);
		}

		void Finalize()
		{
			mCodeBuffer.reset();
			mCodeGenerator.reset();
			mDataBuffer.reset();
			mGlobalSemanticAnalyzer.reset();
			mLocalSemanticAnalyzer.reset();
			mSymbolFactory.reset();
			mSymbolTable.reset();
			mSyntaxNodeHolder.reset();
			mStringPool.reset();
			mTypeDescriptorFactory.reset();
		}

		const std::shared_ptr<CodeBuffer>& GetCodeBuffer() { return mCodeBuffer; }
		const std::shared_ptr<CodeGenerator>& GetCodeGenerator() { return mCodeGenerator; }
		const std::shared_ptr<DataBuffer>& GetDataBuffer() { return mDataBuffer; }
		const std::shared_ptr<GlobalSemanticAnalyzer>& GetGlobalSemanticAnalyzer() { return mGlobalSemanticAnalyzer; }
		const std::shared_ptr<SymbolFactory>& GetSymbolFactory() { return mSymbolFactory; }
		const std::shared_ptr<SymbolTable>& GetSymbolTable() { return mSymbolTable; }
		const std::shared_ptr<SyntaxNodeHolder>& GetSyntaxNodeHolder() { return mSyntaxNodeHolder; }
		const std::shared_ptr<StringPool>& GetStringPool() { return mStringPool; }
		const std::shared_ptr<TypeDescriptorFactory>& GetTypeDescriptorFactory() { return mTypeDescriptorFactory; }

	private:
		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<CodeGenerator> mCodeGenerator;
		std::shared_ptr<DataBuffer> mDataBuffer;
		std::shared_ptr<GlobalSemanticAnalyzer> mGlobalSemanticAnalyzer;
		std::shared_ptr<LocalSemanticAnalyzer> mLocalSemanticAnalyzer;
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<SymbolTable> mSymbolTable;
		std::shared_ptr<SyntaxNodeHolder> mSyntaxNodeHolder;
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;
	};
}
