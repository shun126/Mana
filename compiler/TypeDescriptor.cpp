/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "TypeDescriptor.h"
#include "ErrorHandler.h"
#include "Symbol.h"
#include <iomanip>

namespace mana
{
	TypeDescriptor::TypeDescriptor(const Id tcons)
		: mTcons(tcons)
	{
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		static uint32_t count = 0;
		snprintf(mMagic, sizeof(mMagic), "T%d", count);
		++count;
#endif
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
				const address_t n1 = mArraySize;
				const address_t n2 = typeDescriptor->mArraySize;
				if (n1 != n2 )
					CompileError("array size unmatched");
				return mComponent->Compare(typeDescriptor->mComponent);
			}

		case Id::Actor:
			return true;

		case Id::Struct:
		case Id::Incomplete:
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

		case Id::Actor:
			if(Compare(typeDescriptor))
				return true;

			if(typeDescriptor->mTcons == Id::Nil)
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

	bool TypeDescriptor::Compare(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right)
	{
		MANA_ASSERT(left);
		MANA_ASSERT(right);
		return left->Compare(right);
	}

	bool TypeDescriptor::Compatible(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right)
	{
		MANA_ASSERT(left);
		MANA_ASSERT(right);
		return left->Compatible(right);
	}

	void TypeDescriptor::SetArray(const std::shared_ptr<TypeDescriptor>& arrayElementTypeDescriptor)
	{
#if 1
		if (mComponent == nullptr)
		{
			if (this != arrayElementTypeDescriptor.get())
				mComponent = arrayElementTypeDescriptor;

			mAlignmentMemorySize = arrayElementTypeDescriptor->mAlignmentMemorySize;
			mMemorySize = mArraySize * arrayElementTypeDescriptor->mMemorySize;
		}
		else
		{
			MANA_ASSERT(this != mComponent.get());
			mComponent->SetArray(arrayElementTypeDescriptor);
			//arrayElementTypeDescriptor = mComponent;
			mAlignmentMemorySize = mComponent->mAlignmentMemorySize;
			mMemorySize = mArraySize * mComponent->mMemorySize;
		}
#else
		MANA_BUG("");
		mAlignmentMemorySize = arrayElementTypeDescriptor->mAlignmentMemorySize;
		mMemorySize = mArraySize * arrayElementTypeDescriptor->mMemorySize;
#endif
	}

	std::string_view TypeDescriptor::GetName() const
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

	address_t TypeDescriptor::GetArraySize() const
	{
		return mArraySize;
	}

	address_t TypeDescriptor::GetMemorySize() const
	{
		return mMemorySize;
	}

	address_t TypeDescriptor::GetAlignmentMemorySize() const
	{
		return mAlignmentMemorySize;
	}

	void TypeDescriptor::SetTypeDescriptor(const std::shared_ptr<TypeDescriptor>& component)
	{
		mComponent = component;
	}

	address_t TypeDescriptor::GetActionCount() const
	{
		MANA_ASSERT(mSymbolEntry);

		address_t count = 0;
		for (std::shared_ptr<Symbol> symbol = mSymbolEntry; symbol; symbol = symbol->GetNext())
		{
			if (symbol->GetClassTypeId() == Symbol::ClassTypeId::MemberFunction)
				++count;
		}
		return count;
	}

	bool TypeDescriptor::IsPhantom() const
	{
		return mShare.mActor.mPhantom;
	}

	void TypeDescriptor::SetSymbolEntry(const std::shared_ptr<Symbol>& symbolEntry)
	{
		mSymbolEntry = symbolEntry;
	}

	void TypeDescriptor::SetName(const std::string_view name)
	{
		mName = name;
	}

	void TypeDescriptor::SetMemorySize(const address_t memorySize)
	{
		mMemorySize = memorySize;
	}

	void TypeDescriptor::SetAlignmentMemorySize(const address_t alignmentMemorySize)
	{
		mAlignmentMemorySize = alignmentMemorySize;
	}

	void TypeDescriptor::SetArraySize(const address_t arraySize)
	{
		mArraySize = arraySize;
	}

	const std::shared_ptr<Symbol>& TypeDescriptor::GetParent() const
	{
		return mParent;
	}

	void TypeDescriptor::SetParent(const std::shared_ptr<Symbol>& parent)
	{
		mParent = parent;
	}

	void TypeDescriptor::Dump(std::ofstream& output) const
	{
		output << GetDataTypeName(mTcons);
		switch (mTcons)
		{
		case Id::Reference:
			output << "\\<";
			mComponent->Dump(output);
			output << "\\>";
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
		case Id::Actor:
		case Id::Module:
			output << " { ";
			output << "0x" << std::setfill('0') << std::hex << std::setw(8) << mMemorySize;
			output << " bytes ";
			output << "aligment 0x" << std::setfill('0') << std::hex << std::setw(8) << mAlignmentMemorySize;
			output << " }";
			break;

		default:
			break;
		}
	}
}
