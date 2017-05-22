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
	class CodeBuffer : private Noncopyable
	{
		struct Command
		{
			uint8_t mCode = 0;
			size_t mNextCommand = 0;

			Command() = default;
			
			explicit Command(const uint8_t code, const size_t nextCommand);
		};

	public:
		static const size_t nil = static_cast<size_t>(~0);

	public:
		CodeBuffer() = default;
		virtual ~CodeBuffer() = default;

		void Reduce(const size_t reduceSize);

		template <typename T>
		void Add(const T value);

		template <typename T>
		T Get(const size_t address) const;

		void AddOpecode(const IntermediateLanguage code);

		size_t AddOpecodeAndOperand(const IntermediateLanguage code, const size_t address = nil);

		size_t AddOpecodeAndTwoOperands(const IntermediateLanguage code, const size_t address, const size_t size);

		void ReplaceOpecode(const size_t address, const IntermediateLanguage code);

		void ReplaceAddress(const size_t address, const size_t newAddress);

		void ReplaceAddressAll(const size_t baseAddress, const size_t newAddress);

		std::unique_ptr<void, decltype(&std::free)> Copy() const;

		size_t GetSize() const;

		void Write(OutputStream& stream) const;

	private:
		size_t AddCommand(const uint8_t code, const size_t nextCommand);

		template <typename T>
		void Replace(const size_t address, const T value);

	private:
		std::vector<Command> mCommand;
	};

	template <typename T>
	void CodeBuffer::Add(const T value)
	{
		// Stored in BigEndian in CodeBuffer
		if (IsBigEndian())
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				AddCommand(reinterpret_cast<const uint8_t*>(&value)[i], nil);
		}
		else
		{
			for (int_fast8_t i = static_cast<uint_fast8_t>(sizeof(T) - 1); i >= 0; --i)
				AddCommand(reinterpret_cast<const uint8_t*>(&value)[i], nil);
		}
	}

	/*
	runner\common\FileFormat.h getと重複
	*/
	template <typename T>
	T CodeBuffer::Get(const size_t address) const
	{
		T value;
		uint8_t* p = reinterpret_cast<uint8_t*>(&value);
		if (IsBigEndian())
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				p[i] = mCommand[address + i].mCode;
		}
		else
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				p[i] = mCommand[address + static_cast<uint_fast8_t>(sizeof(T) - 1) - i].mCode;
		}
		return value;
	}

	template <typename T>
	void CodeBuffer::Replace(const size_t address, const T value)
	{
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
		if (IsBigEndian())
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				mCommand[address + i].mCode = p[i];
		}
		else
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				mCommand[address + static_cast<uint_fast8_t>(sizeof(T) - 1) - i].mCode = p[i];
		}
	}
}
