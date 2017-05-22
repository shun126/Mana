/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "IntermediateLanguage.h"
#include "../runner/common/Setup.h"
#include "../runner/common/FileFormat.h"

namespace mana
{
	extern const char* get_string(const void* program, const int32_t address, const void* buffer)
	{
		const char* data = static_cast<const char*>(buffer);
		const size_t index = get<size_t>(program, address);
		return index >= 0 ? &data[index] : "";
	}

	extern const char* mana_get_instruction_text(const char* data, const void* program, const int32_t address)
	{
#if defined(MANA_TARGET_WINDOWS)
#define SPRINTF sprintf_s
#else
#define SPRINTF std::sprintf
#endif
		static char text[256];

		const uint8_t opecode = ((const uint8_t*)program)[address];
		switch (opecode)
		{
			// thread
		case MANA_IL_RESTART:				SPRINTF(text, "0x%08x:restart", address); break;
		case MANA_IL_HALT:					SPRINTF(text, "0x%08x:halt", address); break;
		case MANA_IL_YIELD:					SPRINTF(text, "0x%08x:yield", address); break;
		case MANA_IL_NONPREEMPTIVE:			SPRINTF(text, "0x%08x:lock", address); break;
		case MANA_IL_PREEMPTIVE:			SPRINTF(text, "0x%08x:unlock", address); break;

			// constant
		case MANA_IL_PUSH_ZERO_INTEGER:		SPRINTF(text, "0x%08x:push zero (integer)", address); break;
		case MANA_IL_PUSH_ZERO_FLOAT:		SPRINTF(text, "0x%08x:push zero (float)", address); break;
		case MANA_IL_PUSH_CHAR:				SPRINTF(text, "0x%08x:push %d (char)", address, get<int8_t>(program, address + 1)); break;
		case MANA_IL_PUSH_SHORT:			SPRINTF(text, "0x%08x:push %d (short)", address, get<int16_t>(program, address + 1)); break;
		case MANA_IL_PUSH_INTEGER:			SPRINTF(text, "0x%08x:push %d (integer)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_PUSH_SIZE:				SPRINTF(text, "0x%08x:push %d (size)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_PUSH_FLOAT:			SPRINTF(text, "0x%08x:push %f (float)", address, get<float>(program, address + 1)); break;
		case MANA_IL_PUSH_STRING:			SPRINTF(text, "0x%08x:load effective address 0x%08x (data)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_PUSH_PRIORITY:			SPRINTF(text, "0x%08x:push run-level", address); break;
		case MANA_IL_PUSH_ACTOR:			SPRINTF(text, "0x%08x:push actor '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_PUSH_SELF:				SPRINTF(text, "0x%08x:push self", address); break;
		case MANA_IL_PUSH_SENDER:			SPRINTF(text, "0x%08x:push sender", address); break;

			// stack
		case MANA_IL_ALLOCATE:				SPRINTF(text, "0x%08x:allocate %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_FREE:					SPRINTF(text, "0x%08x:release  %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_DUPLICATE:				SPRINTF(text, "0x%08x:duplicate", address); break;
		case MANA_IL_DUPLICATE_DATA:		SPRINTF(text, "0x%08x:duplicate %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_REMOVE:				SPRINTF(text, "0x%08x:remove", address); break;
		case MANA_IL_REMOVE_DATA:			SPRINTF(text, "0x%08x:remove %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_LOAD_STATIC_ADDRESS:	SPRINTF(text, "0x%08x:load effective address 0x%08x (static)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_LOAD_GLOBAL_ADDRESS:	SPRINTF(text, "0x%08x:load effective address 0x%08x (global)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_LOAD_SELF_ADDRESS:		SPRINTF(text, "0x%08x:load effective address 0x%08x (self)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_LOAD_FRAME_ADDRESS:	SPRINTF(text, "0x%08x:load effective address 0x%08x (frame)", address, get<size_t>(program, address + 1)); break;

			// memory
		case MANA_IL_LOAD_CHAR:				SPRINTF(text, "0x%08x:load (char)", address); break;
		case MANA_IL_LOAD_SHORT:			SPRINTF(text, "0x%08x:load (short)", address); break;
		case MANA_IL_LOAD_INTEGER:			SPRINTF(text, "0x%08x:load (int)", address); break;
		case MANA_IL_LOAD_FLOAT:			SPRINTF(text, "0x%08x:load (float)", address); break;
		case MANA_IL_LOAD_REFFRENCE:		SPRINTF(text, "0x%08x:load (reference)", address); break;
		case MANA_IL_LOAD_DATA:				SPRINTF(text, "0x%08x:load (data) %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_STORE_CHAR:			SPRINTF(text, "0x%08x:store (char)", address); break;
		case MANA_IL_STORE_SHORT:			SPRINTF(text, "0x%08x:store (short)", address); break;
		case MANA_IL_STORE_INTEGER:			SPRINTF(text, "0x%08x:store (int)", address); break;
		case MANA_IL_STORE_FLOAT:			SPRINTF(text, "0x%08x:store (float)", address); break;
		case MANA_IL_STORE_REFFRENCE:		SPRINTF(text, "0x%08x:store (reference)", address); break;
		case MANA_IL_STORE_DATA:			SPRINTF(text, "0x%08x:store (data) %d byte(s)", address, get<int32_t>(program, address + 1)); break;

			// jump
		case MANA_IL_BEQ:					SPRINTF(text, "0x%08x:beq 0x%08x", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_BNE:					SPRINTF(text, "0x%08x:bne 0x%08x", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_BRA:					SPRINTF(text, "0x%08x:bra 0x%08x", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_BSR:					SPRINTF(text, "0x%08x:bsr 0x%08x", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_CALL:					SPRINTF(text, "0x%08x:call '%s' %d %d %d", address, get_string(program, address + 1, data), get<int8_t>(program, address + 5), get<int8_t>(program, address + 6), get<int16_t>(program, address + 7)); break;
		case MANA_IL_REQ:					SPRINTF(text, "0x%08x:req '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_REQWS:					SPRINTF(text, "0x%08x:reqws '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_REQWE:					SPRINTF(text, "0x%08x:reqwe '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_DYNAMIC_REQ:			SPRINTF(text, "0x%08x:dynamic req '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_DYNAMIC_REQWS:			SPRINTF(text, "0x%08x:dynamic reqws '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_DYNAMIC_REQWE:			SPRINTF(text, "0x%08x:dynamic reqwe '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_LOAD_RETURN_ADDRESS:	SPRINTF(text, "0x%08x:load ra", address); break;
		case MANA_IL_SAVE_RETURN_ADDRESS:	SPRINTF(text, "0x%08x:save ra", address); break;
		case MANA_IL_RETURN_FROM_FUNCTION:	SPRINTF(text, "0x%08x:return from function", address); break;
		case MANA_IL_RETURN_FROM_ACTION:	SPRINTF(text, "0x%08x:return from action", address); break;
		case MANA_IL_ROLLBACK:				SPRINTF(text, "0x%08x:rollback", address); break;

			// caluclation
		case MANA_IL_ADD_INTEGER:			SPRINTF(text, "0x%08x:add (integer)", address); break;
		case MANA_IL_DIV_INTEGER:			SPRINTF(text, "0x%08x:div (integer)", address); break;
		case MANA_IL_MINUS_INTEGER:			SPRINTF(text, "0x%08x:neg (integer)", address); break;
		case MANA_IL_MOD_INTEGER:			SPRINTF(text, "0x%08x:mod (integer)", address); break;
		case MANA_IL_MUL_INTEGER:			SPRINTF(text, "0x%08x:mul (integer)", address); break;
		case MANA_IL_SUB_INTEGER:			SPRINTF(text, "0x%08x:sub (integer)", address); break;

		case MANA_IL_ADD_FLOAT:				SPRINTF(text, "0x%08x:add (float)", address); break;
		case MANA_IL_DIV_FLOAT:				SPRINTF(text, "0x%08x:div (float)", address); break;
		case MANA_IL_MINUS_FLOAT:			SPRINTF(text, "0x%08x:neg (float)", address); break;
		case MANA_IL_MOD_FLOAT:				SPRINTF(text, "0x%08x:mod (float)", address); break;
		case MANA_IL_MUL_FLOAT:				SPRINTF(text, "0x%08x:mul (float)", address); break;
		case MANA_IL_SUB_FLOAT:				SPRINTF(text, "0x%08x:sub (float)", address); break;

		case MANA_IL_AND:					SPRINTF(text, "0x%08x:and", address); break;
		case MANA_IL_EOR:					SPRINTF(text, "0x%08x:eor", address); break;
		case MANA_IL_LAND:					SPRINTF(text, "0x%08x:and", address); break;
		case MANA_IL_LOR:					SPRINTF(text, "0x%08x:or", address); break;
		case MANA_IL_LNOT:					SPRINTF(text, "0x%08x:!", address); break;
		case MANA_IL_NOT:					SPRINTF(text, "0x%08x:~", address); break;
		case MANA_IL_OR:					SPRINTF(text, "0x%08x:or", address); break;
		case MANA_IL_SHL:					SPRINTF(text, "0x%08x:shift left", address); break;
		case MANA_IL_SHR:					SPRINTF(text, "0x%08x:shift right", address); break;

		case MANA_IL_INT2FLOAT:				SPRINTF(text, "0x%08x:cast (float)", address); break;
		case MANA_IL_FLOAT2INT:				SPRINTF(text, "0x%08x:cast (int32_t)", address); break;

			// compare
		case MANA_IL_COMPARE_EQ_INTEGER:	SPRINTF(text, "0x%08x:== (integer)", address); break;
		case MANA_IL_COMPARE_NE_INTEGER:	SPRINTF(text, "0x%08x:!= (integer)", address); break;
		case MANA_IL_COMPARE_GE_INTEGER:	SPRINTF(text, "0x%08x:>= (integer)", address); break;
		case MANA_IL_COMPARE_GT_INTEGER:	SPRINTF(text, "0x%08x:> (integer)", address); break;
		case MANA_IL_COMPARE_LE_INTEGER:	SPRINTF(text, "0x%08x:< (integer)", address); break;
		case MANA_IL_COMPARE_LS_INTEGER:	SPRINTF(text, "0x%08x:< (integer)", address); break;

		case MANA_IL_COMPARE_EQ_FLOAT:		SPRINTF(text, "0x%08x:== (float)", address); break;
		case MANA_IL_COMPARE_NE_FLOAT:		SPRINTF(text, "0x%08x:!= (float)", address); break;
		case MANA_IL_COMPARE_GE_FLOAT:		SPRINTF(text, "0x%08x:>= (float)", address); break;
		case MANA_IL_COMPARE_GT_FLOAT:		SPRINTF(text, "0x%08x:> (float)", address); break;
		case MANA_IL_COMPARE_LE_FLOAT:		SPRINTF(text, "0x%08x:<= (float)", address); break;
		case MANA_IL_COMPARE_LS_FLOAT:		SPRINTF(text, "0x%08x:< (float)", address); break;

		case MANA_IL_COMPARE_EQ_DATA:		SPRINTF(text, "0x%08x:== %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_NE_DATA:		SPRINTF(text, "0x%08x:!= %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_GE_DATA:		SPRINTF(text, "0x%08x:>= %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_GT_DATA:		SPRINTF(text, "0x%08x:> %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_LE_DATA:		SPRINTF(text, "0x%08x:<= %d byte(s)", address, get<size_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_LS_DATA:		SPRINTF(text, "0x%08x:< %d byte(s)", address, get<size_t>(program, address + 1)); break;

			// inner function
		case MANA_IL_PRINT:					SPRINTF(text, "0x%08x:print argc=%d", address, get<size_t>(program, address + 1)); break;

		case MANA_IL_JOIN:					SPRINTF(text, "0x%08x:join", address); break;
		case MANA_IL_COMPLY:				SPRINTF(text, "0x%08x:comply", address); break;
		case MANA_IL_REFUSE:				SPRINTF(text, "0x%08x:refuse", address); break;

		default:
			SPRINTF(text, "invalid opecode");
			break;
		}
		return text;

#undef SPRINTF
	}
}
