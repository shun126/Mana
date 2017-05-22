/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include <memory>
#include <vector>

namespace mana
{
	/*
	Data Section Class
	*/
	class DataBuffer final : private Noncopyable
	{
		static constexpr size_t nil = static_cast<size_t>(~0);

	public:
		DataBuffer() noexcept;

		~DataBuffer() = default;

		const char* GetBuffer() const noexcept;

		size_t GetSize() const noexcept;

		size_t Get(const std::string_view& text) const noexcept;

		size_t Set(const std::string_view& text);

		bool Write(OutputStream& stream) const;

	private:
		size_t Find(const std::string_view& text) const noexcept;

	private:
		std::vector<size_t> mEntities;
		std::unique_ptr<char, decltype(&std::free)> mBuffer;
		size_t mAllocatedSize = 0;
		size_t mUsedSize = 0;
	};
}
