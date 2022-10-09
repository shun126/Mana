/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "SymbolFactory.h"
#include "CodeBuffer.h"
#include "CodeGenerator.h"
#include "DataBuffer.h"
#include "ErrorHandler.h"
#include "Path.h"
#include "StringPool.h"
#include "Symbol.h"
#include "SyntaxNode.h"
#include "TypeDescriptorFactory.h"

namespace mana
{
	SymbolFactory::SymbolFactory(
		const std::shared_ptr<CodeBuffer>& codeBuffer,
		const std::shared_ptr<DataBuffer>& dataBuffer,
		const std::shared_ptr<StringPool>& stringPool,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
		: mCodeBuffer(codeBuffer)
		, mDataBuffer(dataBuffer)
		, mStringPool(stringPool)
		, mTypeDescriptorFactory(typeDescriptorFactory)
	{
		/*
		TODO
		symbol->next = symbol_block_table[level].head;
		symbol_block_table[level].head = symbol;
		*/		
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbol(const std::string_view name, const Symbol::ClassTypeId class_type)
	{
		return CreateSymbolWithLevel(name, class_type, GetBlockDepth());
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbolWithAddress(const std::string_view name, const Symbol::ClassTypeId class_type, const int32_t address)
	{
		std::shared_ptr<Symbol> symbol = CreateSymbol(name, class_type);
		symbol->SetAddress(address);
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbolWithLevel(const std::string_view name, Symbol::ClassTypeId class_type, const size_t level)
	{
		std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>(name, class_type, level);
		mSymbolEntries.push_back(symbol);
		
		Define(name, symbol);

		if (!mBlockTable.empty())
		{
			if (mBlockTable.top()->mHead.mSymbolEntry)
				symbol->SetNext(mBlockTable.top()->mHead.mSymbolEntry);
			RegisterToBlock(symbol);
		}
		
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateAlias(const std::string_view name, const std::string_view filename)
	{
		std::shared_ptr<Symbol> symbol;
		char path[_MAX_PATH];

		symbol = Lookup(name);
		if(symbol)
			CompileError("duplicated declaration '%s'", name);

		symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::Alias, -1);
		symbol->SetTypeDescription(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Int));

		if (fullpath(path, filename.data(), sizeof(path)))
		{
			symbol->SetString(mStringPool->Set(path));
		}
		else
		{
			CompileError("unable to open \"%s\"", filename);
			symbol->SetString(filename);
		}

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateConstInt(const std::string_view name, const int32_t value)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if(symbol)
			CompileError("duplicated declaration '%s'", name);

		symbol = CreateSymbol(name, Symbol::ClassTypeId::ConstantInteger);
		symbol->SetTypeDescription(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Int));
		symbol->SetEtc(value);
#if 0
		// TODO
		if(mana_variable_header_file)
		{
			fprintf(mana_variable_header_file, "#define _%s %d\n", name, value);
		}
#endif
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateConstFloat(const std::string_view name, const float value)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if(symbol)
			CompileError("duplicated declaration '%s'", name);

		symbol = CreateSymbol(name, Symbol::ClassTypeId::ConstantFloat);
		symbol->SetTypeDescription(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Float));
		symbol->SetFloat(value);
#if 0
		// TODO
		if(mana_variable_header_file)
		{
			fprintf(mana_variable_header_file, "#define _%s %f\n", name, value);
		}
#endif
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateConstString(const std::string_view name, const std::string_view value)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if(symbol)
			CompileError("duplicated declaration '%s'", name);

		symbol = CreateSymbol(name, Symbol::ClassTypeId::ConstantString);
		symbol->SetTypeDescription(mTypeDescriptorFactory->GetString());
		symbol->SetString(value);
#if 0
		// TODO
		if(mana_variable_header_file)
		{
			fprintf(mana_variable_header_file, "#define _%s \"%s\"\n", name, value);
		}
#endif
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateVariable(const std::string_view name, const std::shared_ptr<TypeDescriptor>& type, const bool isStaticVariable, const bool isBlockOpened, const bool isFunctionOpened)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if(symbol == nullptr || symbol->GetBlockLevel() < GetBlockDepth())
		{
			if (isStaticVariable)
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::StaticVariable, 0);
			}
			else if(isBlockOpened == false)
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::GlobalVariable, 0);
			}
			else if(isFunctionOpened == false)
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::ActorVariable, 0);
			}
			else
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::LocalVariable, 0);
			}
			symbol->SetTypeDescription(type);
		}
		else
		{
			CompileError("duplicated declaration '%s'", name.data());
		}

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateLabel(const std::string_view name)
	{
		std::shared_ptr<Symbol> symbol;

		symbol = Lookup(name);
		if(symbol == nullptr)
		{
			symbol = CreateSymbolWithLevel(name, Symbol::ClassTypeId::Label, 0/* TODO:mFunctionBlockLevel*/);
			symbol->SetAddress(-1);
			symbol->SetEtc(-1);
		}

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateFunction(const std::string_view name, const bool isActorOrStructerOpened, const bool isModuleBlockOpened)
	{
		const Symbol::ClassTypeId class_type = isActorOrStructerOpened
			? Symbol::ClassTypeId::MemberFunction : Symbol::ClassTypeId::Function;

		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol == nullptr)
		{
			symbol = CreateSymbolWithAddress(name, class_type, 0);
			symbol->SetClassTypeId(class_type);
			symbol->SetOverride(isModuleBlockOpened);
		}
		else if (symbol->IsOverride() == false || symbol->GetClassTypeId() != class_type)
		{
			CompileError("the function already declared");
		}
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateType(const std::string_view name, const std::shared_ptr<TypeDescriptor>& type)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol == nullptr)
		{
			symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::Type, 0);
			symbol->SetTypeDescription(type);
		}
		else if (symbol->GetTypeDescriptor() == type)
		{
			return symbol;
		}
		else if (
			symbol->GetClassTypeId() == Symbol::ClassTypeId::Type &&
			symbol->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::Incomplete )
		{
			if (
				type->GetId() == TypeDescriptor::Id::Incomplete ||
				type->GetSymbolEntry() == symbol)
			{
				CompileError("illegal reference to an incomplete name");
			}
			else
			{
				symbol->SetTypeDescription(type);
			}
		}
		else
		{
			CompileError("invalid identifier used as a type name");
		}

		type->SetName(name); // TODO:不要？

		return symbol;
	}

	void SymbolFactory::Destroy(const std::string_view name)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol)
			symbol->SetName(nullptr);
	}

	bool SymbolFactory::IsValid(std::shared_ptr<Symbol> symbol)
	{
		if (symbol &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::StaticVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::GlobalVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ActorVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::LocalVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ConstantInteger &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ConstantFloat &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ConstantString
			) {
			CompileError("non-variable name '%s'", symbol->GetName());
			return false;
		}
		else {
			return true;
		}
	}

	std::shared_ptr<Symbol> SymbolFactory::Lookup(const std::string_view name) const
	{
		const auto i = mHashChainTable.find(name);
		return (i == mHashChainTable.cend()) ? nullptr : i->second;
	}
#if 0
TODO:
	std::shared_ptr<Symbol> SymbolFactory::LookupOrCreateDummy(const std::string_view name)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if(symbol == nullptr)
		{
			CompileError("reference to undeclared identifier '%s'", name);

			symbol = CreateSymbol(name, Symbol::ClassTypeId::LocalVariable, 0);
			symbol->SetTypeDescription(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Int));
		}
		return symbol;
	}
#endif

	void SymbolFactory::Define(const std::string_view name, std::shared_ptr<Symbol> symbolEntry)
	{
		mHashChainTable[name] = symbolEntry;
	}

	void SymbolFactory::Define(std::shared_ptr<Symbol> symbolEntry)
	{
		mHashChainTable[symbolEntry->GetName()] = symbolEntry;
	}

	void SymbolFactory::Undefine(const std::string_view name)
	{
		mHashChainTable.erase(name);
	}

	void SymbolFactory::Undefine(std::shared_ptr<Symbol> symbolEntry)
	{
		mHashChainTable.erase(symbolEntry->GetName());
	}

	void SymbolFactory::OpenBlock(const int32_t address)
	{
		mBlockTable.push(std::make_unique<BlockTable>(address));
	}

	int32_t SymbolFactory::CloseBlock()
	{
		// calculate frame buffer size
		if (mLocalMemoryAddress > mMaxLocalMemoryAddress)
			mMaxLocalMemoryAddress = mLocalMemoryAddress;

		// check and update hash table
		const BlockEntry& blockEntry = mBlockTable.top()->mHead;

		// deleting from hash chain
		for (std::shared_ptr<Symbol> symbol = blockEntry.mSymbolEntry; symbol; symbol = symbol->GetNext())
		{
			mHashChainTable.erase(symbol->GetName());
		}

		// check if undefined type
		if (blockEntry.mSymbolEntry->GetClassTypeId() == Symbol::ClassTypeId::Type && (blockEntry.mSymbolEntry->GetTypeDescriptor())->Is(TypeDescriptor::Id::Incomplete))
			CompileError("incomplete type name '%s'", blockEntry.mSymbolEntry->GetName().data());

		mLocalMemoryAddress = mBlockTable.top()->mAllocp;
		mBlockTable.pop();

		return static_cast<int32_t>(GetBlockDepth());
	}

	size_t SymbolFactory::GetBlockDepth() const
	{
		return mBlockTable.size();
	}

	void SymbolFactory::EachBlock(std::function<void(const std::shared_ptr<Symbol>&)> function)
	{
		if (!mBlockTable.empty())
		{
			const BlockEntry& blockEntry = mBlockTable.top()->mHead;
			function(blockEntry.mSymbolEntry);
		}
	}




	void SymbolFactory::ExtendModule(const std::shared_ptr<Symbol>& symbol)
	{
		if (symbol)
		{
			std::shared_ptr<Symbol> lastSymbol = symbol;
			while (lastSymbol->GetNext())
			{
				lastSymbol = lastSymbol->GetNext();
			}

			// Register in block table to release symbol_hash_chain_table in CloseBlock
			lastSymbol->SetNext(mBlockTable.top()->mHead.mSymbolEntry);
			RegisterToBlock(symbol);
		}
	}

	void SymbolFactory::RegisterToBlock(const std::shared_ptr<Symbol>& symbolEntry)
	{
		mBlockTable.top()->mHead.Set(symbolEntry);
	}

	void SymbolFactory::RegisterToBlock(const std::shared_ptr<TypeDescriptor>& typeDescriptor)
	{
		mBlockTable.top()->mHead.Set(typeDescriptor);
	}


	std::shared_ptr<Symbol> SymbolFactory::GetLastSymbolEntryInBlock() const
	{
		if (mBlockTable.empty())
			return nullptr;
		return mBlockTable.top()->mHead.mSymbolEntry;
	}

	std::shared_ptr<TypeDescriptor> SymbolFactory::GetLastTypeDescriptorInBlock() const
	{
		if (mBlockTable.empty())
			return nullptr;
		return mBlockTable.top()->mHead.mTypeDescriptor;
	}




	void SymbolFactory::Dump(std::ofstream& output) const
	{
		for(const std::shared_ptr<Symbol>& symbol : mSymbolEntries)
		{
			if(symbol->GetBlockLevel() == 0)
				symbol->OnDump(output, 0);
		}
	}

	void SymbolFactory::DumpFunctionNameFromAddress(std::ofstream& output, const int32_t address) const
	{
		for (const std::shared_ptr<Symbol>& symbol : mSymbolEntries)
		{
			if (symbol->GetBlockLevel() > 0)
				continue;

			switch (symbol->GetClassTypeId())
			{
			case Symbol::ClassTypeId::Function:
				if (symbol->GetAddress() == address)
				{
					output << symbol->GetName();
					output << "\n";
					return;
				}
				break;

			case Symbol::ClassTypeId::Type:
				if (symbol->GetTypeDescriptor() && symbol->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::Actor)
				{
					for (std::shared_ptr<Symbol> component_symbol = symbol->GetTypeDescriptor()->GetSymbolEntry(); component_symbol; component_symbol = component_symbol->GetNext())
					{
						if (component_symbol->GetClassTypeId() != Symbol::ClassTypeId::MemberFunction)
							continue;

						if (component_symbol->GetAddress() == address)
						{
							output << symbol->GetName();
							output << "::";
							output << component_symbol->GetName();
							output << "\n";
							return;
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}


	int32_t SymbolFactory::OpenBlock(const bool resetMaxFrameMemoryAddress)
	{
		const size_t blockLevel = GetBlockDepth();

		if (blockLevel <= 0 || resetMaxFrameMemoryAddress)
			mMaxLocalMemoryAddress = mLocalMemoryAddress;
		OpenBlock(mLocalMemoryAddress);

		return static_cast<int32_t>(blockLevel);
	}

	bool SymbolFactory::IsOpenBlock() const
	{
		return GetBlockDepth() > 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	void SymbolFactory::OpenFunction(const std::shared_ptr<SyntaxNode>& node, const bool isAction)
	{
		const std::shared_ptr<Symbol>& functionSymbolEntry = node->GetSymbol();
		const std::shared_ptr<TypeDescriptor>& functionTypeDescriptor = node->GetTypeDescriptor();

		OpenBlock(isAction);

		if (!isAction)
		{
			/*
			function(p1, p2);

			+- stack ------------+
			|  4 return address  |
			|  8 p1              |
			| 12 p2              |
			| 16 local variables |
			|          :         |
			+--------------------+
			*/
			mLocalMemoryAddress += sizeof(void*);
		}

#if defined(NO_IMPLEMENT)
		// シンボルリストに引数シンボルを再登録
		RegisterToBlock(functionSymbolEntry->GetParameterList());

		// reregistration hash chain
		for (std::shared_ptr<Symbol> symbol = functionSymbolEntry->GetParameterList(); symbol; symbol = symbol->GetNext())
		{
			Define(symbol);
		}
#endif

		if (isAction)
		{
			mLocalMemoryAddress += sizeof(void*);
		}

		if (functionTypeDescriptor->GetId() == TypeDescriptor::Id::Incomplete)
		{
			CompileError("incomplete data type is used");
		}

		// TODO:  レジスタ割り当て処理をクリア
		//register_clear();

		if (functionSymbolEntry->GetClassTypeId() == Symbol::ClassTypeId::NewSymbol)
		{
			functionSymbolEntry->SetTypeDescription(functionTypeDescriptor);
		}

		// シンボルの設定
		functionSymbolEntry->SetAddress(mCodeBuffer->GetSize());
		functionSymbolEntry->SetEtc(isAction);

		mFunctionBlockLevel = GetBlockDepth();

		// frame bufferの確保する命令を発行
		mFrameSizeList = mCodeBuffer->AddOpecodeAndOperand(MANA_IL_ALLOCATE, InvalidAddress);

		if (!isAction)
		{
			// return addressをframe bufferに保存する命令を発行
			mCodeBuffer->AddOpecode(MANA_IL_SAVE_RETURN_ADDRESS);
		}

		// returnのジャンプ先リンクを初期化
		mReturnAddressList = InvalidAddress;
		mIsFunctionOpened = true;
	}

	/*
	関数の引数をフレームバッファ領域へコピーするコードを出力する
	@param[in]	function	関数を表すSymbol
	*/
	void SymbolFactory::OpenFunction2(const std::shared_ptr<const Symbol>& function)
	{
		for (std::shared_ptr<const Symbol> symbol = function->GetParameterList(); symbol; symbol = symbol->GetNext())
		{
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_LOAD_FRAME_ADDRESS, symbol->GetAddress());

			switch (symbol->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Char:
				mCodeBuffer->AddOpecode(MANA_IL_STORE_CHAR);
				break;

			case TypeDescriptor::Id::Short:
				mCodeBuffer->AddOpecode(MANA_IL_STORE_SHORT);
				break;

			case TypeDescriptor::Id::Int:
				mCodeBuffer->AddOpecode(MANA_IL_STORE_INTEGER);
				break;

			case TypeDescriptor::Id::Float:
				mCodeBuffer->AddOpecode(MANA_IL_STORE_FLOAT);
				break;

			case TypeDescriptor::Id::Actor:
				mCodeBuffer->AddOpecode(MANA_IL_STORE_INTEGER);
				break;

			default:
				if (symbol->GetTypeDescriptor()->GetMemorySize() <= 0)
					CompileError("missing size information on parameter");
				mCodeBuffer->AddOpecodeAndOperand(MANA_IL_STORE_DATA, symbol->GetTypeDescriptor()->GetMemorySize());
				break;
			}
		}
	}

	void SymbolFactory::CloseFunction(const std::shared_ptr<SyntaxNode>& node, const bool isAction)
	{
		MANA_ASSERT(mFunctionBlockLevel == GetBlockDepth());

		if (isAction)
			mDataBuffer->Set(node->GetString());

		// gotoのジャンプ先を更新
		EachBlock([this](const std::shared_ptr<Symbol>& symbol) {
			if (symbol->GetClassTypeId() != Symbol::ClassTypeId::Label)
				return;
			if (symbol->GetAddress() < 0)
				CompileError("label '%s' used but not defined", symbol->GetName());

			mCodeBuffer->ReplaceAddressAll(symbol->GetEtc(), symbol->GetAddress());
			});

		// returnのジャンプ先を更新
		mCodeBuffer->ReplaceAddressAll(mReturnAddressList, mCodeBuffer->GetSize());

		// 直後のジャンプは削除
		if (mReturnAddressList != InvalidAddress)
		{
			mCodeBuffer->Reduce(GetIntermediateLanguageProperty(IntermediateLanguage::MANA_IL_BRA).mSize);
		}

		// アクションならばGetEtcは0以外
		if (node->GetSymbol()->GetEtc() == 0)
		{
			// return addressをレジスタに復帰する命令を発行
			mCodeBuffer->AddOpecode(MANA_IL_LOAD_RETURN_ADDRESS);
		}

		// free命令の発行
		mFrameSizeList = mCodeBuffer->AddOpecodeAndOperand(MANA_IL_FREE, mFrameSizeList);

		// return命令の発行
		if (IsActorOrStructerOpened())
		{
			mCodeBuffer->AddOpecode(MANA_IL_RETURN_FROM_ACTION);
		}
		else
		{
			MANA_VERIFY_MESSAGE(node->GetSymbol()->GetTypeDescriptor(), "type description is null pointer");
			if (node->GetSymbol()->GetTypeDescriptor()->GetId() != TypeDescriptor::Id::Void)
			{
				if (!node->GetSymbol()->IsUsed())
					CompileError("meaningless return value specification");
			}
			mCodeBuffer->AddOpecode(MANA_IL_RETURN_FROM_FUNCTION);
		}

		/*
		frame bufferのサイズを更新

		symbol_open_blockがsymbol_create_functionで呼ばれるので
		symbol_begin_function_registrationにsymbol_open_blockを呼んでいなくても
		ここでsymbol_close_blockを呼び出す必要がある。
		*/
		CloseBlock();
		mIsFunctionOpened = false;
		mCodeBuffer->ReplaceAddressAll(mFrameSizeList, symbol_align_size(mMaxLocalMemoryAddress, sizeof(uint32_t)));
	}

	void SymbolFactory::BeginNativeFunction()
	{
		/*
		function(p1, p2);

		+- stack ------------+
		|  4 return address  |
		|  8 p1              |
		| 12 p2              |
		| 16 local variables |
		|          :         |
		+--------------------+
		*/
		OpenBlock(false);
		mIsFunctionOpened = true;
		mLocalMemoryAddress += sizeof(void*);
	}

	void SymbolFactory::CloseNativeFunction(const std::shared_ptr<Symbol>& function, const std::shared_ptr<TypeDescriptor>& type)
	{
		// 1) check
		if (GetBlockDepth() > 1)
			CompileError("the prototype declaration ignored");

		if (type->GetId() == TypeDescriptor::Id::Incomplete)
			CompileError("incomplete data type is used");

		// 2) initialize function's symbol entry
		function->mClassTypeId = Symbol::ClassTypeId::NativeFunction;
		function->SetTypeDescription(type);
		function->SetParameterList(GetLastSymbolEntryInBlock());

		CloseBlock();
		mIsFunctionOpened = false;
	}

	////////////////////////////////////////////////////////////////////////////////
	// structuer

	// symbol_open_structure
	void SymbolFactory::OpenStructure()
	{
#if 0
		// TODO
				// 1) output header
		if (mana_variable_header_file)
			PrintHeader();
#endif
		// 2) open block
		OpenBlock(false);
		++mActorOrStructureLevel;
		mActorMemoryAddress = 0;
	}

	// symbol_close_structure
	void SymbolFactory::CloseStructure(const std::string_view name)
	{
		MANA_ASSERT(GetBlockDepth() > 0);

		address_t maxAligmentSize = 0;

		// 最も大きいサイズのタイプにアライメントを合わせる
		EachBlock([&maxAligmentSize](const std::shared_ptr<Symbol>& symbol)
			{
				const std::shared_ptr<TypeDescriptor>& typeDescriptor = symbol->GetTypeDescriptor();
				if (maxAligmentSize < typeDescriptor->GetAlignmentMemorySize())
					maxAligmentSize = typeDescriptor->GetAlignmentMemorySize();
			});
#if 0
		if (!mBlockTable.top()->mHead.empty())
		{
			const std::shared_ptr<Symbol>& symbol = mBlockTable.top()->mHead.;
			while (symbol)
			{
				const std::shared_ptr<TypeDescriptor>& typeDescriptor = symbol->GetTypeDescriptor();
				if (maxAligmentSize < typeDescriptor->GetAlignmentMemorySize())
					maxAligmentSize = typeDescriptor->GetAlignmentMemorySize();
				symbol = symbol->GetNext();
			}
		}
#endif

		// 1) create new type description
		std::shared_ptr<TypeDescriptor> newType = mTypeDescriptorFactory->Create(TypeDescriptor::Id::Struct);
		newType->SetSymbolEntry(GetLastSymbolEntryInBlock());
		newType->SetName(name);
		newType->SetAlignmentMemorySize(maxAligmentSize);

		--mActorOrStructureLevel;
		CloseBlock();

		newType->SetMemorySize(symbol_align_size(mActorMemoryAddress, maxAligmentSize));
#if 0 // TODO
		// 3) output header
		if (mana_variable_header_file)
			PrintFooter(name, newType);
#endif
		CreateType(name, newType);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// actor
	void SymbolFactory::symbol_open_actor_register_member(const std::shared_ptr<Symbol>& symbol)
	{
		if (symbol->GetNext())
		{
			symbol_open_actor_register_member(symbol->GetNext());
		}

		Define(symbol);
	}

	void SymbolFactory::symbol_open_actor_register_member(const std::shared_ptr<TypeDescriptor>&)
	{
	}

	void SymbolFactory::BeginRegistrationActor(const std::shared_ptr<Symbol>& symbol)
	{
		if (GetBlockDepth() != 0)
			CompileError("the actor declaration ignored");

		OpenBlock(false);
		++mActorOrStructureLevel;

		mActorMemoryAddress = 0;

		if (symbol)
		{
			std::shared_ptr<TypeDescriptor> type;

			for (type = symbol->GetTypeDescriptor(); type->GetId() == TypeDescriptor::Id::Array; type = type->GetComponent())
				;

			// typeがactorまたはmoduleではない場合、続行不可能
			if (type->GetId() != TypeDescriptor::Id::Actor && type->GetId() != TypeDescriptor::Id::Module)
			{
				CompileError("%s is NOT actor!", symbol->GetName());
			}
			else
			{
				// symbol_close_blockでsymbol_hash_chain_tableを開放する
				Define(type->GetSymbolEntry());

				// TODO:シンボルリストの末端からhashに登録
				symbol_open_actor_register_member(type->GetSymbolEntry());
			}

			// instance変数サイズの再計算
			if (symbol->GetTypeDescriptor())
				mActorMemoryAddress = symbol->GetTypeDescriptor()->GetMemorySize();
		}
	}

	// symbol_commit_registration_actor
	void SymbolFactory::CommitRegistrationActor(const std::string_view name, const std::string_view parent, const std::shared_ptr<TypeDescriptor>& td, const bool phantom)
	{
		std::shared_ptr<TypeDescriptor> type;
		std::shared_ptr<TypeDescriptor> parent_type;

		const std::shared_ptr<Symbol>& symbol = Lookup(name);
		if (symbol)
		{
			for (type = symbol->GetTypeDescriptor(); type->Is(TypeDescriptor::Id::Array); type = type->GetComponent())
				;

			if (type->Is(TypeDescriptor::Id::Actor))
			{
				type->SetSymbolEntry(GetLastSymbolEntryInBlock());

				// actor and phantom check
				if (type->mShare.mActor.mPhantom != phantom)
					CompileError("already declared %s", type->mShare.mActor.mPhantom ? "a phantom" : "an actor");

				// @TODO: actorの宣言が二つある場合、ワーニングを出す？
				goto SKIP;
			}
		}

		mDataBuffer->Set(name);

		if (!parent.empty())
		{
			const std::shared_ptr<Symbol>& parent_symbol = Lookup(parent);
			parent_type = parent_symbol ? parent_symbol->GetTypeDescriptor() : nullptr;
		}
		else
		{
			parent_type = nullptr;
		}

		type = mTypeDescriptorFactory->Create(TypeDescriptor::Id::Actor);
		type->SetSymbolEntry(GetLastSymbolEntryInBlock());
		//TODO:  parent_type);
		type->SetName(name);
		type->SetAlignmentMemorySize(sizeof(uint32_t));
		type->mShare.mActor.mPhantom = phantom;

	SKIP:
		--mActorOrStructureLevel;
		CloseBlock();

		type->SetMemorySize(symbol_align_size(mActorMemoryAddress, sizeof(uint32_t)));

		if (td)
		{
			std::shared_ptr<TypeDescriptor> nested_type;
			for (nested_type = td; nested_type->GetComponent(); nested_type = nested_type->GetComponent())
				;
			nested_type->mComponent = type;

			CreateType(name, td);
		}
		else
		{
			CreateType(name, type);
		}
	}

	bool SymbolFactory::IsActorOrStructerOpened() const
	{
		return mActorOrStructureLevel > 0;
	}

	void SymbolFactory::OpenActor(const std::string_view name)
	{
		if (GetBlockDepth() != 0)
			CompileError("the actor declaration ignored");

		OpenBlock(false);
		++mActorOrStructureLevel;

		const std::shared_ptr<Symbol>& symbol = Lookup(name);
		if (symbol == nullptr)
		{

		}
		else
		{
			std::shared_ptr<TypeDescriptor> type;

			for (type = symbol->GetTypeDescriptor(); type->GetId() == TypeDescriptor::Id::Array; type = type->GetComponent())
				;

			// typeがactorではない場合、続行不可能
			if (type->GetId() != TypeDescriptor::Id::Actor && type->GetId() != TypeDescriptor::Id::Module)
			{
				CompileError("%s is NOT actor!", symbol->GetName());
			}
			else
			{
				// symbol_close_blockでsymbol_hash_chain_tableを開放する
				// TODO:mBlockTable.top().mHead = (const std::shared_ptr<Symbol>&)type->component;

				// シンボルリストの末端からhashに登録
				// TODO: symbol_open_actor_register_member((const std::shared_ptr<Symbol>&)type->component);
			}
		}

		// instance変数サイズの再計算
		mActorMemoryAddress = (symbol && symbol->GetTypeDescriptor()) ? symbol->GetTypeDescriptor()->GetMemorySize() : 0;
	}

	void SymbolFactory::CloseActor()
	{
		--mActorOrStructureLevel;
		CloseBlock();

		/*
		type->memory_size = symbol_align_size(mActorMemoryAddress, IBSZ);

		if (td)
		{
			const std::shared_ptr<TypeDescriptor>& nested_type;
			for (nested_type = td; nested_type->component; nested_type = nested_type->component)
				;
			nested_type->component = type;
			return td;
		}
		else
		{
			return type;
		}
		*/
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// module
	// symbol_begin_registration_module
	void SymbolFactory::BeginRegistrationModule(const std::shared_ptr<Symbol>& parentSymbol)
	{
		if (GetBlockDepth() != 0)
		{
			CompileError("the module declaration ignored");
		}

		OpenBlock(false);
		mModuleBlockOpened = true;
		++mActorOrStructureLevel;

		if (parentSymbol)
		{
			std::shared_ptr<TypeDescriptor> type;
			for (type = parentSymbol->GetTypeDescriptor(); type->Is(TypeDescriptor::Id::Array); type = type->GetComponent())
				;

			// typeがactorではない場合、続行不可能
			if (type->IsNot(TypeDescriptor::Id::Actor) && type->IsNot(TypeDescriptor::Id::Module))
			{
				CompileError("%s is NOT modeule!", parentSymbol->GetName().data());
			}
			else
			{
				// symbol_close_blockでsymbol_hash_chain_tableを開放する
				RegisterToBlock(type->GetComponent());

				// シンボルリストの末端からhashに登録
				// HACK
				//symbol_open_actor_register_member((const std::shared_ptr<Symbol>&)type->component);
				//void SetParent(const std::shared_ptr<Symbol>&parent) { mParent = parent; }
			}
		}

		// instance変数サイズの再計算
		mActorMemoryAddress = 0;
	}

	// symbol_commit_registration_module
	void SymbolFactory::CommitRegistrationModule(const std::string_view name)
	{
		mDataBuffer->Set(name);

		const std::shared_ptr<Symbol>& refernceSymbol = GetLastSymbolEntryInBlock();
		//const std::shared_ptr<TypeDescriptor>& referenceType = GetLastTypeDescriptorInBlock();
		std::shared_ptr<TypeDescriptor> type = mTypeDescriptorFactory->Create(TypeDescriptor::Id::Module);
		//type = type_create(SYMBOL_DATA_TYPE_MODULE, type, NULL);
		//type->SetTypeDescriptor(referenceType);
		type->SetParent(refernceSymbol);

		//, referenceType);
		//type->name = name;
		type->SetName(name);
		//type->alignment_memory_size = IBSZ;
		type->SetAlignmentMemorySize(sizeof(void*));

		--mActorOrStructureLevel;
		mModuleBlockOpened = false;
		CloseBlock();

		//type->SetMemorySize(symbol_align_size(mActorMemoryAddress, IBSZ));
		type->SetMemorySize(symbol_align_size(mActorMemoryAddress, sizeof(void*)));

		CreateType(name, type);
	}

	void SymbolFactory::OpenModule(const std::shared_ptr<Symbol>& symbol)
	{
		MANA_UNUSED_VAR(symbol);
	}

	void SymbolFactory::CloseModule(const std::string_view name)
	{
		MANA_UNUSED_VAR(name);
	}

	// symbol_extend_module
	void SymbolFactory::ExtendModule(const std::string_view name)
	{
		const std::shared_ptr<Symbol>& symbol = Lookup(name);
		//const std::shared_ptr<Symbol>& symbol = Lookup(name);
		if (symbol && symbol->GetTypeDescriptor() && symbol->GetTypeDescriptor()->Is(TypeDescriptor::Id::Module))
		{
			// TODO: const std::shared_ptr<Symbol>& action_symbol = (const std::shared_ptr<Symbol>&)(symbol->GetTypeDescriptor()->component);
			const std::shared_ptr<Symbol>& action_symbol = symbol->GetTypeDescriptor()->GetParent();

			/* シンボルリストの末端からhashに登録 */
			if (action_symbol)
			{
				//ExtendModule(action_symbol);

				std::shared_ptr<Symbol> last_symbol = action_symbol;
				while (last_symbol->GetNext())
				{
					last_symbol = last_symbol->GetNext();
				}
				last_symbol->SetNext(mBlockTable.top()->mHead.mSymbolEntry);

				/* symbol_close_blockでsymbol_hash_chain_tableを開放する為 */
				mBlockTable.top()->mHead.mSymbolEntry = action_symbol;

				symbol_open_actor_register_member(action_symbol);
			}
		}
		else
		{
			CompileError("name followed by \"::\" must be a module");
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// request
	void SymbolFactory::AddRequest(const std::shared_ptr<CodeGenerator>& codeGenerator, const IntermediateLanguage opcode, const std::shared_ptr<SyntaxNode>& level, const std::shared_ptr<SyntaxNode>& actor, const std::string_view action)
	{
		//mana_generator_expression(level, false);

		if (actor && actor->GetTypeDescriptor())
		{
			switch (actor->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Actor:
				codeGenerator->generator_expression(actor, false);
				mCodeBuffer->AddOpecodeAndOperand(opcode, mDataBuffer->Set(action));
				return;

			case TypeDescriptor::Id::Reference:
				if (actor->GetTypeDescriptor()->GetName() == "string")
				{
					IntermediateLanguage code;
					switch (opcode)
					{
					case MANA_IL_REQ:
						code = MANA_IL_DYNAMIC_REQ;
						break;
					case MANA_IL_REQWS:
						code = MANA_IL_DYNAMIC_REQWS;
						break;
					case MANA_IL_REQWE:
						code = MANA_IL_DYNAMIC_REQWE;
						break;
					default:
						goto ABORT;
					}
					codeGenerator->generator_expression(actor, false);
					mCodeBuffer->AddOpecodeAndOperand(code, mDataBuffer->Set(action));
					return;
				}

			default:
				break;
			}
		}
	ABORT:
		CompileError("incompatible type of operand");
	}

	void SymbolFactory::AddJoin(const std::shared_ptr<CodeGenerator>& codeGenerator, const std::shared_ptr<SyntaxNode>& level, const std::shared_ptr<SyntaxNode>& actor)
	{
		if (actor && actor->GetTypeDescriptor())
		{
			switch (actor->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Reference:
				if (actor->GetTypeDescriptor()->GetName() == "string")
					break;

			case TypeDescriptor::Id::Actor:
				codeGenerator->generator_expression(actor, false);
				codeGenerator->generator_expression(level, false);
				mCodeBuffer->AddOpecode(MANA_IL_JOIN);
				return;

			default:
				break;
			}
		}

		CompileError("incompatible type of operand");
	}

	void SymbolFactory::AllocateMemory(const std::shared_ptr<Symbol>& symbol, std::shared_ptr<TypeDescriptor> type, Symbol::MemoryTypeId parameter)
	{
		if (type->Is(TypeDescriptor::Id::Incomplete) || type->Is(TypeDescriptor::Id::Void))
		{
			CompileError("incomplete type name or void is used for declraration");
			type = mTypeDescriptorFactory->Get(TypeDescriptor::Id::Int);
		}
#if 0
		// TODO
		if (mana_variable_header_file)
		{
			PrintEntry(symbol, type);
		}
#endif

		if (symbol->GetTypeDescriptor() == nullptr)		// 配列型以外の変数 ?
		{
			symbol->SetTypeDescription(type);			// 型の設定
		}
		else if (symbol->GetTypeDescriptor() == type)	// エラーの可能性がある
		{
		}
		else
		{														// 配列型の変数の処理
			symbol->GetTypeDescriptor()->SetArray(type);		// 配列型リストの設定
			type = symbol->GetTypeDescriptor();
		}

		if (type->GetMemorySize() > 0)
		{
			switch (symbol->GetClassTypeId())
			{
			case Symbol::ClassTypeId::StaticVariable:
				symbol->SetAddress(symbol_align_size(mStaticMemoryAddress, type->GetAlignmentMemorySize()));
				mStaticMemoryAddress = symbol->GetAddress() + type->GetMemorySize();
				break;

			case Symbol::ClassTypeId::GlobalVariable:
				symbol->SetAddress(symbol_align_size(mGlobalMemoryAddress, type->GetAlignmentMemorySize()));
				mGlobalMemoryAddress = symbol->GetAddress() + type->GetMemorySize();
				break;

			case Symbol::ClassTypeId::ActorVariable:
				symbol->SetAddress(symbol_align_size(mActorMemoryAddress, type->GetAlignmentMemorySize()));
				mActorMemoryAddress = symbol->GetAddress() + type->GetMemorySize();
				break;

			case Symbol::ClassTypeId::LocalVariable:
				// ローカル変数は後ろから確保される為、他の変数と計算が反対になるので注意
				symbol->SetAddress(symbol_align_size(mLocalMemoryAddress, type->GetAlignmentMemorySize()) + type->GetMemorySize());
				mLocalMemoryAddress = symbol->GetAddress();
				break;

			default:
				MANA_BUG("ivalid class type detect");
				break;
			}
		}
		else
		{
			CompileError("no storage allocated");
		}

		// 仮引数の表示の格納
		symbol->SetMemoryTypeId(parameter);
	}

	int32_t SymbolFactory::GetStaticMemoryAddress()
	{
		return mStaticMemoryAddress;
	}

	void SymbolFactory::SetStaticMemoryAddress(const int32_t size)
	{
		if (mStaticMemoryAddress < size)
			mStaticMemoryAddress = size;
	}

	int32_t SymbolFactory::GetGlobalMemoryAddress()
	{
		return mGlobalMemoryAddress;
	}

	void SymbolFactory::SetGlobalMemoryAddress(const int32_t size)
	{
		if (mGlobalMemoryAddress < size)
			mGlobalMemoryAddress = size;
	}

	size_t SymbolFactory::GetNumberOfActors() const
	{
		size_t count = 0;

		Each([&count](const std::shared_ptr<const Symbol>& symbol) {
			if (symbol->GetBlockLevel() > 0)
				return true;

			MANA_ASSERT(symbol->GetTypeDescriptor());

			if (symbol->GetClassTypeId() == Symbol::ClassTypeId::Type)
			{
				switch (symbol->GetTypeDescriptor()->GetId())
				{
				case TypeDescriptor::Id::Actor:
					++count;
					break;

				case TypeDescriptor::Id::Array:
					if (
						symbol->GetTypeDescriptor()->GetComponent() &&
						symbol->GetTypeDescriptor()->GetComponent()->GetId() == TypeDescriptor::Id::Actor)
						++count;
					break;

				default:
					break;
				}
			}

			return true;
			});

		return count;
	}

	void SymbolFactory::CheckUndefine()
	{
		Each([this](const std::shared_ptr<Symbol>& symbol) {
			if (symbol->GetBlockLevel() > 0)
				return true;

			symbol->symbol_check_undefine_recursive();

			return true;
			});
	}

	void SymbolFactory::PrintHeader()
	{
#if 0
		// TODO
		if (mana_variable_header_file)
		{
			int32_t i;

			for (i = 0; i <= GetBlockDepth(); i++)
			{
				fputc('\t', mana_variable_header_file);
			}
			fprintf(mana_variable_header_file, "typedef struct {\n");
		}
#endif
	}

	void SymbolFactory::PrintFooter(const std::string_view name, const std::shared_ptr<TypeDescriptor>& type)
	{
#if 0
		// TODO
		int32_t i;

		for (i = 0; i <= GetBlockDepth(); i++)
		{
			fputc('\t', mana_variable_header_file);
		}
		fprintf(mana_variable_header_file, "}%s; /* %d byte(s) */\n", name, type->memory_size);
#endif
	}

#if 0
	// TODO
	static void SymbolFactory::symbol_print_entry_core(const std::shared_ptr<Symbol>& symbol, const std::shared_ptr<TypeDescriptor>& type)
	{
		int32_t i;

		for (i = 0; i <= GetBlockDepth(); i++)
		{
			fputc('\t', mana_variable_header_file);
		}
		fprintf(mana_variable_header_file, "%s\t%s", type->name, symbol->name);
		if (symbol->type != nullptr)
		{
			const std::shared_ptr<TypeDescriptor>& symbol_type;
			for (symbol_type = symbol->type; symbol_type; symbol_type = symbol_type->component)
			{
				fprintf(mana_variable_header_file, "[%d]", symbol_type->GetArraySize());
			}
		}
		fprintf(mana_variable_header_file, ";\n");
	}
#endif

	void SymbolFactory::PrintEntry(const std::shared_ptr<Symbol>& symbol, const std::shared_ptr<TypeDescriptor>& type)
	{
#if 0
		// TODO
		if (GetBlockDepth() <= 0)
		{
			symbol_print_entry_core(symbol, type);
		}
		else if (IsActorOrStructerOpened())
		{
			symbol_print_entry_core(symbol, type);
		}
#endif
	}

	void SymbolFactory::PrintDummyGlobalVariable(size_t size)
	{
#if 0
		// TODO
		if (mana_variable_header_file)
		{
			unsigned d1 = (unsigned)ftell(mana_variable_header_file);
			unsigned d2 = (unsigned)rand();
			int32_t i;

			for (i = 0; i <= GetBlockDepth(); i++)
			{
				fputc('\t', mana_variable_header_file);
			}
			fprintf(mana_variable_header_file, "int8_t\tdummy_%x_%x[%d];\n", d1, d2, (int32_t)size);
		}
#endif
	}

	bool SymbolFactory::GenerateActorInfomation(OutputStream& stream) const
	{
		return Each([this, &stream](const std::shared_ptr<const Symbol>& symbol) {
			if (symbol->GetBlockLevel() > 0)
				return true;

			MANA_VERIFY_MESSAGE(symbol->GetTypeDescriptor(), "Null pointer error in Write,Infomation");
			switch (symbol->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Actor:
				if (symbol->GetClassTypeId() == Symbol::ClassTypeId::Type && symbol->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::Actor)
				{
					if (!GenerateActorEntity(stream, symbol, symbol->GetTypeDescriptor()))
						return false;
				}
				break;

			case TypeDescriptor::Id::Array:
			{
				//size_t arraySize = symbol->GetTypeDescriptor()->GetArraySize();
				for (
					std::shared_ptr<TypeDescriptor> nested_type = symbol->GetTypeDescriptor()->GetComponent();
					nested_type != nullptr;
					nested_type = nested_type->GetComponent())
				{
					switch (nested_type->GetId())
					{
					case TypeDescriptor::Id::Actor:
						if (symbol->GetClassTypeId() == Symbol::ClassTypeId::Type && nested_type->GetId() == TypeDescriptor::Id::Actor)
							if (!GenerateActorEntity(stream, symbol, nested_type/*, arraySize*/))
								return false;
						goto ESCAPE;

					case TypeDescriptor::Id::Array:
						//arraySize *= nested_type->GetArraySize();
						break;

					default:
						break;
					}
				}
			}
		ESCAPE:
			default:
				break;
			}
			return true;
			});
	}

	bool SymbolFactory::GenerateActorEntity(OutputStream& stream, const std::shared_ptr<const Symbol>& symbol, const std::shared_ptr<const TypeDescriptor>& type) const
	{
		ActorInfoHeader actorInfoHeader;

		const size_t number_of_actions = type->GetActionCount();
		if (number_of_actions > (1 << (8 * sizeof(actorInfoHeader.mNumberOfActions))))
		{
			LinkerError("Too much actions in %s.\n", symbol->GetName());
			return false;
		}

		memset(&actorInfoHeader, 0, sizeof(actorInfoHeader));
		actorInfoHeader.mName = mDataBuffer->Get(symbol->GetName());
		actorInfoHeader.mNumberOfActions = (uint16_t)number_of_actions;
		actorInfoHeader.mNumberOfInstances = (uint8_t)type->GetArraySize();
		actorInfoHeader.mVariableSize = type->GetMemorySize();

		if (actorInfoHeader.mName == (uint32_t)-1)
		{
			LinkerError("Can't find actor '%s'.\n", symbol->GetName());
			return false;
		}

		if (type->IsPhantom())
		{
			actorInfoHeader.mFlag |= (1 << ActorInfoHeader::Flag::Phantom);
		}

		stream.PushData(&actorInfoHeader, sizeof(actorInfoHeader));

		for (std::shared_ptr<Symbol> component_symbol = type->GetSymbolEntry(); component_symbol; component_symbol = component_symbol->GetNext())
		{
			if (component_symbol->GetClassTypeId() == Symbol::ClassTypeId::MemberFunction)
			{
				ActionInfoHeader actionInfoHeader;

				memset(&actionInfoHeader, 0, sizeof(actionInfoHeader));
				actionInfoHeader.mName = mDataBuffer->Get(component_symbol->GetName());
				actionInfoHeader.mAddress = component_symbol->GetAddress();

				if (actionInfoHeader.mName == (uint32_t)-1)
				{
					LinkerError("Can't find action '%s'.\n", component_symbol->GetName());
					return false;
				}

				stream.PushData(&actionInfoHeader, sizeof(actionInfoHeader));
			}
		}

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////
	// output

	////////////////////////////////////////////////////////////////////////////////
	// dump
#if 0
// TODO
	static void SymbolFactory::type_dump_core(FILE* fp, const const std::shared_ptr<TypeDescriptor>& type)
	{
		if (type)
		{
			fprintf(fp, "[%s]", symbol_data_type_id_name[type->GetId()]);
			if (type->GetId() != TypeDescriptor::Id::Actor &&
				type->GetId() != TypeDescriptor::Id::Module &&
				type->GetId() != TypeDescriptor::Id::Struct)
			{
				type_dump_core(fp, type->component);
			}
		}
	}
#endif
}
