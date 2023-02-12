/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "LocalAddressResolver.h"
#include "CodeGenerator.h"
#include "ErrorHandler.h"

namespace mana
{
	LocalAddressResolver::LocalAddressResolver(const std::shared_ptr<CodeBuffer>& codeBuffer)
		: mCodeBuffer(codeBuffer)
	{
		mJumpChainTablePointer = 0;

		mJumpSwitchStackPointer = 0;
		mJumpSwitchEntryStackPointer = mJumpSwitchEntryStack;
	}

	void LocalAddressResolver::OpenChain(JumpChainStatus status)
	{
		++mJumpChainTablePointer;
		mJumpChainTable[mJumpChainTablePointer].status = status;
		mJumpChainTable[mJumpChainTablePointer].break_chain = -1;
		mJumpChainTable[mJumpChainTablePointer].continue_chain = -1;
		mJumpChainTable[mJumpChainTablePointer].start_address = mCodeBuffer->GetSize();
	}

	int32_t LocalAddressResolver::Break(const int32_t newPc)
	{
		int32_t oldPc = -1;

		if (mJumpChainTablePointer > 0)
		{
			oldPc = mJumpChainTable[mJumpChainTablePointer].break_chain;
			mJumpChainTable[mJumpChainTablePointer].break_chain = newPc;
		}
		else
		{
			CompileError("illegal use of break statement");
		}

		return oldPc;
	}

	int32_t LocalAddressResolver::Continue(const int32_t newPc)
	{
		int32_t i;
		int32_t oldPc = -1;

		for (i = mJumpChainTablePointer; i > 0 && mJumpChainTable[i].status == JumpChainStatus::Switch; i--)
			;
		if (i > 0)
		{
			oldPc = mJumpChainTable[i].continue_chain;
			mJumpChainTable[i].continue_chain = newPc;
		}
		else
		{
			CompileError("illegal use of the continue");
		}

		return oldPc;
	}

	void LocalAddressResolver::CloseContinueOnly(void)
	{
		mCodeBuffer->ReplaceAddressAll(
			mJumpChainTable[mJumpChainTablePointer].continue_chain,
			mCodeBuffer->GetSize()
		);
		mJumpChainTable[mJumpChainTablePointer].continue_chain = -1;
	}

	void LocalAddressResolver::CloseChain(void)
	{
		mCodeBuffer->ReplaceAddressAll(
			mJumpChainTable[mJumpChainTablePointer].continue_chain,
			mJumpChainTable[mJumpChainTablePointer].start_address
		);
		mCodeBuffer->ReplaceAddressAll(
			mJumpChainTable[mJumpChainTablePointer].break_chain,
			mCodeBuffer->GetSize()
		);
		--mJumpChainTablePointer;
	}

	void LocalAddressResolver::OpenSwitchBlock(const std::shared_ptr<TypeDescriptor>& type)
	{
		++mJumpSwitchStackPointer;
		mJumpSwitchStack[mJumpSwitchStackPointer].stack_pointer = mJumpSwitchEntryStackPointer;
		mJumpSwitchStack[mJumpSwitchStackPointer].default_address = -1;
		mJumpSwitchStack[mJumpSwitchStackPointer].type = type;
	}

	void LocalAddressResolver::RegistSwitchCase(const std::shared_ptr<SyntaxNode>& node)
	{
		if (mJumpSwitchStackPointer > 0)
		{
			JumpSwitchEntry *p;
			mJumpSwitchEntryStackPointer->node = node;
			for (p = mJumpSwitchStack[mJumpSwitchStackPointer].stack_pointer; p->node != node; p++)
				;
			if (p >= mJumpSwitchEntryStackPointer)
			{
				mJumpSwitchEntryStackPointer->address = mCodeBuffer->GetSize();
				mJumpSwitchEntryStackPointer++;
			}
			else
			{
				CompileError("duplicated case constant");
			}
		}
	}

	void LocalAddressResolver::RegistSwitchDefault(void)
	{
		if (mJumpSwitchStackPointer > 0 && mJumpSwitchStack[mJumpSwitchStackPointer].default_address < 0)
		{
			mJumpSwitchStack[mJumpSwitchStackPointer].default_address = mCodeBuffer->GetSize();
		}
		else
		{
			CompileError("illegal default label");
		}
	}

	void LocalAddressResolver::ResolveSwitchBlock(const std::shared_ptr<CodeGenerator>& codeGenerator)
	{
		JumpSwitchEntry *p;

		for (p = mJumpSwitchStack[mJumpSwitchStackPointer].stack_pointer; p < mJumpSwitchEntryStackPointer; p++)
		{
			int32_t size;

			switch (p->node->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Int:
			case TypeDescriptor::Id::Actor:
				mCodeBuffer->AddOpecode(IntermediateLanguage::Duplicate);
				codeGenerator->generator_expression(p->node, false);
				mCodeBuffer->AddOpecode(IntermediateLanguage::COMPARE_EQ_INTEGER);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BNE, p->address);
				break;

			case TypeDescriptor::Id::Float:
				mCodeBuffer->AddOpecode(IntermediateLanguage::Duplicate);
				codeGenerator->generator_expression(p->node, false);
				mCodeBuffer->AddOpecode(IntermediateLanguage::COMPARE_EQ_FLOAT);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BNE, p->address);
				break;

			case TypeDescriptor::Id::Struct:
				size = p->node->GetTypeDescriptor()->GetMemorySize();
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::DUPLICATE_DATA, size);
				codeGenerator->generator_expression(p->node, false);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::COMPARE_EQ_DATA, size);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BNE, p->address);
				break;

			default:
				MANA_BUG("Illigal type");
				break;
			}
		}

		if (mJumpSwitchStack[mJumpSwitchStackPointer].default_address > 0)
		{
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BRA, mJumpSwitchStack[mJumpSwitchStackPointer].default_address);
		}

		mJumpSwitchEntryStackPointer = mJumpSwitchStack[mJumpSwitchStackPointer].stack_pointer;
	}

	void LocalAddressResolver::CloseSwitchBlock(void)
	{
		switch (mJumpSwitchStack[mJumpSwitchStackPointer].type->GetId())
		{
		case TypeDescriptor::Id::Char:
		case TypeDescriptor::Id::Short:
		case TypeDescriptor::Id::Int:
		case TypeDescriptor::Id::Float:
		case TypeDescriptor::Id::Actor:
			mCodeBuffer->AddOpecode(IntermediateLanguage::REMOVE);
			break;

		case TypeDescriptor::Id::Struct:
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::REMOVE_DATA, mJumpSwitchStack[mJumpSwitchStackPointer].type->GetMemorySize());
			break;

		default:
			MANA_BUG("Illigal type");
			break;
		}

		--mJumpSwitchStackPointer;
	}
}
