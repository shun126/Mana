/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include "../runner/common/FileFormat.h"
#include "../runner/common/Noncopyable.h"
#include <cstdlib>
#include <memory>
#include <vector>

namespace mana
{
	class OutputStream;

	/*
	Code Section Class
	*/
	class CodeBuffer final : private Noncopyable
	{
		struct Command
		{
			uint8_t mCode = 0;
			address_t mNextCommand = 0;

			Command() = default;
			
			explicit Command(const uint8_t code, const address_t nextCommand);
		};

	public:
		CodeBuffer() = default;
		~CodeBuffer() = default;

		void Reduce(const address_t reduceSize);

		template <typename T>
		void Add(const T value);

		template <typename T>
		T Get(const address_t address) const;

		void AddOpecode(const IntermediateLanguage code);

		address_t AddOpecodeAndOperand(const IntermediateLanguage code, const address_t address = InvalidAddress);

		address_t AddOpecodeAndTwoOperands(const IntermediateLanguage code, const address_t address, const address_t size);

		void ReplaceOpecode(const address_t address, const IntermediateLanguage code);

		void ReplaceAddress(const address_t address, const address_t newAddress);

		void ReplaceAddressAll(const address_t baseAddress, const address_t newAddress);

		std::unique_ptr<void, decltype(&std::free)> Copy() const;
	
		address_t GetSize() const;

		void Write(OutputStream& stream) const;

	private:
		address_t AddCommand(const uint8_t code, const address_t nextCommand);

		template <typename T>
		void Replace(const address_t address, const T value);

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
				AddCommand(reinterpret_cast<const uint8_t*>(&value)[i], InvalidAddress);
		}
		else
		{
			for (int_fast8_t i = static_cast<uint_fast8_t>(sizeof(T) - 1); i >= 0; --i)
				AddCommand(reinterpret_cast<const uint8_t*>(&value)[i], InvalidAddress);
		}
	}

	/*
	runner\common\FileFormat.h getと重複
	*/
	template <typename T>
	T CodeBuffer::Get(const address_t address) const
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
	void CodeBuffer::Replace(const address_t address, const T value)
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
