/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "SymbolEntry.h"
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

	SymbolEntry::SymbolEntry(const char* name, const ClassTypeId classType, const int32_t level)
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

	bool SymbolEntry::IsValidVariable() const
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
			CompileError("non-variable name '%s'", mName);
			return false;
		}

		return true;
	}

	void SymbolEntry::symbol_check_undefine_recursive() const
	{
		switch (mClassTypeId)
		{
		case SymbolEntry::ClassTypeId::TYPEDEF:
			if (mTypeDescription->GetId() == TypeDescriptor::Id::ACTOR)
			{
				auto symbol = mTypeDescription->GetSymbolEntry();
				if (symbol)
					symbol->symbol_check_undefine_recursive();
			}
			break;

		default:
			break;
		}

		if (mNext)
			mNext->symbol_check_undefine_recursive();
	}

	void SymbolEntry::OnDump(std::ofstream& output, const int32_t level) const
	{
		for (int32_t i = 0; i < level; i++)
			output << ' ';

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		output << mMagic;
		output << " ";
#endif
		output << SymbolEntry::GetClassTypeName(mClassTypeId); // TODO
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
			case TypeDescriptor::Id::ACTOR:
			case TypeDescriptor::Id::MODULE:
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
