/*!
mana (compiler/library)

@author	Shun Moriya
@date	2022-
*/

#pragma once
#include <cstddef>
#include <cstdint>

namespace mana
{
#if defined(MANA_TARGET_WINDOWS)
	//! Signed size integer type.
	using ssize_t = intptr_t;
#endif

#if UINTPTR_MAX == UINT64_MAX
	using float_t = double;
	using int_t = std::int64_t;
	using size_t = std::size_t;
#elif UINTPTR_MAX == UINT32_MAX
	using float_t = float;
	using int_t = std::int32_t;
	using size_t = std::size_t;
#else
#error "unsupport pointer size"
#endif

	using address_t = std::uint32_t;
	static constexpr address_t InvalidAddress = static_cast<address_t>(~0);

	using offset_t = std::int32_t;

	inline bool IsValid(const address_t address)
	{
		return address != InvalidAddress;
	}
}
