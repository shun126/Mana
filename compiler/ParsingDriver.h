/*!
mana (compiler)

@author	Shun Moriya
@date	2022-
*/

#pragma once
#include "../runner/common/Noncopyable.h"
#include "Parser.hpp"
#include <memory>
#include <string_view>

namespace mana
{
	class CodeBuffer;
	class CodeGenerator;
	class DataBuffer;
	class GlobalSemanticAnalyzer;
	class LocalSemanticAnalyzer;
	class SymbolFactory;
	class StringPool;
	class TypeDescriptorFactory;

	class ParsingDriver final : private Noncopyable, public std::enable_shared_from_this<ParsingDriver>
	{
	public:
		ParsingDriver();
		~ParsingDriver() = default;

		int32_t Parse();

		const std::shared_ptr<CodeBuffer>& GetCodeBuffer();
		const std::shared_ptr<CodeGenerator>& GetCodeGenerator();
		const std::shared_ptr<DataBuffer>& GetDataBuffer();
		const std::shared_ptr<GlobalSemanticAnalyzer>& GetGlobalSemanticAnalyzer();
		const std::shared_ptr<SymbolFactory>& GetSymbolFactory();
		const std::shared_ptr<StringPool>& GetStringPool();
		const std::shared_ptr<TypeDescriptorFactory>& GetTypeDescriptorFactory();

	protected:
		/*
		TODO: SyntaxNodeFactory class ?
		*/

		std::shared_ptr<SyntaxNode> Bind(std::shared_ptr<SyntaxNode> base, std::shared_ptr<SyntaxNode> next);

		// declarations
		std::shared_ptr<SyntaxNode> CreateNativeFunction(std::shared_ptr<SyntaxNode> returnExpression, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument);
		std::shared_ptr<SyntaxNode> CreateDeclareMemory(const int_t size, std::shared_ptr<SyntaxNode> allocateDeclarations);
		std::shared_ptr<SyntaxNode> CreateDeclareStaticMemory(const int_t size, std::shared_ptr<SyntaxNode> allocateDeclarations);
		std::shared_ptr<SyntaxNode> CreateAlias(const std::string_view identifier, const std::string_view filename);	// Not yet

		// declaration
		std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const int_t value);
		std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const float_t value);
		std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const std::string_view text);
		std::shared_ptr<SyntaxNode> CreateDefineNode(const std::string_view identifier, const std::string_view text);
		std::shared_ptr<SyntaxNode> CreateUndefineNode(const std::string_view identifier);

		// actor
		std::shared_ptr<SyntaxNode> CreateActor(const std::string_view identifier, std::shared_ptr<SyntaxNode> action);
		std::shared_ptr<SyntaxNode> CreatePhantom(const std::string_view identifier, std::shared_ptr<SyntaxNode> action);
		std::shared_ptr<SyntaxNode> CreateModule(const std::string_view identifier, std::shared_ptr<SyntaxNode> action);

		// action
		std::shared_ptr<SyntaxNode> CreateAction(const std::string_view identifier, std::shared_ptr<SyntaxNode> block);
		std::shared_ptr<SyntaxNode> CreateExtend(const std::string_view identifier);

		// struct
		std::shared_ptr<SyntaxNode> CreateStruct(const std::string_view identifier, std::shared_ptr<SyntaxNode> member);

		// function
		std::shared_ptr<SyntaxNode> CreateInternalFunction(std::shared_ptr<SyntaxNode> returnExpression, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument, std::shared_ptr<SyntaxNode> statement);

		// variable_type
		std::shared_ptr<SyntaxNode> CreateActorTypeDescription();
		std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::string_view identifier);
		std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescriptor);

		// block
		std::shared_ptr<SyntaxNode> CreateBlock(std::shared_ptr<SyntaxNode> statement);

		// statement	
		std::shared_ptr<SyntaxNode> CreateIf(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statementIfTrue, std::shared_ptr<SyntaxNode> statementIfFalse);
		std::shared_ptr<SyntaxNode> CreateSwitch(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statement);
		std::shared_ptr<SyntaxNode> CreateWhile(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statement);
		std::shared_ptr<SyntaxNode> CreateDoWhile(std::shared_ptr<SyntaxNode> statement, std::shared_ptr<SyntaxNode> condition);
		std::shared_ptr<SyntaxNode> CreateFor(std::shared_ptr<SyntaxNode> initializeStatement, std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> iterationExpression, std::shared_ptr<SyntaxNode> statement);
		std::shared_ptr<SyntaxNode> CreateFor(std::shared_ptr<SyntaxNode> declareVariable, std::shared_ptr<SyntaxNode> initializeStatement, std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> iterationExpression, std::shared_ptr<SyntaxNode> statement);
		std::shared_ptr<SyntaxNode> CreateLoop(std::shared_ptr<SyntaxNode> statement);
		std::shared_ptr<SyntaxNode> CreateLock(std::shared_ptr<SyntaxNode> statement);
		std::shared_ptr<SyntaxNode> CreateGoto(const std::string_view labelName);
		std::shared_ptr<SyntaxNode> CreateLabel(const std::string_view identifier);
		std::shared_ptr<SyntaxNode> CreateReturn(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateRollback(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateBreak();
		std::shared_ptr<SyntaxNode> CreateContinue();
		std::shared_ptr<SyntaxNode> CreateHalt();
		std::shared_ptr<SyntaxNode> CreateYield();
		std::shared_ptr<SyntaxNode> CreateComply();	// consider the name
		std::shared_ptr<SyntaxNode> CreateRefuse();	// consider the name
		std::shared_ptr<SyntaxNode> CreatePrint(std::shared_ptr<SyntaxNode> argument);
		std::shared_ptr<SyntaxNode> CreateRequest(std::shared_ptr<SyntaxNode> priority, std::shared_ptr<SyntaxNode> expression, const std::string_view actionName);
		std::shared_ptr<SyntaxNode> CreateJoin(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);

		// expression
		std::shared_ptr<SyntaxNode> CreateAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateLogicalAnd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateLogicalOr(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateAdd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateSub(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateMul(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateDiv(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateMod(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreatePow(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateAnd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateOr(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateXor(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateLeftShift(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateRightShift(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateGT(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateGE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateLS(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateLE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateEQ(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateNE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateAddAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateSubAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateMulAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateDivAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateModAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreatePowAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateAndAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateOrAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateXorAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateLeftShiftAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateRightShiftAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
		std::shared_ptr<SyntaxNode> CreateIncrement(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateDecrement(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateExpressionIf(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> trueStatement, std::shared_ptr<SyntaxNode> falseStatement);

		// primary
		std::shared_ptr<SyntaxNode> CreateNegative(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateNot(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateComplement1(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateSizeOf(std::shared_ptr<SyntaxNode> variableType);
		std::shared_ptr<SyntaxNode> CreateCall(const std::string_view identifier, std::shared_ptr<SyntaxNode> argument);

		// constant
		std::shared_ptr<SyntaxNode> CreatePriority();
		std::shared_ptr<SyntaxNode> CreateSelf();
		std::shared_ptr<SyntaxNode> CreateSender();
		std::shared_ptr<SyntaxNode> CreateNil();
		std::shared_ptr<SyntaxNode> CreateInteger(const int_t value);
		std::shared_ptr<SyntaxNode> CreateFloat(const float_t value);
		std::shared_ptr<SyntaxNode> CreateString(const std::string_view text);

		// left hand
		std::shared_ptr<SyntaxNode> CreateMemberVariable(std::shared_ptr<SyntaxNode> leftHand, const std::string_view identifier);
		std::shared_ptr<SyntaxNode> CreateMemberFunction(std::shared_ptr<SyntaxNode> leftHand, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument);
		std::shared_ptr<SyntaxNode> CreateArray(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateIdentifier(const std::string_view identifier);

		// cases
		std::shared_ptr<SyntaxNode> BindCaseNode(std::shared_ptr<SyntaxNode> next, std::shared_ptr<SyntaxNode> kase);

		// case
		std::shared_ptr<SyntaxNode> CreateSwitchCaseNode(std::shared_ptr<SyntaxNode> expression, std::shared_ptr<SyntaxNode> statements);
		std::shared_ptr<SyntaxNode> CreateSwitchDefaultNode(std::shared_ptr<SyntaxNode> statements);

		// arg_calls
		std::shared_ptr<SyntaxNode> CreateArgumentNode(std::shared_ptr<SyntaxNode> expression);
		std::shared_ptr<SyntaxNode> CreateArgumentNode(std::shared_ptr<SyntaxNode> nextArgument, std::shared_ptr<SyntaxNode> expression);

		// arg_decls
		std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(std::shared_ptr<SyntaxNode> declareVariable);
		std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(std::shared_ptr<SyntaxNode> nextDeclareArgument, std::shared_ptr<SyntaxNode> declareVariable);

		// variable_decl
		std::shared_ptr<SyntaxNode> CreateDeclareVariableNode(std::shared_ptr<SyntaxNode> variableType, std::shared_ptr<SyntaxNode> declarator, std::shared_ptr<SyntaxNode> expression);

		// declarator
		std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view identifier);
		std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view identifier, std::shared_ptr<SyntaxNode> variableSizeNode);

		// variable_size
		std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const size_t size);
		std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const std::string_view identifier);

	private:
		std::unique_ptr<Parser> mParser;

		std::shared_ptr<CodeBuffer> mCodeBuffer;
		std::shared_ptr<CodeGenerator> mCodeGenerator;
		std::shared_ptr<DataBuffer> mDataBuffer;
		std::shared_ptr<GlobalSemanticAnalyzer> mGlobalSemanticAnalyzer;
		std::shared_ptr<LocalSemanticAnalyzer> mLocalSemanticAnalyzer;
		std::shared_ptr<SymbolFactory> mSymbolFactory;
		std::shared_ptr<StringPool> mStringPool;
		std::shared_ptr<TypeDescriptorFactory> mTypeDescriptorFactory;

		friend class Parser;
	};
}
