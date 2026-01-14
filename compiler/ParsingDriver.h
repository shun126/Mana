/*!
mana (compiler)

@author	Shun Moriya
@date	2022-
*/

#pragma once
#include "../runner/common/Noncopyable.h"
#include "Parser.hpp"
#include <memory>

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

	/**
	 * Drive the Bison parser.
	 */
	class ParsingDriver final : Noncopyable, public std::enable_shared_from_this<ParsingDriver>
	{
	public:
		ParsingDriver();
		ParsingDriver(const ParsingDriver&) = delete;
		ParsingDriver(ParsingDriver&&) noexcept = delete;
		ParsingDriver& operator=(const ParsingDriver&) = delete;
		ParsingDriver& operator=(ParsingDriver&&) noexcept = delete;
		~ParsingDriver() = default;

		int32_t Parse() const;

		const std::shared_ptr<CodeBuffer>& GetCodeBuffer();
		const std::shared_ptr<CodeGenerator>& GetCodeGenerator();
		const std::shared_ptr<DataBuffer>& GetDataBuffer();
		const std::shared_ptr<GlobalSemanticAnalyzer>& GetGlobalSemanticAnalyzer();
		const std::shared_ptr<SymbolFactory>& GetSymbolFactory();
		const std::shared_ptr<StringPool>& GetStringPool();
		const std::shared_ptr<TypeDescriptorFactory>& GetTypeDescriptorFactory();
		std::shared_ptr<SyntaxNode> InjectGlobalInitializers(const std::shared_ptr<SyntaxNode>& root);

	protected:
		std::shared_ptr<SyntaxNode> Bind(const std::shared_ptr<SyntaxNode>& base, const std::shared_ptr<SyntaxNode>& next);

		// declarations
		static std::shared_ptr<SyntaxNode> CreateNativeFunction(const std::shared_ptr<SyntaxNode>& returnExpression, const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument);
		static std::shared_ptr<SyntaxNode> CreateDeclareMemory(const int_t size, const std::shared_ptr<SyntaxNode>& allocateDeclarations);
		static std::shared_ptr<SyntaxNode> CreateDeclareStaticMemory(const int_t size, const std::shared_ptr<SyntaxNode>& allocateDeclarations);

		// declaration
		std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view& identifier, const int_t value);
		std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view& identifier, const bool value);
		std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view& identifier, const float_t value);
		std::shared_ptr<SyntaxNode> CreateConstantNode(const std::string_view& identifier, const std::string_view& text);
		std::shared_ptr<SyntaxNode> CreateDefineNode(const std::string_view& identifier, const std::string_view& text);
		static std::shared_ptr<SyntaxNode> CreateUndefineNode(const std::string_view& identifier);

		// actor
		static std::shared_ptr<SyntaxNode> CreateActor(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& action);
		static std::shared_ptr<SyntaxNode> CreatePhantom(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& action);
		static std::shared_ptr<SyntaxNode> CreateModule(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& action);

		// action
		std::shared_ptr<SyntaxNode> CreateAction(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& block);
		static std::shared_ptr<SyntaxNode> CreateExtend(const std::string_view& identifier);

		// struct
		static std::shared_ptr<SyntaxNode> CreateStruct(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& member);

		// function
		static std::shared_ptr<SyntaxNode> CreateInternalFunction(const std::shared_ptr<SyntaxNode>& returnExpression, const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument, const std::shared_ptr<SyntaxNode>& statement);

		// variable_type
		std::shared_ptr<SyntaxNode> CreateActorTypeDescription();
		static std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::string_view& identifier);
		static std::shared_ptr<SyntaxNode> CreateTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescriptor);

		// block
		static std::shared_ptr<SyntaxNode> CreateBlock(const std::shared_ptr<SyntaxNode>& statement);

		// statement	
		static std::shared_ptr<SyntaxNode> CreateIf(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& statementIfTrue, const std::shared_ptr<SyntaxNode>& statementIfFalse);
		static std::shared_ptr<SyntaxNode> CreateSwitch(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& statement);
		static std::shared_ptr<SyntaxNode> CreateWhile(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& statement);
		static std::shared_ptr<SyntaxNode> CreateDoWhile(const std::shared_ptr<SyntaxNode>& statement, const std::shared_ptr<SyntaxNode>& condition);
		static std::shared_ptr<SyntaxNode> CreateFor(const std::shared_ptr<SyntaxNode>& initializeStatement, const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& iterationExpression, const std::shared_ptr<SyntaxNode>& statement);
		static std::shared_ptr<SyntaxNode> CreateFor(const std::shared_ptr<SyntaxNode>& declareVariable, const std::shared_ptr<SyntaxNode>& initializeStatement, const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& iterationExpression, const std::shared_ptr<SyntaxNode>& statement);
		static std::shared_ptr<SyntaxNode> CreateLoop(const std::shared_ptr<SyntaxNode>& statement);
		static std::shared_ptr<SyntaxNode> CreateLock(const std::shared_ptr<SyntaxNode>& statement);
		static std::shared_ptr<SyntaxNode> CreateGoto(const std::string_view& labelName);
		static std::shared_ptr<SyntaxNode> CreateLabel(const std::string_view& labelName);
		static std::shared_ptr<SyntaxNode> CreateReturn(const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateRollback(const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateBreak();
		static std::shared_ptr<SyntaxNode> CreateContinue();
		static std::shared_ptr<SyntaxNode> CreateHalt();
		static std::shared_ptr<SyntaxNode> CreateYield();
		static std::shared_ptr<SyntaxNode> CreateComply();	// TODO: consider the name
		static std::shared_ptr<SyntaxNode> CreateRefuse();	// TODO: consider the name
		static std::shared_ptr<SyntaxNode> CreatePrint(const std::shared_ptr<SyntaxNode>& argument);
		static std::shared_ptr<SyntaxNode> CreateRequest(const std::shared_ptr<SyntaxNode>& priority, const std::shared_ptr<SyntaxNode>& expression, const std::string_view& actionName);
		static std::shared_ptr<SyntaxNode> CreateAwaitStart(const std::shared_ptr<SyntaxNode>& priority, const std::shared_ptr<SyntaxNode>& expression, const std::string_view& actionName);
		static std::shared_ptr<SyntaxNode> CreateAwaitCompletion(const std::shared_ptr<SyntaxNode>& priority, const std::shared_ptr<SyntaxNode>& expression, const std::string_view& actionName);
		static std::shared_ptr<SyntaxNode> CreateJoin(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);

		// expression
		static std::shared_ptr<SyntaxNode> CreateAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateLogicalAnd(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateLogicalOr(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateAdd(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateSub(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateMul(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateDiv(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateMod(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreatePow(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateAnd(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateOr(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateXor(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateLeftShift(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateRightShift(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateGreater(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateGreaterEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateLess(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateLessEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateNotEqual(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateAddAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateSubAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateMulAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateDivAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateModAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreatePowAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateAndAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateOrAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateXorAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateLeftShiftAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		static std::shared_ptr<SyntaxNode> CreateRightShiftAndAssign(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& rightHand);
		std::shared_ptr<SyntaxNode> CreateIncrement(const std::shared_ptr<SyntaxNode>& expression);
		std::shared_ptr<SyntaxNode> CreateDecrement(const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateExpressionIf(const std::shared_ptr<SyntaxNode>& condition, const std::shared_ptr<SyntaxNode>& trueStatement, const std::shared_ptr<SyntaxNode>& falseStatement);

		// primary
		static std::shared_ptr<SyntaxNode> CreateNegative(const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateNot(const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateComplement1(const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateSizeOf(const std::shared_ptr<SyntaxNode>& variableType);
		static std::shared_ptr<SyntaxNode> CreateCall(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument);

		// constant
		std::shared_ptr<SyntaxNode> CreatePriority();
		std::shared_ptr<SyntaxNode> CreateSelf();
		std::shared_ptr<SyntaxNode> CreateSender();
		std::shared_ptr<SyntaxNode> CreateNil();
		std::shared_ptr<SyntaxNode> CreateBool(const bool value);
		std::shared_ptr<SyntaxNode> CreateInteger(const int_t value);
		std::shared_ptr<SyntaxNode> CreateFloat(const float_t value);
		std::shared_ptr<SyntaxNode> CreateString(const std::string_view& text);

		// left hand
		static std::shared_ptr<SyntaxNode> CreateMemberVariable(const std::shared_ptr<SyntaxNode>& leftHand, const std::string_view& identifier);
		static std::shared_ptr<SyntaxNode> CreateMemberFunction(const std::shared_ptr<SyntaxNode>& leftHand, const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& argument);
		static std::shared_ptr<SyntaxNode> CreateArray(const std::shared_ptr<SyntaxNode>& leftHand, const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateIdentifier(const std::string_view& identifier);

		// cases
		static std::shared_ptr<SyntaxNode> BindCaseNode(const std::shared_ptr<SyntaxNode>& next, const std::shared_ptr<SyntaxNode>& kase);

		// case
		static std::shared_ptr<SyntaxNode> CreateSwitchCaseNode(const std::shared_ptr<SyntaxNode>& expression, const std::shared_ptr<SyntaxNode>& statements);
		static std::shared_ptr<SyntaxNode> CreateSwitchDefaultNode(const std::shared_ptr<SyntaxNode>& statements);

		// arg_calls
		static std::shared_ptr<SyntaxNode> CreateArgumentNode(const std::shared_ptr<SyntaxNode>& expression);
		static std::shared_ptr<SyntaxNode> CreateArgumentNode(const std::shared_ptr<SyntaxNode>& nextArgument, const std::shared_ptr<SyntaxNode>& expression);

		// arg_decls
		static std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(const std::shared_ptr<SyntaxNode>& declareVariable);
		static std::shared_ptr<SyntaxNode> CreateDeclareArgumentNode(const std::shared_ptr<SyntaxNode>& nextDeclareArgument, const std::shared_ptr<SyntaxNode>& declareVariable);

		// variable_decl
		static std::shared_ptr<SyntaxNode> CreateDeclareVariableNode(const std::shared_ptr<SyntaxNode>& variableType, const std::shared_ptr<SyntaxNode>& declarator, const std::shared_ptr<SyntaxNode>& expression);

		// declarator
		static std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view& identifier);
		static std::shared_ptr<SyntaxNode> CreateDeclaratorNode(const std::string_view& identifier, const std::shared_ptr<SyntaxNode>& variableSizeNode);

		// variable_size
		static std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const size_t size);
		static std::shared_ptr<SyntaxNode> CreateVariableSizeNode(const std::string_view& identifier);

	private:
		std::shared_ptr<SyntaxNode> AppendNode(const std::shared_ptr<SyntaxNode>& base, const std::shared_ptr<SyntaxNode>& next) const;
		std::shared_ptr<SyntaxNode> CollectGlobalInitializerStatements(const std::shared_ptr<SyntaxNode>& root) const;
		std::shared_ptr<SyntaxNode> CollectInitializerStatementsFromDeclarations(const std::shared_ptr<SyntaxNode>& node) const;
		bool HasGlobalNameConflict(const std::shared_ptr<SyntaxNode>& root, const std::string_view& name) const;

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
