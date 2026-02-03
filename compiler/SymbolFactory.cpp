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
#include "StringPool.h"
#include "Symbol.h"
#include "SyntaxNode.h"
#include "TypeDescriptorFactory.h"
#include <algorithm>
#include <string_view>
#include <vector>

namespace mana
{
	namespace
	{
		struct RenderedType final
		{
			std::string mBase;
			std::vector<address_t> mArraySizes;
		};

		std::string RenderBaseType(const std::shared_ptr<TypeDescriptor>& type)
		{
			if (!type)
				return "void*";

			switch (type->GetId())
			{
			case TypeDescriptor::Id::Void:
				return "void";
			case TypeDescriptor::Id::Char:
				return "int8_t";
			case TypeDescriptor::Id::Short:
				return "int16_t";
			case TypeDescriptor::Id::Bool:
				return "bool";
			case TypeDescriptor::Id::Int:
				return "int32_t";
			case TypeDescriptor::Id::Float:
				return "float";
			case TypeDescriptor::Id::Reference:
			{
				const std::shared_ptr<TypeDescriptor> component = type->GetComponent();
				const std::string base = component ? RenderBaseType(component) : std::string("void");
				return base + "*";
			}
			case TypeDescriptor::Id::Struct:
			case TypeDescriptor::Id::Actor:
			case TypeDescriptor::Id::Module:
				return std::string(type->GetName());
			case TypeDescriptor::Id::Array:
			case TypeDescriptor::Id::Nil:
			case TypeDescriptor::Id::Incomplete:
			default:
				return "void*";
			}
		}

		RenderedType RenderType(const std::shared_ptr<TypeDescriptor>& type)
		{
			RenderedType rendered;
			std::shared_ptr<TypeDescriptor> current = type;
			while (current && current->Is(TypeDescriptor::Id::Array))
			{
				rendered.mArraySizes.push_back(current->GetArraySize());
				current = current->GetComponent();
			}
			rendered.mBase = RenderBaseType(current);
			return rendered;
		}

		void SplitQualifiedName(const std::string& name, std::vector<std::string_view>& namespaces, std::string_view& baseName)
		{
			namespaces.clear();
			baseName = name;

			size_t start = 0;
			while (true)
			{
				const size_t pos = name.find("::", start);
				if (pos == std::string::npos)
				{
					baseName = std::string_view(name.data() + start, name.size() - start);
					return;
				}

				namespaces.emplace_back(name.data() + start, pos - start);
				start = pos + 2;
			}
		}

		void WriteArraySuffix(std::ofstream& output, const std::vector<address_t>& sizes)
		{
			for (address_t size : sizes)
			{
				output << "[" << size << "]";
			}
		}
	}

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

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbol(const std::string_view name, const Symbol::ClassTypeId classType)
	{
		return CreateSymbolWithLevel(name, classType, GetBlockDepth());
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbolWithAddress(const std::string_view name, const Symbol::ClassTypeId classType, const int32_t address)
	{
		std::shared_ptr<Symbol> symbol = CreateSymbol(name, classType);
		symbol->SetAddress(address);
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbolWithLevel(const std::string_view name, Symbol::ClassTypeId classType, const size_t level)
	{
		auto symbol = std::make_shared<Symbol>(name, classType, level);
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

	std::shared_ptr<Symbol> SymbolFactory::CreateConstInt(const std::string_view name, const int32_t value)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol)
		{
			if (symbol->GetClassTypeId() == Symbol::ClassTypeId::ConstantInteger &&
				symbol->GetEtc() == value)
			{
				return symbol;
			}
			if (symbol->GetBlockLevel() >= GetBlockDepth())
			{
				CompileError({ "duplicated declaration '", name, "'" });
			}
		}

		symbol = CreateSymbol(name, Symbol::ClassTypeId::ConstantInteger);
		symbol->SetTypeDescription(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Int));
		symbol->SetEtc(value);

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateConstBool(const std::string_view name, const bool value)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol)
		{
			if (symbol->GetClassTypeId() == Symbol::ClassTypeId::ConstantInteger &&
				symbol->GetEtc() == static_cast<int32_t>(value))
			{
				return symbol;
			}
			if (symbol->GetBlockLevel() >= GetBlockDepth())
			{
				CompileError({ "duplicated declaration '", name, "'" });
			}
		}

		symbol = CreateSymbol(name, Symbol::ClassTypeId::ConstantInteger);
		symbol->SetTypeDescription(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Bool));
		symbol->SetEtc(static_cast<int32_t>(value));

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateConstFloat(const std::string_view name, const float value)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol)
		{
			if (symbol->GetClassTypeId() == Symbol::ClassTypeId::ConstantFloat &&
				std::abs(symbol->GetFloat() - value) < std::numeric_limits<float>::epsilon())
			{
				return symbol;
			}
			if (symbol->GetBlockLevel() >= GetBlockDepth())
			{
				CompileError({ "duplicated declaration '", name, "'" });
			}
		}

		symbol = CreateSymbol(name, Symbol::ClassTypeId::ConstantFloat);
		symbol->SetTypeDescription(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Float));
		symbol->SetFloat(value);

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateConstString(const std::string_view name, const std::string_view value)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol)
		{
			if (symbol->GetClassTypeId() == Symbol::ClassTypeId::ConstantString &&
				symbol->GetString() == value)
			{
				return symbol;
			}
			if (symbol->GetBlockLevel() >= GetBlockDepth())
			{
				CompileError({ "duplicated declaration '", name, "'" });
			}
		}

		symbol = CreateSymbol(name, Symbol::ClassTypeId::ConstantString);
		symbol->SetTypeDescription(mTypeDescriptorFactory->GetString());
		symbol->SetString(value);
		symbol->SetAddress(static_cast<int32_t>(mDataBuffer->Set(value)));

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
			CompileError({ "duplicated declaration '", name, "'" });
		}

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateLabel(const std::string_view name)
	{
		std::shared_ptr<Symbol> symbol = Lookup(name);
		if(symbol == nullptr)
		{
			symbol = CreateSymbolWithLevel(name, Symbol::ClassTypeId::Label, 0/* TODO:mFunctionBlockLevel*/);
			symbol->SetAddress(-1);
			symbol->SetEtc(-1);
		}

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateFunction(const std::string_view name, const bool isActorOrStructOpened)
	{
		const Symbol::ClassTypeId classType = isActorOrStructOpened
			? Symbol::ClassTypeId::MemberFunction : Symbol::ClassTypeId::Function;

		std::shared_ptr<Symbol> symbol = Lookup(name);
		if (symbol == nullptr)
		{
			symbol = CreateSymbolWithAddress(name, classType, 0);
			symbol->SetClassTypeId(classType);
			symbol->SetOverride(mModuleBlockOpened /* TODO:不要？  isModuleBlockOpened*/);
		}
		else if (symbol->IsOverride() == false || symbol->GetClassTypeId() != classType)
		{
			CompileError({ "function '", symbol->GetName(), "' already declared" });
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
		const auto i = std::remove_if(mSymbolEntries.begin(), mSymbolEntries.end(), [name](const auto& symbol)
		{
			return name == symbol->GetName();
		});
		mSymbolEntries.erase(i, mSymbolEntries.end());
	}

	bool SymbolFactory::IsValid(const std::shared_ptr<Symbol>& symbol)
	{
		if (symbol &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::StaticVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::GlobalVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ActorVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::LocalVariable &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ConstantInteger &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ConstantFloat &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::ConstantString
			)
		{
			CompileError({ "non-variable name '", symbol->GetName(), "'" });
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

	void SymbolFactory::Define(const std::string_view name, const std::shared_ptr<Symbol>& symbolEntry)
	{
		mHashChainTable[name] = symbolEntry;
	}

	void SymbolFactory::Define(const std::shared_ptr<Symbol>& symbolEntry)
	{
		mHashChainTable[symbolEntry->GetName()] = symbolEntry;
	}

	void SymbolFactory::Undefine(const std::string_view name)
	{
		mHashChainTable.erase(name);
	}

	void SymbolFactory::Undefine(const std::shared_ptr<Symbol>& symbolEntry)
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

		const std::unique_ptr<BlockTable>& blockTable = mBlockTable.top();

		// check and update hash table
		const BlockEntry& blockEntry = blockTable->mHead;

		// check if undefined type
		if (
			blockEntry.mSymbolEntry &&
			blockEntry.mSymbolEntry->GetClassTypeId() == Symbol::ClassTypeId::Type &&
			blockEntry.mSymbolEntry->GetTypeDescriptor()->Is(TypeDescriptor::Id::Incomplete))
		{
			CompileError({ "incomplete type name '", blockEntry.mSymbolEntry->GetName(), "'" });
		}

		// deleting from hash chain
		for (std::shared_ptr<Symbol> symbol = blockEntry.mSymbolEntry; symbol; symbol = symbol->GetNext())
		{
			mHashChainTable.erase(symbol->GetName());
		}

		mLocalMemoryAddress = blockTable->mAllocp;

		mBlockTable.pop();

		return static_cast<int32_t>(GetBlockDepth());
	}

	size_t SymbolFactory::GetBlockDepth() const
	{
		return mBlockTable.size();
	}

	void SymbolFactory::EachBlock(const std::function<void(const std::shared_ptr<Symbol>&)>& function)
	{
		if (!mBlockTable.empty())
		{
			const BlockEntry& blockEntry = mBlockTable.top()->mHead;
			if (blockEntry.mSymbolEntry)
			{
				function(blockEntry.mSymbolEntry);
			}
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

			case Symbol::ClassTypeId::NewSymbol:
			case Symbol::ClassTypeId::NativeFunction:
			case Symbol::ClassTypeId::MemberFunction:
			case Symbol::ClassTypeId::StaticVariable:
			case Symbol::ClassTypeId::GlobalVariable:
			case Symbol::ClassTypeId::ActorVariable:
			case Symbol::ClassTypeId::LocalVariable:
			case Symbol::ClassTypeId::ConstantInteger:
			case Symbol::ClassTypeId::ConstantFloat:
			case Symbol::ClassTypeId::ConstantString:
			case Symbol::ClassTypeId::Label:
				break;
			}
		}
	}

	void SymbolFactory::WritePublicTypeDecl(std::ofstream& output) const
	{
		std::vector<std::string_view> currentNamespaces;
		std::vector<std::string_view> nextNamespaces;
		std::string_view baseName;

		auto setNamespace = [&output, &currentNamespaces, &nextNamespaces](const std::vector<std::string_view>& target)
		{
			size_t common = 0;
			while (common < currentNamespaces.size() && common < target.size())
			{
				if (currentNamespaces[common] != target[common])
					break;
				++common;
			}

			for (size_t i = currentNamespaces.size(); i > common; --i)
			{
				output << "}\n";
			}

			for (size_t i = common; i < target.size(); ++i)
			{
				output << "namespace " << target[i] << "\n{\n";
			}

			currentNamespaces.assign(target.begin(), target.end());
		};

		for (const std::shared_ptr<Symbol>& symbol : mSymbolEntries)
		{
			if (!symbol || symbol->GetBlockLevel() > 0)
				continue;

			const Symbol::ClassTypeId classType = symbol->GetClassTypeId();
			if (classType == Symbol::ClassTypeId::Type)
			{
				const std::shared_ptr<TypeDescriptor>& type = symbol->GetTypeDescriptor();
				if (!type || type->IsNot(TypeDescriptor::Id::Struct))
					continue;

				SplitQualifiedName(symbol->GetName(), nextNamespaces, baseName);
				setNamespace(nextNamespaces);

				output << "struct " << baseName << "\n{\n";
				for (std::shared_ptr<Symbol> member = type->GetSymbolEntry(); member; member = member->GetNext())
				{
					const Symbol::ClassTypeId memberClass = member->GetClassTypeId();
					if (memberClass != Symbol::ClassTypeId::ActorVariable &&
						memberClass != Symbol::ClassTypeId::GlobalVariable &&
						memberClass != Symbol::ClassTypeId::StaticVariable &&
						memberClass != Symbol::ClassTypeId::LocalVariable)
					{
						continue;
					}

					const RenderedType rendered = RenderType(member->GetTypeDescriptor());
					output << "\t" << rendered.mBase << " " << member->GetName();
					WriteArraySuffix(output, rendered.mArraySizes);
					output << ";\n";
				}
				output << "};\n";
			}
			else if (classType == Symbol::ClassTypeId::GlobalVariable || classType == Symbol::ClassTypeId::StaticVariable)
			{
				SplitQualifiedName(symbol->GetName(), nextNamespaces, baseName);
				setNamespace(nextNamespaces);

				const RenderedType rendered = RenderType(symbol->GetTypeDescriptor());
				output << rendered.mBase << " " << baseName;
				WriteArraySuffix(output, rendered.mArraySizes);
				output << ";\n";
			}
		}

		setNamespace({});
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
		mFrameSizeList = mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Allocate, InvalidAddress);

		if (!isAction)
		{
			// return addressをframe bufferに保存する命令を発行
			mCodeBuffer->AddOpecode(IntermediateLanguage::SaveReturnAddress);
		}

		// returnのジャンプ先リンクを初期化
		mReturnAddressList = InvalidAddress;
		mIsFunctionOpened = true;
	}

	/*
	関数の引数をフレームバッファ領域へコピーするコードを出力する
	@param[in]	function	関数を表すSymbol
	*/
	void SymbolFactory::OpenFunction2(const std::shared_ptr<const Symbol>& function) const
	{
		for (std::shared_ptr<const Symbol> symbol = function->GetParameterList(); symbol; symbol = symbol->GetNext())
		{
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::LoadFrameAddress, symbol->GetAddress());

			switch (symbol->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Char:
				mCodeBuffer->AddOpecode(IntermediateLanguage::StoreChar);
				break;

			case TypeDescriptor::Id::Short:
				mCodeBuffer->AddOpecode(IntermediateLanguage::StoreShort);
				break;

			case TypeDescriptor::Id::Bool:
				mCodeBuffer->AddOpecode(IntermediateLanguage::StoreChar);
				break;

			case TypeDescriptor::Id::Int:
				mCodeBuffer->AddOpecode(IntermediateLanguage::StoreInteger);
				break;

			case TypeDescriptor::Id::Float:
				mCodeBuffer->AddOpecode(IntermediateLanguage::StoreFloat);
				break;

			case TypeDescriptor::Id::Actor:
			case TypeDescriptor::Id::Reference:
				mCodeBuffer->AddOpecode(IntermediateLanguage::StoreReference);
				break;

			case TypeDescriptor::Id::Void:
			case TypeDescriptor::Id::Array:
			case TypeDescriptor::Id::Struct:
			case TypeDescriptor::Id::Module:
			case TypeDescriptor::Id::Nil:
			case TypeDescriptor::Id::Incomplete:
			default:
				if (symbol->GetTypeDescriptor()->GetMemorySize() <= 0)
					CompileError("missing size information on parameter");
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::StoreData, symbol->GetTypeDescriptor()->GetMemorySize());
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
				CompileError({ "label '", symbol->GetName(), "' used but not defined" });

			mCodeBuffer->ReplaceAddressAll(symbol->GetEtc(), symbol->GetAddress());
			});

		// returnのジャンプ先を更新
		mCodeBuffer->ReplaceAddressAll(mReturnAddressList, mCodeBuffer->GetSize());

		// 直後のジャンプは削除
		if (mReturnAddressList != InvalidAddress)
		{
			mCodeBuffer->Reduce(GetIntermediateLanguageProperty(IntermediateLanguage::Branch).mSize);
		}

		// アクションならばGetEtcは0以外
		if (node->GetSymbol()->GetEtc() == 0)
		{
			// return addressをレジスタに復帰する命令を発行
			mCodeBuffer->AddOpecode(IntermediateLanguage::LoadReturnAddress);
		}

		// free命令の発行
		mFrameSizeList = mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Free, mFrameSizeList);

		// return命令の発行
		if (isAction)
		{
			mCodeBuffer->AddOpecode(IntermediateLanguage::ReturnFromAction);
		}
		else
		{
			MANA_VERIFY_MESSAGE(node->GetSymbol()->GetTypeDescriptor(), "type description is null pointer");
			if (node->GetSymbol()->GetTypeDescriptor()->GetId() != TypeDescriptor::Id::Void)
			{
				if (!node->GetSymbol()->IsUsed())
					CompileError("meaningless return value specification");
			}
			mCodeBuffer->AddOpecode(IntermediateLanguage::ReturnFromFunction);
		}

		/*
		frame bufferのサイズを更新

		symbol_open_blockがsymbol_create_functionで呼ばれるので
		symbol_begin_function_registrationにsymbol_open_blockを呼んでいなくても
		ここでsymbol_close_blockを呼び出す必要がある。
		*/
		CloseBlock();
		mIsFunctionOpened = false;
		mCodeBuffer->ReplaceAddressAll(mFrameSizeList, AlignSize(mMaxLocalMemoryAddress, sizeof(uint32_t)));
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
		const size_t maxBlockDepth = IsActorOrStructerOpened() ? 2 : 1;
		if (GetBlockDepth() > maxBlockDepth)
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

	void SymbolFactory::BeginRegistrationStructure()
	{
		// 1) open block
		OpenBlock(false);
		++mActorOrStructureLevel;
		mActorMemoryAddress = 0;
	}

	void SymbolFactory::CommitRegistrationStructure(const std::string_view name)
	{
		MANA_ASSERT(GetBlockDepth() > 0);

		address_t maxAlignmentSize = 1;

		// 最も大きいサイズのタイプにアライメントを合わせる
		EachBlock([&maxAlignmentSize](const std::shared_ptr<Symbol>& symbol)
			{
				const std::shared_ptr<TypeDescriptor>& typeDescriptor = symbol->GetTypeDescriptor();
				maxAlignmentSize = std::max(maxAlignmentSize, typeDescriptor->GetAlignmentMemorySize());
			});
#if 0
		if (!mBlockTable.top()->mHead.empty())
		{
			const std::shared_ptr<Symbol>& symbol = mBlockTable.top()->mHead.;
			while (symbol)
			{
				const std::shared_ptr<TypeDescriptor>& typeDescriptor = symbol->GetTypeDescriptor();
				if (maxAlignmentSize < typeDescriptor->GetAlignmentMemorySize())
					maxAlignmentSize = typeDescriptor->GetAlignmentMemorySize();
				symbol = symbol->GetNext();
			}
		}
#endif

		// 1) create new type description
		std::shared_ptr<TypeDescriptor> newType = mTypeDescriptorFactory->Create(TypeDescriptor::Id::Struct);
		newType->SetSymbolEntry(GetLastSymbolEntryInBlock());
		newType->SetName(name);
		newType->SetAlignmentMemorySize(maxAlignmentSize);

		--mActorOrStructureLevel;
		CloseBlock();

		newType->SetMemorySize(AlignSize(mActorMemoryAddress, maxAlignmentSize));

		CreateType(name, newType);
	}

	// symbol_open_structure
	void SymbolFactory::OpenStructure(const std::string_view name)
	{
		OpenBlock(false);
		++mActorOrStructureLevel;

		if (const std::shared_ptr<Symbol>& symbol = Lookup(name))
		{
			std::shared_ptr<TypeDescriptor> type;

			for (type = symbol->GetTypeDescriptor(); type->GetId() == TypeDescriptor::Id::Array; type = type->GetComponent())
				;

			// typeがactorではない場合、続行不可能
			if (type->GetId() != TypeDescriptor::Id::Struct)
			{
				CompileError({ symbol->GetName(), " is Not struct!" });
			}
			else
			{
				// symbol_close_blockでsymbol_hash_chain_tableを開放する
				// TODO:mBlockTable.top().mHead = (const std::shared_ptr<Symbol>&)type->component;

				// シンボルリストの末端からhashに登録
				// TODO: symbol_open_actor_register_member((const std::shared_ptr<Symbol>&)type->component);
			}

			// instance変数サイズの再計算
			mActorMemoryAddress = symbol->GetTypeDescriptor() ? symbol->GetTypeDescriptor()->GetMemorySize() : 0;

			// 現在開いている型を記録
			mBlockTypeDescriptor.push(type);
		}
		else
		{
			// instance変数サイズの再計算
			mActorMemoryAddress = 0;
		}
	}

	// symbol_close_structure
	void SymbolFactory::CloseStructure()
	{
		--mActorOrStructureLevel;
		CloseBlock();

		// 現在開いている型をリセット
		mBlockTypeDescriptor.pop();
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
				CompileError({ symbol->GetName(), " is Not actor!" });
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
	void SymbolFactory::CommitRegistrationActor(const std::string_view name, const std::shared_ptr<TypeDescriptor>& td, const bool phantom)
	{
		std::shared_ptr<TypeDescriptor> type;
		//std::shared_ptr<TypeDescriptor> parent_type;

		if (const std::shared_ptr<Symbol>& symbol = Lookup(name))
		{
			for (type = symbol->GetTypeDescriptor(); type->Is(TypeDescriptor::Id::Array); type = type->GetComponent())
				;

			if (type->Is(TypeDescriptor::Id::Actor))
			{
				type->SetSymbolEntry(GetLastSymbolEntryInBlock());

				// actor and phantom check
				if (type->mShare.mActor.mPhantom != phantom)
					CompileError({ "already declared ", type->mShare.mActor.mPhantom ? "a phantom" : "an actor" });

				// @TODO: actorの宣言が二つある場合、ワーニングを出す？
				goto SKIP;
			}
		}

		mDataBuffer->Set(name);
		/*
		if (!parent.empty())
		{
			const std::shared_ptr<Symbol>& parent_symbol = Lookup(parent);
			parent_type = parent_symbol ? parent_symbol->GetTypeDescriptor() : nullptr;
		}
		else
		{
			parent_type = nullptr;
		}
		*/
		type = mTypeDescriptorFactory->Create(TypeDescriptor::Id::Actor);
		type->SetSymbolEntry(GetLastSymbolEntryInBlock());
		//TODO:  parent_type;
		type->SetName(name);
		type->SetAlignmentMemorySize(sizeof(uint32_t));
		type->mShare.mActor.mPhantom = phantom;

	SKIP:
		--mActorOrStructureLevel;
		CloseBlock();

		type->SetMemorySize(AlignSize(mActorMemoryAddress, sizeof(uint32_t)));

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

		if (const std::shared_ptr<Symbol>& symbol = Lookup(name))
		{
			std::shared_ptr<TypeDescriptor> type;

			for (type = symbol->GetTypeDescriptor(); type->GetId() == TypeDescriptor::Id::Array; type = type->GetComponent())
				;

			// typeがactorではない場合、続行不可能
			if (type->GetId() != TypeDescriptor::Id::Actor && type->GetId() != TypeDescriptor::Id::Module)
			{
				CompileError({ symbol->GetName(), " is Not actor!" });
			}
			else
			{
				// symbol_close_blockでsymbol_hash_chain_tableを開放する
				// TODO:mBlockTable.top().mHead = (const std::shared_ptr<Symbol>&)type->component;

				// シンボルリストの末端からhashに登録
				// TODO: symbol_open_actor_register_member((const std::shared_ptr<Symbol>&)type->component);
			}

			// instance変数サイズの再計算
			mActorMemoryAddress = symbol->GetTypeDescriptor() ? symbol->GetTypeDescriptor()->GetMemorySize() : 0;

			// 現在開いている型を記録
			mBlockTypeDescriptor.push(type);
		}
		else
		{
			// instance変数サイズの再計算
			mActorMemoryAddress = 0;
		}
	}

	void SymbolFactory::CloseActor()
	{
		--mActorOrStructureLevel;
		CloseBlock();

		// 現在開いている型をリセット
		mBlockTypeDescriptor.pop();

		/*
		type->memory_size = AlignSize(mActorMemoryAddress, IBSZ);

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
				CompileError({ parentSymbol->GetName(), " is Not modeule!" });
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

		//type->SetMemorySize(AlignSize(mActorMemoryAddress, IBSZ));
		type->SetMemorySize(AlignSize(mActorMemoryAddress, sizeof(void*)));

		CreateType(name, type);
	}

	void SymbolFactory::OpenModule(const std::shared_ptr<Symbol>& symbol)
	{
		MANA_UNUSED_VAR(symbol);

		if (symbol)
		{
			std::shared_ptr<TypeDescriptor> type;

			for (type = symbol->GetTypeDescriptor(); type->GetId() == TypeDescriptor::Id::Array; type = type->GetComponent())
				;

			// typeがactorではない場合、続行不可能
			if (type->GetId() != TypeDescriptor::Id::Actor && type->GetId() != TypeDescriptor::Id::Module)
			{
				CompileError({ symbol->GetName(), " is Not actor!" });
			}
			else
			{
				// symbol_close_blockでsymbol_hash_chain_tableを開放する
				// TODO:mBlockTable.top().mHead = (const std::shared_ptr<Symbol>&)type->component;

				// シンボルリストの末端からhashに登録
				// TODO: symbol_open_actor_register_member((const std::shared_ptr<Symbol>&)type->component);
			}

			// 現在開いている型を記録
			mBlockTypeDescriptor.push(type);
		}
	}

	void SymbolFactory::CloseModule(const std::string_view name)
	{
		MANA_UNUSED_VAR(name);

		// 現在開いている型をリセット
		mBlockTypeDescriptor.pop();
	}

	// symbol_extend_module
	void SymbolFactory::ExtendModule(const std::string_view name)
	{
		const std::shared_ptr<Symbol>& symbol = Lookup(name);
		//const std::shared_ptr<Symbol>& symbol = Lookup(name);
		if (symbol && symbol->GetTypeDescriptor() && symbol->GetTypeDescriptor()->Is(TypeDescriptor::Id::Module))
		{
			/* シンボルリストの末端からhashに登録 */
			if (const std::shared_ptr<Symbol>& actionSymbol = symbol->GetTypeDescriptor()->GetParent())
			{
				//ExtendModule(action_symbol);

				std::shared_ptr<Symbol> lastSymbol = actionSymbol;
				while (lastSymbol->GetNext())
				{
					lastSymbol = lastSymbol->GetNext();
				}
				lastSymbol->SetNext(mBlockTable.top()->mHead.mSymbolEntry);

				/* symbol_close_blockでsymbol_hash_chain_tableを開放する為 */
				mBlockTable.top()->mHead.mSymbolEntry = actionSymbol;

				symbol_open_actor_register_member(actionSymbol);
			}
		}
		else
		{
			CompileError("name followed by \"::\" must be a module");
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// request
	void SymbolFactory::AddRequest(const std::shared_ptr<CodeGenerator>& codeGenerator, const IntermediateLanguage opcode, const std::shared_ptr<SyntaxNode>& level, const std::shared_ptr<SyntaxNode>& actor, const std::string_view action) const
	{
		codeGenerator->Expression(level, false);

		if (actor && actor->GetTypeDescriptor())
		{
			switch (actor->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Actor:
				codeGenerator->Expression(actor, false);
				mCodeBuffer->AddOpecodeAndOperand(opcode, mDataBuffer->Set(action));
				return;

			case TypeDescriptor::Id::Reference:
				if (actor->GetTypeDescriptor()->GetName() == "string")
				{
					IntermediateLanguage code;
					switch (opcode)
					{
					case IntermediateLanguage::Request:
						code = IntermediateLanguage::DynamicRequest;
						break;
					case IntermediateLanguage::RequestWaitStarting:
						code = IntermediateLanguage::DynamicRequestWaitStarting;
						break;
					case IntermediateLanguage::RequestWaitEnded:
						code = IntermediateLanguage::DynamicRequestWaitEnded;
						break;
					default:
						CompileError("incompatible type of operand");
						return;
					}
					codeGenerator->Expression(actor, false);
					mCodeBuffer->AddOpecodeAndOperand(code, mDataBuffer->Set(action));
					return;
				}
				break;

			case TypeDescriptor::Id::Void:
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Bool:
			case TypeDescriptor::Id::Int:
			case TypeDescriptor::Id::Float:
			case TypeDescriptor::Id::Array:
			case TypeDescriptor::Id::Struct:
			case TypeDescriptor::Id::Module:
			case TypeDescriptor::Id::Nil:
			case TypeDescriptor::Id::Incomplete:
			default:
				break;
			}
		}

		CompileError("incompatible type of operand");
	}

	void SymbolFactory::AddJoin(const std::shared_ptr<CodeGenerator>& codeGenerator, const std::shared_ptr<SyntaxNode>& level, const std::shared_ptr<SyntaxNode>& actor) const
	{
		if (actor && actor->GetTypeDescriptor())
		{
			switch (actor->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Reference:
				if (actor->GetTypeDescriptor()->GetName() == "string")
					break;
				
				[[fallthrough]]
				;

			case TypeDescriptor::Id::Actor:
				codeGenerator->Expression(actor, false);
				codeGenerator->Expression(level, false);
				mCodeBuffer->AddOpecode(IntermediateLanguage::Join);
				return;

			case TypeDescriptor::Id::Void:
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Bool:
			case TypeDescriptor::Id::Int:
			case TypeDescriptor::Id::Float:
			case TypeDescriptor::Id::Array:
			case TypeDescriptor::Id::Struct:
			case TypeDescriptor::Id::Module:
			case TypeDescriptor::Id::Nil:
			case TypeDescriptor::Id::Incomplete:
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
			CompileError("incomplete type name or void is used for declaration");
			type = mTypeDescriptorFactory->Get(TypeDescriptor::Id::Int);
		}

		if (symbol->GetTypeDescriptor() == nullptr)		// 配列型以外の変数 ?
		{
			symbol->SetTypeDescription(type);			// 型の設定
		}
		else if (symbol->GetTypeDescriptor() == type)	// エラーの可能性がある
		{
		}
		else
		{												// 配列型の変数の処理
			symbol->GetTypeDescriptor()->SetArray(type);// 配列型リストの設定
			type = symbol->GetTypeDescriptor();
		}

		switch (symbol->GetClassTypeId())
		{
		case Symbol::ClassTypeId::StaticVariable:
			symbol->SetAddress(AlignSize(mStaticMemoryAddress, type->GetAlignmentMemorySize()));
			mStaticMemoryAddress = symbol->GetAddress() + type->GetMemorySize();
			break;

		case Symbol::ClassTypeId::GlobalVariable:
			symbol->SetAddress(AlignSize(mGlobalMemoryAddress, type->GetAlignmentMemorySize()));
			mGlobalMemoryAddress = symbol->GetAddress() + type->GetMemorySize();
			break;

		case Symbol::ClassTypeId::ActorVariable:
			symbol->SetAddress(AlignSize(mActorMemoryAddress, type->GetAlignmentMemorySize()));
			mActorMemoryAddress = symbol->GetAddress() + type->GetMemorySize();
			break;

		case Symbol::ClassTypeId::LocalVariable:
			// ローカル変数は後ろから確保される為、他の変数と計算が反対になるので注意
			symbol->SetAddress(AlignSize(mLocalMemoryAddress, type->GetAlignmentMemorySize()) + type->GetMemorySize());
			mLocalMemoryAddress = symbol->GetAddress();
			break;

		case Symbol::ClassTypeId::NewSymbol:
		case Symbol::ClassTypeId::Type:
		case Symbol::ClassTypeId::Function:
		case Symbol::ClassTypeId::NativeFunction:
		case Symbol::ClassTypeId::MemberFunction:
		case Symbol::ClassTypeId::ConstantInteger:
		case Symbol::ClassTypeId::ConstantFloat:
		case Symbol::ClassTypeId::ConstantString:
		case Symbol::ClassTypeId::Label:
		default:
			MANA_BUG("invalid class type detect");
		}

		// 仮引数の表示の格納
		symbol->SetMemoryTypeId(parameter);
	}

	int32_t SymbolFactory::GetStaticMemoryAddress() const
	{
		return mStaticMemoryAddress;
	}

	void SymbolFactory::SetStaticMemoryAddress(const int32_t size)
	{
		if (mStaticMemoryAddress < size)
			mStaticMemoryAddress = size;
	}

	int32_t SymbolFactory::GetGlobalMemoryAddress() const
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

				case TypeDescriptor::Id::Void:
				case TypeDescriptor::Id::Char:
				case TypeDescriptor::Id::Short:
				case TypeDescriptor::Id::Bool:
				case TypeDescriptor::Id::Int:
				case TypeDescriptor::Id::Float:
				case TypeDescriptor::Id::Reference:
				case TypeDescriptor::Id::Struct:
				case TypeDescriptor::Id::Module:
				case TypeDescriptor::Id::Nil:
				case TypeDescriptor::Id::Incomplete:
				default:
					break;
				}
			}

			return true;
			});

		return count;
	}

	void SymbolFactory::CheckUndefine() const
	{
		Each([](const std::shared_ptr<Symbol>& symbol)
			{
				if (symbol->GetBlockLevel() > 0)
					return true;

				symbol->CheckUndefineRecursive();

				return true;
			}
		);
	}

	bool SymbolFactory::GenerateActorInformation(OutputStream& stream) const
	{
		return Each([this, &stream](const std::shared_ptr<const Symbol>& symbol)
			{
				if (symbol->GetBlockLevel() > 0)
					return true;

				MANA_VERIFY_MESSAGE(symbol->GetTypeDescriptor(), "Null pointer error in Write,Information");
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

						case TypeDescriptor::Id::Void:
						case TypeDescriptor::Id::Char:
						case TypeDescriptor::Id::Short:
						case TypeDescriptor::Id::Bool:
						case TypeDescriptor::Id::Int:
						case TypeDescriptor::Id::Float:
						case TypeDescriptor::Id::Reference:
						case TypeDescriptor::Id::Struct:
						case TypeDescriptor::Id::Module:
						case TypeDescriptor::Id::Nil:
						case TypeDescriptor::Id::Incomplete:
						default:
							break;
						}
					}
				}
ESCAPE:
				case TypeDescriptor::Id::Void:
				case TypeDescriptor::Id::Char:
				case TypeDescriptor::Id::Short:
				case TypeDescriptor::Id::Bool:
				case TypeDescriptor::Id::Int:
				case TypeDescriptor::Id::Float:
				case TypeDescriptor::Id::Reference:
				case TypeDescriptor::Id::Struct:
				case TypeDescriptor::Id::Module:
				case TypeDescriptor::Id::Nil:
				case TypeDescriptor::Id::Incomplete:
				default:
					break;
				}
				return true;
			}
		);
	}

	bool SymbolFactory::GenerateActorEntity(OutputStream& stream, const std::shared_ptr<const Symbol>& symbol, const std::shared_ptr<const TypeDescriptor>& type) const
	{
		ActorInfoHeader actorInfoHeader;

		const size_t number_of_actions = type->GetActionCount();
		if (number_of_actions > (1 << (8 * sizeof(actorInfoHeader.mNumberOfActions))))
		{
			LinkerError({ "Too much actions in ", symbol->GetName(), "\n" });
			return false;
		}

		memset(&actorInfoHeader, 0, sizeof(actorInfoHeader));
		actorInfoHeader.mName = mDataBuffer->Get(symbol->GetName());
		actorInfoHeader.mNumberOfActions = static_cast<uint16_t>(number_of_actions);
		actorInfoHeader.mNumberOfInstances = static_cast<uint8_t>(type->GetArraySize());
		actorInfoHeader.mVariableSize = type->GetMemorySize();

		if (actorInfoHeader.mName == static_cast<uint32_t>(-1))
		{
			LinkerError({ "Can't find actor '", symbol->GetName(), "'\n" });
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
				ActionInfoHeader actionInfoHeader = {};
				actionInfoHeader.mName = mDataBuffer->Get(component_symbol->GetName());
				actionInfoHeader.mAddress = component_symbol->GetAddress();

				if (actionInfoHeader.mName == static_cast<uint32_t>(-1))
				{
					LinkerError({ "Can't find action '", component_symbol->GetName(), "'\n" });
					return false;
				}

				stream.PushData(&actionInfoHeader, sizeof(actionInfoHeader));
			}
		}

		return true;
	}
}
