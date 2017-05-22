/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "SyntaxNode.h"
#include "Lexer.h"
#include "Main.h"
#include "SymbolEntry.h"
#include "SystemHolder.h"
#include "TypeDescriptor.h"
#include <cstdarg>
#include <cstring>

namespace mana
{
	SyntaxNode::SyntaxNode(const Id id)
		: mId(id)
	{
		GetSystemHolder().GetSyntaxNodeHolder()->Push(this);

		mFilename = lexer_get_current_filename();
		mLineno = lexer_get_current_lineno();

#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
		static size_t count = 0;
		const std::string magic = "N" + std::to_string(count);
		std::strncpy(mMagic, magic.c_str(), sizeof(mMagic));
		++count;
#endif
	}

	std::shared_ptr<SyntaxNode> SyntaxNode::Clone() const
	{
		std::shared_ptr<SyntaxNode> self = std::make_shared<SyntaxNode>(mId);

		if (mLeft != nullptr)
			self->mLeft = mLeft->Clone();
		if (mRight != nullptr)
			self->mRight = mRight->Clone();
		if (mBody != nullptr)
			self->mBody = mBody->Clone();

		self->mSymbol = mSymbol;
		self->mType = mType;
		self->mCode = mCode;
		self->mDigit = mDigit;
		self->mReal = mReal;
		self->mString = mString;

		return self;
	}

	size_t SyntaxNode::GetMemorySize() const
	{
		switch (mId)
		{
		case Id::Const:		// 定数
		case Id::Identifier:	// 変数
									// 参照のactorか、actorの実体か判定できるようにする
			return mType->GetId() == TypeDescriptor::Id::ACTOR ? sizeof(void*) : mType->GetMemorySize();

		case Id::Array:			/* variable[argument] = */
		case Id::MemberVariable:			/* X.variable */
		case Id::Neg:			/* ±符号反転 */
			return mType->GetId() == TypeDescriptor::Id::ACTOR ? sizeof(void*) : mType->GetMemorySize();
			/*		return node_get_memory_size(self->left);	*/

		case Id::Self:			/* self (actor) */
		case Id::Priority:		/* runlevel (integer) */
			return sizeof(void*);

		case Id::Assign:			/* = */
		case Id::CallArgument:		/* 呼び出し側引数 */
		case Id::Call:			/* 関数呼び出し */
		case Id::Add:			/* 加算 */
		case Id::Sub:			/* 減算 */
		case Id::Mul:			/* 乗算 */
		case Id::Div:			/* 除算 */
		case Id::Rem:			/* 余剰 */
		case Id::Pow:			/* べき乗 */
		case Id::Not:			/* ~ */
		case Id::LogicalNot:			/* ! */
		case Id::And:			/* & */
		case Id::Or:				/* | */
		case Id::Xor:			/* ^ */
		case Id::LeftShift:			/* << */
		case Id::RightShift:			/* >> */
		case Id::Less:				/* < */
		case Id::LessEqual:				/* <= */
		case Id::Equal:				/* == */
		case Id::NotEqual:				/* != */
		case Id::GreaterEqual:				/* >= */
		case Id::Greater:				/* > */
		case Id::String:			/* 文字列 */
		case Id::IntegerToFloat:			/* 整数から実数へ変換 */
		case Id::FloatToInteger:			/* 実数から整数へ変換 */
		case Id::LogicalOr:			/* || */
		case Id::LogicalAnd:			/* && */
		case Id::Sender:			/* sender (actor) */
		case Id::ExpressionIf:	/* 三項演算子 '?' */
		default:
#if 1
			return mType->GetMemorySize();
#else
			mana_compile_error("illigal node type detect");
			return 0;
#endif
		}
	}


	static bool node_dump_format_flag_ = false;

	static void node_dump_format_(FILE* file, const char* format, ...)
	{
		if (node_dump_format_flag_)
		{
			node_dump_format_flag_ = false;
			fputc(',', file);
		}

		va_list arg;
		va_start(arg, format);
#if __STDC_WANT_SECURE_LIB__
		vfprintf_s(file, format, arg);
#else
		vfprintf(file, format, arg);
#endif
		va_end(arg);

		node_dump_format_flag_ = true;
	}

	void SyntaxNode::Dump() const
	{
		FILE* file;
#if defined(__STDC_WANT_SECURE_LIB__)
		if (fopen_s(&file, "mana_node_dump.json", "wt") == 0)
#else
		file = fopen("mana_node_dump.json", "wt");
		if (file)
#endif
		{
			fputc('{', file);
			OnDump(file);
			fputc('}', file);
			fclose(file);
		}
	}
	
	void SyntaxNode::OnDump(FILE* file) const
	{
		// TODO:Idと並びを合わせてください
		static const char* name[] = {
			"NEWLINE",

			"Array",								/*!< variable[argument] = */
			"ASSIGN",								/*!< = */
			"MEMOP",								/*!< X.variable */
			"CALL_ARGUMENT",							/*!< 呼び出し側引数 */
			"DECLARE_ARGUMENT",							/*!< 呼び出し側引数 */
			"CONST",								/*!< 定数 */
			"VARIABLE",							/*!< 変数 */
			"CALL",								/*!< 関数呼び出し */
			"ADD",									/*!< 加算 */
			"SUB",									/*!< 減算 */
			"MUL",									/*!< 乗算 */
			"DIV",									/*!< 除算 */
			"REM",									/*!< 余剰 */
			"NEG",									/*!< ±符号反転 */
			"POW",									/*!< べき乗 */
			"NOT",									/*!< ~ */
			"AND",									/*!< & */
			"OR",									/*!< | */
			"XOR",									/*!< ^ */
			"LSH",									/*!< << */
			"RSH",									/*!< >> */
			"LS",									/*!< < */
			"LE",									/*!< <= */
			"EQ",									/*!< == */
			"NE",									/*!< != */
			"GE",									/*!< >= */
			"GT",									/*!< > */
			"STRING",								/*!< 文字列 */
			"I2F",									/*!< 整数から実数へ変換 */
			"F2I",									/*!< 実数から整数へ変換 */
			"LOR",									/*!< || */
			"LAND",								/*!< && */
			"LNOT",								/*!< ! */
			"HALT",								/*!< halt */
			"YIELD",								/*!< yield */
			"REQUEST",								/*!< req */
			"COMPLY",								/*!< comply (req許可) */
			"REFUSE",								/*!< refuse (req拒否) */
			"JOIN",								/*!< join */
			"SENDER",								/*!< sender (actor) */
			"SELF",								/*!< self (actor) */
			"PRIORITY",							/*!< priority (integer) */
			"EXPRESSION_IF",						/*!< 三項演算子 '?' */
			"PRINT",								/*!< print */
			"RETURN",								/*!< return */
			"ROLLBACK",							/*!< rollback */

			"BLOCK",								/*!< ブロック */
			"IF",									/*!< if */
			"SWITCH",								/*!< switch */
			"CASE",								/*!< case */
			"DEFAULT",								/*!< default */
			"WHILE",								/*!< while */
			"DO",									/*!< do while */
			"FOR",									/*!< for */
			"LOOP",								/*!< loop */
			"LOCK",								/*!< lock */
			"GOTO",								/*!< goto */
			"LABEL",								/*!< label */
			"BREAK",								/*!< break */
			"CONTINUE",							/*!< continue */

			"IDENTIFIER",
			"TYPE_DESCRIPTION",
			"DECLARATOR",

			"VARIABLE_DECLARATION",
			"SIZEOF",

			"DECLARE_ACTOR",						//!< アクターの宣言
			"DECLARE_PHANTOM",
			"DECLARE_MODULE",
			"DECLARE_STRUCT",
			"DECLARE_ACTION",
			"DECLARE_EXTEND",
			"DECLARE_ALLOCATE",
			"DECLARE_STATIC",
			"DECLARE_ALIAS",
			"DECLARE_NATIVE_FUNCTION",
			"DECLARE_FUNCTION",

			"DEFINE_CONSTANT",
			"UNDEFINE_CONSTANT",

			"MEMBER_FUNCTION",
			"MEMBER_VARIABLE",
		};

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		node_dump_format_(file, "\"mMagic\": \"%s\"", mMagic);
#endif

		if (static_cast<uint8_t>(mId) < (sizeof(name) / sizeof(name[0])))
		{
			node_dump_format_(file, "\"name\": \"%s\"", name[static_cast<uint8_t>(mId)]);
		}
		else
		{
			node_dump_format_(file, "\"name\": \"%d\"", mId);
		}

		if (mString)
			node_dump_format_(file, "\"mString\": \"%s\"", mString);
		if (mType)
			node_dump_format_(file, "\"mType\": \"%s\"", mType->GetName());

		if (mLeft)
		{
			node_dump_format_(file, "\"mLeft\": {\n");
			node_dump_format_flag_ = false;
			mLeft->OnDump(file);
			fprintf(file, "}\n");
		}
		if (mRight)
		{
			node_dump_format_(file, "\"mRight\": {\n");
			node_dump_format_flag_ = false;
			mRight->OnDump(file);
			fprintf(file, "}\n");
		}
		if (mBody)
		{
			node_dump_format_(file, "\"mBody\": {\n");
			node_dump_format_flag_ = false;
			mBody->OnDump(file);
			fprintf(file, "}\n");
		}
		if (mNext)
		{
			node_dump_format_(file, "\"mNext\": {\n");
			node_dump_format_flag_ = false;
			mNext->OnDump(file);
			fprintf(file, "}\n");
		}
	}
}
