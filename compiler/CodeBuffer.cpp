/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "CodeBuffer.h"

namespace mana
{
	CodeBuffer::Command::Command(const uint8_t code, const size_t nextCommand)
		: mCode(code)
		, mNextCommand(nextCommand)
	{
	}

	void CodeBuffer::Reduce(const size_t reduceSize)
	{
		mCommand.resize(mCommand.size() - reduceSize);
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
		Replace(address, newAddress);
	}

	void CodeBuffer::ReplaceAddressAll(const size_t baseAddress, const size_t newAddress)
	{
		size_t address = baseAddress;
		while (address != nil)
		{
			const size_t nextAddress = mCommand[address].mNextCommand;
			mCommand[address + 0].mNextCommand = nil;
			ReplaceAddress(address + 1, newAddress);
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
