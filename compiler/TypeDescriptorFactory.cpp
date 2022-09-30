/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "TypeDescriptorFactory.h"
#include <array>
#include <string>

namespace mana
{
	struct InitializeType final
	{
		std::string_view mName;
		size_t mMemorySize;
		size_t mAlignmentSize;
	};

	static const std::array<InitializeType, TypeDescriptor::TypeIdSize> mInitializeType = { {
		{ "void", sizeof(int32_t), sizeof(int32_t) },
		{ "char", sizeof(int8_t), sizeof(int8_t) },
		{ "short", sizeof(int16_t), sizeof(int16_t) },
		{ "int", sizeof(int32_t), sizeof(int32_t) },
		{ "float", sizeof(float), sizeof(float) },
		{ "reference", sizeof(void*), sizeof(void*) },
		{ "array", sizeof(void*), sizeof(void*) },
		{ "struct", sizeof(void*), sizeof(void*) },
		{ "actor", sizeof(void*), sizeof(void*) },
		{ "module", sizeof(void*), sizeof(void*) },
		{ "nil", sizeof(void*), sizeof(void*) },
		{ "unknown", sizeof(void*), sizeof(void*) }
	} };

	TypeDescriptorFactory::TypeDescriptorFactory()
	{
		for(size_t i = 0; i < mInitializeType.size(); ++i)
		{
			auto typeDescriptor = std::shared_ptr<TypeDescriptor>(new TypeDescriptor(static_cast<TypeDescriptor::Id>(i)));
			typeDescriptor->SetName(mInitializeType[i].mName);
			typeDescriptor->SetMemorySize(mInitializeType[i].mMemorySize);
			typeDescriptor->SetAlignmentMemorySize(mInitializeType[i].mAlignmentSize);
			mTypeDescriptor.push_back(std::move(typeDescriptor));
		}

		mStringTypeDescriptor = CreateReference(Get(TypeDescriptor::Id::Char));
	}

	const std::shared_ptr<TypeDescriptor>& TypeDescriptorFactory::Get(const TypeDescriptor::Id id) const
	{
		return mTypeDescriptor.at(static_cast<size_t>(id));
	}

	const std::shared_ptr<TypeDescriptor>& TypeDescriptorFactory::GetString() const
	{
		return mStringTypeDescriptor;
	}

	const std::shared_ptr<TypeDescriptor>& TypeDescriptorFactory::Create(const TypeDescriptor::Id tcons)
	{
		auto newTypeDescriptor = std::shared_ptr<TypeDescriptor>(new TypeDescriptor(tcons));
		mTypeDescriptor.push_back(newTypeDescriptor);
		return newTypeDescriptor;
	}

	const std::shared_ptr<TypeDescriptor>& TypeDescriptorFactory::CreateReference(const std::shared_ptr<TypeDescriptor>& component)
	{
		const auto& referenceTypeDescriptor = Get(TypeDescriptor::Id::Reference);

		auto newTypeDescriptor = Create(TypeDescriptor::Id::Reference);
		newTypeDescriptor->SetTypeDescriptor(component);
		newTypeDescriptor->SetMemorySize(referenceTypeDescriptor->GetMemorySize());
		newTypeDescriptor->SetAlignmentMemorySize(referenceTypeDescriptor->GetAlignmentMemorySize());
		return newTypeDescriptor;
	}

	const std::shared_ptr<TypeDescriptor>& TypeDescriptorFactory::CreateArray(const size_t arraySize)
	{
		const auto& referenceTypeDescriptor = Get(TypeDescriptor::Id::Array);

		auto newTypeDescriptor = Create(TypeDescriptor::Id::Array);
		newTypeDescriptor->SetMemorySize(referenceTypeDescriptor->GetMemorySize());
		newTypeDescriptor->SetAlignmentMemorySize(referenceTypeDescriptor->GetAlignmentMemorySize());
		newTypeDescriptor->SetArraySize(arraySize);
		return newTypeDescriptor;
	}
}
