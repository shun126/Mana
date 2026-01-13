/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "GlobalSemanticAnalyzer.h"
#include "ErrorHandler.h"
#include "SyntaxNode.h"

namespace mana
{
	GlobalSemanticAnalyzer::GlobalSemanticAnalyzer(
		const std::shared_ptr<SymbolFactory>& symbolFactory,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
		: SemanticAnalyzer(symbolFactory, typeDescriptorFactory)
	{
		{
			// vec2
			GetSymbolFactory()->BeginRegistrationStructure();
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("x", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("y", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->CommitRegistrationStructure("vec2");

			// vec3
			GetSymbolFactory()->BeginRegistrationStructure();
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("x", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("y", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("z", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->CommitRegistrationStructure("vec3");

			// vec4
			GetSymbolFactory()->BeginRegistrationStructure();
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("x", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("y", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("z", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("w", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->CommitRegistrationStructure("vec4");

			// rotator
			GetSymbolFactory()->BeginRegistrationStructure();
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("pitch", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("yaw", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("roll", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->CommitRegistrationStructure("rotator");

			// color
			GetSymbolFactory()->BeginRegistrationStructure();
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("r", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("g", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("b", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->AllocateMemory(GetSymbolFactory()->CreateVariable("a", nullptr, false, GetSymbolFactory()->IsOpenBlock(), GetSymbolFactory()->IsFunctionOpened()), GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float), Symbol::MemoryTypeId::Normal);
			GetSymbolFactory()->CommitRegistrationStructure("color");

			// TODO: transform
		}

#if 0
		{
			Symbol* GetSymbol();

			/* int getUerData() */
			GetSymbol() = symbol_create_function("getUserData");
			GetSymbol()->number_of_parameters = 0;
			symbol_begin_native_function_registration();
			symbol_commit_native_function_registration(GetSymbol(), type_get(Int));

			/* void setUserData(int data) */
			GetSymbol() = symbol_create_function("setUserData");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("data", nullptr, false), type_get(Int), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* pointer getUserPointer() */
			GetSymbol() = symbol_create_function("getUserPointer");
			GetSymbol()->number_of_parameters = 0;
			symbol_begin_native_function_registration();
			symbol_commit_native_function_registration(GetSymbol(), type_pointer);

			/* void setUserPointer(pointer address) */
			GetSymbol() = symbol_create_function("setUserPointer");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("address", nullptr, false), type_pointer, Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* int debug() */
			GetSymbol() = symbol_create_function("debug");
			GetSymbol()->number_of_parameters = 0;
			symbol_begin_native_function_registration();
			symbol_commit_native_function_registration(GetSymbol(), type_get(Int));

			/* void setTickCount(int count) */
			GetSymbol() = symbol_create_function("setTickCount");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("count", nullptr, false), type_get(Int), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* void wait(float) */
			GetSymbol() = symbol_create_function("wait");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("second", nullptr, false), type_get(Float), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* void waitFrame(int) */
			GetSymbol() = symbol_create_function("waitFrame");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("frame", nullptr, false), type_get(Int), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* void srand(int count) */
			GetSymbol() = symbol_create_function("srand");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("count", nullptr, false), type_get(Int), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* int rand(int, int) */
			GetSymbol() = symbol_create_function("rand");
			GetSymbol()->number_of_parameters = 2;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("min", nullptr, false), type_get(Int), Parameter);
			symbol_allocate_memory(symbol_create_variable("max", nullptr, false), type_get(Int), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Int));

			/* float frand() */
			GetSymbol() = symbol_create_function("frand");
			GetSymbol()->number_of_parameters = 0;
			symbol_begin_native_function_registration();
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float sin(float) */
			GetSymbol() = symbol_create_function("sin");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float cos(float) */
			GetSymbol() = symbol_create_function("cos");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float atan2(float, float) */
			GetSymbol() = symbol_create_function("atan2");
			GetSymbol()->number_of_parameters = 2;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("y", nullptr, false), type_get(Float), Parameter);
			symbol_allocate_memory(symbol_create_variable("x", nullptr, false), type_get(Float), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float tan(float) */
			GetSymbol() = symbol_create_function("tan");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float angleMod(float) */
			GetSymbol() = symbol_create_function("angleMod");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float sqrt(float) */
			GetSymbol() = symbol_create_function("sqrt");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* actor get,(GetString()) */
			GetSymbol() = symbol_create_function("get,");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("name", nullptr, false), type_string, Parameter);
			symbol_commit_native_function_registration(GetSymbol(), type_get(ACTOR));
		}
#endif
	}

	size_t GlobalSemanticAnalyzer::CalcArgumentCount(const size_t count, const std::shared_ptr<const SyntaxNode>& node) const
	{
		if (node == nullptr)
			return count;

		// TODO
		//MANA_ASSERT(node->GetId() == SyntaxNode::Id::DeclareArgument);
		return CalcArgumentCount(count, node->GetRightNode()) + 1;
	}

	void GlobalSemanticAnalyzer::Resolve(std::shared_ptr<SyntaxNode> node)
	{
		if (node == nullptr)
			return;

	DO_RECURSIVE:
		SetCurrentFileInformation(node);

		switch (node->GetId())
		{
			// Nodes with no processing
		case SyntaxNode::Id::Identifier:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			// TODO:post_resolverと確認して下さい
			SearchSymbolFromName(node);
			break;

			// Nodes related to constant definitions
		case SyntaxNode::Id::DefineConstant:
			MANA_ASSERT(node->GetTypeDescriptor());
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			switch (node->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Int:
				GetSymbolFactory()->CreateConstInt(node->GetString(), node->GetInt());
				break;
			case TypeDescriptor::Id::Float:
				GetSymbolFactory()->CreateConstFloat(node->GetString(), node->GetFloat());
				break;
			default:
				// TODO
				GetSymbolFactory()->CreateConstString(node->GetString(), node->GetString());
				break;
			}
			break;

		case SyntaxNode::Id::UndefineConstant:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			GetSymbolFactory()->Destroy(node->GetString());
			break;

			// Nodes related to memory layout
		case SyntaxNode::Id::Allocate:
			{
				int32_t allocatedSize = mStaticBlockOpened ? GetSymbolFactory()->GetStaticMemoryAddress() : GetSymbolFactory()->GetGlobalMemoryAddress();
				allocatedSize += node->GetInt();

				Resolve(node->GetLeftNode());

				const int32_t address = mStaticBlockOpened ? GetSymbolFactory()->GetStaticMemoryAddress() : GetSymbolFactory()->GetGlobalMemoryAddress();
				if (address >= allocatedSize)
					CompileError("static variable range over");

				if(mStaticBlockOpened)
					GetSymbolFactory()->SetStaticMemoryAddress(allocatedSize);
				else
					GetSymbolFactory()->SetGlobalMemoryAddress(allocatedSize);
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Static:
			mStaticBlockOpened = true;
			Resolve(node->GetLeftNode());
			mStaticBlockOpened = false;
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			// Nodes related to structure
		case SyntaxNode::Id::Actor:
			{
				GetSymbolFactory()->BeginRegistrationActor(Lookup(node->GetString()));
				Resolve(node->GetLeftNode());
				GetSymbolFactory()->CommitRegistrationActor(node->GetString(), /*nullptr*/"", nullptr, false);
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Extend:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			GetSymbolFactory()->ExtendModule(node->GetString());
			break;

		case SyntaxNode::Id::Module:
			{
				GetSymbolFactory()->BeginRegistrationModule(Lookup(node->GetString()));
				Resolve(node->GetLeftNode());
				GetSymbolFactory()->CommitRegistrationModule(node->GetString());
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Phantom:
			{
				GetSymbolFactory()->BeginRegistrationActor(Lookup(node->GetString()));
				Resolve(node->GetLeftNode());
				GetSymbolFactory()->CommitRegistrationActor(node->GetString(), /*nullptr*/"", nullptr, true);
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Struct:
			GetSymbolFactory()->BeginRegistrationStructure();
			Resolve(node->GetLeftNode());
			GetSymbolFactory()->CommitRegistrationStructure(node->GetString());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			// Nodes related to function declarations
		case SyntaxNode::Id::Action:
			{
				MANA_ASSERT(node->GetSymbol() == nullptr);
				node->Set(GetSymbolFactory()->CreateFunction(node->GetString(), GetSymbolFactory()->IsActorOrStructerOpened()));
				node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void));
			}
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareArgument:
			ResolveVariableDescription(node->GetLeftNode(), Symbol::MemoryTypeId::Parameter, mStaticBlockOpened);
			Resolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareFunction:
			{
				MANA_ASSERT(node->GetSymbol() == nullptr);
				// 関数の戻り値を評価
				Resolve(node->GetLeftNode());
				// シンボルの作成と型の定義
				node->Set(GetSymbolFactory()->CreateFunction(node->GetString(), GetSymbolFactory()->IsActorOrStructerOpened()));
				node->GetSymbol()->SetTypeDescription(node->GetLeftNode()->GetTypeDescriptor());
				// シンボルに引数の数を登録
				node->GetSymbol()->SetNumberOfParameters(CalcArgumentCount(0, node->GetRightNode()));
			}
			// node->GetBodyNode()
			break;

		case SyntaxNode::Id::NativeFunction:
			{
				MANA_ASSERT(node->GetSymbol() == nullptr);
				// シンボルの作成と型の定義
				node->Set(GetSymbolFactory()->CreateFunction(node->GetString(), GetSymbolFactory()->IsActorOrStructerOpened()));
				GetSymbolFactory()->BeginNativeFunction();
				Resolve(node->GetLeftNode());
				Resolve(node->GetRightNode());
				node->GetSymbol()->SetNumberOfParameters(CalcArgumentCount(0, node->GetRightNode()));
				GetSymbolFactory()->CloseNativeFunction(node->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor());
			}
			break;

			// Nodes related to variable declarations
		case SyntaxNode::Id::Declarator:
			MANA_ASSERT(node->GetSymbol() == nullptr);
			node->Set(GetSymbolFactory()->CreateVariable(
				node->GetString(),
				nullptr,
				mStaticBlockOpened,
				GetSymbolFactory()->IsOpenBlock(),
				GetSymbolFactory()->IsFunctionOpened()
			));
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareVariable:
			MANA_ASSERT(node->GetLeftNode() && node->GetLeftNode()->Is(SyntaxNode::Id::TypeDescription));
			MANA_ASSERT(node->GetRightNode() && node->GetRightNode()->Is(SyntaxNode::Id::Declarator));
			ResolveVariableDescription(node, Symbol::MemoryTypeId::Normal, mStaticBlockOpened);
			if (node->GetBodyNode() != nullptr)
			{
				switch (node->GetRightNode()->GetSymbol()->GetClassTypeId())
				{
				case Symbol::ClassTypeId::GlobalVariable:
				case Symbol::ClassTypeId::StaticVariable:
					Resolve(node->GetBodyNode());
					break;
				default:
					CompileError({ "can initialize variable in global space only" });
					break;
				}
			}
			break;

		case SyntaxNode::Id::TypeDescription:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			ResolveTypeDescription(node);
			break;

		case SyntaxNode::Id::VariableSize:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			if (!node->GetString().empty())
			{
				std::shared_ptr<Symbol> symbol = Lookup(node->GetString());
				if (symbol)
					node->Set(static_cast<int_t>(symbol->GetAddress()));
				else
					CompileError({ "undefined GetSymbol() '", node->GetString(), "'" });
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// The following are nodes that do not generate symbols
			///////////////////////////////////////////////////////////////////////

			// Node on control with block
		case SyntaxNode::Id::Block:
		case SyntaxNode::Id::Break:
		case SyntaxNode::Id::Case:
		case SyntaxNode::Id::Continue:
		case SyntaxNode::Id::Default:
		case SyntaxNode::Id::Do:
		case SyntaxNode::Id::For:
		case SyntaxNode::Id::Goto:
		case SyntaxNode::Id::Halt:
		case SyntaxNode::Id::If:
		case SyntaxNode::Id::Label:
		case SyntaxNode::Id::Lock:
		case SyntaxNode::Id::Loop:
		case SyntaxNode::Id::Return:
		case SyntaxNode::Id::Rollback:
		case SyntaxNode::Id::Switch:
		case SyntaxNode::Id::While:

			// Nodes to be expanded into multiple instructions
		case SyntaxNode::Id::Accept:
		case SyntaxNode::Id::Join:
		case SyntaxNode::Id::Print:
		case SyntaxNode::Id::Reject:
		case SyntaxNode::Id::Request:
		case SyntaxNode::Id::String:
		case SyntaxNode::Id::Yield:
		case SyntaxNode::Id::Sizeof:

			// Nodes paired with intermediate languages
		case SyntaxNode::Id::Array:
		case SyntaxNode::Id::Assign:
		case SyntaxNode::Id::Const:
		case SyntaxNode::Id::ExpressionIf:
		case SyntaxNode::Id::MemberFunction:
		case SyntaxNode::Id::MemberVariable:
		case SyntaxNode::Id::Call:
		case SyntaxNode::Id::CallArgument:
		case SyntaxNode::Id::Add:
		case SyntaxNode::Id::Sub:
		case SyntaxNode::Id::Mul:
		case SyntaxNode::Id::Div:
		case SyntaxNode::Id::Rem:
		case SyntaxNode::Id::Neg:
		case SyntaxNode::Id::Pow:
		case SyntaxNode::Id::Not:
		case SyntaxNode::Id::And:
		case SyntaxNode::Id::Or:
		case SyntaxNode::Id::Xor:
		case SyntaxNode::Id::LeftShift:
		case SyntaxNode::Id::RightShift:
		case SyntaxNode::Id::Less:
		case SyntaxNode::Id::LessEqual:
		case SyntaxNode::Id::Equal:
		case SyntaxNode::Id::NotEqual:
		case SyntaxNode::Id::GreaterEqual:
		case SyntaxNode::Id::Greater:
		case SyntaxNode::Id::IntegerToFloat:
		case SyntaxNode::Id::FloatToInteger:
		case SyntaxNode::Id::LogicalOr:
		case SyntaxNode::Id::LogicalAnd:
		case SyntaxNode::Id::LogicalNot:
		case SyntaxNode::Id::Sender:
		case SyntaxNode::Id::Self:
		case SyntaxNode::Id::Priority:
		default:
			MANA_BUG("illigal node detect");
		}

		// Inherit type from a child node
		ResolveTypeFromChildNode(node);

		if (node->GetNextNode())
		{
			/*
			Since it is tail recursion, it is handled by goto
			Equivalent to Resolve(node->GetNextNode())
			*/
			node = node->GetNextNode();
			goto DO_RECURSIVE;
		}
	}
}
