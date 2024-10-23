/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"

#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
#include "Assert.h"
#endif

#include <array>
#include <cstdio>
#include <string_view>

namespace mana
{
	static constexpr const char* Signature = "MaNa";		//!< manaファイル署名
	static constexpr uint8_t MajorVersion = 0;				//!< manaファイルのメジャーバージョン番号
	static constexpr uint8_t MinorVersion = 13;				//!< manaファイルのマイナーバージョン番号

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
			Is64Bit,	//!< フラグ：64ビットでコンパイルされた
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
		Restart = 0,							/* 00 */
		Halt,									/* 01 */
		Yield,									/* 02 */
		NonPreEmptive,							/* 03 */
		PreEmptive,								/* 04 */

		/* jump */
		BranchEqual,							/* 05 */
		BranchNotEqual,							/* 06 */
		Branch,									/* 07 */
		BranchSubRoutine,						/* 08 */
		Call,									/* 09 */
		Request,								/* 0A */
		RequestWaitStarting,					/* 0B */
		RequestWaitEnded,						/* 0C */
		Join,									/* 0D */
		Comply,									/* 0E */
		Refuse,									/* 0F */
		LoadReturnAddress,						/* 10 */
		SaveReturnAddress,						/* 11 */
		ReturnFromFunction,						/* 12 */
		ReturnFromAction,						/* 13 */
		Rollback,								/* 14 */

		/* constant */
		PushZeroInteger,						/* 15 */
		PushZeroFloat,							/* 16 */
		PushChar,								/* 17 */
		PushShort,								/* 18 */
		PushInteger,							/* 19 */
		PushSize,								/* 19 */
		PushFloat,								/* 1A */
		PushString,								/* 1B */
		PushPriority,							/* 1C */
		PushActor,								/* 1D */
		PushSelf,								/* 1E */
		PushSender,								/* 1F */

		/* stack */
		Allocate,								/* 20 */
		Free,									/* 21 */
		Duplicate,								/* 22 */
		DuplicateData,							/* 23 */
		Remove,									/* 24 */
		RemoveData,								/* 25 */
		LoadStaticAddress,						/* 26 */
		LoadGlobalAddress,						/* 27 */
		LoadFrameAddress,						/* 28 */
		LoadSelfAddress,						/* 29 */

		/* memory operation */
		LoadChar,								/* 2A */
		LoadShort,								/* 2B */
		LoadInteger,							/* 2C */
		LoadFloat,								/* 2D */
		LoadReference,							/* 2E */
		LoadData,								/* 2F */
		StoreChar,								/* 30 */
		StoreShort,								/* 31 */
		StoreInteger,							/* 32 */
		StoreFloat,								/* 33 */
		StoreReference,							/* 34 */
		StoreData,								/* 35 */

		/* calculation */
		AddInteger,								/* 36 */
		DivideInteger,							/* 37 */
		MinusInteger,							/* 38 */
		ModInteger,								/* 39 */
		MultiInteger,							/* 3A */
		SubtractInteger,						/* 3B */

		AddFloat,								/* 3C */
		DivideFloat,							/* 3D */
		MinusFloat,								/* 3E */
		ModFloat,								/* 3F */
		MultiFloat,								/* 40 */
		SubtractFloat,							/* 41 */

		And,									/* 42 */
		ExclusiveOr,							/* 43 */
		LogicalAnd,								/* 44 */
		LogicalOr,								/* 45 */
		LogicalNot,								/* 46 */
		Not,									/* 47 */
		Or,										/* 48 */
		ShiftLeft,								/* 49 */
		ShiftRight,								/* 4A */

		CastIntegerToFloat,						/* 4B */
		CastFloatToInteger,						/* 4C */

		/* compare and branch */
		CompareEqualInteger,					/* 4D */
		CompareNotEqualInteger,					/* 4E */
		CompareGreaterEqualInteger,				/* 4F */
		CompareGreaterInteger,					/* 50 */
		CompareLessEqualInteger,				/* 51 */
		CompareLessInteger,						/* 52 */

		CompareEqualFloat,						/* 53 */
		CompareNotEqualFloat,					/* 54 */
		CompareGreaterEqualFloat,				/* 55 */
		CompareGreaterFloat,					/* 56 */
		CompareLessEqualFloat,					/* 57 */
		CompareLessFloat,						/* 58 */

		CompareEqualData,						/* 59 */
		CompareNotEqualData,					/* 5A */
		CompareGreaterEqualData,				/* 5B */
		CompareGreaterData,						/* 5C */
		CompareLessEqualData,					/* 5D */
		CompareLessData,						/* 5E */

		/* utility */
		Print,									/* 5F */

		/* under discussion */
		DynamicRequest,							/* 60 */
		DynamicRequestWaitStarting,				/* 61 */
		DynamicRequestWaitEnded,				/* 62 */
	};

	static constexpr address_t IntermediateLanguageSize = static_cast<address_t>(IntermediateLanguage::DynamicRequestWaitEnded) + 1;

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
		static constexpr std::array<IntermediateLanguageProperty, static_cast<address_t>(IntermediateLanguageSize)> properties =
		{
			{
				// thread
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Restart, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Halt, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Yield, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(NonPreEmptive, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PreEmptive, 0) },

				// jump
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(BranchEqual, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(BranchNotEqual, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Branch, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(BranchSubRoutine, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Call, sizeof(address_t) + sizeof(int16_t) + sizeof(int16_t) /* + (program[6] * sizeof(int16_t))*/)}, // 引数によってサイズが変わる
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Request, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(RequestWaitStarting, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(RequestWaitEnded, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Join, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Comply, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Refuse, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadReturnAddress, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SaveReturnAddress, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ReturnFromFunction, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ReturnFromAction, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Rollback, 0) },

				// constant
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushZeroInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushZeroFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushChar, sizeof(int8_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushShort, sizeof(int16_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushInteger, sizeof(int32_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushSize, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushFloat, sizeof(float)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushString, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushPriority, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushActor, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushSelf, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(PushSender, 0) },

				// stack
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Allocate, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Free, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Duplicate, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DuplicateData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Remove, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(RemoveData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadStaticAddress, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadGlobalAddress, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadFrameAddress, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadSelfAddress, sizeof(address_t)) },

				// memory operation
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadChar, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadShort, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadReference, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LoadData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(StoreChar, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(StoreShort, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(StoreInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(StoreFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(StoreReference, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(StoreData, sizeof(address_t)) },

				// calculation
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(AddInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DivideInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MinusInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ModInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MultiInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SubtractInteger, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(AddFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DivideFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MinusFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ModFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(MultiFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(SubtractFloat, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(And, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ExclusiveOr, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LogicalAnd, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LogicalOr, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(LogicalNot, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Not, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Or, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ShiftLeft, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(ShiftRight, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CastIntegerToFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CastFloatToInteger, 0) },

				// compare and branch
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareEqualInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareNotEqualInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareGreaterEqualInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareGreaterInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareLessEqualInteger, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareLessInteger, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareEqualFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareNotEqualFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareGreaterEqualFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareGreaterFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareLessEqualFloat, 0) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareLessFloat, 0) },

				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareEqualData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareNotEqualData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareGreaterEqualData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareGreaterData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareLessEqualData, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(CompareLessData, sizeof(address_t)) },

				// utility
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(Print, sizeof(address_t)) },

				// under discussion
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DynamicRequest, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DynamicRequestWaitStarting, sizeof(address_t)) },
				{ MANA_FILE_FORMAT_DEBUG_PARAMETER(DynamicRequestWaitEnded, sizeof(address_t)) }
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
		if (IntermediateLanguage::Call == code)
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
