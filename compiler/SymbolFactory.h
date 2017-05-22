/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "StringPool.h"
#include "Symbol.h"
#include "TypeDescriptorFactory.h"
#include <fstream>
#include <functional>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace mana
{
	class SymbolFactory final
	{
	public:
		/*!
		constructor
		*/
		SymbolFactory(const std::shared_ptr<StringPool>& stringPool, const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);

		/*!
		destructor
		*/
		~SymbolFactory() = default;

		std::shared_ptr<Symbol> CreateAlias(const std::string_view name, const std::string_view alias);
		std::shared_ptr<Symbol> CreateConstInt(const std::string_view name, const int32_t value);
		std::shared_ptr<Symbol> CreateConstFloat(const std::string_view name, const float value);
		std::shared_ptr<Symbol> CreateConstString(const std::string_view name, const std::string_view text);
		std::shared_ptr<Symbol> CreateVariable(const std::string_view name, const std::shared_ptr<TypeDescriptor>& type, const bool staticVariable, const bool isBlockOpened, const bool isFunctionOpened);
		std::shared_ptr<Symbol> CreateLabel(const std::string_view name);
		std::shared_ptr<Symbol> CreateFunction(const std::string_view name, const bool isActorOrStructerOpened, const bool isModuleBlockOpened);
		std::shared_ptr<Symbol> CreateType(const std::string_view name, const std::shared_ptr<TypeDescriptor>& type);

		void Destroy(const std::string_view name);

		bool IsValid(std::shared_ptr<Symbol> symbolEntry);

		bool Each(std::function<bool(const std::shared_ptr<Symbol>&)> function)
		{
			for (auto& symbol : mSymbolEntries)
			{
				if (!function(symbol))
					return false;
			}
			return true;
		}

		// Find
		std::shared_ptr<Symbol> Lookup(const std::string_view name) const;
		std::shared_ptr<Symbol> LookupOrCreateDummy(const std::string_view name);

		void Define(const std::string_view name, std::shared_ptr<Symbol> symbolEntry);
		void Define(std::shared_ptr<Symbol> symbolEntry);
		void Undefine(const std::string_view name);
		void Undefine(std::shared_ptr<Symbol> symbolEntry);

		// Block management
		void OpenBlock(const int32_t address);
		int32_t CloseBlock();
		size_t GetBlockDepth() const;
		void EachBlock(std::function<void(const std::shared_ptr<Symbol>&)> function);


		// TODO:相応しい名前か検討して下さい
		void ExtendModule(const std::shared_ptr<Symbol>& symbol);

		void RegisterToBlock(const std::shared_ptr<Symbol>& symbolEntry);
		void RegisterToBlock(const std::shared_ptr<TypeDescriptor>& typeDescriptor);

		std::shared_ptr<Symbol> GetLastSymbolEntryInBlock() const;
		std::shared_ptr<TypeDescriptor> GetLastTypeDescriptorInBlock() const;



		void Dump(std::ofstream& output) const;
		void DumpFunctionNameFromAddress(std::ofstream& output, const int32_t address) const;

	private:
		std::shared_ptr<Symbol> CreateSymbol(const std::string_view name, const Symbol::ClassTypeId class_type);
		std::shared_ptr<Symbol> CreateSymbolWithAddress(const std::string_view name, const Symbol::ClassTypeId class_type, const int32_t address);
		std::shared_ptr<Symbol> CreateSymbolWithLevel(const std::string_view name, Symbol::ClassTypeId class_type, const int32_t level);

	private:
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;
		//std::unordered_map<std::string, std::shared_ptr<Symbol>, HashValueGenerate, HashValueCompare> mHashChainTable;
		std::unordered_map<std::string_view, std::shared_ptr<Symbol>> mHashChainTable;
		std::vector<std::shared_ptr<Symbol>> mSymbolEntries;

		struct BlockEntry
		{
			explicit BlockEntry(const std::shared_ptr<Symbol>& symbolEntry)
				: mSymbolEntry(symbolEntry)
			{
			}
			explicit BlockEntry(const std::shared_ptr<TypeDescriptor>& typeDescriptor)
				: mTypeDescriptor(typeDescriptor)
			{
			}
			void Set(const std::shared_ptr<Symbol>& symbolEntry)
			{
				mSymbolEntry = symbolEntry;
			}
			void Set(const std::shared_ptr<TypeDescriptor>& typeDescriptor)
			{
				mTypeDescriptor = typeDescriptor;
			}

			std::shared_ptr<Symbol> mSymbolEntry;
			std::shared_ptr<TypeDescriptor> mTypeDescriptor;
		};

		struct BlockTable
		{
			int32_t mAllocp;
			std::vector<BlockEntry> mHead;

			BlockTable(const int32_t allocp)
				: mAllocp(allocp)
			{}
		};
		std::stack<std::unique_ptr<BlockTable>> mBlockTable;
	};
}
