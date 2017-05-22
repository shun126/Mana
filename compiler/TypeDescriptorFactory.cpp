/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "TypeDescriptorFactory.h"
#include <array>

namespace mana
{
	struct InitializeType final
	{
		const std::string mName;
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
		for(size_t i = 0; i < TypeDescriptor::TypeIdSize; ++i)
		{
			std::unique_ptr<TypeDescriptor> typeDescriptor = std::make_unique<TypeDescriptor>(static_cast<TypeDescriptor::Id>(i));
			typeDescriptor->mName = mInitializeType[i].mName;
			typeDescriptor->mMemorySize = mInitializeType[i].mMemorySize;
			typeDescriptor->mAlignmentMemorySize = mInitializeType[i].mAlignmentSize;
			mTypeDescriptor.push_back(std::move(typeDescriptor));
		}

		mStringTypeDescriptor = CreateReference(Get(TypeDescriptor::Id::Char));
	}

	TypeDescriptorFactory::~TypeDescriptorFactory()
	{
		for (auto& typeDescriptor : mTypeDescriptor)
		{
			typeDescriptor.reset();
		}
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::Get(const TypeDescriptor::Id id) const
	{
		return mTypeDescriptor.at(static_cast<size_t>(id));
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::GetString() const
	{
		return mStringTypeDescriptor;
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::Create(const TypeDescriptor::Id tcons)
	{
		auto newTypeDescriptor = std::make_shared<TypeDescriptor>(tcons);
		mTypeDescriptor.push_back(newTypeDescriptor);
		return newTypeDescriptor;
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::CreateReference(const std::shared_ptr<TypeDescriptor>& component)
	{
		const auto& referenceTypeDescriptorFactory = Get(TypeDescriptor::Id::Reference);

		auto newTypeDescriptor = std::make_shared<TypeDescriptor>(TypeDescriptor::Id::Reference);
		newTypeDescriptor->SetTypeDescriptor(component);
		newTypeDescriptor->SetMemorySize(referenceTypeDescriptorFactory->GetMemorySize());
		newTypeDescriptor->mAlignmentMemorySize = referenceTypeDescriptorFactory->mAlignmentMemorySize;
		mTypeDescriptor.push_back(newTypeDescriptor);

		return newTypeDescriptor;
	}

	std::shared_ptr<TypeDescriptor> TypeDescriptorFactory::CreateArray(const size_t arraySize)
	{
		const auto& referenceTypeDescriptorFactory = Get(TypeDescriptor::Id::Array);

		auto newTypeDescriptor = std::make_shared<TypeDescriptor>(TypeDescriptor::Id::Array);
		newTypeDescriptor->mMemorySize = referenceTypeDescriptorFactory->mMemorySize;
		newTypeDescriptor->mAlignmentMemorySize = referenceTypeDescriptorFactory->mAlignmentMemorySize;
		newTypeDescriptor->mArraySize = arraySize;
		mTypeDescriptor.push_back(newTypeDescriptor);

		return newTypeDescriptor;
	}

	bool TypeDescriptorFactory::Compare(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right)
	{
		return left->Compare(right);
	}

	bool TypeDescriptorFactory::Compatible(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right)
	{
		return left->Compatible(right);
	}
}
