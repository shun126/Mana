/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "CodeBuffer.h"
#include "SyntaxNode.h"
#include "TypeDescriptor.h"
#include <array>

namespace mana
{
	class CodeGenerator;

	/*
	ローカルアドレスを解決します
	*/
	class LocalAddressResolver final : Noncopyable
	{
	public:
		enum class JumpChainStatus : uint8_t
		{
			Switch,
			While,
			Do,
			For,
			Loop,
			Lock,
		};

	public:
		explicit LocalAddressResolver(const std::shared_ptr<CodeBuffer>& codeBuffer);
		~LocalAddressResolver() = default;

		//! ジャンプブロックの開始
		void OpenChain(const JumpChainStatus status);

		//! breakの設定
		int32_t Break(const int32_t newPc);

		//! continueの設定
		int32_t Continue(const int32_t newPc);

		//! continueジャンプブロックの終了
		void CloseContinueOnly();

		//! ジャンプブロックの終了
		void CloseChain();

		//! switchブロックの開始
		void OpenSwitchBlock(const std::shared_ptr<TypeDescriptor>& type);

		//! caseの登録
		void RegisterSwitchCase(const std::shared_ptr<SyntaxNode>& node);

		//! defaultの登録
		void RegisterSwitchDefault();

		//! switchブロックをバイナリーコードに変換
		void ResolveSwitchBlock(const std::shared_ptr<CodeGenerator>& codeGenerator);

		//! switchブロックの終了
		void CloseSwitchBlock(void);

	private:
		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<CodeGenerator> mCodeGenerator;

		static constexpr size_t JumpChainTableSize = 20;		//!< ジャンプチェインテーブルサイズ
		static constexpr size_t JumpSwitchEntryStackSize = 256;	//!< switchの最大ネストレベル
		static constexpr size_t JumpSwitchStackSize = 256;		//!< switchの最大ネストレベル

		//! ジャンプチェインテーブル
		struct JumpChainTable
		{
			JumpChainStatus mStatus;			/*!< ジャンプチェインの状態 */
			int32_t mBreakChain;				/*!< breakチェインの位置 */
			int32_t mContinueChain;				/*!< continueチェインの位置 */
			int32_t mStartAddress;				/*!< ブロック開始位置 */
		};
		std::array<JumpChainTable, JumpChainTableSize> mJumpChainTable;
		int32_t mJumpChainTablePointer;			/*!< JumpChainTable の位置 */

		/*! switchブロック内のエントリー */
		struct JumpSwitchEntry
		{
			std::shared_ptr<SyntaxNode> mNode;	/*!< expressionを表す SyntaxNode */
			int32_t mAddress;					/*!< アドレス */
		};

		//! switchブロック内のエントリースタック
		JumpSwitchEntry mJumpSwitchEntryStack[JumpSwitchEntryStackSize];

		//! switchブロック内のエントリースタックポインター
		JumpSwitchEntry* mJumpSwitchEntryStackPointer;

		//! switchブロックスタック */
		struct JumpSwitchStack
		{
			JumpSwitchEntry* mStackPointer;			//!< switchブロック内のエントリースタック
			std::shared_ptr<TypeDescriptor> mType;	//!< TypeDescriptor
			int32_t mDefaultAddress;				//!< defaultへのアドレス
		};
		std::array<JumpSwitchStack, JumpSwitchStackSize> mJumpSwitchStack;

		//! switchブロックスタックポインター */
		int32_t mJumpSwitchStackPointer;
	};
}
