/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "Main.h"
#include "SyntaxNode.h"
#include <memory>

namespace mana
{
	static SyntaxNode* Bind(SyntaxNode* base, SyntaxNode* next);


	// declarations
	static SyntaxNode* CreateNativeFunction(SyntaxNode* returnExpression, const char* identifier, SyntaxNode* argument);
	static SyntaxNode* CreateDeclareMemory(const int_t size, SyntaxNode* allocateDeclarations);
	static SyntaxNode* CreateDeclareStaticMemory(const int_t size, SyntaxNode* allocateDeclarations);
	static SyntaxNode* CreateAlias(const char* identifier, const char* filename);	// Not yet

	// declaration
	static SyntaxNode* CreateConstantNode(const char* identifier, const int_t value);
	static SyntaxNode* CreateConstantNode(const char* identifier, const float_t value);
	static SyntaxNode* CreateConstantNode(const char* identifier, const char* text);

	// actor
	static SyntaxNode* CreateActor(const char* identifier, SyntaxNode* action);
	static SyntaxNode* CreatePhantom(const char* identifier, SyntaxNode* action);
	static SyntaxNode* CreateModule(const char* identifier, SyntaxNode* action);

	// action
	static SyntaxNode* CreateAction(const char* identifier, SyntaxNode* block);
	static SyntaxNode* CreateExtend(const char* identifier);

	// struct
	static SyntaxNode* CreateStruct(const char* identifier, SyntaxNode* member);

	// function
	static SyntaxNode* CreateInternalFunction(SyntaxNode* returnExpression, const char* identifier, SyntaxNode* argument, SyntaxNode* statement);

	// variable_type
	static SyntaxNode* CreateActorTypeDescription();
	static SyntaxNode* CreateTypeDescription(const char* identifier);
	static SyntaxNode* CreateTypeDescription(TypeDescriptor* typeDescriptor);

	// block
	static SyntaxNode* CreateBlock(SyntaxNode* statement);

	// statement	
	static SyntaxNode* CreateIf(SyntaxNode* condition, SyntaxNode* statementIfTrue, SyntaxNode* statementIfFalse);
	static SyntaxNode* CreateSwitch(SyntaxNode* condition, SyntaxNode* statement);
	static SyntaxNode* CreateWhile(SyntaxNode* condition, SyntaxNode* statement);
	static SyntaxNode* CreateDoWhile(SyntaxNode* statement, SyntaxNode* condition);
	static SyntaxNode* CreateFor(SyntaxNode* initializeStatement, SyntaxNode* condition, SyntaxNode* iterationExpression, SyntaxNode* statement);
	static SyntaxNode* CreateFor(SyntaxNode* declareVariable, SyntaxNode* initializeStatement, SyntaxNode* condition, SyntaxNode* iterationExpression, SyntaxNode* statement);
	static SyntaxNode* CreateLoop(SyntaxNode* statement);
	static SyntaxNode* CreateLock(SyntaxNode* statement);
	static SyntaxNode* CreateGoto(const char* labelName);
	static SyntaxNode* CreateLabel(const char* identifier);
	static SyntaxNode* CreateReturn(SyntaxNode* expression);
	static SyntaxNode* CreateRollback(SyntaxNode* expression);
	static SyntaxNode* CreateBreak();
	static SyntaxNode* CreateContinue();
	static SyntaxNode* CreateHalt();
	static SyntaxNode* CreateYield();
	static SyntaxNode* CreateComply();	// consider the name
	static SyntaxNode* CreateRefuse();	// consider the name
	static SyntaxNode* CreatePrint(SyntaxNode* argument);
	static SyntaxNode* CreateRequest(SyntaxNode* priority, SyntaxNode* expression, const char* actionName);
	static SyntaxNode* CreateJoin(SyntaxNode* leftHand, SyntaxNode* rightHand);

	// expression
	static SyntaxNode* CreateAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateLogicalAnd(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateLogicalOr(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateAdd(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateSub(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateMul(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateDiv(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateMod(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreatePow(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateAnd(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateOr(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateXor(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateLeftShift(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateRightShift(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateGT(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateGE(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateLS(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateLE(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateEQ(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateNE(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateAddAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateSubAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateMulAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateDivAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateModAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreatePowAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateAndAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateOrAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateXorAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateLeftShiftAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateRightShiftAndAssign(SyntaxNode* leftHand, SyntaxNode* rightHand);
	static SyntaxNode* CreateIncrement(SyntaxNode* expression);
	static SyntaxNode* CreateDecrement(SyntaxNode* expression);
	static SyntaxNode* CreateExpressionIf(SyntaxNode* condition, SyntaxNode* trueStatement, SyntaxNode* falseStatement);

	// primary
	static SyntaxNode* CreateNegative(SyntaxNode* expression);
	static SyntaxNode* CreateNot(SyntaxNode* expression);
	static SyntaxNode* CreateComplement1(SyntaxNode* expression);
	static SyntaxNode* CreateSizeOf(SyntaxNode* variableType);
	static SyntaxNode* CreateCall(const char* identifier, SyntaxNode* argument);

	// constant
	static SyntaxNode* CreatePriority();
	static SyntaxNode* CreateSelf();
	static SyntaxNode* CreateSender();
	static SyntaxNode* CreateNil();
	static SyntaxNode* CreateInteger(const int_t value);
	static SyntaxNode* CreateFloat(const float_t value);
	static SyntaxNode* CreateString(const char* text);

	// left hand
	static SyntaxNode* CreateMemberVariable(SyntaxNode* leftHand, const char* identifier);
	static SyntaxNode* CreateMemberFunction(SyntaxNode* leftHand, const char* identifier, SyntaxNode* argument);
	static SyntaxNode* CreateArray(SyntaxNode* leftHand, SyntaxNode* expression);
	static SyntaxNode* CreateIdentifier(const char* identifier);

	// cases
	static SyntaxNode* BindCaseNode(SyntaxNode* next, SyntaxNode* kase);

	// case
	static SyntaxNode* CreateSwitchCaseNode(SyntaxNode* expression, SyntaxNode* statements);
	static SyntaxNode* CreateSwitchDefaultNode(SyntaxNode* statements);

	// arg_calls
	static SyntaxNode* CreateArgumentNode(SyntaxNode* expression);
	static SyntaxNode* CreateArgumentNode(SyntaxNode* nextArgument, SyntaxNode* expression);

	// arg_decls
	static SyntaxNode* CreateDeclareArgumentNode(SyntaxNode* declareVariable);
	static SyntaxNode* CreateDeclareArgumentNode(SyntaxNode* nextDeclareArgument, SyntaxNode* declareVariable);

	// variable_decl
	static SyntaxNode* CreateDeclareVariableNode(SyntaxNode* variableType, SyntaxNode* declarator, SyntaxNode* expression);

	// declarator
	static SyntaxNode* CreateDeclaratorNode(const char* identifier);
	static SyntaxNode* CreateDeclaratorNode(const char* identifier, SyntaxNode* variableSizeNode);

	// variable_size
	static SyntaxNode* CreateVariableSizeNode(const size_t size);
	static SyntaxNode* CreateVariableSizeNode(const char* identifier);
}
