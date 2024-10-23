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
		address_t mMemorySize;
		address_t mAlignmentSize;
	};

	static constexpr std::array<InitializeType, TypeDescriptor::TypeIdSize> InitializeType = { {
		{ std::string_view("void"), sizeof(int32_t), sizeof(int32_t) },
		{ std::string_view("char"), sizeof(int8_t), sizeof(int8_t) },
		{ std::string_view("short"), sizeof(int16_t), sizeof(int16_t) },
		{ std::string_view("int"), sizeof(int32_t), sizeof(int32_t) },
		{ std::string_view("float"), sizeof(float), sizeof(float) },
		{ std::string_view("reference"), sizeof(void*), sizeof(void*) },
		{ std::string_view("array"), sizeof(void*), sizeof(void*) },
		{ std::string_view("struct"), sizeof(void*), sizeof(void*) },
		{ std::string_view("actor"), sizeof(void*), sizeof(void*) },
		{ std::string_view("module"), sizeof(void*), sizeof(void*) },
		{ std::string_view("Nil"), sizeof(void*), sizeof(void*) },
		{ std::string_view("unknown"), sizeof(void*), sizeof(void*) }
	} };

	TypeDescriptorFactory::TypeDescriptorFactory()
	{
		for(size_t i = 0; i < InitializeType.size(); ++i)
		{
			auto typeDescriptor = std::shared_ptr<TypeDescriptor>(new TypeDescriptor(static_cast<TypeDescriptor::Id>(i)));
			typeDescriptor->SetName(InitializeType[i].mName);
			typeDescriptor->SetMemorySize(InitializeType[i].mMemorySize);
			typeDescriptor->SetAlignmentMemorySize(InitializeType[i].mAlignmentSize);
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

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::Create(const TypeDescriptor::Id tcons)
	{
		auto newTypeDescriptor = std::shared_ptr<TypeDescriptor>(new TypeDescriptor(tcons));
		mTypeDescriptor.push_back(newTypeDescriptor);
		return newTypeDescriptor;
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::CreateReference(const std::shared_ptr<TypeDescriptor>& component)
	{
		const auto& referenceTypeDescriptor = Get(TypeDescriptor::Id::Reference);

		auto newTypeDescriptor = Create(TypeDescriptor::Id::Reference);
		newTypeDescriptor->SetTypeDescriptor(component);
		newTypeDescriptor->SetMemorySize(referenceTypeDescriptor->GetMemorySize());
		newTypeDescriptor->SetAlignmentMemorySize(referenceTypeDescriptor->GetAlignmentMemorySize());
		return newTypeDescriptor;
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::CreateArray(const address_t arraySize)
	{
		const auto& referenceTypeDescriptor = Get(TypeDescriptor::Id::Array);

		auto newTypeDescriptor = Create(TypeDescriptor::Id::Array);
		newTypeDescriptor->SetMemorySize(referenceTypeDescriptor->GetMemorySize());
		newTypeDescriptor->SetAlignmentMemorySize(referenceTypeDescriptor->GetAlignmentMemorySize());
		newTypeDescriptor->SetArraySize(arraySize);
		return newTypeDescriptor;
	}
}
