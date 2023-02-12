/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "SyntaxNode.h"
#include "Lexer.h"
#include "Main.h"
#include "Symbol.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"
#include <cstdarg>
#include <cstring>

namespace mana
{
	SyntaxNode::SyntaxNode(const Id id)
		: mId(id)
	{
		mFilename = lexer_get_current_filename();
		mLineno = lexer_get_current_lineno();

#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
		static size_t count = 0;
		const std::string magic = "N" + std::to_string(count);
#if defined(MANA_TARGET_WINDOWS)
		strncpy_s(mMagic, sizeof(mMagic), magic.c_str(), sizeof(mMagic));
#else
		std::strncpy(mMagic, magic.c_str(), sizeof(mMagic));
#endif
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

	bool SyntaxNode::Is(const Id id) const
	{
		return mId == id;
	}

	bool SyntaxNode::IsNot(const Id id) const
	{
		return mId != id;
	}

	SyntaxNode::Id SyntaxNode::GetId() const
	{
		return mId;
	}

	address_t SyntaxNode::GetMemorySize() const
	{
		switch (mId)
		{
		case Id::Const:		// 定数
		case Id::Identifier:	// 変数
									// 参照のactorか、actorの実体か判定できるようにする
			return mType->Is(TypeDescriptor::Id::Actor) ? sizeof(void*) : mType->GetMemorySize();

		case Id::Array:			/* variable[argument] = */
		case Id::MemberVariable:			/* X.variable */
		case Id::Neg:			/* ±符号反転 */
			return mType->Is(TypeDescriptor::Id::Actor) ? sizeof(void*) : mType->GetMemorySize();
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

	std::shared_ptr<SyntaxNode> SyntaxNode::GetLeftNode() const
	{
		return mLeft;
	}

	void SyntaxNode::SetLeftNode(const std::shared_ptr<SyntaxNode>& left)
	{
		mLeft = left;
	}

	std::shared_ptr<SyntaxNode> SyntaxNode::GetRightNode() const
	{
		return mRight;
	}

	void SyntaxNode::SetRightNode(const std::shared_ptr<SyntaxNode>& right)
	{
		mRight = right;
	}

	std::shared_ptr<SyntaxNode> SyntaxNode::GetBodyNode() const
	{
		return mBody;
	}

	void SyntaxNode::SetBodyNode(const std::shared_ptr<SyntaxNode>& body)
	{
		mBody = body;
	}

	std::shared_ptr<SyntaxNode> SyntaxNode::GetNextNode() const
	{
		return mNext;
	}

	void SyntaxNode::SetNextNode(const std::shared_ptr<SyntaxNode>& next)
	{
		mNext = next;
	}

	std::shared_ptr<Symbol> SyntaxNode::GetSymbol() const
	{
		return mSymbol;
	}

	std::shared_ptr<TypeDescriptor> SyntaxNode::GetTypeDescriptor() const
	{
		return mType;
	}

	IntermediateLanguage SyntaxNode::SyntaxNode::GetOpecode() const
	{
		return mCode;
	}

	int_t SyntaxNode::GetInt() const
	{
		return mDigit;
	}

	float_t SyntaxNode::GetFloat() const
	{
		return mReal;
	}

	const std::string_view SyntaxNode::GetString() const
	{
		return mString;
	}

	const std::string_view SyntaxNode::GetFilename() const
	{
		return mFilename;
	}

	int32_t SyntaxNode::GetLineno() const
	{
		return mLineno;
	}

	void SyntaxNode::Set(const std::shared_ptr<Symbol>& symbol)
	{
		mSymbol = symbol;
	}

	void SyntaxNode::Set(const std::shared_ptr<TypeDescriptor>& type)
	{
		mType = type;
	}

	void SyntaxNode::Set(const IntermediateLanguage code)
	{
		mCode = code;
	}

	void SyntaxNode::Set(const int_t value)
	{
		mDigit = value;
	}

	void SyntaxNode::Set(const float_t value)
	{
		mReal = value;
	}

	void SyntaxNode::Set(const std::string_view text)
	{
		mString = text;
	}

	// node_create_cast
	std::shared_ptr<SyntaxNode> SyntaxNode::CreateCast(const std::shared_ptr<TypeDescriptor>& type, const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
	{
		if (type->GetId() == GetTypeDescriptor()->GetId())
			return shared_from_this();

		std::shared_ptr<SyntaxNode> newNode;

		if (type->GetId() == TypeDescriptor::Id::Float)
		{
			newNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::IntegerToFloat);
			newNode->mType = typeDescriptorFactory->Get(TypeDescriptor::Id::Float);
		}
		else
		{
			newNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::FloatToInteger);
			newNode->mType = typeDescriptorFactory->Get(TypeDescriptor::Id::Int);
		}

		newNode->SetLeftNode(shared_from_this());

		return newNode;
	}

	// node_cast
	std::shared_ptr<SyntaxNode> SyntaxNode::Cast(const std::shared_ptr<TypeDescriptor>& type, const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
	{
		switch (GetTypeDescriptor()->GetId())
		{
		case TypeDescriptor::Id::Char:
		case TypeDescriptor::Id::Short:
		case TypeDescriptor::Id::Int:
			if (type->GetId() == TypeDescriptor::Id::Float)
			{
				if (GetId() == SyntaxNode::Id::Const)
				{
					mReal = static_cast<float_t>(mDigit);
					mType = typeDescriptorFactory->Get(TypeDescriptor::Id::Float);
				}
				else
				{
					return CreateCast(typeDescriptorFactory->Get(TypeDescriptor::Id::Float), typeDescriptorFactory);
				}
			}
			break;

		case TypeDescriptor::Id::Float:
			switch (type->GetId())
			{
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Int:
				if (GetId() == SyntaxNode::Id::Const)
				{
					mDigit = static_cast<int_t>(mReal);
					mType = typeDescriptorFactory->Get(TypeDescriptor::Id::Int);
				}
				else
				{
					return CreateCast(typeDescriptorFactory->Get(TypeDescriptor::Id::Int), typeDescriptorFactory);
				}
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}

		return shared_from_this();
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
		static const char* name[IdSize] = {
			"Array",								//!< variable[argument] =
			"Assign",								//!< =
			"CallArgument",						//!< 引数（呼び出し）
			"DeclareArgument",					//!< 引数（宣言）
			"Const",								//!< 定数
			"Call",								//!< 関数呼び出し
			"Add",								//!< 加算
			"Sub",								//!< 減算
			"Mul",								//!< 乗算
			"Div",								//!< 除算
			"Rem",								//!< 余剰
			"Neg",								//!< ±符号反転
			"Pow",								//!< べき乗
			"Not",								//!< ~
			"And",								//!< &
			"Or",									//!< |
			"Xor",								//!< ^
			"LeftShift",							//!< <<
			"RightShift",							//!< >>
			"Less",								//!< <
			"LessEqual",							//!< <=
			"Equal",								//!< ==
			"NotEqual",							//!< !=
			"GreaterEqual",						//!< >=
			"Greater",							//!< >
			"String",								//!< 文字列
			"IntegerToFloat",						//!< 整数から実数へ変換
			"FloatToInteger",						//!< 実数から整数へ変換
			"LogicalOr",							//!< ||
			"LogicalAnd",							//!< &&
			"LogicalNot",							//!< !
			"Halt",								//!< halt
			"Yield",								//!< yield
			"Request",							//!< req
			"Accept",								//!< comply (req許可)
			"Reject",								//!< refuse (req拒否)
			"Join",								//!< join
			"Sender",								//!< sender (actor)
			"Self",								//!< self (actor)
			"Priority",							//!< priority (integer)
			"ExpressionIf",						//!< 三項演算子 '?'
			"Print",								//!< print
			"Return",								//!< return
			"Rollback",							//!< rollback

			"Block",								//!< ブロック
			"If",									//!< if
			"Switch",								//!< switch
			"Case",								//!< case
			"Default",							//!< default
			"While",								//!< while
			"Do",									//!< do while
			"For",								//!< for
			"Loop",								//!< loop
			"Lock",								//!< lock
			"Goto",								//!< goto
			"Label",								//!< label
			"Break",								//!< break
			"Continue",							//!< continue

			"Identifier",
			"TypeDescription",
			"Declarator",

			"DeclareVariable",
			"Sizeof",

			"Actor",								//!< アクターの宣言
			"Phantom",
			"Module",
			"Struct",
			"Action",
			"Extend",
			"Allocate",
			"Static",
			"NativeFunction",
			"DeclareFunction",

			"DefineConstant",
			"UndefineConstant",

			"MemberFunction",
			"MemberVariable",
		};

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		node_dump_format_(file, "\"mMagic\": \"%s\"", mMagic);
#endif

		if (static_cast<uint8_t>(mId) < (sizeof(name) / sizeof(name[0])))
		{
			node_dump_format_(file, "\"Name\": \"%s\"", name[static_cast<uint8_t>(mId)]);
		}
		else
		{
			node_dump_format_(file, "\"Name\": \"%d\"", mId);
		}

		if (!mString.empty())
		{
			node_dump_format_(file, "\"String\": \"%s\"", mString.data());
		}
		if (mType)
		{
			node_dump_format_(file, "\"Type\": \"%s\"", mType->GetName().data());
		}

		if (mLeft)
		{
			node_dump_format_(file, "\"Left\": {\n");
			node_dump_format_flag_ = false;
			mLeft->OnDump(file);
			fprintf(file, "}\n");
		}
		if (mRight)
		{
			node_dump_format_(file, "\"Right\": {\n");
			node_dump_format_flag_ = false;
			mRight->OnDump(file);
			fprintf(file, "}\n");
		}
		if (mBody)
		{
			node_dump_format_(file, "\"Body\": {\n");
			node_dump_format_flag_ = false;
			mBody->OnDump(file);
			fprintf(file, "}\n");
		}
		if (mNext)
		{
			node_dump_format_(file, "\"Next\": {\n");
			node_dump_format_flag_ = false;
			mNext->OnDump(file);
			fprintf(file, "}\n");
		}
	}
}
