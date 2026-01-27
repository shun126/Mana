%{
/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
#define YYERROR_VERBOSE
#endif

%}

%require "3.2"
%language "c++"

%define api.parser.class {Parser}
%define api.namespace {mana}
%define api.value.type variant

%parse-param { mana::ParsingDriver* mParsingDriver }

%code requires
{
	#include "../runner/common/Setup.h"
	#include "SyntaxNode.h"
	#include "TypeDescriptor.h"

	namespace mana {
		class ParsingDriver;

		struct ActionReference
		{
			std::shared_ptr<mana::SyntaxNode> actor;
			std::string_view action;
			bool legacy = false;
		};
	}
}

%code top
{
	#include "../runner/common/Setup.h"
	#include "ErrorHandler.h"
	#include "Lexer.h"
	#include "Main.h"
	#include "ParsingDriver.h"
	#include "Symbol.h"
	#include "SyntaxNode.h"
	#include "TypeDescriptor.h"

#include "Main.h"
#include "CodeGenerator.h"
#include "GlobalAddressResolver.h"
#include "GlobalSemanticAnalyzer.h"
#include "DataBuffer.h"
#include "SyntaxNode.h"
#include "TypeDescriptorFactory.h"

	#include "ParserDeclaration.inl"
	#include <memory>
	#include <string_view>
}

%type	<std::shared_ptr<mana::SyntaxNode>> block case cases left_hand constant expression statement statements variable_size variable_sizes variable_type function struct_member struct_members struct action actions actor declarator declaration allocate_declarations declarations primary arg_calls arg_decls variable_decl line
%type	<std::string_view> qualified_name
%type	<mana::ActionReference> action_ref
%token	<mana::int_t> tDIGIT
%token	<mana::float_t> tREAL
%token	<std::string_view> tSTRING tIDENTIFIER
%token	<std::shared_ptr<mana::TypeDescriptor>> tTYPE

%token	tDEFINE tUNDEF tINCLUDE tIMPORT
%token	tNATIVE tSTRUCT tACTOR tPHANTOM tACTION tMODULE tEXTEND
%token	tNAMESPACE tUSING
%token	tFALSE tTRUE tPRIORITY tSELF tSENDER tNIL
%token	tREQUEST tAwaitStart tAwaitCompletion tJOIN
%token	tBREAK
%token	tCONTINUE
%token	tCASE
%token	tDEFAULT
%token	GOTO
%token	tHALT
%token	tLOCK
%token	tDC
%token	tARROW
%token	tDO
%token	tELSE
%token	tFOR
%token	tIF
%token	tLOOP
%token	tYIELD
%token	tRETURN
%token	tROLLBACK
%token	tSWITCH
%token	tWHILE
%token	tPRINT tSTATIC tALLOCATE tCOMPLY tREFUSE

%nonassoc	tAMUL tADIV tAMOD;
%nonassoc	tAADD tASUB;
%nonassoc	tALSHFT tARSHFT;
%nonassoc	tAAND tAOR tAXOR;
%nonassoc	'='
%right		'?' ':'
%left		tAND tOR
%nonassoc	tEQ tNE
%nonassoc	'>' tGE tLE '<'
%left		'|' '^'
%left		'&'
%left		tLSHFT tRSHFT
%left		'+' '-'
%left		'*' '/' '%'
%right		tPOW
%right		tSIZEOF
%right		'!' '~'
%right		tUPLUS tUMINUS
%left		tINC tDEC
%right		tUINC tUDEC

%%
program			: line
					{
						auto root = mParsingDriver->InjectGlobalInitializers($1);
						mParsingDriver->SetRootSyntaxNode(root);

						auto globalSemanticAnalyzer = mParsingDriver->GetGlobalSemanticAnalyzer();
						globalSemanticAnalyzer->Resolve(root);

						auto codeGenerator = mParsingDriver->GetCodeGenerator();
						codeGenerator->GenerateCode(root, true);

						auto globalAddressResolver = mParsingDriver->GetCodeGenerator()->GetGlobalAddressResolver();
						globalAddressResolver->ResolveAddress();
					}
				;
				
line			: // empty
					{ $$ = nullptr; }
				| declarations line
					{ $$ = mParsingDriver->Bind($1, $2); }
				;

declarations	: actor
				| struct
				| function
				| declaration ';'
				| tNAMESPACE qualified_name '{' line '}'
					{ $$ = mParsingDriver->CreateNamespace($2, $4); }
				| tUSING qualified_name ';'
					{ $$ = mParsingDriver->CreateUsing($2); }

				| tNATIVE variable_type tIDENTIFIER '(' arg_decls ')' ';'
					{ $$ = mParsingDriver->CreateNativeFunction($2, $3, $5); }

				| tALLOCATE tDIGIT '{' allocate_declarations '}' 
					{ $$ = mParsingDriver->CreateDeclareMemory($2, $4); }
				| tSTATIC '{' allocate_declarations '}'
					{ $$ = mParsingDriver->CreateDeclareStaticMemory(0, $3); }
				| tSTATIC tALLOCATE tDIGIT '{' allocate_declarations '}'
					{ $$ = mParsingDriver->CreateDeclareStaticMemory($3, $5); }

				| tINCLUDE tSTRING ';'
					{ $$ = nullptr; if(! lexer::Open($2, false)){ YYABORT; } }
				| tIMPORT tSTRING ';'
					{ $$ = nullptr; if(! lexer::Open($2, true)){ YYABORT; } }
				;

allocate_declarations
				: // empty
					{ $$ = nullptr; }
				| variable_decl ';' allocate_declarations
					{ $$ = mParsingDriver->Bind($1, $3); }
				;

declaration		: variable_decl
				| tDEFINE tIDENTIFIER tDIGIT
					{ $$ = mParsingDriver->CreateConstantNode($2, $3); }
				| tDEFINE tIDENTIFIER tREAL
					{ $$ = mParsingDriver->CreateConstantNode($2, $3); }
				| tDEFINE tIDENTIFIER tFALSE
					{ $$ = mParsingDriver->CreateConstantNode($2, false); }
				| tDEFINE tIDENTIFIER tTRUE
					{ $$ = mParsingDriver->CreateConstantNode($2, true); }
				| tDEFINE tIDENTIFIER '-' tDIGIT
					{ $$ = mParsingDriver->CreateConstantNode($2, -$4); }
				| tDEFINE tIDENTIFIER '-' tREAL
					{ $$ = mParsingDriver->CreateConstantNode($2, -$4); }
				| tDEFINE tIDENTIFIER tSTRING
					{ $$ = mParsingDriver->CreateConstantNode($2, $3); }
				| tDEFINE tIDENTIFIER tIDENTIFIER
					{ $$ = mParsingDriver->CreateDefineNode($2, $3); }
				| tUNDEF tIDENTIFIER
					{ $$ = mParsingDriver->CreateUndefineNode($2); }
				;

actor			: tACTOR tIDENTIFIER '{' actions '}'
					{ $$ = mParsingDriver->CreateActor($2, $4); }
				| tPHANTOM tIDENTIFIER '{' actions '}'
					{ $$ = mParsingDriver->CreatePhantom($2, $4); }
				| tMODULE tIDENTIFIER '{' actions '}'
					{ $$ = mParsingDriver->CreateModule($2, $4); }
				;

actions			: // empty
					{ $$ = nullptr; }
				| action actions
					{ $$ = mParsingDriver->Bind($1, $2); }
				;

action			: tACTION tIDENTIFIER block
					{ $$ = mParsingDriver->CreateAction($2, $3); }
				| tEXTEND qualified_name ';'
					{ $$ = mParsingDriver->CreateExtend($2); }
				| declaration ';'
				;

struct			: tSTRUCT tIDENTIFIER '{' struct_members '}'
					{ $$ = mParsingDriver->CreateStruct($2, $4); }
				;

struct_members	: // empty
					{ $$ = nullptr; }
				| struct_member struct_members
					{ $$ = mParsingDriver->Bind($1, $2); }
				;

struct_member	: variable_decl ';'
				;

function		: variable_type tIDENTIFIER '(' arg_decls ')' block
					{ $$ = mParsingDriver->CreateInternalFunction($1, $2, $4, $6); }
				;

variable_type	: tACTOR
					{ $$ = mParsingDriver->CreateActorTypeDescription(); }
				| qualified_name
					{ $$ = mParsingDriver->CreateTypeDescription($1); }
				| tTYPE
					{ $$ = mParsingDriver->CreateTypeDescription($1); }
				;

block			: '{' statements '}'
					{ $$ = mParsingDriver->CreateBlock($2); }
				;

statements		: // empty
					{ $$ = nullptr; }
				| statement statements
					{ $$ = mParsingDriver->Bind($1, $2); }
				;
statement		: tIF '(' expression ')' statement
					{ $$ = mParsingDriver->CreateIf($3, $5, nullptr); }
				| tIF '(' expression ')' statement tELSE statement
					{ $$ = mParsingDriver->CreateIf($3, $5, $7); }
				| tSWITCH '(' expression ')' '{' cases '}'
					{ $$ = mParsingDriver->CreateSwitch($3, $6); }
				| tWHILE '(' expression ')' statement
					{ $$ = mParsingDriver->CreateWhile($3, $5); }
				| tDO statement tWHILE '(' expression ')' ';'
					{ $$ = mParsingDriver->CreateDoWhile($2, $5); }
				| tFOR '(' expression ';' expression ';' expression ')' statement
					{ $$ = mParsingDriver->CreateFor($3, $5, $7, $9); }
				| tFOR '(' variable_decl ';' expression ';' expression ')' statement
					{ $$ = mParsingDriver->CreateFor($3, $5, $7, $9); }
				| tLOOP statement
					{ $$ = mParsingDriver->CreateLoop($2); }
				| tLOCK statement
					{ $$ = mParsingDriver->CreateLock($2); }
				| GOTO tIDENTIFIER ';'
					{ $$ = mParsingDriver->CreateGoto($2); }
				| tIDENTIFIER ':'
					{ $$ = mParsingDriver->CreateLabel($1); }
				| tRETURN ';'
					{ $$ = mParsingDriver->CreateReturn(nullptr); }
				| tRETURN expression ';'
					{ $$ = mParsingDriver->CreateReturn($2); }
				| tROLLBACK expression ';'
					{ $$ = mParsingDriver->CreateRollback($2); }
				| tBREAK ';'
					{ $$ = mParsingDriver->CreateBreak(); }
				| tCONTINUE ';'
					{ $$ = mParsingDriver->CreateContinue(); }
				| tHALT '(' ')' ';'
					{ $$ = mParsingDriver->CreateHalt(); }
				| tYIELD '(' ')' ';'
					{ $$ = mParsingDriver->CreateYield(); }
				| tCOMPLY '(' ')' ';'
					{ $$ = mParsingDriver->CreateComply(); }
				| tREFUSE '(' ')' ';'
					{ $$ = mParsingDriver->CreateRefuse(); }
				| tPRINT '(' arg_calls ')' ';'
					{ $$ = mParsingDriver->CreatePrint($3); }
				| tREQUEST '(' expression ',' action_ref ')' ';'
					{ $$ = mParsingDriver->CreateRequest($3, $5.actor, $5.action); }
				| tAwaitStart '(' expression ',' action_ref ')' ';'
					{ $$ = mParsingDriver->CreateAwaitStart($3, $5.actor, $5.action); }
				| tAwaitCompletion '(' expression ',' action_ref ')' ';'
					{ $$ = mParsingDriver->CreateAwaitCompletion($3, $5.actor, $5.action); }
				| tJOIN '(' expression ','  expression ')' ';'
					{ $$ = mParsingDriver->CreateJoin($3, $5); }
				| block
				| declaration ';'
				| expression ';'
				| ';'
					{ $$ = nullptr; }
				| error ';'
					{ $$ = nullptr; yyerrok; yyclearin; }
				| error '}'
					{ $$ = nullptr; yyerrok; yyclearin; }
				;

expression		: left_hand '=' expression
					{ $$ = mParsingDriver->CreateAssign($1, $3); }
				| expression tAND expression
					{ $$ = mParsingDriver->CreateLogicalAnd($1, $3); }
				| expression tOR expression
					{ $$ = mParsingDriver->CreateLogicalOr($1, $3); }
				| expression tAADD expression
					{ $$ = mParsingDriver->CreateAddAndAssign($1, $3); }
				| expression tASUB expression
					{ $$ = mParsingDriver->CreateSubAndAssign($1, $3); }
				| expression tAMUL expression
					{ $$ = mParsingDriver->CreateMulAndAssign($1, $3); }
				| expression tADIV expression
					{ $$ = mParsingDriver->CreateDivAndAssign($1, $3); }
				| expression tAMOD expression
					{ $$ = mParsingDriver->CreateModAndAssign($1, $3); }
				| expression tAAND expression
					{ $$ = mParsingDriver->CreateAndAndAssign($1, $3); }
				| expression tAOR expression
					{ $$ = mParsingDriver->CreateOrAndAssign($1, $3); }
				| expression tAXOR expression
					{ $$ = mParsingDriver->CreateXorAndAssign($1, $3); }
				| expression tALSHFT expression
					{ $$ = mParsingDriver->CreateLeftShiftAndAssign($1, $3); }
				| expression tARSHFT expression
					{ $$ = mParsingDriver->CreateRightShiftAndAssign($1, $3); }
				| expression '+' expression
					{ $$ = mParsingDriver->CreateAdd($1, $3); }
				| expression '-' expression
					{ $$ = mParsingDriver->CreateSub($1, $3); }
				| expression '*' expression
					{ $$ = mParsingDriver->CreateMul($1, $3); }
				| expression '/' expression
					{ $$ = mParsingDriver->CreateDiv($1, $3); }
				| expression '%' expression
					{ $$ = mParsingDriver->CreateMod($1, $3); }
				| expression tPOW expression
					{ $$ = mParsingDriver->CreatePow($1, $3); }
				| expression '&' expression
					{ $$ = mParsingDriver->CreateAnd($1, $3); }
				| expression '|' expression
					{ $$ = mParsingDriver->CreateOr($1, $3); }
				| expression '^' expression
					{ $$ = mParsingDriver->CreateXor($1, $3); }
				| expression tLSHFT expression
					{ $$ = mParsingDriver->CreateLeftShift($1, $3); }
				| expression tRSHFT expression
					{ $$ = mParsingDriver->CreateRightShift($1, $3); }
				| expression '>' expression
					{ $$ = mParsingDriver->CreateGreater($1, $3); }
				| expression tGE expression
					{ $$ = mParsingDriver->CreateGreaterEqual($1, $3); }
				| expression '<' expression
					{ $$ = mParsingDriver->CreateLess($1, $3); }
				| expression tLE expression
					{ $$ = mParsingDriver->CreateLessEqual($1, $3); }
				| expression tEQ expression
					{ $$ = mParsingDriver->CreateEqual($1, $3); }
				| expression tNE expression
					{ $$ = mParsingDriver->CreateNotEqual($1, $3); }
				| tINC expression
					{ $$ = mParsingDriver->CreateIncrement($2); }
				| tDEC expression
					{ $$ = mParsingDriver->CreateDecrement($2); }
				| expression tINC %prec tUINC
					{ $$ = mParsingDriver->CreateIncrement($1); }
				| expression tDEC %prec tUDEC
					{ $$ = mParsingDriver->CreateDecrement($1); }
				| expression '?' expression ':' expression
					{ $$ = mParsingDriver->CreateExpressionIf($1, $3, $5); }
				| primary
				;

primary			: '-' expression %prec tUMINUS
					{ $$ = mParsingDriver->CreateNegative($2); }
				| '+' expression %prec tUPLUS
					{ $$ = $2; }
				| '!' expression
					{ $$ = mParsingDriver->CreateNot($2); }
				| '~' expression
					{ $$ = mParsingDriver->CreateComplement1($2); }
				| tSIZEOF '(' variable_type ')'
					{ $$ = mParsingDriver->CreateSizeOf($3); }
				| tIDENTIFIER '(' arg_calls ')'
					{ $$ = mParsingDriver->CreateCall($1, $3); }
				| constant
				| left_hand
				;

constant		: tFALSE
					{ $$ = mParsingDriver->CreateBool(false); }
				| tTRUE
					{ $$ = mParsingDriver->CreateBool(true); }
				| tPRIORITY
					{ $$ = mParsingDriver->CreatePriority(); }
				| tSELF
					{ $$ = mParsingDriver->CreateSelf(); }
				| tSENDER
					{ $$ = mParsingDriver->CreateSender(); }
				| tNIL
					{ $$ = mParsingDriver->CreateNil(); }
				| tDIGIT
					{ $$ = mParsingDriver->CreateInteger($1); }
				| tREAL
					{ $$ = mParsingDriver->CreateFloat($1); }
				| tSTRING
					{ $$ = mParsingDriver->CreateString($1); }
				;

left_hand		: left_hand '.' tIDENTIFIER
					{ $$ = mParsingDriver->CreateMemberVariable($1, $3); }
				| left_hand '.' tIDENTIFIER '(' arg_calls ')'
					{ $$ = mParsingDriver->CreateMemberFunction($1, $3, $5); }
				| left_hand '[' expression ']'
					{ $$ = mParsingDriver->CreateArray($1, $3); }
				| tIDENTIFIER
					{ $$ = mParsingDriver->CreateIdentifier($1); }
				| '(' expression ')'
					{ $$ = $2; }
				;

qualified_name	: tIDENTIFIER
					{ $$ = $1; }
				| qualified_name tDC tIDENTIFIER
					{ $$ = mParsingDriver->CreateQualifiedName($1, $3); }
				;

action_ref		: expression tARROW tIDENTIFIER
					{
						$$.actor = $1;
						$$.action = $3;
					}
				| qualified_name tARROW tIDENTIFIER
					{
						$$.actor = mParsingDriver->CreateIdentifier($1);
						$$.action = $3;
					}
				| qualified_name tDC tIDENTIFIER
					{
						if ($1.find("::") != std::string_view::npos)
						{
							mana::CompileError({ "legacy action reference cannot use namespace '", $1, "'" });
						}
						else
						{
							mana::CompileWarning({ "legacy action reference '", $1, "::", $3, "' is deprecated. Use '->' instead." });
						}
						$$.actor = mParsingDriver->CreateIdentifier($1);
						$$.action = $3;
						$$.legacy = true;
					}
				;

cases			: case
				| cases case
					{ $$ = mParsingDriver->BindCaseNode($1, $2); }
				;
case			: tCASE expression ':' statements
					{ $$ = mParsingDriver->CreateSwitchCaseNode($2, $4); }
				| tDEFAULT ':' statements
					{ $$ = mParsingDriver->CreateSwitchDefaultNode($3); }
				;

arg_calls		: /* empty */
					{ $$ = nullptr; }
				| expression
					{ $$ = mParsingDriver->CreateArgumentNode($1); }
				| expression ',' arg_calls
					{ $$ = mParsingDriver->CreateArgumentNode($1, $3); }
				;

arg_decls		: // empty
					{ $$ = nullptr; }
				| variable_decl
					{ $$ = mParsingDriver->CreateDeclareArgumentNode($1); }
				| arg_decls ',' variable_decl
					{ $$ = mParsingDriver->CreateDeclareArgumentNode($1, $3); }
				;


variable_decl	: variable_type declarator
					{ $$ = mParsingDriver->CreateDeclareVariableNode($1, $2, nullptr); }
				| variable_type declarator '=' expression
					{ $$ = mParsingDriver->CreateDeclareVariableNode($1, $2, $4); }
				;
				;

declarator		: tIDENTIFIER
					{ $$ = mParsingDriver->CreateDeclaratorNode($1); }
				| tIDENTIFIER variable_sizes
					{ $$ = mParsingDriver->CreateDeclaratorNode($1, $2); }
				;

variable_sizes	: variable_size
				| variable_size variable_sizes
					{ $$ = 	mParsingDriver->Bind($1, $2); }
				;
variable_size	: '[' tDIGIT ']'
					{ $$ = 	mParsingDriver->CreateVariableSizeNode($2); }
				| '[' tIDENTIFIER ']'
					{ $$ = 	mParsingDriver->CreateVariableSizeNode($2); }
				;
%%

#include "ParserDefinition.inl"
