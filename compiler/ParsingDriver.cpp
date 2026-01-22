/*!
mana (compiler)

@author	Shun Moriya
@date	2022-
*/

#include "../runner/common/Setup.h"
#include "ParsingDriver.h"
#include "CodeBuffer.h"
#include "CodeGenerator.h"
#include "DataBuffer.h"
#include "ErrorHandler.h"
#include "GlobalSemanticAnalyzer.h"
#include "LocalAddressResolver.h"
#include "LocalSemanticAnalyzer.h"
#include "NamespaceRegistry.h"
#include "Parser.hpp"
#include "SymbolFactory.h"
#include "SyntaxNode.h"
#include "StringPool.h"
#include "TypeDescriptorFactory.h"

namespace mana
{
	namespace
	{
		constexpr std::string_view kInitGlobalsActorName = "__init_globals";
		constexpr std::string_view kInitGlobalsActionName = "__init";
	}

	ParsingDriver::ParsingDriver()
		: mParser(std::make_unique<Parser>(this))
		, mCodeBuffer(std::make_shared<CodeBuffer>())
		, mDataBuffer(std::make_shared<DataBuffer>())
		, mStringPool(std::make_shared<StringPool>())
		, mTypeDescriptorFactory(std::make_shared<TypeDescriptorFactory>())
	{
		mSymbolFactory = std::make_shared<SymbolFactory>(mCodeBuffer, mDataBuffer, mStringPool, mTypeDescriptorFactory);
		mNamespaceRegistry = std::make_shared<NamespaceRegistry>(mStringPool);
		mLocalSemanticAnalyzer = std::make_shared<LocalSemanticAnalyzer>(
			mSymbolFactory,
			mTypeDescriptorFactory,
			mStringPool,
			mNamespaceRegistry);
		mGlobalSemanticAnalyzer = std::make_shared<GlobalSemanticAnalyzer>(
			mSymbolFactory,
			mTypeDescriptorFactory,
			mStringPool,
			mNamespaceRegistry);
		mCodeGenerator = std::make_shared<CodeGenerator>(
			mCodeBuffer,
			mDataBuffer,
			mGlobalSemanticAnalyzer,
			mLocalSemanticAnalyzer,
			mSymbolFactory,
			mTypeDescriptorFactory
		);
	}

	int32_t ParsingDriver::Parse() const
	{
		return mParser->parse();
	}

	const std::shared_ptr<CodeBuffer>& ParsingDriver::GetCodeBuffer()
	{
		return mCodeBuffer;
	}

	const std::shared_ptr<CodeGenerator>& ParsingDriver::GetCodeGenerator()
	{
		return mCodeGenerator;
	}

	const std::shared_ptr<DataBuffer>& ParsingDriver::GetDataBuffer()
	{
		return mDataBuffer;
	}

	const std::shared_ptr<GlobalSemanticAnalyzer>& ParsingDriver::GetGlobalSemanticAnalyzer()
	{
		return mGlobalSemanticAnalyzer;
	}

	const std::shared_ptr<NamespaceRegistry>& ParsingDriver::GetNamespaceRegistry()
	{
		return mNamespaceRegistry;
	}

	const std::shared_ptr<SyntaxNode>& ParsingDriver::GetRootSyntaxNode() const
	{
		return mRootSyntaxNode;
	}

	void ParsingDriver::SetRootSyntaxNode(const std::shared_ptr<SyntaxNode>& rootSyntaxNode)
	{
		mRootSyntaxNode = rootSyntaxNode;
	}

	const std::shared_ptr<SymbolFactory>& ParsingDriver::GetSymbolFactory()
	{
		return mSymbolFactory;
	}

	const std::shared_ptr<StringPool>& ParsingDriver::GetStringPool()
	{
		return mStringPool;
	}

	const std::shared_ptr<TypeDescriptorFactory>& ParsingDriver::GetTypeDescriptorFactory()
	{
		return mTypeDescriptorFactory;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::InjectGlobalInitializers(const std::shared_ptr<SyntaxNode>& root)
	{
		if (root == nullptr)
			return root;

		if (HasGlobalNameConflict(root, kInitGlobalsActorName))
		{
			CompileError({ "reserved name is already used: ", kInitGlobalsActorName });
			return root;
		}

		const std::shared_ptr<SyntaxNode> initializerStatements = CollectGlobalInitializerStatements(root);
		if (initializerStatements == nullptr)
			return root;

		const std::shared_ptr<SyntaxNode> initBlock = CreateBlock(initializerStatements);
		const std::shared_ptr<SyntaxNode> initAction = CreateAction(kInitGlobalsActionName, initBlock);
		const std::shared_ptr<SyntaxNode> initActor = CreateActor(kInitGlobalsActorName, initAction);

		return AppendNode(root, initActor);
	}

	//if ($1) { $$ = $1; $$->left = $2; } else { $$ = $2; }
	std::shared_ptr<SyntaxNode> ParsingDriver::Bind(const std::shared_ptr<SyntaxNode>& base, const std::shared_ptr<SyntaxNode>& next)
	{
		std::shared_ptr<SyntaxNode> result;
		if (base)
		{
			result = base;
			if (next)
				result->SetNextNode(next);
		}
		else
		{
			result = next;
		}
		return result;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::AppendNode(const std::shared_ptr<SyntaxNode>& base, const std::shared_ptr<SyntaxNode>& next)
	{
		if (base == nullptr)
			return next;
		if (next == nullptr)
			return base;

		std::shared_ptr<SyntaxNode> tail = base;
		while (tail->GetNextNode())
		{
			tail = tail->GetNextNode();
		}
		tail->SetNextNode(next);
		return base;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CollectGlobalInitializerStatements(const std::shared_ptr<SyntaxNode>& root) const
	{
		std::shared_ptr<SyntaxNode> statements;

		for (std::shared_ptr<SyntaxNode> node = root; node; node = node->GetNextNode())
		{
			switch (node->GetId())
			{
			case SyntaxNode::Id::DeclareVariable:
				statements = AppendNode(statements, CollectInitializerStatementsFromDeclarations(node));
				break;

			case SyntaxNode::Id::Allocate:
				statements = AppendNode(statements, CollectInitializerStatementsFromDeclarations(node->GetLeftNode()));
				break;

			case SyntaxNode::Id::Static:
				if (const auto& allocateNode = node->GetLeftNode())
					statements = AppendNode(statements, CollectInitializerStatementsFromDeclarations(allocateNode->GetLeftNode()));
				break;

			default:
				break;
			}
		}

		return statements;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CollectInitializerStatementsFromDeclarations(const std::shared_ptr<SyntaxNode>& node)
	{
		std::shared_ptr<SyntaxNode> statements;

		for (std::shared_ptr<SyntaxNode> current = node; current; current = current->GetNextNode())
		{
			if (current->Is(SyntaxNode::Id::DeclareVariable))
			{
				if (const std::shared_ptr<SyntaxNode> initializer = current->GetBodyNode())
					statements = AppendNode(statements, initializer->Clone());
			}
		}

		return statements;
	}

	bool ParsingDriver::HasGlobalNameConflict(const std::shared_ptr<SyntaxNode>& root, const std::string_view& name)
	{
		for (std::shared_ptr<SyntaxNode> node = root; node; node = node->GetNextNode())
		{
			switch (node->GetId())
			{
			case SyntaxNode::Id::Actor:
			case SyntaxNode::Id::Phantom:
			case SyntaxNode::Id::Module:
			case SyntaxNode::Id::Struct:
			case SyntaxNode::Id::DeclareFunction:
			case SyntaxNode::Id::NativeFunction:
			case SyntaxNode::Id::DefineConstant:
				if (node->GetString() == name)
					return true;
				break;

			case SyntaxNode::Id::DeclareVariable:
				if (node->GetRightNode() && node->GetRightNode()->GetString() == name)
					return true;
				break;

			case SyntaxNode::Id::Allocate:
			case SyntaxNode::Id::Static:
				if (HasGlobalNameConflict(node->GetLeftNode(), name))
					return true;
				break;

			default:
				break;
			}
		}
		return false;
	}

	// declarations
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateNativeFunction(const std::shared_ptr<SyntaxNode>& returnExpression, const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::NativeFunction);
		node->SetLeftNode(returnExpression);
		node->SetRightNode(argument);
		node->Set(identifier);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDeclareMemory(const int_t size, const std::shared_ptr<SyntaxNode>& allocateDeclarations)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Allocate);
		node->SetLeftNode(allocateDeclarations);
		node->Set(size);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDeclareStaticMemory(const int_t size, const std::shared_ptr<SyntaxNode>& allocateDeclarations)
	{
		std::shared_ptr<SyntaxNode> staticNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::Static);

		if (size > 0)
		{
			std::shared_ptr<SyntaxNode> allocateNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::Allocate);
			allocateNode->Set(size);
			allocateNode->SetLeftNode(allocateDeclarations);
			staticNode->SetLeftNode(allocateNode);
		}
		else
		{
			staticNode->SetLeftNode(allocateDeclarations);
		}

		return staticNode;
	}

	// declaration
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateConstantNode(const std::string_view& identifier, const int_t value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(value);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateConstantNode(const std::string_view& identifier, const bool value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(static_cast<int_t>(value));
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Bool));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateConstantNode(const std::string_view& identifier, const float_t value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(value);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateConstantNode(const std::string_view& identifier, const std::string_view& text)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(GetTypeDescriptorFactory()->GetString());
		node->Set(text);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDefineNode(const std::string_view& identifier, const std::string_view& text)
	{
		// HACK: シンボルを検索した方が良い？
		return CreateConstantNode(identifier, text);
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateUndefineNode(const std::string_view& identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::UndefineConstant);
		node->Set(identifier);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateNamespace(const std::string_view& name, const std::shared_ptr<SyntaxNode>& declarations)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Namespace);
		node->Set(name);
		node->SetLeftNode(declarations);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateUsing(const std::string_view& name)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Using);
		node->Set(name);
		return node;
	}

	std::string_view ParsingDriver::CreateQualifiedName(const std::string_view& left, const std::string_view& right)
	{
		std::string combined;
		combined.reserve(left.size() + 2 + right.size());
		combined.append(left);
		combined.append("::");
		combined.append(right);
		return GetStringPool()->Set(combined);
	}

	// actor
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateActor(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& action)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Actor);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreatePhantom(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& action)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Phantom);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateModule(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& action)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Module);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	// action
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateAction(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& block)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Action);
		node->SetLeftNode(block);
		node->Set(identifier);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateExtend(const std::string_view& identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Extend);
		node->Set(identifier);
		return node;
	}

	// struct
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateStruct(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& member)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Struct);
		node->Set(identifier);
		node->SetLeftNode(member);
		return node;
	}

	// function
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateInternalFunction(const std::shared_ptr<SyntaxNode>& returnExpression, const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument, const std::shared_ptr<SyntaxNode>& statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DeclareFunction);
		node->Set(identifier);
		node->SetLeftNode(returnExpression);
		node->SetRightNode(argument);
		node->SetBodyNode(statement);

		if (statement)
		{
			int_t argc = 0;
			for (std::shared_ptr<SyntaxNode> p = argument; p != nullptr; p = p->GetRightNode())
				++argc;
			node->Set(argc);
		}

		return node;
	}

	// variable_type
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateActorTypeDescription()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::TypeDescription);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Actor));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateTypeDescription(const std::string_view& identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::TypeDescription);
		node->Set(identifier);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescriptor)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::TypeDescription);
		node->Set(typeDescriptor);
		return node;
	}

	// block
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateBlock(const std::shared_ptr<SyntaxNode>& statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Block);
		node->SetLeftNode(statement);
		return node;
	}

	// statement
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateIf(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& statementIfTrue, const std::shared_ptr<SyntaxNode>& statementIfFalse)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::If);
		node->SetLeftNode(statementIfTrue);
		node->SetRightNode(statementIfFalse);
		node->SetBodyNode(condition);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSwitch(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Switch);
		node->SetLeftNode(condition);
		node->SetRightNode(statement);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateWhile(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::While);
		node->SetLeftNode(condition);
		node->SetRightNode(statement);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDoWhile(const std::shared_ptr<SyntaxNode>& statement, const std::shared_ptr<SyntaxNode>& condition)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Do);
		node->SetLeftNode(statement);
		node->SetRightNode(condition);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateFor(const std::shared_ptr<SyntaxNode>& initializeStatement, const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& iterationExpression, const std::shared_ptr<SyntaxNode>& statement)
	{
		std::shared_ptr<SyntaxNode> forNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::For);
		forNode->SetLeftNode(condition);
		forNode->SetRightNode(iterationExpression);
		forNode->SetBodyNode(statement);

		std::shared_ptr<SyntaxNode> blockNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::Block);
		blockNode->SetLeftNode(initializeStatement);
		blockNode->SetRightNode(forNode);
		return blockNode;
	}

#if 0
	| tFOR '(' variable_decl '=' expression ';' expression ';' expression ')' statement

		/*
		MANA_ASSERT($3->right);
		MANA_ASSERT($3->right->mString);
		MANA_ASSERT($3->next == nullptr);

		node_entry* mNode = node_create_node(Identifier, nullptr, nullptr, nullptr);
		mNode->mString = $3->right->mString;
		$3->next = mNode;

		$$ = node_create_node(Block,
		node_create_node(Assign, $3, $5, nullptr),
		node_create_node(For, $7, $9, $11),
		nullptr
		);
		*/
}
#endif
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateFor(const std::shared_ptr<SyntaxNode>& declareVariable, const std::shared_ptr<SyntaxNode>& initializeStatement, const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& iterationExpression, const std::shared_ptr<SyntaxNode>& statement)
	{
		MANA_ASSERT(declareVariable->GetRightNode() != nullptr);
		//MANA_ASSERT(declareVariable->GetRightNode()->GetString());
		MANA_ASSERT(declareVariable->GetNextNode() == nullptr);

		std::shared_ptr<SyntaxNode> initNode = CreateAssign(declareVariable, initializeStatement);

		std::shared_ptr<SyntaxNode> forNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::For);
		forNode->SetLeftNode(condition);
		forNode->SetRightNode(iterationExpression);
		forNode->SetBodyNode(statement);

		std::shared_ptr<SyntaxNode> blockNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::Block);
		blockNode->SetLeftNode(initNode);
		blockNode->SetRightNode(forNode);
		return blockNode;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLoop(const std::shared_ptr<SyntaxNode>& statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Loop);
		node->SetLeftNode(statement);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLock(const std::shared_ptr<SyntaxNode>& statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Lock);
		node->SetLeftNode(statement);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateGoto(const std::string_view& labelName)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Goto);
		node->Set(labelName);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLabel(const std::string_view& labelName)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Label);
		node->Set(labelName);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateReturn(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Return);
		node->SetLeftNode(expression);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateRollback(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Rollback);
		node->SetLeftNode(expression);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateBreak()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Break);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateContinue()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Continue);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateHalt()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Halt);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateYield()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Yield);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateComply()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Accept);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateRefuse()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Reject);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreatePrint(const std::shared_ptr<SyntaxNode>& argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Print);
		node->SetLeftNode(argument);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateRequest(const std::shared_ptr<SyntaxNode>& priority, const std::shared_ptr<SyntaxNode>& expression, const std::string_view& actionName)
	{
		auto node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Request);
		node->SetLeftNode(priority);
		node->SetRightNode(expression);
		node->Set(actionName);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateAwaitStart(const std::shared_ptr<SyntaxNode>& priority, const std::shared_ptr<SyntaxNode>& expression, const std::string_view& actionName)
	{
		auto node = std::make_shared<SyntaxNode>(SyntaxNode::Id::AwaitStart);
		node->SetLeftNode(priority);
		node->SetRightNode(expression);
		node->Set(actionName);
		return node;
	}

std::shared_ptr<SyntaxNode> ParsingDriver::CreateAwaitCompletion(const std::shared_ptr<SyntaxNode>& priority, const std::shared_ptr<SyntaxNode>& expression, const std::string_view& actionName)
	{
		auto node = std::make_shared<SyntaxNode>(SyntaxNode::Id::AwaitCompletion);
		node->SetLeftNode(priority);
		node->SetRightNode(expression);
		node->Set(actionName);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateJoin(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Join);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	// expression
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Assign);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLogicalAnd(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LogicalAnd);
		node->Set(IntermediateLanguage::LogicalAnd);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLogicalOr(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LogicalOr);
		node->Set(IntermediateLanguage::LogicalOr);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateAdd(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Add);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSub(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Sub);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateMul(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Mul);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDiv(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Div);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateMod(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Rem);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreatePow(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Pow);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateAnd(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::And);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateOr(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Or);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateXor(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Xor);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLeftShift(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LeftShift);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateRightShift(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::RightShift);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateGreater(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Greater);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateGreaterEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::GreaterEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLess(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Less);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLessEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LessEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Equal);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateNotEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::NotEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateAddAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateAdd(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSubAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateSub(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateMulAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateMul(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDivAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateDiv(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateModAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateMod(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreatePowAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreatePow(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateAndAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateAnd(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateOrAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateOr(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateXorAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateXor(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateLeftShiftAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateLeftShift(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateRightShiftAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateRightShift(leftHand->Clone(), rightHand)
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateIncrement(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			expression,
			CreateAdd(expression->Clone(), CreateInteger(1))
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDecrement(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			expression,
			CreateSub(expression->Clone(), CreateInteger(1))
		);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateExpressionIf(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& trueStatement, const std::shared_ptr<SyntaxNode>& falseStatement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::ExpressionIf);
		node->SetLeftNode(trueStatement);
		node->SetRightNode(falseStatement);
		node->SetBodyNode(condition);
		return node;
	}

	// primary
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateNegative(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Neg);
		node->SetLeftNode(expression);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateNot(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LogicalNot);
		node->SetLeftNode(expression);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateComplement1(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Not);
		node->SetLeftNode(expression);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSizeOf(const std::shared_ptr<SyntaxNode>& variableType)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Sizeof);
		node->SetRightNode(variableType);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateCall(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Call);
		node->Set(identifier);
		node->SetRightNode(argument);
		return node;
	}

	// constant
	std::shared_ptr<SyntaxNode> ParsingDriver::CreatePriority()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Priority);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSelf()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Self);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Actor));
		//$$->etc = 0;
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSender()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Sender);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Actor));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateNil()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Const);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Nil));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateBool(const bool value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Const);
		node->Set(static_cast<int_t>(value));
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Bool));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateInteger(const int_t value)
	{
		TypeDescriptor::Id typeId;
		if (value <= INT8_MAX && value >= INT8_MIN)
			typeId = TypeDescriptor::Id::Char;
		else if (value <= INT16_MAX && value >= INT16_MIN)
			typeId = TypeDescriptor::Id::Short;
		else
			typeId = TypeDescriptor::Id::Int;

		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Const);
		node->Set(value);
		node->Set(GetTypeDescriptorFactory()->Get(typeId));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateFloat(const float_t value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Const);
		node->Set(value);
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateString(const std::string_view& text)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::String);
		const int_t offset = static_cast<int_t>(GetDataBuffer()->Set(text));
		node->Set(offset);
		node->Set(GetTypeDescriptorFactory()->GetString());
		return node;
	}

	// left hand
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateMemberVariable(const std::shared_ptr<SyntaxNode>& leftHand, const std::string_view& identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::MemberVariable);
		node->SetLeftNode(leftHand);
		node->Set(identifier);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateMemberFunction(const std::shared_ptr<SyntaxNode>& leftHand, const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::MemberFunction);
		node->SetLeftNode(leftHand);
		node->SetRightNode(argument);
		node->Set(identifier);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateArray(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Array);
		node->SetLeftNode(leftHand);
		node->SetRightNode(expression);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateIdentifier(const std::string_view& identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Identifier);
		node->Set(identifier);
		return node;
	}

	// cases
	std::shared_ptr<SyntaxNode> ParsingDriver::BindCaseNode(const std::shared_ptr<SyntaxNode>& next, const std::shared_ptr<SyntaxNode>& kase)
	{
		std::shared_ptr<SyntaxNode> result = kase;
		result->SetNextNode(next);
		return result;
	}

	// case
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSwitchCaseNode(const std::shared_ptr<SyntaxNode>& expression, const std::shared_ptr<SyntaxNode>& statements)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Case);
		node->SetLeftNode(expression);
		node->SetRightNode(statements);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateSwitchDefaultNode(const std::shared_ptr<SyntaxNode>& statements)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Default);
		node->SetBodyNode(statements);
		return node;
	}

	// arg_calls
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateArgumentNode(const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::CallArgument);
		node->SetLeftNode(expression);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateArgumentNode(const std::shared_ptr<SyntaxNode>& nextArgument, const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::CallArgument);
		node->SetLeftNode(nextArgument);
		node->SetRightNode(expression);
		return node;
	}

	//arg_decls
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDeclareArgumentNode(const std::shared_ptr<SyntaxNode>& declareVariable)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DeclareArgument);
		node->SetLeftNode(declareVariable);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDeclareArgumentNode(const std::shared_ptr<SyntaxNode>& nextDeclareArgument, const std::shared_ptr<SyntaxNode>& declareVariable)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DeclareArgument);
		node->SetLeftNode(declareVariable);
		node->SetRightNode(nextDeclareArgument);
		return node;
	}

	// variable_decl
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDeclareVariableNode(const std::shared_ptr<SyntaxNode>& variableType, const std::shared_ptr<SyntaxNode>& declarator, const std::shared_ptr<SyntaxNode>& expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DeclareVariable);
		node->SetLeftNode(variableType);
		node->SetRightNode(declarator);
		if (expression)
		{
			MANA_ASSERT(variableType);
			MANA_ASSERT(variableType->GetTypeDescriptor());
			MANA_ASSERT(declarator);
			MANA_ASSERT(declarator->GetString().data());

			const std::shared_ptr<SyntaxNode> identifierNode = CreateIdentifier(declarator->GetString());
			identifierNode->Set(variableType->GetTypeDescriptor());

			const std::shared_ptr<SyntaxNode> assignNode = CreateAssign(identifierNode, expression);
			assignNode->Set(variableType->GetTypeDescriptor());

			node->SetBodyNode(assignNode);
		}
		return node;
	}

	// declarator
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDeclaratorNode(const std::string_view& identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Declarator);
		node->Set(identifier);
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateDeclaratorNode(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& variableSizeNode)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Declarator);
		node->Set(identifier);
		node->SetLeftNode(variableSizeNode);
		return node;
	}

	// variable_size
	std::shared_ptr<SyntaxNode> ParsingDriver::CreateVariableSizeNode(const size_t size)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::VariableSize);
		node->Set(static_cast<int_t>(size));
		return node;
	}

	std::shared_ptr<SyntaxNode> ParsingDriver::CreateVariableSizeNode(const std::string_view& identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::VariableSize);
		node->Set(identifier);
		return node;
	}
}
