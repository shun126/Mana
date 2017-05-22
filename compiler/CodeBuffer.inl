/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "CodeBuffer.h"

namespace mana
{
	CodeBuffer::Command::Command()
		: mCode(0)
		, mNextCommand(0)
	{
	}

	CodeBuffer::Command::Command(const uint8_t code, const size_t nextCommand)
		: mCode(code)
		, mNextCommand(nextCommand)
	{
	}

	CodeBuffer::CodeBuffer()
	{
	}

	void CodeBuffer::Reduce(const size_t reduceSize)
	{
		mCommand.resize(mCommand.size() - reduceSize);
	}

	template <typename T>
	void CodeBuffer::Add(const T value)
	{
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
		if (IsBigEndian())
		{
			for (size_t i = 0; i < sizeof(T); ++i)
				AddCommand(p[i], nil);
		}
		else
		{
			for (int_fast8_t i = static_cast<uint_fast8_t>(sizeof(T) - 1); i >= 0; --i)
				AddCommand(p[i], nil);
		}
	}

	template <typename T>
	T CodeBuffer::Get(const size_t address)
	{
		T value;
		uint8_t* p = reinterpret_cast<uint8_t*>(&value);
		if (IsBigEndian())
		{
			for (size_t i = 0; i < sizeof(T); ++i)
				p[i] = mCommand[address + i].mCode;
		}
		else
		{
			for (int_fast8_t i = static_cast<uint_fast8_t>(sizeof(T) - 1); i >= 0; --i)
				p[i] = mCommand[address + i].mCode;
		}
		return value;
	}

	void CodeBuffer::AddOpecode(const IntermediateLanguage code)
	{
		AddCommand(code, nil);
	}

	size_t CodeBuffer::AddOpecodeAndOperand(const IntermediateLanguage code, const size_t address)
	{
		const auto pc = GetSize();
		AddCommand(code, address);
		Add(address);
		return pc;
	}

	size_t CodeBuffer::AddOpecodeAndTwoOperands(const IntermediateLanguage code, const size_t address, const size_t size)
	{
		const auto pc = GetSize();
		AddCommand(code, address);
		Add(address);
		Add(size);
		return pc;
	}

	void CodeBuffer::ReplaceOpecode(const size_t address, const IntermediateLanguage code)
	{
		mCommand[address].mCode = code;
	}

	void CodeBuffer::ReplaceAddress(const size_t address, const size_t newAddress)
	{
		mCommand[address + 0].mCode = (const uint8_t)(newAddress >> 24);
		mCommand[address + 1].mCode = (const uint8_t)(newAddress >> 16);
		mCommand[address + 2].mCode = (const uint8_t)(newAddress >> 8);
		mCommand[address + 3].mCode = (const uint8_t)(newAddress);
	}

	void CodeBuffer::ReplaceAddressAll(const size_t baseAddress, const size_t newAddress)
	{
		size_t address = baseAddress;
		while (address != nil)
		{
			const size_t nextAddress = mCommand[address].mNextCommand;
			mCommand[address + 0].mNextCommand = nil;
			mCommand[address + 1].mCode = (const uint8_t)(newAddress >> 24);
			mCommand[address + 2].mCode = (const uint8_t)(newAddress >> 16);
			mCommand[address + 3].mCode = (const uint8_t)(newAddress >> 8);
			mCommand[address + 4].mCode = (const uint8_t)(newAddress);
			address = nextAddress;
		}
	}

	std::unique_ptr<void, decltype(&std::free)> CodeBuffer::Copy() const
	{
		std::unique_ptr<void, decltype(&std::free)> buffer(std::malloc(mCommand.size()), std::free);
		if (buffer.get())
		{
			IntermediateLanguage* pointer = static_cast<IntermediateLanguage*>(buffer.get());
			for (auto command : mCommand)
			{
				*pointer++ = static_cast<IntermediateLanguage>(command.mCode);
			}
		}
		return std::move(buffer);
	}

	size_t CodeBuffer::GetSize() const
	{
		return mCommand.size();
	}

	void CodeBuffer::Write(OutputStream& stream) const
	{
		for (const auto& command : mCommand)
		{
			stream.Push(command.mCode);
		}
	}

	size_t CodeBuffer::AddCommand(const uint8_t code, const size_t nextCommand)
	{
		const auto pc = mCommand.size();
		mCommand.push_back(Command(code, nextCommand));
		return pc;
	}
}
