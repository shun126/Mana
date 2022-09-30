/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Buffer.h"
#include "Stack.h"

#include <bitset>
#include <map>
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
		virtual ~Actor() = default;
		
		std::shared_ptr<Actor> Clone() const;
		
		//void Serialize(mana_stream* stream);
		//void Deserialize(mana_stream* stream);
				
		bool Run();
		bool SyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender);
		bool AsyncCall(const int32_t level, const char* action, const std::shared_ptr<Actor>& sender);
		
		bool Request(const int32_t level, const char* mName, const std::shared_ptr<Actor>& sender);
		void Rollback(const int32_t level);
		void Restart();

		const std::string_view& GetName();
		uint32_t GetAction(const std::string_view& actionName) const;

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
		void SetAction(const std::string_view& actionName, const uint32_t address);
		//void Initialize(const ActionInfoHeader* actionInfo);

	private:
		static const uint32_t nil = static_cast<uint32_t>(~0);

		static const size_t MANA_ACTOR_MAX_INTERRUPT_LEVEL = 32;

		//! 割り込みテーブル
		struct Interrupt
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
			};
		};

		//! 戻り値
		struct ReturnValue
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
		};

		static constexpr int32_t MANA_RETURN_VALUE_TYPE_INVALID = 0;	/*!< 戻り値：無効 */
		static constexpr int32_t MANA_RETURN_VALUE_TYPE_ACTOR = -1;		/*!< 戻り値：アクターへの参照 */
		static constexpr int32_t MANA_RETURN_VALUE_TYPE_INTEGER = -2;	/*!< 戻り値：整数 */
		static constexpr int32_t MANA_RETURN_VALUE_TYPE_FLOAT = -3;		/*!< 戻り値：小数 */
		static constexpr int32_t MANA_RETURN_VALUE_TYPE_STRING = -4;	/*!< 戻り値：文字列への参照 */
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		char* mName;
#endif
		std::weak_ptr<VM> mVM;
		std::unordered_map <std::string_view, uint32_t> mActions;
		Buffer mFrame;
		Stack mStack;
		//std::map<int32_t, Interrupt> mInterrupt;
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
			HALT,			//!< 停止フラグ
			RUNNING,		//!< 実行フラグ
			TOUCHED,		//!< 接触フラグ
			HIDED,			//!< 非表示フラグ
			REQUESTED,		//!< リクエストフラグ
			REFUSED		//!< リクエスト禁止フラグ
		};
		uintptr_t mUserData;										//!< ユーザーデータ
		void* mUserPointer;											//!< ユーザーポインター

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
		static void CommandLoadReffrence(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreInt8(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreInt16(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreInt32(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandStoreReffrence(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchEqual(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchNotEqual(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchAway(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandBranchSubRoutine(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCall(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRequest(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRequestWaitStarting(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandRequestWaitEnding(const std::shared_ptr<VM>& vm, Actor& self);
		static void Commanddynamic_request(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandDynamicRequestWaitStarting(const std::shared_ptr<VM>& vm, Actor& self);
		static void Commanddynamic_request_wait_ending(const std::shared_ptr<VM>& vm, Actor& self);
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
		static void CommandModfloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandMultiplyInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandMultiplyFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandSubtractInteger(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandSubtractFloat(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandAnd(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandExclusiveOr(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLogicalAnd(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandLogicalOr(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandNot(const std::shared_ptr<VM>& vm, Actor& self);
		static void Commandlogical_not(const std::shared_ptr<VM>& vm, Actor& self);
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
		static void CommandCompareGreaterdata(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareNotEqualData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessEqualData(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandCompareLessdata(const std::shared_ptr<VM>& vm, Actor& self);
		static void CommandPrint(const std::shared_ptr<VM>& vm, Actor& self);
	};
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

#include "Actor.inl"
