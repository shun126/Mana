/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "TypeDescriptor.h"
#include "ErrorHandler.h"
#include "SymbolEntry.h"

namespace mana
{
	TypeDescriptor::TypeDescriptor(const Id tcons)
		: mTcons(tcons)
	{
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		static uint32_t count = 0;
		snprintf(magic, sizeof(magic), "T%d", count);
		++count;
#endif
	}

	void TypeDescriptor::SetArray(const std::shared_ptr<TypeDescriptor>& ElmnT)
	{
#if 0
		if(mComponent.get() == nullptr)
		{
			if(this != ElmnT.get())
				mComponent = ElmnT;
		}
		else
		{
			MANA_ASSERT(this != mComponent);
			mComponent->SetArray(ElmnT);
			ElmnT = mComponent;
		}
#else
		MANA_BUG("");
#endif
		mAlignmentMemorySize = ElmnT->mAlignmentMemorySize;
		mMemorySize = mArraySize * ElmnT->mMemorySize;
	}

	bool TypeDescriptor::Compare(const std::shared_ptr<TypeDescriptor>& typeDescriptor) const
	{
		if(mTcons != typeDescriptor->mTcons)
			return false;

		switch(mTcons)
		{
		case Id::Reference:
			return mComponent->Compare(typeDescriptor->mComponent);

		case Id::Array:
			{
				const size_t n1 = mArraySize;
				const size_t n2 = typeDescriptor->mArraySize;
				if (n1 != n2 )
					CompileError("array size unmatched");
				return mComponent->Compare(typeDescriptor->mComponent);
			}

		case Id::ACTOR:
			return true;

		case Id::Struct:
		case Id::INCOMPLETE:
		default:
			return (this == typeDescriptor.get());
		}
	}

	bool TypeDescriptor::Compatible(const std::shared_ptr<TypeDescriptor>& typeDescriptor) const
	{
		switch(mTcons)
		{
	#if 0
		case Id::Char:
			if(typeDescriptor->mTcons == Id::Char)
				return true;
			break;

		case Id::Short:
			switch(typeDescriptor->mTcons)
			{
			case Id::Char:
			case Id::Short:
				return true;
			default:
				break;
			}
			break;
	#else
		case Id::Char:
		case Id::Short:
	#endif
		case Id::Int:
			switch(typeDescriptor->mTcons)
			{
			case Id::Char:
			case Id::Short:
			case Id::Int:
				return true;
			default:
				break;
			}
			break;

		case Id::Float:
			if(typeDescriptor->mTcons == Id::Float)
				return true;
			break;

		case Id::Struct:
		case Id::Array:
			if(Compare(typeDescriptor))
				return true;
			break;

		case Id::ACTOR:
			if(Compare(typeDescriptor))
				return true;

			if(typeDescriptor->mTcons == Id::NIL)
				return true;
			break;

		case Id::Reference:
			if(this == typeDescriptor.get())
				return true;
			break;

		default:
			break;
		}

		CompileError("incompatible type of operand");

		return false;
	}

	size_t TypeDescriptor::GetActionCount() const
	{
		size_t count = 0;
		for (std::shared_ptr<SymbolEntry> symbol = mSymbolEntry; symbol; symbol = symbol->GetNext())
		{
			if (symbol->GetClassTypeId() == SymbolEntry::ClassTypeId::MEMBER_FUNCTION)
				++count;
		}
		return count;
	}

	bool TypeDescriptor::IsPhantom() const
	{
		return share.actor.mPhantom;
	}

	void TypeDescriptor::Dump(std::ofstream& output) const
	{
		output << GetDataTypeName(mTcons);
		switch (mTcons)
		{
		case Id::Reference:
			output << '(';
			mComponent->Dump(output);
			output << ')';
			break;

		case Id::Array:
			output << '[';
			mComponent->Dump(output);
			if (mArraySize >= 0)
			{
				output << ", ";
				output << mArraySize;
			}
			else
			{
				output << ", *";
			}
			output << ']';
			break;

		case Id::Struct:
		case Id::ACTOR:
		case Id::MODULE:
			output << "{";
			output << mMemorySize;
			output << " bytes(";
			output << mAlignmentMemorySize;
			output << ")}";
			break;

		default:
			break;
		}
	}
}
