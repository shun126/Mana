%{
/*!
mana (compiler)

@file	parser.y
@brief	意味解析に関するソースファイル
@detail	このファイルは意味解析ノードに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#if !defined(___MANA_GENERATOR_H___)
#include "generator.h"
#endif
#if !defined(___MANA_LEXER_H___)
#include "lexer.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "linker.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_PRE_RESOLVER_H___)
#include "pre_resolver.h"
#endif

#define YYERROR_VERBOSE
#define YYMALLOC	mana_malloc
#define YYFREE		mana_free

int yynerrs;

%}

%union {
	int digit;
	float real;
	char* string;
	node_entry* node;
	symbol_entry* symbol;
	type_description* type;
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
						node_dump($1);
						pre_resolver_resolve($1);
						generator_genearte_code($1, true);
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
					{ $$ = node_create_declare_native_function($2, $3, $5, NULL); }

				| tALLOCATE tDIGIT '{' allocate_declarations '}' 
					{ $$ = node_create_node(NODE_DECLARE_ALLOCATE, $4, NULL, NULL); $$->digit = $2; }
				| tSTATIC '{' allocate_declarations '}'
					{ $$ = node_create_node(NODE_DECLARE_STATIC, $3, NULL, NULL); }
				| tSTATIC tALLOCATE tDIGIT '{' allocate_declarations '}'
					{
						node_entry* allocate_node;
						allocate_node = node_create_node(NODE_DECLARE_ALLOCATE, $5, NULL, NULL);
						allocate_node->digit = $3;
						$$ = node_create_node(NODE_DECLARE_STATIC, allocate_node, NULL, NULL);
					}

				| tALIAS tIDENTIFIER tSTRING ';'
					{ $$ = node_create_node(NODE_DEFINE_ALIAS, NULL, NULL, NULL); $$->string = $2; }

				| tINCLUDE tSTRING ';'
					{ $$ = NULL; if(! lexer_open($2, false)){ YYABORT; } }
				| tIMPORT tSTRING ';'
					{ $$ = NULL; if(! lexer_open($2, true)){ YYABORT; } }
				;

allocate_declarations
				: /* empty */
					{ $$ = NULL; }
				| variable_decl ';' allocate_declarations
					{ if ($1) { $$ = $1; $$->next = $3; } else { $$ = $3; } }
				;

declaration		: variable_decl
				| variable_decl '=' expression
					{
						MANA_ASSERT($1->right);
						MANA_ASSERT($1->right->string);
						MANA_ASSERT($1->next == NULL);

						node_entry* node = node_create_node(NODE_IDENTIFIER, NULL, NULL, NULL);
						node->string = $1->right->string;
						$1->next = node;

						$$ = node_create_node(NODE_ASSIGN, $1, $3, NULL);

						/*
						if($2->class_type != MANA_CLASS_TYPE_VARIABLE_LOCAL)
							mana_compile_error("can initialize variable in local space only");
						mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL);
						mana_linker_expression(node_create_node(MANA_NODE_TYPE_ASSIGN, node_create_leaf($2->name), $4), MANA_TRUE);
						*/
					}
				| tDEFINE tIDENTIFIER tDIGIT
					{ $$ = node_create_node(NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->digit = $3; }
				| tDEFINE tIDENTIFIER tREAL
					{ $$ = node_create_node(NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->real = $3; }
				| tDEFINE tIDENTIFIER '-' tDIGIT	/* プリプロセッサを作る予定 */
					{ $$ = node_create_node(NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->digit = -$4; }
				| tDEFINE tIDENTIFIER '-' tREAL		/* プリプロセッサを作る予定 */
					{ $$ = node_create_node(NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; $$->real = -$4; }
				| tDEFINE tIDENTIFIER tSTRING
					{ $$ = node_create_node(NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $3; }
				| tDEFINE tIDENTIFIER tIDENTIFIER
					{ $$ = node_create_node(NODE_DEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $3; }
				| tUNDEF tIDENTIFIER
					{ $$ = node_create_node(NODE_UNDEFINE_CONSTANT, NULL, NULL, NULL); $$->string = $2; }
				;

actor			: tACTOR tIDENTIFIER '{' actions '}'
					{ $$ = node_create_node(NODE_DECLARE_ACTOR, $4, NULL, NULL); $$->string = $2; }
				| tPHANTOM tIDENTIFIER '{' actions '}'
					{ $$ = node_create_node(NODE_DECLARE_PHANTOM, $4, NULL, NULL); $$->string = $2; }
				| tMODULE tIDENTIFIER '{' actions '}'
					{ $$ = node_create_node(NODE_DECLARE_MODULE, $4, NULL, NULL); $$->string = $2; }
				;

actions			: /* empty */
					{ $$ = NULL; }
				| action actions
					{ if ($1) { $$ = $1; $$->next = $2; } else { $$ = $2; } }
				;

action			: tACTION tIDENTIFIER block
					{ $$ = node_create_node(NODE_DECLARE_ACTION, $3, NULL, NULL); $$->string = $2; $$->type = type_get(SYMBOL_DATA_TYPE_VOID); }
				| tEXTEND tIDENTIFIER ';'
					{ $$ = node_create_node(NODE_DECLARE_EXTEND, NULL, NULL, NULL); $$->string = $2; }
				| declaration ';'
				;

struct			: tSTRUCT tIDENTIFIER '{' struct_members '}'
					{ $$ = node_create_node(NODE_DECLARE_STRUCT, $4, NULL, NULL); $$->string = $2; }
				;

struct_members	: /* empty */
					{ $$ = NULL; }
				| struct_member struct_members
					{ if ($1) { $$ = $1; $$->next = $2; } else { $$ = $2; } }
				;

struct_member	: variable_decl ';'
				;

function		: variable_type tIDENTIFIER '(' arg_decls ')' block
					{ $$ = node_create_declare_function($1, $2, $4, $6); }
				;

variable_type	: tACTOR2
					{ $$ = node_create_node(NODE_TYPE_DESCRIPTION, NULL, NULL, NULL); $$->type = type_get(SYMBOL_DATA_TYPE_ACTOR); }
				| tIDENTIFIER
					{ $$ = node_create_node(NODE_TYPE_DESCRIPTION, NULL, NULL, NULL); $$->string = $1; }
				| tTYPE
					{ $$ = node_create_node(NODE_TYPE_DESCRIPTION, NULL, NULL, NULL); $$->type = $1; }
				;

block			: '{' statements '}'
					{ $$ = node_create_node(NODE_BLOCK, $2, NULL, NULL); }
				;

statements		: /* empty */
					{ $$ = NULL; }
				| statement statements
					{ if ($1) { $$ = $1; $$->next = $2; } else { $$ = $2; } }
				;
statement		: tIF '(' expression ')' statement
					{ $$ = node_create_node(NODE_IF, $3, $5, NULL); }
				| tIF '(' expression ')' statement tELSE statement
					{ $$ = node_create_node(NODE_IF, $3, $5, $7); }
				| tSWITCH '(' expression ')' '{' cases '}'
					{ $$ = node_create_node(NODE_SWITCH, $3, $6, NULL); }
				| tWHILE '(' expression ')' statement
					{ $$ = node_create_node(NODE_WHILE, $3, $5, NULL); }
				| tDO statement tWHILE '(' expression ')' ';'
					{ $$ = node_create_node(NODE_DO, $2, $5, NULL); }
				| tFOR '(' expression ';' expression ';' expression ')' statement
					{
						$$ = node_create_node(NODE_BLOCK,
							$3,
							node_create_node(NODE_FOR, $5, $7, $9),
							NULL
						);
					}
				| tFOR '(' variable_decl '=' expression ';' expression ';' expression ')' statement
					{
						MANA_ASSERT($3->right);
						MANA_ASSERT($3->right->string);
						MANA_ASSERT($3->next == NULL);

						node_entry* node = node_create_node(NODE_IDENTIFIER, NULL, NULL, NULL);
						node->string = $3->right->string;
						$3->next = node;

						$$ = node_create_node(NODE_BLOCK,
							node_create_node(NODE_ASSIGN, $3, $5, NULL),
							node_create_node(NODE_FOR, $7, $9, $11),
							NULL
						);
					}
				| tLOOP statement
					{ $$ = node_create_node(NODE_LOOP, $2, NULL, NULL); }
				| tLOCK statement
					{ $$ = node_create_node(NODE_LOCK, $2, NULL, NULL); }
				| GOTO tIDENTIFIER ';'
					{ $$ = node_create_node(NODE_GOTO, NULL, NULL, NULL); $$->string = $2; }
				| tIDENTIFIER ':'
					{ $$ = node_create_node(NODE_LABEL, NULL, NULL, NULL); $$->string = $1; }
				| tRETURN ';'
					{ $$ = node_create_node(NODE_RETURN, NULL, NULL, NULL); }
				| tRETURN expression ';'
					{ $$ = node_create_node(NODE_RETURN, $2, NULL, NULL); }
				| tROLLBACK expression ';'
					{ $$ = node_create_node(NODE_ROLLBACK, $2, NULL, NULL); }
				| tBREAK ';'
					{ $$ = node_create_node(NODE_BREAK, NULL, NULL, NULL); }
				| tCONTINUE ';'
					{ $$ = node_create_node(NODE_CONTINUE, NULL, NULL, NULL); }
				| tHALT '(' ')' ';'
					{ $$ = node_create_node(NODE_HALT, NULL, NULL, NULL); }
				| tYIELD '(' ')' ';'
					{ $$ = node_create_node(NODE_YIELD, NULL, NULL, NULL); }
				| tCOMPLY '(' ')' ';'
					{ $$ = node_create_node(NODE_COMPLY, NULL, NULL, NULL); }
				| tREFUSE '(' ')' ';'
					{ $$ = node_create_node(NODE_REFUSE, NULL, NULL, NULL); }
				| tPRINT '(' arg_calls ')' ';'
					{ $$ = node_create_node(NODE_PRINT, $3, NULL, NULL); }
				| tREQUEST '(' expression ','  expression tDC tIDENTIFIER ')' ';'
					{ $$ = node_create_node(NODE_REQUEST, $3, $5, NULL); $$->string = $7; }
				| tJOIN '(' expression ','  expression ')' ';'
					{ $$ = node_create_node(NODE_JOIN, $3, $5, NULL); }
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
					{ $$ = node_create_node(NODE_ASSIGN, $1, $3, NULL); }
				| expression tAND expression
					{ $$ = node_create_node(NODE_LAND, $1, $3, NULL); }
				| expression tOR expression
					{ $$ = node_create_node(NODE_LOR, $1, $3, NULL); }
				| expression tAADD expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_ADD, node_clone($1), $3, NULL), NULL); }
				| expression tASUB expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_SUB, node_clone($1), $3, NULL), NULL); }
				| expression tAMUL expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_MUL, node_clone($1), $3, NULL), NULL); }
				| expression tADIV expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_DIV, node_clone($1), $3, NULL), NULL); }
				| expression tAMOD expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_REM, node_clone($1), $3, NULL), NULL); }
				| expression tAAND expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_AND, node_clone($1), $3, NULL), NULL); }
				| expression tAOR expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_OR, node_clone($1), $3, NULL), NULL); }
				| expression tAXOR expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_XOR, node_clone($1), $3, NULL), NULL); }
				| expression tALSHFT expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_LSH, node_clone($1), $3, NULL), NULL); }
				| expression tARSHFT expression
					{ $$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_RSH, node_clone($1), $3, NULL), NULL); }
				| expression '+' expression
					{ $$ = node_create_node(NODE_ADD, $1, $3, NULL); }
				| expression '-' expression
					{ $$ = node_create_node(NODE_SUB, $1, $3, NULL); }
				| expression '*' expression
					{ $$ = node_create_node(NODE_MUL, $1, $3, NULL); }
				| expression '/' expression
					{ $$ = node_create_node(NODE_DIV, $1, $3, NULL); }
				| expression '%' expression
					{ $$ = node_create_node(NODE_REM, $1, $3, NULL); }
				| expression tPOW expression
					{ $$ = node_create_node(NODE_POW, $1, $3, NULL); }
				| expression '&' expression
					{ $$ = node_create_node(NODE_AND, $1, $3, NULL); }
				| expression '|' expression
					{ $$ = node_create_node(NODE_OR, $1, $3, NULL); }
				| expression '^' expression
					{ $$ = node_create_node(NODE_XOR, $1, $3, NULL); }
				| expression tLSHFT expression
					{ $$ = node_create_node(NODE_LSH, $1, $3, NULL); }
				| expression tRSHFT expression
					{ $$ = node_create_node(NODE_RSH, $1, $3, NULL); }
				| expression '>' expression
					{ $$ = node_create_node(NODE_GT, $1, $3, NULL); }
				| expression tGE expression
					{ $$ = node_create_node(NODE_GE, $1, $3, NULL); }
				| expression '<' expression
					{ $$ = node_create_node(NODE_LS, $1, $3, NULL); }
				| expression tLE expression
					{ $$ = node_create_node(NODE_LE, $1, $3, NULL); }
				| expression tEQ expression
					{ $$ = node_create_node(NODE_EQ, $1, $3, NULL); }
				| expression tNE expression
					{ $$ = node_create_node(NODE_NE, $1, $3, NULL); }
				| tINC expression
					{
						node_entry* node = node_allocate(NODE_CONST);
						node->digit = 1;
						node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
						$$ = node_create_node(NODE_ASSIGN, $2, node_create_node(NODE_ADD, node_clone($2), node, NULL), NULL);
					}
				| tDEC expression
					{
						node_entry* node = node_allocate(NODE_CONST);
						node->digit = 1;
						node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
						$$ = node_create_node(NODE_ASSIGN, $2, node_create_node(NODE_SUB, node_clone($2), node, NULL), NULL);
					}
				| expression tINC	%prec tUINC
					{
						node_entry* node = node_allocate(NODE_CONST);
						node->digit = 1;
						node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
						$$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_ADD, node_clone($1), node, NULL), NULL);
					}
				| expression tDEC	%prec tUDEC
					{
						node_entry* node = node_allocate(NODE_CONST);
						node->digit = 1;
						node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
						$$ = node_create_node(NODE_ASSIGN, $1, node_create_node(NODE_SUB, node_clone($1), node, NULL), NULL);
					}
				| expression '?' expression ':' expression
					{ $$ = node_create_node(NODE_EXPRESSION_IF, $3, $5, $1); }
				| primary
				;

primary			: '-' expression	%prec tUMINUS
					{ $$ = node_create_node(NODE_NEG, $2, NULL, NULL); }
				| '+' expression	%prec tUPLUS
					{ $$ = $2; }
				| '!' expression
					{ $$ = node_create_node(NODE_LNOT, $2, NULL, NULL); }
				| '~' expression
					{ $$ = node_create_node(NODE_NOT, $2, NULL, NULL); }
				| tSIZEOF '(' variable_type ')'
					{ $$ = node_create_node(NODE_SIZEOF, $3, NULL, NULL); }
				| tIDENTIFIER '(' arg_calls ')'
					{ $$ = node_create_node(NODE_CALL, NULL, $3, NULL); $$->string = $1; }
				| constant
				| left_hand
				;

constant		: tFALSE
					{ $$ = node_create_digit(0); }
				| tTRUE
					{ $$ = node_create_digit(1); }
				| tPRIORITY
					{ $$ = node_allocate(NODE_PRIORITY); $$->type = type_get(SYMBOL_DATA_TYPE_INT); }
				| tSELF
					{ $$ = node_allocate(NODE_SELF); $$->type = type_get(SYMBOL_DATA_TYPE_ACTOR); $$->etc = 0; }
				| tSENDER
					{ $$ = node_allocate(NODE_SENDER); $$->type = type_get(SYMBOL_DATA_TYPE_ACTOR); }
				| tNIL
					{ $$ = node_allocate(NODE_CONST); $$->type = type_get(SYMBOL_DATA_TYPE_NIL); }
				| tDIGIT
					{ $$ = node_create_digit($1); }
				| tREAL
					{ $$ = node_create_real($1); }
				| tSTRING
					{ $$ = node_create_string($1); }
				;

left_hand		: left_hand '.' tIDENTIFIER
					{ $$ = node_create_node(NODE_MEMBER_VARIABLE, $1, NULL, NULL); $$->string = $3; }
				| left_hand '.' tIDENTIFIER '(' arg_calls ')'
					{ $$ = node_create_node(NODE_MEMBER_FUNCTION, $1, $5, NULL); $$->string = $3; }
				| left_hand '[' expression ']'
					{ $$ = node_create_node(NODE_ARRAY, $1, $3, NULL); }
				| tIDENTIFIER
					{ $$ = node_create_node(NODE_IDENTIFIER, NULL, NULL, NULL); $$->string = $1; }
				| '(' expression ')'
					{ $$ = $2; }
				;

cases			: case
				| cases case
					{ $$ = $2; $$->next = $1; }
				;
case			: tCASE expression ':' statements
					{ $$ = node_create_node(NODE_CASE, $2, $4, NULL); }
				| tDEFAULT ':' statements
					{ $$ = node_create_node(NODE_DEFAULT, NULL, $3, NULL); }
				;


arg_calls		: /* empty */
					{ $$ = NULL; }
				| expression
					{ $$ = node_create_node(NODE_CALL_ARGUMENT, $1, NULL, NULL); }
				| expression ',' arg_calls
					{ $$ = node_create_node(NODE_CALL_ARGUMENT, $1, $3, NULL); }
				;

arg_decls		: /* empty */
					{ $$ = NULL; }
				| variable_decl
					{ $$ = node_create_node(NODE_DECLARE_ARGUMENT, $1, NULL, NULL); }
				| arg_decls ',' variable_decl
					{ $$ = node_create_node(NODE_DECLARE_ARGUMENT, $3, $1, NULL); }
				;

variable_decl	: variable_type declarator
					{ $$ = node_create_node(NODE_DECLARE_VARIABLE, $1, $2, NULL); }
				;

declarator		: tIDENTIFIER
					{ $$ = node_create_node(NODE_DECLARATOR, NULL, NULL, NULL); $$->string = $1; }
				| tIDENTIFIER variable_sizes
					{ $$ = node_create_node(NODE_DECLARATOR, $2, NULL, NULL); $$->string = $1; }
				;

variable_sizes	: variable_size
				| variable_size variable_sizes
					{ if ($1) { $$ = $1; $$->left = $2; } else { $$ = $2; } }
				;
variable_size	: '[' tDIGIT ']'
					{ $$ = node_create_node(NODE_VARIABLE_SIZE, NULL, NULL, NULL); $$->digit = $2; }
				| '[' tIDENTIFIER ']'
					{ $$ = node_create_node(NODE_VARIABLE_SIZE, NULL, NULL, NULL); $$->string = $2; }
				;
%%

/*!
 * @brief	MANA_PRINT error message
 * @param	message		error message
 */
void yyerror(const char* message)
{
#if defined(_MSC_VER)
	MANA_PRINT("%s(%d): error: %s\n", lexer_get_current_filename(), lexer_get_current_line(), message);
#else
	MANA_PRINT("%s:%d: error: %s\n", lexer_get_current_filename(), lexer_get_current_line(), message);
#endif
	yynerrs ++;
}

/*!
 * initialize parser
 */
void parser_initialize(void)
{
}

/*!
 * shutdown parser
 */
void parser_finalize(void)
{
}
