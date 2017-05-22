/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

// SyntaxNode

#pragma once
#include "../runner/common/Setup.h"
#include <memory>
#include <vector>

namespace mana
{
	class SymbolEntry;
	class TypeDescriptor;

	/*!
	構文木クラス
	*/
	class SyntaxNode
	{
	public:
		//! 構文木識別子
		enum class Id : uint8_t
		{
			Array,								//!< variable[argument] =
			Assign,								//!< =
			CallArgument,						//!< 引数（呼び出し）
			DeclareArgument,					//!< 引数（宣言）
			Const,								//!< 定数
			Call,								//!< 関数呼び出し
			Add,								//!< 加算
			Sub,								//!< 減算
			Mul,								//!< 乗算
			Div,								//!< 除算
			Rem,								//!< 余剰
			Neg,								//!< ±符号反転
			Pow,								//!< べき乗
			Not,								//!< ~
			And,								//!< &
			Or,									//!< |
			Xor,								//!< ^
			LeftShift,							//!< <<
			RightShift,							//!< >>
			Less,								//!< <
			LessEqual,							//!< <=
			Equal,								//!< ==
			NotEqual,							//!< !=
			GreaterEqual,						//!< >=
			Greater,							//!< >
			String,								//!< 文字列
			IntegerToFloat,						//!< 整数から実数へ変換
			FloatToInteger,						//!< 実数から整数へ変換
			LogicalOr,							//!< ||
			LogicalAnd,							//!< &&
			LogicalNot,							//!< !
			Halt,								//!< halt
			Yield,								//!< yield
			Request,							//!< req
			Accept,								//!< comply (req許可)
			Reject,								//!< refuse (req拒否)
			Join,								//!< join
			Sender,								//!< sender (actor)
			Self,								//!< self (actor)
			Priority,							//!< priority (integer)
			ExpressionIf,						//!< 三項演算子 '?'
			Print,								//!< print
			Return,								//!< return
			Rollback,							//!< rollback

			Block,								//!< ブロック
			If,									//!< if
			Switch,								//!< switch
			Case,								//!< case
			Default,							//!< default
			While,								//!< while
			Do,									//!< do while
			For,								//!< for
			Loop,								//!< loop
			Lock,								//!< lock
			Goto,								//!< goto
			Label,								//!< label
			Break,								//!< break
			Continue,							//!< continue

			Identifier,
			TypeDescription,
			Declarator,

			DeclareVariable,
			Sizeof,

			Actor,								//!< アクターの宣言
			Phantom,
			Module,
			Struct,
			Action,
			Extend,
			Allocate,
			Static,
			Alias,
			NativeFunction,
			DeclareFunction,

			DefineConstant,
			UndefineConstant,

			MemberFunction,
			MemberVariable,

			VariableSize,
		};
		static constexpr size_t IdSize = static_cast<size_t>(Id::VariableSize);

	public:
		explicit SyntaxNode(const Id id);
		virtual ~SyntaxNode() = default;

		std::shared_ptr<SyntaxNode> Clone() const;

		bool Is(const Id id) const { return mId == id; }
		bool IsNot(const Id id) const { return mId != id; }
		Id GetId() const { return mId; }

		size_t GetMemorySize() const;

		std::shared_ptr<SyntaxNode> GetLeftNode() const { return mLeft; }
		void SetLeftNode(const std::shared_ptr<SyntaxNode>& left) { mLeft = left; }

		std::shared_ptr<SyntaxNode> GetRightNode() const { return mRight; }
		void SetRightNode(const std::shared_ptr<SyntaxNode>& right) { mRight = right; }

		std::shared_ptr<SyntaxNode> GetBodyNode() const { return mBody; }
		void SetBodyNode(const std::shared_ptr<SyntaxNode>& body) { mBody = body; }

		std::shared_ptr<SyntaxNode> GetNextNode() const { return mNext; }
		void SetNextNode(const std::shared_ptr<SyntaxNode>& next) { mNext = next; }

		std::shared_ptr<SymbolEntry> GetSymbol() const { return mSymbol; }
		std::shared_ptr<TypeDescriptor> GetTypeDescriptor() const { return mType; }
		
		IntermediateLanguage GetOpecode() const { return mCode; }
		int_t GetInt() const { return mDigit; }
		float_t GetFloat() const { return mReal; }
		const char* GetString() const { return mString; }

		const char* GetFilename() const { return mFilename; }
		int32_t GetLineno() const { return mLineno; }

		void Set(const std::shared_ptr<SymbolEntry>& symbol) { mSymbol = symbol; }
		void Set(const std::shared_ptr<TypeDescriptor>& type) { mType = type; }
		void Set(const IntermediateLanguage code) { mCode = code; }
		void Set(const int_t value) { mDigit = value; }
		void Set(const float_t value) { mReal = value; }
		void Set(const char* text) { mString = text; }

		void Dump() const;

	private:
		SyntaxNode() {}
		void OnDump(FILE* file) const;

	private:
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		char mMagic[8] = "";
#endif
		Id mId = Id::VariableSize;
		std::shared_ptr<SyntaxNode> mLeft;
		std::shared_ptr<SyntaxNode> mRight;
		std::shared_ptr<SyntaxNode> mBody;
		std::shared_ptr<SyntaxNode> mNext;

		std::shared_ptr<SymbolEntry> mSymbol;
		std::shared_ptr<TypeDescriptor> mType;

		IntermediateLanguage mCode = MANA_IL_HALT;
		int_t mDigit = 0;
		float_t mReal = 0;
		const char* mString = nullptr;

		const char* mFilename = nullptr;
		int32_t mLineno = -1;
	};
}
