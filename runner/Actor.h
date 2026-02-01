/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Buffer.h"
#include "Event.h"
#include "Stack.h"

#include <bitset>
#include <map>
#include <string>
#include <unordered_map>

namespace mana
{
	class VM;

	/*!
	Actor class
	*/
	class Actor : Noncopyable, public std::enable_shared_from_this<Actor>
	{
		friend class VM;

	public:
		//! Constructor
		explicit Actor(const std::shared_ptr<VM>& vm, const address_t variableSize);

		Actor(const Actor&) = delete;
		Actor(Actor&&) noexcept = delete;
		Actor& operator=(const Actor&) = delete;
		Actor& operator=(Actor&&) noexcept = delete;
		virtual ~Actor() = default;
		
		std::shared_ptr<Actor> Clone() const;
		
		bool Run();
		bool SyncCall(const int32_t priority, const char* action, const std::shared_ptr<Actor>& sender);
		bool AsyncCall(const int32_t priority, const char* action, const std::shared_ptr<Actor>& sender);
		
		bool Request(const int32_t priority, const char* action, const std::shared_ptr<Actor>& sender);
		void Rollback(const int32_t priority);
		void Restart();

		std::string_view GetName();
		uint32_t GetAction(const std::string_view& actionName) const;

		[[nodiscard]] int32_t GetArgumentCount() const;
		[[nodiscard]] int32_t GetArgumentCountByAddress(const uint32_t address) const;
		[[nodiscard]] int32_t GetArgumentSize(const uint32_t address) const;
		[[nodiscard]] bool HasReturnValue(const uint32_t address) const;
		[[nodiscard]] int32_t GetParameterInteger(const int32_t value) const;
		[[nodiscard]] float GetParameterFloat(const int32_t value) const;
		[[nodiscard]] const char* GetParameterString(const int32_t value) const;
		[[nodiscard]] std::shared_ptr<Actor> GetParameterActor(const int32_t value) const;
		[[nodiscard]] Actor* GetParameterActorPointer(const int32_t value) const;
		[[nodiscard]] void* GetParameterPointer(const int32_t value) const;
		[[nodiscard]] void* GetParameterAddress(const int32_t value) const;
		void SetReturnInteger(const int32_t value);
		void SetReturnFloat(const float value);
		void SetReturnString(const char* string);
		void SetReturnActor(const std::shared_ptr<Actor>& actor);
		void SetReturnPointer(void* pointer);
		void SetReturnData(const void* pointer, const int32_t size);

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		[[nodiscard]] std::string_view GetActorName() const;
		void SetActorName(const std::string_view& name);
		[[nodiscard]] std::string GetActionName() const;
		[[nodiscard]] std::string_view GetFunctionName() const;
#endif

		std::shared_ptr<VM> GetVirtualMachine() const;
		[[nodiscard]] bool IsCommandInitialized() const;
		[[nodiscard]] bool IsCommandRepeat() const;
		[[nodiscard]] bool IsRunning() const;
		void Repeat(const bool initialComplete);
		void Again();
		void Halt();
		void Stop();
		void yield();
		void Accept();
		void Refuse();
		[[nodiscard]] int32_t GetInterruptPriority() const;
		[[nodiscard]] bool IsSynchronized() const;
		void SetSynchronized(const bool synchronized);
		void SetSynchronizedWithPriority(const int32_t priority, const bool synchronized);
		
		[[nodiscard]] EventNameType AddPriorityChangedEvent(const std::function<void(int32_t, int32_t)>& function);
		void RemovePriorityChangedEvent(const EventNameType eventName);

		[[nodiscard]] Stack& GetStack();
		[[nodiscard]] const Stack& GetStack() const;

	private:
		void SetAction(const std::string_view& actionName, const uint32_t address);
		//void Initialize(const ActionInfoHeader* actionInfo);

	private:
		static constexpr int32_t LowestInterruptPriority = std::numeric_limits<int32_t>::lowest();
		static constexpr uint32_t Nil = static_cast<uint32_t>(~0);

		//! 割り込みテーブル
		struct Interrupt final
		{
			std::shared_ptr<Actor> mSender;			//!< 要求したActorオブジェクト
			uint32_t mAddress = Nil;				//!< 割り込み先アドレス
			uint32_t mReturnAddress = Nil;			//!< リターンアドレス
			address_t mFramePointer = Nil;			//!< フレームポインタ
			address_t mStackPointer = Nil;			//!< スタックポインタ
			std::bitset<8> mFlag = 0;
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			std::string mActionName;				//!< 実行中のアクション名
#endif

			enum class Flag : uint8_t
			{
				Initialized,	//!< 初期化済みフラグ
				IsInSyncCall,	//!< 同期実行フラグ
				Repeat,			//!< リピートフラグ
				Suspend,		//!< サスペンドフラグ
				Synchronized	//!< 同期フラグ
			};
		};

		//! 戻り値
		struct ReturnValue final
		{
			union
			{
				int_t mIntegerValue;				//!< 整数値
				float_t mFloatValue;				//!< 実数値
				const char* mStringValue;			//!< 文字列
				void* mPointerValue;				//!< 構造体
				Actor* mActorValue;					//!< アクター
			} mValues;
			int32_t mSize;							//!< サイズ(mPointerValue)

			static constexpr int32_t Invalid = 0;	//!< 戻り値：無効
			static constexpr int32_t Actor = -1;	//!< 戻り値：アクターへの参照
			static constexpr int32_t Integer = -2;	//!< 戻り値：整数
			static constexpr int32_t Float = -3;	//!< 戻り値：小数
			static constexpr int32_t String = -4;	//!< 戻り値：文字列への参照

			ReturnValue()
				: mSize(0)
			{
				mValues.mPointerValue = nullptr;
			}
		};

		std::weak_ptr<VM> mVM;
		std::unordered_map <std::string_view, uint32_t> mActions;
		Buffer mFrame;
		Stack mStack;
		std::map<int32_t, Interrupt> mInterrupts;
		ReturnValue mReturnValue;
		Event<int32_t, int32_t> mOnPriorityChanged;
		Buffer mVariable;
		address_t mPc = Nil;									//!< プログラムカウンタ
		int32_t mInterruptPriority = LowestInterruptPriority;	//!< 割り込みレベル
		std::bitset<8> mFlag = 0;
		enum class Flag : uint8_t
		{
			Halt,			//!< 停止フラグ
			Running,		//!< 実行フラグ
			Touched,		//!< 接触フラグ
			Hided,			//!< 非表示フラグ
			Requested,		//!< リクエストフラグ
			Refused			//!< リクエスト禁止フラグ
		};

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		std::string_view mName;
#endif

	private:
		static void CommandRestart(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandHalt(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandYield(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandSetNonPreemptive(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandSetPreemptive(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushZeroInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushZeroFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushChar(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushShort(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushString(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushPriority(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushSelf(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushSender(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandAllocate(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRelease(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDuplicate(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRemove(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadStaticAddress(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadGlobalAddress(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadFrameAddress(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadSelfAddress(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadInt8(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadInt16(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadInt32(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadReference(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreInt8(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreInt16(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreInt32(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreReference(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchEqual(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchNotEqual(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchAway(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchSubRoutine(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCall(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRequest(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRequestWaitStarting(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRequestWaitEnding(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDynamicRequest(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDynamicRequestWaitStarting(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDynamicRequestWaitEnded(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandJoin(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandComply(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRefuse(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadReturnAddress(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreReturnAddress(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandReturnFromFunction(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandReturnFromAction(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRollback(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandAddInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandAddFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDivideInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDivideFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandMinusInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandMinusFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandModInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandModFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandMultiplyInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandMultiplyFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandSubtractInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandSubtractFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandAnd(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandExclusiveOr(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLogicalAnd(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLogicalOr(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandNot(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLogicalNot(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandOr(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandShiftLeft(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandShiftRight(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareEqualInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareEqualFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareGreaterEqualInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareGreaterEqualFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareGreaterInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareGreaterFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareNotEqualInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareNotEqualFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessEqualInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessEqualFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandIntegerToFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandFloatToInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPushActor(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLoadData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDuplicateData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRemoveData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareEqualData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareGreaterEqualData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareGreaterData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareNotEqualData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessEqualData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPrint(const std::shared_ptr<VM>& vm, Actor& self);
	};
}

#if MANA_BUILD_TARGET > MANA_BUILD_DEBUG
//! 引数の数を調べ、一致しない場合はreturnします
#define MANA_ASSERT_PARAMETER(P, I) {					\
	if(P->GetArgumentCount() != I)						\
		return;											\
}
//! initアクション中ならばreturnします
#define MANA_ASSERT_ILLIGAL_CALL_IN_INIT_ACTION(P) {	\
	if(P->GetVirtualMachine()->IsInInitAction()){		\
		return;											\
}
#else
//! 引数の数を調べ、一致しない場合は警告を表示してreturnします
#define MANA_ASSERT_PARAMETER(P, I)	\
	if((P)->GetArgumentCount() != (I)){	\
		 MANA_PRINT({ "ERROR: ", (P)->GetName(), ": function ", (P)->GetFunctionName(), " number of arguments ", std::to_string((P)->GetArgumentCount()), " correct ", std::to_string(I), "\n" });\
		 return;					\
	}
//! initアクション中ならば警告を表示してreturnします
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)			\
	if((P)->GetVirtualMachine()->IsInInitAction()){		\
		MANA_PRINT({ "ERROR: ", (P)->GetName(), ": init action ", (P)->GetFunctionName()," can not call\n" });\
		return;											\
	}
#endif

#include "Actor.inl"
