/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "../runner/common/Noncopyable.h"
#include <memory>

namespace mana
{
	class CodeBuffer;
	class CodeGenerator;
	class DataBuffer;
	class GlobalSemanticAnalyzer;
	class LocalSemanticAnalyzer;
	class SymbolFactory;
	class SymbolTable;
	class StringPool;
	class TypeDescriptorFactory;

	class SystemHolder final : private Noncopyable
	{
	public:
		SystemHolder() = default;
		~SystemHolder() = default;

		void Initialize();
		void Finalize();

		const std::shared_ptr<CodeBuffer>& GetCodeBuffer();
		const std::shared_ptr<CodeGenerator>& GetCodeGenerator();
		const std::shared_ptr<DataBuffer>& GetDataBuffer();
		const std::shared_ptr<GlobalSemanticAnalyzer>& GetGlobalSemanticAnalyzer();
		const std::shared_ptr<SymbolFactory>& GetSymbolFactory();
		const std::shared_ptr<SymbolTable>& GetSymbolTable();
		const std::shared_ptr<StringPool>& GetStringPool();
		const std::shared_ptr<TypeDescriptorFactory>& GetTypeDescriptorFactory();

	private:
		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<CodeGenerator> mCodeGenerator;
		std::shared_ptr<DataBuffer> mDataBuffer;
		std::shared_ptr<GlobalSemanticAnalyzer> mGlobalSemanticAnalyzer;
		std::shared_ptr<LocalSemanticAnalyzer> mLocalSemanticAnalyzer;
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<SymbolTable> mSymbolTable;
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;
	};
}
