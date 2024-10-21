/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include "../runner/common/Noncopyable.h"
#include "TypeDescriptor.h"
#include <memory>
#include <vector>

namespace mana
{
	class TypeDescriptorFactory final : Noncopyable
	{
	public:
		TypeDescriptorFactory();
		~TypeDescriptorFactory() = default;

		[[nodiscard]] const std::shared_ptr<TypeDescriptor>& Get(const TypeDescriptor::Id id) const;
		[[nodiscard]] const std::shared_ptr<TypeDescriptor>& GetString() const;

		[[nodiscard]] std::shared_ptr<TypeDescriptor> Create(const TypeDescriptor::Id tcons);
		[[nodiscard]] std::shared_ptr<TypeDescriptor> CreateReference(const std::shared_ptr<TypeDescriptor>& component);
		[[nodiscard]] std::shared_ptr<TypeDescriptor> CreateArray(const address_t arraySize);

	private:
		std::vector<std::shared_ptr<TypeDescriptor>> mTypeDescriptor;
		std::shared_ptr<TypeDescriptor> mStringTypeDescriptor = nullptr;
	};
}
