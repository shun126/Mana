/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Actor.h"
#include "VM.h"
#include "common/FileFormat.h"

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

#if 0	
	inline void Actor::Serialize(mana_stream* stream)
	{
		int32_t i;

		MANA_ASSERT(self && stream);

		mana_frame_serialize(&mFrame, stream);
		mana_stack_serialize(&mStack, stream);

		mana_stream_mark(stream);

		for (i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			Interrupt* p = &mInterrupt[i];

			const char* sender_name = mana_get_actor_name(mVM, p->mSender);
			if (sender_name)
			{
				mana_stream_push_char(stream, 1);
				mana_stream_push_string(stream, sender_name);
			}
			else {
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

		for (int32_t i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			Interrupt* p = &mInterrupt[i];

			if (mana_stream_pop_char(stream))
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
		using IntermediateLanguageFunction = void(*)(const std::shared_ptr<VM>& vm, Actor& self);
		struct IntermediateLanguageCommand final
		{
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			IntermediateLanguage mCode;
#endif
			IntermediateLanguageFunction mCommand;
		};
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
#define MANA_ACTOR_SET_COMMAND(code, command)	code, command
#else
#define MANA_ACTOR_SET_COMMAND(code, command)	command
#endif
		static const std::array<IntermediateLanguageCommand, IntermediateLanguageSize> mIntermediateLanguage = {
			// thread
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_RESTART, &CommandRestart),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_HALT, &CommandHalt),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_YIELD, &CommandYield),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_NONPREEMPTIVE, &CommandSetNonPreemptive),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PREEMPTIVE, &CommandSetPreemptive),

			// jump
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_BEQ, &CommandBranchEqual),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_BNE, &CommandBranchNotEqual),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_BRA, &CommandBranchAway),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_BSR, &CommandBranchSubRoutine),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_CALL, &CommandCall),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_REQ, &CommandRequest),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_REQWS, &CommandRequestWaitStarting),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_REQWE, &CommandRequestWaitEnding),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_JOIN, &CommandJoin),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPLY, &CommandComply),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_REFUSE, &CommandRefuse),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_RETURN_ADDRESS, &CommandLoadReturnAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_SAVE_RETURN_ADDRESS, &CommandStoreReturnAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_RETURN_FROM_FUNCTION, &CommandReturnFromFunction),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_RETURN_FROM_ACTION, &CommandReturnFromAction),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_ROLLBACK, &CommandRollback),

			// constant
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_ZERO_INTEGER, &CommandPushZeroInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_ZERO_FLOAT, &CommandPushZeroFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_CHAR, &CommandPushChar),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_SHORT, &CommandPushShort),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_INTEGER, &CommandPushInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_SIZE, &CommandPushInteger),	// TODO:サイズに対応して下さい
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_FLOAT, &CommandPushFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_STRING, &CommandPushString),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_PRIORITY, &CommandPushPriority),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_ACTOR, &CommandPushActor),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_SELF, &CommandPushSelf),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PUSH_SENDER, &CommandPushSender),

			// stack
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_ALLOCATE, &CommandAllocate),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_FREE, &CommandRelease),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_DUPLICATE, &CommandDuplicate),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_DUPLICATE_DATA, &CommandDuplicateData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_REMOVE, &CommandRemove),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_REMOVE_DATA, &CommandRemoveData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_STATIC_ADDRESS, &CommandLoadStaticAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_GLOBAL_ADDRESS, &CommandLoadGlobalAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_FRAME_ADDRESS, &CommandLoadFrameAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_SELF_ADDRESS, &CommandLoadSelfAddress),

			// memory operation
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_CHAR, &CommandLoadInt8),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_SHORT, &CommandLoadInt16),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_INTEGER, &CommandLoadInt32),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_FLOAT, &CommandLoadFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_REFFRENCE, &CommandLoadReffrence),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOAD_DATA, &CommandLoadData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_STORE_CHAR, &CommandStoreInt8),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_STORE_SHORT, &CommandStoreInt16),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_STORE_INTEGER, &CommandStoreInt32),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_STORE_FLOAT, &CommandStoreFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_STORE_REFFRENCE, &CommandStoreReffrence),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_STORE_DATA, &CommandStoreData),

			// caluclation
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_ADD_INTEGER, &CommandAddInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_DIV_INTEGER, &CommandDivideInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_MINUS_INTEGER, &CommandMinusInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_MOD_INTEGER, &CommandModInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_MUL_INTEGER, &CommandMultiplyInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_SUB_INTEGER, &CommandSubtractInteger),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_ADD_FLOAT, &CommandAddFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_DIV_FLOAT, &CommandDivideFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_MINUS_FLOAT, &CommandMinusFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_MOD_FLOAT, &CommandModfloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_MUL_FLOAT, &CommandMultiplyFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_SUB_FLOAT, &CommandSubtractFloat),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_AND, &CommandAnd),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_EOR, &CommandExclusiveOr),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LAND, &CommandLogicalAnd),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LOR, &CommandLogicalOr),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_LNOT, &Commandlogical_not),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_NOT, &CommandNot),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_OR, &CommandOr),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_SHL, &CommandShiftLeft),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_SHR, &CommandShiftRight),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_INT2FLOAT, &CommandIntegerToFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_FLOAT2INT, &CommandFloatToInteger),

			// compare and branch
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_EQ_INTEGER, &CommandCompareEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_NE_INTEGER, &CommandCompareNotEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_GE_INTEGER, &CommandCompareGreaterEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_GT_INTEGER, &CommandCompareGreaterInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_LE_INTEGER, &CommandCompareLessEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_LS_INTEGER, &CommandCompareLessInteger),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_EQ_FLOAT, &CommandCompareEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_NE_FLOAT, &CommandCompareNotEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_GE_FLOAT, &CommandCompareGreaterEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_GT_FLOAT, &CommandCompareGreaterFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_LE_FLOAT, &CommandCompareLessEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_LS_FLOAT, &CommandCompareLessFloat),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_EQ_DATA, &CommandCompareEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_NE_DATA, &CommandCompareNotEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_GE_DATA, &CommandCompareGreaterEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_GT_DATA, &CommandCompareGreaterdata),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_LE_DATA, &CommandCompareLessEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_COMPARE_LS_DATA, &CommandCompareLessdata),

			// utility
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_PRINT, &CommandPrint),

			// under discussion
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_DYNAMIC_REQ, &Commanddynamic_request),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_DYNAMIC_REQWS, &CommandDynamicRequestWaitStarting),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MANA_IL_DYNAMIC_REQWE, &Commanddynamic_request_wait_ending),
		};
#undef MANA_ACTOR_SET_COMMAND

		std::shared_ptr<VM> vm = mVM.lock();

		size_t timer = 0;
		do {
			mFlag.reset(Flag::REQUESTED);

			if (!IsRunning())
				return false;
			
			printf("%x\n", mPc);

			const int32_t opecode = vm->GetOpecode(mPc);
			const IntermediateLanguageCommand& command = mIntermediateLanguage.at(opecode);
			MANA_ASSERT(static_cast<IntermediateLanguage>(opecode) == command.mCode);
			MANA_ASSERT(command.mCommand);
			command.mCommand(vm, *this);

			if (!IsRunning())
				return false;

			if (IsRepeat())
			{
				mInterrupt[mInterruptLevel].mFlag.reset(Actor::Interrupt::Flag::Repeat);
			}
			else
			{
				mPc += GetInstructionSize(mVM.lock()->mInstructionPool, mPc);
				mInterrupt[mInterruptLevel].mFlag.reset(Actor::Interrupt::Flag::Initialized);
			}

			if (mInterrupt[mInterruptLevel].mFlag[Actor::Interrupt::Flag::Suspend] && !IsSynchronized())
			{
				mInterrupt[mInterruptLevel].mFlag.reset(Actor::Interrupt::Flag::Suspend);
				break;
			}
		} while (++timer < 500 || IsSynchronized());

		return mFlag[Flag::RUNNING];
	}

	inline bool Actor::SyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if (Request(level, action, sender))
		{
			mInterrupt[level].mFlag.set(Actor::Interrupt::Flag::IsInSyncCall);
			while (true)
			{
				mVM.lock()->Run();
				if (mInterruptLevel < level)
					return true;
			}
		}
		return false;
	}

	inline bool Actor::AsyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if (Request(level, action, sender))
		{
			mInterrupt[level].mFlag.set(Actor::Interrupt::Flag::IsInSyncCall);
			while (true)
			{
				Run();
				if (mInterruptLevel < level)
					return true;
			}
		}
		return false;
	}

	inline bool Actor::Request(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if (level < 0 || level >= MANA_ACTOR_MAX_INTERRUPT_LEVEL)
		{
			MANA_TRACE("MANA: %s::%s request failed. reason: level %d range over\n", GetName().data(), action, level);
			return false;
		}

		if (mFlag[Flag::HALT])
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. reason: halt\n", level, GetName().data(), action);
			return false;
		}

		if (mFlag[Flag::REFUSED])
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. reason: refuse\n", level, GetName().data(), action);
			return false;
		}

		if (mInterrupt[level].mAddress != nil)
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. ", level, GetName().data(), action);
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			if (mInterrupt[level].mActionName)
				MANA_TRACE("reason : %s running", mInterrupt[level].mActionName);
#endif
			MANA_TRACE("\n");
			return false;
		}

		uint32_t address = GetAction(action);
		if (address == nil)
		{
			MANA_TRACE("MANA: level %d, %s::%s request failed. reason: not found\n", level, GetName().data(), action);
			return false;
		}

		mFlag.set(Flag::RUNNING);
		mInterrupt[level].mCounter = 0;
		mInterrupt[level].mSender = sender;
		mInterrupt[level].mAddress = address;
		mInterrupt[level].mFlag.reset(Actor::Interrupt::Flag::Synchronized);
		mInterrupt[level].mFileCallbackParameter = nullptr;

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		MANA_TRACE("Mana:request: %s ", GetName().data());
		if (mInterrupt[mInterruptLevel].mActionName)
			MANA_TRACE("level %d %s => ", mInterruptLevel, mInterrupt[mInterruptLevel].mActionName);
		MANA_TRACE("level %d %s (%08x) registered\n", level, action, address);

		// 実行するアクション名を記録
		mInterrupt[level].mActionName = action;
#endif

		if (level >= mInterruptLevel)
		{
			MANA_TRACE("mana:request: %s ", GetName().data());
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
		if (mInterrupt[mInterruptLevel].mSender)
		{
			mInterrupt[mInterruptLevel].mSender->Again();
			mInterrupt[mInterruptLevel].mSender = nullptr;
		}
#if 0
		/* ファイル読み込み中ならば解放 */
		if (mAsyncFileCallback && mInterrupt[mInterruptLevel].mFileCallbackParameter)
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
		if (inSyncCall)
		{
			mInterrupt[mInterruptLevel].mFlag.reset(Interrupt::Flag::IsInSyncCall);
		}

		// FPとSPを取得
		size_t framePointer = mInterrupt[mInterruptLevel].mFramePointer;
		size_t stackPointer = mInterrupt[mInterruptLevel].mStackPointer;

		// 優先度(高いほど優先)が指定されているならば、強制的に指定優先度まで戻る
		int32_t currentLevel = mInterruptLevel - 1;
		if (currentLevel > level)
		{
			while (currentLevel > level)
			{
				Interrupt* interrupt = &mInterrupt[currentLevel];

				// 優先度開放
				interrupt->mAddress = nil;
#if 0
				/* ファイルエントリの削除 */
				if (mAsyncFileCallback && mInterrupt->mFileCallbackParameter)
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
				if (mRollbackCallback)
					mRollbackCallback(mRollbackCallbackParameter);
#endif			
				--currentLevel;
			}
		}

		// FPとSPを復元します
		mFrame.SetSize(framePointer);
		mStack.SetSize(stackPointer);

		// 中断されたアクションの再開
		while (currentLevel >= 0)
		{
			Interrupt* interrupt = &mInterrupt[currentLevel];
			if (interrupt->mAddress != ~0)
			{
				// 中断していた場所から復帰させます
				mPc = interrupt->mAddress;
#if 0
				// コールバック関数を呼びます
				if (mRollbackCallback)
					mRollbackCallback(mRollbackCallbackParameter);
#endif			
				// 優先度(高いほど優先)変更
				mInterruptLevel = static_cast<uint8_t>(currentLevel);

				// 次回のTickでプログラムカウンターを加算しない
				mInterrupt[mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);

				// SyncCall中ならばTickを抜ける
				if (inSyncCall)
					yield();

				MANA_TRACE(
					"mana:rollback: %s level %d %s => level %d %s succeed\n",
					GetName().data(),
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
		MANA_TRACE("mana:rollback: %s level %d %s stoped\n", GetName().data(), lastInterruptLevel, lastActionName);
	}

	inline void Actor::Restart()
	{
		mPc = nil;
		mInterruptLevel = 0;
		mFlag.reset(Flag::HALT);
		mFlag.reset(Flag::RUNNING);
		mFlag.reset(Flag::REFUSED);

		memset(&mInterrupt, 0, sizeof(mInterrupt));
		for (uint32_t i = 0; i < MANA_ACTOR_MAX_INTERRUPT_LEVEL; i++)
		{
			mInterrupt[i].mAddress = nil;
			mInterrupt[i].mReturnAddress = nil;
		}

		mFrame.Clear();
		mStack.Clear();
	}

	inline const std::string_view& Actor::GetName()
	{
		return mVM.lock()->GetActorName(shared_from_this());
	}

	inline uint32_t Actor::GetAction(const std::string_view& actionName) const
	{
		const auto i = mActions.find(actionName);
		return (i == mActions.end()) ? nil : i->second;
	}

	inline void Actor::SetAction(const std::string_view& actionName, const uint32_t address)
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
		if (size > 0)
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
		if (initial_complete)
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
		if (synchronized)
			mInterrupt[mInterruptLevel].mFlag.set(Interrupt::Flag::Synchronized);
		else
			mInterrupt[mInterruptLevel].mFlag.reset(Interrupt::Flag::Synchronized);
	}

	inline void Actor::SetSynchronizedWithLevel(const size_t level, const bool synchronized)
	{
		MANA_ASSERT(level >= 0 && level < MANA_ACTOR_MAX_INTERRUPT_LEVEL);
		if (synchronized)
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

	inline void Actor::CommandRestart(const std::shared_ptr<VM>&, Actor& self)
	{
		self.Restart();
	}

	inline void Actor::CommandHalt(const std::shared_ptr<VM>&, Actor& self)
	{
		self.Halt();
	}

	inline void Actor::CommandYield(const std::shared_ptr<VM>&, Actor& self)
	{
		self.yield();
	}

	inline void Actor::CommandSetNonPreemptive(const std::shared_ptr<VM>&, Actor& self)
	{
		self.SetSynchronized(true);
	}

	inline void Actor::CommandSetPreemptive(const std::shared_ptr<VM>&, Actor& self)
	{
		self.SetSynchronized(false);
	}

	inline void Actor::CommandPushZeroInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Push<int_t>(0);
	}

	inline void Actor::CommandPushZeroFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Push(static_cast<float_t>(0.0));
	}

	inline void Actor::CommandPushChar(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int8_t value = vm->GetInt8FromMemory(self.mPc + 1);
		self.mStack.Push<int_t>(value);
	}

	inline void Actor::CommandPushShort(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int16_t value = vm->GetInt16FromMemory(self.mPc + 1);
		self.mStack.Push<int_t>(value);
	}

	inline void Actor::CommandPushInteger(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mStack.Push<int_t>(vm->GetInt32FromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushFloat(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mStack.Push(vm->GetFloatFromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushString(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mStack.Push(vm->GetStringFromMemory(self.mPc + 1));
	}

	inline void Actor::CommandPushPriority(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Push<int_t>(self.mInterruptLevel);
	}

	inline void Actor::CommandPushSelf(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Push(&self);
	}

	inline void Actor::CommandPushSender(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Push(self.mInterrupt[self.mInterruptLevel].mSender.get());
	}

	inline void Actor::CommandAllocate(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1);
		self.mFrame.Allocate(size);
	}

	inline void Actor::CommandRelease(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1);
		self.mFrame.Release(size);
	}

	inline void Actor::CommandDuplicate(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Duplicate();
	}

	inline void Actor::CommandRemove(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Remove(1);
	}

	inline void Actor::CommandLoadStaticAddress(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		//MANA_ASSERT(offset < mana_static_memory_size);
		//mana_stack_push_pointer(&actor->mStack, &mana_static_memory[offset]);
	}

	inline void Actor::CommandLoadGlobalAddress(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		//MANA_ASSERT(offset < actor->mVM->file_header->size_of_global_memory);
		//mana_stack_push_pointer(&actor->mStack, &actor->mVM->global_memory[offset]);
	}

	inline void Actor::CommandLoadFrameAddress(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		self.mStack.Push(self.mFrame.GetAddressFromBottom<void*>(offset));
	}

	inline void Actor::CommandLoadSelfAddress(const std::shared_ptr<VM>& vm, Actor& self)
	{
		uint32_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		self.mStack.Push(self.mVariable.GetAddressFromBottom<void*>(offset));
	}

	inline void Actor::CommandLoadInt8(const std::shared_ptr<VM>&, Actor& self)
	{
		const int8_t* pointer = static_cast<const int8_t*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadInt16(const std::shared_ptr<VM>&, Actor& self)
	{
		const int16_t* pointer = static_cast<const int16_t*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadInt32(const std::shared_ptr<VM>&, Actor& self)
	{
		const int32_t* pointer = static_cast<const int32_t*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float* pointer = static_cast<const float*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, *pointer);
	}

	inline void Actor::CommandLoadReffrence(const std::shared_ptr<VM>&, Actor& self)
	{
		void* pointer = static_cast<void*>(self.mStack.Get<void*>(0));
		self.mStack.Set(0, pointer);
	}

	inline void Actor::CommandStoreInt8(const std::shared_ptr<VM>&, Actor& self)
	{
		int8_t* pointer = static_cast<int8_t*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<int8_t>(self.mStack.Get<int_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreInt16(const std::shared_ptr<VM>&, Actor& self)
	{
		int16_t* pointer = static_cast<int16_t*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<int16_t>(self.mStack.Get<int_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreInt32(const std::shared_ptr<VM>&, Actor& self)
	{
		int32_t* pointer = static_cast<int32_t*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<int32_t>(self.mStack.Get<int_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		float* pointer = static_cast<float*>(self.mStack.Get<void*>(0));
		*pointer = static_cast<float>(self.mStack.Get<float_t>(1));
		self.mStack.Remove(2);
	}

	inline void Actor::CommandStoreReffrence(const std::shared_ptr<VM>&, Actor& self)
	{
		//void* pointer = self.mStack.Get<void*>(0);
		//*pointer = self.mStack.GetAddress(1);
		self.mStack.Remove(2);
	}

	inline void Actor::CommandBranchEqual(const std::shared_ptr<VM>& vm, Actor& self)
	{
		if (!self.mStack.Pop<int_t>())
		{
			self.mPc = vm->GetUint32FromMemory(self.mPc + 1);
			self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);

			MANA_ASSERT(self.mPc < vm->mFileHeader->mSizeOfInstructionPool);
		}
	}

	inline void Actor::CommandBranchNotEqual(const std::shared_ptr<VM>& vm, Actor& self)
	{
		if (self.mStack.Pop<int_t>())
		{
			self.mPc = vm->GetUint32FromMemory(self.mPc + 1);
			self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);

			MANA_ASSERT(self.mPc < vm->mFileHeader->mSizeOfInstructionPool);
		}
	}

	inline void Actor::CommandBranchAway(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mPc = vm->GetUint32FromMemory(self.mPc + 1);
		self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);

		MANA_ASSERT(self.mPc < vm->mFileHeader->mSizeOfInstructionPool);
	}

	inline void Actor::CommandBranchSubRoutine(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mInterrupt[self.mInterruptLevel].mReturnAddress = self.mPc + 4 + 1; // saved return program address
		self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);
		self.mPc = vm->GetUint32FromMemory(self.mPc + 1);

		MANA_ASSERT(self.mPc < vm->mFileHeader->mSizeOfInstructionPool);
	}

	inline void Actor::CommandComply(const std::shared_ptr<VM>&, Actor& self)
	{
		self.Accept();
	}

	inline void Actor::CommandRefuse(const std::shared_ptr<VM>&, Actor& self)
	{
		self.Refuse();
	}

	inline void Actor::CommandLoadReturnAddress(const std::shared_ptr<VM>&, Actor& self)
	{
		uint32_t* pointer = self.mFrame.GetAddressFromBottom<uint32_t>(sizeof(int32_t));
		self.mInterrupt[self.mInterruptLevel].mReturnAddress = *pointer;
	}

	inline void Actor::CommandStoreReturnAddress(const std::shared_ptr<VM>&, Actor& self)
	{
		uint32_t* pointer = self.mFrame.GetAddressFromBottom<uint32_t>(sizeof(int32_t));
		*pointer = self.mInterrupt[self.mInterruptLevel].mReturnAddress;
	}

	inline void Actor::CommandReturnFromFunction(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mPc = self.mInterrupt[self.mInterruptLevel].mReturnAddress;
		self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);
	}

	inline void Actor::CommandReturnFromAction(const std::shared_ptr<VM>&, Actor& self)
	{
		self.Rollback(self.mInterruptLevel);
	}

	inline void Actor::CommandRollback(const std::shared_ptr<VM>&, Actor& self)
	{
		self.Rollback(self.mStack.Pop<int_t>());
	}

	inline void Actor::CommandAddInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left + right);
	}

	inline void Actor::CommandAddFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left + right);
	}

	inline void Actor::CommandDivideInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left / right);
	}

	inline void Actor::CommandDivideFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left / right);
	}

	inline void Actor::CommandMinusInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Set(0, -self.mStack.Get<int_t>(0));
	}

	inline void Actor::CommandMinusFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Set(0, -self.mStack.Get<float_t>(0));
	}

	inline void Actor::CommandModInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left % right);
	}

	inline void Actor::CommandModfloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, std::fmod(left, right));
	}

	inline void Actor::CommandMultiplyInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left * right);
	}

	inline void Actor::CommandMultiplyFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left * right);
	}

	inline void Actor::CommandSubtractInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left - right);
	}

	inline void Actor::CommandSubtractFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left - right);
	}

	inline void Actor::CommandAnd(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left & right);
	}

	inline void Actor::CommandExclusiveOr(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left ^ right);
	}

	inline void Actor::CommandLogicalAnd(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left && right);
	}

	inline void Actor::CommandLogicalOr(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left || right);
	}

	inline void Actor::CommandNot(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Set(0, ~self.mStack.Get<int_t>(0));
	}

	inline void Actor::Commandlogical_not(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Set(0, !self.mStack.Get<int_t>(0));
	}

	inline void Actor::CommandOr(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left | right);
	}

	inline void Actor::CommandShiftLeft(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left << right);
	}

	inline void Actor::CommandShiftRight(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left >> right);
	}

	inline void Actor::CommandCompareEqualInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left == right);
	}

	inline void Actor::CommandCompareEqualFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left == right);
	}

	inline void Actor::CommandCompareGreaterEqualInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left >= right);
	}

	inline void Actor::CommandCompareGreaterEqualFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left >= right);
	}

	inline void Actor::CommandCompareGreaterInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left > right);
	}

	inline void Actor::CommandCompareGreaterFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left > right);
	}

	inline void Actor::CommandCompareNotEqualInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left != right);
	}

	inline void Actor::CommandCompareNotEqualFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left != right);
	}

	inline void Actor::CommandCompareLessEqualInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left <= right);
	}

	inline void Actor::CommandCompareLessEqualFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left <= right);
	}

	inline void Actor::CommandCompareLessInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		const int_t left = self.mStack.Get<int_t>(1);
		const int_t right = self.mStack.Get<int_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left < right);
	}

	inline void Actor::CommandCompareLessFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		const float_t left = self.mStack.Get<float_t>(1);
		const float_t right = self.mStack.Get<float_t>(0);
		self.mStack.Remove(1);
		self.mStack.Set(0, left < right);
	}

	inline void Actor::CommandIntegerToFloat(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Set(0, static_cast<float_t>(self.mStack.Get<int_t>(0)));
	}

	inline void Actor::CommandFloatToInteger(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Set(0, static_cast<int_t>(self.mStack.Get<float_t>(0)));
	}

	inline void Actor::CommandPushActor(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const char* pszActorName = vm->GetStringFromMemory(self.mPc + 1);
		MANA_ASSERT(pszActorName);

		const std::shared_ptr<Actor>& targetActor = vm->GetActor(pszActorName);
		self.mStack.Push(targetActor.get());
	}

	inline void Actor::CommandLoadData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1);
		const void* pointer = self.mStack.Pop<void*>();
		self.mStack.Push(pointer, size);
	}

	inline void Actor::CommandStoreData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1);
		void* pointer = self.mStack.Pop<void*>();
		self.mStack.PopData(pointer, size);
	}

	inline void Actor::CommandDuplicateData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1);
		void* pointer = self.mStack.GetAddress(size / sizeof(void*));
		self.mStack.Push(pointer, size);
	}

	inline void Actor::CommandRemoveData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mStack.Remove(vm->GetInt32FromMemory(self.mPc + 1) / sizeof(void*));
	}

	inline void Actor::CommandCompareEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) == 0);
	}

	inline void Actor::CommandCompareGreaterEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) >= 0);
	}

	inline void Actor::CommandCompareGreaterdata(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) > 0);
	}

	inline void Actor::CommandCompareNotEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) != 0);
	}

	inline void Actor::CommandCompareLessEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) <= 0);
	}

	inline void Actor::CommandCompareLessdata(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const int32_t size = vm->GetInt32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) < 0);
	}

	inline void Actor::CommandCall(const std::shared_ptr<VM>& vm, Actor& self)
	{
		uint32_t last_pc = self.mPc;
		int32_t last_interrupt_level = self.mInterruptLevel;

		// 外部関数の実行
		const char* functionName = vm->GetStringFromMemory(self.mPc + 1);
		VM::ExternalFuntionType function = vm->FindFunction(functionName);
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
					// TODO:即値を使わないで下さい
					self.mInterrupt[last_interrupt_level].mAddress = last_pc + 4 + 2 + 2 + 1 + (nNumberOfArguments * sizeof(int16_t));
					self.mInterrupt[self.mInterruptLevel].mStackPointer = self.mStack.GetSize();
					self.mInterrupt[self.mInterruptLevel].mFlag.set(Interrupt::Flag::Repeat);
				}
			}
		}
	}

	inline void Actor::CommandRequest(const std::shared_ptr<VM>& vm, Actor& self)
	{
		Actor* target_actor = self.mStack.Pop<Actor*>();
		int_t level = self.mStack.Pop<int_t>();
		const char* action = vm->GetStringFromMemory(self.mPc + 1);

		if (target_actor)
		{
			self.mInterrupt[self.mInterruptLevel].mFlag = Interrupt::Flag::Repeat;
			self.mPc += sizeof(int32_t) + 1;
			target_actor->Request(level, action, self.shared_from_this());
		}
	}

	inline void Actor::CommandRequestWaitStarting(const std::shared_ptr<VM>& vm, Actor& self)
	{
		Actor* target_actor = (Actor*)self.mStack.Get<void*>(0);
		int_t level = self.mStack.Get<int_t>(1);
		const char* action = vm->GetStringFromMemory(self.mPc + 1);

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

	inline void Actor::CommandRequestWaitEnding(const std::shared_ptr<VM>& vm, Actor& self)
	{
		Actor* target_actor = (Actor*)self.mStack.Get<void*>(0);
		int_t level = self.mStack.Get<int_t>(1);
		const char* action = vm->GetStringFromMemory(self.mPc + 1);

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

	inline void Actor::Commanddynamic_request(const std::shared_ptr<VM>&, Actor&)
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

	inline void Actor::CommandDynamicRequestWaitStarting(const std::shared_ptr<VM>&, Actor&)
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

	inline void Actor::Commanddynamic_request_wait_ending(const std::shared_ptr<VM>&, Actor&)
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

	inline void Actor::CommandJoin(const std::shared_ptr<VM>&, Actor& self)
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

	inline void Actor::CommandPrint(const std::shared_ptr<VM>& vm, Actor& self)
	{
		char message[1024];

		int32_t message_pointer = 0;
		int32_t number_of_arguments = vm->GetInt32FromMemory(self.mPc + 1);

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
						message_pointer += sprintf_s(&message[message_pointer], sizeof(message) - message_pointer, "%lld", self.mStack.Get<int_t>(counter++));
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
