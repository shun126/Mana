/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "Symbol.h"
#include "ErrorHandler.h"
#include <iomanip>

namespace mana
{
#if 0

static char* symbol_data_type_id_name[NUMBER_OF] = {
	"void",												/* void型 */
	"char",												/* int8_t型 */
	"short",											/* int16_t型 */
	"int",												/* int32_t型 */
	"float",											/* float型 */
	"reference",										/* reference型 */
	"array",											/* array型 */
	"struct",											/* struct型 */
	"actor",											/* actor型 */
	"module",											/* module型 */
	"nil",												/* nil型 */
	"incomplete",										/* 宣言が未完了の型 */
};
#endif

	Symbol::Symbol(const std::string_view name, const ClassTypeId classType, const int32_t level)
		: mClassTypeId(classType)
		, mName(name)
		, mBlockLevel(level)
	{
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		static uint32_t count = 0;
		snprintf(mMagic, sizeof(mMagic), "S%d", count);
		++count;
#endif
	}

	bool Symbol::IsValid() const
	{
		if (
			GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_STATIC &&
			GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_GLOBAL &&
			GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_ACTOR &&
			GetClassTypeId() != Symbol::ClassTypeId::VARIABLE_LOCAL &&
			GetClassTypeId() != Symbol::ClassTypeId::CONSTANT_INT &&
			GetClassTypeId() != Symbol::ClassTypeId::CONSTANT_FLOAT &&
			GetClassTypeId() != Symbol::ClassTypeId::CONSTANT_STRING)
		{
			CompileError("non-variable name '%s'", GetName());
			return false;
		}
		else {
			return true;
		}
	}

	bool Symbol::IsValidVariable() const
	{
		if(
			mClassTypeId != ClassTypeId::VARIABLE_STATIC &&
			mClassTypeId != ClassTypeId::VARIABLE_GLOBAL &&
			mClassTypeId != ClassTypeId::VARIABLE_ACTOR &&
			mClassTypeId != ClassTypeId::VARIABLE_LOCAL &&
			mClassTypeId != ClassTypeId::CONSTANT_INT &&
			mClassTypeId != ClassTypeId::CONSTANT_FLOAT &&
			mClassTypeId != ClassTypeId::CONSTANT_STRING
		){
			CompileError("non-variable name '%s'", mName.c_str());
			return false;
		}

		return true;
	}

	std::shared_ptr<Symbol> Symbol::GetParameterList()
	{
		return mParameterList;
	}
	
	const std::shared_ptr<Symbol> Symbol::GetParameterList() const
	{
		return mParameterList;
	}

	void Symbol::SetParameterList(const std::shared_ptr<Symbol>& symbol)
	{
		mParameterList = symbol;
	}

	std::shared_ptr<Symbol> Symbol::GetNext()
	{
		return mNext;
	}
	
	void Symbol::SetNext(const std::shared_ptr<Symbol>& symbol)
	{
		mNext = symbol;
	}

	const std::shared_ptr<Symbol> Symbol::GetNext() const
	{
		return mNext;
	}

	std::shared_ptr<TypeDescriptor> Symbol::GetTypeDescriptor()
	{
		return mTypeDescription;
	}

	const std::shared_ptr<TypeDescriptor> Symbol::GetTypeDescriptor() const
	{
		return mTypeDescription;
	}

	Symbol::ClassTypeId Symbol::GetClassTypeId() const
	{
		return mClassTypeId;
	}
	
	void Symbol::SetClassTypeId(const Symbol::ClassTypeId classTypeId)
	{
		mClassTypeId = classTypeId;
	}

	Symbol::MemoryTypeId Symbol::GetMemoryTypeId() const
	{
		return mMemoryTypeId;
	}

	void Symbol::SetMemoryTypeId(const MemoryTypeId MemoryTypeId)
	{
		mMemoryTypeId = MemoryTypeId;
	}

	const std::string_view Symbol::GetName() const
	{
		return mName;
	}

	void Symbol::SetName(const std::string_view name)
	{
		mName = name;
	}

	int32_t Symbol::GetAddress() const
	{
		return mAddress;
	}

	void Symbol::SetAddress(const int32_t address)
	{
		mAddress = address;
	}

	int32_t Symbol::GetEtc() const
	{
		return mEtc;
	}
	
	void Symbol::SetEtc(const int32_t etc)
	{
		mEtc = etc;
	}

	float Symbol::GetFloat() const
	{
		return mFloat;
	}

	void Symbol::SetFloat(const float value)
	{
		mFloat = value;
	}

	const std::string_view Symbol::GetString() const
	{
		return mString;
	}

	void Symbol::SetString(const std::string_view value)
	{
		mString = value;
	}

	int32_t Symbol::GetBlockLevel() const
	{
		return mBlockLevel;
	}

	void Symbol::SetBlockLevel(const int32_t blockLevel)
	{
		mBlockLevel = blockLevel;
	}

	int32_t Symbol::GetNumberOfParameters() const
	{
		return mNumberOfParameters;
	}

	bool Symbol::IsOverride() const
	{
		return mOverride;
	}
	
	void Symbol::SetOverride(const bool override_)
	{
		mOverride = override_;
	}

	bool Symbol::IsUsed() const
	{
		return mUsed;
	}

	void Symbol::SetUsed(const bool used)
	{
		mUsed = used;
	}

	void Symbol::SetTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescription)
	{
		mTypeDescription = typeDescription;
	}

	void Symbol::SetNumberOfParameters(const int32_t numberOfParameters)
	{
		mNumberOfParameters = numberOfParameters;
	}

	void Symbol::symbol_check_undefine_recursive() const
	{
		switch (mClassTypeId)
		{
		case Symbol::ClassTypeId::TYPEDEF:
			if (mTypeDescription->Is(TypeDescriptor::Id::Actor))
			{
				if (auto symbol = mTypeDescription->GetSymbolEntry())
					symbol->symbol_check_undefine_recursive();
			}
			break;

		default:
			break;
		}

		if (mNext)
			mNext->symbol_check_undefine_recursive();
	}

	void Symbol::OnDump(std::ofstream& output, const int32_t level) const
	{
		for (int32_t i = 0; i < level; i++)
			output << ' ';

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		output << mMagic;
		output << " ";
#endif
		output << Symbol::GetClassTypeName(mClassTypeId); // TODO
		output << " ";

		if (mTypeDescription)
		{
			mTypeDescription->Dump(output);
			output << " ";
		}

		output << mName;
		output << " (";
		output << "0x" << std::setfill('0') << std::hex << std::setw(8) << mAddress;
		output << ")";

		output << "\n";

		if (GetTypeDescriptor())
		{
			switch (GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Actor:
			case TypeDescriptor::Id::Module:
			case TypeDescriptor::Id::Struct:
				if (GetTypeDescriptor()->GetSymbolEntry())
				{
					GetTypeDescriptor()->GetSymbolEntry()->OnDump(output, level + 1);
				}
				break;

			default:
				break;
			}
		}

		if (GetParameterList())
			GetParameterList()->OnDump(output, level + 1);

		if (GetNext())
			GetNext()->OnDump(output, level);
	}

}
