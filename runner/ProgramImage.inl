/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "ProgramImage.h"

#include <algorithm>
#include <cstring>
#include <limits>

namespace mana
{
	bool ProgramImage::LoadProgram(const std::shared_ptr<const void> programBytes, const size_t programSizeBytes)
	{
		Clear();

		if (programBytes == nullptr || programSizeBytes == 0)
		{
			mLastError = "program buffer is empty";
			return false;
		}

		mProgram = programBytes;
		mProgramSize = programSizeBytes;

		const auto* data = static_cast<const std::byte*>(mProgram.get());
		if (!ParseAndBuildIndex(data, mProgramSize))
		{
			const std::string error = mLastError;
			Clear();
			mLastError = error;
			return false;
		}

		mLoaded = true;
		return true;
	}

	bool ProgramImage::IsLoaded() const noexcept
	{
		return mLoaded;
	}

	const std::string& ProgramImage::GetLastError() const noexcept
	{
		return mLastError;
	}

	std::vector<std::string_view> ProgramImage::GetActorNames() const
	{
		return mActorNames;
	}

	bool ProgramImage::HasActor(const std::string_view actorFullName) const
	{
		if (!mLoaded)
			return false;
		return mActors.find(actorFullName) != mActors.end();
	}

	std::vector<std::string_view> ProgramImage::GetActorActionNames(const std::string_view actorFullName) const
	{
		if (!mLoaded)
			return {};
		const auto found = mActors.find(actorFullName);
		if (found == mActors.end())
			return {};
		return found->second.mActionNames;
	}

	bool ProgramImage::HasActorAction(const std::string_view actorFullName, const std::string_view actionName) const
	{
		if (!mLoaded)
			return false;
		const auto found = mActors.find(actorFullName);
		if (found == mActors.end())
			return false;
		const auto& actionNames = found->second.mActionNames;
		return std::binary_search(actionNames.begin(), actionNames.end(), actionName);
	}

	bool ProgramImage::HasPhantom(const std::string_view phantomFullName) const
	{
		if (!mLoaded)
			return false;
		return mPhantoms.find(phantomFullName) != mPhantoms.end();
	}

	std::vector<std::string_view> ProgramImage::GetPhantomActionNames(const std::string_view phantomFullName) const
	{
		if (!mLoaded)
			return {};
		const auto found = mPhantoms.find(phantomFullName);
		if (found == mPhantoms.end())
			return {};
		return found->second.mActionNames;
	}

	bool ProgramImage::HasPhantomAction(const std::string_view phantomFullName, const std::string_view actionName) const
	{
		if (!mLoaded)
			return false;
		const auto found = mPhantoms.find(phantomFullName);
		if (found == mPhantoms.end())
			return false;
		const auto& actionNames = found->second.mActionNames;
		return std::binary_search(actionNames.begin(), actionNames.end(), actionName);
	}

	void ProgramImage::Clear()
	{
		mProgram.reset();
		mProgramSize = 0;
		mLastError.clear();
		mLoaded = false;
		mActors.clear();
		mPhantoms.clear();
		mActorNames.clear();
		mPhantomNames.clear();
	}

	bool ProgramImage::ParseAndBuildIndex(const std::byte* data, const size_t size)
	{
		auto setError = [this](const std::string& message) -> bool
		{
			mLastError = message;
			return false;
		};

		auto addChecked = [&](const size_t value, const size_t add, size_t& result) -> bool
		{
			if (add > (std::numeric_limits<size_t>::max() - value))
				return setError("size overflow");
			result = value + add;
			return true;
		};

		if (size < sizeof(FileHeader))
			return setError("program buffer is too small for file header");

		FileHeader header = {};
		std::memcpy(&header, data, sizeof(header));

		if (std::memcmp(Signature, header.mHeader, sizeof(header.mHeader)) != 0)
			return setError("program signature mismatch");

		if (header.mMajorVersion != MajorVersion || header.mMinorVersion != MinorVersion)
			return setError("program version mismatch");

		{
#if UINTPTR_MAX == UINT64_MAX
			constexpr uint8_t is64bit = 1 << FileHeader::Flag::Is64Bit;
#else
			constexpr uint8_t is64bit = 0;
#endif
			if ((header.mFlag & (1 << FileHeader::Flag::Is64Bit)) != is64bit)
				return setError("program bitness mismatch");
		}

		{
			const bool fileIsBigEndian = (header.mFlag & (1 << FileHeader::Flag::BigEndian)) != 0;
			if (fileIsBigEndian != IsBigEndian())
				return setError("program endianness mismatch");
		}

		size_t offset = sizeof(FileHeader);
		for (uint32_t i = 0; i < header.mNumberOfActors; ++i)
		{
			size_t nextOffset = 0;
			if (!addChecked(offset, sizeof(ActorInfoHeader), nextOffset) || nextOffset > size)
				return setError("actor info header out of bounds");
			ActorInfoHeader actorInfo = {};
			std::memcpy(&actorInfo, data + offset, sizeof(actorInfo));
			offset = nextOffset;

			const size_t actionCount = actorInfo.mNumberOfActions;
			if (actionCount > (std::numeric_limits<size_t>::max() / sizeof(ActionInfoHeader)))
				return setError("actor action count overflow");
			const size_t actionBytes = actionCount * sizeof(ActionInfoHeader);
			if (!addChecked(offset, actionBytes, nextOffset) || nextOffset > size)
				return setError("action info header out of bounds");
			offset = nextOffset;
		}

		const size_t constantPoolOffset = offset;
		const size_t constantPoolSize = header.mSizeOfConstantPool;
		size_t afterConstantPool = 0;
		if (!addChecked(constantPoolOffset, constantPoolSize, afterConstantPool) || afterConstantPool > size)
			return setError("constant pool out of bounds");

		const size_t instructionPoolOffset = afterConstantPool;
		const size_t instructionPoolSize = header.mSizeOfInstructionPool;
		size_t afterInstructionPool = 0;
		if (!addChecked(instructionPoolOffset, instructionPoolSize, afterInstructionPool) || afterInstructionPool > size)
			return setError("instruction pool out of bounds");

		const auto* constantPool = reinterpret_cast<const char*>(data + constantPoolOffset);

		auto getStringView = [&](const uint32_t stringOffset, std::string_view& out) -> bool
		{
			if (stringOffset >= constantPoolSize)
				return setError("string offset out of bounds");
			const char* start = constantPool + stringOffset;
			const size_t remaining = constantPoolSize - stringOffset;
			const void* terminator = std::memchr(start, '\0', remaining);
			if (terminator == nullptr)
				return setError("string terminator not found");
			const auto* end = static_cast<const char*>(terminator);
			out = std::string_view(start, static_cast<size_t>(end - start));
			return true;
		};

		offset = sizeof(FileHeader);
		for (uint32_t i = 0; i < header.mNumberOfActors; ++i)
		{
			size_t nextOffset = 0;
			if (!addChecked(offset, sizeof(ActorInfoHeader), nextOffset) || nextOffset > size)
				return setError("actor info header out of bounds");
			ActorInfoHeader actorInfo = {};
			std::memcpy(&actorInfo, data + offset, sizeof(actorInfo));
			offset = nextOffset;

			std::string_view actorName;
			if (!getStringView(actorInfo.mName, actorName))
				return false;

			SymbolIndex symbolIndex;
			symbolIndex.mActionNames.reserve(actorInfo.mNumberOfActions);

			for (uint16_t actionIndex = 0; actionIndex < actorInfo.mNumberOfActions; ++actionIndex)
			{
				if (!addChecked(offset, sizeof(ActionInfoHeader), nextOffset) || nextOffset > size)
					return setError("action info header out of bounds");
				ActionInfoHeader actionInfo = {};
				std::memcpy(&actionInfo, data + offset, sizeof(actionInfo));
				offset = nextOffset;

				std::string_view actionName;
				if (!getStringView(actionInfo.mName, actionName))
					return false;
				symbolIndex.mActionNames.push_back(actionName);
			}

			std::sort(symbolIndex.mActionNames.begin(), symbolIndex.mActionNames.end());
			symbolIndex.mActionNames.erase(std::unique(symbolIndex.mActionNames.begin(), symbolIndex.mActionNames.end()), symbolIndex.mActionNames.end());

			if (actorInfo.mFlag & (1 << ActorInfoHeader::Flag::Phantom))
			{
				if (mPhantoms.find(actorName) != mPhantoms.end())
					return setError("duplicate phantom name detected");
				mPhantoms.emplace(actorName, std::move(symbolIndex));
				mPhantomNames.push_back(actorName);
			}
			else
			{
				if (mActors.find(actorName) != mActors.end())
					return setError("duplicate actor name detected");
				mActors.emplace(actorName, std::move(symbolIndex));
				mActorNames.push_back(actorName);
			}
		}

		mLastError.clear();
		return true;
	}
}
