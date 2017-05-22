/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "Main.h"
#include "CodeGenerator.h"
#include "GlobalAddressResolver.h"
#include "GlobalSemanticAnalyzer.h"
#include "DataBuffer.h"
#include "SyntaxNode.h"
#include "TypeDescriptorFactory.h"
#include <memory>

int yynerrs = 0;

extern int yylex(mana::Parser::value_type* yylval);

namespace mana
{
	static std::shared_ptr<SyntaxNode> Bind(std::shared_ptr<SyntaxNode> base, std::shared_ptr<SyntaxNode> next);

	// declarations
	static std::shared_ptr<SyntaxNode> CreateNativeFunction(std::shared_ptr<SyntaxNode> returnExpression, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument);
	static std::shared_ptr<SyntaxNode> CreateDeclareMemory(const int_t size, std::shared_ptr<SyntaxNode> allocateDeclarations);
	static std::shared_ptr<SyntaxNode> CreateDeclareStaticMemory(const int_t size, std::shared_ptr<SyntaxNode> allocateDeclarations);
	static std::shared_ptr<SyntaxNode> CreateAlias(const std::string_view identifier, const std::string_view filename);	// Not yet

	// declaration
	static std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const int_t value);
	static std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const float_t value);
	static std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view identifier, const std::string_view text);
	static std::shared_ptr<SyntaxNode> CreateDefineNode(const std::string_view identifier, const std::string_view text);
	static std::shared_ptr<SyntaxNode> CreateUndefineNode(const std::string_view identifier);

	// actor
	static std::shared_ptr<SyntaxNode> CreateActor(const std::string_view identifier, std::shared_ptr<SyntaxNode> action);
	static std::shared_ptr<SyntaxNode> CreatePhantom(const std::string_view identifier, std::shared_ptr<SyntaxNode> action);
	static std::shared_ptr<SyntaxNode> CreateModule(const std::string_view identifier, std::shared_ptr<SyntaxNode> action);

	// action
	static std::shared_ptr<SyntaxNode> CreateAction(const std::string_view identifier, std::shared_ptr<SyntaxNode> block);
	static std::shared_ptr<SyntaxNode> CreateExtend(const std::string_view identifier);

	// struct
	static std::shared_ptr<SyntaxNode> CreateStruct(const std::string_view identifier, std::shared_ptr<SyntaxNode> member);

	// function
	static std::shared_ptr<SyntaxNode> CreateInternalFunction(std::shared_ptr<SyntaxNode> returnExpression, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument, std::shared_ptr<SyntaxNode> statement);

	// variable_type
	static std::shared_ptr<SyntaxNode> CreateActorTypeDescription();
	static std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::string_view identifier);
	static std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescriptor);

	// block
	static std::shared_ptr<SyntaxNode> CreateBlock(std::shared_ptr<SyntaxNode> statement);

	// statement	
	static std::shared_ptr<SyntaxNode> CreateIf(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statementIfTrue, std::shared_ptr<SyntaxNode> statementIfFalse);
	static std::shared_ptr<SyntaxNode> CreateSwitch(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statement);
	static std::shared_ptr<SyntaxNode> CreateWhile(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> statement);
	static std::shared_ptr<SyntaxNode> CreateDoWhile(std::shared_ptr<SyntaxNode> statement, std::shared_ptr<SyntaxNode> condition);
	static std::shared_ptr<SyntaxNode> CreateFor(std::shared_ptr<SyntaxNode> initializeStatement, std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> iterationExpression, std::shared_ptr<SyntaxNode> statement);
	static std::shared_ptr<SyntaxNode> CreateFor(std::shared_ptr<SyntaxNode> declareVariable, std::shared_ptr<SyntaxNode> initializeStatement, std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> iterationExpression, std::shared_ptr<SyntaxNode> statement);
	static std::shared_ptr<SyntaxNode> CreateLoop(std::shared_ptr<SyntaxNode> statement);
	static std::shared_ptr<SyntaxNode> CreateLock(std::shared_ptr<SyntaxNode> statement);
	static std::shared_ptr<SyntaxNode> CreateGoto(const std::string_view labelName);
	static std::shared_ptr<SyntaxNode> CreateLabel(const std::string_view identifier);
	static std::shared_ptr<SyntaxNode> CreateReturn(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateRollback(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateBreak();
	static std::shared_ptr<SyntaxNode> CreateContinue();
	static std::shared_ptr<SyntaxNode> CreateHalt();
	static std::shared_ptr<SyntaxNode> CreateYield();
	static std::shared_ptr<SyntaxNode> CreateComply();	// consider the name
	static std::shared_ptr<SyntaxNode> CreateRefuse();	// consider the name
	static std::shared_ptr<SyntaxNode> CreatePrint(std::shared_ptr<SyntaxNode> argument);
	static std::shared_ptr<SyntaxNode> CreateRequest(std::shared_ptr<SyntaxNode> priority, std::shared_ptr<SyntaxNode> expression, const std::string_view actionName);
	static std::shared_ptr<SyntaxNode> CreateJoin(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);

	// expression
	static std::shared_ptr<SyntaxNode> CreateAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateLogicalAnd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateLogicalOr(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateAdd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateSub(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateMul(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateDiv(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateMod(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreatePow(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateAnd(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateOr(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateXor(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateLeftShift(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateRightShift(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateGT(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateGE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateLS(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateLE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateEQ(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateNE(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateAddAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateSubAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateMulAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateDivAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateModAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreatePowAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateAndAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateOrAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateXorAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateLeftShiftAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateRightShiftAndAssign(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> rightHand);
	static std::shared_ptr<SyntaxNode> CreateIncrement(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateDecrement(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateExpressionIf(std::shared_ptr<SyntaxNode> condition, std::shared_ptr<SyntaxNode> trueStatement, std::shared_ptr<SyntaxNode> falseStatement);

	// primary
	static std::shared_ptr<SyntaxNode> CreateNegative(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateNot(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateComplement1(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateSizeOf(std::shared_ptr<SyntaxNode> variableType);
	static std::shared_ptr<SyntaxNode> CreateCall(const std::string_view identifier, std::shared_ptr<SyntaxNode> argument);

	// constant
	static std::shared_ptr<SyntaxNode> CreatePriority();
	static std::shared_ptr<SyntaxNode> CreateSelf();
	static std::shared_ptr<SyntaxNode> CreateSender();
	static std::shared_ptr<SyntaxNode> CreateNil();
	static std::shared_ptr<SyntaxNode> CreateInteger(const int_t value);
	static std::shared_ptr<SyntaxNode> CreateFloat(const float_t value);
	static std::shared_ptr<SyntaxNode> CreateString(const std::string_view text);

	// left hand
	static std::shared_ptr<SyntaxNode> CreateMemberVariable(std::shared_ptr<SyntaxNode> leftHand, const std::string_view identifier);
	static std::shared_ptr<SyntaxNode> CreateMemberFunction(std::shared_ptr<SyntaxNode> leftHand, const std::string_view identifier, std::shared_ptr<SyntaxNode> argument);
	static std::shared_ptr<SyntaxNode> CreateArray(std::shared_ptr<SyntaxNode> leftHand, std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateIdentifier(const std::string_view identifier);

	// cases
	static std::shared_ptr<SyntaxNode> BindCaseNode(std::shared_ptr<SyntaxNode> next, std::shared_ptr<SyntaxNode> kase);

	// case
	static std::shared_ptr<SyntaxNode> CreateSwitchCaseNode(std::shared_ptr<SyntaxNode> expression, std::shared_ptr<SyntaxNode> statements);
	static std::shared_ptr<SyntaxNode> CreateSwitchDefaultNode(std::shared_ptr<SyntaxNode> statements);

	// arg_calls
	static std::shared_ptr<SyntaxNode> CreateArgumentNode(std::shared_ptr<SyntaxNode> expression);
	static std::shared_ptr<SyntaxNode> CreateArgumentNode(std::shared_ptr<SyntaxNode> nextArgument, std::shared_ptr<SyntaxNode> expression);

	// arg_decls
	static std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(std::shared_ptr<SyntaxNode> declareVariable);
	static std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(std::shared_ptr<SyntaxNode> nextDeclareArgument, std::shared_ptr<SyntaxNode> declareVariable);

	// variable_decl
	static std::shared_ptr<SyntaxNode> CreateDeclareVariableNode(std::shared_ptr<SyntaxNode> variableType, std::shared_ptr<SyntaxNode> declarator, std::shared_ptr<SyntaxNode> expression);

	// declarator
	static std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view identifier);
	static std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view identifier, std::shared_ptr<SyntaxNode> variableSizeNode);

	// variable_size
	static std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const size_t size);
	static std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const std::string_view identifier);
}
