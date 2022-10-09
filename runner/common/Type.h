/*!
mana (compiler/library)

@author	Shun Moriya
@date	2022-
*/

#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace mana
{
#if defined(MANA_TARGET_WINDOWS)
	//! Signed size integer type.
	using ssize_t = intptr_t;
#endif

#if UINTPTR_MAX == UINT64_MAX
#elif UINTPTR_MAX == UINT32_MAX
#else
#error "unsupport pointer size"
#endif
	using float_t = float;
	using int_t = std::int32_t;

	using address_t = std::uint32_t;
	static constexpr address_t InvalidAddress = static_cast<address_t>(~0);

	using offset_t = std::int32_t;

	inline bool IsValid(const address_t address)
	{
		return address != InvalidAddress;
	}

	template<typename T>
	inline address_t ToAddress(const T size)
	{
		if constexpr (std::is_signed<T>())
		{
			if (size < 0)
			{
				throw std::underflow_error("Negative values for data or program size are not allowed");
			}
			else if (static_cast<size_t>(size) > std::numeric_limits<address_t>::max())
			{
				throw std::overflow_error("Data or program size must not exceed 32 bits");
			}
		}
		else
		{
			if (static_cast<size_t>(size) > std::numeric_limits<address_t>::max())
			{
				throw std::overflow_error("Data or program size must not exceed 32 bits");
			}
		}
		return static_cast<address_t>(size);
	}
}
