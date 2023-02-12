/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "Memory.h"

#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
#include "Assert.h"
#endif

#include <array>
#include <cstdio>
#include <string_view>

namespace mana
{
	static constexpr const char* SIGNATURE = "MaNa";				//!< manaファイル署名
	static constexpr uint8_t MAJOR_VERSION = 0;						//!< manaファイルのメジャーバージョン番号
	static constexpr const uint8_t MINOR_VERSION = 13;				//!< manaファイルのマイナーバージョン番号

	/*!
	manaファイルヘッダー
	*/
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

	/*!
	manaファイル内のアクター情報ヘッダー
	*/
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

	/*!
	manaファイル内のアクション情報ヘッダー
	*/
	struct ActionInfoHeader
	{
		uint32_t mName;
		uint32_t mAddress;
	};

	/*!
	byte code for virtual machine
	*/
	enum class IntermediateLanguage : uint8_t
	{
		/* thread */
		RESTART = 0,							/* 00 */
		HALT,									/* 01 */
		YIELD,									/* 02 */
		NONPREEMPTIVE,							/* 03 */
		PREEMPTIVE,								/* 04 */

		/* jump */
		BEQ,									/* 05 */
		BNE,									/* 06 */
		BRA,									/* 07 */
		BSR,									/* 08 */
		CALL,									/* 09 */
		REQ,									/* 0A */
		REQWS,									/* 0B */
		REQWE,									/* 0C */
		JOIN,									/* 0D */
		COMPLY,									/* 0E */
		REFUSE,									/* 0F */
		LOAD_RETURN_ADDRESS,					/* 10 */
		SAVE_RETURN_ADDRESS,					/* 11 */
		RETURN_FROM_FUNCTION,					/* 12 */
		RETURN_FROM_ACTION,						/* 13 */
		ROLLBACK,								/* 14 */

		/* constant */
		PUSH_ZERO_INTEGER,						/* 15 */
		PUSH_ZERO_FLOAT,						/* 16 */
		PUSH_CHAR,								/* 17 */
		PUSH_SHORT,								/* 18 */
		PUSH_INTEGER,							/* 19 */
		PUSH_SIZE,								/* 19 */
		PUSH_FLOAT,								/* 1A */
		PUSH_STRING,							/* 1B */
		PUSH_PRIORITY,							/* 1C */
		PUSH_ACTOR,								/* 1D */
		PUSH_SELF,								/* 1E */
		PUSH_SENDER,							/* 1F */

		/* stack */
		ALLOCATE,								/* 20 */
		FREE,									/* 21 */
		Duplicate,								/* 22 */
		DUPLICATE_DATA,							/* 23 */
		REMOVE,									/* 24 */
		REMOVE_DATA,							/* 25 */
		LOAD_STATIC_ADDRESS,					/* 26 */
		LOAD_GLOBAL_ADDRESS,					/* 27 */
		LOAD_FRAME_ADDRESS,						/* 28 */
		LOAD_SELF_ADDRESS,						/* 29 */

		/* memory operation */
		LOAD_CHAR,								/* 2A */
		LOAD_SHORT,								/* 2B */
		LOAD_INTEGER,							/* 2C */
		LOAD_FLOAT,								/* 2D */
		LOAD_REFFRENCE,							/* 2E */
		LOAD_DATA,								/* 2F */
		STORE_CHAR,								/* 30 */
		STORE_SHORT,							/* 31 */
		STORE_INTEGER,							/* 32 */
		STORE_FLOAT,							/* 33 */
		STORE_REFFRENCE,						/* 34 */
		STORE_DATA,								/* 35 */

		/* caluclation */
		ADD_INTEGER,							/* 36 */
		DIV_INTEGER,							/* 37 */
		MINUS_INTEGER,							/* 38 */
		MOD_INTEGER,							/* 39 */
		MUL_INTEGER,							/* 3A */
		SUB_INTEGER,							/* 3B */

		ADD_FLOAT,								/* 3C */
		DIV_FLOAT,								/* 3D */
		MINUS_FLOAT,							/* 3E */
		MOD_FLOAT,								/* 3F */
		MUL_FLOAT,								/* 40 */
		SUB_FLOAT,								/* 41 */

		AND,									/* 42 */
		EOR,									/* 43 */
		LAND,									/* 44 */
		LOR,									/* 45 */
		LNOT,									/* 46 */
		NOT,									/* 47 */
		OR,										/* 48 */
		SHL,									/* 49 */
		SHR,									/* 4A */

		INT2FLOAT,								/* 4B */
		FLOAT2INT,								/* 4C */

		/* compare and branch */
		COMPARE_EQ_INTEGER,						/* 4D */
		COMPARE_NE_INTEGER,						/* 4E */
		COMPARE_GE_INTEGER,						/* 4F */
		COMPARE_GT_INTEGER,						/* 50 */
		COMPARE_LE_INTEGER,						/* 51 */
		COMPARE_LS_INTEGER,						/* 52 */

		COMPARE_EQ_FLOAT,						/* 53 */
		COMPARE_NE_FLOAT,						/* 54 */
		COMPARE_GE_FLOAT,						/* 55 */
		COMPARE_GT_FLOAT,						/* 56 */
		COMPARE_LE_FLOAT,						/* 57 */
		COMPARE_LS_FLOAT,						/* 58 */

		COMPARE_EQ_DATA,						/* 59 */
		COMPARE_NE_DATA,						/* 5A */
		COMPARE_GE_DATA,						/* 5B */
		COMPARE_GT_DATA,						/* 5C */
		COMPARE_LE_DATA,						/* 5D */
		COMPARE_LS_DATA,						/* 5E */

		/* utility */
		PRINT,									/* 5F */

		/* under discussion */
		DYNAMIC_REQ,							/* 60 */
		DYNAMIC_REQWS,							/* 61 */
		DYNAMIC_REQWE,							/* 62 */
	};

	static constexpr address_t IntermediateLanguageSize = static_cast<address_t>(IntermediateLanguage::DYNAMIC_REQWE) + 1;

	struct IntermediateLanguageProperty final
	{
		uint8_t mSize;
#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
		IntermediateLanguage mIntermediateLanguage;
#endif
	};

#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
#define MANA_FILE_FORMAT_DEBUG_PARAMETER(code, size)	(1 + (size)), (IntermediateLanguage::code)
#else
#define MANA_FILE_FORMAT_DEBUG_PARAMETER(code, size)	(1 + (size))
#endif

	inline const IntermediateLanguageProperty& GetIntermediateLanguageProperty(const IntermediateLanguage code)
	{
		static const std::array<IntermediateLanguageProperty, static_cast<address_t>(IntermediateLanguageSize)> properties =
		{
			{
				// thread
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(RESTART, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(HALT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(YIELD, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(NONPREEMPTIVE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PREEMPTIVE, 0) },

				// jump
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(BEQ, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(BNE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(BRA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(BSR, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CALL, sizeof(address_t) + sizeof(int16_t) + sizeof(int16_t) /* + (program[6] * sizeof(int16_t))*/)}, // 引数によってサイズが変わる
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(REQ, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(REQWS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(REQWE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(JOIN, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPLY, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(REFUSE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_RETURN_ADDRESS, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SAVE_RETURN_ADDRESS, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(RETURN_FROM_FUNCTION, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(RETURN_FROM_ACTION, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ROLLBACK, 0) },

				// constant
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_ZERO_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_ZERO_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_CHAR, sizeof(int8_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_SHORT, sizeof(int16_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_INTEGER, sizeof(int32_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_SIZE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_FLOAT, sizeof(float)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_STRING, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_PRIORITY, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_ACTOR, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_SELF, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PUSH_SENDER, 0) },

				// stack
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ALLOCATE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(FREE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Duplicate, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DUPLICATE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(REMOVE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(REMOVE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_STATIC_ADDRESS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_GLOBAL_ADDRESS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_FRAME_ADDRESS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_SELF_ADDRESS, sizeof(address_t)) },

				// memory operation
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_CHAR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_SHORT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_REFFRENCE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOAD_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(STORE_CHAR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(STORE_SHORT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(STORE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(STORE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(STORE_REFFRENCE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(STORE_DATA, sizeof(address_t)) },

				// caluclation
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ADD_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DIV_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MINUS_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MOD_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MUL_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SUB_INTEGER, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ADD_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DIV_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MINUS_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MOD_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MUL_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SUB_FLOAT, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(AND, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(EOR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LAND, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LOR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LNOT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(NOT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(OR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SHL, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SHR, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(INT2FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(FLOAT2INT, 0) },

				// compare and branch
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_EQ_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_NE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_GE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_GT_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_LE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_LS_INTEGER, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_EQ_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_NE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_GE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_GT_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_LE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_LS_FLOAT, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_EQ_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_NE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_GE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_GT_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_LE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(COMPARE_LS_DATA, sizeof(address_t)) },

				// utility
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PRINT, sizeof(address_t)) },

				// under discussion
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DYNAMIC_REQ, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DYNAMIC_REQWS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DYNAMIC_REQWE, sizeof(address_t)) }
			}
		 };

		const IntermediateLanguageProperty& property = properties.at(static_cast<size_t>(code));
#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
		MANA_ASSERT(property.mIntermediateLanguage == code);
#endif
		return property;
	}

#undef MANA_FILE_FORMAT_DEBUG_PARAMETER

	inline address_t GetInstructionSize(const void* codeBuffer, const address_t index)
	{
		const uint8_t* program = &static_cast<const uint8_t*>(codeBuffer)[index];
		const IntermediateLanguage code = static_cast<IntermediateLanguage>(*program);
		if (IntermediateLanguage::CALL == code)
		{
			// Size varies depending on the argument.
			return sizeof(address_t) + sizeof(int16_t) + sizeof(int16_t) + (program[6] * sizeof(int16_t));
		}
		else
		{
			return GetIntermediateLanguageProperty(code).mSize;
		}
	}
}
