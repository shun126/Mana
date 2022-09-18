%{
/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Main.h"
#include "Symbol.h"
#include "TypeDescriptor.h"
//#include "generator.h"
#include "Lexer.h"
//#include "linker.h"
//#include "main.h"
//#include "pre_resolver.h"

#include "SyntaxNode.h"
#include "Parser.hpp"

#include "ParserDeclaration.inl"
#include <memory>
#include <string_view>

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
#define YYERROR_VERBOSE
#endif

%}

%require "3.2"
%language "c++"

%define api.parser.class {Parser}
%define api.namespace {mana}
%define api.value.type variant

%type	<std::shared_ptr<mana::SyntaxNode>> block case cases left_hand constant expression statement statements variable_size variable_sizes variable_type function struct_member struct_members struct action actions actor declarator declaration allocate_declarations declarations primary arg_calls arg_decls variable_decl line
%token	<mana::int_t> tDIGIT
%token	<mana::float_t> tREAL
%token	<std::string_view> tSTRING tIDENTIFIER
%token	<std::shared_ptr<mana::TypeDescriptor>> tTYPE

%token	tALIAS tDEFINE tUNDEF tINCLUDE tIMPORT
%token	tNATIVE tSTRUCT tACTOR tACTOR2 tPHANTOM tACTION tMODULE tEXTEND
%token	tFALSE tTRUE tPRIORITY tSELF tSENDER tNIL
%token	tREQUEST tJOIN
%token	tBREAK
%token	tCONTINUE
%token	tCASE
%token	tDEFAULT
%token	GOTO
%token	tHALT
%token	tLOCK
%token	tDC
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
						if ($1)
						{
							$1->Dump();
						}

						auto globalSemanticAnalyzer = mana::GetSystemHolder().GetGlobalSemanticAnalyzer();
						globalSemanticAnalyzer->Resolve($1);

						auto codeGenerator = mana::GetSystemHolder().GetCodeGenerator();
						codeGenerator->generator_genearte_code($1, true);

						auto globalAddressResolver = mana::GetSystemHolder().GetCodeGenerator()->GetGlobalAddressResolver();
						globalAddressResolver->ResolveAddress();
						//globalAddressResolver->mana_linker_resolve_address();
					}
				;
				
line			: // empty
					{ $$ = nullptr; }
				| declarations line
					{ $$ = mana::Bind($1, $2); }
				;

declarations	: actor
				| struct
				| function
				| declaration ';'

				| tNATIVE variable_type tIDENTIFIER '(' arg_decls ')' ';'
					{ $$ = mana::CreateNativeFunction($2, $3, $5); }

				| tALLOCATE tDIGIT '{' allocate_declarations '}' 
					{ $$ = mana::CreateDeclareMemory($2, $4); }
				| tSTATIC '{' allocate_declarations '}'
					{ $$ = mana::CreateDeclareStaticMemory(0, $3); }
				| tSTATIC tALLOCATE tDIGIT '{' allocate_declarations '}'
					{ $$ = mana::CreateDeclareStaticMemory($3, $5); }

				| tALIAS tIDENTIFIER tSTRING ';'
					{ $$ = mana::CreateAlias($2, $3); }

				| tINCLUDE tSTRING ';'
					{ $$ = nullptr; if(! lexer_open($2, false)){ YYABORT; } }
				| tIMPORT tSTRING ';'
					{ $$ = nullptr; if(! lexer_open($2, true)){ YYABORT; } }
				;

allocate_declarations
				: // empty
					{ $$ = nullptr; }
				| variable_decl ';' allocate_declarations
					{ $$ = mana::Bind($1, $3); }
				;

declaration		: variable_decl
				| tDEFINE tIDENTIFIER tDIGIT
					{ $$ = mana::CreateConstantNode($2, $3); }
				| tDEFINE tIDENTIFIER tREAL
					{ $$ = mana::CreateConstantNode($2, $3); }
				| tDEFINE tIDENTIFIER '-' tDIGIT
					{ $$ = mana::CreateConstantNode($2, -$4); }
				| tDEFINE tIDENTIFIER '-' tREAL
					{ $$ = mana::CreateConstantNode($2, -$4); }
				| tDEFINE tIDENTIFIER tSTRING
					{ $$ = CreateConstantNode($2, $3); }
				| tDEFINE tIDENTIFIER tIDENTIFIER
					{ $$ = CreateDefineNode($2, $3); }
				| tUNDEF tIDENTIFIER
					{ $$ = CreateUndefineNode($2); }
				;

actor			: tACTOR tIDENTIFIER '{' actions '}'
					{ $$ = mana::CreateActor($2, $4); }
				| tPHANTOM tIDENTIFIER '{' actions '}'
					{ $$ = mana::CreatePhantom($2, $4); }
				| tMODULE tIDENTIFIER '{' actions '}'
					{ $$ = mana::CreateModule($2, $4); }
				;

actions			: // empty
					{ $$ = nullptr; }
				| action actions
					{ $$ = mana::Bind($1, $2); }
				;

action			: tACTION tIDENTIFIER block
					{ $$ = mana::CreateAction($2, $3); }
				| tEXTEND tIDENTIFIER ';'
					{ $$ = mana::CreateExtend($2); }
				| declaration ';'
				;

struct			: tSTRUCT tIDENTIFIER '{' struct_members '}'
					{ $$ = mana::CreateStruct($2, $4); }
				;

struct_members	: // empty
					{ $$ = nullptr; }
				| struct_member struct_members
					{ $$ = mana::Bind($1, $2); }
				;

struct_member	: variable_decl ';'
				;

function		: variable_type tIDENTIFIER '(' arg_decls ')' block
					{ $$ = mana::CreateInternalFunction($1, $2, $4, $6); }
				;

variable_type	: tACTOR2
					{ $$ = mana::CreateActorTypeDescription(); }
				| tIDENTIFIER
					{ $$ = mana::CreateTypeDescription($1); }
				| tTYPE
					{ $$ = mana::CreateTypeDescription($1); }
				;

block			: '{' statements '}'
					{ $$ = mana::CreateBlock($2); }
				;

statements		: // empty
					{ $$ = nullptr; }
				| statement statements
					{ $$ = mana::Bind($1, $2); }
				;
statement		: tIF '(' expression ')' statement
					{ $$ = mana::CreateIf($3, $5, nullptr); }
				| tIF '(' expression ')' statement tELSE statement
					{ $$ = mana::CreateIf($3, $5, $7); }
				| tSWITCH '(' expression ')' '{' cases '}'
					{ $$ = mana::CreateSwitch($3, $6); }
				| tWHILE '(' expression ')' statement
					{ $$ = mana::CreateWhile($3, $5); }
				| tDO statement tWHILE '(' expression ')' ';'
					{ $$ = mana::CreateDoWhile($2, $5); }
				| tFOR '(' expression ';' expression ';' expression ')' statement
					{ $$ = mana::CreateFor($3, $5, $7, $9); }
				| tFOR '(' variable_decl ';' expression ';' expression ')' statement
					{ $$ = mana::CreateFor($3, $5, $7, $9); }
				| tLOOP statement
					{ $$ = mana::CreateLoop($2); }
				| tLOCK statement
					{ $$ = mana::CreateLock($2); }
				| GOTO tIDENTIFIER ';'
					{ $$ = mana::CreateGoto($2); }
				| tIDENTIFIER ':'
					{ $$ = mana::CreateLabel($1); }
				| tRETURN ';'
					{ $$ = mana::CreateReturn(nullptr); }
				| tRETURN expression ';'
					{ $$ = mana::CreateReturn($2); }
				| tROLLBACK expression ';'
					{ $$ = mana::CreateRollback($2); }
				| tBREAK ';'
					{ $$ = mana::CreateBreak(); }
				| tCONTINUE ';'
					{ $$ = mana::CreateContinue(); }
				| tHALT '(' ')' ';'
					{ $$ = mana::CreateHalt(); }
				| tYIELD '(' ')' ';'
					{ $$ = mana::CreateYield(); }
				| tCOMPLY '(' ')' ';'
					{ $$ = mana::CreateComply(); }
				| tREFUSE '(' ')' ';'
					{ $$ = mana::CreateRefuse(); }
				| tPRINT '(' arg_calls ')' ';'
					{ $$ = mana::CreatePrint($3); }
				| tREQUEST '(' expression ','  expression tDC tIDENTIFIER ')' ';'
					{ $$ = mana::CreateRequest($3, $5, $7); }
				| tJOIN '(' expression ','  expression ')' ';'
					{ $$ = mana::CreateJoin($3, $5); }
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
					{ $$ = mana::CreateAssign($1, $3); }
				| expression tAND expression
					{ $$ = mana::CreateLogicalAnd($1, $3); }
				| expression tOR expression
					{ $$ = mana::CreateLogicalOr($1, $3); }
				| expression tAADD expression
					{ $$ = mana::CreateAddAndAssign($1, $3); }
				| expression tASUB expression
					{ $$ = mana::CreateSubAndAssign($1, $3); }
				| expression tAMUL expression
					{ $$ = mana::CreateMulAndAssign($1, $3); }
				| expression tADIV expression
					{ $$ = mana::CreateDivAndAssign($1, $3); }
				| expression tAMOD expression
					{ $$ = mana::CreateModAndAssign($1, $3); }
				| expression tAAND expression
					{ $$ = mana::CreateAndAndAssign($1, $3); }
				| expression tAOR expression
					{ $$ = mana::CreateOrAndAssign($1, $3); }
				| expression tAXOR expression
					{ $$ = mana::CreateXorAndAssign($1, $3); }
				| expression tALSHFT expression
					{ $$ = mana::CreateLeftShiftAndAssign($1, $3); }
				| expression tARSHFT expression
					{ $$ = mana::CreateRightShiftAndAssign($1, $3); }
				| expression '+' expression
					{ $$ = mana::CreateAdd($1, $3); }
				| expression '-' expression
					{ $$ = mana::CreateSub($1, $3); }
				| expression '*' expression
					{ $$ = mana::CreateMul($1, $3); }
				| expression '/' expression
					{ $$ = mana::CreateDiv($1, $3); }
				| expression '%' expression
					{ $$ = mana::CreateMod($1, $3); }
				| expression tPOW expression
					{ $$ = mana::CreatePow($1, $3); }
				| expression '&' expression
					{ $$ = mana::CreateAnd($1, $3); }
				| expression '|' expression
					{ $$ = mana::CreateOr($1, $3); }
				| expression '^' expression
					{ $$ = mana::CreateXor($1, $3); }
				| expression tLSHFT expression
					{ $$ = mana::CreateLeftShift($1, $3); }
				| expression tRSHFT expression
					{ $$ = mana::CreateRightShift($1, $3); }
				| expression '>' expression
					{ $$ = mana::CreateGT($1, $3); }
				| expression tGE expression
					{ $$ = mana::CreateGE($1, $3); }
				| expression '<' expression
					{ $$ = mana::CreateLS($1, $3); }
				| expression tLE expression
					{ $$ = mana::CreateLE($1, $3); }
				| expression tEQ expression
					{ $$ = mana::CreateEQ($1, $3); }
				| expression tNE expression
					{ $$ = mana::CreateNE($1, $3); }
				| tINC expression
					{ $$ = mana::CreateIncrement($2); }
				| tDEC expression
					{ $$ = mana::CreateDecrement($2); }
				| expression tINC %prec tUINC
					{ $$ = mana::CreateIncrement($1); }
				| expression tDEC %prec tUDEC
					{ $$ = mana::CreateDecrement($1); }
				| expression '?' expression ':' expression
					{ $$ = mana::CreateExpressionIf($1, $3, $5); }
				| primary
				;

primary			: '-' expression %prec tUMINUS
					{ $$ = mana::CreateNegative($2); }
				| '+' expression %prec tUPLUS
					{ $$ = $2; }
				| '!' expression
					{ $$ = mana::CreateNot($2); }
				| '~' expression
					{ $$ = mana::CreateComplement1($2); }
				| tSIZEOF '(' variable_type ')'
					{ $$ = mana::CreateSizeOf($3); }
				| tIDENTIFIER '(' arg_calls ')'
					{ $$ = mana::CreateCall($1, $3); }
				| constant
				| left_hand
				;

constant		: tFALSE
					{ $$ = mana::CreateInteger(0); }
				| tTRUE
					{ $$ = mana::CreateInteger(1); }
				| tPRIORITY
					{ $$ = mana::CreatePriority(); }
				| tSELF
					{ $$ = mana::CreateSelf(); }
				| tSENDER
					{ $$ = mana::CreateSender(); }
				| tNIL
					{ $$ = mana::CreateNil(); }
				| tDIGIT
					{ $$ = mana::CreateInteger($1); }
				| tREAL
					{ $$ = mana::CreateFloat($1); }
				| tSTRING
					{ $$ = mana::CreateString($1); }
				;

left_hand		: left_hand '.' tIDENTIFIER
					{ $$ = mana::CreateMemberVariable($1, $3); }
				| left_hand '.' tIDENTIFIER '(' arg_calls ')'
					{ $$ = mana::CreateMemberFunction($1, $3, $5); }
				| left_hand '[' expression ']'
					{ $$ = mana::CreateArray($1, $3); }
				| tIDENTIFIER
					{ $$ = mana::CreateIdentifier($1); }
				| '(' expression ')'
					{ $$ = $2; }
				;

cases			: case
				| cases case
					{ $$ = mana::BindCaseNode($1, $2); }
				;
case			: tCASE expression ':' statements
					{ $$ = mana::CreateSwitchCaseNode($2, $4); }
				| tDEFAULT ':' statements
					{ $$ = mana::CreateSwitchDefaultNode($3); }
				;

arg_calls		: /* empty */
					{ $$ = nullptr; }
				| expression
					{ $$ = mana::CreateArgumentNode($1); }
				| expression ',' arg_calls
					{ $$ = mana::CreateArgumentNode($1, $3); }
				;

arg_decls		: // empty
					{ $$ = nullptr; }
				| variable_decl
					{ $$ = mana::CreateDeclareArgumentNode($1); }
				| arg_decls ',' variable_decl
					{ $$ = mana::CreateDeclareArgumentNode($1, $3); }
				;


variable_decl	: variable_type declarator
					{ $$ = mana::CreateDeclareVariableNode($1, $2, nullptr); }
				| variable_type declarator '=' expression
					{ $$ = mana::CreateDeclareVariableNode($1, $2, $4); }
				;
				;

declarator		: tIDENTIFIER
					{ $$ = mana::CreateDeclaratorNode($1); }
				| tIDENTIFIER variable_sizes
					{ $$ = mana::CreateDeclaratorNode($1, $2); }
				;

variable_sizes	: variable_size
				| variable_size variable_sizes
					{ $$ = 	mana::Bind($1, $2); }
				;
variable_size	: '[' tDIGIT ']'
					{ $$ = 	mana::CreateVariableSizeNode($2); }
				| '[' tIDENTIFIER ']'
					{ $$ = 	mana::CreateVariableSizeNode($2); }
				;
%%

#include "ParserDefinition.inl"
