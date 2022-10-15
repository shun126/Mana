/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "CodeBuffer.h"
#include "../runner/common/OutputStream.h"

namespace mana
{
	CodeBuffer::Command::Command(const uint8_t code, const address_t nextCommand)
		: mCode(code)
		, mNextCommand(nextCommand)
	{
	}

	void CodeBuffer::Reduce(const address_t reduceSize)
	{
		mCommand.resize(mCommand.size() - reduceSize);
	}

	void CodeBuffer::AddOpecode(const IntermediateLanguage code)
	{
		AddCommand(code, InvalidAddress);
	}

	address_t CodeBuffer::AddOpecodeAndOperand(const IntermediateLanguage code, const address_t address)
	{
		const auto pc = GetSize();
		AddCommand(code, address);
		Add(address);
		return pc;
	}

	address_t CodeBuffer::AddOpecodeAndTwoOperands(const IntermediateLanguage code, const address_t address, const address_t size)
	{
		const auto pc = GetSize();
		AddCommand(code, address);
		Add(address);
		Add(size);
		return pc;
	}

	void CodeBuffer::ReplaceOpecode(const address_t address, const IntermediateLanguage code)
	{
		mCommand[address].mCode = code;
	}

	void CodeBuffer::ReplaceAddress(const address_t address, const address_t newAddress)
	{
		Replace(address, newAddress);
	}

	void CodeBuffer::ReplaceAddressAll(const address_t baseAddress, const address_t newAddress)
	{
		address_t address = baseAddress;
		while (address != InvalidAddress)
		{
			const address_t nextAddress = mCommand[address].mNextCommand;
			mCommand[address + 0].mNextCommand = InvalidAddress;
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
		return buffer;
	}

	address_t CodeBuffer::GetSize() const
	{
		return static_cast<address_t>(mCommand.size());
	}

	void CodeBuffer::Write(OutputStream& stream) const
	{
		for (const auto& command : mCommand)
		{
			stream.Push(command.mCode);
		}
	}

	address_t CodeBuffer::AddCommand(const uint8_t code, const address_t nextCommand)
	{
		const address_t pc = static_cast<address_t>(mCommand.size());
		mCommand.push_back(Command(code, nextCommand));
		return pc;
	}
}
