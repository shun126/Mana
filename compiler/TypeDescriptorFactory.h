/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "TypeDescriptor.h"
#include <memory>
#include <vector>

namespace mana
{
	class TypeDescriptorFactory final : private Noncopyable
	{
	public:
		TypeDescriptorFactory();
		~TypeDescriptorFactory();

		std::shared_ptr<TypeDescriptor> Get(const TypeDescriptor::Id id) const;
		std::shared_ptr<TypeDescriptor> GetString() const;

		std::shared_ptr<TypeDescriptor> Create(const TypeDescriptor::Id tcons);
		std::shared_ptr<TypeDescriptor> CreateReference(const std::shared_ptr<TypeDescriptor>& component);
		std::shared_ptr<TypeDescriptor> CreateArray(const size_t arraySize);

		static bool Compare(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right);
		static bool Compatible(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right);

	private:
		std::vector<std::shared_ptr<TypeDescriptor>> mTypeDescriptor;
		std::shared_ptr<TypeDescriptor> mStringTypeDescriptor = nullptr;
	};
}
