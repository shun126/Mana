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
		MANA_IL_PUSH_SIZE,								/* 19 */
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
	};

	static constexpr address_t IntermediateLanguageSize = static_cast<address_t>(MANA_IL_DYNAMIC_REQWE + 1);

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
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_RESTART, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_HALT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_YIELD, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_NONPREEMPTIVE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PREEMPTIVE, 0) },

				// jump
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_BEQ, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_BNE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_BRA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_BSR, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_CALL, sizeof(address_t) + sizeof(int16_t) + sizeof(int16_t) /* + (program[6] * sizeof(int16_t))*/)}, // 引数によってサイズが変わる
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_REQ, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_REQWS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_REQWE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_JOIN, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPLY, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_REFUSE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_RETURN_ADDRESS, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_SAVE_RETURN_ADDRESS, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_RETURN_FROM_FUNCTION, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_RETURN_FROM_ACTION, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_ROLLBACK, 0) },

				// constant
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_ZERO_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_ZERO_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_CHAR, sizeof(int8_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_SHORT, sizeof(int16_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_INTEGER, sizeof(int32_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_SIZE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_FLOAT, sizeof(float)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_STRING, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_PRIORITY, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_ACTOR, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_SELF, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PUSH_SENDER, 0) },

				// stack
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_ALLOCATE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_FREE, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_DUPLICATE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_DUPLICATE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_REMOVE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_REMOVE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_STATIC_ADDRESS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_GLOBAL_ADDRESS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_FRAME_ADDRESS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_SELF_ADDRESS, sizeof(address_t)) },

				// memory operation
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_CHAR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_SHORT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_REFFRENCE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOAD_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_STORE_CHAR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_STORE_SHORT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_STORE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_STORE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_STORE_REFFRENCE, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_STORE_DATA, sizeof(address_t)) },

				// caluclation
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_ADD_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_DIV_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_MINUS_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_MOD_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_MUL_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_SUB_INTEGER, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_ADD_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_DIV_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_MINUS_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_MOD_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_MUL_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_SUB_FLOAT, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_AND, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_EOR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LAND, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LOR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_LNOT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_NOT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_OR, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_SHL, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_SHR, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_INT2FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_FLOAT2INT, 0) },

				// compare and branch
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_EQ_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_NE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_GE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_GT_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_LE_INTEGER, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_LS_INTEGER, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_EQ_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_NE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_GE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_GT_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_LE_FLOAT, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_LS_FLOAT, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_EQ_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_NE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_GE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_GT_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_LE_DATA, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_COMPARE_LS_DATA, sizeof(address_t)) },

				// utility
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_PRINT, sizeof(address_t)) },

				// under discussion
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_DYNAMIC_REQ, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_DYNAMIC_REQWS, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MANA_IL_DYNAMIC_REQWE, sizeof(address_t)) }
			}
		 };

		const IntermediateLanguageProperty& property = properties.at(code);
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
		if (IntermediateLanguage::MANA_IL_CALL == code)
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
