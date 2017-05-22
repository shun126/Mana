/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "StringPool.h"
#include "SymbolEntry.h"
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
	class SymbolFactory
	{
	public:
		/*!
		constructor
		*/
		SymbolFactory(const std::shared_ptr<StringPool>& stringPool, const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);

		/*!
		destructor
		*/
		virtual ~SymbolFactory() = default;

		std::shared_ptr<SymbolEntry> CreateAlias(const char* name, const char* alias);
		std::shared_ptr<SymbolEntry> CreateConstInt(const char* name, const int32_t value);
		std::shared_ptr<SymbolEntry> CreateConstFloat(const char* name, const float value);
		std::shared_ptr<SymbolEntry> CreateConstString(const char* name, const char* text);
		std::shared_ptr<SymbolEntry> CreateVariable(const char* name, const std::shared_ptr<TypeDescriptor>& type, const bool staticVariable, const bool isBlockOpened, const bool isFunctionOpened);
		std::shared_ptr<SymbolEntry> CreateLabel(const char* name);
		std::shared_ptr<SymbolEntry> CreateFunction(const char* name, const bool isActorOrStructerOpened, const bool isModuleBlockOpened);
		std::shared_ptr<SymbolEntry> CreateType(const char* name, const std::shared_ptr<TypeDescriptor>& type);

		void Destroy(const char* name);

		bool IsValid(std::shared_ptr<SymbolEntry> symbolEntry);

		bool Each(std::function<bool(std::shared_ptr<SymbolEntry>)> function)
		{
			for (auto& symbol : mSymbolEntries)
			{
				if (!function(symbol))
					return false;
			}
			return true;
		}

		// Find
		std::shared_ptr<SymbolEntry> Lookup(const char* name) const;
		std::shared_ptr<SymbolEntry> LookupOrCreateDummy(const char* name);

		void Define(const char* name, std::shared_ptr<SymbolEntry> symbolEntry);
		void Define(std::shared_ptr<SymbolEntry> symbolEntry);
		void Undefine(const char* name);
		void Undefine(std::shared_ptr<SymbolEntry> symbolEntry);

		// Block management
		void OpenBlock(const int32_t address);
		int32_t CloseBlock();
		size_t GetBlockDepth() const;
		std::shared_ptr<SymbolEntry> GetLastSymbolEntryInBlock() const;
		void EachBlock(std::function<void(std::shared_ptr<SymbolEntry>)> function)
		{
			if (mBlockTable.empty())
				return;
			for (auto symbol : mBlockTable.top()->mHead)
				function(symbol);
		}

		void Dump(std::ofstream& output) const;
		void DumpFunctionNameFromAddress(std::ofstream& output, const int32_t address) const;

	private:
		std::shared_ptr<SymbolEntry> CreateSymbol(const char* name, const SymbolEntry::ClassTypeId class_type);
		std::shared_ptr<SymbolEntry> CreateSymbolWithAddress(const char* name, const SymbolEntry::ClassTypeId class_type, const int32_t address);
		std::shared_ptr<SymbolEntry> CreateSymbolWithLevel(const char* name, SymbolEntry::ClassTypeId class_type, const int32_t level);

	private:
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;
		std::unordered_map <const char*, std::shared_ptr<SymbolEntry>, HashValueGenerate, HashValueCompare> mHashChainTable;
		std::vector<std::shared_ptr<SymbolEntry>> mSymbolEntries;

		struct BlockTable
		{
			int32_t mAllocp;
			std::vector<std::shared_ptr<SymbolEntry>> mHead;

			BlockTable(const int32_t allocp)
				: mAllocp(allocp)
			{}
		};
		std::stack<std::unique_ptr<BlockTable>> mBlockTable;
	};
}
