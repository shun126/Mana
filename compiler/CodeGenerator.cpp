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
	void CodeGenerator::ResolveLoad(const std::shared_ptr<SyntaxNode>& node) const
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
			mCodeBuffer->AddOpecode(IntermediateLanguage::LoadChar);
			break;

		case TypeDescriptor::Id::Short:
			mCodeBuffer->AddOpecode(IntermediateLanguage::LoadShort);
			break;

		case TypeDescriptor::Id::Bool:
			mCodeBuffer->AddOpecode(IntermediateLanguage::LoadChar);
			break;

		case TypeDescriptor::Id::Int:
			mCodeBuffer->AddOpecode(IntermediateLanguage::LoadInteger);
			break;

		case TypeDescriptor::Id::Float:
			mCodeBuffer->AddOpecode(IntermediateLanguage::LoadFloat);
			break;

		case TypeDescriptor::Id::Reference:
			mCodeBuffer->AddOpecode(IntermediateLanguage::LoadReference);
			break;

		case TypeDescriptor::Id::Actor:
			if (node->GetTypeDescriptor() == mTypeDescriptorFactory->Get(TypeDescriptor::Id::Actor))
				mCodeBuffer->AddOpecode(IntermediateLanguage::LoadReference);
			else
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::PushActor, mDataBuffer->Set(node->GetTypeDescriptor()->GetName()));
			break;

		case TypeDescriptor::Id::Array:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);

		case TypeDescriptor::Id::Struct:
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::LoadData, node->GetTypeDescriptor()->GetMemorySize());
			break;

		default:
			MANA_BUG("illigal data GetTypeDescriptor()");
		}
	}

	/*!
	スタックにあるアドレスへデータを書き込む命令を出力する
	*/
	void CodeGenerator::ResolveStore(const std::shared_ptr<SyntaxNode>& node) const
	{
		MANA_ASSERT(node);
		MANA_ASSERT(node->GetTypeDescriptor());

		switch (node->GetTypeDescriptor()->GetId())
		{
		case TypeDescriptor::Id::Char:
			mCodeBuffer->AddOpecode(IntermediateLanguage::StoreChar);
			break;

		case TypeDescriptor::Id::Short:
			mCodeBuffer->AddOpecode(IntermediateLanguage::StoreShort);
			break;

		case TypeDescriptor::Id::Bool:
			mCodeBuffer->AddOpecode(IntermediateLanguage::StoreChar);
			break;

		case TypeDescriptor::Id::Int:
			mCodeBuffer->AddOpecode(IntermediateLanguage::StoreInteger);
			break;

		case TypeDescriptor::Id::Float:
			mCodeBuffer->AddOpecode(IntermediateLanguage::StoreFloat);
			break;

		case TypeDescriptor::Id::Reference:
		case TypeDescriptor::Id::Actor:
			mCodeBuffer->AddOpecode(IntermediateLanguage::StoreReference);
			break;

		case TypeDescriptor::Id::Array:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			[[fallthrough]];

		case TypeDescriptor::Id::Struct:
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::StoreData, node->GetTypeDescriptor()->GetMemorySize());
			break;

		default:
			CompileError("illegal type stored");
			break;
		}
	}

	/*!
	returnの処理
	@param	function	returnが所属する関数のSymbol
	@param	tree		return文のSyntaxNode
	*/
	void CodeGenerator::Return(const std::shared_ptr<Symbol>& function, const std::shared_ptr<SyntaxNode>& tree)
	{
		// Error check
		std::shared_ptr<TypeDescriptor> type = function->GetTypeDescriptor();
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
			const int32_t inDepth = mSymbolFactory->OpenBlock(false);
			GenerateCode(tree->GetLeftNode(), true);
			const int32_t out_depth = mSymbolFactory->CloseBlock();
			MANA_VERIFY_MESSAGE(
				inDepth == out_depth,
				Concat({ "ブロックの深さが一致しません in:", std::to_string(inDepth), " out:", std::to_string(out_depth) })
			);
		}

		// 関数の最後にジャンプ
		// TODO:SymbolTable側でコード生成できるかも
		mSymbolFactory->SetReturnAddressList(
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, mSymbolFactory->GetReturnAddressList())
		);

		// 関数を使用したフラグを立てる
		function->SetUsed(true);
	}

	/*!
	rollbackの処理
	@param	tree	rollback文のSyntaxNode
	*/
	void CodeGenerator::Rollback(const std::shared_ptr<SyntaxNode>& tree)
	{
		if (tree)
		{
			const int32_t inDepth = mSymbolFactory->OpenBlock(false);
			GenerateCode(tree, true);
			const int32_t out_depth = mSymbolFactory->CloseBlock();
			MANA_VERIFY_MESSAGE(
				inDepth == out_depth,
				Concat({ "ブロックの深さが一致しません in:", std::to_string(inDepth), " out:", std::to_string(out_depth) })
			);
		}
		mCodeBuffer->AddOpecode(IntermediateLanguage::Rollback);
	}

	/*!
	関数呼び出し時の引数を出力（再帰呼び出し）
	@param	count	引数の番号
	@param	param	引数のSymbol
	@param	arg		引数のSyntaxNode
	@return	引数の数
	*/
	int32_t CodeGenerator::Argument(int32_t count, const std::shared_ptr<Symbol>& param, std::shared_ptr<SyntaxNode> arg)
	{
		if (param && arg)
		{
			if (arg->Is(SyntaxNode::Id::CallArgument))
			{
				count = Argument(count, param->GetNext(), arg->GetRightNode());

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
			GenerateCode(arg->Is(SyntaxNode::Id::CallArgument) ? arg->GetLeftNode() : arg, true);
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
	int32_t CodeGenerator::CallArgumentSize(int32_t address, const std::shared_ptr<Symbol>& param, std::shared_ptr<SyntaxNode> arg)
	{
		if (param && arg)
		{
			address += CallArgumentSize(address, param->GetNext(), arg->GetRightNode());

			if (arg->Is(SyntaxNode::Id::CallArgument))
				arg = arg->GetLeftNode();

			address += Alignment(arg->GetMemorySize(), ToAddress(sizeof(int32_t)));
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
	int32_t CodeGenerator::CallArgument(int32_t address, const std::shared_ptr<Symbol>& param, std::shared_ptr<SyntaxNode> arg)
	{
		if (param && arg)
		{
			address = CallArgument(address, param->GetNext(), arg->GetRightNode());
			if (address < std::numeric_limits<int16_t>::min() || std::numeric_limits<int16_t>::max() < address)
				throw std::range_error("call argument size");
			mCodeBuffer->Add<int16_t>(static_cast<int16_t>(address));

			if (arg->Is(SyntaxNode::Id::CallArgument))
				arg = arg->GetLeftNode();

			address -= Alignment(arg->GetMemorySize(), ToAddress(sizeof(int32_t)));
		}
		return address;
	}

	/*!
	* 関数呼び出しのノードを評価します
	* @param	node	関数呼び出しのSyntaxNode
	*/
	void CodeGenerator::Call(const std::shared_ptr<SyntaxNode>& node)
	{
		std::shared_ptr<SyntaxNode> argument = node->GetRightNode();
		int32_t argument_counter = Argument(0, (node->GetSymbol())->GetParameterList(), argument);

		// エラーチェック
		if (argument_counter < 0 || std::numeric_limits<uint8_t>::max() < argument_counter)
		{
			// 256以上の引数は指定できません
			CompileError("Cannot specify more than 256 arguments.");
		}
		else if ((node->GetSymbol())->GetNumberOfParameters() != argument_counter)
		{
			// 引数の数が一致しない
			CompileError("unmatched argument.");
		}
		else if ((node->GetSymbol())->GetClassTypeId() == Symbol::ClassTypeId::NativeFunction)
		{
			// for external function
			int32_t argument_size = CallArgumentSize(0, (node->GetSymbol())->GetParameterList(), argument);

			node->GetSymbol()->SetAddress(mDataBuffer->Set(node->GetSymbol()->GetName()));

			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Call, (node->GetSymbol())->GetAddress());
			mCodeBuffer->Add<uint8_t>(((node->GetSymbol())->GetTypeDescriptor()->GetId() != TypeDescriptor::Id::Void));
			mCodeBuffer->Add<uint8_t>(static_cast<uint8_t>(argument_counter));
			mCodeBuffer->Add<uint16_t>(static_cast<uint16_t>(argument_size));
			CallArgument(argument_size - 1, (node->GetSymbol())->GetParameterList(), argument);
		}
		else
		{
			// for internal function
			const int32_t address = mCodeBuffer->AddOpecodeAndOperand(
				IntermediateLanguage::BranchSubRoutine,
				node->GetSymbol()->GetAddress()
			);
			mGlobalAddressResolver->AddCallList(
				address + 1,
				node->GetSymbol()
			);
		}
	}

	int32_t CodeGenerator::CallPrintGenerateArgument(int32_t argc, const std::shared_ptr<SyntaxNode>& node)
	{
		if (node)
		{
			argc = CallPrintGenerateArgument(argc, node->GetRightNode());

			GenerateCode(node->Is(SyntaxNode::Id::CallArgument) ? node->GetLeftNode() : node, true);

			++argc;
		}
		return argc;
	}

	/*!
	* print文の処理
	* @param[in]	node	print文のSyntaxNode
	*/
	void CodeGenerator::CallPrint(const std::shared_ptr<SyntaxNode>& node)
	{
		mCodeBuffer->AddOpecodeAndOperand(
			IntermediateLanguage::Print,
			CallPrintGenerateArgument(0, node)
		);
	}

	/*!
	* 判別式内に代入文があるか調べます
	* @param	tree	評価式のSyntaxNode
	*/
	void CodeGenerator::ConditionCheck(const std::shared_ptr<SyntaxNode>& tree)
	{
		if (tree)
		{
			if (tree->Is(SyntaxNode::Id::Assign))
			{
				// 条件判定に代入文は使用できない
				CompileError("can't assign expression in condition");
			}

			ConditionCheck(tree->GetLeftNode());
			ConditionCheck(tree->GetRightNode());
		}
	}

	/*!
	* 判別式の評価
	* @param	tree	評価式のSyntaxNode
	* @return	現在のプログラムアドレス
	*/
	address_t CodeGenerator::ConditionCore(const std::shared_ptr<SyntaxNode>& tree)
	{
		// 判別式内に代入式があるか調べます
		ConditionCheck(tree);

		if (tree)
		{
			if (
				tree->GetTypeDescriptor()->Is(TypeDescriptor::Id::Void) ||
				tree->GetTypeDescriptor()->GetId() > TypeDescriptor::Id::Reference)
			{
				CompileError("illegal type of expression in condition");
			}
			GenerateCode(tree, true);
		}
		return mCodeBuffer->GetSize() - 5;
	}

	/*!
	判別式の評価
	@param	tree	評価式のSyntaxNode
	@return	現在のプログラムアドレス
	*/
	address_t CodeGenerator::Condition(const std::shared_ptr<SyntaxNode>& tree, const bool match)
	{
		//generator_resolve_symbol(tree);
		//generator_automatic_cast(tree);

		/* 判別式の評価 */
		const int32_t inDepth = mSymbolFactory->OpenBlock(false);
		ConditionCore(tree);
		const int32_t out_depth = mSymbolFactory->CloseBlock();
		MANA_VERIFY_MESSAGE(
			inDepth == out_depth,
			Concat({ "ブロックの深さが一致しません in:", std::to_string(inDepth), " out:", std::to_string(out_depth) })
		);

		return mCodeBuffer->AddOpecodeAndOperand(match ? IntermediateLanguage::BranchEqual : IntermediateLanguage::BranchNotEqual);
	}

	void CodeGenerator::GenerateConstantInteger(const TypeDescriptor::Id typeId, const int32_t value) const
	{
		switch (typeId)
		{
		case TypeDescriptor::Id::Char:
			if (value == 0)
			{
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushZeroInteger);
			}
			else {
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushChar);
				mCodeBuffer->Add((int8_t)value);
			}
			break;

		case TypeDescriptor::Id::Short:
			if (value == 0)
			{
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushZeroInteger);
			}
			else {
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushShort);
				mCodeBuffer->Add((int16_t)value);
			}
			break;

		case TypeDescriptor::Id::Bool:
			if (value == 0)
			{
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushZeroInteger);
			}
			else {
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushChar);
				mCodeBuffer->Add(static_cast<int8_t>(value));
			}
			break;

		case TypeDescriptor::Id::Int:
			if (value == 0)
			{
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushZeroInteger);
			}
			else {
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushInteger);
				mCodeBuffer->Add(value);
			}
			break;

		default:
			//CompileError("illegal type of expression in statement");
			MANA_BUG("illegal type of expression");
		}
	}

	void CodeGenerator::GenerateConstantFloat(const TypeDescriptor::Id typeId, const float value) const
	{
		switch (typeId)
		{
		case TypeDescriptor::Id::Float:
			if (value == 0.0f)
			{
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushZeroFloat);
			}
			else
			{
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushFloat);
				mCodeBuffer->Add(value);
			}
			break;

		default:
			//CompileError("illegal type of expression in statement");
			MANA_BUG("illegal type of expression");
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	/*!
	式の評価
	@param	tree			式のSyntaxNode
	@param	enableAssign	trueならば代入式、falseならばそれ以外
	*/
	void CodeGenerator::Expression(const std::shared_ptr<SyntaxNode>& tree, const int32_t enableAssign)
	{
		if (tree == nullptr)
			return;

		//generator_resolve_symbol(tree);

		const int32_t inDepth = mSymbolFactory->OpenBlock(false);

		if (enableAssign)
		{
			if (tree->IsNot(SyntaxNode::Id::Assign) && tree->IsNot(SyntaxNode::Id::Call) && tree->GetId() != SyntaxNode::Id::FloatToInteger)
				CompileError("illegal expression in write-statement");
		}
		else {
			if (tree->Is(SyntaxNode::Id::Assign))
				CompileError("illegal expression in write-statement");
		}

		GenerateCode(tree, true);

		if (enableAssign && tree->Is(SyntaxNode::Id::Call) && tree->GetTypeDescriptor())
		{
			switch (tree->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Void:
				break;

			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Bool:
			case TypeDescriptor::Id::Int:
			case TypeDescriptor::Id::Float:
			case TypeDescriptor::Id::Actor:
				mCodeBuffer->AddOpecode(IntermediateLanguage::Remove);
				break;

			case TypeDescriptor::Id::Struct:
				mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::RemoveData, tree->GetTypeDescriptor()->GetMemorySize());
				break;

			default:
				MANA_BUG("illegal type of expression");
				break;
			}
		}

		const int32_t out_depth = mSymbolFactory->CloseBlock();
		MANA_VERIFY_MESSAGE(
			inDepth == out_depth,
			Concat({ "ブロックの深さが一致しません in:", std::to_string(inDepth), " out:", std::to_string(out_depth) })
		);
	}


	////////////////////////////////////////////////////////////////////////////////
	void CodeGenerator::GenerateCode(std::shared_ptr<SyntaxNode> node, const int32_t enableLoad)
	{
		if (node == nullptr)
			return;

DO_RECURSIVE:
		mGlobalSemanticAnalyzer->SetCurrentFileInformation(node);

		switch (node->GetId())
		{
			///////////////////////////////////////////////////////////////////////
			// 定数定義に関するノード									
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
			mActorSymbolEntryPointer = mSymbolFactory->Lookup(node->GetString());
			mSymbolFactory->OpenActor(node->GetString());
			GenerateCode(node->GetLeftNode(), enableLoad);
			mSymbolFactory->CloseActor();
			mActorSymbolEntryPointer = nullptr;
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
			mActorSymbolEntryPointer = mSymbolFactory->Lookup(node->GetString());
			mSymbolFactory->OpenModule(mActorSymbolEntryPointer);
			GenerateCode(node->GetLeftNode(), enableLoad);
			mSymbolFactory->CloseModule(node->GetString());
			mActorSymbolEntryPointer = nullptr;
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Phantom:
		{
			mActorSymbolEntryPointer = mSymbolFactory->Lookup(node->GetString());
			mSymbolFactory->OpenActor(node->GetString());
			GenerateCode(node->GetLeftNode(), enableLoad);
			mSymbolFactory->CloseActor();
			mActorSymbolEntryPointer = nullptr;
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Struct:
			mSymbolFactory->OpenStructure(node->GetString());
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mLocalSemanticAnalyzer->PostResolverResolve(node);
			mSymbolFactory->CloseStructure();
			break;

			///////////////////////////////////////////////////////////////////////
			// 関数宣言に関するノード									
		case SyntaxNode::Id::Action:
			{
				node->Set(mTypeDescriptorFactory->Get(TypeDescriptor::Id::Void));
				mFunctionSymbolEntryPointer = mSymbolFactory->Lookup(node->GetString());
				{
					mSymbolFactory->OpenFunction(node, true);
					GenerateCode(node->GetLeftNode(), enableLoad);
					mSymbolFactory->CloseFunction(node, true);
				}
				mFunctionSymbolEntryPointer = nullptr;
			}
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareArgument:
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			MANA_BUG("illegal type of expression");

		case SyntaxNode::Id::DeclareFunction:
		{
			MANA_ASSERT(node->GetSymbol());
			// 関数の戻り値を評価
			GenerateCode(node->GetLeftNode(), enableLoad);
			// シンボルの検索と型の定義
			mFunctionSymbolEntryPointer = mSymbolFactory->Lookup(node->GetString());
			node->Set(mFunctionSymbolEntryPointer);

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

				GenerateCode(node->GetBodyNode(), enableLoad);
				mSymbolFactory->CloseFunction(node, false);
			}
			mSymbolFactory->CloseBlock();

			mFunctionSymbolEntryPointer = nullptr;
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
			//symbol_allocate_memory(node->GetRightNode()->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor(), Normal);
			// node->GetLeftNode()
			// node->GetRightNode()
			if (node->GetBodyNode())
			{
				const std::shared_ptr<Symbol> symbol = node->GetRightNode()->GetSymbol();
				if (symbol && symbol->GetClassTypeId() == Symbol::ClassTypeId::LocalVariable)
				{
					// initializer
					GenerateCode(node->GetBodyNode(), enableLoad);
				}
			}
			//resolver_resolve_variable_description(node, Normal);
			/*
			GenerateCode(node->GetLeftNode(), enableLoad); // SyntaxNode::Id::TypeDescription
			GenerateCode(node->GetRightNode(), enableLoad);// SyntaxNode::Id::Declarator
			if(node->GetRightNode()->GetSymbol()->class_type == LocalVariable)
			symbol_allocate_memory(node->GetRightNode()->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor(), Normal);
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
			GenerateCode(node->GetLeftNode(), enableLoad);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// ブロックを伴う制御に関するノード
		case SyntaxNode::Id::Block:
		{
			const int32_t inDepth = mSymbolFactory->OpenBlock(false);

			mLocalSemanticAnalyzer->PostResolverResolve(node->GetLeftNode());
			GenerateCode(node->GetLeftNode(), enableLoad);

			mLocalSemanticAnalyzer->PostResolverResolve(node->GetRightNode());
			GenerateCode(node->GetRightNode(), enableLoad);

			const int32_t out_depth = mSymbolFactory->CloseBlock();
			MANA_VERIFY_MESSAGE(
				inDepth == out_depth,
				Concat({ "ブロックの深さが一致しません in:", std::to_string(inDepth), " out:", std::to_string(out_depth) })
			);
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Break:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, mLocalAddressResolver->Break(mCodeBuffer->GetSize()));
			break;

		case SyntaxNode::Id::Case:
			mLocalAddressResolver->RegistSwitchCase(node->GetLeftNode());
			if (node->GetLeftNode() && node->GetLeftNode()->GetTypeDescriptor())
				node->Set(node->GetLeftNode()->GetTypeDescriptor());
			GenerateCode(node->GetRightNode(), enableLoad);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Continue:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, mLocalAddressResolver->Continue(mCodeBuffer->GetSize()));
			break;

		case SyntaxNode::Id::Default:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			mLocalAddressResolver->RegistSwitchDefault();
			break;

		case SyntaxNode::Id::Do:
		{
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::Do);
			const address_t address = mCodeBuffer->GetSize();
			GenerateCode(node->GetLeftNode(), enableLoad);
			mLocalAddressResolver->CloseContinueOnly();

			mCodeBuffer->ReplaceAddressAll(Condition(node->GetRightNode(), false), address);
			mLocalAddressResolver->CloseChain();
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::For:
			/* 'for(GetTypeDescriptor() variable = expression' の形式 */
		{
			//symbol_allocate_memory($2, $1, Normal);
			//Expression(node_create_node(SyntaxNode::Id::NODE_TYPE_ASSIGN, node_create_leaf($2->name), $4), true);
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::For);
			//$$ = GetSize();

			//GenerateCode(node->GetLeftNode(), enableLoad);
			mLocalAddressResolver->Break(Condition(node->GetLeftNode(), true));

			GenerateCode(node->GetBodyNode(), enableLoad);

			mLocalAddressResolver->CloseContinueOnly();
			Expression(node->GetRightNode(), true);
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, mLocalAddressResolver->Continue(mCodeBuffer->GetSize()));
			mLocalAddressResolver->CloseChain();
		}
		break;

		case SyntaxNode::Id::Goto:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				std::shared_ptr<Symbol> symbol = mSymbolFactory->CreateLabel(node->GetString());
				symbol->SetEtc(mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, symbol->GetEtc()));
			}
			break;

		case SyntaxNode::Id::Halt:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(IntermediateLanguage::Halt);
			break;

		case SyntaxNode::Id::If:
		{
			address_t address = Condition(node->GetBodyNode(), true);
			GenerateCode(node->GetLeftNode(), enableLoad);
			if (node->GetRightNode())
			{
				// else block
				const address_t else_begin_address = mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch);
				mCodeBuffer->ReplaceAddressAll(address, mCodeBuffer->GetSize());
				GenerateCode(node->GetRightNode(), enableLoad);
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
			mCodeBuffer->AddOpecode(IntermediateLanguage::NonPreEmptive);
			GenerateCode(node->GetLeftNode(), enableLoad);
			mLocalAddressResolver->CloseChain();
			mCodeBuffer->AddOpecode(IntermediateLanguage::PreEmptive);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Loop:
		{
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::Loop);
			const int32_t address = mCodeBuffer->GetSize();
			GenerateCode(node->GetLeftNode(), enableLoad);
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, address);
			mLocalAddressResolver->CloseChain();
		}
		MANA_ASSERT(node->GetRightNode() == nullptr);
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Return:
			if (node->GetLeftNode())
				node->Set(node->GetLeftNode()->GetTypeDescriptor());
			Return(mFunctionSymbolEntryPointer, node);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Rollback:
			GenerateCode(node->GetLeftNode(), enableLoad);
			if (node->GetLeftNode())
				node->Set(node->GetLeftNode()->GetTypeDescriptor());
			Rollback(node);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Switch:
		{
			Expression(node->GetLeftNode(), false);
			const int32_t address = mCodeBuffer->GetSize();
			mLocalAddressResolver->OpenChain(LocalAddressResolver::JumpChainStatus::Switch);
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch);
			mLocalAddressResolver->OpenSwitchBlock(node->GetLeftNode()->GetTypeDescriptor());

			GenerateCode(node->GetRightNode(), enableLoad);

			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, mLocalAddressResolver->Break(mCodeBuffer->GetSize()));
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
			mLocalAddressResolver->Break(Condition(node->GetLeftNode(), true));
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch, mLocalAddressResolver->Continue(mCodeBuffer->GetSize()));
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
			mCodeBuffer->AddOpecode(IntermediateLanguage::Comply);
			break;

		case SyntaxNode::Id::Join:
			//generator_resolve_symbol(node);
			mSymbolFactory->AddJoin(shared_from_this(), node->GetLeftNode(), node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Print:
			CallPrint(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Reject:
			mCodeBuffer->AddOpecode(IntermediateLanguage::Refuse);
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Request:
			mSymbolFactory->AddRequest(shared_from_this(), IntermediateLanguage::Request, node->GetLeftNode(), node->GetRightNode(), node->GetString());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::AwaitStart:
			mSymbolFactory->AddRequest(shared_from_this(), IntermediateLanguage::RequestWaitStarting, node->GetLeftNode(), node->GetRightNode(), node->GetString());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::AwaitCompletion:
			mSymbolFactory->AddRequest(shared_from_this(), IntermediateLanguage::RequestWaitEnded, node->GetLeftNode(), node->GetRightNode(), node->GetString());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Yield:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(IntermediateLanguage::Yield);
			break;

			///////////////////////////////////////////////////////////////////////
			// 二項演算子に関するノード
		case SyntaxNode::Id::Add:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? IntermediateLanguage::AddFloat : IntermediateLanguage::AddInteger);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Sub:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? IntermediateLanguage::SubtractFloat : IntermediateLanguage::SubtractInteger);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Mul:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? IntermediateLanguage::MultiFloat : IntermediateLanguage::MultiInteger);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Div:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? IntermediateLanguage::DivideFloat : IntermediateLanguage::DivideInteger);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Rem:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? IntermediateLanguage::ModFloat : IntermediateLanguage::ModInteger);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Pow:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode((node->GetLeftNode()->GetTypeDescriptor())->Is(TypeDescriptor::Id::Float) ? IntermediateLanguage::SubtractFloat : IntermediateLanguage::SubtractInteger);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::And:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode(IntermediateLanguage::And);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Or:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode(IntermediateLanguage::Or);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Xor:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode(IntermediateLanguage::ExclusiveOr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::LeftShift:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode(IntermediateLanguage::ShiftLeft);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::RightShift:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->AddOpecode(IntermediateLanguage::ShiftRight);
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
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(node->GetOpecode());
			break;

			///////////////////////////////////////////////////////////////////////
			// 単項演算子に関するノード
		case SyntaxNode::Id::LogicalNot:
		case SyntaxNode::Id::Not:
			//generator_automatic_cast(node);
			GenerateCode(node->GetLeftNode(), enableLoad);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(node->GetOpecode());
			break;

		case SyntaxNode::Id::Neg:
			GenerateCode(node->GetLeftNode(), enableLoad);
			mCodeBuffer->AddOpecode(node->GetLeftNode()->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::Float ? IntermediateLanguage::MinusFloat : IntermediateLanguage::MinusInteger);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 演算に関するノード
		case SyntaxNode::Id::Array:
			/* variable[index] */
			GenerateCode(node->GetRightNode(), true);
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::PushSize, (node->GetTypeDescriptor())->GetMemorySize());
			mCodeBuffer->AddOpecode(IntermediateLanguage::MultiInteger);
			GenerateCode(node->GetLeftNode(), false);
			mCodeBuffer->AddOpecode(IntermediateLanguage::AddInteger);
			if (enableLoad)
			{
				ResolveLoad(node);
			}
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Assign:
			//generator_resolve_symbol(node->GetLeftNode());
			//generator_resolve_symbol(node->GetRightNode());

			//generator_automatic_cast(node);
			GenerateCode(node->GetRightNode(), enableLoad);
			GenerateCode(node->GetLeftNode(), false);
			ResolveStore(node->GetLeftNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Call:
			// 関数、アクションを呼びます
			//generator_resolve_symbol(node);
			// resolver_search_symbol_from_name(node)
			if (node->GetSymbol())
			{
				//GenerateCode(node->GetRightNode(), enableLoad);
				Call(node);
			}
			break;

		case SyntaxNode::Id::CallArgument:
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
			break;

		case SyntaxNode::Id::Const:
			switch (node->GetTypeDescriptor()->GetId())
			{
			case TypeDescriptor::Id::Char:
			case TypeDescriptor::Id::Short:
			case TypeDescriptor::Id::Bool:
			case TypeDescriptor::Id::Int:
				GenerateConstantInteger(node->GetTypeDescriptor()->GetId(), node->GetInt());
				break;

			case TypeDescriptor::Id::Float:
				GenerateConstantFloat(node->GetTypeDescriptor()->GetId(), node->GetFloat());
				break;

			case TypeDescriptor::Id::Nil:
				mCodeBuffer->AddOpecode(IntermediateLanguage::PushZeroInteger);
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
			ConditionCore(node->GetNextNode());
			pc1 = mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::BranchEqual);
			GenerateCode(node->GetLeftNode(), enableLoad);
			pc2 = mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::Branch);
			mCodeBuffer->ReplaceAddressAll(pc1, mCodeBuffer->GetSize());
			GenerateCode(node->GetRightNode(), enableLoad);
			mCodeBuffer->ReplaceAddressAll(pc2, mCodeBuffer->GetSize());
		}
		break;

		case SyntaxNode::Id::IntegerToFloat:
			GenerateCode(node->GetLeftNode(), enableLoad);
			mCodeBuffer->AddOpecode(IntermediateLanguage::CastIntegerToFloat);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::FloatToInteger:
			GenerateCode(node->GetLeftNode(), enableLoad);
			mCodeBuffer->AddOpecode(IntermediateLanguage::CastFloatToInteger);
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
				case Symbol::ClassTypeId::ConstantInteger:
					GenerateConstantInteger(node->GetSymbol()->GetTypeDescriptor()->GetId(), node->GetSymbol()->GetEtc());
					break;

				case Symbol::ClassTypeId::ConstantFloat:
					GenerateConstantFloat(node->GetSymbol()->GetTypeDescriptor()->GetId(), node->GetSymbol()->GetFloat());
					break;

				case Symbol::ClassTypeId::ConstantString:
					GenerateConstantInteger(node->GetSymbol()->GetTypeDescriptor()->GetId(), node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::StaticVariable:
					mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::LoadStaticAddress, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::GlobalVariable:
					mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::LoadGlobalAddress, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::ActorVariable:
					mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::LoadSelfAddress, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::LocalVariable:
					mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::LoadFrameAddress, node->GetSymbol()->GetAddress());
					break;

				case Symbol::ClassTypeId::Type:
					break;

				case Symbol::ClassTypeId::NewSymbol:

				default:
					CompileError("illigal data GetTypeDescriptor()");
					break;
				}

				if (enableLoad)
					ResolveLoad(node);
			}
			break;

		case SyntaxNode::Id::MemberFunction:
			// TODO:実装してください
			GenerateCode(node->GetLeftNode(), enableLoad);
			GenerateCode(node->GetRightNode(), enableLoad);
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
								if (symbol->GetName() == node->GetString() && symbol->GetClassTypeId() == Symbol::ClassTypeId::ActorVariable)
								{
									// variable.member
									mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::PushSize, symbol->GetAddress());
									GenerateCode(node->GetLeftNode(), false);
									mCodeBuffer->AddOpecode(IntermediateLanguage::AddInteger);
									if (enableLoad)
										ResolveLoad(node);
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
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::PushSize, node->etc);
			GenerateCode(node->GetLeftNode(), false);
			mCodeBuffer->AddOpecode(IntermediateLanguage::AddInteger);
			if (enableLoad)
			{
				ResolveLoad(node);
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
			mCodeBuffer->AddOpecode(IntermediateLanguage::PushSender);
			break;

		case SyntaxNode::Id::Self:
			// nodeをpushします
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(IntermediateLanguage::PushSelf);
			break;

		case SyntaxNode::Id::Priority:
			// priorityをpushします
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			mCodeBuffer->AddOpecode(IntermediateLanguage::PushPriority);
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
			mCodeBuffer->AddOpecodeAndOperand(IntermediateLanguage::PushString, node->GetInt());
			break;
#if 0
		case SyntaxNode::Id::NODE_VARIABLE:
			/* variable */
			GenerateCode(node->GetRightNode(), enableLoad);
			GenerateCode(node->GetLeftNode(), enableLoad);

			//generator_generate_variable_value(node);
			switch ((node->GetSymbol())->class_type)
			{
			case StaticVariable:
				AddOpecodeAndOperand(IntermediateLanguage::LoadStaticAddress, (node->GetSymbol())->address);
				break;

			case GlobalVariable:
				AddOpecodeAndOperand(IntermediateLanguage::LoadGlobalAddress, (node->GetSymbol())->address);
				break;

			case ActorVariable:
				AddOpecodeAndOperand(IntermediateLanguage::LoadSelfAddress, (node->GetSymbol())->address);
				break;

			case LocalVariable:
				AddOpecodeAndOperand(IntermediateLanguage::LoadFrameAddress, (node->GetSymbol())->address);
				break;

			case Type:
				break;

			default:
				CompileError(node, "illegal type of expression in statement");
				break;
			}

			if (enableLoad)
			{
				ResolveLoad(node);
			}

			break;
#endif
		default:
			MANA_BUG("illegal right-hand side value");
		}

		// 子ノードから型を継承する
		//resolver_resolve_type_from_child_node(node);

		if (node->GetNextNode())
		{
			// 末尾再帰なのでgotoにて処理する GenerateCode(node->GetNextNode());
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

	void CodeGenerator::Dump(std::ofstream& output) const
	{
		const auto codeBuffer = mCodeBuffer->Copy();
		if (codeBuffer.get())
		{
			address_t pc = 0;
			while (pc < mCodeBuffer->GetSize())
			{
				mSymbolFactory->DumpFunctionNameFromAddress(output, pc);
				output << GetInstructionText(mDataBuffer->GetBuffer(), codeBuffer.get(), pc);
				output << "\n";
				
				output.flush();

				pc += GetInstructionSize(codeBuffer.get(), pc);
			}
		}
	}
}
