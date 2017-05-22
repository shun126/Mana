/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once

namespace mana
{
	//if ($1) { $$ = $1; $$->left = $2; } else { $$ = $2; }
	static SyntaxNode* Bind(SyntaxNode* base, SyntaxNode* next)
	{
		SyntaxNode* result;
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
	static SyntaxNode* CreateNativeFunction(SyntaxNode* returnExpression, const char* identifier, SyntaxNode* argument)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::NativeFunction);
		node->SetLeftNode(returnExpression);
		node->SetRightNode(argument);
		node->Set(identifier);
		return node;
	}

	static SyntaxNode* CreateDeclareMemory(const int_t size, SyntaxNode* allocateDeclarations)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Allocate);
		node->SetLeftNode(allocateDeclarations);
		node->Set(size);
		return node;
	}

	static SyntaxNode* CreateDeclareStaticMemory(const int_t size, SyntaxNode* allocateDeclarations)
	{
		SyntaxNode* staticNode = new SyntaxNode(SyntaxNode::Id::Static);

		if (size > 0)
		{
			SyntaxNode* allocateNode = new SyntaxNode(SyntaxNode::Id::Allocate);
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

	static SyntaxNode* CreateAlias(const char* identifier, const char* filename)
	{
		//TODO:実装して下さい
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Alias);
		return node;
	}

	// declaration
	static SyntaxNode* CreateConstantNode(const char* identifier, const int_t value)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(value);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
		return node;
	}

	static SyntaxNode* CreateConstantNode(const char* identifier, const float_t value)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(value);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
		return node;
	}

	static SyntaxNode* CreateConstantNode(const char* identifier, const char* text)
	{
		// TODO:textを保存して下さい
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::DefineConstant);
		node->Set(identifier);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->GetString());
		return node;
	}

	// actor
	static SyntaxNode* CreateActor(const char* identifier, SyntaxNode* action)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Actor);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	static SyntaxNode* CreatePhantom(const char* identifier, SyntaxNode* action)
	{		
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Phantom);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	static SyntaxNode* CreateModule(const char* identifier, SyntaxNode* action)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Module);
		node->Set(identifier);
		node->SetLeftNode(action);
		return node;
	}

	// action
	static SyntaxNode* CreateAction(const char* identifier, SyntaxNode* block)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Action);
		node->SetLeftNode(block);
		node->Set(identifier);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void));
		return node;
	}

	static SyntaxNode* CreateExtend(const char* identifier)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Extend);
		node->Set(identifier);
		return node;
	}

	// struct
	static SyntaxNode* CreateStruct(const char* identifier, SyntaxNode* member)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Struct);
		node->Set(identifier);
		node->SetLeftNode(member);
		return node;
	}

	// function
	static SyntaxNode* CreateInternalFunction(SyntaxNode* returnExpression, const char* identifier, SyntaxNode* argument, SyntaxNode* statement)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::DeclareFunction);
		node->Set(identifier);
		node->SetLeftNode(returnExpression);
		node->SetRightNode(argument);
		node->SetBodyNode(statement);

		if (statement)
		{
			int_t argc = 0;
			for (const SyntaxNode* p = argument; p != nullptr; p = p->GetRightNode())
				++argc;
			node->Set(argc);
		}

		return node;
	}

	// variable_type
	static SyntaxNode* CreateActorTypeDescription()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::TypeDescription);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::ACTOR));
		return node;
	}

	static SyntaxNode* CreateTypeDescription(const char* identifier)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::TypeDescription);
		node->Set(identifier);
		return node;
	}

	static SyntaxNode* CreateTypeDescription(TypeDescriptor* typeDescriptor)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::TypeDescription);
		node->Set(typeDescriptor);
		return node;
	}

	// block
	static SyntaxNode* CreateBlock(SyntaxNode* statement)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Block);
		node->SetLeftNode(statement);
		return node;
	}

	// statement
	static SyntaxNode* CreateIf(SyntaxNode* condition, SyntaxNode* statementIfTrue, SyntaxNode* statementIfFalse)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::If);
		node->SetLeftNode(statementIfTrue);
		node->SetRightNode(statementIfFalse);
		node->SetBodyNode(condition);
		return node;
	}

	static SyntaxNode* CreateSwitch(SyntaxNode* condition, SyntaxNode* statement)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Switch);
		node->SetLeftNode(condition);
		node->SetRightNode(statement);
		return node;
	}

	static SyntaxNode* CreateWhile(SyntaxNode* condition, SyntaxNode* statement)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::While);
		node->SetLeftNode(condition);
		node->SetRightNode(statement);
		return node;
	}

	static SyntaxNode* CreateDoWhile(SyntaxNode* statement, SyntaxNode* condition)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Do);
		node->SetLeftNode(statement);
		node->SetRightNode(condition);
		return node;
	}

	static SyntaxNode* CreateFor(SyntaxNode* initializeStatement, SyntaxNode* condition, SyntaxNode* iterationExpression, SyntaxNode* statement)
	{
		SyntaxNode* forNode = new SyntaxNode(SyntaxNode::Id::For);
		forNode->SetLeftNode(condition);
		forNode->SetRightNode(iterationExpression);
		forNode->SetBodyNode(statement);

		SyntaxNode* blockNode = new SyntaxNode(SyntaxNode::Id::Block);
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
	static SyntaxNode* CreateFor(SyntaxNode* declareVariable, SyntaxNode* initializeStatement, SyntaxNode* condition, SyntaxNode* iterationExpression, SyntaxNode* statement)
	{
		MANA_ASSERT(declareVariable->GetRightNode() != nullptr);
		MANA_ASSERT(declareVariable->GetRightNode()->GetString());
		MANA_ASSERT(declareVariable->GetNextNode() == nullptr);

		SyntaxNode* initNode = mana::CreateAssign(declareVariable, initializeStatement);

		SyntaxNode* forNode = new SyntaxNode(SyntaxNode::Id::For);
		forNode->SetLeftNode(condition);
		forNode->SetRightNode(iterationExpression);
		forNode->SetBodyNode(statement);

		SyntaxNode* blockNode = new SyntaxNode(SyntaxNode::Id::Block);
		blockNode->SetLeftNode(initNode);
		blockNode->SetRightNode(forNode);
		return blockNode;
	}

	static SyntaxNode* CreateLoop(SyntaxNode* statement)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Loop);
		node->SetLeftNode(statement);
		return node;
	}

	static SyntaxNode* CreateLock(SyntaxNode* statement)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Lock);
		node->SetLeftNode(statement);
		return node;
	}

	static SyntaxNode* CreateGoto(const char* labelName)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Goto);
		node->Set(labelName);
		return node;
	}

	static SyntaxNode* CreateLabel(const char* labelName)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Label);
		node->Set(labelName);
		return node;
	}

	static SyntaxNode* CreateReturn(SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Return);
		node->SetLeftNode(expression);
		return node;
	}

	static SyntaxNode* CreateRollback(SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Rollback);
		node->SetLeftNode(expression);
		return node;
	}

	static SyntaxNode* CreateBreak()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Break);
		return node;
	}

	static SyntaxNode* CreateContinue()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Continue);
		return node;
	}

	static SyntaxNode* CreateHalt()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Halt);
		return node;
	}

	static SyntaxNode* CreateYield()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Yield);
		return node;
	}

	static SyntaxNode* CreateComply()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Accept);
		return node;
	}

	static SyntaxNode* CreateRefuse()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Reject);
		return node;
	}

	static SyntaxNode* CreatePrint(SyntaxNode* argument)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Print);
		node->SetLeftNode(argument);
		return node;
	}

	static SyntaxNode* CreateRequest(SyntaxNode* priority, SyntaxNode* expression, const char* actionName)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Request);
		node->SetLeftNode(priority);
		node->SetRightNode(expression);
		node->Set(actionName);
		return node;
	}

	static SyntaxNode* CreateJoin(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Join);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	// expression
	static SyntaxNode* CreateAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Assign);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateLogicalAnd(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::LogicalAnd);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateLogicalOr(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::LogicalOr);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateAdd(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Add);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateSub(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Sub);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateMul(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Mul);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateDiv(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Div);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateMod(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Rem);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreatePow(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Pow);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateAnd(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::And);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateOr(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Or);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateXor(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Xor);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateLeftShift(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::LeftShift);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateRightShift(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::RightShift);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateGT(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Greater);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateGE(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::GreaterEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateLS(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Less);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateLE(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::LessEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateEQ(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Equal);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateNE(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::NotEqual);
		node->SetLeftNode(leftHand);
		node->SetRightNode(rightHand);
		return node;
	}

	static SyntaxNode* CreateAddAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateAdd(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateSubAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateSub(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateMulAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateMul(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateDivAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateDiv(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateModAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateMod(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreatePowAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreatePow(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateAndAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateAnd(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateOrAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateOr(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateXorAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateXor(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateLeftShiftAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateLeftShift(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateRightShiftAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand)
	{
		SyntaxNode* node = CreateAssign(
			leftHand,
			CreateRightShift(leftHand->Clone(), rightHand)
		);
		return node;
	}

	static SyntaxNode* CreateIncrement(SyntaxNode* expression)
	{
		SyntaxNode* node = CreateAssign(
			expression,
			CreateAdd(expression->Clone(), CreateInteger(1))
		);
		return node;
	}

	static SyntaxNode* CreateDecrement(SyntaxNode* expression)
	{
		SyntaxNode* node = CreateAssign(
			expression,
			CreateSub(expression->Clone(), CreateInteger(1))
		);
		return node;
	}

	static SyntaxNode* CreateExpressionIf(SyntaxNode* condition, SyntaxNode* trueStatement, SyntaxNode* falseStatement)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::ExpressionIf);
		node->SetLeftNode(trueStatement);
		node->SetRightNode(falseStatement);
		node->SetBodyNode(condition);
		return node;
	}

	// primary
	static SyntaxNode* CreateNegative(SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Neg);
		node->SetLeftNode(expression);
		return node;
	}

	static SyntaxNode* CreateNot(SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::LogicalNot);
		node->SetLeftNode(expression);
		return node;
	}

	static SyntaxNode* CreateComplement1(SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Not);
		node->SetLeftNode(expression);
		return node;
	}

	static SyntaxNode* CreateSizeOf(SyntaxNode* variableType)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Sizeof);
		node->SetRightNode(variableType);
		return node;
	}

	static SyntaxNode* CreateCall(const char* identifier, SyntaxNode* argument)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Call);
		node->Set(identifier);
		node->SetRightNode(argument);
		return node;
	}

	// constant
	static SyntaxNode* CreatePriority()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Priority);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
		return node;
	}

	static SyntaxNode* CreateSelf()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Self);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::ACTOR));
		//$$->etc = 0;
		return node;
	}

	static SyntaxNode* CreateSender()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Sender);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::ACTOR));
		return node;
	}

	static SyntaxNode* CreateNil()
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Const);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::NIL));
		return node;
	}

	static SyntaxNode* CreateInteger(const int_t value)
	{
		TypeDescriptor::Id typeId;
		if (value <= INT8_MAX && value >= INT8_MIN)
			typeId = TypeDescriptor::Id::Char;
		else if (value <= INT16_MAX && value >= INT16_MIN)
			typeId = TypeDescriptor::Id::Short;
		else
			typeId = TypeDescriptor::Id::Int;

		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Const);
		node->Set(value);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(typeId));
		return node;
	}

	static SyntaxNode* CreateFloat(const float_t value)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Const);
		node->Set(value);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
		return node;
	}

	static SyntaxNode* CreateString(const char* text)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::String);
		const int_t offset = static_cast<int_t>(GetSystemHolder().GetDataBuffer()->Set(text));
		node->Set(offset);
		node->Set(GetSystemHolder().GetTypeDescriptorFactory()->GetString());
		return node;
	}

	// left hand
	static SyntaxNode* CreateMemberVariable(SyntaxNode* leftHand, const char* identifier)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::MemberVariable);
		node->SetLeftNode(leftHand);
		node->Set(identifier);
		return node;
	}

	static SyntaxNode* CreateMemberFunction(SyntaxNode* leftHand, const char* identifier, SyntaxNode* argument)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::MemberFunction);
		node->SetLeftNode(leftHand);
		node->SetRightNode(argument);
		node->Set(identifier);
		return node;
	}

	static SyntaxNode* CreateArray(SyntaxNode* leftHand, SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Array);
		node->SetLeftNode(leftHand);
		node->SetRightNode(expression);
		return node;
	}

	static SyntaxNode* CreateIdentifier(const char* identifier)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Identifier);
		node->Set(identifier);
		return node;
	}

	// cases
	static SyntaxNode* BindCaseNode(SyntaxNode* next, SyntaxNode* kase)
	{
		SyntaxNode* result = kase;
		result->SetNextNode(next);
		return result;
	}

	// case
	static SyntaxNode* CreateSwitchCaseNode(SyntaxNode* expression, SyntaxNode* statements)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Case);
		node->SetLeftNode(expression);
		node->SetRightNode(statements);
		return node;
	}

	static SyntaxNode* CreateSwitchDefaultNode(SyntaxNode* statements)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Default);
		node->SetBodyNode(statements);
		return node;
	}

	// arg_calls
	static SyntaxNode* CreateArgumentNode(SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::CallArgument);
		node->SetLeftNode(expression);
		return node;
	}

	static SyntaxNode* CreateArgumentNode(SyntaxNode* nextArgument, SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::CallArgument);
		node->SetLeftNode(nextArgument);
		node->SetRightNode(expression);
		return node;
	}

	//arg_decls
	static SyntaxNode* CreateDeclareArgumentNode(SyntaxNode* declareVariable)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::DeclareArgument);
		node->SetLeftNode(declareVariable);
		return node;
	}

	static SyntaxNode* CreateDeclareArgumentNode(SyntaxNode* nextDeclareArgument, SyntaxNode* declareVariable)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::DeclareArgument);
		node->SetLeftNode(declareVariable);
		node->SetRightNode(nextDeclareArgument);
		return node;
	}

	// variable_decl
	static SyntaxNode* CreateDeclareVariableNode(SyntaxNode* variableType, SyntaxNode* declarator, SyntaxNode* expression)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::DeclareVariable);
		node->SetLeftNode(variableType);
		node->SetRightNode(declarator);
		if (expression)
		{
			MANA_ASSERT(variableType);
			MANA_ASSERT(variableType->GetTypeDescriptor());
			MANA_ASSERT(declarator);
			MANA_ASSERT(declarator->GetString());

			SyntaxNode* identifierNode = mana::CreateIdentifier(declarator->GetString());
			identifierNode->Set(variableType->GetTypeDescriptor());

			SyntaxNode* assignNode = mana::CreateAssign(identifierNode, expression);
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
	static SyntaxNode* CreateDeclaratorNode(const char* identifier)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Declarator);
		node->Set(identifier);
		return node;
	}

	static SyntaxNode* CreateDeclaratorNode(const char* identifier, SyntaxNode* variableSizeNode)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::Declarator);
		node->Set(identifier);
		node->SetLeftNode(variableSizeNode);
		return node;
	}

	// variable_size
	static SyntaxNode* CreateVariableSizeNode(const size_t size)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::VariableSize);
		node->Set(static_cast<int_t>(size));
		return node;
	}

	static SyntaxNode* CreateVariableSizeNode(const char* identifier)
	{
		SyntaxNode* node = new SyntaxNode(SyntaxNode::Id::VariableSize);
		node->Set(identifier);
		return node;
	}
}
