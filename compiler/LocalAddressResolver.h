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

	class LocalAddressResolver : private Noncopyable
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
		LocalAddressResolver(const std::shared_ptr<CodeBuffer>& codeBuffer);
		virtual ~LocalAddressResolver();

		/*! ジャンプブロックの開始 */
		void OpenChain(const JumpChainStatus status);

		/*! breakの設定 */
		int32_t Break(int32_t);

		/*! continueの設定 */
		int32_t Continue(int32_t);

		/*! continueジャンプブロックの終了 */
		void CloseContinueOnly(void);

		/*! ジャンプブロックの終了 */
		void CloseChain(void);

		/*! switchブロックの開始 */
		void OpenSwitchBlock(const std::shared_ptr<TypeDescriptor>& type);

		/*! caseの登録 */
		void RegistSwitchCase(const std::shared_ptr<SyntaxNode>& node);

		/*! defaultの登録 */
		void RegistSwitchDefault(void);

		/*! switchブロックをバイナリーコードに変換 */
		void ResolveSwitchBlock(const std::shared_ptr<CodeGenerator>& codeGenerator);

		/*! switchブロックの終了 */
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
			JumpChainStatus status;				/*!< ジャンプチェインの状態 */
			int32_t break_chain;						/*!< breakチェインの位置 */
			int32_t continue_chain;						/*!< continueチェインの位置 */
			int32_t start_address;						/*!< ブロック開始位置 */
		};
		std::array<JumpChainTable, JumpChainTableSize> mJumpChainTable;

		int32_t mJumpChainTablePointer;	/*!< JumpChainTable の位置 */

														/*! switchブロック内のエントリー */
		struct JumpSwitchEntry
		{
			std::shared_ptr<SyntaxNode> node;							/*!< expressionを表す SyntaxNode */
			int32_t address;							/*!< アドレス */
		};

		/*! switchブロック内のエントリースタック */
		JumpSwitchEntry mJumpSwitchEntryStack[JumpSwitchEntryStackSize];

		/*! switchブロック内のエントリースタックポインター */
		JumpSwitchEntry* mJumpSwitchEntryStackPointer;

		/*! switchブロックスタック */
		struct JumpSwitchStack
		{
			JumpSwitchEntry* stack_pointer;		/*!< switchブロック内のエントリースタック */
			std::shared_ptr<TypeDescriptor> type;				/*!< TypeDescriptor */
			int32_t default_address;					/*!< @biref defaultへのアドレス */
		};
		std::array<JumpSwitchStack, JumpSwitchStackSize> mJumpSwitchStack;

		/*! switchブロックスタックポインター */
		int32_t mJumpSwitchStackPointer;
	};
}
