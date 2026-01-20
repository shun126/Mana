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
		, mJumpChainTable()
		, mJumpSwitchStack()
	{
		mJumpChainTablePointer = 0;

		mJumpSwitchStackPointer = 0;
		mJumpSwitchEntryStackPointer = mJumpSwitchEntryStack;
	}

	void LocalAddressResolver::OpenChain(JumpChainStatus status)
	{
		++mJumpChainTablePointer;
		mJumpChainTable[mJumpChainTablePointer].mStatus = status;
		mJumpChainTable[mJumpChainTablePointer].mBreakChain = -1;
		mJumpChainTable[mJumpChainTablePointer].mContinueChain = -1;
		mJumpChainTable[mJumpChainTablePointer].mStartAddress = mCodeBuffer->GetSize();
	}

	int32_t LocalAddressResolver::Break(const int32_t newPc)
	{
		int32_t oldPc = -1;

		if (mJumpChainTablePointer > 0)
		{
			oldPc = mJumpChainTable[mJumpChainTablePointer].mBreakChain;
			mJumpChainTable[mJumpChainTablePointer].mBreakChain = newPc;
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

		for (i = mJumpChainTablePointer; i > 0 && mJumpChainTable[i].mStatus == JumpChainStatus::Switch; i--)
			;
		if (i > 0)
		{
			oldPc = mJumpChainTable[i].mContinueChain;
			mJumpChainTable[i].mContinueChain = newPc;
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
			mJumpChainTable[mJumpChainTablePointer].mContinueChain,
			mCodeBuffer->GetSize()
		);
		mJumpChainTable[mJumpChainTablePointer].mContinueChain = -1;
	}

	void LocalAddressResolver::CloseChain(void)
	{
		mCodeBuffer->ReplaceAddressAll(
			mJumpChainTable[mJumpChainTablePointer].mContinueChain,
			mJumpChainTable[mJumpChainTablePointer].mStartAddress
		);
		mCodeBuffer->ReplaceAddressAll(
			mJumpChainTable[mJumpChainTablePointer].mBreakChain,
			mCodeBuffer->GetSize()
		);
		--mJumpChainTablePointer;
	}

	void LocalAddressResolver::OpenSwitchBlock(const std::shared_ptr<TypeDescriptor>& type)
	{
		++mJumpSwitchStackPointer;
		mJumpSwitchStack[mJumpSwitchStackPointer].mStackPointer = mJumpSwitchEntryStackPointer;
		mJumpSwitchStack[mJumpSwitchStackPointer].mDefaultAddress = -1;
		mJumpSwitchStack[mJumpSwitchStackPointer].mType = type;
	}

	void LocalAddressResolver::RegisterSwitchCase(const std::shared_ptr<SyntaxNode>& node)
	{
		if (mJumpSwitchStackPointer > 0)
		{
			JumpSwitchEntry *p;
			mJumpSwitchEntryStackPointer->mNode = node;
			for (p = mJumpSwitchStack[mJumpSwitchStackPointer].mStackPointer; p->mNode != node; p++)
				;
			if (p >= mJumpSwitchEntryStackPointer)
			{
				mJumpSwitchEntryStackPointer->mAddress = mCodeBuffer->GetSize();
				mJumpSwitchEntryStackPointer++;
			}
			else
			{
				CompileError("duplicated case constant");
			}
		}
	}

	void LocalAddressResolver::RegisterSwitchDefault(void)
	{
		if (mJumpSwitchStackPointer > 0 && mJumpSwitchStack[mJumpSwitchStackPointer].mDefaultAddress < 0)
		{
			mJumpSwitchStack[mJumpSwitchStackPointer].mDefaultAddress = mCodeBuffer->GetSize();
		}
		else
		{
			CompileError("illegal default label");
		}
	}

	void LocalAddressResolver::ResolveSwitchBlock(const std::shared_ptr<CodeGenerator>& codeGenerator)
	{
		for (const auto* p = mJumpSwitchStack[mJumpSwitchStackPointer].mStackPointer; p < mJumpSwitchEntryStackPointer; p++)
		{
			switch (p->mNode->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Bool:
			case TypeDescriptor::Id::Int:
			case TypeDescriptor::Id::Actor:
				mCodeBuffer->AddOpecode(IntermediateLanguage::Duplicate);
				codeGenerator->Expression(p->mNode, false);
				mCodeBuffer->AddOpecode(IntermediateLanguage::CompareEqualInteger);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BranchNotEqual, p->mAddress);
				break;

			case TypeDescriptor::Id::Float:
				mCodeBuffer->AddOpecode(IntermediateLanguage::Duplicate);
				codeGenerator->Expression(p->mNode, false);
				mCodeBuffer->AddOpecode(IntermediateLanguage::CompareEqualFloat);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BranchNotEqual, p->mAddress);
				break;

			case TypeDescriptor::Id::Struct:
			{
				const auto size = p->mNode->GetTypeDescriptor()->GetMemorySize();
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::DuplicateData, size);
				codeGenerator->Expression(p->mNode, false);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::CompareEqualData, size);
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BranchNotEqual, p->mAddress);
				break;
			}

			case TypeDescriptor::Id::Void:
			case TypeDescriptor::Id::Reference:
			case TypeDescriptor::Id::Array:
			case TypeDescriptor::Id::Module:
			case TypeDescriptor::Id::Nil:
			case TypeDescriptor::Id::Incomplete:
			default:
				MANA_BUG("Illegal type");
			}
		}

		if (mJumpSwitchStack[mJumpSwitchStackPointer].mDefaultAddress > 0)
		{
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, mJumpSwitchStack[mJumpSwitchStackPointer].mDefaultAddress);
		}

		mJumpSwitchEntryStackPointer = mJumpSwitchStack[mJumpSwitchStackPointer].mStackPointer;
	}

	void LocalAddressResolver::CloseSwitchBlock(void)
	{
		switch (mJumpSwitchStack[mJumpSwitchStackPointer].mType->GetId())
		{
		case TypeDescriptor::Id::Char:
		case TypeDescriptor::Id::Short:
		case TypeDescriptor::Id::Bool:
		case TypeDescriptor::Id::Int:
		case TypeDescriptor::Id::Float:
		case TypeDescriptor::Id::Actor:
			mCodeBuffer->AddOpecode(IntermediateLanguage::Remove);
			break;

		case TypeDescriptor::Id::Struct:
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::RemoveData, mJumpSwitchStack[mJumpSwitchStackPointer].mType->GetMemorySize());
			break;

		case TypeDescriptor::Id::Void:
		case TypeDescriptor::Id::Reference:
		case TypeDescriptor::Id::Array:
		case TypeDescriptor::Id::Module:
		case TypeDescriptor::Id::Nil:
		case TypeDescriptor::Id::Incomplete:
		default:
			MANA_BUG("Illegal type");
		}

		--mJumpSwitchStackPointer;
	}
}
