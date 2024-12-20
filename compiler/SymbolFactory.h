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
#include <functional>
#include <memory>
#include <stack>
#include <unordered_map>
#include <vector>

namespace mana
{
	class CodeBuffer;
	class CodeGenerator;
	class DataBuffer;
	class StringPool;
	class Symbol;
	class SyntaxNode;
	class TypeDescriptorFactory;

	class SymbolFactory final : Noncopyable
	{
	public:
		// deprecated
		bool IsValid(std::shared_ptr<Symbol> symbolEntry);


		SymbolFactory(
			const std::shared_ptr<CodeBuffer>& codeBuffer,
			const std::shared_ptr<DataBuffer>& dataBuffer,
			const std::shared_ptr<StringPool>& stringPool,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory
		);
		~SymbolFactory() = default;

		// create symbol
		std::shared_ptr<Symbol> CreateConstInt(const std::string_view name, const int32_t value);
		std::shared_ptr<Symbol> CreateConstFloat(const std::string_view name, const float value);
		std::shared_ptr<Symbol> CreateConstString(const std::string_view name, const std::string_view text);
		std::shared_ptr<Symbol> CreateVariable(const std::string_view name, const std::shared_ptr<TypeDescriptor>& type, const bool staticVariable, const bool isBlockOpened, const bool isFunctionOpened);
		std::shared_ptr<Symbol> CreateLabel(const std::string_view name);
		std::shared_ptr<Symbol> CreateFunction(const std::string_view name, const bool isActorOrStructOpened, const bool isModuleBlockOpened);
		std::shared_ptr<Symbol> CreateType(const std::string_view name, const std::shared_ptr<TypeDescriptor>& type);
		void Destroy(const std::string_view name);

		bool Each(std::function<bool(const std::shared_ptr<Symbol>&)> function)
		{
			for (auto& symbol : mSymbolEntries)
			{
				if (!function(symbol))
					return false;
			}
			return true;
		}

		bool Each(std::function<bool(const std::shared_ptr<const Symbol>&)> function) const
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
		//std::shared_ptr<Symbol> LookupOrCreateDummy(const std::string_view name);

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



		////////////////////////////////////////////////////////////////////////////////////////////////////
		int32_t OpenBlock(const bool resetMaxFrameMemoryAddress);
		bool IsOpenBlock() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// function
		void OpenFunction(const std::shared_ptr<SyntaxNode>& node, const bool is_action);
		void OpenFunction2(const std::shared_ptr<const Symbol>& function) const;
		void CloseFunction(const std::shared_ptr<SyntaxNode>& node, const bool is_action);
		bool IsFunctionOpened() const { return mIsFunctionOpened; }

		void BeginNativeFunction();
		void CloseNativeFunction(const std::shared_ptr<Symbol>& function, const std::shared_ptr<TypeDescriptor>& type);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// struct
		void OpenStructure();
		void CloseStructure(const std::string_view name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// actor
		void BeginRegistrationActor(const std::shared_ptr<Symbol>& symbolEntry);
		void CommitRegistrationActor(const std::string_view name, const std::string_view parent, const std::shared_ptr<TypeDescriptor>& type, const bool phantom);
		bool IsActorOrStructerOpened() const;

		void OpenActor(const std::string_view name);
		void CloseActor();
		size_t GetNumberOfActors() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// module
		void BeginRegistrationModule(const std::shared_ptr<Symbol>& symbolEntry);
		void CommitRegistrationModule(const std::string_view name);

		void OpenModule(const std::shared_ptr<Symbol>& symbolEntry);
		void CloseModule(const std::string_view name);
		bool IsModuleOpened() const { return mModuleBlockOpened; }

		void ExtendModule(const std::string_view name);

		////////////////////////////////////////////////////////////////////////////////////////////////////

		void AddRequest(const std::shared_ptr<CodeGenerator>& codeGenerator, const IntermediateLanguage opcode, const std::shared_ptr<SyntaxNode>& level, const std::shared_ptr<SyntaxNode>& actor, const std::string_view action);
		void AddJoin(const std::shared_ptr<CodeGenerator>& codeGenerator, const std::shared_ptr<SyntaxNode>& level, const std::shared_ptr<SyntaxNode>& actor);

		void AllocateMemory(const std::shared_ptr<Symbol>& symbolEntry, std::shared_ptr<TypeDescriptor> type, Symbol::MemoryTypeId);


		//const std::shared_ptr<Symbol>& symbol_get_head_symbol();

		int32_t GetStaticMemoryAddress();
		void SetStaticMemoryAddress(const int32_t size);

		int32_t GetGlobalMemoryAddress();
		void SetGlobalMemoryAddress(const int32_t size);

		void CheckUndefine();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GenerateActorInfomation(OutputStream& stream) const;




		void symbol_open_actor_register_member(const std::shared_ptr<Symbol>& symbol);
		void symbol_open_actor_register_member(const std::shared_ptr<TypeDescriptor>& typeDescriptor);




		address_t GetReturnAddressList() const
		{
			return mReturnAddressList;
		}

		void SetReturnAddressList(const address_t returnAddressList)
		{
			mReturnAddressList = returnAddressList;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Dump
		void Dump(std::ofstream& output) const;
		void DumpFunctionNameFromAddress(std::ofstream& output, const int32_t address) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// debug
		void PrintHeader();
		void PrintFooter(const std::string_view, const std::shared_ptr<TypeDescriptor>& type);
		void PrintEntry(const std::shared_ptr<Symbol>& symbolEntry, const std::shared_ptr<TypeDescriptor>& type);
		void PrintDummyGlobalVariable(size_t size);

	private:
		std::shared_ptr<Symbol> CreateSymbol(const std::string_view name, const Symbol::ClassTypeId class_type);
		std::shared_ptr<Symbol> CreateSymbolWithAddress(const std::string_view name, const Symbol::ClassTypeId class_type, const int32_t address);
		std::shared_ptr<Symbol> CreateSymbolWithLevel(const std::string_view name, Symbol::ClassTypeId class_type, const size_t blockLevel);


		bool GenerateActorEntity(OutputStream& stream, const std::shared_ptr<const Symbol>& symbol, const std::shared_ptr<const TypeDescriptor>& type) const;

		static int32_t symbol_align_size(const int32_t X, const int32_t Y)
		{
			return (X + Y - 1) / Y * Y;
		}


	private:
		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<DataBuffer> mDataBuffer;
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;

		std::unordered_map<std::string_view, std::shared_ptr<Symbol>> mHashChainTable;
		std::vector<std::shared_ptr<Symbol>> mSymbolEntries;

		struct BlockEntry final
		{
			BlockEntry() = default;
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

		struct BlockTable final
		{
			BlockEntry mHead;
			int32_t mAllocp;

			explicit BlockTable(const int32_t allocp)
				: mAllocp(allocp)
			{}
		};
		std::stack<std::unique_ptr<BlockTable>> mBlockTable;


		ssize_t mActorOrStructureLevel = 0;
		size_t mFunctionBlockLevel = 0;
		bool mIsFunctionOpened = false;
		bool mModuleBlockOpened = false;


		int32_t mStaticMemoryAddress = 0;
		int32_t mGlobalMemoryAddress = 0;
		int32_t mActorMemoryAddress = 0;
		int32_t mLocalMemoryAddress = 0;
		int32_t mMaxLocalMemoryAddress = 0;

		int32_t mFrameSizeList = 0;
		address_t mReturnAddressList = InvalidAddress;
	};
}
