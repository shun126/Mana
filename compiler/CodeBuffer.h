/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include "../runner/common/FileFormat.h"
#include "../runner/common/Memory.h"
#include "../runner/common/Noncopyable.h"
#include <cstdlib>
#include <memory>
#include <vector>

namespace mana
{
	class OutputStream;

	/*
	Code Section
	コードセクション
	*/
	class CodeBuffer final : Noncopyable
	{
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

		template <typename T>
		static T Raw(const void* program, const address_t address);

	private:
		address_t AddCommand(const IntermediateLanguage code, const address_t nextCommand);
		address_t AddCommand(const uint8_t code, const address_t nextCommand);

		template <typename T>
		void Replace(const address_t address, const T value);

	private:
		struct Command final
		{
			uint8_t mCode = 0;
			address_t mNextCommand = 0;

			Command() = default;

			Command(const IntermediateLanguage code, const address_t nextCommand) noexcept
				: mCode(static_cast<uint8_t>(code))
				, mNextCommand(nextCommand)
			{
			}

			Command(const uint8_t data, const address_t nextCommand) noexcept
				: mCode(data)
				, mNextCommand(nextCommand)
			{
			}

			void Replace(const IntermediateLanguage code) noexcept
			{
				mCode = static_cast<uint8_t>(code);
			}

			void Replace(const uint8_t data) noexcept
			{
				mCode = data;
			}
		};

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

	template <typename T>
	void CodeBuffer::Replace(const address_t address, const T value){
		const uint8_t* pointer = reinterpret_cast<const uint8_t*>(&value);
		// Stored in BigEndian in CodeBuffer
		if (IsBigEndian())
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				mCommand[address + i].mCode = pointer[i];
		}
		else
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				mCommand[address + i].mCode = pointer[static_cast<uint_fast8_t>(sizeof(T) - 1) - i];
		}
	}

	/*
	runner\common\FileFormat.h getと重複
	*/
	template <typename T>
	T CodeBuffer::Get(const address_t address) const
	{
		T value;
		uint8_t* pointer = reinterpret_cast<uint8_t*>(&value);
		// Stored in BigEndian in CodeBuffer
		if (IsBigEndian())
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				pointer[i] = mCommand[address + i].mCode;
		}
		else
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				pointer[i] = mCommand[address + static_cast<uint_fast8_t>(sizeof(T) - 1) - i].mCode;
		}
		return value;
	}

	template <typename T>
	T CodeBuffer::Raw(const void* program, const address_t address)
	{
		T value;
		uint8_t* pointer = reinterpret_cast<uint8_t*>(&value);
		const uint8_t* source = static_cast<const uint8_t*>(program) + address;
		// Stored in BigEndian in CodeBuffer
		if (IsBigEndian())
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				pointer[i] = source[i];
		}
		else
		{
			for (uint_fast8_t i = 0; i < static_cast<uint_fast8_t>(sizeof(T)); ++i)
				pointer[i] = source[static_cast<int_fast16_t>(sizeof(T) - 1) - i];
		}
		return value;
	}
}
