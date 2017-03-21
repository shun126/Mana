%{
/*!
mana (compiler)

@file	mana_parser.y
@brief	意味解析に関するソースファイル
@detail	このファイルは意味解析ノードに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_COMPILER_H___)
#include "mana_compiler.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "mana_linker.h"
#endif
#if !defined(___MANA_EVALUATOR_H___)
#include "mana_evaluator.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif

#define YYERROR_VERBOSE

int yynerrs;

%}

%union {
	int digit;
	float real;
	char* string;
	mana_node* node;
	mana_symbol_entry* symbol;
	mana_type_description* type;
}

%type	<node>		block case cases left_hand constant expression statement statements variable_size variable_sizes variable_type function struct_member struct_members struct action actions actor declarator declaration allocate_declarations declarations primary arg_calls arg_decls variable_decl line

%token	<digit>		tDIGIT
%token	<real>		tREAL
%token	<string>	tSTRING tIDENTIFIER
%token	<type>		tTYPE

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
						mana_node_dump($1);
						mana_evaluator_evaluate($1);
						mana_compiler_genearte_code($1, true);
						mana_linker_resolve_address();
					}
				;
				
line			: /* empty */
				 { $$ = NULL; }
				| declarations line
					{ if ($1) { $$ = $1; $$->next = $2; } else { $$ = $2; } }
				;

declarations	: actor
				| struct
				| function
				| declaration ';'

				| tNATIVE variable_type tIDENTIFIER '(' arg_decls ')' ';'
					{ $$ = mana_node_create_declare_native_function($2, $3, $5, NULL); }

				| tALLOCATE tDIGIT '{' allocate_declarations '}' 
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_ALLOCATE, $4, NULL, NULL); $$->digit = $2; }
				| tSTATIC '{' allocate_declarations '}'
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_STATIC, $3, NULL, NULL); }
				| tSTATIC tALLOCATE tDIGIT '{' allocate_declarations '}'
					{
						mana_node* allocate_node;
						allocate_node = mana_node_create_node(MANA_NODE_DECLARE_ALLOCATE, $5, NULL, NULL);
						allocate_node->digit = $3;
						$$ = mana_node_create_node(MANA_NODE_DECLARE_STATIC, allocate_node, NULL, NULL);
					}

				| tALIAS tIDENTIFIER tSTRING ';'
					{ $$ = mana_node_create_node(MANA_NODE_DEFINE_ALIAS, NULL, NULL, NULL); $$->string = $2; }

				| tINCLUDE tSTRING ';'
					{ $$ = NULL; if(! mana_lexer_open($2, false)){ YYABORT; } }
				| tIMPORT tSTRING ';'
					{ $$ = NULL; if(! mana_lexer_open($2, true)){ YYABORT; } }
				;

allocate_declarations
				: /* empty */
					{ $$ = NULL; }
				| variable_decl ';' allocate_declarations
					{ if ($1) { $$ = $1; $$->next = $3; } else { $$ = $3; } }
				;

declaration		: variable_decl
				| variable_decl '=' expression
					{ $$ = $1; }
				| tDEFINE tIDENTIFIER tDIGIT
					{ $$ = mana_node_create_node(MANA_NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->digit = $3; }
				| tDEFINE tIDENTIFIER tREAL
					{ $$ = mana_node_create_node(MANA_NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->real = $3; }
				| tDEFINE tIDENTIFIER '-' tDIGIT	/* プリプロセッサを作る予定 */
					{ $$ = mana_node_create_node(MANA_NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->digit = -$4; }
				| tDEFINE tIDENTIFIER '-' tREAL		/* プリプロセッサを作る予定 */
					{ $$ = mana_node_create_node(MANA_NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->real = -$4; }
				| tDEFINE tIDENTIFIER tSTRING
					{ $$ = mana_node_create_node(MANA_NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $3; }
				| tDEFINE tIDENTIFIER tIDENTIFIER
					{ $$ = mana_node_create_node(MANA_NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $3; }
				| tUNDEF tIDENTIFIER
					{ $$ = mana_node_create_node(MANA_NODE_UNDEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; }
				;

actor			: tACTOR tIDENTIFIER '{' actions '}'
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_ACTOR, $4, NULL, NULL); $$->string = $2; }
				| tPHANTOM tIDENTIFIER '{' actions '}'
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_PHANTOM, $4, NULL, NULL); $$->string = $2; }
				| tMODULE tIDENTIFIER '{' actions '}'
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_MODULE, $4, NULL, NULL); $$->string = $2; }
				;

actions			: /* empty */
					{ $$ = NULL; }
				| action actions
					{ if ($1) { $$ = $1; $$->next = $2; } else { $$ = $2; } }
				;

action			: tACTION tIDENTIFIER block
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_ACTION, $3, NULL, NULL); $$->string = $2; $$->type = mana_type_get(MANA_DATA_TYPE_VOID); }
				| tEXTEND tIDENTIFIER ';'
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_EXTEND, NULL, NULL, NULL); $$->string = $2; }
				| declaration ';'
				;

struct			: tSTRUCT tIDENTIFIER '{' struct_members '}'
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_STRUCT, $4, NULL, NULL); $$->string = $2; }
				;

struct_members	: /* empty */
					{ $$ = NULL; }
				| struct_member struct_members
					{ if ($1) { $$ = $1; $$->next = $2; } else { $$ = $2; } }
				;

struct_member	: variable_decl ';'
				;

function		: variable_type tIDENTIFIER '(' arg_decls ')' block
					{ $$ = mana_node_create_declare_function($1, $2, $4, $6); }
				;

variable_type	: tACTOR2
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_DESCRIPTION, NULL, NULL, NULL); $$->type = mana_type_get(MANA_DATA_TYPE_ACTOR); }
				| tIDENTIFIER
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_DESCRIPTION, NULL, NULL, NULL); $$->string = $1; }
				| tTYPE
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_DESCRIPTION, NULL, NULL, NULL); $$->type = $1; }
				;

block			: '{' statements '}'
					{ $$ = mana_node_create_node(MANA_NODE_BLOCK, $2, NULL, NULL); }
				;

statements		: /* empty */
					{ $$ = NULL; }
				| statement statements
					{ if ($1) { $$ = $1; $$->next = $2; } else { $$ = $2; } }
				;
statement		: tIF '(' expression ')' statement
					{ $$ = mana_node_create_node(MANA_NODE_IF, $3, $5, NULL); }
				| tIF '(' expression ')' statement tELSE statement
					{ $$ = mana_node_create_node(MANA_NODE_IF, $3, $5, $7); }
				| tSWITCH '(' expression ')' '{' cases '}'
					{ $$ = mana_node_create_node(MANA_NODE_SWITCH, $3, $6, NULL); }
				| tWHILE '(' expression ')' statement
					{ $$ = mana_node_create_node(MANA_NODE_WHILE, $3, $5, NULL); }
				| tDO statement tWHILE '(' expression ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_DO, $2, $5, NULL); }
				| tFOR '(' expression ';' expression ';' expression ')' statement
					{
						$$ = mana_node_create_node(MANA_NODE_BLOCK,
							$3,
							mana_node_create_node(MANA_NODE_FOR, $5, $7, $9),
							NULL
						);
					}
				| tFOR '(' variable_decl '=' expression ';' expression ';' expression ')' statement
					{
						$$ = mana_node_create_node(MANA_NODE_BLOCK,
							mana_node_create_node(MANA_NODE_ASSIGN, $3, $5, NULL),
							mana_node_create_node(MANA_NODE_FOR, $7, $9, $11),
							NULL
						);
					}
				| tLOOP statement
					{ $$ = mana_node_create_node(MANA_NODE_LOOP, $2, NULL, NULL); }
				| tLOCK statement
					{ $$ = mana_node_create_node(MANA_NODE_LOCK, $2, NULL, NULL); }
				| GOTO tIDENTIFIER ';'
					{ $$ = mana_node_create_node(MANA_NODE_GOTO, NULL, NULL, NULL); $$->string = $2; }
				| tIDENTIFIER ':'
					{ $$ = mana_node_create_node(MANA_NODE_LABEL, NULL, NULL, NULL); $$->string = $1; }
				| tRETURN ';'
					{ $$ = mana_node_create_node(MANA_NODE_RETURN, NULL, NULL, NULL); }
				| tRETURN expression ';'
					{ $$ = mana_node_create_node(MANA_NODE_RETURN, $2, NULL, NULL); }
				| tROLLBACK expression ';'
					{ $$ = mana_node_create_node(MANA_NODE_ROLLBACK, $2, NULL, NULL); }
				| tBREAK ';'
					{ $$ = mana_node_create_node(MANA_NODE_BREAK, NULL, NULL, NULL); }
				| tCONTINUE ';'
					{ $$ = mana_node_create_node(MANA_NODE_CONTINUE, NULL, NULL, NULL); }
				| tHALT '(' ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_HALT, NULL, NULL, NULL); }
				| tYIELD '(' ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_YIELD, NULL, NULL, NULL); }
				| tCOMPLY '(' ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_COMPLY, NULL, NULL, NULL); }
				| tREFUSE '(' ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_REFUSE, NULL, NULL, NULL); }
				| tPRINT '(' arg_calls ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_PRINT, $3, NULL, NULL); }
				| tREQUEST '(' expression ','  expression tDC tIDENTIFIER ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_REQUEST, $3, $5, NULL); $$->string = $7; }
				| tJOIN '(' expression ','  expression ')' ';'
					{ $$ = mana_node_create_node(MANA_NODE_JOIN, $3, $5, NULL); }
				| block
				| declaration ';'
				| expression ';'
				| ';'
					{ $$ = NULL; }
				| error ';'
					{ $$ = NULL; yyerrok; yyclearin; }
				| error '}'
					{ $$ = NULL; yyerrok; yyclearin; }
				;

expression		: left_hand '=' expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, $3, NULL); }
				| expression tAND expression
					{ $$ = mana_node_create_node(MANA_NODE_LAND, $1, $3, NULL); }
				| expression tOR expression
					{ $$ = mana_node_create_node(MANA_NODE_LOR, $1, $3, NULL); }
				| expression tAADD expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_ADD, mana_node_clone($1), $3, NULL), NULL); }
				| expression tASUB expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_SUB, mana_node_clone($1), $3, NULL), NULL); }
				| expression tAMUL expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_MUL, mana_node_clone($1), $3, NULL), NULL); }
				| expression tADIV expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_DIV, mana_node_clone($1), $3, NULL), NULL); }
				| expression tAMOD expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_REM, mana_node_clone($1), $3, NULL), NULL); }
				| expression tAAND expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_AND, mana_node_clone($1), $3, NULL), NULL); }
				| expression tAOR expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_OR, mana_node_clone($1), $3, NULL), NULL); }
				| expression tAXOR expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_XOR, mana_node_clone($1), $3, NULL), NULL); }
				| expression tALSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_LSH, mana_node_clone($1), $3, NULL), NULL); }
				| expression tARSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_RSH, mana_node_clone($1), $3, NULL), NULL); }
				| expression '+' expression
					{ $$ = mana_node_create_node(MANA_NODE_ADD, $1, $3, NULL); }
				| expression '-' expression
					{ $$ = mana_node_create_node(MANA_NODE_SUB, $1, $3, NULL); }
				| expression '*' expression
					{ $$ = mana_node_create_node(MANA_NODE_MUL, $1, $3, NULL); }
				| expression '/' expression
					{ $$ = mana_node_create_node(MANA_NODE_DIV, $1, $3, NULL); }
				| expression '%' expression
					{ $$ = mana_node_create_node(MANA_NODE_REM, $1, $3, NULL); }
				| expression tPOW expression
					{ $$ = mana_node_create_node(MANA_NODE_POW, $1, $3, NULL); }
				| expression '&' expression
					{ $$ = mana_node_create_node(MANA_NODE_AND, $1, $3, NULL); }
				| expression '|' expression
					{ $$ = mana_node_create_node(MANA_NODE_OR, $1, $3, NULL); }
				| expression '^' expression
					{ $$ = mana_node_create_node(MANA_NODE_XOR, $1, $3, NULL); }
				| expression tLSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_LSH, $1, $3, NULL); }
				| expression tRSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_RSH, $1, $3, NULL); }
				| expression '>' expression
					{ $$ = mana_node_create_node(MANA_NODE_GT, $1, $3, NULL); }
				| expression tGE expression
					{ $$ = mana_node_create_node(MANA_NODE_GE, $1, $3, NULL); }
				| expression '<' expression
					{ $$ = mana_node_create_node(MANA_NODE_LS, $1, $3, NULL); }
				| expression tLE expression
					{ $$ = mana_node_create_node(MANA_NODE_LE, $1, $3, NULL); }
				| expression tEQ expression
					{ $$ = mana_node_create_node(MANA_NODE_EQ, $1, $3, NULL); }
				| expression tNE expression
					{ $$ = mana_node_create_node(MANA_NODE_NE, $1, $3, NULL); }
				| tINC expression
					{
						mana_node* node = mana_node_allocate(MANA_NODE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_ASSIGN, $2, mana_node_create_node(MANA_NODE_ADD, mana_node_clone($2), node, NULL), NULL);
					}
				| tDEC expression
					{
						mana_node* node = mana_node_allocate(MANA_NODE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_ASSIGN, $2, mana_node_create_node(MANA_NODE_SUB, mana_node_clone($2), node, NULL), NULL);
					}
				| expression tINC	%prec tUINC
					{
						mana_node* node = mana_node_allocate(MANA_NODE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_ADD, mana_node_clone($1), node, NULL), NULL);
					}
				| expression tDEC	%prec tUDEC
					{
						mana_node* node = mana_node_allocate(MANA_NODE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_ASSIGN, $1, mana_node_create_node(MANA_NODE_SUB, mana_node_clone($1), node, NULL), NULL);
					}
				| expression '?' expression ':' expression
					{ $$ = mana_node_create_node(MANA_NODE_EXPRESSION_IF, $3, $5, $1); }
				| primary
				;

primary			: '-' expression	%prec tUMINUS
					{ $$ = mana_node_create_node(MANA_NODE_NEG, $2, NULL, NULL); }
				| '+' expression	%prec tUPLUS
					{ $$ = $2; }
				| '!' expression
					{ $$ = mana_node_create_node(MANA_NODE_LNOT, $2, NULL, NULL); }
				| '~' expression
					{ $$ = mana_node_create_node(MANA_NODE_NOT, $2, NULL, NULL); }
				| tSIZEOF '(' variable_type ')'
					{ $$ = mana_node_create_node(MANA_NODE_SIZEOF, $3, NULL, NULL); }
				| tIDENTIFIER '(' arg_calls ')'
					{ $$ = mana_node_create_node(MANA_NODE_CALL, NULL, $3, NULL); $$->string = $1; }
				| constant
				| left_hand
				;

constant		: tFALSE
					{ $$ = mana_node_create_digit(0); }
				| tTRUE
					{ $$ = mana_node_create_digit(1); }
				| tPRIORITY
					{ $$ = mana_node_allocate(MANA_NODE_PRIORITY); $$->type = mana_type_get(MANA_DATA_TYPE_INT); }
				| tSELF
					{ $$ = mana_node_allocate(MANA_NODE_SELF); $$->type = mana_type_get(MANA_DATA_TYPE_ACTOR); $$->etc = 0; }
				| tSENDER
					{ $$ = mana_node_allocate(MANA_NODE_SENDER); $$->type = mana_type_get(MANA_DATA_TYPE_ACTOR); }
				| tNIL
					{ $$ = mana_node_allocate(MANA_NODE_CONST); $$->type = mana_type_get(MANA_DATA_TYPE_NIL); }
				| tDIGIT
					{ $$ = mana_node_create_digit($1); }
				| tREAL
					{ $$ = mana_node_create_real($1); }
				| tSTRING
					{ $$ = mana_node_create_string($1); }
				;

left_hand		: left_hand '.' tIDENTIFIER
					{ $$ = mana_node_create_node(MANA_NODE_MEMBER_VARIABLE, $1, NULL, NULL); $$->string = $3; }
				| left_hand '.' tIDENTIFIER '(' arg_calls ')'
					{ $$ = mana_node_create_node(MANA_NODE_MEMBER_FUNCTION, $1, $5, NULL); $$->string = $3; }
				| left_hand '[' expression ']'
					{ $$ = mana_node_create_node(MANA_NODE_ARRAY, $1, $3, NULL); }
				| tIDENTIFIER
					{ $$ = mana_node_create_node(MANA_NODE_IDENTIFIER, NULL, NULL, NULL); $$->string = $1; }
				| '(' expression ')'
					{ $$ = $2; }
				;

cases			: case
				| cases case
					{ $$ = $2; $$->next = $1; }
				;
case			: tCASE expression ':' statements
					{ $$ = mana_node_create_node(MANA_NODE_CASE, $2, $4, NULL); }
				| tDEFAULT ':' statements
					{ $$ = mana_node_create_node(MANA_NODE_DEFAULT, NULL, $3, NULL); }
				;


arg_calls		: /* empty */
					{ $$ = NULL; }
				| expression
					{ $$ = mana_node_create_node(MANA_NODE_CALL_ARGUMENT, $1, NULL, NULL); }
				| expression ',' arg_calls
					{ $$ = mana_node_create_node(MANA_NODE_CALL_ARGUMENT, $1, $3, NULL); }
				;

arg_decls		: /* empty */
					{ $$ = NULL; }
				| variable_decl
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_ARGUMENT, $1, NULL, NULL); }
				| arg_decls ',' variable_decl
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_ARGUMENT, $3, $1, NULL); }
				;

variable_decl	: variable_type declarator
					{ $$ = mana_node_create_node(MANA_NODE_DECLARE_VARIABLE, $1, $2, NULL); }
				;

declarator		: tIDENTIFIER
					{ $$ = mana_node_create_node(MANA_NODE_DECLARATOR, NULL, NULL, NULL); $$->string = $1; }
				| tIDENTIFIER variable_sizes
					{ $$ = mana_node_create_node(MANA_NODE_DECLARATOR, $2, NULL, NULL); $$->string = $1; }
				;

variable_sizes	: variable_size
				| variable_size variable_sizes
					{ if ($1) { $$ = $1; $$->left = $2; } else { $$ = $2; } }
				;
variable_size	: '[' tDIGIT ']'
					{ $$ = mana_node_create_node(MANA_NODE_VARIABLE_SIZE, NULL, NULL, NULL); $$->digit = $2; }
				| '[' tIDENTIFIER ']'
					{ $$ = mana_node_create_node(MANA_NODE_VARIABLE_SIZE, NULL, NULL, NULL); $$->string = $2; }
				;
%%

/*!
 * @brief	mana_print error message
 * @param	message		error message
 */
void yyerror(const char* message)
{
	mana_error(mana_lexer_get_current_filename(), mana_lexer_get_current_line(), message);
	yynerrs ++;
}

/*!
 * initialize parser
 */
void mana_parser_initialize(void)
{
}

/*!
 * shutdown parser
 */
void mana_parser_finalize(void)
{
}
