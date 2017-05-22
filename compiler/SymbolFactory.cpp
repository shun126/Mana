/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "SymbolFactory.h"
#include "ErrorHandler.h"
#include "Path.h"
#include "SymbolTable.h"

namespace mana
{
	SymbolFactory::SymbolFactory(const std::shared_ptr<StringPool>& stringPool, const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
		: mStringPool(stringPool)
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
		return CreateSymbolWithLevel(name, class_type, mBlockTable.size());
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbolWithAddress(const std::string_view name, const Symbol::ClassTypeId class_type, const int32_t address)
	{
		std::shared_ptr<Symbol> symbol = CreateSymbol(name, class_type);
		symbol->SetAddress(address);
		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateSymbolWithLevel(const std::string_view name, Symbol::ClassTypeId class_type, const int32_t level)
	{
		std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>(name, class_type, level);
		mSymbolEntries.push_back(symbol);
		
		Define(name, symbol);

		if (!mBlockTable.empty())
		{
			if (!mBlockTable.top()->mHead.empty())
				symbol->SetNext(mBlockTable.top()->mHead.back().mSymbolEntry);
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

		symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::ALIAS, -1);
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

		symbol = CreateSymbol(name, Symbol::ClassTypeId::CONSTANT_INT);
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

		symbol = CreateSymbol(name, Symbol::ClassTypeId::CONSTANT_FLOAT);
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

		symbol = CreateSymbol(name, Symbol::ClassTypeId::CONSTANT_STRING);
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
		if(symbol == nullptr || symbol->GetBlockLevel() < mBlockTable.size())
		{
			if (isStaticVariable)
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::VARIABLE_STATIC, 0);
			}
			else if(isBlockOpened == false)
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::VARIABLE_GLOBAL, 0);
			}
			else if(isFunctionOpened == false)
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::VARIABLE_ACTOR, 0);
			}
			else
			{
				symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::VARIABLE_LOCAL, 0);
			}
			symbol->SetTypeDescription(type);
		}
		else
		{
			CompileError("duplicated declaration '%s'", name);
		}

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateLabel(const std::string_view name)
	{
		std::shared_ptr<Symbol> symbol;

		symbol = Lookup(name);
		if(symbol == nullptr)
		{
			symbol = CreateSymbolWithLevel(name, Symbol::ClassTypeId::LABEL, 0/* TODO:mFunctionBlockLevel*/);
			symbol->SetAddress(-1);
			symbol->SetEtc(-1);
		}

		return symbol;
	}

	std::shared_ptr<Symbol> SymbolFactory::CreateFunction(const std::string_view name, const bool isActorOrStructerOpened, const bool isModuleBlockOpened)
	{
		const Symbol::ClassTypeId class_type = isActorOrStructerOpened
			? Symbol::ClassTypeId::MEMBER_FUNCTION : Symbol::ClassTypeId::FUNCTION;

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
			symbol = CreateSymbolWithAddress(name, Symbol::ClassTypeId::TYPEDEF, 0);
			symbol->SetTypeDescription(type);
		}
		else if (symbol->GetTypeDescriptor() == type)
		{
			return symbol;
		}
		else if (
			symbol->GetClassTypeId() == Symbol::ClassTypeId::TYPEDEF &&
			symbol->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::INCOMPLETE )
		{
			if (
				type->GetId() == TypeDescriptor::Id::INCOMPLETE ||
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
			symbol->GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_STATIC &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_GLOBAL &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_ACTOR &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_LOCAL &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::CONSTANT_INT &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::CONSTANT_FLOAT &&
			symbol->GetClassTypeId() != Symbol::ClassTypeId::CONSTANT_STRING
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

			symbol = CreateSymbol(name, Symbol::ClassTypeId::VARIABLE_LOCAL, 0);
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
		// check and update hash table
		for (const BlockEntry& blockEntry : mBlockTable.top()->mHead)
		{
			// deleting from hash chain
			mHashChainTable.erase(blockEntry.mSymbolEntry->GetName());

			// check if undefined type
			if (blockEntry.mSymbolEntry->GetClassTypeId() == Symbol::ClassTypeId::TYPEDEF /*&& (symbol->GetTypeDescriptor())->GetId() == TypeDescriptor::Id::INCOMPLETE*/)
				CompileError("incomplete type name '%s'", blockEntry.mSymbolEntry->GetName().data());
		}

		const int32_t address = mBlockTable.top()->mAllocp;
		mBlockTable.pop();
		return address;
	}

	size_t SymbolFactory::GetBlockDepth() const
	{
		return mBlockTable.size();
	}

	void SymbolFactory::EachBlock(std::function<void(const std::shared_ptr<Symbol>&)> function)
	{
		if (!mBlockTable.empty())
		{
			for (const BlockEntry& blockEntry : mBlockTable.top()->mHead)
			{
				function(blockEntry.mSymbolEntry);
			}
		}
	}




	void SymbolFactory::ExtendModule(const std::shared_ptr< Symbol>& symbol)
	{
		if (symbol)
		{
			std::shared_ptr<Symbol> lastSymbol = symbol;
			while (lastSymbol->GetNext())
			{
				lastSymbol = lastSymbol->GetNext();
			}

			// Register in block table to release symbol_hash_chain_table in CloseBlock
			lastSymbol->SetNext(mBlockTable.top()->mHead.back().mSymbolEntry);
			RegisterToBlock(symbol);
		}
	}

	void SymbolFactory::RegisterToBlock(const std::shared_ptr<Symbol>& symbolEntry)
	{
		mBlockTable.top()->mHead.emplace_back(symbolEntry);
	}

	void SymbolFactory::RegisterToBlock(const std::shared_ptr<TypeDescriptor>& typeDescriptor)
	{
		mBlockTable.top()->mHead.emplace_back(typeDescriptor);
	}


	std::shared_ptr<Symbol> SymbolFactory::GetLastSymbolEntryInBlock() const
	{
		if (mBlockTable.empty())
			return nullptr;
		if (mBlockTable.top()->mHead.empty())
			return nullptr;
		return mBlockTable.top()->mHead.back().mSymbolEntry;
	}

	std::shared_ptr<TypeDescriptor> SymbolFactory::GetLastTypeDescriptorInBlock() const
	{
		if (mBlockTable.empty())
			return nullptr;
		if (mBlockTable.top()->mHead.empty())
			return nullptr;
		return mBlockTable.top()->mHead.back().mTypeDescriptor;
	}




	void SymbolFactory::Dump(std::ofstream& output) const
	{
		for(const std::shared_ptr<Symbol>& symbol : mSymbolEntries)
		{
			if(symbol->GetBlockLevel() <= 0)
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
			case Symbol::ClassTypeId::FUNCTION:
				if (symbol->GetAddress() == address)
				{
					output << symbol->GetName();
					output << "\n";
					return;
				}
				break;

			case Symbol::ClassTypeId::TYPEDEF:
				if (symbol->GetTypeDescriptor() && symbol->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::ACTOR)
				{
					for (std::shared_ptr<Symbol> component_symbol = symbol->GetTypeDescriptor()->GetSymbolEntry(); component_symbol; component_symbol = component_symbol->GetNext())
					{
						if (component_symbol->GetClassTypeId() != Symbol::ClassTypeId::MEMBER_FUNCTION)
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
}
