/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Buffer.h"
#include "Stack.h"

#include <bitset>
#include <unordered_map>

#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace mana
{
	class VM;

	class Actor : private Noncopyable, public std::enable_shared_from_this<Actor>
	{
		friend class VM;

		explicit Actor(const std::shared_ptr<VM>& vm, const size_t variableSize);

	public:
		//! request / rollbackコールバック
		using Callback = int32_t (*)(const std::shared_ptr<Actor>& actor, void* parameter);
				
	public:
		virtual ~Actor();
		
		std::shared_ptr<Actor> Clone() const;
		
		//void Serialize(mana_stream* stream);
		//void Deserialize(mana_stream* stream);
				
		bool Run();
		bool SyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender);
		bool AsyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender);
		
		bool Request(const int32_t level, const char* mName, const std::shared_ptr<Actor>& sender);
		void Rollback(const int32_t level);
		void Restart();

		const char* GetName();
		uint32_t GetAction(const char* actionName) const;

		int32_t GetCounter() const;
		void SetCounter(const int32_t counter);
		int32_t GetArgumentCount() const;
		int32_t GetArgumentCountByAddress(const uint32_t address) const;
		int32_t GetArgumentSize(const uint32_t address);
		bool HasReturnValue(const uint32_t address);
		int32_t GetParameterInteger(const int32_t value);
		float GetParameterFloat(const int32_t value);
		const char* GetParameterString(const int32_t value);
		Actor* GetParameterActor(const int32_t value);
		void* GetParameterPointer(const int32_t value);
		void* GetParameterAddress(const int32_t value);
		void SetReturnInteger(const int32_t value);
		void SetReturnFloat(const float value);
		void SetReturnString(const char* string);
		void SetReturnActor(const std::shared_ptr<Actor>& actor);
		void SetReturnPointer(void* pointer);
		void SetReturnData(const void* pointer, const int32_t size);

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		const char* GetActionName() const;
		const char* GetFunctionName() const;
#endif

		std::shared_ptr<VM> GetVirtualMachine() const { return mVM.lock(); }
		bool IsInit();
		bool IsRepeat();
		bool IsRunning();
		void Repeat(const bool initialComplete);
		void Again();
		void Halt();
		void Stop() { mFlag.reset(Flag::RUNNING); }
		void yield() { mInterrupt[mInterruptLevel].mFlag |= Interrupt::Flag::Suspend; }
		void Accept() { mFlag.reset(Flag::REFUSED); }
		void Refuse() { mFlag.set(Flag::REFUSED); }
		int32_t GetInterruptLevel() const { return mInterruptLevel; }
		bool IsSynchronized() const { return mInterrupt[mInterruptLevel].mFlag.test(Interrupt::Flag::Synchronized); }
		void SetSynchronized(const bool synchronized);
		void SetSynchronizedWithLevel(const size_t level, const bool synchronized);
		
#if 0
		void GetData(const int32_t resouce_id, const void** buffer, size_t* size);
		
		fileCallback* GetFileCallback(void);
		
		void SetFileCallback(fileCallback* function);
		void* GetFileCallbackParameter(int32_t level);
		void SetFileCallbackParameter(int32_t level, void* parameter);
		void SetRequestCallback(Callback* function);
		void SetRequestCallbackParameter(void* parameter);
		void SetRollbackCallback(Callback* function);
		void SetRollbackCallbackParameter(void* parameter);
#endif

		uintptr_t GetUserData() const;
		void SetUserData(const uintptr_t userData);
		void* GetUserPointer() const;
		void SetUserPointer(void* userPointer);

		Stack& GetStack() { return mStack; }
		const Stack& GetStack() const { return mStack; }

	private:
		void SetAction(const char* actionName, const uint32_t address);
		//void Initialize(const ActionInfoHeader* actionInfo);

	private:
		static const uint32_t nil = static_cast<uint32_t>(~0);

		static const size_t MANA_ACTOR_MAX_INTERRUPT_LEVEL = 32;

		//! 割り込みテーブル
		typedef struct Interrupt
		{
			std::shared_ptr<Actor> mSender;				//!< 要求したmana_actor オブジェクト
			uint32_t mAddress;						//!< 割り込み先アドレス
			uint32_t mReturnAddress;				//!< リターンアドレス
			int32_t mCounter;						//!< 汎用カウンタ
			int32_t mFramePointer;					//!< フレームポインタ
			int32_t mStackPointer;					//!< スタックポインタ
			void* mFileCallbackParameter;			//!< ファイル終了コールバックパラメータ
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			const char* mActionName;				//!< 実行中のアクション名 */
#endif
			std::bitset<8> mFlag;

			enum Flag : uint8_t
			{
				Initialized,	//!< 初期化済みフラグ
				IsInSyncCall,	//!< 同期実行フラグ
				Repeat,			//!< リピートフラグ
				Suspend,		//!< サスペンドフラグ
				Synchronized	//!< 同期フラグ
			}Flag;
		}Interrupt;

		//! 戻り値
		typedef struct ReturnValue
		{
			union
			{
				int_t mIntegerValue;			//!< 整数値
				float_t mFloatValue;				//!< 実数値
				const char* mStringValue;		//!< 文字列
				void* mPointerValue;			//!< 構造体
				Actor* mActorValue;				//!< アクター
			} mValues;
			int32_t mSize;						//!< サイズ(mPointerValue)
		}ReturnValue;

		static const int32_t MANA_RETURN_VALUE_TYPE_INVALID = 0;	/*!< 戻り値：無効 */
		static const int32_t MANA_RETURN_VALUE_TYPE_ACTOR = -1;		/*!< 戻り値：アクターへの参照 */
		static const int32_t MANA_RETURN_VALUE_TYPE_INTEGER = -2;	/*!< 戻り値：整数 */
		static const int32_t MANA_RETURN_VALUE_TYPE_FLOAT = -3;		/*!< 戻り値：小数 */
		static const int32_t MANA_RETURN_VALUE_TYPE_STRING = -4;	/*!< 戻り値：文字列への参照 */
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		char* mName;
#endif
		std::weak_ptr<VM> mVM;
		std::unordered_map <const char*, uint32_t, HashValueGenerate, HashValueCompare> mActions;
		Buffer mFrame;
		Stack mStack;
		Interrupt mInterrupt[MANA_ACTOR_MAX_INTERRUPT_LEVEL];
		ReturnValue mReturnValue;

		//std::map<const char* action, uint8_t* address> mAction;
		void* mRequestCallbackParameter;							/*!< リクエストコールバック */
		void* mRollbackCallbackParameter;							/*!< ロールバックコールバック */
		Buffer mVariable;	// 将来的にBufferクラスへ
		uint32_t mPc;											/*!< プログラムカウンタ */
		uint8_t mInterruptLevel;								/*!< 割り込みレベル */
		std::bitset<8> mFlag;
		enum Flag : uint8_t
		{
			//BATTLER,		//!< 戦闘参加フラグ
			//CHARACTER,		//!< キャラクターフラグ
			HALT,			//!< 停止フラグ
			RUNNING,		//!< 実行フラグ
			TOUCHED,		//!< 接触フラグ
			HIDED,			//!< 非表示フラグ
			REQUESTED,		//!< リクエストフラグ
			REFUSED		//!< リクエスト禁止フラグ
		}Flag;
		uintptr_t mUserData;										//!< ユーザーデータ
		void* mUserPointer;											//!< ユーザーポインター

	private:
		static void CommandRestart(Actor& self);
		static void CommandHalt(Actor& self);
		static void CommandYield(Actor& self);
		static void CommandSetNonPreemptive(Actor& self);
		static void CommandSetPreemptive(Actor& self);
		static void CommandPushZeroInteger(Actor& self);
		static void CommandPushZeroFloat(Actor& self);
		static void CommandPushChar(Actor& self);
		static void CommandPushShort(Actor& self);
		static void CommandPushInteger(Actor& self);
		static void CommandPushFloat(Actor& self);
		static void CommandPushString(Actor& self);
		static void CommandPushPriority(Actor& self);
		static void CommandPushSelf(Actor& self);
		static void CommandPushSender(Actor& self);
		static void CommandAllocate(Actor& self);
		static void CommandRelease(Actor& self);
		static void CommandDuplicate(Actor& self);
		static void CommandRemove(Actor& self);
		static void CommandLoadStaticAddress(Actor& self);
		static void CommandLoadGlobalAddress(Actor& self);
		static void CommandLoadFrameAddress(Actor& self);
		static void CommandLoadSelfAddress(Actor& self);
		static void CommandLoadInt8(Actor& self);
		static void CommandLoadInt16(Actor& self);
		static void CommandLoadInt32(Actor& self);
		static void CommandLoadFloat(Actor& self);
		static void CommandLoadReffrence(Actor& self);
		static void CommandStoreInt8(Actor& self);
		static void CommandStoreInt16(Actor& self);
		static void CommandStoreInt32(Actor& self);
		static void CommandStoreFloat(Actor& self);
		static void CommandStoreReffrence(Actor& self);
		static void CommandBranchEqual(Actor& self);
		static void CommandBranchNotEqual(Actor& self);
		static void CommandBranchAway(Actor& self);
		static void CommandBranchSubRoutine(Actor& self);
		static void CommandCall(Actor& self);
		static void CommandRequest(Actor& self);
		static void CommandRequestWaitStarting(Actor& self);
		static void CommandRequestWaitEnding(Actor& self);
		static void Commanddynamic_request(Actor& self);
		static void CommandDynamicRequestWaitStarting(Actor& self);
		static void Commanddynamic_request_wait_ending(Actor& self);
		static void CommandJoin(Actor& self);
		static void CommandComply(Actor& self);
		static void CommandRefuse(Actor& self);
		static void CommandLoadReturnAddress(Actor& self);
		static void CommandStoreReturnAddress(Actor& self);
		static void CommandReturnFromFunction(Actor& self);
		static void CommandReturnFromAction(Actor& self);
		static void CommandRollback(Actor& self);
		static void CommandAddInteger(Actor& self);
		static void CommandAddFloat(Actor& self);
		static void CommandDivideInteger(Actor& self);
		static void CommandDivideFloat(Actor& self);
		static void CommandMinusInteger(Actor& self);
		static void CommandMinusFloat(Actor& self);
		static void CommandModInteger(Actor& self);
		static void CommandModfloat(Actor& self);
		static void CommandMultiplyInteger(Actor& self);
		static void CommandMultiplyFloat(Actor& self);
		static void CommandSubtractInteger(Actor& self);
		static void CommandSubtractFloat(Actor& self);
		static void CommandAnd(Actor& self);
		static void CommandExclusiveOr(Actor& self);
		static void CommandLogicalAnd(Actor& self);
		static void CommandLogicalOr(Actor& self);
		static void CommandNot(Actor& self);
		static void Commandlogical_not(Actor& self);
		static void CommandOr(Actor& self);
		static void CommandShiftLeft(Actor& self);
		static void CommandShiftRight(Actor& self);
		static void CommandCompareEqualInteger(Actor& self);
		static void CommandCompareEqualFloat(Actor& self);
		static void CommandCompareGreaterEqualInteger(Actor& self);
		static void CommandCompareGreaterEqualFloat(Actor& self);
		static void CommandCompareGreaterInteger(Actor& self);
		static void CommandCompareGreaterFloat(Actor& self);
		static void CommandCompareNotEqualInteger(Actor& self);
		static void CommandCompareNotEqualFloat(Actor& self);
		static void CommandCompareLessEqualInteger(Actor& self);
		static void CommandCompareLessEqualFloat(Actor& self);
		static void CommandCompareLessInteger(Actor& self);
		static void CommandCompareLessFloat(Actor& self);
		static void CommandIntegerToFloat(Actor& self);
		static void CommandFloatToInteger(Actor& self);
		static void CommandPushActor(Actor& self);
		static void CommandLoadData(Actor& self);
		static void CommandStoreData(Actor& self);
		static void CommandDuplicateData(Actor& self);
		static void CommandRemoveData(Actor& self);
		static void CommandCompareEqualData(Actor& self);
		static void CommandCompareGreaterEqualData(Actor& self);
		static void CommandCompareGreaterdata(Actor& self);
		static void CommandCompareNotEqualData(Actor& self);
		static void CommandCompareLessEqualData(Actor& self);
		static void CommandCompareLessdata(Actor& self);
		static void CommandPrint(Actor& self);
	};
}

#include "VM.h"

namespace mana
{
	inline Actor::Actor(const std::shared_ptr<VM>& vm, const size_t variableSize)
	{
		mVM = vm;
		MANA_ASSERT(vm.get() != nullptr);
		mVariable.Allocate(variableSize);
	}
	
	inline std::shared_ptr<Actor> Actor::Clone() const
	{
		std::shared_ptr<Actor> sharedActor = std::shared_ptr<Actor>(
			new Actor(mVM.lock(), mVariable.GetSize())
		);
		sharedActor->mActions = mActions;

		return sharedActor;
	}
	
	inline Actor::~Actor()
	{
	}

#if 0	
	inline void Actor::Serialize(mana_stream* stream)
	{
		int32_t i;
		
		MANA_ASSERT(self && stream);
		
		mana_frame_serialize(&mFrame, stream);
		mana_stack_serialize(&mStack, stream);
		
		mana_stream_mark(stream);
		
		for(i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			Interrupt* p = &mInterrupt[i];
			
			const char* sender_name = mana_get_actor_name(mVM, p->mSender);
			if(sender_name)
			{
				mana_stream_push_char(stream, 1);
				mana_stream_push_string(stream, sender_name);
			}else{
				mana_stream_push_char(stream, 0);
			}
			
			mana_stream_pushInteger(stream, p->mAddress);
			mana_stream_pushInteger(stream, p->mReturnAddress);
			mana_stream_pushInteger(stream, p->mCounter);
			mana_stream_pushInteger(stream, p->mFramePointer);
			mana_stream_pushInteger(stream, p->mStackPointer);
			mana_stream_push_unsigned_char(stream, p->mFlag);
			
			mana_stream_mark(stream);
		}
		
		mana_stream_push_size(stream, mVariableSize);
		mana_stream_pushData(stream, mVariable, mVariableSize);
		mana_stream_pushInteger(stream, mPc);
		
		mana_stream_push_char(stream, mInterruptLevel);
		mana_stream_push_unsigned_char(stream, mFlag);
		
		mana_stream_mark(stream);
	}
	
	inline void Actor::Deserialize(mana_stream* stream)
	{
		MANA_ASSERT(self && stream);
		
		mana_frame_deserialize(&mFrame, stream);
		mana_stack_deserialize(&mStack, stream);
		
		mana_stream_check(stream);
		
		for(int32_t i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			Interrupt* p = &mInterrupt[i];
			
			if(mana_stream_pop_char(stream))
			{
				p->mSender = mana_get_actor(mVM, mana_stream_get_string_pointer(stream));
				mana_steram_seek(stream, mana_stream_get_string_length(stream));
			}
			
			p->mAddress = mana_stream_popInteger(stream);
			p->mReturnAddress = mana_stream_popInteger(stream);
			p->mCounter = mana_stream_popInteger(stream);
			p->mFramePointer = mana_stream_popInteger(stream);
			p->mStackPointer = mana_stream_popInteger(stream);
			p->mFlag = mana_stream_pop_unsigned_char(stream);
			
			mana_stream_check(stream);
		}
		
		mVariableSize = mana_stream_pop_size(stream);
		mVariable = mana_malloc(mVariableSize);
		mana_stream_popData(stream, mVariable, mVariableSize);
		
		mPc = mana_stream_popInteger(stream);
		
		mInterruptLevel = mana_stream_pop_char(stream);
		mFlag = mana_stream_pop_unsigned_char(stream);
		
		mana_stream_check(stream);
	}
#endif
	
	inline bool Actor::Run()
	{
		using IntermediateLanguage = void(*)(Actor& self);
		static const IntermediateLanguage mIntermediateLanguage[MANA_IL_MAX_SIZE] = {
			// thread
			&CommandRestart,						/* 00 */
			&CommandHalt,							/* 01 */
			&CommandYield,							/* 02 */
			&CommandSetNonPreemptive,				/* 03 */
			&CommandSetPreemptive,					/* 04 */

			// jump
			&CommandBranchEqual,					/* 05 */
			&CommandBranchNotEqual,				/* 06 */
			&CommandBranchAway,					/* 07 */
			&CommandBranchSubRoutine,				/* 08 */
			&CommandCall,							/* 09 */
			&CommandRequest,						/* 0A */
			&CommandRequestWaitStarting,			/* 0B */
			&CommandRequestWaitEnding,			/* 0C */
			&CommandJoin,							/* 0D */
			&CommandComply,							/* 0E */
			&CommandRefuse,							/* 0F */
			&CommandLoadReturnAddress,			/* 10 */
			&CommandStoreReturnAddress,			/* 11 */
			&CommandReturnFromFunction,			/* 12 */
			&CommandReturnFromAction,				/* 13 */
			&CommandRollback,						/* 14 */

			// constant
			&CommandPushZeroInteger,				/* 15 */
			&CommandPushZeroFloat,				/* 16 */
			&CommandPushChar,						/* 17 */
			&CommandPushShort,						/* 18 */
			&CommandPushInteger,					/* 19 */
			&CommandPushFloat,						/* 1A */
			&CommandPushString,					/* 1B */
			&CommandPushPriority,					/* 1C */
			&CommandPushActor,						/* 1D */
			&CommandPushSelf,						/* 1E */
			&CommandPushSender,					/* 1F */

			// stack
			&CommandAllocate,						/* 20 */
			&CommandRelease,							/* 21 */
			&CommandDuplicate,						/* 22 */
			&CommandDuplicateData,					/* 23 */
			&CommandRemove,							/* 24 */
			&CommandRemoveData,					/* 25 */
			&CommandLoadStaticAddress,			/* 26 */
			&CommandLoadGlobalAddress,			/* 27 */
			&CommandLoadFrameAddress,				/* 28 */
			&CommandLoadSelfAddress,				/* 29 */

			// memory operation
			&CommandLoadInt8,						/* 2A */
			&CommandLoadInt16,						/* 2B */
			&CommandLoadInt32,					/* 2C */
			&CommandLoadFloat,						/* 2D */
			&CommandLoadReffrence,					/* 2E */
			&CommandLoadData,						/* 2F */
			&CommandStoreInt8,						/* 30 */
			&CommandStoreInt16,					/* 31 */
			&CommandStoreInt32,					/* 32 */
			&CommandStoreFloat,					/* 33 */
			&CommandStoreReffrence,				/* 34 */
			&CommandStoreData,						/* 35 */

			// caluclation
			&CommandAddInteger,					/* 36 */
			&CommandDivideInteger,					/* 37 */
			&CommandMinusInteger,					/* 38 */
			&CommandModInteger,					/* 39 */
			&CommandMultiplyInteger,				/* 3A */
			&CommandSubtractInteger,				/* 3B */

			&CommandAddFloat,						/* 3C */
			&CommandDivideFloat,					/* 3D */
			&CommandMinusFloat,					/* 3E */
			&CommandModfloat,						/* 3F */
			&CommandMultiplyFloat,					/* 40 */
			&CommandSubtractFloat,					/* 41 */

			&CommandAnd,							/* 42 */
			&CommandExclusiveOr,					/* 43 */
			&CommandLogicalAnd,					/* 44 && */
			&CommandLogicalOr,						/* 45 || */
			&Commandlogical_not,					/* 46 ! */
			&CommandNot,							/* 47 ~ */
			&CommandOr,								/* 48 */
			&CommandShiftLeft,						/* 49 */
			&CommandShiftRight,					/* 4A */

			&CommandIntegerToFloat,				/* 4B */
			&CommandFloatToInteger,				/* 4C */

			// compare and branch
			&CommandCompareEqualInteger,			/* 4D */
			&CommandCompareNotEqualInteger,		/* 4E */
			&CommandCompareGreaterEqualInteger,	/* 4F */
			&CommandCompareGreaterInteger,		/* 50 */
			&CommandCompareLessEqualInteger,		/* 51 */
			&CommandCompareLessInteger,			/* 52 */

			&CommandCompareEqualFloat,			/* 53 */
			&CommandCompareNotEqualFloat,		/* 54 */
			&CommandCompareGreaterEqualFloat,	/* 55 */
			&CommandCompareGreaterFloat,			/* 56 */
			&CommandCompareLessEqualFloat,		/* 57 */
			&CommandCompareLessFloat,				/* 58 */

			&CommandCompareEqualData,				/* 59 */
			&CommandCompareNotEqualData,			/* 5A */
			&CommandCompareGreaterEqualData,		/* 5B */
			&CommandCompareGreaterdata,			/* 5C */
			&CommandCompareLessEqualData,		/* 5D */
			&CommandCompareLessdata,				/* 5E */

			// support
			&CommandPrint,							/* 5F */

			&Commanddynamic_request,				/* 60 */
			&CommandDynamicRequestWaitStarting,		/* 61 */
			&Commanddynamic_request_wait_ending,	/* 62 */
		};

		size_t timer = 0;		
		do{
			mFlag.reset(Flag::REQUESTED);
			
			if(!IsRunning())
				return false;
			
			const int32_t opecode = mVM.lock()->GetOpecode(mPc);
			(*mIntermediateLanguage[opecode])(*this);
			
			if(!IsRunning())
				return false;
			
			if(IsRepeat())
			{
				mInterrupt[mInterruptLevel].mFlag.reset(Actor::Interrupt::Flag::Repeat);
			}
			else
			{
				mPc += GetInstructionSize(mVM.lock()->mInstructionPool, mPc);
				mInterrupt[mInterruptLevel].mFlag.reset(Actor::Interrupt::Flag::Initialized);
			}
			
			if(mInterrupt[mInterruptLevel].mFlag[Actor::Interrupt::Flag::Suspend] && !IsSynchronized())
			{
				mInterrupt[mInterruptLevel].mFlag.reset(Actor::Interrupt::Flag::Suspend);
				break;
			}
		}while(++timer < 500 || IsSynchronized());
		
		return mFlag[Flag::RUNNING];
	}
	
	inline bool Actor::SyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if(Request(level, action, sender))
		{
			mInterrupt[level].mFlag.set(Actor::Interrupt::Flag::IsInSyncCall);
			while(true)
			{
				mVM.lock()->Run();
				if(mInterruptLevel < level)
					return true;
			}
		}
		return false;
	}
	
	inline bool Actor::AsyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if(Request(level, action, sender))
		{
			mInterrupt[level].mFlag.set(Actor::Interrupt::Flag::IsInSyncCall);
			while(true)
			{
				Run();
				if(mInterruptLevel < level)
					return true;
			}
		}
		return false;
	}
	
	inline bool Actor::Request(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if(level < 0 || level >= MANA_ACTOR_MAX_INTERRUPT_LEVEL)
		{
			MANA_TRACE("MANA: %s::%s request failed. reason: level %d range over\n", GetName(), action, level);
			return false;
		}
		
		if(mFlag[Flag::HALT])
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. reason: halt\n", level, GetName(), action);
			return false;
		}
		
		if(mFlag[Flag::REFUSED])
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. reason: refuse\n", level, GetName(), action);
			return false;
		}
		
		if(mInterrupt[level].mAddress != nil)
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. ", level, GetName(), action);
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			if(mInterrupt[level].mActionName)
				MANA_TRACE("reason : %s running", mInterrupt[level].mActionName);
#endif
			MANA_TRACE("\n");
			return false;
		}
		
		uint32_t address = GetAction(action);
		if(address == nil)
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. reason: not found\n", level, GetName(), action);
			return false;
		}
		
		mFlag.set(Flag::RUNNING);
		mInterrupt[level].mCounter = 0;
		mInterrupt[level].mSender = sender;
		mInterrupt[level].mAddress = address;
		mInterrupt[level].mFlag.reset(Actor::Interrupt::Flag::Synchronized);
		mInterrupt[level].mFileCallbackParameter = nullptr;
		
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		MANA_TRACE("Mana:request: %s ", GetName());
		if(mInterrupt[mInterruptLevel].mActionName)
			MANA_TRACE("level %d %s => ", mInterruptLevel, mInterrupt[mInterruptLevel].mActionName);
		MANA_TRACE("level %d %s (%08x) registered\n", level, action, address);

		// 実行するアクション名を記録
		mInterrupt[level].mActionName = action;
#endif
		
		if(level >= mInterruptLevel)
		{
			MANA_TRACE("mana:request: %s ", GetName());
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			if (mInterruptLevel)
			{
				MANA_TRACE("%s => level %d %s ", mInterruptLevel, mInterrupt[mInterruptLevel].mActionName);
			}
#endif
			MANA_TRACE("level %d %s succeed\n", level, mInterrupt[level].mActionName);

			// コールバック関数を呼びます
			//if(mRequestCallback)
			//	mRequestCallback(mRequestCallbackParameter);
			
			/* 現在よりも高い優先度(高いほど優先)の場合、すぐに割り込む */
			Again();
			
			/* 現在のFPとSPを保存します */
			mInterrupt[level].mFramePointer = mFrame.GetSize();
			mInterrupt[level].mStackPointer = mStack.GetSize();
			
			/* 現在のプログラムカウンタを保存します */
			mInterrupt[mInterruptLevel].mAddress = mPc;
			
			/* 新しい優先度(高いほど優先)とプログラムカウンタを設定します */
			mInterruptLevel = (uint8_t)level;
			mPc = address;
			
			/* 次のTickでプログラムカウンタを進めない処理 */
			mInterrupt[mInterruptLevel].mFlag.set(Actor::Interrupt::Flag::Repeat);
		}
		else
		{
			/* 現在よりも低い優先度(高いほど優先)の場合、現在のアクション終了後に実行 */
			
			/* 現在の優先度(高いほど優先)に入ったときのFPとSPを保存します */
			mInterrupt[level].mFramePointer = mInterrupt[mInterruptLevel].mFramePointer;
			mInterrupt[level].mStackPointer = mInterrupt[mInterruptLevel].mStackPointer;
			
			/* 次のTickでプログラムカウンタを進める処理 */
			mInterrupt[level].mFlag.reset(Actor::Interrupt::Flag::Repeat);
		}
		
		MANA_ASSERT(mPc != nil);
		
		return true;
	}
	
	inline void Actor::Rollback(const int32_t level)
	{		
		// 非同期モードに変更
		SetSynchronized(false);
		
		// リクエストが終了したことをsenderに通知する
		if(mInterrupt[mInterruptLevel].mSender)
		{
			mInterrupt[mInterruptLevel].mSender->Again();
			mInterrupt[mInterruptLevel].mSender = nullptr;
		}
#if 0
		/* ファイル読み込み中ならば解放 */
		if(mAsyncFileCallback && mInterrupt[mInterruptLevel].mFileCallbackParameter)
		{
			mAsyncFileCallback(MANA_FILE_COMMAND_CLOSE, mInterrupt[mInterruptLevel].mFileCallbackParameter);
			mInterrupt[mInterruptLevel].mFileCallbackParameter = nullptr;
		}
#endif

		// 優先度開放
		mInterrupt[mInterruptLevel].mAddress = nil;
		
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		const uint8_t lastInterruptLevel = mInterruptLevel;
		const char* lastActionName = mInterrupt[mInterruptLevel].mActionName;

		// 実行中のアクション名を消去
		mInterrupt[mInterruptLevel].mActionName = nullptr;
#endif
		
		// SyncCall中か取得
		const bool inSyncCall = mInterrupt[mInterruptLevel].mFlag[Interrupt::Flag::IsInSyncCall];
		if(inSyncCall)
		{
			mInterrupt[mInterruptLevel].mFlag.reset(Interrupt::Flag::IsInSyncCall);
		}
		
		// FPとSPを取得
		size_t framePointer = mInterrupt[mInterruptLevel].mFramePointer;
		size_t stackPointer = mInterrupt[mInterruptLevel].mStackPointer;
		
		// 優先度(高いほど優先)が指定されているならば、強制的に指定優先度まで戻る
		int32_t currentLevel = mInterruptLevel - 1;
		if(currentLevel > level)
		{
			while(currentLevel > level)
			{
				Interrupt* interrupt = &mInterrupt[currentLevel];
				
				// 優先度開放
				interrupt->mAddress = nil;
#if 0
				/* ファイルエントリの削除 */
				if(mAsyncFileCallback && mInterrupt->mFileCallbackParameter)
				{
					MANA_TRACE("Rollback: %d: waitting for file loading\n", currentLevel);
					
					mAsyncFileCallback(MANA_FILE_COMMAND_CLOSE, interrupt->mFileCallbackParameter);
					interrupt->mFileCallbackParameter = nullptr;
				}
#endif				
				// フレームポインタとスタックポインタを取得
				framePointer = mInterrupt->mFramePointer;
				stackPointer = mInterrupt->mStackPointer;
#if 0
				/* コールバック関数を呼びます */
				if(mRollbackCallback)
					mRollbackCallback(mRollbackCallbackParameter);
#endif			
				--currentLevel;
			}
		}
		
		// FPとSPを復元します
		mFrame.SetSize(framePointer);
		mStack.SetSize(stackPointer);
		
		// 中断されたアクションの再開
		while(currentLevel >= 0)
		{
			Interrupt* interrupt = &mInterrupt[currentLevel];
			if(interrupt->mAddress != ~0)
			{
				// 中断していた場所から復帰させます
				mPc = interrupt->mAddress;
#if 0
				// コールバック関数を呼びます
				if(mRollbackCallback)
					mRollbackCallback(mRollbackCallbackParameter);
#endif			
				// 優先度(高いほど優先)変更
				mInterruptLevel = static_cast<uint8_t>(currentLevel);
				
				// 次回のTickでプログラムカウンターを加算しない
				mInterrupt[mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);
				
				// SyncCall中ならばTickを抜ける
				if(inSyncCall)
					yield();

				MANA_TRACE(
					"mana:rollback: %s level %d %s => level %d %s succeed\n",
					GetName(),
					lastInterruptLevel, lastActionName,
					mInterruptLevel, interrupt->mActionName
				);

				/*
				 MANA_ASSERT(
				 mPc >= GetParent().instruction_pool &&
				 mPc < &GetParent().instruction_pool[GetParent().file_header->size_of_instruction_pool]);
				 */
				
				return;
			}
			--currentLevel;
		}
		
		// 実行可能なアクションが無いので、アクターは停止する
		Restart();
		MANA_TRACE("mana:rollback: %s level %d %s stoped\n", GetName(), lastInterruptLevel, lastActionName);
	}
	
	inline void Actor::Restart()
	{
		mPc = nil;
		mInterruptLevel = 0;
		mFlag.reset(Flag::HALT);
		mFlag.reset(Flag::RUNNING);
		mFlag.reset(Flag::REFUSED);
		
		memset(&mInterrupt, 0, sizeof(mInterrupt));
		for(uint32_t i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			mInterrupt[i].mAddress = nil;
			mInterrupt[i].mReturnAddress = nil;
		}
		
		mFrame.Clear();
		mStack.Clear();
	}
	
	inline const char* Actor::GetName()
	{
		return mVM.lock()->GetActorName(shared_from_this());
	}
	
	inline uint32_t Actor::GetAction(const char* actionName) const
	{
		const auto i = mActions.find(actionName);
		return (i == mActions.end()) ? nil : i->second;
	}
	
	inline void Actor::SetAction(const char* actionName, const uint32_t address)
	{
		mActions[actionName] = address;
	}

	inline int32_t Actor::GetCounter() const
	{
		return mInterrupt[mInterruptLevel].mCounter;
	}
	
	inline void Actor::SetCounter(const int32_t counter)
	{
		mInterrupt[mInterruptLevel].mCounter = counter;
	}
	
	inline int32_t Actor::GetArgumentCount() const
	{
		return GetArgumentCountByAddress(mPc);
	}
	
	inline int32_t Actor::GetArgumentCountByAddress(const uint32_t address) const
	{
		return (int32_t)mVM.lock()->GetUint8FromMemory(address + 1 + sizeof(int32_t) + sizeof(uint8_t));
	}
	
	inline int32_t Actor::GetArgumentSize(const uint32_t address)
	{
		return (int32_t)mVM.lock()->GetUint16FromMemory(address + 1 + sizeof(int32_t) + sizeof(uint8_t) + sizeof(uint8_t));
	}
	
	inline bool Actor::HasReturnValue(const uint32_t address)
	{
		return mVM.lock()->GetUint8FromMemory(address + 1 + sizeof(int32_t)) ? true : false;
	}
	
	inline int32_t Actor::GetParameterInteger(const int32_t value)
	{
		MANA_ASSERT(GetArgumentCount() > value);

		uint32_t address = mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t));
		size_t offset = (size_t)mVM.lock()->GetUint16FromMemory(address);
		return mStack.Get<int_t>(offset);
	}
	
	inline float Actor::GetParameterFloat(const int32_t value)
	{
		MANA_ASSERT(GetArgumentCount() > value);		
		return mStack.Get<float_t>(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))));
	}
	
	inline const char* Actor::GetParameterString(const int32_t value)
	{
		MANA_ASSERT(GetArgumentCount() > value);		
		return mStack.Get<const char*>(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))));
	}
	
	inline Actor* Actor::GetParameterActor(const int32_t value)
	{
		return static_cast<Actor*>(GetParameterPointer(value));
	}
	
	inline void* Actor::GetParameterPointer(const int32_t value)
	{
		MANA_ASSERT(GetArgumentCount() > value);		
		return mStack.Get<void*>(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))));
	}
	
	inline void* Actor::GetParameterAddress(const int32_t value)
	{
		MANA_ASSERT(GetArgumentCount() > value);
		return mStack.GetAddress(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))) + 1);
	}
	
	inline void Actor::SetReturnInteger(const int32_t value)
	{
		mReturnValue.mSize = MANA_RETURN_VALUE_TYPE_INTEGER;
		mReturnValue.mValues.mIntegerValue = value;
	}
	
	inline void Actor::SetReturnFloat(const float value)
	{
		mReturnValue.mSize = MANA_RETURN_VALUE_TYPE_FLOAT;
		mReturnValue.mValues.mFloatValue = value;
	}
	
	inline void Actor::SetReturnString(const char* string)
	{
		mReturnValue.mSize = MANA_RETURN_VALUE_TYPE_STRING;
		mReturnValue.mValues.mStringValue = string;
	}
	
	inline void Actor::SetReturnActor(const std::shared_ptr<Actor>& actor)
	{
		mReturnValue.mSize = MANA_RETURN_VALUE_TYPE_ACTOR;
		mReturnValue.mValues.mActorValue = actor.get();
	}
	
	inline void Actor::SetReturnPointer(void* pointer)
	{
		mReturnValue.mSize = MANA_RETURN_VALUE_TYPE_ACTOR;
		mReturnValue.mValues.mPointerValue = pointer;
	}
	
	inline void Actor::SetReturnData(const void* pointer, const int32_t size)
	{		
		if(size > 0)
		{
			mReturnValue.mValues.mPointerValue = std::malloc(size);
			memcpy(mReturnValue.mValues.mPointerValue, pointer, size);
		}
		else
		{
			mReturnValue.mValues.mPointerValue = nullptr;
		}
		mReturnValue.mSize = size;
	}
	
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
	inline const char* Actor::GetActionName() const
	{
		const char* action;
		
		action = mInterrupt[mInterruptLevel].mActionName;
		
		return action ? action : "SLEEP!";
	}
	
	inline const char* Actor::GetFunctionName() const
	{
		return mVM.lock()->GetStringFromMemory(mPc + 1);
	}
#endif
			
	inline bool Actor::IsInit()
	{
		return mInterrupt[mInterruptLevel].mFlag[Interrupt::Flag::Initialized];
	}
	
	inline bool Actor::IsRepeat()
	{
		return mInterrupt[mInterruptLevel].mFlag[Interrupt::Flag::Repeat];
	}
	
	inline bool Actor::IsRunning()
	{
		const uint8_t flag = mFlag[Flag::HALT] | mFlag[Flag::RUNNING];
		return flag == Flag::RUNNING;
	}
	
	inline void Actor::Repeat(const bool initial_complete)
	{
		if(initial_complete)
			mInterrupt[mInterruptLevel].mFlag.set(Interrupt::Flag::Initialized);
		else
			mInterrupt[mInterruptLevel].mFlag.reset(Interrupt::Flag::Initialized);
		mInterrupt[mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);
		mInterrupt[mInterruptLevel].mFlag.set(Interrupt::Flag::Suspend);
	}
	
	inline void Actor::Again()
	{
		mFlag.set(Flag::REQUESTED);
		mVM.lock()->mFlag.set(VM::Flag::Requested);
	}
	
	inline void Actor::Halt()
	{
		Stop();
		yield();
		
		mFlag.set(Flag::HALT);
		mInterruptLevel = MANA_ACTOR_MAX_INTERRUPT_LEVEL - 1;
		mInterrupt[mInterruptLevel].mAddress = nil;
	}
	
	inline void Actor::SetSynchronized(const bool synchronized)
	{
		if(synchronized)
			mInterrupt[mInterruptLevel].mFlag.set(Interrupt::Flag::Synchronized);
		else
			mInterrupt[mInterruptLevel].mFlag.reset(Interrupt::Flag::Synchronized);
	}
	
	inline void Actor::SetSynchronizedWithLevel(const size_t level, const bool synchronized)
	{
		MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);
		if(synchronized)
			mInterrupt[level].mFlag.set(Interrupt::Flag::Synchronized);
		else
			mInterrupt[level].mFlag.reset(Interrupt::Flag::Synchronized);
	}
	
#if 0
	inline void Actor::GetData(const const int32_t resouce_id, const void** buffer, size_t* mSize)
	{
		MANA_ASSERT(resouce_id >= 0 && resouce_id < manaDatalink_get_number_ofDatas(&mVM->datalink));
		*buffer = manaDatalink_getData(&mVM->datalink, resouce_id);
		*mSize = manaDatalink_getData_size(&mVM->datalink, resouce_id);
	}
	
	/*!
	 * @return	ファイルコールバック関数
	 */
	inline fileCallback* Actor::GetFileCallback(void)
	{
		return mAsyncFileCallback;
	}
	
	/*!
	 * @param[in]	function	ファイルコールバック関数
	 */
	inline void Actor::SetFileCallback(fileCallback* function)
	{
		mAsyncFileCallback = function;
	}
	
	/*!
	 * @param[in]	self	, オブジェクト
	 * @param[in]	level	優先度
	 * @return		ファイルコールバックパラメーター
	 */
	inline void* Actor::GetFileCallbackParameter(int32_t level)
	{
		MANA_ASSERT(self);
		MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);
		
		return mInterrupt[level].mFileCallbackParameter;
	}
	
	/*!
	 * @param[in]	self		, オブジェクト
	 * @param[in]	level		優先度
	 * @param[in]	parameter	ファイルコールバックパラメーター
	 */
	inline void Actor::SetFileCallbackParameter(int32_t level, void* parameter)
	{
		MANA_ASSERT(self);
		MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);
		
		mInterrupt[level].mFileCallbackParameter = parameter;
	}
	
	/*!
	 * @param[in]	function	リクエストコールバック関数
	 */
	inline void Actor::SetRequestCallback(Callback* function)
	{
		mRequestCallback = function;
	}
	
	/*!
	 * @param[in]	self		, オブジェクト
	 * @param[in]	parameter	リクエストコールバックパラメーター
	 */
	inline void Actor::SetRequestCallbackParameter(void* parameter)
	{
		MANA_ASSERT(self);
		
		mRequestCallbackParameter = parameter;
	}
	/*!
	 * @param[in]	function	ロールバックコールバック関数
	 */
	inline void Actor::SetRollbackCallback(Callback* function)
	{
		mRollbackCallback = function;
	}
	
	/*!
	 * @param[in]	function	ロールバックコールバック関数
	 * @param[in]	parameter	ロールバックコールバックパラメーター
	 */
	inline void Actor::SetRollbackCallbackParameter(void* parameter)
	{
		mRollbackCallbackParameter = parameter;
	}
#endif

	inline uintptr_t Actor::GetUserData() const
	{
		return mUserData;
	}
	
	inline void Actor::SetUserData(uintptr_t userData)
	{
		mUserData = userData;
	}
	
	inline void* Actor::GetUserPointer() const
	{
		return mUserPointer;
	}
	
	inline void Actor::SetUserPointer(void* userPointer)
	{
		mUserPointer = userPointer;
	}

	inline void Actor::CommandRestart(Actor& self)
	{
		self.Restart();
	}

	inline void Actor::CommandHalt(Actor& self)
	{
		self.Halt();
	}

	inline void Actor::CommandYield(Actor& self)
	{
		self.yield();
	}

	inline void Actor::CommandSetNonPreemptive(Actor& self)
	{
		self.SetSynchronized(true);
	}

	inline void Actor::CommandSetPreemptive(Actor& self)
	{
		self.SetSynchronized(false);
	}

	inline void Actor::CommandPushZeroInteger(Actor& self)
	{
		self.mStack.Push<int_t>(0);
	}

	inline void Actor::CommandPushZeroFloat(Actor& self)
	{
		self.mStack.Push(static_cast<float_t>(0.0));
	}

	inline void Actor::CommandPushChar(Actor& self)
	{
		self.mStack.Push<int_t>(self.mVM.lock()->GetInt8FromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushShort(Actor& self)
	{
		self.mStack.Push<int_t>(self.mVM.lock()->GetInt16FromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushInteger(Actor& self)
	{
		self.mStack.Push<int_t>(self.mVM.lock()->GetInt32FromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushFloat(Actor& self)
	{
		self.mStack.Push(self.mVM.lock()->GetFloatFromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushString(Actor& self)
	{
		self.mStack.Push(self.mVM.lock()->GetStringFromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushPriority(Actor& self)
	{
		self.mStack.Push<int_t>(self.mInterruptLevel);
	}

	inline void Actor::CommandPushSelf(Actor& self)
	{
		self.mStack.Push(&self);
	}

	inline void Actor::CommandPushSender(Actor& self)
	{
		self.mStack.Push(self.mInterrupt[self.mInterruptLevel].mSender.get());
	}

	inline void Actor::CommandAllocate(Actor& self)
	{
		self.mFrame.Allocate(self.mVM.lock()->GetInt32FromMemory(self.mPc + 1));
	}

	inline void Actor::CommandRelease(Actor& self)
	{
		self.mFrame.Release(self.mVM.lock()->GetInt32FromMemory(self.mPc + 1));
	}

	inline void Actor::CommandDuplicate(Actor& self)
	{
		self.mStack.Duplicate();
	}

	inline void Actor::CommandRemove(Actor& self)
	{
		self.mStack.Remove(1);
	}

	inline void Actor::CommandLoadStaticAddress(Actor& self)
	{
		const uint32_t offset = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);
		//MANA_ASSERT(offset < mana_static_memory_size);
		//mana_stack_push_pointer(&actor->mStack, &mana_static_memory[offset]);
	}

	inline void Actor::CommandLoadGlobalAddress(Actor& self)
	{
		const uint32_t offset = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);
		//MANA_ASSERT(offset < actor->mVM->file_header->size_of_global_memory);
		//mana_stack_push_pointer(&actor->mStack, &actor->mVM->global_memory[offset]);
	}

	inline void Actor::CommandLoadFrameAddress(Actor& self)
	{
		const uint32_t offset = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);
		self.mStack.Push(self.mFrame.GetAddressFromBottom<void*>(offset));
	}

	inline void Actor::CommandLoadSelfAddress(Actor& self)
	{
		uint32_t offset = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);
		self.mStack.Push(self.mVariable.GetAddressFromBottom<void*>(offset));
	}

	inline void Actor::CommandLoadInt8(Actor& self)
	{
		const int8_t* pointer = static_cast<const int8_t*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadInt16(Actor& self)
	{
		const int16_t* pointer = static_cast<const int16_t*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadInt32(Actor& self)
	{
		const int32_t* pointer = static_cast<const int32_t*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadFloat(Actor& self)
	{
		const float* pointer = static_cast<const float*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadReffrence(Actor& self)
	{
		void* pointer = static_cast<void*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, pointer);
	}

	inline void Actor::CommandStoreInt8(Actor& self)
	{
		int8_t* pointer = static_cast<int8_t*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<int8_t>(self.mStack.Get<int_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreInt16(Actor& self)
	{
		int16_t* pointer = static_cast<int16_t*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<int16_t>(self.mStack.Get<int_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreInt32(Actor& self)
	{
		int32_t* pointer = static_cast<int32_t*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<int32_t>(self.mStack.Get<int_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreFloat(Actor& self)
	{
		float* pointer = static_cast<float*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<float>(self.mStack.Get<float_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreReffrence(Actor& self)
	{
		//void* pointer = self.mStack.Get<void*>(0);
		//*pointer = self.mStack.GetAddress(1);
		self.mStack.Remove(2);
	}

	inline void Actor::CommandBranchEqual(Actor& self)
	{
		if (!self.mStack.Pop<int_t>())
		{
			self.mPc = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);
			self.mInterrupt[self.mInterruptLevel].mFlag.test(Interrupt::Flag::Repeat);

			MANA_ASSERT(self.mPc < self.mVM.lock()->mFileHeader->mSizeOfInstructionPool);
		}
	}

	inline void Actor::CommandBranchNotEqual(Actor& self)
	{
		if (self.mStack.Pop<int_t>())
		{
			self.mPc = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);
			self.mInterrupt[self.mInterruptLevel].mFlag.test(Interrupt::Flag::Repeat);

			MANA_ASSERT(self.mPc < self.mVM.lock()->mFileHeader->mSizeOfInstructionPool);
		}
	}

	inline void Actor::CommandBranchAway(Actor& self)
	{
		self.mPc = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);
		self.mInterrupt[self.mInterruptLevel].mFlag.test(Interrupt::Flag::Repeat);

		MANA_ASSERT(self.mPc < self.mVM.lock()->mFileHeader->mSizeOfInstructionPool);
	}

	inline void Actor::CommandBranchSubRoutine(Actor& self)
	{
		self.mInterrupt[self.mInterruptLevel].mReturnAddress = self.mPc + 4 + 1; // saved return program address
		self.mInterrupt[self.mInterruptLevel].mFlag.test(Interrupt::Flag::Repeat);
		self.mPc = self.mVM.lock()->GetUint32FromMemory(self.mPc + 1);

		MANA_ASSERT(self.mPc < self.mVM.lock()->mFileHeader->mSizeOfInstructionPool);
	}

	inline void Actor::CommandComply(Actor& self)
	{
		self.Accept();
	}

	inline void Actor::CommandRefuse(Actor& self)
	{
		self.Refuse();
	}

	inline void Actor::CommandLoadReturnAddress(Actor& self)
	{
		uint32_t* pointer = self.mFrame.GetAddressFromBottom<uint32_t>(sizeof(int32_t));
		self.mInterrupt[self.mInterruptLevel].mReturnAddress = *pointer;
	}

	inline void Actor::CommandStoreReturnAddress(Actor& self)
	{
		uint32_t* pointer = self.mFrame.GetAddressFromBottom<uint32_t>(sizeof(int32_t));
		*pointer = self.mInterrupt[self.mInterruptLevel].mReturnAddress;
	}

	inline void Actor::CommandReturnFromFunction(Actor& self)
	{
		self.mPc = self.mInterrupt[self.mInterruptLevel].mReturnAddress;
		self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);
	}

	inline void Actor::CommandReturnFromAction(Actor& self)
	{
		self.Rollback(self.mInterruptLevel);
	}

	inline void Actor::CommandRollback(Actor& self)
	{
		self.Rollback(self.mStack.Pop<int_t>());
	}

	inline void Actor::CommandAddInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left + right);
	}

	inline void Actor::CommandAddFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left + right);
	}

	inline void Actor::CommandDivideInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left / right);
	}

	inline void Actor::CommandDivideFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left / right);
	}

	inline void Actor::CommandMinusInteger(Actor& self)
	{
		self.mStack.Set(0, -self.mStack.Get<int_t>(0));
	}

	inline void Actor::CommandMinusFloat(Actor& self)
	{
		self.mStack.Set(0, -self.mStack.Get<float_t>(0));
	}

	inline void Actor::CommandModInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left % right);
	}

	inline void Actor::CommandModfloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, std::fmodf(left, right));
	}

	inline void Actor::CommandMultiplyInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left * right);
	}

	inline void Actor::CommandMultiplyFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left * right);
	}

	inline void Actor::CommandSubtractInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left - right);
	}

	inline void Actor::CommandSubtractFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left - right);
	}

	inline void Actor::CommandAnd(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left & right);
	}

	inline void Actor::CommandExclusiveOr(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left ^ right);
	}

	inline void Actor::CommandLogicalAnd(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left && right);
	}

	inline void Actor::CommandLogicalOr(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left || right);
	}

	inline void Actor::CommandNot(Actor& self)
	{
		self.mStack.Set(0, ~self.mStack.Get<int_t>(0));
	}

	inline void Actor::Commandlogical_not(Actor& self)
	{
		self.mStack.Set(0, !self.mStack.Get<int_t>(0));
	}

	inline void Actor::CommandOr(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left | right);
	}

	inline void Actor::CommandShiftLeft(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left << right);
	}

	inline void Actor::CommandShiftRight(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left >> right);
	}

	inline void Actor::CommandCompareEqualInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left == right);
	}

	inline void Actor::CommandCompareEqualFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left == right);
	}

	inline void Actor::CommandCompareGreaterEqualInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left >= right);
	}

	inline void Actor::CommandCompareGreaterEqualFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left >= right);
	}

	inline void Actor::CommandCompareGreaterInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left > right);
	}

	inline void Actor::CommandCompareGreaterFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left > right);
	}

	inline void Actor::CommandCompareNotEqualInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left != right);
	}

	inline void Actor::CommandCompareNotEqualFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left != right);
	}

	inline void Actor::CommandCompareLessEqualInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left <= right);
	}

	inline void Actor::CommandCompareLessEqualFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left <= right);
	}

	inline void Actor::CommandCompareLessInteger(Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left < right);
	}

	inline void Actor::CommandCompareLessFloat(Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left < right);
	}

	inline void Actor::CommandIntegerToFloat(Actor& self)
	{
		self.mStack.Set(0, static_cast<float_t>(self.mStack.Get<int_t>(0)));
	}

	inline void Actor::CommandFloatToInteger(Actor& self)
	{
		self.mStack.Set(0, static_cast<int_t>(self.mStack.Get<float_t>(0)));
	}

	inline void Actor::CommandPushActor(Actor& self)
	{
		const char* pszActorName = self.mVM.lock()->GetStringFromMemory(self.mPc + 1);
		MANA_ASSERT(pszActorName);

		const std::shared_ptr<Actor>& targetActor = self.mVM.lock()->GetActor(pszActorName);
		self.mStack.Push(targetActor.get());
	}

	inline void Actor::CommandLoadData(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1);
		const void* pointer = self.mStack.Pop<void*>();
		self.mStack.Push(pointer, size);
	}

	inline void Actor::CommandStoreData(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1);
		void* pointer = self.mStack.Pop<void*>();
		self.mStack.PopData(pointer, size);
	}

	inline void Actor::CommandDuplicateData(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1);
		void* pointer = self.mStack.GetAddress(size / sizeof(void*));
		self.mStack.Push(pointer, size);
	}

	inline void Actor::CommandRemoveData(Actor& self)
	{
		self.mStack.Remove(self.mVM.lock()->GetInt32FromMemory(self.mPc + 1) / sizeof(void*));
	}

	inline void Actor::CommandCompareEqualData(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) == 0);
	}

	inline void Actor::CommandCompareGreaterEqualData(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) >= 0);
	}

	inline void Actor::CommandCompareGreaterdata(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) > 0);
	}

	inline void Actor::CommandCompareNotEqualData(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) != 0);
	}

	inline void Actor::CommandCompareLessEqualData(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) <= 0);
	}

	inline void Actor::CommandCompareLessdata(Actor& self)
	{
		const int32_t size = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) < 0);
	}

	inline void Actor::CommandCall(Actor& self)
	{
		uint32_t last_pc = self.mPc;
		int32_t last_interrupt_level = self.mInterruptLevel;

		// 外部関数の実行
		const char* functionName = self.mVM.lock()->GetStringFromMemory(self.mPc + 1);
		VM::ExternalFuntionType function = self.mVM.lock()->FindFunction(functionName);
		function(self.shared_from_this());

		if (self.IsRunning())
		{
			if (!self.IsRepeat())
			{
				bool bHasReturnValue = self.HasReturnValue(last_pc);
				int32_t nNumberOfArguments = self.GetArgumentCountByAddress(last_pc);
				int32_t nSizeOfArguments = self.GetArgumentSize(last_pc);

				// スタックに入っているパラメータをpopする
				self.mStack.Remove(nSizeOfArguments);

				// 戻り値があるなら、戻り値をpush
				if (bHasReturnValue)
				{
					switch (self.mReturnValue.mSize)
					{
					case MANA_RETURN_VALUE_TYPE_INVALID:
						break;

					case MANA_RETURN_VALUE_TYPE_ACTOR:
						self.mStack.Push(self.mReturnValue.mValues.mActorValue);
						break;

					case MANA_RETURN_VALUE_TYPE_INTEGER:
						self.mStack.Push(self.mReturnValue.mValues.mIntegerValue);
						break;

					case MANA_RETURN_VALUE_TYPE_FLOAT:
						self.mStack.Push(self.mReturnValue.mValues.mFloatValue);
						break;

					case MANA_RETURN_VALUE_TYPE_STRING:
						self.mStack.Push(self.mReturnValue.mValues.mStringValue);
						break;

					default:
						self.mStack.Push(self.mReturnValue.mValues.mPointerValue, self.mReturnValue.mSize);

						std::free(self.mReturnValue.mValues.mPointerValue);
						self.mReturnValue.mValues.mPointerValue = nullptr;
						break;
					}
				}

				// 外部関数内でreq系の命令が実行された場合、スタックの状態を修正する
				if (self.mInterruptLevel > last_interrupt_level)
				{
					self.mInterrupt[last_interrupt_level].mAddress = last_pc + 4 + 2 + 2 + 1 + (nNumberOfArguments * sizeof(int16_t));
					self.mInterrupt[self.mInterruptLevel].mStackPointer = self.mStack.GetSize();
					self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);
				}
			}
		}
	}

	inline void Actor::CommandRequest(Actor& self)
	{
		Actor* target_actor = self.mStack.Pop<Actor*>();
		int_t level = self.mStack.Pop<int_t>();
		const char* action = self.mVM.lock()->GetStringFromMemory(self.mPc + 1);

		if (target_actor)
		{
			self.mInterrupt[self.mInterruptLevel].mFlag = Interrupt::Flag::Repeat;
			self.mPc += sizeof(int32_t) + 1;
			target_actor->Request(level, action, self.shared_from_this());
		}
	}

	inline void Actor::CommandRequestWaitStarting(Actor& self)
	{
		Actor* target_actor = (Actor*)self.mStack.Get<void*>(0);
		int_t level = self.mStack.Get<int_t>(1);
		const char* action = self.mVM.lock()->GetStringFromMemory(self.mPc + 1);

		if (target_actor == nullptr)
		{
			self.mStack.Remove(2);
			return;
		}

		if (&self == target_actor)
		{
			self.mStack.Remove(2);
			return;
		}

		if (self.IsInit() && !target_actor->Request(level, action, self.shared_from_this()))
		{
			self.mStack.Remove(2);
			return;
		}

		if (target_actor->mInterruptLevel <= level)
		{
			self.mStack.Remove(2);
		}
		else
		{
			self.Repeat(true);
		}
	}

	inline void Actor::CommandRequestWaitEnding(Actor& self)
	{
		Actor* target_actor = (Actor*)self.mStack.Get<void*>(0);
		int_t level = self.mStack.Get<int_t>(1);
		const char* action = self.mVM.lock()->GetStringFromMemory(self.mPc + 1);

		if (target_actor == 0)
		{
			self.mStack.Remove(2);
			return;
		}

		if (&self == target_actor)
		{
			self.mStack.Remove(2);
			return;
		}

		if (self.IsInit() && !target_actor->Request(level, action, self.shared_from_this()))
		{
			self.mStack.Remove(2);
			return;
		}

		if (target_actor->mInterruptLevel < level)
		{
			self.mStack.Remove(2);
		}
		else
		{
			self.Repeat(true);
		}
	}

	inline void Actor::Commanddynamic_request(Actor& self)
	{
		/*
		CManaStack& Stack = actor->self.mStack;
		const char* pszTarget, = Stack.PopString();
		const int32_t level = Stack.PopInteger();
		const char* action = actor->GetStringFromMemory(1);

		actor->self.mInterrupt[actor->GetInterruptLevel()].repeat = true;
		actor->self.mPc += sizeof(int32_t) + 1;

		Actor* target_actor = actor->GetParent().GetActor(pszTarget,);
		if(target_actor)
		target_actor->Request(level, action, actor);
		*/
	}

	inline void Actor::CommandDynamicRequestWaitStarting(Actor& self)
	{
		/*
		CManaStack& Stack = actor->self.mStack;
		const char* pszTarget, = Stack.GetString(0);
		int32_t level = Stack.Get<int_t>(1);
		const char* action = actor->GetStringFromMemory(1);

		Actor* target_actor = actor->GetParent().GetActor(pszTarget,);
		if(target_actor == 0)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor == target_actor)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor->IsInit() && !target_actor->Request(level, action, actor))
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(target_actor->GetInterruptLevel() <= level)
		{
		actor->self.mStack.Remove(2);
		}else{
		actor->Repeat();
		}
		*/
	}

	inline void Actor::Commanddynamic_request_wait_ending(Actor& self)
	{
		/*
		CManaStack& Stack = actor->self.mStack;
		const char* pszTarget, = Stack.GetString(0);
		int32_t level = Stack.Get<int_t>(1);
		const char* action = actor->GetStringFromMemory(1);

		Actor* target_actor = actor->GetParent().GetActor(pszTarget,);
		if(target_actor == 0)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor == target_actor)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor->IsInit() && !target_actor->Request(level, action, actor))
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(target_actor->self.mInterruptLevel < level)
		{
		actor->self.mStack.Remove(2);
		}else{
		actor->Repeat();
		}
		*/
	}

	inline void Actor::CommandJoin(Actor& self)
	{
		Actor* target_actor = (Actor*)self.mStack.Get<void*>(0);
		if (target_actor == nullptr)
		{
			self.mStack.Remove(2);
			return;
		}

		if (target_actor->mInterruptLevel < self.mStack.Get<int_t>(1))
		{
			self.mStack.Remove(2);
		}
		else
		{
			self.Repeat(true);
		}
	}

	inline void Actor::CommandPrint(Actor& self)
	{
		char message[1024];

		int32_t message_pointer = 0;
		int32_t number_of_arguments = self.mVM.lock()->GetInt32FromMemory(self.mPc + 1);

		if (number_of_arguments > 0)
		{
			int32_t counter = 1;
			char* format;

			for (format = static_cast<char*>(self.mStack.Get<void*>(0)); *format; format++)
			{
				if (*format == '%')
				{
					if (counter > number_of_arguments)
					{
						message[message_pointer++] = '\n';
						break;
					}

					format++;

					switch (*format)
					{
					case 'd':
#if __STDC_WANT_SECURE_LIB__
						message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, "%d", self.mStack.Get<int_t>(counter++));
#else
						message_pointer += sprintf(&message[message_pointer], "%lld", self.mStack.Get<int_t>(counter++));
#endif
						break;

					case 'f':
#if __STDC_WANT_SECURE_LIB__
						message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, "%f", self.mStack.Get<float_t>(counter++));
#else
						message_pointer += sprintf(&message[message_pointer], "%f", self.mStack.Get<float_t>(counter++));
#endif
						break;

					case 's':
#if __STDC_WANT_SECURE_LIB__
						message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, self.mStack.Get<const char*>(counter++));
#else
						message_pointer += sprintf(&message[message_pointer], "%s", self.mStack.Get<const char*>(counter++));
#endif
						break;

					case 'p':
#if __STDC_WANT_SECURE_LIB__
						message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, "%p", self.mStack.Get<void*>(counter++));
#else
						message_pointer += sprintf(&message[message_pointer], "%p", self.mStack.Get<void*>(counter++));
#endif
						break;

					default:
						message[message_pointer] = *format;
						message_pointer++;
						break;
					}
				}
				else
				{
					message[message_pointer] = *format;
					message_pointer++;
				}
			}
		}
		message[message_pointer] = '\0';

		MANA_PRINT(message);

		self.mStack.Remove(number_of_arguments);
	}
}

#if MANA_BUILD_TARGET > MANA_BUILD_DEBUG
/*! 引数の数を調べ、一致しない場合はreturnします */
#define MANA_ASSERT_PARAMETER(P, I) {									\
	if(P->GetArgumentCount() != I)							\
		return;															\
}
/*! initアクション中ならばreturnします */
#define MANA_ASSERT_ILLIGAL_CALL_IN_INIT_ACTION(P) {	\
	if(P->GetVirtualMachine()->IsInInitAction()){		\
		return;											\
}
#else
/*! 引数の数を調べ、一致しない場合は警告を表示してreturnします */
#if 1
#define MANA_ASSERT_PARAMETER(P, I)	\
	if(P->GetArgumentCount() != I){	\
		 MANA_PRINT("ERROR: %s: function %s number of arguments %d correct%d\n", P->GetName(), P->GetFunctionName(), P->GetArgumentCount(), I);\
		 return;					\
	}
/*! initアクション中ならば警告を表示してreturnします */
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)			\
	if(P->GetVirtualMachine()->IsInInitAction()){		\
		MANA_PRINT("ERROR: %s: init action %s can not call\n", P->GetName(), P->GetFunctionName());\
		return;											\
	}
#else
#define MANA_ASSERT_PARAMETER(P, I)										\
	if(GetArgumentCount(P) != I){							\
		MANA_PRINT("ERROR: %s: 関数%sの引数の数が%dです。正しい引数の数は%dです。\n", P->GetName(P), GetFunctionName(P), GetArgumentCount(P), I);\
		return;															\
	}
/*! initアクション中ならば警告を表示してreturnします */
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)							\
	if(mana_is_in_init_action(P->parent)){								\
		MANA_PRINT("ERROR: %s: initアクション内で %s は使えません。\n", GetName(P), GetFunctionName(P));\
		return;															\
	}
#endif
#endif
