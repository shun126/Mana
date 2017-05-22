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
		static const size_t nil = static_cast<size_t>(~0);

		struct Command
		{
			uint8_t mCode;
			size_t mNextCommand;

			Command();
			
			explicit Command(const uint8_t code, const size_t nextCommand);
		};

	public:
		CodeBuffer();

		virtual ~CodeBuffer() = default;

		void Reduce(const size_t reduceSize);

		template <typename T>
		void Add(const T value);

		template <typename T>
		T Get(const size_t address);

		void AddOpecode(const IntermediateLanguage code);

		size_t AddOpecodeAndOperand(const IntermediateLanguage code, const size_t address);

		size_t AddOpecodeAndTwoOperands(const IntermediateLanguage code, const size_t address, const size_t size);

		void ReplaceOpecode(const size_t address, const IntermediateLanguage code);

		void ReplaceAddress(const size_t address, const size_t newAddress);

		void ReplaceAddressAll(const size_t baseAddress, const size_t newAddress);

		std::unique_ptr<void, decltype(&std::free)> Copy() const;

		size_t GetSize() const;

		void Write(OutputStream& stream) const;

	private:
		size_t AddCommand(const uint8_t code, const size_t nextCommand);

	private:
		std::vector<Command> mCommand;
	};
}

#include "CodeBuffer.h"
