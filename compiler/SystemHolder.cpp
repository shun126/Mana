/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "SystemHolder.h"
#include "CodeBuffer.h"
#include "CodeGenerator.h"
#include "DataBuffer.h"
#include "GlobalAddressResolver.h"
#include "GlobalSemanticAnalyzer.h"
#include "LocalAddressResolver.h"
#include "LocalSemanticAnalyzer.h"
#include "SymbolFactory.h"
#include "SymbolTable.h"
#include "StringPool.h"
#include "TypeDescriptorFactory.h"

namespace mana
{
	void SystemHolder::Initialize()
	{
		mCodeBuffer = std::make_shared<CodeBuffer>();
		mDataBuffer = std::make_shared<DataBuffer>();
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
			mSymbolFactory,
			mSymbolTable,
			mTypeDescriptorFactory
			);
	}

	void SystemHolder::Finalize()
	{
		mCodeBuffer.reset();
		mCodeGenerator.reset();
		mDataBuffer.reset();
		mGlobalSemanticAnalyzer.reset();
		mLocalSemanticAnalyzer.reset();

		mSymbolFactory.reset();
		mSymbolTable.reset();
		mTypeDescriptorFactory.reset();

		mStringPool.reset();
	}

	const std::shared_ptr<CodeBuffer>& SystemHolder::GetCodeBuffer()
	{
		return mCodeBuffer;
	}
	
	const std::shared_ptr<CodeGenerator>& SystemHolder::GetCodeGenerator()
	{
		return mCodeGenerator;
	}

	const std::shared_ptr<DataBuffer>& SystemHolder::GetDataBuffer()
	{
		return mDataBuffer;
	}

	const std::shared_ptr<GlobalSemanticAnalyzer>& SystemHolder::GetGlobalSemanticAnalyzer()
	{
		return mGlobalSemanticAnalyzer;
	}

	const std::shared_ptr<SymbolFactory>& SystemHolder::GetSymbolFactory()
	{
		return mSymbolFactory;
	}

	const std::shared_ptr<SymbolTable>& SystemHolder::GetSymbolTable()
	{
		return mSymbolTable;
	}

	const std::shared_ptr<StringPool>& SystemHolder::GetStringPool()
	{
		return mStringPool;
	}

	const std::shared_ptr<TypeDescriptorFactory>& SystemHolder::GetTypeDescriptorFactory()
	{
		return mTypeDescriptorFactory;
	}
}
