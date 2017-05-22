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
		const std::shared_ptr<SymbolTable>& symbolTable,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
		: SemanticAnalyzer(symbolFactory, symbolTable, typeDescriptorFactory)
	{
#if 0
		{
			/* vec2 */
			symbol_open_structure();
			symbol_allocate_memory(symbol_create_variable("x", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("y", nullptr, false), type_get(Float), NORMAL);
			symbol_close_structure("vec2");

			/* vec3 */
			symbol_open_structure();
			symbol_allocate_memory(symbol_create_variable("x", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("y", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("z", nullptr, false), type_get(Float), NORMAL);
			symbol_close_structure("vec3");

			/* vec4 */
			symbol_open_structure();
			symbol_allocate_memory(symbol_create_variable("x", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("y", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("z", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("w", nullptr, false), type_get(Float), NORMAL);
			symbol_close_structure("vec4");

			/* color */
			symbol_open_structure();
			symbol_allocate_memory(symbol_create_variable("r", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("g", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("b", nullptr, false), type_get(Float), NORMAL);
			symbol_allocate_memory(symbol_create_variable("a", nullptr, false), type_get(Float), NORMAL);
			symbol_close_structure("color");
		}

		{
			SymbolEntry* GetSymbol();

			/* int getUerData() */
			GetSymbol() = symbol_create_function("getUserData");
			GetSymbol()->number_of_parameters = 0;
			symbol_begin_native_function_registration();
			symbol_commit_native_function_registration(GetSymbol(), type_get(Int));

			/* void setUserData(int data) */
			GetSymbol() = symbol_create_function("setUserData");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("data", nullptr, false), type_get(Int), PARAMETER);
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
			symbol_allocate_memory(symbol_create_variable("address", nullptr, false), type_pointer, PARAMETER);
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
			symbol_allocate_memory(symbol_create_variable("count", nullptr, false), type_get(Int), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* void wait(float) */
			GetSymbol() = symbol_create_function("wait");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("second", nullptr, false), type_get(Float), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* void waitFrame(int) */
			GetSymbol() = symbol_create_function("waitFrame");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("frame", nullptr, false), type_get(Int), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* void srand(int count) */
			GetSymbol() = symbol_create_function("srand");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("count", nullptr, false), type_get(Int), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Void));

			/* int rand(int, int) */
			GetSymbol() = symbol_create_function("rand");
			GetSymbol()->number_of_parameters = 2;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("min", nullptr, false), type_get(Int), PARAMETER);
			symbol_allocate_memory(symbol_create_variable("max", nullptr, false), type_get(Int), PARAMETER);
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
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float cos(float) */
			GetSymbol() = symbol_create_function("cos");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float atan2(float, float) */
			GetSymbol() = symbol_create_function("atan2");
			GetSymbol()->number_of_parameters = 2;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("y", nullptr, false), type_get(Float), PARAMETER);
			symbol_allocate_memory(symbol_create_variable("x", nullptr, false), type_get(Float), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float tan(float) */
			GetSymbol() = symbol_create_function("tan");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float angleMod(float) */
			GetSymbol() = symbol_create_function("angleMod");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* float sqrt(float) */
			GetSymbol() = symbol_create_function("sqrt");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("degree", nullptr, false), type_get(Float), PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(Float));

			/* actor get,(GetString()) */
			GetSymbol() = symbol_create_function("get,");
			GetSymbol()->number_of_parameters = 1;
			symbol_begin_native_function_registration();
			symbol_allocate_memory(symbol_create_variable("name", nullptr, false), type_string, PARAMETER);
			symbol_commit_native_function_registration(GetSymbol(), type_get(ACTOR));
		}
#endif
	}

	GlobalSemanticAnalyzer::~GlobalSemanticAnalyzer()
	{
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
		SetCurrentFileInfomation(node);

		switch (node->GetId())
		{
			// 特に処理を行わないノード
		case SyntaxNode::Id::Identifier:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			// TODO:post_resolverと確認して下さい
			SearchSymbolFromName(node);
			break;

			// 定数定義に関するノード									
		case SyntaxNode::Id::Alias:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);

			GetSymbolFactory()->CreateAlias(node->GetString(), node->GetString());
			break;

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

			// メモリレイアウトに関するノード									
		case SyntaxNode::Id::Allocate:
			{
				int32_t allocatedSize = mStaticBlockOpend ? GetSymbolTable()->GetStaticMemoryAddress() : GetSymbolTable()->GetGlobalMemoryAddress();
				allocatedSize += node->GetInt();

				Resolve(node->GetLeftNode());

				const int32_t address = mStaticBlockOpend ? GetSymbolTable()->GetStaticMemoryAddress() : GetSymbolTable()->GetGlobalMemoryAddress();
				if (address >= allocatedSize)
					CompileError("static variable range over");

				if(mStaticBlockOpend)
					GetSymbolTable()->SetStaticMemoryAddress(allocatedSize);
				else
					GetSymbolTable()->SetGlobalMemoryAddress(allocatedSize);
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Static:
			mStaticBlockOpend = true;
			Resolve(node->GetLeftNode());
			mStaticBlockOpend = false;
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			// 構造に関するノード
		case SyntaxNode::Id::Actor:
			{
				GetSymbolTable()->BeginRegistrationActor(Lookup(node->GetString()));
				Resolve(node->GetLeftNode());
				GetSymbolTable()->CommitRegistrationActor(node->GetString(), nullptr, nullptr, false);
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Extend:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			GetSymbolTable()->ExtendModule(node->GetString());
			break;

		case SyntaxNode::Id::Module:
			{
				GetSymbolTable()->BeginRegistrationModule(Lookup(node->GetString()));
				Resolve(node->GetLeftNode());
				GetSymbolTable()->CommitRegistrationModule(node->GetString());
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Phantom:
			{
				GetSymbolTable()->BeginRegistrationActor(Lookup(node->GetString()));
				Resolve(node->GetLeftNode());
				GetSymbolTable()->CommitRegistrationActor(node->GetString(), nullptr, nullptr, true);
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Struct:
			GetSymbolTable()->OpenStructure();
			Resolve(node->GetLeftNode());
			GetSymbolTable()->CloseStructure(node->GetString());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			// 関数宣言に関するノード									
		case SyntaxNode::Id::Action:
			{
				MANA_ASSERT(node->GetSymbol() == nullptr);
				node->Set(GetSymbolFactory()->CreateFunction(node->GetString(), GetSymbolTable()->IsActorOrStructerOpened(), GetSymbolTable()->IsFunctionOpened()));
				node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void));
			}
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareArgument:
			ResolveVariableDescription(node->GetLeftNode(), SymbolEntry::MemoryTypeId::PARAMETER, mStaticBlockOpend);
			Resolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareFunction:
			{
				MANA_ASSERT(node->GetSymbol() == nullptr);
				// 関数の戻り値を評価
				Resolve(node->GetLeftNode());
				// シンボルの作成と型の定義
				node->Set(GetSymbolFactory()->CreateFunction(node->GetString(), GetSymbolTable()->IsActorOrStructerOpened(), GetSymbolTable()->IsModuleOpened()));
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
				node->Set(GetSymbolFactory()->CreateFunction(node->GetString(), GetSymbolTable()->IsActorOrStructerOpened(), GetSymbolTable()->IsModuleOpened()));
				GetSymbolTable()->BeginNativeFunction();
				Resolve(node->GetLeftNode());
				Resolve(node->GetRightNode());
				node->GetSymbol()->SetNumberOfParameters(CalcArgumentCount(0, node->GetRightNode()));
				GetSymbolTable()->CloseNativeFunction(node->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor());
			}
			break;

			// 変数宣言に関するノード									
		case SyntaxNode::Id::Declarator:
			MANA_ASSERT(node->GetSymbol() == nullptr);
			node->Set(GetSymbolFactory()->CreateVariable(
				node->GetString(),
				nullptr,
				mStaticBlockOpend,
				GetSymbolTable()->IsOpenBlock(),
				GetSymbolTable()->IsFunctionOpened()
			));
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareVariable:
			ResolveVariableDescription(node, SymbolEntry::MemoryTypeId::NORMAL, mStaticBlockOpend);
			MANA_ASSERT(node->GetLeftNode() && node->GetLeftNode()->GetId() == SyntaxNode::Id::TypeDescription);
			MANA_ASSERT(node->GetRightNode() && node->GetRightNode()->GetId() == SyntaxNode::Id::Declarator);
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
			if (node->GetString())
			{
				std::shared_ptr<SymbolEntry> symbol = Lookup(node->GetString());
				if (symbol)
					node->Set(static_cast<int_t>(symbol->GetAddress()));
				else
					CompileError("undefined GetSymbol() '%s'", node->GetString());
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// 以下はシンボル生成を行わないノード
			///////////////////////////////////////////////////////////////////////

			// ブロックを伴う制御に関するノード
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

			// 複数の命令に展開されるノード									
		case SyntaxNode::Id::Accept:
		case SyntaxNode::Id::Join:
		case SyntaxNode::Id::Print:
		case SyntaxNode::Id::Reject:
		case SyntaxNode::Id::Request:
		case SyntaxNode::Id::String:
		case SyntaxNode::Id::Yield:
		case SyntaxNode::Id::Sizeof:

			// 中間言語と対になるノード
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

		// 子ノードから型を継承する
		ResolveTypeFromChildNode(node);

		if (node->GetNextNode())
		{
			// 末尾再帰なのでgotoにて処理する Resolve(node->GetNextNode());
			node = node->GetNextNode();
			goto DO_RECURSIVE;
		}
	}
}
