/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once

namespace mana
{
	/*!
	print error message
	@param	message		error message
	*/
	void Parser::error(const std::string& message)
	{
		mana::CompileError(message.c_str());
	}

	//if ($1) { $$ = $1; $$->left = $2; } else { $$ = $2; }
	static std::shared_ptr<SyntaxNode> Bind(std::shared_ptr<SyntaxNode> base, std::shared_ptr<SyntaxNode> next)
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

	// declarations
	static std::shared_ptr<SyntaxNode> CreateNativeFunction(std::shared_ptr<SyntaxNode> returnExpression, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::NativeFunction);
		node->SetLeftNode(returnExpression);
		node->SetRightNode(argument);
		node->Set(identifier);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDeclareMemory(const int_t size, std::shared_ptr<SyntaxNode> allocateDeclarations)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Allocate);
		node->SetLeftNode(allocateDeclarations);
		node->Set(size);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDeclareStaticMemory(const int_t size, std::shared_ptr<SyntaxNode> allocateDeclarations)
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

	static std::shared_ptr<SyntaxNode> CreateAlias(const std::string_view identifier, const std::string_view filename)
	{
		//TODO:実装して下さい
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Alias);
		return node;
	}

	// declaration
	static std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const int_t value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(value);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const float_t value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(value);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const std::string_view text)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->GetString());
		node->Set(text);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDefineNode(const std::string_view identifier, const std::string_view text)
	{
		// HACK: シンボルを検索した方が良い？
		return CreateConstantNode(identifier, text);
	}

	static std::shared_ptr<SyntaxNode> CreateUndefineNode(const std::string_view identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::UndefineConstant);
		node->Set(identifier);
		return node;
	}

	// actor
	static std::shared_ptr<SyntaxNode> CreateActor(const std::string_view identifier, std::shared_ptr<SyntaxNode> action)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Actor);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreatePhantom(const std::string_view identifier, std::shared_ptr<SyntaxNode> action)
	{		
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Phantom);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateModule(const std::string_view identifier, std::shared_ptr<SyntaxNode> action)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Module);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	// action
	static std::shared_ptr<SyntaxNode> CreateAction(const std::string_view identifier, std::shared_ptr<SyntaxNode> block)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Action);
		node->SetLeftNode(block);
		node->Set(identifier);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateExtend(const std::string_view identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Extend);
		node->Set(identifier);
		return node;
	}

	// struct
	static std::shared_ptr<SyntaxNode> CreateStruct(const std::string_view identifier, std::shared_ptr<SyntaxNode> member)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Struct);
		node->Set(identifier);
		node->SetLeftNode(member);
		return node;
	}

	// function
	static std::shared_ptr<SyntaxNode> CreateInternalFunction(std::shared_ptr<SyntaxNode> returnExpression, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument, std::shared_ptr<SyntaxNode> statement)
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
	static std::shared_ptr<SyntaxNode> CreateActorTypeDescription()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::TypeDescription);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::ACTOR));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::string_view identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::TypeDescription);
		node->Set(identifier);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescriptor)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::TypeDescription);
		node->Set(typeDescriptor);
		return node;
	}

	// block
	static std::shared_ptr<SyntaxNode> CreateBlock(std::shared_ptr<SyntaxNode> statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Block);
		node->SetLeftNode(statement);
		return node;
	}

	// statement
	static std::shared_ptr<SyntaxNode> CreateIf(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statementIfTrue, std::shared_ptr<SyntaxNode> statementIfFalse)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::If);
		node->SetLeftNode(statementIfTrue);
		node->SetRightNode(statementIfFalse);
		node->SetBodyNode(condition);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateSwitch(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Switch);
		node->SetLeftNode(condition);
		node->SetRightNode(statement);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateWhile(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::While);
		node->SetLeftNode(condition);
		node->SetRightNode(statement);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDoWhile(std::shared_ptr<SyntaxNode> statement, std::shared_ptr<SyntaxNode> condition)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Do);
		node->SetLeftNode(statement);
		node->SetRightNode(condition);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateFor(std::shared_ptr<SyntaxNode> initializeStatement, std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> iterationExpression, std::shared_ptr<SyntaxNode> statement)
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
	static std::shared_ptr<SyntaxNode> CreateFor(std::shared_ptr<SyntaxNode> declareVariable, std::shared_ptr<SyntaxNode> initializeStatement, std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> iterationExpression, std::shared_ptr<SyntaxNode> statement)
	{
		MANA_ASSERT(declareVariable->GetRightNode() != nullptr);
		//MANA_ASSERT(declareVariable->GetRightNode()->GetString());
		MANA_ASSERT(declareVariable->GetNextNode() == nullptr);

		std::shared_ptr<SyntaxNode> initNode = mana::CreateAssign(declareVariable, initializeStatement);

		std::shared_ptr<SyntaxNode> forNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::For);
		forNode->SetLeftNode(condition);
		forNode->SetRightNode(iterationExpression);
		forNode->SetBodyNode(statement);

		std::shared_ptr<SyntaxNode> blockNode = std::make_shared<SyntaxNode>(SyntaxNode::Id::Block);
		blockNode->SetLeftNode(initNode);
		blockNode->SetRightNode(forNode);
		return blockNode;
	}

	static std::shared_ptr<SyntaxNode> CreateLoop(std::shared_ptr<SyntaxNode> statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Loop);
		node->SetLeftNode(statement);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLock(std::shared_ptr<SyntaxNode> statement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Lock);
		node->SetLeftNode(statement);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateGoto(const std::string_view labelName)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Goto);
		node->Set(labelName);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLabel(const std::string_view labelName)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Label);
		node->Set(labelName);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateReturn(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Return);
		node->SetLeftNode(expression);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateRollback(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Rollback);
		node->SetLeftNode(expression);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateBreak()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Break);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateContinue()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Continue);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateHalt()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Halt);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateYield()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Yield);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateComply()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Accept);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateRefuse()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Reject);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreatePrint(std::shared_ptr<SyntaxNode> argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Print);
		node->SetLeftNode(argument);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateRequest(std::shared_ptr<SyntaxNode> priority, std::shared_ptr<SyntaxNode> expression, const std::string_view actionName)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Request);
		node->SetLeftNode(priority);
		node->SetRightNode(expression);
		node->Set(actionName);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateJoin(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Join);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	// expression
	static std::shared_ptr<SyntaxNode> CreateAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Assign);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLogicalAnd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LogicalAnd);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLogicalOr(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LogicalOr);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateAdd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Add);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateSub(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Sub);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateMul(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Mul);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDiv(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Div);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateMod(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Rem);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreatePow(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Pow);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateAnd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::And);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateOr(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Or);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateXor(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Xor);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLeftShift(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LeftShift);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateRightShift(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::RightShift);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateGT(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Greater);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateGE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::GreaterEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLS(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Less);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LessEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateEQ(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Equal);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateNE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::NotEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateAddAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateAdd(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateSubAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateSub(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateMulAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateMul(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDivAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateDiv(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateModAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateMod(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreatePowAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreatePow(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateAndAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateAnd(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateOrAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateOr(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateXorAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateXor(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateLeftShiftAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateLeftShift(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateRightShiftAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			leftHand,
			CreateRightShift(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateIncrement(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			expression,
			CreateAdd(expression->Clone(), CreateInteger(1))
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDecrement(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = CreateAssign(
			expression,
			CreateSub(expression->Clone(), CreateInteger(1))
		);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateExpressionIf(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> trueStatement, std::shared_ptr<SyntaxNode> falseStatement)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::ExpressionIf);
		node->SetLeftNode(trueStatement);
		node->SetRightNode(falseStatement);
		node->SetBodyNode(condition);
		return node;
	}

	// primary
	static std::shared_ptr<SyntaxNode> CreateNegative(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Neg);
		node->SetLeftNode(expression);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateNot(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::LogicalNot);
		node->SetLeftNode(expression);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateComplement1(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Not);
		node->SetLeftNode(expression);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateSizeOf(std::shared_ptr<SyntaxNode> variableType)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Sizeof);
		node->SetRightNode(variableType);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateCall(const std::string_view identifier, std::shared_ptr<SyntaxNode> argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Call);
		node->Set(identifier);
		node->SetRightNode(argument);
		return node;
	}

	// constant
	static std::shared_ptr<SyntaxNode> CreatePriority()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Priority);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateSelf()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Self);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::ACTOR));
		//$$->etc = 0;
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateSender()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Sender);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::ACTOR));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateNil()
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Const);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::NIL));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateInteger(const int_t value)
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
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(typeId));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateFloat(const float_t value)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Const);
		node->Set(value);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateString(const std::string_view text)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::String);
		const int_t offset = static_cast<int_t>(GetSystemHolder().GetDataBuffer()->Set(text));
		node->Set(offset);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->GetString());
		return node;
	}

	// left hand
	static std::shared_ptr<SyntaxNode> CreateMemberVariable(std::shared_ptr<SyntaxNode> leftHand, const std::string_view identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::MemberVariable);
		node->SetLeftNode(leftHand);
		node->Set(identifier);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateMemberFunction(std::shared_ptr<SyntaxNode> leftHand, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::MemberFunction);
		node->SetLeftNode(leftHand);
		node->SetRightNode(argument);
		node->Set(identifier);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateArray(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Array);
		node->SetLeftNode(leftHand);
		node->SetRightNode(expression);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateIdentifier(const std::string_view identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Identifier);
		node->Set(identifier);
		return node;
	}

	// cases
	static std::shared_ptr<SyntaxNode> BindCaseNode(std::shared_ptr<SyntaxNode> next, std::shared_ptr<SyntaxNode> kase)
	{
		std::shared_ptr<SyntaxNode> result = kase;
		result->SetNextNode(next);
		return result;
	}

	// case
	static std::shared_ptr<SyntaxNode> CreateSwitchCaseNode(std::shared_ptr<SyntaxNode> expression, std::shared_ptr<SyntaxNode> statements)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Case);
		node->SetLeftNode(expression);
		node->SetRightNode(statements);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateSwitchDefaultNode(std::shared_ptr<SyntaxNode> statements)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Default);
		node->SetBodyNode(statements);
		return node;
	}

	// arg_calls
	static std::shared_ptr<SyntaxNode> CreateArgumentNode(std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::CallArgument);
		node->SetLeftNode(expression);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateArgumentNode(std::shared_ptr<SyntaxNode> nextArgument, std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::CallArgument);
		node->SetLeftNode(nextArgument);
		node->SetRightNode(expression);
		return node;
	}

	//arg_decls
	static std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(std::shared_ptr<SyntaxNode> declareVariable)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DeclareArgument);
		node->SetLeftNode(declareVariable);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(std::shared_ptr<SyntaxNode> nextDeclareArgument, std::shared_ptr<SyntaxNode> declareVariable)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DeclareArgument);
		node->SetLeftNode(declareVariable);
		node->SetRightNode(nextDeclareArgument);
		return node;
	}

	// variable_decl
	static std::shared_ptr<SyntaxNode> CreateDeclareVariableNode(std::shared_ptr<SyntaxNode> variableType, std::shared_ptr<SyntaxNode> declarator, std::shared_ptr<SyntaxNode> expression)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::DeclareVariable);
		node->SetLeftNode(variableType);
		node->SetRightNode(declarator);
		if (expression)
		{
			MANA_ASSERT(variableType);
			MANA_ASSERT(variableType->GetTypeDescriptor());
			MANA_ASSERT(declarator);
			//MANA_ASSERT(declarator->GetString());

			std::shared_ptr<SyntaxNode> identifierNode = mana::CreateIdentifier(declarator->GetString());
			identifierNode->Set(variableType->GetTypeDescriptor());

			std::shared_ptr<SyntaxNode> assignNode = mana::CreateAssign(identifierNode, expression);
			assignNode->Set(variableType->GetTypeDescriptor());
#if 0

			/*
			if($2->class_type != MANA_CLASS_TYPE_VARIABLE_LOCAL)
			mana_compile_error("can initialize variable in local space only");
			mana_symbol_allocate_memory($2, $1, MANA_NORMAL);
			mana_linker_expression(node_create_node(MANA_NODE_TYPE_ASSIGN, node_create_leaf($2->name), $4), MANA_TRUE);
			*/
#endif
			node->SetBodyNode(assignNode);
		}
		return node;
	}

	// declarator
	static std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Declarator);
		node->Set(identifier);
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view identifier, std::shared_ptr<SyntaxNode> variableSizeNode)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::Declarator);
		node->Set(identifier);
		node->SetLeftNode(variableSizeNode);
		return node;
	}

	// variable_size
	static std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const size_t size)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::VariableSize);
		node->Set(static_cast<int_t>(size));
		return node;
	}

	static std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const std::string_view identifier)
	{
		std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(SyntaxNode::Id::VariableSize);
		node->Set(identifier);
		return node;
	}
}
