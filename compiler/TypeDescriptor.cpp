/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "TypeDescriptor.h"
#include "ErrorHandler.h"
#include "Symbol.h"

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
#if 1
		if (mComponent == nullptr)
		{
			if (this != ElmnT.get())
				mComponent = ElmnT;

			mAlignmentMemorySize = ElmnT->mAlignmentMemorySize;
			mMemorySize = mArraySize * ElmnT->mMemorySize;
		}
		else
		{
			MANA_ASSERT(this != mComponent.get());
			mComponent->SetArray(ElmnT);
			//ElmnT = mComponent;
			mAlignmentMemorySize = mComponent->mAlignmentMemorySize;
			mMemorySize = mArraySize * mComponent->mMemorySize;
		}
#else
		MANA_BUG("");
		mAlignmentMemorySize = ElmnT->mAlignmentMemorySize;
		mMemorySize = mArraySize * ElmnT->mMemorySize;
#endif
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
		switch (mTcons)
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

	bool TypeDescriptor::Is(const Id id) const
	{
		return mTcons == id;
	}

	bool TypeDescriptor::IsNot(const Id id) const
	{
		return mTcons != id;
	}

	TypeDescriptor::Id TypeDescriptor::GetId() const
	{
		return mTcons;
	}

	const std::string_view TypeDescriptor::GetName() const
	{
		return mName;
	}

	std::shared_ptr<Symbol> TypeDescriptor::GetSymbolEntry() const
	{
		return mSymbolEntry;
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptor::GetComponent() const
	{
		return mComponent;
	}

	size_t TypeDescriptor::GetArraySize() const
	{
		return mArraySize;
	}

	size_t TypeDescriptor::GetMemorySize() const
	{
		return mMemorySize;
	}

	size_t TypeDescriptor::GetAlignmentMemorySize() const
	{
		return mAlignmentMemorySize;
	}

	void TypeDescriptor::SetTypeDescriptor(const std::shared_ptr<TypeDescriptor>& component)
	{
		mComponent = component;
	}

	size_t TypeDescriptor::GetActionCount() const
	{
		size_t count = 0;
		for (std::shared_ptr<Symbol> symbol = mSymbolEntry; symbol; symbol = symbol->GetNext())
		{
			if (symbol->GetClassTypeId() == Symbol::ClassTypeId::MEMBER_FUNCTION)
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

	void TypeDescriptor::SetSymbolEntry(const std::shared_ptr<Symbol>& symbolEntry)
	{
		mSymbolEntry = symbolEntry;
	}

	void TypeDescriptor::SetName(const std::string_view name)
	{
		mName = name;
	}

	void TypeDescriptor::SetMemorySize(const size_t memorySize)
	{
		mMemorySize = memorySize;
	}

	void TypeDescriptor::SetAlignmentMemorySize(const size_t alignmentMemorySize)
	{
		mAlignmentMemorySize = alignmentMemorySize;
	}
}
