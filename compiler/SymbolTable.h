/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "CodeBuffer.h"
#include "DataBuffer.h"
#include "StringPool.h"
#include "Symbol.h"
#include "SymbolFactory.h"
#include "SyntaxNode.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"

namespace mana
{
	class CodeGenerator;
	class Symbol;

	class SymbolTable final
	{
		static constexpr size_t MANA_SYMBOL_BLOCK_TABLE_SIZE = (256);
		static constexpr size_t nil = static_cast<size_t>(~0);

	public:
		explicit SymbolTable(
			const std::shared_ptr<CodeBuffer>& codeBuffer,
			const std::shared_ptr<DataBuffer>& dataBuffer,
			const std::shared_ptr<StringPool>& stringPool,
			const std::shared_ptr<SymbolFactory>& symbolFactory,
			const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);
		~SymbolTable() = default;

		//const std::shared_ptr<Symbol>& Lookup(const std::string_view name) const;

		int32_t OpenBlock(const bool resetMaxFrameMemoryAddress);
		int32_t CloseBlock();
		bool IsOpenBlock() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// function
		void OpenFunction(const std::shared_ptr<SyntaxNode>& node, const bool is_action);
		void OpenFunction2(const std::shared_ptr<const Symbol>& function);
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
		bool IsActorOrStructerOpened() const { return mActorOrStructureLevel > 0; }

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




		size_t GetReturnAddressList() const
		{
			return mReturnAddressList;
		}

		void SetReturnAddressList(const size_t returnAddressList)
		{
			mReturnAddressList = returnAddressList;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// debug
		void PrintHeader();
		void PrintFooter(const std::string_view, const std::shared_ptr<TypeDescriptor>& type);
		void PrintEntry(const std::shared_ptr<Symbol>& symbolEntry, const std::shared_ptr<TypeDescriptor>& type);
		void PrintDummyGlobalVariable(size_t size);

	private:
		bool GenerateActorEntity(OutputStream& stream, const std::shared_ptr<const Symbol>& symbol, const std::shared_ptr<const TypeDescriptor>& type, const int32_t arraySize) const;

		static int32_t symbol_align_size(const int32_t X, const int32_t Y)
		{
			return (X + Y - 1) / Y * Y;
		}

	private:
		int32_t mActorOrStructureLevel = 0;
		int32_t mFunctionBlockLevel = 0;
		//TODO:int32_t mBlockLevel = 0; mBlockTable.size()で代用可能？
		bool mIsFunctionOpened = false;
		bool mModuleBlockOpened = false;

		int32_t mStaticMemoryAddress = 0;
		int32_t mGlobalMemoryAddress = 0;
		int32_t mActorMemoryAddress = 0;
		int32_t mLocalMemoryAddress = 0;
		int32_t mMaxLocalMemoryAddress = 0;

		int32_t mFrameSizeList;
		size_t mReturnAddressList = nil;

		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<DataBuffer> mDataBuffer;
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;
	};
}
