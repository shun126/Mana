/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "LocalSemanticAnalyzer.h"
#include "ErrorHandler.h"
#include "SyntaxNode.h"

namespace mana
{
	LocalSemanticAnalyzer::LocalSemanticAnalyzer(
		const std::shared_ptr<SymbolFactory>& symbolFactory,
		const std::shared_ptr<SymbolTable>& symbolTable,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
		: SemanticAnalyzer(symbolFactory, symbolTable, typeDescriptorFactory)
	{
	}

	LocalSemanticAnalyzer::~LocalSemanticAnalyzer()
	{
	}

	bool LocalSemanticAnalyzer::GetNodeType(TypeDescriptor::Id* t1, TypeDescriptor::Id* t2, const std::shared_ptr<const SyntaxNode>& node)
	{
		MANA_ASSERT(node);
		MANA_ASSERT(t1);
		MANA_ASSERT(t2);

		if (node->GetLeftNode() == nullptr || node->GetLeftNode()->GetTypeDescriptor() == nullptr)
		{
			CompileError("illegal GetLeftNode()-hand side expression");
			return false;
		}

		if (node->GetRightNode() != nullptr && node->GetRightNode()->GetTypeDescriptor() == nullptr)
		{
			CompileError("illegal GetRightNode()-hand side expression");
			return false;
		}

		*t1 = (node->GetLeftNode()->GetTypeDescriptor())->GetId();
		*t2 = node->GetRightNode() ? (node->GetRightNode()->GetTypeDescriptor())->GetId() : TypeDescriptor::Id::Int;

		return true;
	}

	/*!
	自動型変換ノードを挿入します
	@param[in]	node	ノードオブジェクト
	*/
	void LocalSemanticAnalyzer::AutoCast(const std::shared_ptr<SyntaxNode>& node)
	{
		if (node == nullptr)
			return;

		ResolveTypeFromChildNode(node);

		if (node->IsNot(SyntaxNode::Id::Assign) && node->GetRightNode() && node->GetRightNode()->GetTypeDescriptor() && node->GetRightNode()->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::Float)
		{
			node->Set(node->GetRightNode()->GetTypeDescriptor());
		}

		if (node->GetLeftNode())
		{
			if (node->GetTypeDescriptor()->GetId() != node->GetLeftNode()->GetTypeDescriptor()->GetId())
				node->SetLeftNode(node->GetLeftNode()->Cast(node->GetTypeDescriptor(), GetTypeDescriptorFactory()));
		}
		if (node->GetRightNode())
		{
			if (node->GetTypeDescriptor()->GetId() != node->GetRightNode()->GetTypeDescriptor()->GetId())
				node->SetRightNode(node->GetRightNode()->Cast(node->GetTypeDescriptor(), GetTypeDescriptorFactory()));
		}
	}

	void LocalSemanticAnalyzer::PostResolverResolve(std::shared_ptr<SyntaxNode> node)
	{
		if (node == nullptr)
			return;
DO_RECURSIVE:
		SetCurrentFileInfomation(node);

		switch (node->GetId())
		{
			///////////////////////////////////////////////////////////////////////
			// 定数定義に関するノード
		case SyntaxNode::Id::Alias:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DefineConstant:
			// TODO:将来的には対応して下さい
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::UndefineConstant:
			// TODO:将来的には対応して下さい
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// メモリレイアウトに関するノード									
		case SyntaxNode::Id::Allocate:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Static:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 構造に関するノード
		case SyntaxNode::Id::Actor:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Extend:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Module:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Phantom:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Struct:
			/*
			TODO:ローカルスコープ内で宣言された時のみ有効にして下さい
			symbol_open_structure();
			PostResolverResolve(node->GetLeftNode());
			symbol_close_structure(node->GetString());
			*/
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 関数宣言に関するノード									
		case SyntaxNode::Id::Action:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareArgument:
			// node->GetLeftNode()
			// node->GetRightNode()
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareFunction:
			// node->GetLeftNode()
			// node->GetRightNode()
			// node->GetBodyNode()
			break;

		case SyntaxNode::Id::NativeFunction:
			// node->GetLeftNode()
			// node->GetRightNode()
			// node->GetBodyNode()
			break;

			///////////////////////////////////////////////////////////////////////
			// 変数宣言に関するノード									
		case SyntaxNode::Id::Declarator:
			if (node->GetSymbol() == nullptr)
				node->Set(GetSymbolFactory()->CreateVariable(
					node->GetString(),
					nullptr,
					GetSymbolTable()->IsOpenBlock(),
					GetSymbolTable()->IsFunctionOpened(),
					false
				));
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareVariable:
			ResolveVariableDescription(node, Symbol::MemoryTypeId::NORMAL, false);
			// node->GetLeftNode()
			// node->GetRightNode()
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::TypeDescription:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			ResolveTypeDescription(node);
			break;

		case SyntaxNode::Id::VariableSize:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// ブロックを伴う制御に関するノード
		case SyntaxNode::Id::Block:
		{
			const int32_t in_depth = GetSymbolTable()->OpenBlock(false);
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			const int32_t out_depth = GetSymbolTable()->CloseBlock();
			MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Break:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Case:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Continue:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Default:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::Do:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::For:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::Goto:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Halt:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::If:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::Label:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Lock:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Loop:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Return:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Rollback:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Switch:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::While:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 制御に関するノード
		case SyntaxNode::Id::Accept:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Join:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Print:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Reject:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Request:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Yield:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 二項演算子に関するノード
		case SyntaxNode::Id::Add:
		case SyntaxNode::Id::Sub:
		case SyntaxNode::Id::Mul:
		case SyntaxNode::Id::Div:
		case SyntaxNode::Id::Rem:
		case SyntaxNode::Id::Pow:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Float || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Float)
					{
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
					}
					else
					{
						AutoCast(node);
						TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					}
				}
			}
			break;

		case SyntaxNode::Id::And:
		case SyntaxNode::Id::Or:
		case SyntaxNode::Id::Xor:
		case SyntaxNode::Id::LeftShift:
		case SyntaxNode::Id::RightShift:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Int)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
				}
			}
			break;

		case SyntaxNode::Id::LogicalAnd:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Int)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
					// TODO
					//node->Set( = MANA_IL_LAND;
				}
			}
			break;

		case SyntaxNode::Id::LogicalOr:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Int)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
					node->Set(MANA_IL_LOR);
				}
			}
			break;

		case SyntaxNode::Id::Equal:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::ACTOR:
						node->Set(MANA_IL_COMPARE_EQ_INTEGER);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(MANA_IL_COMPARE_EQ_FLOAT);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(MANA_IL_COMPARE_EQ_DATA);
						break;
					default:
						MANA_BUG("illigal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::GreaterEqual:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::ACTOR:
						node->Set(MANA_IL_COMPARE_GE_INTEGER);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(MANA_IL_COMPARE_GE_FLOAT);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(MANA_IL_COMPARE_GE_DATA);
						break;
					default:
						MANA_BUG("illigal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::Greater:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::ACTOR:
						node->Set(MANA_IL_COMPARE_GT_INTEGER);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(MANA_IL_COMPARE_GT_FLOAT);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(MANA_IL_COMPARE_GT_DATA);
						break;
					default:
						MANA_BUG("illigal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::LessEqual:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::ACTOR:
						node->Set(MANA_IL_COMPARE_LE_INTEGER);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(MANA_IL_COMPARE_LE_FLOAT);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(MANA_IL_COMPARE_LE_DATA);
						break;
					default:
						MANA_BUG("illigal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::Less:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::ACTOR:
						node->Set(MANA_IL_COMPARE_LS_INTEGER);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(MANA_IL_COMPARE_LS_FLOAT);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(MANA_IL_COMPARE_LS_DATA);
						break;
					default:
						MANA_BUG("illigal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::NotEqual:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::ACTOR:
						node->Set(MANA_IL_COMPARE_NE_INTEGER);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(MANA_IL_COMPARE_NE_FLOAT);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(MANA_IL_COMPARE_NE_DATA);
						break;
					default:
						MANA_BUG("illigal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// 単項演算子に関するノード
		case SyntaxNode::Id::LogicalNot:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
					node->Set(MANA_IL_LNOT);
				}
			}
			break;

		case SyntaxNode::Id::Not:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
					node->Set(MANA_IL_NOT);
				}
			}
			break;

		case SyntaxNode::Id::Neg:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Float)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// 演算に関するノード
		case SyntaxNode::Id::Array:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t2 == TypeDescriptor::Id::Void || t2 > TypeDescriptor::Id::Float)
						CompileError("non-integer expression used as subscript");
					if (t1 != TypeDescriptor::Id::Array)
						CompileError("subscript specified to non-array");
					else
					{
						if (node->GetRightNode()->Is(SyntaxNode::Id::Const))
						{
							MANA_ASSERT(
								node->GetRightNode()->GetTypeDescriptor() == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char) ||
								node->GetRightNode()->GetTypeDescriptor() == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Short) ||
								node->GetRightNode()->GetTypeDescriptor() == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));

							if (node->GetRightNode()->GetInt() >= (node->GetLeftNode()->GetTypeDescriptor())->GetArraySize())
								CompileError("subscript range over");
						}

						node->Set(node->GetLeftNode()->GetTypeDescriptor()->GetComponent());
					}
				}
			}
			break;

		case SyntaxNode::Id::Assign:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);

			if (node->GetLeftNode()->Is(SyntaxNode::Id::Const))
			{
				CompileError("already initialized constant '%s'", node->GetLeftNode()->GetSymbol()->GetName());
				return;
			}
			AutoCast(node);
			TypeDescriptorFactory::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
			break;

			break;

		case SyntaxNode::Id::Call:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				SearchSymbolFromName(node);
#if 0
				mana_generator_event_funtion_type* function = mana_hash_get(mana_generator_object.event_hash, node->GetSymbol()->name);
				if (function)
				{
					node = (*function)(node);
				}

				if (node->GetSymbol()->class_type != ClassTypeId::FUNCTION &&
					node->GetSymbol()->class_type != ClassTypeId::NATIVE_FUNCTION &&
					node->GetSymbol()->class_type != ClassTypeId::MEMBER_FUNCTION)
				{
					CompileError("trying to call non-funcation");
				}
#endif
			}
			break;

		case SyntaxNode::Id::CallArgument:
			PostResolverResolve(node->GetLeftNode());
			if (node->GetRightNode())
			{
				PostResolverResolve(node->GetRightNode());
				//node->Set( = node->GetRightNode()->Is(SyntaxNode::Id::CallArgument) ? node->GetRightNode()->Set( + 1 : 2;
			}
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Const:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::ExpressionIf:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::IntegerToFloat:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
				}
			}
			break;

		case SyntaxNode::Id::FloatToInteger:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 != TypeDescriptor::Id::Float)
						CompileError("imcompatible GetTypeDescriptor() operation in expression");
				}
			}
			break;

		case SyntaxNode::Id::Identifier:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);

			if (SearchSymbolFromName(node))
			{
				if (node->GetSymbol())
					node->Set(node->GetSymbol()->GetTypeDescriptor());

				switch (node->GetSymbol()->GetClassTypeId())
				{
				case Symbol::ClassTypeId::ALIAS:
					/*TODO
					if (!node->GetSymbol()->IsUsed())
					{
						node->GetSymbol()->SetAddress(Append(node->GetSymbol()->GetString()));
						node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
						node->GetSymbol()->SetUsed(true);
					}
					*/
					break;

				case Symbol::ClassTypeId::CONSTANT_INT:
					node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
					break;

				case Symbol::ClassTypeId::CONSTANT_FLOAT:
					node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
					break;

				case Symbol::ClassTypeId::CONSTANT_STRING:
					node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->GetString());
					break;

				case Symbol::ClassTypeId::VARIABLE_STATIC:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::VARIABLE_GLOBAL:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::VARIABLE_ACTOR:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::VARIABLE_LOCAL:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::TYPEDEF:
					break;

				case Symbol::ClassTypeId::NEW_SYMBOL:
				default:
					CompileError("illigal data GetTypeDescriptor()");
					break;
				}
			}
			break;

		case SyntaxNode::Id::MemberFunction:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			abort();
			break;

		case SyntaxNode::Id::MemberVariable:
		{
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);

			std::shared_ptr<TypeDescriptor> type = node->GetLeftNode()->GetTypeDescriptor();
			if (type)
			{
				while (type->Is(TypeDescriptor::Id::Array))
				{
					type = type->GetComponent();
				}
				if (type->Is(TypeDescriptor::Id::Struct))
				{
					// TODO
					//while (type)
					{
						for (std::shared_ptr<Symbol> symbol = type->GetSymbolEntry(); symbol; symbol = symbol->GetNext())
						{
							if (symbol->GetName() == node->GetString() && symbol->GetClassTypeId() == Symbol::ClassTypeId::VARIABLE_ACTOR)
							{
								// variable.member
								node->Set(symbol->GetTypeDescriptor());
								goto ESCAPE;
							}
						}
						//type = type->parent;
					}
					CompileError("reference to undefined field '%s'", node->GetString());
				}
				else
				{
					CompileError("reference to non-struct type");
				}
			}
		}
	ESCAPE:
		break;

		case SyntaxNode::Id::Sender:
		case SyntaxNode::Id::Self:
		case SyntaxNode::Id::Priority:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Sizeof:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::String:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		default:
			MANA_BUG("illigal node detect");
		}

		// 子ノードから型を継承する
		ResolveTypeFromChildNode(node);

		if (node->GetNextNode())
		{
			// 末尾再帰なのでgotoにて処理する
			//PostResolverResolve(node->GetNextNode());
			node = node->GetNextNode();
			goto DO_RECURSIVE;
		}
	}
}
