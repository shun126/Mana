/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "Memory.h"
#include <cstdio>

namespace mana
{
	static constexpr const char* SIGNATURE = "MaNa";				//!< manaファイル署名
	static constexpr uint8_t MAJOR_VERSION = 0;						//!< manaファイルのメジャーバージョン番号
	static constexpr const uint8_t MINOR_VERSION = 13;				//!< manaファイルのマイナーバージョン番号
	static constexpr const uint8_t ACTOR_MAX_INTERRUPT_LEVEL = 32;	//!< 優先度数
	static constexpr const uint8_t FUNCTION_HASH_SIZE = 127;		//!< 外部関数ハッシュテーブルサイズ(素数が好ましい)
	static constexpr const uint8_t REGISTER_COUNT = 8;				//!< レジスタの数

	//! manaファイルヘッダー
	struct FileHeader
	{
		char mHeader[4];
		uint8_t mMajorVersion;
		uint8_t mMinorVersion;
		uint8_t mPad;
		enum Flag : uint8_t
		{
			Resource,	//!< フラグ：リソースセクションあり
			Is64bit,	//!< フラグ：64ビットでコンパイルされた
			BigEndian,	//!< フラグ：ビックエンディアンでコンパイルされた
			Compiled	//!< フラグ：コンパイル済み
		};
		uint8_t mFlag;
		uint32_t mNumberOfActors;
		uint32_t mSizeOfConstantPool;
		uint32_t mSizeOfInstructionPool;
		uint32_t mSizeOfStaticMemory;
		uint32_t mSizeOfGlobalMemory;
		uint32_t mRandomSeedNumber;
	};

	//! manaファイル内のアクター情報ヘッダー
	struct ActorInfoHeader
	{
		enum Flag : uint8_t
		{
			Phantom		//!< フラグ：ファントム
		};
		uint8_t mFlag;

		uint8_t mNumberOfInstances;		//!< インスタンス数（未使用）
		uint16_t mNumberOfActions;		//!< アクション数
		uint32_t mName;					//!< アクター名
		uint32_t mVariableSize;			//!< インスタンス変数サイズ
	};

	//! manaファイル内のアクション情報ヘッダー
	struct ActionInfoHeader
	{
		uint32_t mName;
		uint32_t mAddress;
	};

	//! byte code for virtual machine
	enum IntermediateLanguage : uint8_t
	{
		/* thread */
		MANA_IL_RESTART = 0,							/* 00 */
		MANA_IL_HALT,									/* 01 */
		MANA_IL_YIELD,									/* 02 */
		MANA_IL_NONPREEMPTIVE,							/* 03 */
		MANA_IL_PREEMPTIVE,								/* 04 */

		/* jump */
		MANA_IL_BEQ,									/* 05 */
		MANA_IL_BNE,									/* 06 */
		MANA_IL_BRA,									/* 07 */
		MANA_IL_BSR,									/* 08 */
		MANA_IL_CALL,									/* 09 */
		MANA_IL_REQ,									/* 0A */
		MANA_IL_REQWS,									/* 0B */
		MANA_IL_REQWE,									/* 0C */
		MANA_IL_JOIN,									/* 0D */
		MANA_IL_COMPLY,									/* 0E */
		MANA_IL_REFUSE,									/* 0F */
		MANA_IL_LOAD_RETURN_ADDRESS,					/* 10 */
		MANA_IL_SAVE_RETURN_ADDRESS,					/* 11 */
		MANA_IL_RETURN_FROM_FUNCTION,					/* 12 */
		MANA_IL_RETURN_FROM_ACTION,						/* 13 */
		MANA_IL_ROLLBACK,								/* 14 */

		/* constant */
		MANA_IL_PUSH_ZERO_INTEGER,						/* 15 */
		MANA_IL_PUSH_ZERO_FLOAT,						/* 16 */
		MANA_IL_PUSH_CHAR,								/* 17 */
		MANA_IL_PUSH_SHORT,								/* 18 */
		MANA_IL_PUSH_INTEGER,							/* 19 */
		MANA_IL_PUSH_FLOAT,								/* 1A */
		MANA_IL_PUSH_STRING,							/* 1B */
		MANA_IL_PUSH_PRIORITY,							/* 1C */
		MANA_IL_PUSH_ACTOR,								/* 1D */
		MANA_IL_PUSH_SELF,								/* 1E */
		MANA_IL_PUSH_SENDER,							/* 1F */

		/* stack */
		MANA_IL_ALLOCATE,								/* 20 */
		MANA_IL_FREE,									/* 21 */
		MANA_IL_DUPLICATE,								/* 22 */
		MANA_IL_DUPLICATE_DATA,							/* 23 */
		MANA_IL_REMOVE,									/* 24 */
		MANA_IL_REMOVE_DATA,							/* 25 */
		MANA_IL_LOAD_STATIC_ADDRESS,					/* 26 */
		MANA_IL_LOAD_GLOBAL_ADDRESS,					/* 27 */
		MANA_IL_LOAD_FRAME_ADDRESS,						/* 28 */
		MANA_IL_LOAD_SELF_ADDRESS,						/* 29 */

		/* memory operation */
		MANA_IL_LOAD_CHAR,								/* 2A */
		MANA_IL_LOAD_SHORT,								/* 2B */
		MANA_IL_LOAD_INTEGER,							/* 2C */
		MANA_IL_LOAD_FLOAT,								/* 2D */
		MANA_IL_LOAD_REFFRENCE,							/* 2E */
		MANA_IL_LOAD_DATA,								/* 2F */
		MANA_IL_STORE_CHAR,								/* 30 */
		MANA_IL_STORE_SHORT,							/* 31 */
		MANA_IL_STORE_INTEGER,							/* 32 */
		MANA_IL_STORE_FLOAT,							/* 33 */
		MANA_IL_STORE_REFFRENCE,						/* 34 */
		MANA_IL_STORE_DATA,								/* 35 */

		/* caluclation */
		MANA_IL_ADD_INTEGER,							/* 36 */
		MANA_IL_DIV_INTEGER,							/* 37 */
		MANA_IL_MINUS_INTEGER,							/* 38 */
		MANA_IL_MOD_INTEGER,							/* 39 */
		MANA_IL_MUL_INTEGER,							/* 3A */
		MANA_IL_SUB_INTEGER,							/* 3B */

		MANA_IL_ADD_FLOAT,								/* 3C */
		MANA_IL_DIV_FLOAT,								/* 3D */
		MANA_IL_MINUS_FLOAT,							/* 3E */
		MANA_IL_MOD_FLOAT,								/* 3F */
		MANA_IL_MUL_FLOAT,								/* 40 */
		MANA_IL_SUB_FLOAT,								/* 41 */

		MANA_IL_AND,									/* 42 */
		MANA_IL_EOR,									/* 43 */
		MANA_IL_LAND,									/* 44 */
		MANA_IL_LOR,									/* 45 */
		MANA_IL_LNOT,									/* 46 */
		MANA_IL_NOT,									/* 47 */
		MANA_IL_OR,										/* 48 */
		MANA_IL_SHL,									/* 49 */
		MANA_IL_SHR,									/* 4A */

		MANA_IL_INT2FLOAT,								/* 4B */
		MANA_IL_FLOAT2INT,								/* 4C */

		/* compare and branch */
		MANA_IL_COMPARE_EQ_INTEGER,						/* 4D */
		MANA_IL_COMPARE_NE_INTEGER,						/* 4E */
		MANA_IL_COMPARE_GE_INTEGER,						/* 4F */
		MANA_IL_COMPARE_GT_INTEGER,						/* 50 */
		MANA_IL_COMPARE_LE_INTEGER,						/* 51 */
		MANA_IL_COMPARE_LS_INTEGER,						/* 52 */

		MANA_IL_COMPARE_EQ_FLOAT,						/* 53 */
		MANA_IL_COMPARE_NE_FLOAT,						/* 54 */
		MANA_IL_COMPARE_GE_FLOAT,						/* 55 */
		MANA_IL_COMPARE_GT_FLOAT,						/* 56 */
		MANA_IL_COMPARE_LE_FLOAT,						/* 57 */
		MANA_IL_COMPARE_LS_FLOAT,						/* 58 */

		MANA_IL_COMPARE_EQ_DATA,						/* 59 */
		MANA_IL_COMPARE_NE_DATA,						/* 5A */
		MANA_IL_COMPARE_GE_DATA,						/* 5B */
		MANA_IL_COMPARE_GT_DATA,						/* 5C */
		MANA_IL_COMPARE_LE_DATA,						/* 5D */
		MANA_IL_COMPARE_LS_DATA,						/* 5E */

		/* utility */
		MANA_IL_PRINT,									/* 5F */

		/* under discussion */
		MANA_IL_DYNAMIC_REQ,							/* 60 */
		MANA_IL_DYNAMIC_REQWS,							/* 61 */
		MANA_IL_DYNAMIC_REQWE,							/* 62 */

		MANA_IL_MAX_SIZE
	};

	inline size_t GetInstructionSize(const void* codeBuffer, const size_t index)
	{
		// TODO:最終的には配列にしよう
		static const size_t SIZE_OF_POINTER = sizeof(uint32_t);

		const uint8_t* program = &static_cast<const uint8_t*>(codeBuffer)[index];
		switch(static_cast<IntermediateLanguage>(*program))
		{
		// thread
		case MANA_IL_RESTART:				return 1;
		case MANA_IL_HALT:					return 1;
		case MANA_IL_YIELD:					return 1;
		case MANA_IL_NONPREEMPTIVE:			return 1;
		case MANA_IL_PREEMPTIVE:			return 1;

		// constant
		case MANA_IL_PUSH_ZERO_INTEGER:		return 1;
		case MANA_IL_PUSH_ZERO_FLOAT:		return 1;
		case MANA_IL_PUSH_CHAR:				return 1 + sizeof(int8_t);
		case MANA_IL_PUSH_SHORT:			return 1 + sizeof(int16_t);
		case MANA_IL_PUSH_INTEGER:			return 1 + sizeof(int32_t);
		case MANA_IL_PUSH_FLOAT:			return 1 + sizeof(float);
		case MANA_IL_PUSH_STRING:			return 1 + SIZE_OF_POINTER;
		case MANA_IL_PUSH_PRIORITY:			return 1;
		case MANA_IL_PUSH_ACTOR:			return 1 + sizeof(int32_t);
		case MANA_IL_PUSH_SELF:				return 1;
		case MANA_IL_PUSH_SENDER:			return 1;

		// stack
		case MANA_IL_ALLOCATE:				return 1 + sizeof(int32_t);
		case MANA_IL_FREE:					return 1 + sizeof(int32_t);
		case MANA_IL_DUPLICATE:				return 1;
		case MANA_IL_DUPLICATE_DATA:		return 1 + sizeof(int32_t);
		case MANA_IL_REMOVE:				return 1;
		case MANA_IL_REMOVE_DATA:			return 1 + sizeof(int32_t);
		case MANA_IL_LOAD_STATIC_ADDRESS:	return 1 + sizeof(int32_t);
		case MANA_IL_LOAD_GLOBAL_ADDRESS:	return 1 + sizeof(int32_t);
		case MANA_IL_LOAD_FRAME_ADDRESS:	return 1 + sizeof(int32_t);
		case MANA_IL_LOAD_SELF_ADDRESS:		return 1 + sizeof(int32_t);

		// memory operation
		case MANA_IL_LOAD_CHAR:				return 1;
		case MANA_IL_LOAD_SHORT:			return 1;
		case MANA_IL_LOAD_INTEGER:			return 1;
		case MANA_IL_LOAD_FLOAT:			return 1;
		case MANA_IL_LOAD_REFFRENCE:		return 1;
		case MANA_IL_LOAD_DATA:				return 1 + SIZE_OF_POINTER;
		case MANA_IL_STORE_CHAR:			return 1;
		case MANA_IL_STORE_SHORT:			return 1;
		case MANA_IL_STORE_INTEGER:			return 1;
		case MANA_IL_STORE_FLOAT:			return 1;
		case MANA_IL_STORE_REFFRENCE:		return 1;
		case MANA_IL_STORE_DATA:			return 1 + SIZE_OF_POINTER;

		// jump
		case MANA_IL_BEQ:					return 1 + sizeof(int32_t);
		case MANA_IL_BNE:					return 1 + sizeof(int32_t);
		case MANA_IL_BRA:					return 1 + sizeof(int32_t);
		case MANA_IL_BSR:					return 1 + sizeof(int32_t);
		case MANA_IL_REQ:					return 1 + sizeof(int32_t);
		case MANA_IL_REQWS:					return 1 + sizeof(int32_t);
		case MANA_IL_REQWE:					return 1 + sizeof(int32_t);
		case MANA_IL_DYNAMIC_REQ:			return 1 + sizeof(int32_t);
		case MANA_IL_DYNAMIC_REQWS:			return 1 + sizeof(int32_t);
		case MANA_IL_DYNAMIC_REQWE:			return 1 + sizeof(int32_t);
		case MANA_IL_LOAD_RETURN_ADDRESS:	return 1;
		case MANA_IL_SAVE_RETURN_ADDRESS:	return 1;
		case MANA_IL_RETURN_FROM_FUNCTION:	return 1;
		case MANA_IL_RETURN_FROM_ACTION:	return 1;
		case MANA_IL_ROLLBACK:				return 1;
		case MANA_IL_CALL:					return 1 + sizeof(int32_t) + sizeof(int16_t) + sizeof(int16_t) + (program[6] * sizeof(int16_t));

		// caluclation
		case MANA_IL_ADD_INTEGER:			return 1;
		case MANA_IL_DIV_INTEGER:			return 1;
		case MANA_IL_MINUS_INTEGER:			return 1;
		case MANA_IL_MOD_INTEGER:			return 1;
		case MANA_IL_MUL_INTEGER:			return 1;
		case MANA_IL_SUB_INTEGER:			return 1;

		case MANA_IL_ADD_FLOAT:				return 1;
		case MANA_IL_DIV_FLOAT:				return 1;
		case MANA_IL_MINUS_FLOAT:			return 1;
		case MANA_IL_MOD_FLOAT:				return 1;
		case MANA_IL_MUL_FLOAT:				return 1;
		case MANA_IL_SUB_FLOAT:				return 1;

		case MANA_IL_AND:					return 1;
		case MANA_IL_EOR:					return 1;
		case MANA_IL_LAND:					return 1;
		case MANA_IL_LOR:					return 1;
		case MANA_IL_LNOT:					return 1;
		case MANA_IL_NOT:					return 1;
		case MANA_IL_OR:					return 1;
		case MANA_IL_SHL:					return 1;
		case MANA_IL_SHR:					return 1;

		case MANA_IL_INT2FLOAT:				return 1;
		case MANA_IL_FLOAT2INT:				return 1;

		// compare
		case MANA_IL_COMPARE_EQ_INTEGER:	return 1;
		case MANA_IL_COMPARE_NE_INTEGER:	return 1;
		case MANA_IL_COMPARE_GE_INTEGER:	return 1;
		case MANA_IL_COMPARE_GT_INTEGER:	return 1;
		case MANA_IL_COMPARE_LE_INTEGER:	return 1;
		case MANA_IL_COMPARE_LS_INTEGER:	return 1;

		case MANA_IL_COMPARE_EQ_FLOAT:		return 1;
		case MANA_IL_COMPARE_NE_FLOAT:		return 1;
		case MANA_IL_COMPARE_GE_FLOAT:		return 1;
		case MANA_IL_COMPARE_GT_FLOAT:		return 1;
		case MANA_IL_COMPARE_LE_FLOAT:		return 1;
		case MANA_IL_COMPARE_LS_FLOAT:		return 1;

		case MANA_IL_COMPARE_EQ_DATA:		return 1 + SIZE_OF_POINTER;
		case MANA_IL_COMPARE_NE_DATA:		return 1 + SIZE_OF_POINTER;
		case MANA_IL_COMPARE_GE_DATA:		return 1 + SIZE_OF_POINTER;
		case MANA_IL_COMPARE_GT_DATA:		return 1 + SIZE_OF_POINTER;
		case MANA_IL_COMPARE_LE_DATA:		return 1 + SIZE_OF_POINTER;
		case MANA_IL_COMPARE_LS_DATA:		return 1 + SIZE_OF_POINTER;

		// inner function
		case MANA_IL_PRINT:					return 1 + sizeof(int32_t);

		case MANA_IL_JOIN:					return 1;
		case MANA_IL_COMPLY:				return 1;
		case MANA_IL_REFUSE:				return 1;

		default:
			return 1;
		}
	}

	template <typename T> T get(const void* program, const int32_t address)
	{
		T value;
		uint8_t* pointer = reinterpret_cast<uint8_t*>(&value);
		const int8_t* instruction = static_cast<const int8_t*>(program);

		if (IsBigEndian())
		{
			for(size_t i = 0; i < sizeof(T); ++i)
				pointer[i] = instruction[address + i];
		}
		else
		{
			for (ssize_t i = sizeof(T) - 1; i >= 0; --i)
				pointer[i] = instruction[address + i];
		}

		return value;
	}

	inline const char* get_string(const void* program, const int32_t address, const void* buffer)
	{
		const char* data = static_cast<const char*>(buffer);
		const int32_t index = get<int32_t>(program, address);
		return index >= 0 ? &data[index] : "";
	}

	inline const char* mana_get_instruction_text(const char* data, const void* program, const int32_t address)
	{
#if defined(MANA_TARGET_WINDOWS)
#define SPRINTF sprintf_s
#else
#define SPRINTF std::sprintf
#endif
		static char text[256];


		const uint8_t opecode = ((const uint8_t*)program)[address];
		switch(opecode)
		{
		// thread
		case MANA_IL_RESTART:				SPRINTF(text, "%08x:restart", address); break;
		case MANA_IL_HALT:					SPRINTF(text, "%08x:halt", address); break;
		case MANA_IL_YIELD:					SPRINTF(text, "%08x:yield", address); break;
		case MANA_IL_NONPREEMPTIVE:			SPRINTF(text, "%08x:lock", address); break;
		case MANA_IL_PREEMPTIVE:			SPRINTF(text, "%08x:unlock", address); break;

		// constant
		case MANA_IL_PUSH_ZERO_INTEGER:		SPRINTF(text, "%08x:push zero (integer)", address); break;
		case MANA_IL_PUSH_ZERO_FLOAT:		SPRINTF(text, "%08x:push zero (float)", address); break;
		case MANA_IL_PUSH_CHAR:				SPRINTF(text, "%08x:push %d (char)", address, get<int8_t>(program, address + 1)); break;
		case MANA_IL_PUSH_SHORT:			SPRINTF(text, "%08x:push %d (short)", address, get<int16_t>(program, address + 1)); break;
		case MANA_IL_PUSH_INTEGER:			SPRINTF(text, "%08x:push %d (integer)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_PUSH_FLOAT:			SPRINTF(text, "%08x:push %f (float)", address, get<float>(program, address + 1)); break;
		case MANA_IL_PUSH_STRING:			SPRINTF(text, "%08x:load effective address %08x (data)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_PUSH_PRIORITY:			SPRINTF(text, "%08x:push run-level", address); break;
		case MANA_IL_PUSH_ACTOR:			SPRINTF(text, "%08x:push actor '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_PUSH_SELF:				SPRINTF(text, "%08x:push self", address); break;
		case MANA_IL_PUSH_SENDER:			SPRINTF(text, "%08x:push sender", address); break;

		// stack
		case MANA_IL_ALLOCATE:				SPRINTF(text, "%08x:allocate %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_FREE:					SPRINTF(text, "%08x:release  %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_DUPLICATE:				SPRINTF(text, "%08x:duplicate", address); break;
		case MANA_IL_DUPLICATE_DATA:		SPRINTF(text, "%08x:duplicate %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_REMOVE:				SPRINTF(text, "%08x:remove", address); break;
		case MANA_IL_REMOVE_DATA:			SPRINTF(text, "%08x:remove %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_LOAD_STATIC_ADDRESS:	SPRINTF(text, "%08x:load effective address %08x (static)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_LOAD_GLOBAL_ADDRESS:	SPRINTF(text, "%08x:load effective address %08x (global)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_LOAD_SELF_ADDRESS:		SPRINTF(text, "%08x:load effective address %08x (self)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_LOAD_FRAME_ADDRESS:	SPRINTF(text, "%08x:load effective address %08x (frame)", address, get<int32_t>(program, address + 1)); break;

		// memory
		case MANA_IL_LOAD_CHAR:				SPRINTF(text, "%08x:load (char)", address); break;
		case MANA_IL_LOAD_SHORT:			SPRINTF(text, "%08x:load (short)", address); break;
		case MANA_IL_LOAD_INTEGER:			SPRINTF(text, "%08x:load (int)", address); break;
		case MANA_IL_LOAD_FLOAT:			SPRINTF(text, "%08x:load (float)", address); break;
		case MANA_IL_LOAD_REFFRENCE:		SPRINTF(text, "%08x:load (reference)", address); break;
		case MANA_IL_LOAD_DATA:				SPRINTF(text, "%08x:load (data) %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_STORE_CHAR:			SPRINTF(text, "%08x:store (char)", address); break;
		case MANA_IL_STORE_SHORT:			SPRINTF(text, "%08x:store (short)", address); break;
		case MANA_IL_STORE_INTEGER:			SPRINTF(text, "%08x:store (int)", address); break;
		case MANA_IL_STORE_FLOAT:			SPRINTF(text, "%08x:store (float)", address); break;
		case MANA_IL_STORE_REFFRENCE:		SPRINTF(text, "%08x:store (reference)", address); break;
		case MANA_IL_STORE_DATA:			SPRINTF(text, "%08x:store (data) %d byte(s)", address, get<int32_t>(program, address + 1)); break;

		// jump
		case MANA_IL_BEQ:					SPRINTF(text, "%08x:beq %08x", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_BNE:					SPRINTF(text, "%08x:bne %08x", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_BRA:					SPRINTF(text, "%08x:bra %08x", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_BSR:					SPRINTF(text, "%08x:bsr %08x", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_CALL:					SPRINTF(text, "%08x:call '%s' %d %d %d", address, get_string(program, address + 1, data), get<int8_t>(program, address + 5), get<int8_t>(program, address + 6), get<int16_t>(program, address + 7)); break;
		case MANA_IL_REQ:					SPRINTF(text, "%08x:req '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_REQWS:					SPRINTF(text, "%08x:reqws '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_REQWE:					SPRINTF(text, "%08x:reqwe '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_DYNAMIC_REQ:			SPRINTF(text, "%08x:dynamic req '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_DYNAMIC_REQWS:			SPRINTF(text, "%08x:dynamic reqws '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_DYNAMIC_REQWE:			SPRINTF(text, "%08x:dynamic reqwe '%s'", address, get_string(program, address + 1, data)); break;
		case MANA_IL_LOAD_RETURN_ADDRESS:	SPRINTF(text, "%08x:load ra", address); break;
		case MANA_IL_SAVE_RETURN_ADDRESS:	SPRINTF(text, "%08x:save ra", address); break;
		case MANA_IL_RETURN_FROM_FUNCTION:	SPRINTF(text, "%08x:return from function", address); break;
		case MANA_IL_RETURN_FROM_ACTION:	SPRINTF(text, "%08x:return from action", address); break;
		case MANA_IL_ROLLBACK:				SPRINTF(text, "%08x:rollback", address); break;

		// caluclation
		case MANA_IL_ADD_INTEGER:			SPRINTF(text, "%08x:add (integer)", address); break;
		case MANA_IL_DIV_INTEGER:			SPRINTF(text, "%08x:div (integer)", address); break;
		case MANA_IL_MINUS_INTEGER:			SPRINTF(text, "%08x:neg (integer)", address); break;
		case MANA_IL_MOD_INTEGER:			SPRINTF(text, "%08x:mod (integer)", address); break;
		case MANA_IL_MUL_INTEGER:			SPRINTF(text, "%08x:mul (integer)", address); break;
		case MANA_IL_SUB_INTEGER:			SPRINTF(text, "%08x:sub (integer)", address); break;

		case MANA_IL_ADD_FLOAT:				SPRINTF(text, "%08x:add (float)", address); break;
		case MANA_IL_DIV_FLOAT:				SPRINTF(text, "%08x:div (float)", address); break;
		case MANA_IL_MINUS_FLOAT:			SPRINTF(text, "%08x:neg (float)", address); break;
		case MANA_IL_MOD_FLOAT:				SPRINTF(text, "%08x:mod (float)", address); break;
		case MANA_IL_MUL_FLOAT:				SPRINTF(text, "%08x:mul (float)", address); break;
		case MANA_IL_SUB_FLOAT:				SPRINTF(text, "%08x:sub (float)", address); break;

		case MANA_IL_AND:					SPRINTF(text, "%08x:and", address); break;
		case MANA_IL_EOR:					SPRINTF(text, "%08x:eor", address); break;
		case MANA_IL_LAND:					SPRINTF(text, "%08x:and", address); break;
		case MANA_IL_LOR:					SPRINTF(text, "%08x:or", address); break;
		case MANA_IL_LNOT:					SPRINTF(text, "%08x:!", address); break;
		case MANA_IL_NOT:					SPRINTF(text, "%08x:~", address); break;
		case MANA_IL_OR:					SPRINTF(text, "%08x:or", address); break;
		case MANA_IL_SHL:					SPRINTF(text, "%08x:shift left", address); break;
		case MANA_IL_SHR:					SPRINTF(text, "%08x:shift right", address); break;

		case MANA_IL_INT2FLOAT:				SPRINTF(text, "%08x:cast (float)", address); break;
		case MANA_IL_FLOAT2INT:				SPRINTF(text, "%08x:cast (int32_t)", address); break;

		// compare
		case MANA_IL_COMPARE_EQ_INTEGER:	SPRINTF(text, "%08x:== (integer)", address); break;
		case MANA_IL_COMPARE_NE_INTEGER:	SPRINTF(text, "%08x:!= (integer)", address); break;
		case MANA_IL_COMPARE_GE_INTEGER:	SPRINTF(text, "%08x:>= (integer)", address); break;
		case MANA_IL_COMPARE_GT_INTEGER:	SPRINTF(text, "%08x:> (integer)", address); break;
		case MANA_IL_COMPARE_LE_INTEGER:	SPRINTF(text, "%08x:< (integer)", address); break;
		case MANA_IL_COMPARE_LS_INTEGER:	SPRINTF(text, "%08x:< (integer)", address); break;

		case MANA_IL_COMPARE_EQ_FLOAT:		SPRINTF(text, "%08x:== (float)", address); break;
		case MANA_IL_COMPARE_NE_FLOAT:		SPRINTF(text, "%08x:!= (float)", address); break;
		case MANA_IL_COMPARE_GE_FLOAT:		SPRINTF(text, "%08x:>= (float)", address); break;
		case MANA_IL_COMPARE_GT_FLOAT:		SPRINTF(text, "%08x:> (float)", address); break;
		case MANA_IL_COMPARE_LE_FLOAT:		SPRINTF(text, "%08x:<= (float)", address); break;
		case MANA_IL_COMPARE_LS_FLOAT:		SPRINTF(text, "%08x:< (float)", address); break;

		case MANA_IL_COMPARE_EQ_DATA:		SPRINTF(text, "%08x:== %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_NE_DATA:		SPRINTF(text, "%08x:!= %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_GE_DATA:		SPRINTF(text, "%08x:>= %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_GT_DATA:		SPRINTF(text, "%08x:> %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_LE_DATA:		SPRINTF(text, "%08x:<= %d byte(s)", address, get<int32_t>(program, address + 1)); break;
		case MANA_IL_COMPARE_LS_DATA:		SPRINTF(text, "%08x:< %d byte(s)", address, get<int32_t>(program, address + 1)); break;

		// inner function
		case MANA_IL_PRINT:					SPRINTF(text, "%08x:print argc=%d", address, get<int32_t>(program, address + 1)); break;

		case MANA_IL_JOIN:					SPRINTF(text, "%08x:join", address); break;
		case MANA_IL_COMPLY:				SPRINTF(text, "%08x:comply", address); break;
		case MANA_IL_REFUSE:				SPRINTF(text, "%08x:refuse", address); break;

		default:
			SPRINTF(text, "invalid opecode");
			break;
		}
		return text;

#undef SPRINTF
	}
}
