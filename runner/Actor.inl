/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "VM.h"
#include "common/FileFormat.h"

namespace mana
{
	inline Actor::Actor(const std::shared_ptr<VM>& vm, const address_t variableSize)
		: mVM(vm)
	{
		MANA_ASSERT(vm.get() != nullptr);
		mVariable.Allocate(variableSize);
	}

	inline std::shared_ptr<Actor> Actor::Clone() const
	{
		std::shared_ptr<Actor> sharedActor = std::make_shared<Actor>(mVM.lock(), mVariable.GetSize());
		sharedActor->mActions = mActions;

		return sharedActor;
	}

	inline bool Actor::Run()
	{
		if (mInterrupts.empty())
			return false;

		using IntermediateLanguageFunction = void(*)(const std::shared_ptr<VM>& vm, Actor& self);
		struct IntermediateLanguageCommand final
		{
			explicit IntermediateLanguageCommand(IntermediateLanguageFunction command)
				: mCommand(command)
			{
			}
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			IntermediateLanguageCommand(IntermediateLanguage code, IntermediateLanguageFunction command)
				: mCode(code)
				, mCommand(command)
			{
			}
			IntermediateLanguage mCode = IntermediateLanguage::Halt;
#endif
			IntermediateLanguageFunction mCommand;
		};
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
#define MANA_ACTOR_SET_COMMAND(code, command)	IntermediateLanguageCommand(code, command)
#else
#define MANA_ACTOR_SET_COMMAND(code, command)	IntermediateLanguageCommand(command)
#endif
		static const std::array<IntermediateLanguageCommand, IntermediateLanguageSize> mIntermediateLanguage = {
			// thread
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Restart, &CommandRestart),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Halt, &CommandHalt),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Yield, &CommandYield),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::NonPreEmptive, &CommandSetNonPreemptive),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PreEmptive, &CommandSetPreemptive),

			// jump
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::BranchEqual, &CommandBranchEqual),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::BranchNotEqual, &CommandBranchNotEqual),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Branch, &CommandBranchAway),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::BranchSubRoutine, &CommandBranchSubRoutine),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Call, &CommandCall),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Request, &CommandRequest),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::RequestWaitStarting, &CommandRequestWaitStarting),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::RequestWaitEnded, &CommandRequestWaitEnding),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Join, &CommandJoin),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Comply, &CommandComply),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Refuse, &CommandRefuse),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadReturnAddress, &CommandLoadReturnAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::SaveReturnAddress, &CommandStoreReturnAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::ReturnFromFunction, &CommandReturnFromFunction),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::ReturnFromAction, &CommandReturnFromAction),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Rollback, &CommandRollback),

			// constant
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushZeroInteger, &CommandPushZeroInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushZeroFloat, &CommandPushZeroFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushChar, &CommandPushChar),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushShort, &CommandPushShort),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushInteger, &CommandPushInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushSize, &CommandPushInteger),	// TODO:サイズに対応して下さい
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushFloat, &CommandPushFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushString, &CommandPushString),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushPriority, &CommandPushPriority),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushActor, &CommandPushActor),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushSelf, &CommandPushSelf),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::PushSender, &CommandPushSender),

			// stack
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Allocate, &CommandAllocate),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Free, &CommandRelease),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Duplicate, &CommandDuplicate),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::DuplicateData, &CommandDuplicateData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Remove, &CommandRemove),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::RemoveData, &CommandRemoveData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadStaticAddress, &CommandLoadStaticAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadGlobalAddress, &CommandLoadGlobalAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadFrameAddress, &CommandLoadFrameAddress),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadSelfAddress, &CommandLoadSelfAddress),

			// memory operation
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadChar, &CommandLoadInt8),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadShort, &CommandLoadInt16),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadInteger, &CommandLoadInt32),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadFloat, &CommandLoadFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadReference, &CommandLoadReference),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LoadData, &CommandLoadData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::StoreChar, &CommandStoreInt8),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::StoreShort, &CommandStoreInt16),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::StoreInteger, &CommandStoreInt32),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::StoreFloat, &CommandStoreFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::StoreReference, &CommandStoreReference),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::StoreData, &CommandStoreData),

			// caluclation
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::AddInteger, &CommandAddInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::DivideInteger, &CommandDivideInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MinusInteger, &CommandMinusInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::ModInteger, &CommandModInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MultiInteger, &CommandMultiplyInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::SubtractInteger, &CommandSubtractInteger),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::AddFloat, &CommandAddFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::DivideFloat, &CommandDivideFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MinusFloat, &CommandMinusFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::ModFloat, &CommandModFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::MultiFloat, &CommandMultiplyFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::SubtractFloat, &CommandSubtractFloat),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::And, &CommandAnd),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::ExclusiveOr, &CommandExclusiveOr),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LogicalAnd, &CommandLogicalAnd),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LogicalOr, &CommandLogicalOr),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::LogicalNot, &CommandLogicalNot),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Not, &CommandNot),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Or, &CommandOr),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::ShiftLeft, &CommandShiftLeft),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::ShiftRight, &CommandShiftRight),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CastIntegerToFloat, &CommandIntegerToFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CastFloatToInteger, &CommandFloatToInteger),

			// compare and branch
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareEqualInteger, &CommandCompareEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareNotEqualInteger, &CommandCompareNotEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareGreaterEqualInteger, &CommandCompareGreaterEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareGreaterInteger, &CommandCompareGreaterInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareLessEqualInteger, &CommandCompareLessEqualInteger),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareLessInteger, &CommandCompareLessInteger),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareEqualFloat, &CommandCompareEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareNotEqualFloat, &CommandCompareNotEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareGreaterEqualFloat, &CommandCompareGreaterEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareGreaterFloat, &CommandCompareGreaterFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareLessEqualFloat, &CommandCompareLessEqualFloat),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareLessFloat, &CommandCompareLessFloat),

			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareEqualData, &CommandCompareEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareNotEqualData, &CommandCompareNotEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareGreaterEqualData, &CommandCompareGreaterEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareGreaterData, &CommandCompareGreaterData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareLessEqualData, &CommandCompareLessEqualData),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::CompareLessData, &CommandCompareLessData),

			// utility
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::Print, &CommandPrint),

			// under discussion
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::DynamicRequest, &CommandDynamicRequest),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::DynamicRequestWaitStarting, &CommandDynamicRequestWaitStarting),
			MANA_ACTOR_SET_COMMAND(IntermediateLanguage::DynamicRequestWaitEnded, &CommandDynamicRequestWaitEnded),
		};
#undef MANA_ACTOR_SET_COMMAND

		std::shared_ptr<VM> vm = mVM.lock();

		size_t timer = 0;
		do {
			mFlag.reset(static_cast<uint8_t>(Flag::Requested));

			if (!IsRunning())
				return false;
			
			const int32_t opecode = vm->GetOpecode(mPc);
			const IntermediateLanguageCommand& command = mIntermediateLanguage.at(opecode);
			MANA_ASSERT(static_cast<IntermediateLanguage>(opecode) == command.mCode);
			MANA_ASSERT(command.mCommand);
			command.mCommand(vm, *this);

			if (!IsRunning())
				return false;

			const auto currentInterruptIterator = mInterrupts.find(mInterruptPriority);
			if (currentInterruptIterator == mInterrupts.end())
				return false;
			Interrupt& currentInterrupt = currentInterruptIterator->second;
			if (currentInterrupt.mFlag.test(static_cast<uint8_t>(Interrupt::Flag::Repeat)))
			{
				currentInterrupt.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::Repeat));
			}
			else
			{
				mPc += GetInstructionSize(mVM.lock()->mInstructionPool, mPc);
				currentInterrupt.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::Initialized));
			}

			if (currentInterrupt.mFlag.test(static_cast<uint8_t>(Interrupt::Flag::Suspend)))
			{
				currentInterrupt.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::Suspend));
				break;
			}
		} while (++timer < 500);

		return mFlag.test(static_cast<uint8_t>(Flag::Running));
	}

	inline bool Actor::SyncCall(const int32_t priority, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if (Request(priority, action, sender))
		{
			const auto interruptIterator = mInterrupts.find(priority);
			if (interruptIterator == mInterrupts.end())
				return false;
			interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::IsInSyncCall));
			while (true)
			{
				mVM.lock()->Run();
				if (mInterruptPriority < priority)
					return true;
			}
		}
		return false;
	}

	inline bool Actor::AsyncCall(const int32_t priority, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if (Request(priority, action, sender))
		{
			const auto interruptIterator = mInterrupts.find(priority);
			if (interruptIterator == mInterrupts.end())
				return false;
			interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::IsInSyncCall));
			while (true)
			{
				Run();
				if (mInterruptPriority < priority)
					return true;
			}
		}
		return false;
	}

	inline bool Actor::Request(const int32_t priority, const char* action, const std::shared_ptr<Actor>& sender)
	{
		if (priority <= LowestInterruptPriority)
		{
			MANA_TRACE({ "mana:request: priority ", std::to_string(priority), ", ", GetName(), "::", action, " request ignored. reason: lowest priority or below\n" });
			return false;
		}

		if (mFlag.test(static_cast<uint8_t>(Flag::Halt)))
		{
			MANA_TRACE({ "mana:request: priority ", std::to_string(priority), ", ", GetName(), "::", action, " request failed. reason: halt\n" });
			return false;
		}

		if (mFlag.test(static_cast<uint8_t>(Flag::Refused)))
		{
			MANA_TRACE({ "mana:request: priority ", std::to_string(priority), ", ", GetName(), "::", action, " request failed. reason: refuse\n" });
			return false;
		}

		if (mInterrupts.find(priority) != mInterrupts.end())
		{
			MANA_TRACE({ "mana:request: priority ", std::to_string(priority), ", ", GetName(), "::", action, " request failed.\n" });
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			const auto interruptIterator = mInterrupts.find(priority);
			if (interruptIterator != mInterrupts.end() && interruptIterator->second.mActionName.empty() == false)
				MANA_TRACE({ "reason : ", interruptIterator->second.mActionName," running" });
#endif
			MANA_TRACE("\n");
			return false;
		}

		uint32_t address = GetAction(action);
		if (address == Nil)
		{
			MANA_TRACE({ "mana:request: priority ", std::to_string(priority), ",", GetName(), "::", action, " request failed.reason: not found\n" });
			return false;
		}

		mFlag.set(static_cast<uint8_t>(Flag::Running));

		Interrupt interrupt;
		interrupt.mSender = sender;
		interrupt.mReturnAddress = Nil;
		interrupt.mAddress = address;
		interrupt.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Synchronized));
		interrupt.mFileCallbackParameter = nullptr;

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		MANA_TRACE({ "mana:request: ", GetName(), " " });
		if (mInterrupts.empty() == false)
		{
			const auto interruptIterator = mInterrupts.find(mInterruptPriority);
			if (interruptIterator != mInterrupts.end())
				MANA_TRACE({ "priority ", std::to_string(mInterruptPriority), " ", interruptIterator->second.mActionName, " => " });
		}
		MANA_TRACE({ "priority ", std::to_string(priority), " ",  action , " (address:", std::to_string(address), ")\n" });

		// 実行するアクション名を記録
		interrupt.mActionName = action;
#endif

		if (priority >= mInterruptPriority)
		{
			// 現在よりも高い優先度(高いほど優先)の場合、すぐに割り込む

			MANA_TRACE({ "mana:request: ", GetName(), " " });
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			if (mInterruptPriority)
			{
				const auto interruptIterator = mInterrupts.find(mInterruptPriority);
				if (interruptIterator != mInterrupts.end())
					MANA_TRACE({ "priority ", std::to_string(mInterruptPriority), " ", interruptIterator->second.mActionName, " => " });
			}
#endif
			MANA_TRACE({ "priority ", std::to_string(priority), " ", interrupt.mActionName, " succeed\n" });

			// コールバック関数を呼びます
			mRequestEvent.Broadcast(priority);

			Again();

			// 現在のFPとSPを保存します
			interrupt.mFramePointer = mFrame.GetSize();
			interrupt.mStackPointer = mStack.GetSize();

			if (mPc != Nil)
			{
				// 現在のプログラムカウンタを保存します
				const auto interruptIterator = mInterrupts.find(mInterruptPriority);
				if (interruptIterator != mInterrupts.end())
					interruptIterator->second.mAddress = mPc;
			}

			// 新しい優先度(高いほど優先)とプログラムカウンタを設定します
			mInterruptPriority = priority;
			mPc = address;

			// 次のTickでプログラムカウンタを進めない処理
			interrupt.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));
		}
		else
		{
			// 現在よりも低い優先度(高いほど優先)の場合、現在のアクション終了後に実行

			// 現在の優先度(高いほど優先)に入ったときのFPとSPを保存します
			const auto interruptIterator = mInterrupts.find(mInterruptPriority);
			if (interruptIterator != mInterrupts.end())
			{
				interrupt.mFramePointer = interruptIterator->second.mFramePointer;
				interrupt.mStackPointer = interruptIterator->second.mStackPointer;
			}

			// 次のTickでプログラムカウンタを進める処理
			interrupt.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::Repeat));
		}

		mInterrupts[priority] = interrupt;

		MANA_ASSERT(mPc != Nil);

		return true;
	}

	inline void Actor::Rollback(const int32_t priority)
	{
		if (mInterrupts.empty())
			return;

		// 非同期モードに変更
		SetSynchronized(false);

		// 現在実行中のリクエストを検索
		const auto currentInterruptIterator = mInterrupts.find(mInterruptPriority);
		if (currentInterruptIterator == mInterrupts.end())
			return;
		Interrupt& currentInterrupt = currentInterruptIterator->second;

		// リクエストが終了したことをsenderに通知する
		if (currentInterrupt.mSender)
		{
			currentInterrupt.mSender->Again();
			currentInterrupt.mSender = nullptr;
		}
#if 0
		/* ファイル読み込み中ならば解放 */
		if (mAsyncFileCallback && currentInterrupt.mFileCallbackParameter)
		{
			mAsyncFileCallback(MANA_FILE_COMMAND_CLOSE, currentInterrupt.mFileCallbackParameter);
			currentInterrupt.mFileCallbackParameter = nullptr;
		}
#endif

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		const int32_t lastInterruptPriority = mInterruptPriority;
		const std::string lastActionName = currentInterrupt.mActionName;
#endif

		// SyncCall中か取得
		const bool inSyncCall = currentInterrupt.mFlag.test(static_cast<uint8_t>(Interrupt::Flag::IsInSyncCall));
		if (inSyncCall)
		{
			currentInterrupt.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::IsInSyncCall));
		}

		// FPとSPを取得
		address_t framePointer = currentInterrupt.mFramePointer;
		address_t stackPointer = currentInterrupt.mStackPointer;

		// 優先度開放
		mInterrupts.erase(mInterruptPriority);

		const int32_t targetPriority = (priority < LowestInterruptPriority) ? LowestInterruptPriority : priority;

		// 優先度(高いほど優先)が指定されているならば、強制的に指定優先度まで戻る
		if (mInterruptPriority - 1 > targetPriority)
		{
			std::vector<int32_t> prioritiesToRelease;
			prioritiesToRelease.reserve(mInterrupts.size());

			for (auto iterator = mInterrupts.rbegin(); iterator != mInterrupts.rend(); ++iterator)
			{
				if (iterator->first <= targetPriority)
					break;
				prioritiesToRelease.push_back(iterator->first);
			}

			for (const int32_t currentLevel : prioritiesToRelease)
			{
				const auto interruptIterator = mInterrupts.find(currentLevel);
				if (interruptIterator == mInterrupts.end())
					continue;

				Interrupt* interrupt = &interruptIterator->second;

				// 優先度開放
				interrupt->mAddress = Nil;
#if 0
				/* ファイルエントリの削除 */
				if (mAsyncFileCallback && mInterrupts->mFileCallbackParameter)
				{
					MANA_TRACE("mana:rollback: %d: waiting for file loading\n", currentLevel);

					mAsyncFileCallback(MANA_FILE_COMMAND_CLOSE, interrupt->mFileCallbackParameter);
					interrupt->mFileCallbackParameter = nullptr;
				}
#endif				
				// フレームポインタとスタックポインタを取得
				framePointer = interrupt->mFramePointer;
				stackPointer = interrupt->mStackPointer;

				/* コールバック関数を呼びます */
				mRollbackEvent.Broadcast(currentLevel);

				mInterrupts.erase(interruptIterator);
			}
		}

		// FPとSPを復元します
		mFrame.SetSize(framePointer);
		mStack.SetSize(stackPointer);

		// 中断されたアクションの再開
		for (auto iterator = mInterrupts.rbegin(); iterator != mInterrupts.rend(); ++iterator)
		{
			auto interrupt = &iterator->second;
			if (interrupt->mAddress == Nil)
				continue;

			// 中断していた場所から復帰させます
			mPc = interrupt->mAddress;
#if 0
			// コールバック関数を呼びます
			if (mRollbackCallback)
				mRollbackCallback(mRollbackCallbackParameter);
#endif			
			// 優先度(高いほど優先)変更
			mInterruptPriority = iterator->first;

			// 次回のTickでプログラムカウンターを加算しない
			interrupt->mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));

			// SyncCall中ならばTickを抜ける
			if (inSyncCall)
				yield();

			MANA_TRACE(
				{
					"mana:rollback: ", GetName(),
					" priority ", std::to_string(lastInterruptPriority), " ", lastActionName,
					" =>",
					" priority ", std::to_string(mInterruptPriority), " ", interrupt->mActionName,
					" succeed\n"
				}
			);

			/*
			 MANA_ASSERT(
			 mPc >= GetParent().instruction_pool &&
			 mPc < &GetParent().instruction_pool[GetParent().file_header->size_of_instruction_pool]);
			 */

			return;
		}

		// 実行可能なアクションが無いので、アクターは停止する
		Restart();
		MANA_TRACE({ "mana:rollback: ", GetName(), " priority ", std::to_string(lastInterruptPriority), " ", lastActionName, " stopped\n" });
	}

	inline void Actor::Restart()
	{
		mPc = Nil;
		mInterruptPriority = LowestInterruptPriority;
		mFlag.reset(static_cast<uint8_t>(Flag::Halt));
		mFlag.reset(static_cast<uint8_t>(Flag::Running));
		mFlag.reset(static_cast<uint8_t>(Flag::Refused));
		mInterrupts.clear();
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
		return (i == mActions.end()) ? Nil : i->second;
	}

	inline void Actor::SetAction(const std::string_view& actionName, const uint32_t address)
	{
		mActions[actionName] = address;
	}

	inline int32_t Actor::GetArgumentCount() const
	{
		return GetArgumentCountByAddress(mPc);
	}

	inline int32_t Actor::GetArgumentCountByAddress(const uint32_t address) const
	{
		return (int32_t)mVM.lock()->GetUint8FromMemory(address + 1 + sizeof(int32_t) + sizeof(uint8_t));
	}

	inline int32_t Actor::GetArgumentSize(const uint32_t address) const
	{
		return (int32_t)mVM.lock()->GetUint16FromMemory(address + 1 + sizeof(int32_t) + sizeof(uint8_t) + sizeof(uint8_t));
	}

	inline bool Actor::HasReturnValue(const uint32_t address) const
	{
		return mVM.lock()->GetUint8FromMemory(address + 1 + sizeof(int32_t)) ? true : false;
	}

	inline int32_t Actor::GetParameterInteger(const int32_t value) const
	{
		MANA_ASSERT(GetArgumentCount() > value);

		uint32_t address = mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t));
		return mStack.Get<int_t>(mVM.lock()->GetUint16FromMemory(address));
	}

	inline float Actor::GetParameterFloat(const int32_t value) const
	{
		MANA_ASSERT(GetArgumentCount() > value);
		return mStack.Get<float_t>(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))));
	}

	inline const char* Actor::GetParameterString(const int32_t value) const
	{
		MANA_ASSERT(GetArgumentCount() > value);
		return mStack.Get<const char*>(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))));
	}

	inline Actor* Actor::GetParameterActor(const int32_t value) const
	{
		return static_cast<Actor*>(GetParameterPointer(value));
	}

	inline void* Actor::GetParameterPointer(const int32_t value) const
	{
		MANA_ASSERT(GetArgumentCount() > value);
		return mStack.Get<void*>(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))));
	}

	inline void* Actor::GetParameterAddress(const int32_t value) const
	{
		MANA_ASSERT(GetArgumentCount() > value);
		return mStack.GetAddress(mVM.lock()->GetInt16FromMemory(mPc + 5 + sizeof(int16_t) + sizeof(int16_t) + (value * sizeof(int16_t))) + 1);
	}

	inline void Actor::SetReturnInteger(const int32_t value)
	{
		mReturnValue.mSize = ReturnValue::Integer;
		mReturnValue.mValues.mIntegerValue = value;
	}

	inline void Actor::SetReturnFloat(const float value)
	{
		mReturnValue.mSize = ReturnValue::Float;
		mReturnValue.mValues.mFloatValue = value;
	}

	inline void Actor::SetReturnString(const char* string)
	{
		mReturnValue.mSize = ReturnValue::String;
		mReturnValue.mValues.mStringValue = string;
	}

	inline void Actor::SetReturnActor(const std::shared_ptr<Actor>& actor)
	{
		mReturnValue.mSize = ReturnValue::Actor;
		mReturnValue.mValues.mActorValue = actor.get();
	}

	inline void Actor::SetReturnPointer(void* pointer)
	{
		mReturnValue.mSize = ReturnValue::Actor;
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
	inline std::string_view Actor::GetActorName() const
	{
		return mName;
	}

	inline void Actor::SetActorName(const std::string_view& name)
	{
		mName = name;
	}

	inline std::string Actor::GetActionName() const
	{
		const auto interruptIterator = const_cast<Actor*>(this)->mInterrupts.find(mInterruptPriority);
		if (interruptIterator == const_cast<Actor*>(this)->mInterrupts.end())
			return std::string();
		return interruptIterator->second.mActionName;
	}

	inline std::string_view Actor::GetFunctionName() const
	{
		return std::string_view(mVM.lock()->GetStringFromMemory(mPc + 1));
	}
#endif

	inline std::shared_ptr<VM> Actor::GetVirtualMachine() const
	{
		return mVM.lock();
	}

	inline bool Actor::IsCommandInitialized() const
	{
		const auto interrupt = mInterrupts.find(mInterruptPriority);
		if (interrupt == mInterrupts.end())
			return false;
		return interrupt->second.mFlag.test(static_cast<uint8_t>(Interrupt::Flag::Initialized));
	}

	inline bool Actor::IsCommandRepeat() const
	{
		const auto interrupt = mInterrupts.find(mInterruptPriority);
		if (interrupt == mInterrupts.end())
			return false;
		return interrupt->second.mFlag.test(static_cast<uint8_t>(Interrupt::Flag::Repeat));
	}

	inline bool Actor::IsRunning() const
	{
		return
			mFlag.test(static_cast<uint8_t>(Flag::Halt)) == false &&
			mFlag.test(static_cast<uint8_t>(Flag::Running)) == true;
	}

	inline void Actor::Repeat(const bool initialComplete)
	{
		const auto interruptIterator = mInterrupts.find(mInterruptPriority);
		if (interruptIterator == mInterrupts.end())
			return;
		auto& interrupt = interruptIterator->second;
		if (initialComplete)
			interrupt.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Initialized));
		else
			interrupt.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::Initialized));
		interrupt.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));
		interrupt.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Suspend));
	}

	inline void Actor::Again()
	{
		mFlag.set(static_cast<uint8_t>(Flag::Requested));
		mVM.lock()->mFlag.set(static_cast<uint8_t>(VM::Flag::Requested));
	}

	inline void Actor::Halt()
	{
		Stop();
		yield();

		mFlag.set(static_cast<uint8_t>(Flag::Halt));
		mInterruptPriority = LowestInterruptPriority;
		mInterrupts.clear();
	}

	inline void Actor::Stop()
	{
		mFlag.reset(static_cast<uint8_t>(Flag::Running));
	}

	inline void Actor::yield()
	{
		const auto interruptIterator = mInterrupts.find(mInterruptPriority);
		if (interruptIterator == mInterrupts.end())
			return;
		interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Suspend));
	}
	
	inline void Actor::Accept()
	{
		mFlag.reset(static_cast<uint8_t>(Flag::Refused));
	}

	inline void Actor::Refuse()
	{
		mFlag.set(static_cast<uint8_t>(Flag::Refused));
	}

	inline int32_t Actor::GetInterruptPriority() const
	{
		return mInterruptPriority;
	}

	inline bool Actor::IsSynchronized() const
	{
		auto interrupt = mInterrupts.find(mInterruptPriority);
		if (interrupt == mInterrupts.end())
			return false;
		return interrupt->second.mFlag.test(static_cast<uint8_t>(Interrupt::Flag::Synchronized));
	}

	inline void Actor::SetSynchronized(const bool synchronized)
	{
		const auto interruptIterator = mInterrupts.find(mInterruptPriority);
		if (interruptIterator == mInterrupts.end())
			return;
		if (synchronized)
			interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Synchronized));
		else
			interruptIterator->second.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::Synchronized));
	}

	inline void Actor::SetSynchronizedWithPriority(const int32_t priority, const bool synchronized)
	{
		const auto interruptIterator = mInterrupts.find(priority);
		if (interruptIterator == mInterrupts.end())
			return;
		if (synchronized)
			interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Synchronized));
		else
			interruptIterator->second.mFlag.reset(static_cast<uint8_t>(Interrupt::Flag::Synchronized));
	}

	inline EventNameType Actor::AddRequestEvent(const std::function<void(int32_t)>& function)
	{
		return mRequestEvent.Add(function);
	}

	inline void Actor::RemoveRequestEvent(const EventNameType eventName)
	{
		mRequestEvent.Remove(eventName);
	}

	inline EventNameType Actor::AddRollbackEvent(const std::function<void(int32_t)>& function)
	{
		return mRollbackEvent.Add(function);
	}

	inline void Actor::RemoveRollbackEvent(const EventNameType eventName)
	{
		mRollbackEvent.Remove(eventName);
	}

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

	inline Stack& Actor::GetStack()
	{
		return mStack;
	}

	inline const Stack& Actor::GetStack() const
	{
		return mStack;
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
		self.mStack.Push<int_t>(self.mInterruptPriority);
	}

	inline void Actor::CommandPushSelf(const std::shared_ptr<VM>&, Actor& self)
	{
		self.mStack.Push(&self);
	}

	inline void Actor::CommandPushSender(const std::shared_ptr<VM>&, Actor& self)
	{
		const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
		if (interruptIterator != self.mInterrupts.end())
			self.mStack.Push(interruptIterator->second.mSender.get());
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
		const address_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		Buffer& staticVariables = vm->GetStaticVariables();
		MANA_ASSERT(offset < staticVariables.GetSize());
		self.mStack.Push(staticVariables.GetAddressFromTop<void>(offset));
	}

	inline void Actor::CommandLoadGlobalAddress(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const address_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		Buffer& globalVariables = vm->GetGlobalVariables();
		MANA_ASSERT(offset < globalVariables.GetSize());
		self.mStack.Push(globalVariables.GetAddressFromTop<void>(offset));
	}

	inline void Actor::CommandLoadFrameAddress(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		self.mStack.Push(self.mFrame.GetAddressFromBottom<void>(offset));
	}

	inline void Actor::CommandLoadSelfAddress(const std::shared_ptr<VM>& vm, Actor& self)
	{
		uint32_t offset = vm->GetUint32FromMemory(self.mPc + 1);
		self.mStack.Push(self.mVariable.GetAddressFromBottom<void>(offset));
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

	inline void Actor::CommandLoadReference(const std::shared_ptr<VM>&, Actor& self)
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

	inline void Actor::CommandStoreReference(const std::shared_ptr<VM>&, Actor& self)
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
			const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
			if (interruptIterator != self.mInterrupts.end())
				interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));

			MANA_ASSERT(self.mPc < vm->mFileHeader->mSizeOfInstructionPool);
		}
	}

	inline void Actor::CommandBranchNotEqual(const std::shared_ptr<VM>& vm, Actor& self)
	{
		if (self.mStack.Pop<int_t>())
		{
			self.mPc = vm->GetUint32FromMemory(self.mPc + 1);
			const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
			if (interruptIterator != self.mInterrupts.end())
				interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));

			MANA_ASSERT(self.mPc < vm->mFileHeader->mSizeOfInstructionPool);
		}
	}

	inline void Actor::CommandBranchAway(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mPc = vm->GetUint32FromMemory(self.mPc + 1);
		const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
		if (interruptIterator != self.mInterrupts.end())
			interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));

		MANA_ASSERT(self.mPc < vm->mFileHeader->mSizeOfInstructionPool);
	}

	inline void Actor::CommandBranchSubRoutine(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
		if (interruptIterator != self.mInterrupts.end())
		{
			interruptIterator->second.mReturnAddress = self.mPc + 4 + 1; // saved return program address
			interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));
		}
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
		const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
		if (interruptIterator != self.mInterrupts.end())
			interruptIterator->second.mReturnAddress = *pointer;
	}

	inline void Actor::CommandStoreReturnAddress(const std::shared_ptr<VM>&, Actor& self)
	{
		uint32_t* pointer = self.mFrame.GetAddressFromBottom<uint32_t>(sizeof(int32_t));
		const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
		if (interruptIterator != self.mInterrupts.end())
			*pointer = interruptIterator->second.mReturnAddress;
	}

	inline void Actor::CommandReturnFromFunction(const std::shared_ptr<VM>&, Actor& self)
	{
		const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
		if (interruptIterator != self.mInterrupts.end())
		{
			self.mPc = interruptIterator->second.mReturnAddress;
			interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));
		}
	}

	inline void Actor::CommandReturnFromAction(const std::shared_ptr<VM>&, Actor& self)
	{
		self.Rollback(self.mInterruptPriority);
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

	inline void Actor::CommandModFloat(const std::shared_ptr<VM>&, Actor& self)
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

	inline void Actor::CommandLogicalNot(const std::shared_ptr<VM>&, Actor& self)
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
		self.mStack.Set(0, std::abs(left - right) <= std::numeric_limits<float>::epsilon());
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
		self.mStack.Set(0, std::abs(left - right) > std::numeric_limits<float>::epsilon());
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
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1);
		const void* pointer = self.mStack.Pop<void*>();
		self.mStack.Push(pointer, size);
	}

	inline void Actor::CommandStoreData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1);
		void* pointer = self.mStack.Pop<void*>();
		self.mStack.PopData(pointer, size);
	}

	inline void Actor::CommandDuplicateData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1);
		void* pointer = self.mStack.GetAddress(size / sizeof(void*));
		self.mStack.Push(pointer, size);
	}

	inline void Actor::CommandRemoveData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		self.mStack.Remove(vm->GetUint32FromMemory(self.mPc + 1) / sizeof(void*));
	}

	inline void Actor::CommandCompareEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) == 0);
	}

	inline void Actor::CommandCompareGreaterEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) >= 0);
	}

	inline void Actor::CommandCompareGreaterData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) > 0);
	}

	inline void Actor::CommandCompareNotEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) != 0);
	}

	inline void Actor::CommandCompareLessEqualData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) <= 0);
	}

	inline void Actor::CommandCompareLessData(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t size = vm->GetUint32FromMemory(self.mPc + 1 + sizeof(int32_t));
		const void* buf1 = self.mStack.GetAddress(size * 1 / sizeof(void*));
		const void* buf2 = self.mStack.GetAddress(size * 2 / sizeof(void*));
		self.mStack.Remove(size * 2 / sizeof(void*));
		self.mStack.Push(std::memcmp(buf1, buf2, size) < 0);
	}

	inline void Actor::CommandCall(const std::shared_ptr<VM>& vm, Actor& self)
	{
		const uint32_t lastPc = self.mPc;
		const int32_t lastInterruptPriority = self.mInterruptPriority;

		// 外部関数の実行
		const char* functionName = vm->GetStringFromMemory(self.mPc + 1);
		const VM::ExternalFunctionType function = vm->FindFunction(functionName);
		function(self.shared_from_this());

		if (self.IsRunning())
		{
			if (!self.IsCommandRepeat())
			{
				const bool bHasReturnValue = self.HasReturnValue(lastPc);
				const int32_t nNumberOfArguments = self.GetArgumentCountByAddress(lastPc);
				const int32_t nSizeOfArguments = self.GetArgumentSize(lastPc);

				// スタックに入っているパラメータをpopする
				self.mStack.Remove(nSizeOfArguments);

				// 戻り値があるなら、戻り値をpush
				if (bHasReturnValue)
				{
					switch (self.mReturnValue.mSize)
					{
					case ReturnValue::Invalid:
						break;

					case ReturnValue::Actor:
						self.mStack.Push(self.mReturnValue.mValues.mActorValue);
						break;

					case ReturnValue::Integer:
						self.mStack.Push(self.mReturnValue.mValues.mIntegerValue);
						break;

					case ReturnValue::Float:
						self.mStack.Push(self.mReturnValue.mValues.mFloatValue);
						break;

					case ReturnValue::String:
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
				if (self.mInterruptPriority > lastInterruptPriority)
				{
					// TODO:即値を使わないで下さい
					const auto lastInterruptIterator = self.mInterrupts.find(lastInterruptPriority);
					if (lastInterruptIterator != self.mInterrupts.end())
						lastInterruptIterator->second.mAddress = lastPc + 4 + 2 + 2 + 1 + (nNumberOfArguments * sizeof(int16_t));
					const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
					if (interruptIterator != self.mInterrupts.end())
					{
						interruptIterator->second.mStackPointer = self.mStack.GetSize();
						interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));
					}
				}
			}
		}
	}

	inline void Actor::CommandRequest(const std::shared_ptr<VM>& vm, Actor& self)
	{
		Actor* targetActor = self.mStack.Pop<Actor*>();
		const int_t priority = self.mStack.Pop<int_t>();
		const char* action = vm->GetStringFromMemory(self.mPc + 1);

		if (targetActor)
		{
			const auto interruptIterator = self.mInterrupts.find(self.mInterruptPriority);
			if (interruptIterator != self.mInterrupts.end())
				interruptIterator->second.mFlag.set(static_cast<uint8_t>(Interrupt::Flag::Repeat));
			self.mPc += sizeof(int32_t) + 1;
			targetActor->Request(priority, action, self.shared_from_this());
		}
	}

	inline void Actor::CommandRequestWaitStarting(const std::shared_ptr<VM>& vm, Actor& self)
	{
		Actor* targetActor = static_cast<Actor*>(self.mStack.Get<void*>(0));
		const int_t priority = self.mStack.Get<int_t>(1);
		const char* action = vm->GetStringFromMemory(self.mPc + 1);

		if (targetActor == nullptr)
		{
			self.mStack.Remove(2);
			return;
		}
		MANA_ASSERT(&self != targetActor);

		if (self.IsCommandInitialized() == false && targetActor->Request(priority, action, self.shared_from_this()) == false)
		{
			self.mStack.Remove(2);
			return;
		}

		if (targetActor->mInterruptPriority <= priority)
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
		Actor* targetActor = static_cast<Actor*>(self.mStack.Get<void*>(0));
		const int_t priority = self.mStack.Get<int_t>(1);
		const char* action = vm->GetStringFromMemory(self.mPc + 1);

		if (targetActor == nullptr)
		{
			self.mStack.Remove(2);
			return;
		}
		MANA_ASSERT(&self != targetActor);

		if (self.IsCommandInitialized() == false && targetActor->Request(priority, action, self.shared_from_this()) == false)
		{
			self.mStack.Remove(2);
			return;
		}

		if (targetActor->mInterruptPriority < priority)
		{
			self.mStack.Remove(2);
		}
		else
		{
			self.Repeat(true);
		}
	}

	inline void Actor::CommandDynamicRequest(const std::shared_ptr<VM>&, Actor&)
	{
		MANA_NOT_IMPLEMENTED();
		/*
		CManaStack& Stack = actor->self.mStack;
		const char* pszTarget, = Stack.PopString();
		const int32_t priority = Stack.PopInteger();
		const char* action = actor->GetStringFromMemory(1);

		actor->self.mInterrupts[actor->GetInterruptPriority()].repeat = true;
		actor->self.mPc += sizeof(int32_t) + 1;

		Actor* targetActor = actor->GetParent().GetActor(pszTarget,);
		if(targetActor)
		targetActor->Request(priority, action, actor);
		*/
	}

	inline void Actor::CommandDynamicRequestWaitStarting(const std::shared_ptr<VM>&, Actor&)
	{
		MANA_NOT_IMPLEMENTED();
		/*
		CManaStack& Stack = actor->self.mStack;
		const char* pszTarget, = Stack.GetString(0);
		int32_t priority = Stack.Get<int_t>(1);
		const char* action = actor->GetStringFromMemory(1);

		Actor* targetActor = actor->GetParent().GetActor(pszTarget,);
		if(targetActor == 0)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor == targetActor)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor->IsInit() && !targetActor->Request(priority, action, actor))
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(targetActor->GetInterruptPriority() <= priority)
		{
		actor->self.mStack.Remove(2);
		}else{
		actor->Repeat();
		}
		*/
	}

	inline void Actor::CommandDynamicRequestWaitEnded(const std::shared_ptr<VM>&, Actor&)
	{
		MANA_NOT_IMPLEMENTED();
		/*
		CManaStack& Stack = actor->self.mStack;
		const char* pszTarget, = Stack.GetString(0);
		int32_t priority = Stack.Get<int_t>(1);
		const char* action = actor->GetStringFromMemory(1);

		Actor* targetActor = actor->GetParent().GetActor(pszTarget,);
		if(targetActor == 0)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor == targetActor)
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(actor->IsInit() && !targetActor->Request(priority, action, actor))
		{
		actor->self.mStack.Remove(2);
		return;
		}
		if(targetActor->self.mInterruptPriority < priority)
		{
		actor->self.mStack.Remove(2);
		}else{
		actor->Repeat();
		}
		*/
	}

	inline void Actor::CommandJoin(const std::shared_ptr<VM>&, Actor& self)
	{
		const auto* targetActor = static_cast<Actor*>(self.mStack.Get<void*>(1));
		if (targetActor == nullptr)
		{
			self.mStack.Remove(2);
			return;
		}

		const auto interruptPriority = self.mStack.Get<int_t>(0);
		if (targetActor->mInterruptPriority <= interruptPriority)
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

		int32_t messagePointer = 0;
		const int32_t numberOfArguments = vm->GetInt32FromMemory(self.mPc + 1);

		if (numberOfArguments > 0)
		{
			int32_t counter = 1;

			for (char* format = static_cast<char*>(self.mStack.Get<void*>(0)); *format; format++)
			{
				if (*format == '%')
				{
					if (counter > numberOfArguments)
					{
						message[messagePointer] = '\n';
						++messagePointer;
						break;
					}

					format++;

					switch (*format)
					{
					case 'd':
#if __STDC_WANT_SECURE_LIB__
						messagePointer += sprintf_s(&message[messagePointer], sizeof(message) - messagePointer, "%ld", self.mStack.Get<int_t>(counter++));
#else
						messagePointer += sprintf(&message[messagePointer], "%d", self.mStack.Get<int_t>(counter++));
#endif
						break;

					case 'f':
#if __STDC_WANT_SECURE_LIB__
						messagePointer += sprintf_s(&message[messagePointer], sizeof(message) - messagePointer, "%f", self.mStack.Get<float_t>(counter++));
#else
						messagePointer += sprintf(&message[messagePointer], "%f", self.mStack.Get<float_t>(counter++));
#endif
						break;

					case 's':
#if __STDC_WANT_SECURE_LIB__
						messagePointer += sprintf_s(&message[messagePointer], sizeof(message) - messagePointer, self.mStack.Get<const char*>(counter++));
#else
						messagePointer += sprintf(&message[messagePointer], "%s", self.mStack.Get<const char*>(counter++));
#endif
						break;

					case 'p':
#if __STDC_WANT_SECURE_LIB__
						messagePointer += sprintf_s(&message[messagePointer], sizeof(message) - messagePointer, "%p", self.mStack.Get<void*>(counter++));
#else
						messagePointer += sprintf(&message[messagePointer], "%p", self.mStack.Get<void*>(counter++));
#endif
						break;

					default:
						message[messagePointer] = *format;
						messagePointer++;
						break;
					}
				}
				else
				{
					message[messagePointer] = *format;
					messagePointer++;
				}
			}
		}
		message[messagePointer] = '\0';

		MANA_PRINT(message);

		self.mStack.Remove(numberOfArguments);
	}
}
