/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "SemanticAnalyzer.h"
#include "Lexer.h"
#include "ErrorHandler.h"
#include "StringPool.h"

namespace mana
{
	SemanticAnalyzer::SemanticAnalyzer(
		const std::shared_ptr<SymbolFactory>& symbolFactory,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory,
		const std::shared_ptr<StringPool>& stringPool)
		: mSymbolFactory(symbolFactory)
		, mStringPool(stringPool)
		, mTypeDescriptorFactory(typeDescriptorFactory)
	{
	}

	void SemanticAnalyzer::SetCurrentFileInformation(const std::shared_ptr<SyntaxNode>& node)
	{
		MANA_ASSERT(node);
		lexer::SetCurrentFilename(node->GetFilename().data());
		lexer::SetCurrentLineNo(node->GetLineno());
	}

	bool SemanticAnalyzer::SearchSymbolFromName(const std::shared_ptr<SyntaxNode>& node)
	{
		MANA_ASSERT(node);

		if (node->GetSymbol() == nullptr)
		{
			node->Set(Lookup(node->GetString()));
			if (node->GetSymbol())
			{
				if (node->GetTypeDescriptor() == nullptr)
					node->Set(node->GetSymbol()->GetTypeDescriptor());
			}
			else
			{
				CompileError({ "incomplete type name '", node->GetString(), "'" });
				node->Set(GetTypeDescriptorFactory()->Create(TypeDescriptor::Id::Int));
				return false;
			}
		}

		return true;
	}

	void SemanticAnalyzer::ResolveTypeDescription(const std::shared_ptr<SyntaxNode>& node)
	{
		MANA_ASSERT(node);
		
		if (node->GetTypeDescriptor() == nullptr)
		{
			node->Set(Lookup(node->GetString()));
			if (node->GetSymbol())
			{
				node->Set(node->GetSymbol()->GetTypeDescriptor());
			}
			else
			{
				CompileError({ "incomplete type name '", node->GetString(), "'" });
				node->Set(GetTypeDescriptorFactory()->Create(TypeDescriptor::Id::Int));
			}
		}
	}

	void SemanticAnalyzer::ResolveDeclarator(const std::shared_ptr<SyntaxNode>& node, const bool isStaticVariable)
	{
		MANA_ASSERT(node);

		if (node->GetSymbol() == nullptr)
		{
			std::shared_ptr<TypeDescriptor> type = nullptr;
			if (node->GetLeftNode() && node->GetLeftNode()->GetId() == SyntaxNode::Id::VariableSize)
				type = ResolveVariableSize(node->GetLeftNode());

			node->Set(GetSymbolFactory()->CreateVariable(
				node->GetString(),
				type,
				isStaticVariable,
				mSymbolFactory->IsOpenBlock(),
				mSymbolFactory->IsFunctionOpened()
			));
		}
	}

	std::shared_ptr<TypeDescriptor> SemanticAnalyzer::ResolveVariableSize(const std::shared_ptr<SyntaxNode>& node)
	{
		if (node == nullptr)
			return nullptr;

		MANA_ASSERT(node->GetLeftNode() == nullptr);
		MANA_ASSERT(node->GetRightNode() == nullptr);

		if (!node->GetString().empty())
		{
			if (const std::shared_ptr<Symbol> symbol = Lookup(node->GetString()))
			{
				if (symbol->GetClassTypeId() == Symbol::ClassTypeId::ConstantInteger)
				{
					node->Set(GetTypeDescriptorFactory()->CreateArray(symbol->GetAddress()));
				}
				else
				{
					CompileError("invalid size information on parameter");
				}
			}
			else
			{
				CompileError({ "identifier ", node->GetString(), " is not defined" });
			}
		}
		else
		{
			if (node->GetInt() > 0)
				node->Set(GetTypeDescriptorFactory()->CreateArray(node->GetInt()));
			else
				CompileError("invalid size information on parameter");
		}

		if (node->GetTypeDescriptor())
		{
			node->GetTypeDescriptor()->SetTypeDescriptor(ResolveVariableSize(node->GetLeftNode()));
		}

		return node->GetTypeDescriptor();
	}

	void SemanticAnalyzer::ResolveVariableDescription(const std::shared_ptr<SyntaxNode>& node, const Symbol::MemoryTypeId memoryTypeId, const bool isStaticVariable)
	{
		MANA_ASSERT(node);
		MANA_ASSERT(node->GetLeftNode() && node->GetLeftNode()->Is(SyntaxNode::Id::TypeDescription));
		ResolveTypeDescription(node->GetLeftNode());

		MANA_ASSERT(node->GetRightNode() && node->GetRightNode()->Is(SyntaxNode::Id::Declarator));
		ResolveDeclarator(node->GetRightNode(), isStaticVariable);

		GetSymbolFactory()->AllocateMemory(node->GetRightNode()->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor(), memoryTypeId);
		//symbol_allocate_memory(node->right->symbol, node->left->type, Parameter);
	}

	void SemanticAnalyzer::ResolveTypeFromChildNode(const std::shared_ptr<SyntaxNode>& node)
	{
		MANA_ASSERT(node);

		// 自分の型が登録されていない
		if (node->GetTypeDescriptor() == nullptr)
		{
			// シンボルが登録されているなら、シンボルの型を継承する
			if (node->GetSymbol() && node->GetSymbol()->GetTypeDescriptor())
			{
				node->Set(node->GetSymbol()->GetTypeDescriptor());
			}
			else if (node->GetLeftNode())
			{
				// 左辺の型が登録されているなら、左辺の型を継承する
				if (node->GetLeftNode()->GetTypeDescriptor())
				{
					node->Set(node->GetLeftNode()->GetTypeDescriptor());
				}
				// 左辺のシンボルが登録されているなら、左辺のシンボルの型を継承する
				else if (node->GetLeftNode()->GetSymbol() && node->GetLeftNode()->GetSymbol()->GetTypeDescriptor())
				{
					node->Set(node->GetLeftNode()->GetSymbol()->GetTypeDescriptor());
				}
			}
		}
	}

	std::shared_ptr<Symbol> SemanticAnalyzer::Lookup(const std::string_view name) const
	{
		return mSymbolFactory->Lookup(name);
	}

	bool SemanticAnalyzer::EvaluateConstExpression(const std::shared_ptr<SyntaxNode>& node, ConstValue& value)
	{
		if (node == nullptr)
		{
			CompileError("constant expression required");
			return false;
		}

		auto isIntegralType = [](const TypeDescriptor::Id id)
		{
			switch (id)
			{
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Bool:
			case TypeDescriptor::Id::Int:
				return true;
			default:
				return false;
			}
		};

		auto isFloatType = [](const TypeDescriptor::Id id)
		{
			return id == TypeDescriptor::Id::Float;
		};

		auto setIntegralResult = [&](const int32_t result)
		{
			value.type = GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int);
			value.intValue = result;
		};

		switch (node->GetId())
		{
		case SyntaxNode::Id::Const:
		{
			const auto& type = node->GetTypeDescriptor();
			if (!type)
			{
				CompileError("constant expression has no type");
				return false;
			}
			switch (type->GetId())
			{
			case TypeDescriptor::Id::Float:
				value.type = type;
				value.floatValue = node->GetFloat();
				return true;
			case TypeDescriptor::Id::Bool:
				value.type = type;
				value.intValue = node->GetInt();
				return true;
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Int:
				setIntegralResult(node->GetInt());
				return true;
			case TypeDescriptor::Id::Nil:
				CompileError("Nil is not allowed in constant expressions");
				return false;
			default:
				CompileError("unsupported constant literal");
				return false;
			}
		}

		case SyntaxNode::Id::String:
			value.type = GetTypeDescriptorFactory()->GetString();
			value.stringValue = node->GetString();
			return true;

		case SyntaxNode::Id::Identifier:
		{
			const std::shared_ptr<Symbol> symbol = Lookup(node->GetString());
			if (!symbol)
			{
				CompileError({ "undefined symbol '", node->GetString(), "'" });
				return false;
			}

			switch (symbol->GetClassTypeId())
			{
			case Symbol::ClassTypeId::ConstantInteger:
				value.type = symbol->GetTypeDescriptor();
				value.intValue = symbol->GetEtc();
				return true;
			case Symbol::ClassTypeId::ConstantFloat:
				value.type = symbol->GetTypeDescriptor();
				value.floatValue = symbol->GetFloat();
				return true;
			case Symbol::ClassTypeId::ConstantString:
				value.type = symbol->GetTypeDescriptor();
				value.stringValue = symbol->GetString();
				return true;
			default:
				CompileError({ "non-constant symbol '", symbol->GetName(), "' used in constant expression" });
				return false;
			}
		}

		case SyntaxNode::Id::Neg:
		{
			ConstValue operand;
			if (!EvaluateConstExpression(node->GetLeftNode(), operand) || !operand.type)
				return false;
			const auto typeId = operand.type->GetId();
			if (isFloatType(typeId))
			{
				value.type = operand.type;
				value.floatValue = -operand.floatValue;
				return true;
			}
			if (isIntegralType(typeId))
			{
				setIntegralResult(-operand.intValue);
				return true;
			}
			CompileError("invalid operand in constant expression");
			return false;
		}

		case SyntaxNode::Id::LogicalNot:
		{
			ConstValue operand;
			if (!EvaluateConstExpression(node->GetLeftNode(), operand) || !operand.type)
				return false;
			const auto typeId = operand.type->GetId();
			if (!isIntegralType(typeId))
			{
				CompileError("invalid operand in constant expression");
				return false;
			}
			value.type = GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Bool);
			value.intValue = operand.intValue == 0 ? 1 : 0;
			return true;
		}

		case SyntaxNode::Id::Add:
		case SyntaxNode::Id::Sub:
		case SyntaxNode::Id::Mul:
		case SyntaxNode::Id::Div:
		case SyntaxNode::Id::Rem:
		{
			ConstValue left;
			ConstValue right;
			if (!EvaluateConstExpression(node->GetLeftNode(), left) || !EvaluateConstExpression(node->GetRightNode(), right))
				return false;

			const auto leftTypeId = left.type ? left.type->GetId() : TypeDescriptor::Id::Incomplete;
			const auto rightTypeId = right.type ? right.type->GetId() : TypeDescriptor::Id::Incomplete;

			if (isFloatType(leftTypeId) && isFloatType(rightTypeId))
			{
				value.type = left.type;
				switch (node->GetId())
				{
				case SyntaxNode::Id::Add:
					value.floatValue = left.floatValue + right.floatValue;
					break;
				case SyntaxNode::Id::Sub:
					value.floatValue = left.floatValue - right.floatValue;
					break;
				case SyntaxNode::Id::Mul:
					value.floatValue = left.floatValue * right.floatValue;
					break;
				case SyntaxNode::Id::Div:
					value.floatValue = left.floatValue / right.floatValue;
					break;
				case SyntaxNode::Id::Rem:
					CompileError("remainder is not supported for float constant expressions");
					return false;
				default:
					break;
				}
				return true;
			}

			if (isIntegralType(leftTypeId) && isIntegralType(rightTypeId))
			{
				switch (node->GetId())
				{
				case SyntaxNode::Id::Add:
					setIntegralResult(left.intValue + right.intValue);
					break;
				case SyntaxNode::Id::Sub:
					setIntegralResult(left.intValue - right.intValue);
					break;
				case SyntaxNode::Id::Mul:
					setIntegralResult(left.intValue * right.intValue);
					break;
				case SyntaxNode::Id::Div:
					setIntegralResult(left.intValue / right.intValue);
					break;
				case SyntaxNode::Id::Rem:
					setIntegralResult(left.intValue % right.intValue);
					break;
				default:
					break;
				}
				return true;
			}

			CompileError("invalid operands in constant expression");
			return false;
		}

		case SyntaxNode::Id::Greater:
		case SyntaxNode::Id::GreaterEqual:
		case SyntaxNode::Id::Less:
		case SyntaxNode::Id::LessEqual:
		case SyntaxNode::Id::Equal:
		case SyntaxNode::Id::NotEqual:
		{
			ConstValue left;
			ConstValue right;
			if (!EvaluateConstExpression(node->GetLeftNode(), left) || !EvaluateConstExpression(node->GetRightNode(), right))
				return false;

			const auto leftTypeId = left.type ? left.type->GetId() : TypeDescriptor::Id::Incomplete;
			const auto rightTypeId = right.type ? right.type->GetId() : TypeDescriptor::Id::Incomplete;
			bool result = false;

			if (isFloatType(leftTypeId) && isFloatType(rightTypeId))
			{
				switch (node->GetId())
				{
				case SyntaxNode::Id::Greater:
					result = left.floatValue > right.floatValue;
					break;
				case SyntaxNode::Id::GreaterEqual:
					result = left.floatValue >= right.floatValue;
					break;
				case SyntaxNode::Id::Less:
					result = left.floatValue < right.floatValue;
					break;
				case SyntaxNode::Id::LessEqual:
					result = left.floatValue <= right.floatValue;
					break;
				case SyntaxNode::Id::Equal:
					result = left.floatValue == right.floatValue;
					break;
				case SyntaxNode::Id::NotEqual:
					result = left.floatValue != right.floatValue;
					break;
				default:
					break;
				}
			}
			else if (isIntegralType(leftTypeId) && isIntegralType(rightTypeId))
			{
				switch (node->GetId())
				{
				case SyntaxNode::Id::Greater:
					result = left.intValue > right.intValue;
					break;
				case SyntaxNode::Id::GreaterEqual:
					result = left.intValue >= right.intValue;
					break;
				case SyntaxNode::Id::Less:
					result = left.intValue < right.intValue;
					break;
				case SyntaxNode::Id::LessEqual:
					result = left.intValue <= right.intValue;
					break;
				case SyntaxNode::Id::Equal:
					result = left.intValue == right.intValue;
					break;
				case SyntaxNode::Id::NotEqual:
					result = left.intValue != right.intValue;
					break;
				default:
					break;
				}
			}
			else
			{
				CompileError("invalid operands in constant expression");
				return false;
			}

			value.type = GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Bool);
			value.intValue = result ? 1 : 0;
			return true;
		}

		case SyntaxNode::Id::LogicalAnd:
		case SyntaxNode::Id::LogicalOr:
		{
			ConstValue left;
			ConstValue right;
			if (!EvaluateConstExpression(node->GetLeftNode(), left) || !EvaluateConstExpression(node->GetRightNode(), right))
				return false;

			const auto leftTypeId = left.type ? left.type->GetId() : TypeDescriptor::Id::Incomplete;
			const auto rightTypeId = right.type ? right.type->GetId() : TypeDescriptor::Id::Incomplete;
			if (!isIntegralType(leftTypeId) || !isIntegralType(rightTypeId))
			{
				CompileError("invalid operands in constant expression");
				return false;
			}

			value.type = GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Bool);
			if (node->GetId() == SyntaxNode::Id::LogicalAnd)
				value.intValue = (left.intValue != 0 && right.intValue != 0) ? 1 : 0;
			else
				value.intValue = (left.intValue != 0 || right.intValue != 0) ? 1 : 0;
			return true;
		}

		default:
			break;
		}

		CompileError("expression is not a constant expression");
		return false;
	}

	bool SemanticAnalyzer::IsConstTypeCompatible(const std::shared_ptr<TypeDescriptor>& declaredType, const std::shared_ptr<TypeDescriptor>& expressionType) const
	{
		if (!declaredType || !expressionType)
			return false;

		if (declaredType->Compare(expressionType))
			return true;

		CompileError("incompatible type in constant initializer");
		return false;
	}

	std::shared_ptr<Symbol> SemanticAnalyzer::CreateConstSymbol(const std::string_view name, const std::shared_ptr<TypeDescriptor>& declaredType, const ConstValue& value)
	{
		if (!declaredType)
			return nullptr;

		const auto& stringType = GetTypeDescriptorFactory()->GetString();
		if (declaredType->Compare(stringType))
		{
			auto symbol = mSymbolFactory->CreateConstString(name, value.stringValue);
			symbol->SetTypeDescription(stringType);
			return symbol;
		}

		switch (declaredType->GetId())
		{
		case TypeDescriptor::Id::Float:
		{
			auto symbol = mSymbolFactory->CreateConstFloat(name, value.floatValue);
			symbol->SetTypeDescription(declaredType);
			return symbol;
		}
		case TypeDescriptor::Id::Bool:
		{
			auto symbol = mSymbolFactory->CreateConstBool(name, value.intValue != 0);
			symbol->SetTypeDescription(declaredType);
			return symbol;
		}
		case TypeDescriptor::Id::Char:
		case TypeDescriptor::Id::Short:
		case TypeDescriptor::Id::Int:
		{
			auto symbol = mSymbolFactory->CreateConstInt(name, value.intValue);
			symbol->SetTypeDescription(declaredType);
			return symbol;
		}
		default:
			break;
		}

		CompileError("invalid constant type");
		return nullptr;
	}

	const std::shared_ptr<SymbolFactory>& SemanticAnalyzer::GetSymbolFactory() const
	{
		return mSymbolFactory;
	}

	const std::shared_ptr<StringPool>& SemanticAnalyzer::GetStringPool() const
	{
		return mStringPool;
	}

	const std::shared_ptr<TypeDescriptorFactory>& SemanticAnalyzer::GetTypeDescriptorFactory() const
	{
		return mTypeDescriptorFactory;
	}
}
