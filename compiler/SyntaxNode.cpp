/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "SyntaxNode.h"
#include "Lexer.h"
#include "Symbol.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"

namespace mana
{
	SyntaxNode::SyntaxNode(const Id id)
		: mId(id)
		, mLineNo(static_cast<int16_t>(lexer::GetCurrentLineNo()))
		, mFilename(lexer::GetCurrentFilename())
	{
		static uint32_t count = 0;
		mMagic = ++count;
	}

	std::shared_ptr<SyntaxNode> SyntaxNode::Clone() const
	{
		auto self = std::make_shared<SyntaxNode>(mId);

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
		case Id::Const:				// 定数
		case Id::Identifier:		// 変数
			// 参照のactorか、actorの実体か判定できるようにする
			return mType->Is(TypeDescriptor::Id::Actor) ? sizeof(void*) : mType->GetMemorySize();

		case Id::Array:				// variable[argument] =
		case Id::MemberVariable:	// X.variable
		case Id::Neg:				// ±符号反転
			return mType->Is(TypeDescriptor::Id::Actor) ? sizeof(void*) : mType->GetMemorySize();
			// return node_get_memory_size(self->left);

		case Id::Self:				// self (actor)
		case Id::Priority:			// runlevel (integer)
			return sizeof(void*);

		case Id::Assign:			// =
		case Id::CallArgument:		// 呼び出し側引数
		case Id::Call:				// 関数呼び出し
		case Id::Add:				// 加算
		case Id::Sub:				// 減算
		case Id::Mul:				// 乗算
		case Id::Div:				// 除算
		case Id::Rem:				// 余剰
		case Id::Pow:				// べき乗
		case Id::Not:				// ~
		case Id::LogicalNot:		// !
		case Id::And:				// &
		case Id::Or:				// |
		case Id::Xor:				// ^
		case Id::LeftShift:			// <<
		case Id::RightShift:		// >>
		case Id::Less:				// <
		case Id::LessEqual:			// <=
		case Id::Equal:				// ==
		case Id::NotEqual:			// !=
		case Id::GreaterEqual:		// >=
		case Id::Greater:			// >
		case Id::String:			// 文字列
		case Id::IntegerToFloat:	// 整数から実数へ変換
		case Id::FloatToInteger:	// 実数から整数へ変換
		case Id::LogicalOr:			// ||
		case Id::LogicalAnd:		// &&
		case Id::Sender:			// sender (actor)
		case Id::ExpressionIf:		// 三項演算子 '?'
		default:
			return mType->GetMemorySize();
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

	const std::string_view& SyntaxNode::GetString() const
	{
		return mString;
	}

	const std::string_view& SyntaxNode::GetFilename() const
	{
		return mFilename;
	}

	int32_t SyntaxNode::GetLineno() const
	{
		return mLineNo;
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
			newNode->mType = type;
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
		case TypeDescriptor::Id::Bool:
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
			case TypeDescriptor::Id::Bool:
			case TypeDescriptor::Id::Int:
				if (GetId() == SyntaxNode::Id::Const)
				{
					mDigit = static_cast<int_t>(mReal);
					mType = type->GetId() == TypeDescriptor::Id::Bool ? typeDescriptorFactory->Get(TypeDescriptor::Id::Bool) : typeDescriptorFactory->Get(TypeDescriptor::Id::Int);
				}
				else
				{
					const auto targetType = type->GetId() == TypeDescriptor::Id::Bool ? typeDescriptorFactory->Get(TypeDescriptor::Id::Bool) : typeDescriptorFactory->Get(TypeDescriptor::Id::Int);
					return CreateCast(targetType, typeDescriptorFactory);
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

	std::string SyntaxNode::GetMagic() const
	{
		return "N" + std::to_string(mMagic);
	}

	void SyntaxNode::Dump(std::ofstream& output) const
	{
		output << "```mermaid\n";
		output << "flowchart TD\n";
		DumpRecursive(output);
		output << "```\n";
	}
	
	void SyntaxNode::DumpRecursive(std::ofstream& output) const
	{
		// Idと並びを合わせてください
		static const char* name[] = {
			"Array",
			"Assign",
			"CallArgument",
			"DeclareArgument",
			"Const",
			"Call",
			"Add",
			"Sub",
			"Mul",
			"Div",
			"Rem",
			"Neg",
			"Pow",
			"Not",
			"And",
			"Or",
			"Xor",
			"LeftShift",
			"RightShift",
			"Less",
			"LessEqual",
			"Equal",
			"NotEqual",
			"GreaterEqual",
			"Greater",
			"String",
			"IntegerToFloat",
			"FloatToInteger",
			"LogicalOr",
			"LogicalAnd",
			"LogicalNot",
			"Halt",
			"Yield",
			"Request",
			"AwaitStart",
			"AwaitCompletion",
			"Accept",
			"Reject",
			"Join",
			"Sender",
			"Self",
			"Priority",
			"ExpressionIf",
			"Print",
			"Return",
			"Rollback",

			"Block",
			"If",
			"Switch",
			"Case",
			"Default",
			"While",
			"Do",
			"For",
			"Loop",
			"Lock",
			"Goto",
			"Label",
			"Break",
			"Continue",

			"Identifier",
			"TypeDescription",
			"Declarator",

			"DeclareVariable",
			"Sizeof",

			"Actor",
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
		constexpr size_t idNameSize = std::size(name);
		static_assert(idNameSize == IdSize);

		const auto magic = GetMagic();
		output << magic << "[\n";
		output << "mMagic: " << magic << "\n";

		if (static_cast<uint8_t>(mId) < idNameSize)
		{
			output << "Name: " << name[static_cast<uint8_t>(mId)] << "\n";
		}
		else
		{
			output << "Name: invalid node id\n";
		}

		if (!mString.empty())
		{
			output << "String: " << mString << "\n";
		}
		if (mType)
		{
			output << "Type: " << mType->GetName() << "\n";
		}
		output << "]\n";

		if (mLeft)
		{
			mLeft->DumpRecursive(output);
			output << magic << " --> " << mLeft->GetMagic() << "\n";
		}
		if (mRight)
		{
			mRight->DumpRecursive(output);
			output << magic << " --> " << mRight->GetMagic() << "\n";
		}
		if (mBody)
		{
			mBody->DumpRecursive(output);
			output << magic << " --> " << mBody->GetMagic() << "\n";
		}
		if (mNext)
		{
			mNext->DumpRecursive(output);
			output << magic << " --> " << mNext->GetMagic() << "\n";
		}
	}
}
