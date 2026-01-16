/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "IntermediateLanguage.h"
#include "CodeBuffer.h"
#include "../runner/common/FileFormat.h"

namespace mana
{
	extern const char* GetString(const void* program, const address_t address, const void* buffer)
	{
		const char* data = static_cast<const char*>(buffer);
		const address_t index = CodeBuffer::Raw<address_t>(program, address);
		return &data[index];
	}

	extern const char* GetInstructionText(const char* data, const void* program, const address_t address)
	{
#if defined(MANA_TARGET_WINDOWS)
#define SPRINTF sprintf_s
#else
#define SPRINTF std::sprintf
#endif
		static char text[256];

		const IntermediateLanguage opecode = static_cast<const IntermediateLanguage*>(program)[address];
		switch (opecode)
		{
		// thread
		case IntermediateLanguage::Restart:				SPRINTF(text, "0x%08x:restart", address); break;
		case IntermediateLanguage::Halt:				SPRINTF(text, "0x%08x:halt", address); break;
		case IntermediateLanguage::Yield:				SPRINTF(text, "0x%08x:yield", address); break;
		case IntermediateLanguage::NonPreEmptive:		SPRINTF(text, "0x%08x:lock", address); break;
		case IntermediateLanguage::PreEmptive:			SPRINTF(text, "0x%08x:unlock", address); break;

		// constant
		case IntermediateLanguage::PushZeroInteger:		SPRINTF(text, "0x%08x:push zero (integer)", address); break;
		case IntermediateLanguage::PushZeroFloat:		SPRINTF(text, "0x%08x:push zero (float)", address); break;
		case IntermediateLanguage::PushChar:			SPRINTF(text, "0x%08x:push %d (char)", address, CodeBuffer::Raw<int8_t>(program, address + 1)); break;
		case IntermediateLanguage::PushShort:			SPRINTF(text, "0x%08x:push %d (short)", address, CodeBuffer::Raw<int16_t>(program, address + 1)); break;
		case IntermediateLanguage::PushInteger:			SPRINTF(text, "0x%08x:push %d (integer)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;
		case IntermediateLanguage::PushSize:			SPRINTF(text, "0x%08x:push %d (size)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::PushFloat:			SPRINTF(text, "0x%08x:push %f (float)", address, CodeBuffer::Raw<float>(program, address + 1)); break;
		case IntermediateLanguage::PushString:			SPRINTF(text, "0x%08x:load effective address 0x%08x (data)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;
		case IntermediateLanguage::PushPriority:		SPRINTF(text, "0x%08x:push run-level", address); break;
		case IntermediateLanguage::PushActor:			SPRINTF(text, "0x%08x:push actor '%s'", address, GetString(program, address + 1, data)); break;
		case IntermediateLanguage::PushSelf:			SPRINTF(text, "0x%08x:push self", address); break;
		case IntermediateLanguage::PushSender:			SPRINTF(text, "0x%08x:push sender", address); break;

		// stack
		case IntermediateLanguage::Allocate:			SPRINTF(text, "0x%08x:allocate %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::Free:				SPRINTF(text, "0x%08x:release  %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::Duplicate:			SPRINTF(text, "0x%08x:duplicate", address); break;
		case IntermediateLanguage::DuplicateData:		SPRINTF(text, "0x%08x:duplicate %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::Remove:				SPRINTF(text, "0x%08x:remove", address); break;
		case IntermediateLanguage::RemoveData:			SPRINTF(text, "0x%08x:remove %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LoadStaticAddress:	SPRINTF(text, "0x%08x:load effective address 0x%08x (static)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LoadGlobalAddress:	SPRINTF(text, "0x%08x:load effective address 0x%08x (global)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LoadSelfAddress:		SPRINTF(text, "0x%08x:load effective address 0x%08x (self)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LoadFrameAddress:	SPRINTF(text, "0x%08x:load effective address 0x%08x (frame)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;

		// memory
		case IntermediateLanguage::LoadChar:			SPRINTF(text, "0x%08x:load (char)", address); break;
		case IntermediateLanguage::LoadShort:			SPRINTF(text, "0x%08x:load (short)", address); break;
		case IntermediateLanguage::LoadInteger:			SPRINTF(text, "0x%08x:load (int)", address); break;
		case IntermediateLanguage::LoadFloat:			SPRINTF(text, "0x%08x:load (float)", address); break;
		case IntermediateLanguage::LoadReference:		SPRINTF(text, "0x%08x:load (reference)", address); break;
		case IntermediateLanguage::LoadData:			SPRINTF(text, "0x%08x:load (data) %d byte(s)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;
		case IntermediateLanguage::StoreChar:			SPRINTF(text, "0x%08x:store (char)", address); break;
		case IntermediateLanguage::StoreShort:			SPRINTF(text, "0x%08x:store (short)", address); break;
		case IntermediateLanguage::StoreInteger:		SPRINTF(text, "0x%08x:store (int)", address); break;
		case IntermediateLanguage::StoreFloat:			SPRINTF(text, "0x%08x:store (float)", address); break;
		case IntermediateLanguage::StoreReference:		SPRINTF(text, "0x%08x:store (reference)", address); break;
		case IntermediateLanguage::StoreData:			SPRINTF(text, "0x%08x:store (data) %d byte(s)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;

		// jump
		case IntermediateLanguage::BranchEqual:			SPRINTF(text, "0x%08x:beq 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::BranchNotEqual:		SPRINTF(text, "0x%08x:bne 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::Branch:				SPRINTF(text, "0x%08x:bra 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::BranchSubRoutine:	SPRINTF(text, "0x%08x:bsr 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::Call:				SPRINTF(text, "0x%08x:call '%s' %d %d %d", address, GetString(program, address + 1, data), CodeBuffer::Raw<int8_t>(program, address + 5), CodeBuffer::Raw<int8_t>(program, address + 6), CodeBuffer::Raw<int16_t>(program, address + 7)); break;
		case IntermediateLanguage::Request:				SPRINTF(text, "0x%08x:req '%s'", address, GetString(program, address + 1, data)); break;
		case IntermediateLanguage::RequestWaitStarting:	SPRINTF(text, "0x%08x:reqws '%s'", address, GetString(program, address + 1, data)); break;
		case IntermediateLanguage::RequestWaitEnded:	SPRINTF(text, "0x%08x:reqwe '%s'", address, GetString(program, address + 1, data)); break;
		case IntermediateLanguage::DynamicRequest:		SPRINTF(text, "0x%08x:dynamic req '%s'", address, GetString(program, address + 1, data)); break;
		case IntermediateLanguage::DynamicRequestWaitStarting:	SPRINTF(text, "0x%08x:dynamic reqws '%s'", address, GetString(program, address + 1, data)); break;
		case IntermediateLanguage::DynamicRequestWaitEnded:		SPRINTF(text, "0x%08x:dynamic reqwe '%s'", address, GetString(program, address + 1, data)); break;
		case IntermediateLanguage::LoadReturnAddress:	SPRINTF(text, "0x%08x:load ra", address); break;
		case IntermediateLanguage::SaveReturnAddress:	SPRINTF(text, "0x%08x:save ra", address); break;
		case IntermediateLanguage::ReturnFromFunction:	SPRINTF(text, "0x%08x:return from function", address); break;
		case IntermediateLanguage::ReturnFromAction:	SPRINTF(text, "0x%08x:return from action", address); break;
		case IntermediateLanguage::Rollback:			SPRINTF(text, "0x%08x:rollback", address); break;

		// caluclation
		case IntermediateLanguage::AddInteger:			SPRINTF(text, "0x%08x:add (integer)", address); break;
		case IntermediateLanguage::DivideInteger:		SPRINTF(text, "0x%08x:div (integer)", address); break;
		case IntermediateLanguage::MinusInteger:		SPRINTF(text, "0x%08x:neg (integer)", address); break;
		case IntermediateLanguage::ModInteger:			SPRINTF(text, "0x%08x:mod (integer)", address); break;
		case IntermediateLanguage::MultiInteger:		SPRINTF(text, "0x%08x:mul (integer)", address); break;
		case IntermediateLanguage::SubtractInteger:		SPRINTF(text, "0x%08x:sub (integer)", address); break;

		case IntermediateLanguage::AddFloat:			SPRINTF(text, "0x%08x:add (float)", address); break;
		case IntermediateLanguage::DivideFloat:			SPRINTF(text, "0x%08x:div (float)", address); break;
		case IntermediateLanguage::MinusFloat:			SPRINTF(text, "0x%08x:neg (float)", address); break;
		case IntermediateLanguage::ModFloat:			SPRINTF(text, "0x%08x:mod (float)", address); break;
		case IntermediateLanguage::MultiFloat:			SPRINTF(text, "0x%08x:mul (float)", address); break;
		case IntermediateLanguage::SubtractFloat:		SPRINTF(text, "0x%08x:sub (float)", address); break;

		case IntermediateLanguage::And:					SPRINTF(text, "0x%08x:and", address); break;
		case IntermediateLanguage::ExclusiveOr:			SPRINTF(text, "0x%08x:eor", address); break;
		case IntermediateLanguage::LogicalAnd:			SPRINTF(text, "0x%08x:and", address); break;
		case IntermediateLanguage::LogicalOr:			SPRINTF(text, "0x%08x:or", address); break;
		case IntermediateLanguage::LogicalNot:			SPRINTF(text, "0x%08x:!", address); break;
		case IntermediateLanguage::Not:					SPRINTF(text, "0x%08x:~", address); break;
		case IntermediateLanguage::Or:					SPRINTF(text, "0x%08x:or", address); break;
		case IntermediateLanguage::ShiftLeft:			SPRINTF(text, "0x%08x:shift left", address); break;
		case IntermediateLanguage::ShiftRight:			SPRINTF(text, "0x%08x:shift right", address); break;

		case IntermediateLanguage::CastIntegerToFloat:	SPRINTF(text, "0x%08x:cast (float)", address); break;
		case IntermediateLanguage::CastFloatToInteger:	SPRINTF(text, "0x%08x:cast (int32_t)", address); break;

		// compare
		case IntermediateLanguage::CompareEqualInteger:	SPRINTF(text, "0x%08x:== (integer)", address); break;
		case IntermediateLanguage::CompareNotEqualInteger:	SPRINTF(text, "0x%08x:!= (integer)", address); break;
		case IntermediateLanguage::CompareGreaterEqualInteger:	SPRINTF(text, "0x%08x:>= (integer)", address); break;
		case IntermediateLanguage::CompareGreaterInteger:	SPRINTF(text, "0x%08x:> (integer)", address); break;
		case IntermediateLanguage::CompareLessEqualInteger:	SPRINTF(text, "0x%08x:< (integer)", address); break;
		case IntermediateLanguage::CompareLessInteger:	SPRINTF(text, "0x%08x:< (integer)", address); break;

		case IntermediateLanguage::CompareEqualFloat:	SPRINTF(text, "0x%08x:== (float)", address); break;
		case IntermediateLanguage::CompareNotEqualFloat:	SPRINTF(text, "0x%08x:!= (float)", address); break;
		case IntermediateLanguage::CompareGreaterEqualFloat:	SPRINTF(text, "0x%08x:>= (float)", address); break;
		case IntermediateLanguage::CompareGreaterFloat:	SPRINTF(text, "0x%08x:> (float)", address); break;
		case IntermediateLanguage::CompareLessEqualFloat:	SPRINTF(text, "0x%08x:<= (float)", address); break;
		case IntermediateLanguage::CompareLessFloat:	SPRINTF(text, "0x%08x:< (float)", address); break;

		case IntermediateLanguage::CompareEqualData:	SPRINTF(text, "0x%08x:== %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::CompareNotEqualData:	SPRINTF(text, "0x%08x:!= %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::CompareGreaterEqualData:	SPRINTF(text, "0x%08x:>= %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::CompareGreaterData:	SPRINTF(text, "0x%08x:> %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::CompareLessEqualData:	SPRINTF(text, "0x%08x:<= %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::CompareLessData:		SPRINTF(text, "0x%08x:< %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;

		// inner function
		case IntermediateLanguage::Print:				SPRINTF(text, "0x%08x:print argc=%d", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;

		case IntermediateLanguage::Join:				SPRINTF(text, "0x%08x:join", address); break;
		case IntermediateLanguage::Comply:				SPRINTF(text, "0x%08x:comply", address); break;
		case IntermediateLanguage::Refuse:				SPRINTF(text, "0x%08x:refuse", address); break;

		default:
			SPRINTF(text, "invalid opecode");
			break;
		}
		return text;

#undef SPRINTF
	}
}
