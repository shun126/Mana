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
#include <string>
#include <unordered_map>

#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace mana
{
	class VM;

	/*!
	Actor class
	*/
	class Actor : private Noncopyable, public std::enable_shared_from_this<Actor>
	{
		friend class VM;

		//! Constructor
		explicit Actor(const std::shared_ptr<VM>& vm, const address_t variableSize);

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
		std::string_view GetActorName() const;
		void SetActorName(const std::string_view& name);
		std::string GetActionName() const;
		const std::string_view GetFunctionName() const;
#endif

		std::shared_ptr<VM> GetVirtualMachine() const;
		bool IsInit();
		bool IsRepeat();
		bool IsRunning();
		void Repeat(const bool initialComplete);
		void Again();
		void Halt();
		void Stop();
		void yield();
		void Accept();
		void Refuse();
		int32_t GetInterruptLevel() const;
		bool IsSynchronized() const;
		void SetSynchronized(const bool synchronized);
		void SetSynchronizedWithLevel(const int32_t level, const bool synchronized);
		
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

		Stack& GetStack();
		const Stack& GetStack() const;

	private:
		void SetAction(const std::string_view& actionName, const uint32_t address);
		//void Initialize(const ActionInfoHeader* actionInfo);

	private:
		static const uint32_t nil = static_cast<uint32_t>(~0);

		static const size_t MANA_ACTOR_MAX_INTERRUPT_LEVEL = 32;

		//! 割り込みテーブル
		struct Interrupt final
		{
			std::shared_ptr<Actor> mSender;			//!< 要求したmana_actor オブジェクト
			uint32_t mAddress;						//!< 割り込み先アドレス
			uint32_t mReturnAddress;				//!< リターンアドレス
			int32_t mCounter;						//!< 汎用カウンタ
			address_t mFramePointer;				//!< フレームポインタ
			address_t mStackPointer;				//!< スタックポインタ
			void* mFileCallbackParameter;			//!< ファイル終了コールバックパラメータ
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
			std::string mActionName;				//!< 実行中のアクション名 */
#endif
			std::bitset<8> mFlag;

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
				int_t mIntegerValue;			//!< 整数値
				float_t mFloatValue;			//!< 実数値
				const char* mStringValue;		//!< 文字列
				void* mPointerValue;			//!< 構造体
				Actor* mActorValue;				//!< アクター
			} mValues;
			int32_t mSize;						//!< サイズ(mPointerValue)

			static constexpr int32_t Invalid = 0;	/*!< 戻り値：無効 */
			static constexpr int32_t Actor = -1;	/*!< 戻り値：アクターへの参照 */
			static constexpr int32_t Integer = -2;	/*!< 戻り値：整数 */
			static constexpr int32_t Float = -3;	/*!< 戻り値：小数 */
			static constexpr int32_t String = -4;	/*!< 戻り値：文字列への参照 */
		};

		std::weak_ptr<VM> mVM;
		std::unordered_map <std::string_view, uint32_t> mActions;
		Buffer mFrame;
		Stack mStack;
		std::map<int32_t, Interrupt> mInterrupts;
		ReturnValue mReturnValue;

		void* mRequestCallbackParameter;			/*!< リクエストコールバック */
		void* mRollbackCallbackParameter;			/*!< ロールバックコールバック */
		Buffer mVariable;	// 将来的にBufferクラスへ
		address_t mPc;								/*!< プログラムカウンタ */
		int32_t mInterruptLevel;					/*!< 割り込みレベル */
		std::bitset<8> mFlag;
		enum class Flag : uint8_t
		{
			HALT,			//!< 停止フラグ
			RUNNING,		//!< 実行フラグ
			TOUCHED,		//!< 接触フラグ
			HIDED,			//!< 非表示フラグ
			REQUESTED,		//!< リクエストフラグ
			REFUSED			//!< リクエスト禁止フラグ
		};
		uintptr_t mUserData;										//!< ユーザーデータ
		void* mUserPointer;											//!< ユーザーポインター

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
#define MANA_ASSERT_PARAMETER(P, I) {					\
	if(P->GetArgumentCount() != I)						\
		return;											\
}
/*! initアクション中ならばreturnします */
#define MANA_ASSERT_ILLIGAL_CALL_IN_INIT_ACTION(P) {	\
	if(P->GetVirtualMachine()->IsInInitAction()){		\
		return;											\
}
#else
/*! 引数の数を調べ、一致しない場合は警告を表示してreturnします */
#define MANA_ASSERT_PARAMETER(P, I)	\
	if(P->GetArgumentCount() != I){	\
		 MANA_PRINT({ "ERROR: ", P->GetName(), ": function ", P->GetFunctionName(), " number of arguments ", std::to_string(P->GetArgumentCount()), " correct ", std::to_string(I), "\n" });\
		 return;					\
	}
/*! initアクション中ならば警告を表示してreturnします */
#define MANA_ASSERT_CANT_CALL_IN_INIT_ACTION(P)			\
	if(P->GetVirtualMachine()->IsInInitAction()){		\
		MANA_PRINT({ "ERROR: ", P->GetName(), ": init action ", P->GetFunctionName()," can not call\n" });\
		return;											\
	}
#endif

#include "Actor.inl"
