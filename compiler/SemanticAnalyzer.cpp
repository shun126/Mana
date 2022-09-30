/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "SemanticAnalyzer.h"
#include "Lexer.h"
#include "ErrorHandler.h"

namespace mana
{
	SemanticAnalyzer::SemanticAnalyzer(
		const std::shared_ptr<SymbolFactory>& symbolFactory,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
		: mSymbolFactory(symbolFactory)
		, mTypeDescriptorFactory(typeDescriptorFactory)
	{
	}

	void SemanticAnalyzer::SetCurrentFileInfomation(const std::shared_ptr<SyntaxNode>& node)
	{
		MANA_ASSERT(node);
		lexer_set_current_filename(node->GetFilename().data());
		lexer_set_current_lineno(node->GetLineno());
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
				CompileError("incomplete type name '%s'", node->GetString().data());
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
				CompileError("incomplete type name '%s'", node->GetString().data());
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
			std::shared_ptr<Symbol> symbol = Lookup(node->GetString());
			if (symbol)
			{
				if (symbol->GetClassTypeId() == Symbol::ClassTypeId::CONSTANT_INT)
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
				CompileError("identifier %s is not defined", node->GetString());
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

		//if (node->right->symbol->class_type == VARIABLE_LOCAL)
		GetSymbolFactory()->AllocateMemory(node->GetRightNode()->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor(), memoryTypeId);
		//symbol_allocate_memory(node->right->symbol, node->left->type, PARAMETER);
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

	const std::shared_ptr<SymbolFactory>& SemanticAnalyzer::GetSymbolFactory()
	{
		return mSymbolFactory;
	}

	const std::shared_ptr<TypeDescriptorFactory>& SemanticAnalyzer::GetTypeDescriptorFactory()
	{
		return mTypeDescriptorFactory;
	}
}
