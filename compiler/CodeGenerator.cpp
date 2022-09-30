/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "CodeGenerator.h"
#include "ErrorHandler.h"
#include "IntermediateLanguage.h"
#include "Symbol.h"
#include "TypeDescriptor.h"

namespace mana
{
	CodeGenerator::CodeGenerator(
		const std::shared_ptr<CodeBuffer>& codeBuffer,
		const std::shared_ptr<DataBuffer>& dataBuffer,
		const std::shared_ptr<GlobalSemanticAnalyzer>& globalSemanticAnalyzer,
		const std::shared_ptr<LocalSemanticAnalyzer>& localSemanticAnalyzer,
		const std::shared_ptr<SymbolFactory>& symbolFactory,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory)
		: mCodeBuffer(codeBuffer)
		, mDataBuffer(dataBuffer)
		, mGlobalAddressResolver(std::make_shared<GlobalAddressResolver>(mCodeBuffer))
		, mGlobalSemanticAnalyzer(globalSemanticAnalyzer)
		, mLocalAddressResolver(std::make_shared<LocalAddressResolver>(mCodeBuffer))
		, mLocalSemanticAnalyzer(localSemanticAnalyzer)
		, mSymbolFactory(symbolFactory)
		, mTypeDescriptorFactory(typeDescriptorFactory)
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	// 以下は シンボルを解決しつつコードを出力する 関数郡
	////////////////////////////////////////////////////////////////////////////////
	/*!
	スタックにあるアドレスからデータを読み込む命令を出力する
	*/
	void CodeGenerator::generator_resolve_load(std::shared_ptr<SyntaxNode> node)
	{
		//	register_entity* register_entity;

		MANA_ASSERT(node);
		MANA_ASSERT(node->GetTypeDescriptor());
#if 0
		/* TODO: レジスタ割り当てテスト */
		register_entity = register_allocate(node->GetSymbol(), node->GetSymbol()->address);
		if (register_entity == nullptr)
		{
			register_entity* register_entity = register_allocate(node->GetSymbol(), node->GetSymbol()->address);
			if (register_entity == nullptr)
			{
				/* 割り当てるレジスタが無い場合はspillを行う */
			}
			/* レジスタに空きがある、または既に割り当て済みの場合 */
		}
#endif
		switch (node->GetTypeDescriptor()->GetId())
		{
		case TypeDescriptor::Id::Char:
			mCodeBuffer->AddOpecode(MANA_IL_LOAD_CHAR);
			break;

		case TypeDescriptor::Id::Short:
			mCodeBuffer->AddOpecode(MANA_IL_LOAD_SHORT);
			break;

		case TypeDescriptor::Id::Int:
			mCodeBuffer->AddOpecode(MANA_IL_LOAD_INTEGER);
			break;

		case TypeDescriptor::Id::Float:
			mCodeBuffer->AddOpecode(MANA_IL_LOAD_FLOAT);
			break;

		case TypeDescriptor::Id::Reference:
			mCodeBuffer->AddOpecode(MANA_IL_LOAD_REFFRENCE);
			break;

		case TypeDescriptor::Id::Actor:
			if (node->GetTypeDescriptor() == mTypeDescriptorFactory->Get(TypeDescriptor::Id::Actor))
				mCodeBuffer->AddOpecode(MANA_IL_LOAD_REFFRENCE);
			else
				mCodeBuffer->AddOpecodeAndOperand(MANA_IL_PUSH_ACTOR, mDataBuffer->Set(node->GetTypeDescriptor()->GetName()));
			break;

		case TypeDescriptor::Id::Array:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);

		case TypeDescriptor::Id::Struct:
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_LOAD_DATA, node->GetTypeDescriptor()->GetMemorySize());
			break;

		default:
			MANA_BUG("illigal data GetTypeDescriptor()");
		}
	}

	/*!
	スタックにあるアドレスへデータを書き込む命令を出力する
	*/
	void CodeGenerator::generator_resolve_store(std::shared_ptr<SyntaxNode> node)
	{
		MANA_ASSERT(node);
		MANA_ASSERT(node->GetTypeDescriptor());

		switch (node->GetTypeDescriptor()->GetId())
		{
		case TypeDescriptor::Id::Char:
			mCodeBuffer->AddOpecode(MANA_IL_STORE_CHAR);
			break;

		case TypeDescriptor::Id::Short:
			mCodeBuffer->AddOpecode(MANA_IL_STORE_SHORT);
			break;

		case TypeDescriptor::Id::Int:
			mCodeBuffer->AddOpecode(MANA_IL_STORE_INTEGER);
			break;

		case TypeDescriptor::Id::Float:
			mCodeBuffer->AddOpecode(MANA_IL_STORE_FLOAT);
			break;

		case TypeDescriptor::Id::Reference:
		case TypeDescriptor::Id::Actor:
			mCodeBuffer->AddOpecode(MANA_IL_STORE_REFFRENCE);
			break;

		case TypeDescriptor::Id::Array:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);

		case TypeDescriptor::Id::Struct:
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_STORE_DATA, node->GetTypeDescriptor()->GetMemorySize());
			break;

		default:
			CompileError("illegal type stored");
			break;
		}
	}

	/*!
	returnの処理
	@param	func	returnが所属する関数のSymbol
	@param	tree	return文のSyntaxNode
	*/
	void CodeGenerator::generator_return(std::shared_ptr<Symbol> func, std::shared_ptr<SyntaxNode> tree)
	{
		// Error check
		std::shared_ptr<TypeDescriptor> type = func->GetTypeDescriptor();
		if (type->Is(TypeDescriptor::Id::Void))
		{
			if (tree != nullptr)
				CompileError("meaningless return value specification");
		}
		else if (tree == nullptr)
		{
			CompileError("missing return value");
		}
		else
		{
			// Auto type cast
			tree->SetLeftNode(tree->GetLeftNode()->Cast(type, mTypeDescriptorFactory));

			// 型の検証
			TypeDescriptor::Compatible(type, tree->GetLeftNode()->GetTypeDescriptor());

			// ノードの評価
			const int32_t in_depth = mSymbolFactory->OpenBlock(false);
			generator_genearte_code(tree->GetLeftNode(), true);
			const int32_t out_depth = mSymbolFactory->CloseBlock();
			MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
		}

		// 関数の最後にジャンプ
		// TODO:SymbolTable側でコード生成できるかも
		mSymbolFactory->SetReturnAddressList(
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, mSymbolFactory->GetReturnAddressList())
		);

		// 関数を使用したフラグを立てる
		func->SetUsed(true);
	}

	/*!
	rollbackの処理
	@param	tree	rollback文のSyntaxNode
	*/
	void CodeGenerator::generator_rollback(std::shared_ptr<SyntaxNode> tree)
	{
		if (tree)
		{
			const int32_t in_depth = mSymbolFactory->OpenBlock(false);
			generator_genearte_code(tree, true);
			const int32_t out_depth = mSymbolFactory->CloseBlock();
			MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
		}
		mCodeBuffer->AddOpecode(MANA_IL_ROLLBACK);
	}

	/*!
	関数呼び出し時の引数を出力（再帰呼び出し）
	@param	count	引数の番号
	@param	param	引数のSymbol
	@param	arg		引数のSyntaxNode
	@return	引数の数
	*/
	int32_t CodeGenerator::generate_argument(int32_t count, std::shared_ptr<Symbol> param, std::shared_ptr<SyntaxNode> arg)
	{
		if (param && arg)
		{
			if (arg->Is(SyntaxNode::Id::CallArgument))
			{
				count = generate_argument(count, param->GetNext(), arg->GetRightNode());

				if (arg->GetTypeDescriptor() == nullptr)
				{
					MANA_ASSERT(arg->GetLeftNode());
					if (arg->GetLeftNode()->Is(SyntaxNode::Id::Identifier))
						mGlobalSemanticAnalyzer->SearchSymbolFromName(arg->GetLeftNode());

					// 子ノードから型を継承する
					mGlobalSemanticAnalyzer->ResolveTypeFromChildNode(arg);
				}
			}

			// TODO:cast
			arg = arg->Cast(param->GetTypeDescriptor(), mTypeDescriptorFactory);
			param->GetTypeDescriptor()->Compatible(arg->GetTypeDescriptor());
			generator_genearte_code(arg->Is(SyntaxNode::Id::CallArgument) ? arg->GetLeftNode() : arg, true);
		}
		if (arg)
			++count;
		return count;
	}

	/*!
	* 関数の引数のサイズを調べます
	* @param	address	アドレス
	* @param	param	引数のSymbol
	* @param	arg		引数のSyntaxNode
	* @return	引数のサイズ
	*/
	int32_t CodeGenerator::generator_call_argument_size(int32_t address, std::shared_ptr<Symbol> param, std::shared_ptr<SyntaxNode> arg)
	{
		if (param && arg)
		{
			address += generator_call_argument_size(address, param->GetNext(), arg->GetRightNode());

			if (arg->Is(SyntaxNode::Id::CallArgument))
				arg = arg->GetLeftNode();

			address += Alignment(arg->GetMemorySize(), sizeof(int32_t));
		}
		return address;
	}

	/*!
	* 関数の引数を評価します
	* @param	address	引数のアドレス
	* @param	param	引数のSymbol
	* @param	arg		引数のSyntaxNode
	* @return	引数のアドレス
	*/
	int32_t CodeGenerator::generator_call_argument(int32_t address, std::shared_ptr<Symbol> param, std::shared_ptr<SyntaxNode> arg)
	{
		if (param && arg)
		{
			address = generator_call_argument(address, param->GetNext(), arg->GetRightNode());
			if (address < std::numeric_limits<int16_t>::min() || std::numeric_limits<int16_t>::max() < address)
				throw std::range_error("call argument size");
			mCodeBuffer->Add<int16_t>(static_cast<int16_t>(address));

			if (arg->Is(SyntaxNode::Id::CallArgument))
				arg = arg->GetLeftNode();

			address -= Alignment(arg->GetMemorySize(), sizeof(int32_t));
		}
		return address;
	}

	/*!
	* 関数呼び出しのノードを評価します
	* @param	関数呼び出しのSyntaxNode
	*/
	void CodeGenerator::generator_call(std::shared_ptr<SyntaxNode> node)
	{
		std::shared_ptr<SyntaxNode> argument = node->GetRightNode();
		int32_t argument_counter = generate_argument(0, (node->GetSymbol())->GetParameterList(), argument);

		// エラーチェック
		if ((node->GetSymbol())->GetNumberOfParameters() != argument_counter)
		{
			// 引数の数が一致しない
			CompileError("unmatched argument.");
		}
		else if ((node->GetSymbol())->GetClassTypeId() == Symbol::ClassTypeId::NATIVE_FUNCTION)
		{
			// for external function
			int32_t argument_size = generator_call_argument_size(0, (node->GetSymbol())->GetParameterList(), argument);

			node->GetSymbol()->SetAddress(mDataBuffer->Set(node->GetSymbol()->GetName()));

			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_CALL, (node->GetSymbol())->GetAddress());
			mCodeBuffer->Add<uint8_t>(((node->GetSymbol())->GetTypeDescriptor()->GetId() != TypeDescriptor::Id::Void));
			mCodeBuffer->Add<uint8_t>(argument_counter);
			mCodeBuffer->Add<uint16_t>(argument_size);
			generator_call_argument(argument_size - 1, (node->GetSymbol())->GetParameterList(), argument);
		}
		else
		{
			// for internal function
			const int32_t address = mCodeBuffer->AddOpecodeAndOperand(
				MANA_IL_BSR,
				node->GetSymbol()->GetAddress()
			);
			mGlobalAddressResolver->AddCallList(
				address + 1,
				node->GetSymbol()
			);
		}
	}

	int32_t CodeGenerator::generator_call_print_generate_argument(int32_t argc, std::shared_ptr<SyntaxNode> node)
	{
		if (node)
		{
			argc = generator_call_print_generate_argument(argc, node->GetRightNode());

			generator_genearte_code(node->Is(SyntaxNode::Id::CallArgument) ? node->GetLeftNode() : node, true);

			++argc;
		}
		return argc;
	}

	/*!
	* print文の処理
	* @param[in]	node	print文のSyntaxNode
	*/
	void CodeGenerator::generator_call_print(std::shared_ptr<SyntaxNode> node)
	{
		mCodeBuffer->AddOpecodeAndOperand(
			MANA_IL_PRINT,
			generator_call_print_generate_argument(0, node)
		);
	}

	/*!
	* 判別式内に代入文があるか調べます
	* @param	tree	評価式のSyntaxNode
	*/
	void CodeGenerator::generator_condition_check(std::shared_ptr<SyntaxNode> tree)
	{
		if (tree)
		{
			if (tree->Is(SyntaxNode::Id::Assign))
			{
				// 条件判定に代入文は使用できない
				CompileError("can't assign expression in condition");
			}

			generator_condition_check(tree->GetLeftNode());
			generator_condition_check(tree->GetRightNode());
		}
	}

	/*!
	* 判別式の評価
	* @param	tree	評価式のSyntaxNode
	* @param	pc		プログラムカウンタ
	* @return	現在のプログラムアドレス
	*/
	size_t CodeGenerator::generator_condition_core(std::shared_ptr<SyntaxNode> tree)
	{
		// 判別式内に代入式があるか調べます
		generator_condition_check(tree);

		if (tree)
		{
			if (
				tree->GetTypeDescriptor()->Is(TypeDescriptor::Id::Void) ||
				tree->GetTypeDescriptor()->GetId() > TypeDescriptor::Id::Reference)
			{
				CompileError("illegal type of expression in condition");
			}
			generator_genearte_code(tree, true);
		}
		return mCodeBuffer->GetSize() - 5;
	}

	/*!
	判別式の評価
	@param	tree	評価式のSyntaxNode
	@return	現在のプログラムアドレス
	*/
	size_t CodeGenerator::generator_condition(std::shared_ptr<SyntaxNode> tree, int32_t match)
	{
		//generator_resolve_symbol(tree);
		//generator_automatic_cast(tree);

		/* 判別式の評価 */
		const int32_t in_depth = mSymbolFactory->OpenBlock(false);
		generator_condition_core(tree);
		const int32_t out_depth = mSymbolFactory->CloseBlock();
		MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);

		return mCodeBuffer->AddOpecodeAndOperand(match ? MANA_IL_BEQ : MANA_IL_BNE);
	}

	void CodeGenerator::generator_generate_const_int(const TypeDescriptor::Id type_id, const int32_t value)
	{
		switch (type_id)
		{
		case TypeDescriptor::Id::Char:
			if (value == 0)
			{
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_ZERO_INTEGER);
			}
			else {
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_CHAR);
				mCodeBuffer->Add((int8_t)value);
			}
			break;

		case TypeDescriptor::Id::Short:
			if (value == 0)
			{
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_ZERO_INTEGER);
			}
			else {
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_SHORT);
				mCodeBuffer->Add((int16_t)value);
			}
			break;

		case TypeDescriptor::Id::Int:
			if (value == 0)
			{
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_ZERO_INTEGER);
			}
			else {
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_INTEGER);
				mCodeBuffer->Add(value);
			}
			break;

		default:
			//CompileError("illegal type of expression in statement");
			MANA_BUG("illegal type of expression");
			break;
		}
	}

	void CodeGenerator::generator_generate_const_float(const TypeDescriptor::Id type_id, const float value)
	{
		switch (type_id)
		{
		case TypeDescriptor::Id::Float:
			if (value == 0.0f)
			{
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_ZERO_FLOAT);
			}
			else
			{
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_FLOAT);
				mCodeBuffer->Add(value);
			}
			break;

		default:
			//CompileError("illegal type of expression in statement");
			MANA_BUG("illegal type of expression");
			break;
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	/*!
	式の評価
	@param	tree			式のSyntaxNode
	@param	enable_assign	trueならば代入式、falseならばそれ以外
	*/
	void CodeGenerator::generator_expression(std::shared_ptr<SyntaxNode> tree, int32_t enable_assign)
	{
		if (tree == nullptr)
			return;

		//generator_resolve_symbol(tree);

		const int32_t in_depth = mSymbolFactory->OpenBlock(false);

		if (enable_assign)
		{
			if (tree->IsNot(SyntaxNode::Id::Assign) && tree->IsNot(SyntaxNode::Id::Call) && tree->GetId() != SyntaxNode::Id::FloatToInteger)
				CompileError("illegal expression in write-statement");
		}
		else {
			if (tree->Is(SyntaxNode::Id::Assign))
				CompileError("illegal expression in write-statement");
		}

		generator_genearte_code(tree, true);

		if (enable_assign && tree->Is(SyntaxNode::Id::Call) && tree->GetTypeDescriptor())
		{
			switch (tree->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Void:
				break;

			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Int:
			case TypeDescriptor::Id::Float:
			case TypeDescriptor::Id::Actor:
				mCodeBuffer->AddOpecode(MANA_IL_REMOVE);
				break;

			case TypeDescriptor::Id::Struct:
				mCodeBuffer->AddOpecodeAndOperand(MANA_IL_REMOVE_DATA, tree->GetTypeDescriptor()->GetMemorySize());
				break;

			default:
				MANA_BUG("illegal type of expression");
				break;
			}
		}

		const int32_t out_depth = mSymbolFactory->CloseBlock();
		MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
	}


	////////////////////////////////////////////////////////////////////////////////
	void CodeGenerator::generator_genearte_code(std::shared_ptr<SyntaxNode> node, int32_t enable_load)
	{
		if (node == nullptr)
			return;

	DO_RECURSIVE:
		mGlobalSemanticAnalyzer->SetCurrentFileInfomation(node);

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
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::UndefineConstant:
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
		{
			actor_symbol_entry_pointer = mSymbolFactory->Lookup(node->GetString());
			mSymbolFactory->OpenActor(node->GetString());
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mSymbolFactory->CloseActor();
			actor_symbol_entry_pointer = nullptr;
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Extend:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Module:
		{
			actor_symbol_entry_pointer = mSymbolFactory->Lookup(node->GetString());
			mSymbolFactory->OpenModule(actor_symbol_entry_pointer);
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mSymbolFactory->CloseModule(node->GetString());
			actor_symbol_entry_pointer = nullptr;
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Phantom:
		{
			actor_symbol_entry_pointer = mSymbolFactory->Lookup(node->GetString());
			mSymbolFactory->OpenActor(node->GetString());
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mSymbolFactory->CloseActor();
			actor_symbol_entry_pointer = nullptr;
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Struct:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mLocalSemanticAnalyzer->PostResolverResolve(node);
			break;

			///////////////////////////////////////////////////////////////////////
			// 関数宣言に関するノード									
		case SyntaxNode::Id::Action:
			{
				node->Set(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Void));
				function_symbol_entry_pointer = mSymbolFactory->Lookup(node->GetString());
				{
					mSymbolFactory->OpenFunction(node, true);
					generator_genearte_code(node->GetLeftNode(), enable_load);
					mSymbolFactory->CloseFunction(node, true);
				}
				function_symbol_entry_pointer = nullptr;
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareArgument:
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			MANA_BUG("illegal type of expression");
			break;

		case SyntaxNode::Id::DeclareFunction:
		{
			MANA_ASSERT(node->GetSymbol());
			// 関数の戻り値を評価
			generator_genearte_code(node->GetLeftNode(), enable_load);
			// シンボルの検索と型の定義
			function_symbol_entry_pointer = mSymbolFactory->Lookup(node->GetString());
			node->Set(function_symbol_entry_pointer);

			// 引数の為にスコープを分ける
			mSymbolFactory->OpenBlock(false);
			{
				// 引数を登録
				mSymbolFactory->OpenFunction(node, false);

				//TODO
				// pre_resolver_resolve
				mGlobalSemanticAnalyzer->Resolve(node->GetRightNode());
				//node->GetSymbol()->GetParameterList() = symbol_get_head_symbol();
				node->GetSymbol()->SetParameterList(mSymbolFactory->GetLastSymbolEntryInBlock());


				mSymbolFactory->OpenFunction2(node->GetSymbol());

				generator_genearte_code(node->GetBodyNode(), enable_load);
				mSymbolFactory->CloseFunction(node, false);
			}
			mSymbolFactory->CloseBlock();

			function_symbol_entry_pointer = nullptr;
		}
		break;

		case SyntaxNode::Id::NativeFunction:
			// node->GetLeftNode()
			// node->GetRightNode()
			// node->GetBodyNode()
			break;

			///////////////////////////////////////////////////////////////////////
			// 変数宣言に関するノード									
		case SyntaxNode::Id::Declarator:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareVariable:
			//symbol_allocate_memory(node->GetRightNode()->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor(), NORMAL);
			// node->GetLeftNode()
			// node->GetRightNode()
			if (node->GetBodyNode())
			{
				// initializer
				generator_genearte_code(node->GetBodyNode(), enable_load);
			}
			//resolver_resolve_variable_description(node, NORMAL);
			/*
			generator_genearte_code(node->GetLeftNode(), enable_load); // SyntaxNode::Id::TypeDescription
			generator_genearte_code(node->GetRightNode(), enable_load);// SyntaxNode::Id::Declarator
			if(node->GetRightNode()->GetSymbol()->class_type == VARIABLE_LOCAL)
			symbol_allocate_memory(node->GetRightNode()->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor(), NORMAL);
			*/
			break;

		case SyntaxNode::Id::TypeDescription:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			//resolver_resolve_type_description(node);
			mGlobalSemanticAnalyzer->ResolveTypeDescription(node);
			break;

		case SyntaxNode::Id::VariableSize:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// ブロックを伴う制御に関するノード
		case SyntaxNode::Id::Block:
		{
			const int32_t in_depth = mSymbolFactory->OpenBlock(false);

			mLocalSemanticAnalyzer->PostResolverResolve(node->GetLeftNode());
			generator_genearte_code(node->GetLeftNode(), enable_load);

			mLocalSemanticAnalyzer->PostResolverResolve(node->GetRightNode());
			generator_genearte_code(node->GetRightNode(), enable_load);

			const int32_t out_depth = mSymbolFactory->CloseBlock();
			MANA_VERIFY_MESSAGE(in_depth == out_depth, "ブロックの深さが一致しません in:%d out:%d", in_depth, out_depth);
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Break:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, mLocalAddressResolver->Break(mCodeBuffer->GetSize()));
			break;

		case SyntaxNode::Id::Case:
			mLocalAddressResolver->RegistSwitchCase(node->GetLeftNode());
			if (node->GetLeftNode() && node->GetLeftNode()->GetTypeDescriptor())
				node->Set(node->GetLeftNode()->GetTypeDescriptor());
			generator_genearte_code(node->GetRightNode(), enable_load);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Continue:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, mLocalAddressResolver->Continue(mCodeBuffer->GetSize()));
			break;

		case SyntaxNode::Id::Default:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			mLocalAddressResolver->RegistSwitchDefault();
			break;

		case SyntaxNode::Id::Do:
		{
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::Do);
			const int32_t address = mCodeBuffer->GetSize();
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mLocalAddressResolver->CloseContinueOnly();

			mCodeBuffer->ReplaceAddressAll(generator_condition(node->GetRightNode(), false), address);
			mLocalAddressResolver->CloseChain();
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::For:
			/* 'for(GetTypeDescriptor() variable = expression' の形式 */
		{
			//symbol_allocate_memory($2, $1, NORMAL);
			//generator_expression(node_create_node(SyntaxNode::Id::NODE_TYPE_ASSIGN, node_create_leaf($2->name), $4), true);
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::For);
			//$$ = GetSize();

			//generator_genearte_code(node->GetLeftNode(), enable_load);
			mLocalAddressResolver->Break(generator_condition(node->GetLeftNode(), true));

			generator_genearte_code(node->GetBodyNode(), enable_load);

			mLocalAddressResolver->CloseContinueOnly();
			generator_expression(node->GetRightNode(), true);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, mLocalAddressResolver->Continue(mCodeBuffer->GetSize()));
			mLocalAddressResolver->CloseChain();
		}
		break;

		case SyntaxNode::Id::Goto:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				std::shared_ptr<Symbol> symbol = mSymbolFactory->CreateLabel(node->GetString());
				symbol->SetEtc(mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, symbol->GetEtc()));
			}
			break;

		case SyntaxNode::Id::Halt:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(MANA_IL_HALT);
			break;

		case SyntaxNode::Id::If:
		{
			int32_t address = generator_condition(node->GetBodyNode(), true);
			generator_genearte_code(node->GetLeftNode(), enable_load);
			if (node->GetRightNode())
			{
				// else block
				const size_t else_begin_address = mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA);
				mCodeBuffer->ReplaceAddressAll(address, mCodeBuffer->GetSize());
				generator_genearte_code(node->GetRightNode(), enable_load);
				address = else_begin_address;
			}
			mCodeBuffer->ReplaceAddressAll(address, mCodeBuffer->GetSize());
		}
		break;

		case SyntaxNode::Id::Label:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				std::shared_ptr<Symbol> symbol = mSymbolFactory->CreateLabel(node->GetString());
				symbol->SetAddress(mCodeBuffer->GetSize());
			}
			break;

		case SyntaxNode::Id::Lock:
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::Lock);
			mCodeBuffer->AddOpecode(MANA_IL_NONPREEMPTIVE);
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mLocalAddressResolver->CloseChain();
			mCodeBuffer->AddOpecode(MANA_IL_PREEMPTIVE);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Loop:
		{
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::Loop);
			const int32_t address = mCodeBuffer->GetSize();
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, address);
			mLocalAddressResolver->CloseChain();
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Return:
			if (node->GetLeftNode())
				node->Set(node->GetLeftNode()->GetTypeDescriptor());
			generator_return(function_symbol_entry_pointer, node);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Rollback:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			if (node->GetLeftNode())
				node->Set(node->GetLeftNode()->GetTypeDescriptor());
			generator_rollback(node);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Switch:
		{
			generator_expression(node->GetLeftNode(), false);
			const int32_t address = mCodeBuffer->GetSize();
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::Switch);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA);
			mLocalAddressResolver->OpenSwitchBlock(node->GetLeftNode()->GetTypeDescriptor());

			generator_genearte_code(node->GetRightNode(), enable_load);

			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, mLocalAddressResolver->Break(mCodeBuffer->GetSize()));
			mCodeBuffer->ReplaceAddressAll(address, mCodeBuffer->GetSize());
			mLocalAddressResolver->ResolveSwitchBlock(shared_from_this());
			mLocalAddressResolver->CloseChain();
			mLocalAddressResolver->CloseSwitchBlock();
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::While:
		{
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::While);
			mLocalAddressResolver->Break(generator_condition(node->GetLeftNode(), true));
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA, mLocalAddressResolver->Continue(mCodeBuffer->GetSize()));
			mLocalAddressResolver->CloseChain();
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		///////////////////////////////////////////////////////////////////////
		// 制御に関するノード
		case SyntaxNode::Id::Accept:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(MANA_IL_COMPLY);
			break;

		case SyntaxNode::Id::Join:
			//generator_resolve_symbol(node);
			mSymbolFactory->AddJoin(shared_from_this(), node->GetLeftNode(), node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Print:
			generator_call_print(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Reject:
			mCodeBuffer->AddOpecode(MANA_IL_REFUSE);
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Request:
			mSymbolFactory->AddRequest(shared_from_this(), MANA_IL_REQ, node->GetLeftNode(), node->GetRightNode(), node->GetString());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Yield:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(MANA_IL_YIELD);
			break;

			///////////////////////////////////////////////////////////////////////
			// 二項演算子に関するノード
		case SyntaxNode::Id::Add:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? MANA_IL_ADD_FLOAT : MANA_IL_ADD_INTEGER);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Sub:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Mul:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? MANA_IL_MUL_FLOAT : MANA_IL_MUL_INTEGER);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Div:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? MANA_IL_DIV_FLOAT : MANA_IL_DIV_INTEGER);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Rem:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? MANA_IL_MOD_FLOAT : MANA_IL_MOD_INTEGER);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Pow:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? MANA_IL_SUB_FLOAT : MANA_IL_SUB_INTEGER);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::And:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode(MANA_IL_AND);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Or:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode(MANA_IL_OR);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Xor:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode(MANA_IL_EOR);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::LeftShift:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode(MANA_IL_SHL);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::RightShift:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->AddOpecode(MANA_IL_SHR);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::LogicalAnd:
		case SyntaxNode::Id::LogicalOr:
		case SyntaxNode::Id::Equal:
		case SyntaxNode::Id::GreaterEqual:
		case SyntaxNode::Id::Greater:
		case SyntaxNode::Id::Less:
		case SyntaxNode::Id::LessEqual:
		case SyntaxNode::Id::NotEqual:
			// 比較、論理演算子
			//generator_automatic_cast(node);
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(node->GetOpecode());
			break;

			///////////////////////////////////////////////////////////////////////
			// 単項演算子に関するノード
		case SyntaxNode::Id::LogicalNot:
		case SyntaxNode::Id::Not:
			//generator_automatic_cast(node);
			generator_genearte_code(node->GetLeftNode(), enable_load);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(node->GetOpecode());
			break;

		case SyntaxNode::Id::Neg:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mCodeBuffer->AddOpecode(node->GetLeftNode()->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::Float ? MANA_IL_MINUS_FLOAT : MANA_IL_MINUS_INTEGER);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 演算に関するノード
		case SyntaxNode::Id::Array:
			/* variable[index] */
			generator_genearte_code(node->GetRightNode(), true);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_PUSH_SIZE, (node->GetTypeDescriptor())->GetMemorySize());
			mCodeBuffer->AddOpecode(MANA_IL_MUL_INTEGER);
			generator_genearte_code(node->GetLeftNode(), false);
			mCodeBuffer->AddOpecode(MANA_IL_ADD_INTEGER);
			if (enable_load)
			{
				generator_resolve_load(node);
			}
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Assign:
			//generator_resolve_symbol(node->GetLeftNode());
			//generator_resolve_symbol(node->GetRightNode());

			//generator_automatic_cast(node);
			generator_genearte_code(node->GetRightNode(), enable_load);
			generator_genearte_code(node->GetLeftNode(), false);
			generator_resolve_store(node->GetLeftNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Call:
			// 関数、アクションを呼びます
			//generator_resolve_symbol(node);
			// resolver_search_symbol_from_name(node)
			if (node->GetSymbol())
			{
				//generator_genearte_code(node->GetRightNode(), enable_load);
				generator_call(node);
			}
			break;

		case SyntaxNode::Id::CallArgument:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			break;

		case SyntaxNode::Id::Const:
			switch (node->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Int:
				generator_generate_const_int(node->GetTypeDescriptor()->GetId(), node->GetInt());
				break;

			case TypeDescriptor::Id::Float:
				generator_generate_const_float(node->GetTypeDescriptor()->GetId(), node->GetFloat());
				break;

			case TypeDescriptor::Id::Nil:
				mCodeBuffer->AddOpecode(MANA_IL_PUSH_ZERO_INTEGER);
				break;

			default:
				CompileError("illegal type of expression in statement");
				break;
			}
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::ExpressionIf:
			// 三項演算子
		{
			int32_t pc1, pc2;
			generator_condition_core(node->GetNextNode());
			pc1 = mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BEQ);
			generator_genearte_code(node->GetLeftNode(), enable_load);
			pc2 = mCodeBuffer->AddOpecodeAndOperand(MANA_IL_BRA);
			mCodeBuffer->ReplaceAddressAll(pc1, mCodeBuffer->GetSize());
			generator_genearte_code(node->GetRightNode(), enable_load);
			mCodeBuffer->ReplaceAddressAll(pc2, mCodeBuffer->GetSize());
		}
		break;

		case SyntaxNode::Id::IntegerToFloat:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mCodeBuffer->AddOpecode(MANA_IL_INT2FLOAT);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::FloatToInteger:
			generator_genearte_code(node->GetLeftNode(), enable_load);
			mCodeBuffer->AddOpecode(MANA_IL_FLOAT2INT);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Identifier:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			if (node->GetSymbol())
			{
				switch (node->GetSymbol()->GetClassTypeId())
				{
				case Symbol::ClassTypeId::ALIAS:
					generator_generate_const_int(node->GetSymbol()->GetTypeDescriptor()->GetId(), node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::CONSTANT_INT:
					generator_generate_const_int(node->GetSymbol()->GetTypeDescriptor()->GetId(), node->GetSymbol()->GetEtc());
					break;

				case Symbol::ClassTypeId::CONSTANT_FLOAT:
					generator_generate_const_float(node->GetSymbol()->GetTypeDescriptor()->GetId(), node->GetSymbol()->GetFloat());
					break;

				case Symbol::ClassTypeId::CONSTANT_STRING:
					generator_generate_const_int(node->GetSymbol()->GetTypeDescriptor()->GetId(), node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::VARIABLE_STATIC:
					mCodeBuffer->AddOpecodeAndOperand(MANA_IL_LOAD_STATIC_ADDRESS, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::VARIABLE_GLOBAL:
					mCodeBuffer->AddOpecodeAndOperand(MANA_IL_LOAD_GLOBAL_ADDRESS, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::VARIABLE_ACTOR:
					mCodeBuffer->AddOpecodeAndOperand(MANA_IL_LOAD_SELF_ADDRESS, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::VARIABLE_LOCAL:
					mCodeBuffer->AddOpecodeAndOperand(MANA_IL_LOAD_FRAME_ADDRESS, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::TYPEDEF:
					break;

				case Symbol::ClassTypeId::NEW_SYMBOL:

				default:
					CompileError("illigal data GetTypeDescriptor()");
					break;
				}

				if (enable_load)
					generator_resolve_load(node);
			}
			break;

		case SyntaxNode::Id::MemberFunction:
			// TODO:実装してください
			generator_genearte_code(node->GetLeftNode(), enable_load);
			generator_genearte_code(node->GetRightNode(), enable_load);
			break;

		case SyntaxNode::Id::MemberVariable:
			MANA_ASSERT(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			{
				//generator_resolve_symbol(node->GetLeftNode());
				/////resolver_search_symbol_from_name(node);

				std::shared_ptr<TypeDescriptor> type = node->GetLeftNode()->GetTypeDescriptor();
				if (type)
				{
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
									mCodeBuffer->AddOpecodeAndOperand(MANA_IL_PUSH_SIZE, symbol->GetAddress());
									generator_genearte_code(node->GetLeftNode(), false);
									mCodeBuffer->AddOpecode(MANA_IL_ADD_INTEGER);
									if (enable_load)
										generator_resolve_load(node);
									goto ESCAPE;
								}
							}
							//type = GetTypeDescriptor()->parent;
						}
						CompileError("reference to undefined field");
					}
					else
					{
						CompileError("reference to non-struct type");
					}
				}
			}
		ESCAPE:
			break;
#if 0
		case SyntaxNode::Id::NODE_MEMOP:
			/* variable.member */
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_PUSH_SIZE, node->etc);
			generator_genearte_code(node->GetLeftNode(), false);
			mCodeBuffer->AddOpecode(MANA_IL_ADD_INTEGER);
			if (enable_load)
			{
				generator_resolve_load(node);
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;
#endif
		case SyntaxNode::Id::Sender:
			// senderをpushします
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(MANA_IL_PUSH_SENDER);
			break;

		case SyntaxNode::Id::Self:
			// nodeをpushします
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(MANA_IL_PUSH_SELF);
			break;

		case SyntaxNode::Id::Priority:
			// priorityをpushします
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(MANA_IL_PUSH_PRIORITY);
			break;

		case SyntaxNode::Id::Sizeof:
		{
#if 0
			TODO:
			int32_t max_char = (1 << (8 * CBSZ - 1)) - 1;
			int32_t min_char = -1 << (8 * CBSZ - 1);
			int32_t max_short = (1 << (8 * SBSZ - 1)) - 1;
			int32_t min_short = -1 << (8 * SBSZ - 1);

			node->Set(node->GetLeftNode()->GetTypeDescriptor()->GetMemorySize());

			if (node->digit <= max_char && node->digit >= min_char)
				node->GetTypeDescriptor() = type_get(TypeDescriptor::Id::Char);
			else if (node->digit <= max_short && node->digit >= min_short)
				node->GetTypeDescriptor() = type_get(TypeDescriptor::Id::Short);
			else
				node->GetTypeDescriptor() = type_get(TypeDescriptor::Id::Int);
#endif
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::String:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecodeAndOperand(MANA_IL_PUSH_STRING, node->GetInt());
			break;
#if 0
		case SyntaxNode::Id::NODE_VARIABLE:
			/* variable */
			generator_genearte_code(node->GetRightNode(), enable_load);
			generator_genearte_code(node->GetLeftNode(), enable_load);

			//generator_generate_variable_value(node);
			switch ((node->GetSymbol())->class_type)
			{
			case VARIABLE_STATIC:
				AddOpecodeAndOperand(MANA_IL_LOAD_STATIC_ADDRESS, (node->GetSymbol())->address);
				break;

			case VARIABLE_GLOBAL:
				AddOpecodeAndOperand(MANA_IL_LOAD_GLOBAL_ADDRESS, (node->GetSymbol())->address);
				break;

			case VARIABLE_ACTOR:
				AddOpecodeAndOperand(MANA_IL_LOAD_SELF_ADDRESS, (node->GetSymbol())->address);
				break;

			case VARIABLE_LOCAL:
				AddOpecodeAndOperand(MANA_IL_LOAD_FRAME_ADDRESS, (node->GetSymbol())->address);
				break;

			case TYPEDEF:
				break;

			default:
				CompileError(node, "illegal type of expression in statement");
				break;
			}

			if (enable_load)
			{
				generator_resolve_load(node);
			}

			break;
#endif
		default:
			MANA_BUG("illegal right-hand side value");
			break;
		}

		// 子ノードから型を継承する
		//resolver_resolve_type_from_child_node(node);

		if (node->GetNextNode())
		{
			// 末尾再帰なのでgotoにて処理する generator_genearte_code(node->GetNextNode());
			node = node->GetNextNode();
			goto DO_RECURSIVE;
		}
	}

	const std::shared_ptr<GlobalAddressResolver>& CodeGenerator::GetGlobalAddressResolver()
	{
		return mGlobalAddressResolver;
	}

	const std::shared_ptr<LocalAddressResolver>& CodeGenerator::GetLocalAddressResolver()
	{
		return mLocalAddressResolver;
	}

	void CodeGenerator::Dump(std::ofstream& output)
	{
		const auto codeBuffer = mCodeBuffer->Copy();
		if (codeBuffer.get())
		{
			size_t pc = 0;
			while (pc < mCodeBuffer->GetSize())
			{
				mSymbolFactory->DumpFunctionNameFromAddress(output, pc);
				output << mana_get_instruction_text(mDataBuffer->GetBuffer(), codeBuffer.get(), pc);
				output << "\n";
				
				output.flush();

				pc += GetInstructionSize(codeBuffer.get(), pc);
			}
		}
	}
}
