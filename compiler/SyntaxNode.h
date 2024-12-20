/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include <memory>
#include <vector>

namespace mana
{
	class Symbol;
	class TypeDescriptor;
	class TypeDescriptorFactory;

	/*!
	構文木クラス
	*/
	class SyntaxNode final : public std::enable_shared_from_this<SyntaxNode>
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
		~SyntaxNode() = default;

		std::shared_ptr<SyntaxNode> Clone() const;

		bool Is(const Id id) const;
		bool IsNot(const Id id) const;
		Id GetId() const;

		address_t GetMemorySize() const;

		std::shared_ptr<SyntaxNode> GetLeftNode() const;
		void SetLeftNode(const std::shared_ptr<SyntaxNode>& left);

		std::shared_ptr<SyntaxNode> GetRightNode() const;
		void SetRightNode(const std::shared_ptr<SyntaxNode>& right);

		std::shared_ptr<SyntaxNode> GetBodyNode() const;
		void SetBodyNode(const std::shared_ptr<SyntaxNode>& body);

		std::shared_ptr<SyntaxNode> GetNextNode() const;
		void SetNextNode(const std::shared_ptr<SyntaxNode>& next);

		std::shared_ptr<Symbol> GetSymbol() const;
		std::shared_ptr<TypeDescriptor> GetTypeDescriptor() const;
		
		IntermediateLanguage GetOpecode() const;
		int_t GetInt() const;
		float_t GetFloat() const;
		const std::string_view& GetString() const;

		const std::string_view& GetFilename() const;
		int32_t GetLineno() const;

		void Set(const std::shared_ptr<Symbol>& symbol);
		void Set(const std::shared_ptr<TypeDescriptor>& type);
		void Set(const IntermediateLanguage code);
		void Set(const int_t value);
		void Set(const float_t value);
		void Set(const std::string_view text);

		std::shared_ptr<SyntaxNode> Cast(const std::shared_ptr<TypeDescriptor>& type, const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);

		void Dump() const;

	private:
		SyntaxNode() = default;

		std::shared_ptr<SyntaxNode> CreateCast(const std::shared_ptr<TypeDescriptor>& type, const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory);
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

		std::shared_ptr<Symbol> mSymbol;
		std::shared_ptr<TypeDescriptor> mType;

		IntermediateLanguage mCode = IntermediateLanguage::Halt;
		int_t mDigit = 0;
		float_t mReal = 0;
		std::string_view mString;

		std::string_view mFilename;
		int32_t mLineNo = -1;
	};
}
