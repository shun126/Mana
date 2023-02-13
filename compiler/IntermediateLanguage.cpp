/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "IntermediateLanguage.h"
#include "CodeBuffer.h"
#include "../runner/common/Setup.h"
#include "../runner/common/FileFormat.h"

namespace mana
{
	extern const char* get_string(const void* program, const address_t address, const void* buffer)
	{
		const char* data = static_cast<const char*>(buffer);
		const address_t index = CodeBuffer::Raw<address_t>(program, address);
		return index >= 0 ? &data[index] : "";
	}

	extern const char* mana_get_instruction_text(const char* data, const void* program, const address_t address)
	{
#if defined(MANA_TARGET_WINDOWS)
#define SPRINTF sprintf_s
#else
#define SPRINTF std::sprintf
#endif
		static char text[256];

		const IntermediateLanguage opecode = reinterpret_cast<const IntermediateLanguage*>(program)[address];
		switch (opecode)
		{
			// thread
		case IntermediateLanguage::RESTART:				SPRINTF(text, "0x%08x:restart", address); break;
		case IntermediateLanguage::HALT:					SPRINTF(text, "0x%08x:halt", address); break;
		case IntermediateLanguage::YIELD:					SPRINTF(text, "0x%08x:yield", address); break;
		case IntermediateLanguage::NONPREEMPTIVE:			SPRINTF(text, "0x%08x:lock", address); break;
		case IntermediateLanguage::PREEMPTIVE:			SPRINTF(text, "0x%08x:unlock", address); break;

			// constant
		case IntermediateLanguage::PUSH_ZERO_INTEGER:		SPRINTF(text, "0x%08x:push zero (integer)", address); break;
		case IntermediateLanguage::PUSH_ZERO_FLOAT:		SPRINTF(text, "0x%08x:push zero (float)", address); break;
		case IntermediateLanguage::PUSH_CHAR:				SPRINTF(text, "0x%08x:push %d (char)", address, CodeBuffer::Raw<int8_t>(program, address + 1)); break;
		case IntermediateLanguage::PUSH_SHORT:			SPRINTF(text, "0x%08x:push %d (short)", address, CodeBuffer::Raw<int16_t>(program, address + 1)); break;
		case IntermediateLanguage::PUSH_INTEGER:			SPRINTF(text, "0x%08x:push %d (integer)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;
		case IntermediateLanguage::PUSH_SIZE:				SPRINTF(text, "0x%08x:push %d (size)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::PUSH_FLOAT:			SPRINTF(text, "0x%08x:push %f (float)", address, CodeBuffer::Raw<float>(program, address + 1)); break;
		case IntermediateLanguage::PUSH_STRING:			SPRINTF(text, "0x%08x:load effective address 0x%08x (data)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;
		case IntermediateLanguage::PUSH_PRIORITY:			SPRINTF(text, "0x%08x:push run-level", address); break;
		case IntermediateLanguage::PUSH_ACTOR:			SPRINTF(text, "0x%08x:push actor '%s'", address, get_string(program, address + 1, data)); break;
		case IntermediateLanguage::PUSH_SELF:				SPRINTF(text, "0x%08x:push self", address); break;
		case IntermediateLanguage::PUSH_SENDER:			SPRINTF(text, "0x%08x:push sender", address); break;

			// stack
		case IntermediateLanguage::ALLOCATE:				SPRINTF(text, "0x%08x:allocate %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::FREE:					SPRINTF(text, "0x%08x:release  %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::Duplicate:				SPRINTF(text, "0x%08x:duplicate", address); break;
		case IntermediateLanguage::DUPLICATE_DATA:		SPRINTF(text, "0x%08x:duplicate %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::REMOVE:				SPRINTF(text, "0x%08x:remove", address); break;
		case IntermediateLanguage::REMOVE_DATA:			SPRINTF(text, "0x%08x:remove %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LOAD_STATIC_ADDRESS:	SPRINTF(text, "0x%08x:load effective address 0x%08x (static)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LOAD_GLOBAL_ADDRESS:	SPRINTF(text, "0x%08x:load effective address 0x%08x (global)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LOAD_SELF_ADDRESS:		SPRINTF(text, "0x%08x:load effective address 0x%08x (self)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::LOAD_FRAME_ADDRESS:	SPRINTF(text, "0x%08x:load effective address 0x%08x (frame)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;

			// memory
		case IntermediateLanguage::LOAD_CHAR:				SPRINTF(text, "0x%08x:load (char)", address); break;
		case IntermediateLanguage::LOAD_SHORT:			SPRINTF(text, "0x%08x:load (short)", address); break;
		case IntermediateLanguage::LOAD_INTEGER:			SPRINTF(text, "0x%08x:load (int)", address); break;
		case IntermediateLanguage::LOAD_FLOAT:			SPRINTF(text, "0x%08x:load (float)", address); break;
		case IntermediateLanguage::LOAD_REFFRENCE:		SPRINTF(text, "0x%08x:load (reference)", address); break;
		case IntermediateLanguage::LOAD_DATA:				SPRINTF(text, "0x%08x:load (data) %d byte(s)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;
		case IntermediateLanguage::STORE_CHAR:			SPRINTF(text, "0x%08x:store (char)", address); break;
		case IntermediateLanguage::STORE_SHORT:			SPRINTF(text, "0x%08x:store (short)", address); break;
		case IntermediateLanguage::STORE_INTEGER:			SPRINTF(text, "0x%08x:store (int)", address); break;
		case IntermediateLanguage::STORE_FLOAT:			SPRINTF(text, "0x%08x:store (float)", address); break;
		case IntermediateLanguage::STORE_REFFRENCE:		SPRINTF(text, "0x%08x:store (reference)", address); break;
		case IntermediateLanguage::STORE_DATA:			SPRINTF(text, "0x%08x:store (data) %d byte(s)", address, CodeBuffer::Raw<int32_t>(program, address + 1)); break;

			// jump
		case IntermediateLanguage::BEQ:					SPRINTF(text, "0x%08x:beq 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::BNE:					SPRINTF(text, "0x%08x:bne 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::BRA:					SPRINTF(text, "0x%08x:bra 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::BSR:					SPRINTF(text, "0x%08x:bsr 0x%08x", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::CALL:					SPRINTF(text, "0x%08x:call '%s' %d %d %d", address, get_string(program, address + 1, data), CodeBuffer::Raw<int8_t>(program, address + 5), CodeBuffer::Raw<int8_t>(program, address + 6), CodeBuffer::Raw<int16_t>(program, address + 7)); break;
		case IntermediateLanguage::REQ:					SPRINTF(text, "0x%08x:req '%s'", address, get_string(program, address + 1, data)); break;
		case IntermediateLanguage::REQWS:					SPRINTF(text, "0x%08x:reqws '%s'", address, get_string(program, address + 1, data)); break;
		case IntermediateLanguage::REQWE:					SPRINTF(text, "0x%08x:reqwe '%s'", address, get_string(program, address + 1, data)); break;
		case IntermediateLanguage::DYNAMIC_REQ:			SPRINTF(text, "0x%08x:dynamic req '%s'", address, get_string(program, address + 1, data)); break;
		case IntermediateLanguage::DYNAMIC_REQWS:			SPRINTF(text, "0x%08x:dynamic reqws '%s'", address, get_string(program, address + 1, data)); break;
		case IntermediateLanguage::DYNAMIC_REQWE:			SPRINTF(text, "0x%08x:dynamic reqwe '%s'", address, get_string(program, address + 1, data)); break;
		case IntermediateLanguage::LOAD_RETURN_ADDRESS:	SPRINTF(text, "0x%08x:load ra", address); break;
		case IntermediateLanguage::SAVE_RETURN_ADDRESS:	SPRINTF(text, "0x%08x:save ra", address); break;
		case IntermediateLanguage::RETURN_FROM_FUNCTION:	SPRINTF(text, "0x%08x:return from function", address); break;
		case IntermediateLanguage::RETURN_FROM_ACTION:	SPRINTF(text, "0x%08x:return from action", address); break;
		case IntermediateLanguage::ROLLBACK:				SPRINTF(text, "0x%08x:rollback", address); break;

			// caluclation
		case IntermediateLanguage::ADD_INTEGER:			SPRINTF(text, "0x%08x:add (integer)", address); break;
		case IntermediateLanguage::DIV_INTEGER:			SPRINTF(text, "0x%08x:div (integer)", address); break;
		case IntermediateLanguage::MINUS_INTEGER:			SPRINTF(text, "0x%08x:neg (integer)", address); break;
		case IntermediateLanguage::MOD_INTEGER:			SPRINTF(text, "0x%08x:mod (integer)", address); break;
		case IntermediateLanguage::MUL_INTEGER:			SPRINTF(text, "0x%08x:mul (integer)", address); break;
		case IntermediateLanguage::SUB_INTEGER:			SPRINTF(text, "0x%08x:sub (integer)", address); break;

		case IntermediateLanguage::ADD_FLOAT:				SPRINTF(text, "0x%08x:add (float)", address); break;
		case IntermediateLanguage::DIV_FLOAT:				SPRINTF(text, "0x%08x:div (float)", address); break;
		case IntermediateLanguage::MINUS_FLOAT:			SPRINTF(text, "0x%08x:neg (float)", address); break;
		case IntermediateLanguage::MOD_FLOAT:				SPRINTF(text, "0x%08x:mod (float)", address); break;
		case IntermediateLanguage::MUL_FLOAT:				SPRINTF(text, "0x%08x:mul (float)", address); break;
		case IntermediateLanguage::SUB_FLOAT:				SPRINTF(text, "0x%08x:sub (float)", address); break;

		case IntermediateLanguage::AND:					SPRINTF(text, "0x%08x:and", address); break;
		case IntermediateLanguage::EOR:					SPRINTF(text, "0x%08x:eor", address); break;
		case IntermediateLanguage::LAND:					SPRINTF(text, "0x%08x:and", address); break;
		case IntermediateLanguage::LOR:					SPRINTF(text, "0x%08x:or", address); break;
		case IntermediateLanguage::LNOT:					SPRINTF(text, "0x%08x:!", address); break;
		case IntermediateLanguage::NOT:					SPRINTF(text, "0x%08x:~", address); break;
		case IntermediateLanguage::OR:					SPRINTF(text, "0x%08x:or", address); break;
		case IntermediateLanguage::SHL:					SPRINTF(text, "0x%08x:shift left", address); break;
		case IntermediateLanguage::SHR:					SPRINTF(text, "0x%08x:shift right", address); break;

		case IntermediateLanguage::INT2FLOAT:				SPRINTF(text, "0x%08x:cast (float)", address); break;
		case IntermediateLanguage::FLOAT2INT:				SPRINTF(text, "0x%08x:cast (int32_t)", address); break;

			// compare
		case IntermediateLanguage::COMPARE_EQ_INTEGER:	SPRINTF(text, "0x%08x:== (integer)", address); break;
		case IntermediateLanguage::COMPARE_NE_INTEGER:	SPRINTF(text, "0x%08x:!= (integer)", address); break;
		case IntermediateLanguage::COMPARE_GE_INTEGER:	SPRINTF(text, "0x%08x:>= (integer)", address); break;
		case IntermediateLanguage::COMPARE_GT_INTEGER:	SPRINTF(text, "0x%08x:> (integer)", address); break;
		case IntermediateLanguage::COMPARE_LE_INTEGER:	SPRINTF(text, "0x%08x:< (integer)", address); break;
		case IntermediateLanguage::COMPARE_LS_INTEGER:	SPRINTF(text, "0x%08x:< (integer)", address); break;

		case IntermediateLanguage::COMPARE_EQ_FLOAT:		SPRINTF(text, "0x%08x:== (float)", address); break;
		case IntermediateLanguage::COMPARE_NE_FLOAT:		SPRINTF(text, "0x%08x:!= (float)", address); break;
		case IntermediateLanguage::COMPARE_GE_FLOAT:		SPRINTF(text, "0x%08x:>= (float)", address); break;
		case IntermediateLanguage::COMPARE_GT_FLOAT:		SPRINTF(text, "0x%08x:> (float)", address); break;
		case IntermediateLanguage::COMPARE_LE_FLOAT:		SPRINTF(text, "0x%08x:<= (float)", address); break;
		case IntermediateLanguage::COMPARE_LS_FLOAT:		SPRINTF(text, "0x%08x:< (float)", address); break;

		case IntermediateLanguage::COMPARE_EQ_DATA:		SPRINTF(text, "0x%08x:== %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::COMPARE_NE_DATA:		SPRINTF(text, "0x%08x:!= %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::COMPARE_GE_DATA:		SPRINTF(text, "0x%08x:>= %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::COMPARE_GT_DATA:		SPRINTF(text, "0x%08x:> %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::COMPARE_LE_DATA:		SPRINTF(text, "0x%08x:<= %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;
		case IntermediateLanguage::COMPARE_LS_DATA:		SPRINTF(text, "0x%08x:< %d byte(s)", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;

			// inner function
		case IntermediateLanguage::PRINT:					SPRINTF(text, "0x%08x:print argc=%d", address, CodeBuffer::Raw<address_t>(program, address + 1)); break;

		case IntermediateLanguage::JOIN:					SPRINTF(text, "0x%08x:join", address); break;
		case IntermediateLanguage::COMPLY:				SPRINTF(text, "0x%08x:comply", address); break;
		case IntermediateLanguage::REFUSE:				SPRINTF(text, "0x%08x:refuse", address); break;

		default:
			SPRINTF(text, "invalid opecode");
			break;
		}
		return text;

#undef SPRINTF
	}
}
