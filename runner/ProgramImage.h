/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Setup.h"

#include <cstddef>

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace mana
{
	class ProgramImage
	{
	public:
		ProgramImage() = default;

		// Initializes this instance from raw program bytes.
		// Returns true if successful; false if parsing/validation fails.
		// Store a diagnostic message on failure.
		bool LoadProgram(const std::shared_ptr<const void> programBytes, size_t programSizeBytes);

		bool IsLoaded() const noexcept;
		const std::string& GetLastError() const noexcept;

		// Actors
		std::vector<std::string_view> GetActorNames() const;
		bool HasActor(std::string_view actorFullName) const;

		std::vector<std::string_view> GetActorActionNames(std::string_view actorFullName) const;
		bool HasActorAction(std::string_view actorFullName, std::string_view actionName) const;

		// Phantoms
		bool HasPhantom(std::string_view phantomFullName) const;
		std::vector<std::string_view> GetPhantomActionNames(std::string_view phantomFullName) const;
		bool HasPhantomAction(std::string_view phantomFullName, std::string_view actionName) const;

	private:
		struct SymbolIndex
		{
			std::vector<std::string_view> mActionNames;
		};

		void Clear();

		// Internal parse entry point (split by concern).
		bool ParseAndBuildIndex(const std::byte* data, size_t size);

	private:
		std::shared_ptr<const void> mProgram;
		size_t mProgramSize = 0;
		std::string mLastError;
		bool mLoaded = false;

		std::unordered_map<std::string_view, SymbolIndex> mActors;
		std::unordered_map<std::string_view, SymbolIndex> mPhantoms;
		std::vector<std::string_view> mActorNames;
		std::vector<std::string_view> mPhantomNames;
	};
}


#include "ProgramImage.inl"