%{
/*
 * mana (compiler)
 *
 * @file	mana_parser.y
 * @brief	意味解析に関するソースファイル
 * @detail	このファイルは意味解析ノードに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "mana_jump.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "mana_linker.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_NODE_H___)
#include "mana_node.h"
#endif
#if !defined(___MANA_REGISTER_H___)
#include "mana_register.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYERROR_VERBOSE

static mana_symbol_entry* mana_actor_symbol_entry_pointer;
static mana_symbol_entry* mana_function_symbol_entry_pointer;
static int mana_allocated_size;
static int mana_static_block_opend;
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

%type	<digit>		decl_args for_prefix for_initialize if_prefix
%type	<node>		call_args constant expression left_hand primary
%type	<symbol>	declarator function_header
%type	<type>		variable_size variable_sizes variable_type

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
program			: declarations
				;

declarations	: /* empty */
				| declarations declaration ';'
				| declarations actor
				| declarations struct
				| declarations function
				| declarations tALIAS tIDENTIFIER tSTRING ';'
					{ mana_symbol_create_alias($3, $4); }
				| declarations tINCLUDE tSTRING ';'
					{ if(! mana_lexer_open($3, false)){ YYABORT; } }
				| declarations tIMPORT tSTRING ';'
					{ if(! mana_lexer_open($3, true)){ YYABORT; } }
				| declarations tNATIVE variable_type function_header ';'
					{ mana_symbol_set_native_function($4, $3); }
				| declarations tSTATIC tALLOCATE tDIGIT
					{
						mana_static_block_opend = true;
						mana_allocated_size = mana_symbol_get_static_memory_address() + $4;
					}
				  '{' allocate_declarations '}'
					{
						int address = mana_symbol_get_static_memory_address();
						if(address >= mana_allocated_size)
						{
							mana_compile_error("static variable range over");
						}
						mana_symbol_set_static_memory_address(mana_allocated_size);
						mana_static_block_opend = false;
					}
				| declarations tSTATIC
					{ mana_static_block_opend = true; }
				  '{' allocate_declarations '}'
					{ mana_static_block_opend = false; }
				;

allocate_declarations
				: /* empty */
				| allocate_declarations declaration ';'
				;

declaration		: variable_type declarator
					{ mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL); }
				| variable_type declarator '=' expression
					{
						if($2->class_type != MANA_CLASS_TYPE_VARIABLE_LOCAL)
							mana_compile_error("can initialize variable in local space only");
						mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL);
						mana_linker_expression(mana_node_create_node(MANA_NODE_TYPE_ASSIGN, mana_node_create_leaf($2->name), $4), true);
					}
				| variable_type function_header
					{ mana_symbol_create_prototype($2, $1); }
				| tDEFINE tIDENTIFIER tDIGIT
					{ mana_symbol_create_const_int($2, $3); }
				| tDEFINE tIDENTIFIER tREAL
					{ mana_symbol_create_const_float($2, $3); }
				| tDEFINE tIDENTIFIER '-' tDIGIT	/* プリプロセッサを作る予定 */
					{ mana_symbol_create_const_int($2, - $4); }
				| tDEFINE tIDENTIFIER '-' tREAL		/* プリプロセッサを作る予定 */
					{ mana_symbol_create_const_float($2, - $4); }
				| tDEFINE tIDENTIFIER tSTRING
					{ mana_symbol_create_const_string($2, $3); }
				| tDEFINE tIDENTIFIER tIDENTIFIER
					{
						mana_symbol_entry* symbol = mana_symbol_lookup($3);
						if(symbol)
						{
							switch(symbol->class_type)
							{
							case MANA_CLASS_TYPE_CONSTANT_INT:
								mana_symbol_create_const_int($2, symbol->address);
								break;
							case MANA_CLASS_TYPE_CONSTANT_FLOAT:
								mana_symbol_create_const_float($2, symbol->hloat);
								break;
							case MANA_CLASS_TYPE_CONSTANT_STRING:
								mana_symbol_create_const_string($2, symbol->string);
								break;
							case MANA_CLASS_TYPE_ALIAS:
								mana_symbol_create_alias($2, symbol->string);
								break;
							default:
								mana_compile_error("invalid identifier used as a define name");
								break;
							}
						}else{
							mana_compile_error("identifier %s is not defined", $3);
						}
					}
				| tUNDEF tIDENTIFIER
					{ mana_symbol_destroy($2); }
				;

declarator		: tIDENTIFIER
					{ $$ = mana_symbol_create_identification($1, NULL, mana_static_block_opend); }
				| tIDENTIFIER variable_sizes
					{ $$ = mana_symbol_create_identification($1, $2, mana_static_block_opend); }
				;

actor			: tACTOR tIDENTIFIER
					{
						mana_actor_symbol_entry_pointer = mana_symbol_lookup($2);
						mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
					}
				  '{' actions '}'
					{
						mana_symbol_set_type($2, mana_symbol_close_actor($2, NULL, NULL, false));
						mana_actor_symbol_entry_pointer = NULL;
					}

				| tPHANTOM tIDENTIFIER
					{
						mana_actor_symbol_entry_pointer = mana_symbol_lookup($2);
						mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
					}
				  '{' actions '}'
					{
						mana_symbol_set_type($2, mana_symbol_close_actor($2, NULL, NULL, true));
						mana_actor_symbol_entry_pointer = NULL;
					}

				| tMODULE tIDENTIFIER
					{
						mana_symbol_open_module();
					}
				  '{' actions '}'
					{
						mana_symbol_set_type($2, mana_symbol_close_module($2));
					}
				;

actions			: /* empty */
				| actions action
				;

action			: tACTION tIDENTIFIER
					{
						mana_function_symbol_entry_pointer = mana_symbol_create_function($2);
						mana_symbol_open_function(true, mana_function_symbol_entry_pointer, mana_type_get(MANA_DATA_TYPE_VOID));
					}
				  block
					{
						mana_symbol_close_function(mana_function_symbol_entry_pointer);
					}
				| declaration ';'
				| tINCLUDE tSTRING ';'
					{ if(! mana_lexer_open($2, false)){ YYABORT; } }
				| tIMPORT tSTRING ';'
					{ if(! mana_lexer_open($2, true)){ YYABORT; } }
				| tEXTEND tIDENTIFIER ';'
					{ mana_symbol_extend_module($2); }
				;

struct			: tSTRUCT
					{
						mana_symbol_open_structure();
					}
				  tIDENTIFIER '{' struct_members '}'
					{
						mana_symbol_set_type($3, mana_symbol_close_structure($3));
					}
				;

struct_members	: /* empty */
				| struct_members struct_member
				;

struct_member	: variable_type declarator ';'
					{ mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL); }
				;

function		: variable_type function_header
					{
						mana_symbol_open_function(false, $2, $1);
						mana_function_symbol_entry_pointer = $2;
					}
				  block
					{ mana_symbol_close_function($2); }
				;

function_header	: tIDENTIFIER
					{ $<symbol>$ = mana_symbol_create_function($1); }
				  '(' decl_args ')'
					{
						$<symbol>$ = $<symbol>2;
						$<symbol>$->number_of_parameters = $4;
					}

variable_type	: tACTOR2
					{ $$ = mana_type_get(MANA_DATA_TYPE_ACTOR); }
				| tIDENTIFIER
					{
						mana_symbol_entry* symbol = mana_symbol_lookup($1);
						if(symbol)
						{
							$$ = symbol->type;
						}
						else
						{
							mana_compile_error("incomplete type name %s", $1);
							$$ = mana_type_get(MANA_DATA_TYPE_INT);
						}
					}
				| tTYPE
				;

variable_sizes	: variable_size
				| variable_size variable_sizes
					{
						assert($1->component != $2);
						$1->component = $2;
					}
				;
variable_size	: '[' tDIGIT ']'
					{ $$ = mana_type_create_array($2); }
				| '[' tIDENTIFIER ']'
					{
						mana_symbol_entry* symbol = mana_symbol_lookup($2);
						if(symbol)
						{
							if(symbol->class_type == MANA_CLASS_TYPE_CONSTANT_INT)
							{
								$$ = mana_type_create_array(symbol->address);
							}else{
								mana_compile_error("invalid size information on parameter");
								$$ = 0;
							}
						}else{
							mana_compile_error("identifier %s is not defined", $2);
							$$ = 0;
						}
					}
				;

block			: '{'
					{ mana_symbol_open_block(false); }
				   statements '}'
					{ mana_symbol_close_block(); }
				;

for_prefix		: tFOR '('
					{ mana_symbol_open_block(false); }
				  for_initialize
					{ $$ = $4; }
				;

for_initialize	: expression ';' expression ';'
					{	/* 'for(variable = expression' の形式 */
						mana_linker_expression($1, true);
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
						$$ = mana_code_get_pc();
						mana_jump_break(mana_linker_condition($3, true));
					}
				| variable_type declarator '=' expression ';'
					{	/* 'for(type variable = expression' の形式 */
						mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_NORMAL);
						mana_linker_expression(mana_node_create_node(MANA_NODE_TYPE_ASSIGN, mana_node_create_leaf($2->name), $4), true);
					}
				  expression ';'
					{
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
						$$ = mana_code_get_pc();
						mana_jump_break(mana_linker_condition($7, true));
					}
				;
if_prefix		: tIF '(' expression ')'
					{ $<digit>$ = mana_linker_condition($3, true); }
				  statement
					{ $$ = $<digit>5; }
				;

statements		: /* empty */
				| statements statement
				;
statement		: if_prefix
					{ mana_code_replace_all($1, mana_code_get_pc()); }
				| if_prefix tELSE
					{
						$<digit>$ = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
						mana_code_replace_all($<digit>1, mana_code_get_pc());
					}
				  statement
					{ mana_code_replace_all($<digit>3, mana_code_get_pc()); }
				| tSWITCH '(' expression ')'
					{
						mana_type_description* type = $3->type;

						mana_linker_expression($3, false);
						$<digit>$ = mana_code_get_pc();
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_SWITCH);
						mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
						mana_jump_open_switch(type);
					}
				  '{' case_block '}'
					{
						mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc()));
						mana_code_replace_all($<digit>5, mana_code_get_pc());
						mana_jump_switch_build();
						mana_jump_close_chain();
						mana_jump_close_switch();
					}
				| tWHILE
					{
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_WHILE);
					}
				  '(' expression ')'
					{ mana_jump_break(mana_linker_condition($4, true)); }
				  statement
					{
						mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
						mana_jump_close_chain();
					}
				| tDO
					{
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_DO);
						$<digit>$ = mana_code_get_pc();
					}
				  statement tWHILE '(' expression ')' ';'
					{
						mana_jump_close_continue_only();
						mana_code_replace_all(mana_linker_condition($6, false), $<digit>2);
						mana_jump_close_chain();
					}
				| for_prefix expression ')' statement
					{
						mana_jump_close_continue_only();
						mana_linker_expression($2, true);
						mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
						mana_jump_close_chain();
						mana_symbol_close_block();
					}
				| tLOOP
					{
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOOP);
						$<digit>$ = mana_code_get_pc();
					}
				  statement
					{
						mana_code_set_opecode_and_operand(MANA_IL_BRA, $<digit>2);
						mana_jump_close_chain();
					}
				| tLOCK
					{
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOCK);
						mana_code_set_opecode(MANA_IL_NONPREEMPTIVE);
					}
				  statement
					{
						mana_jump_close_chain();
						mana_code_set_opecode(MANA_IL_PREEMPTIVE);
					}
				| GOTO tIDENTIFIER ';'
					{
						mana_symbol_entry* symbol = mana_symbol_create_label($2);
						symbol->etc = mana_code_set_opecode_and_operand(MANA_IL_BRA, symbol->etc);
					}
				| tIDENTIFIER ':'
					{
						mana_symbol_entry* symbol = mana_symbol_create_label($1);
						symbol->address = mana_code_get_pc();
					}
				| tRETURN ';'
					{ mana_linker_return(mana_function_symbol_entry_pointer, NULL); }
				| tRETURN expression ';'
					{ mana_linker_return(mana_function_symbol_entry_pointer, $2); }
				| tROLLBACK expression ';'
					{ mana_linker_rollback($2); }
				| tBREAK ';'
					{ mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc())); }
				| tCONTINUE ';'
					{ mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc())); }
				| tHALT '(' ')' ';'
					{ mana_code_set_opecode(MANA_IL_HALT); }
				| tYIELD '(' ')' ';'
					{ mana_code_set_opecode(MANA_IL_YIELD); }
				| tCOMPLY '(' ')' ';'
					{ mana_code_set_opecode(MANA_IL_COMPLY); }
				| tREFUSE '(' ')' ';'
					{ mana_code_set_opecode(MANA_IL_REFUSE); }
				| tPRINT '(' call_args ')' ';'
					{ mana_linker_call_print($3); }
				| tREQUEST '(' expression ','  expression tDC tIDENTIFIER ')' ';'
					{ mana_symbol_add_request(MANA_IL_REQ, $3, $5, $7); }
				| tREQUEST '(' expression ','  expression ',' tSTRING ')' ';'
					{ mana_symbol_add_request(MANA_IL_REQ, $3, $5, $7); }
				| tJOIN '(' expression ','  expression ')' ';'
					{ mana_symbol_add_join($3, $5); }
				| block
				| declaration ';'
				| expression ';'
					{ mana_linker_expression($1, true); }
				| ';'
				| error ';'
				  { yyerrok; yyclearin; }
				| error '}'
				  { yyerrok; yyclearin; }
				;
expression		: left_hand '=' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, $3); }
				| expression tAND expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_LAND, $1, $3); }
				| expression tOR expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_LOR, $1, $3); }
				| expression tAADD expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_ADD, mana_node_clone($1), $3)); }
				| expression tASUB expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_SUB, mana_node_clone($1), $3)); }
				| expression tAMUL expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_MUL, mana_node_clone($1), $3)); }
				| expression tADIV expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_DIV, mana_node_clone($1), $3)); }
				| expression tAMOD expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_REM, mana_node_clone($1), $3)); }
				| expression tAAND expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_AND, mana_node_clone($1), $3)); }
				| expression tAOR expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_OR, mana_node_clone($1), $3)); }
				| expression tAXOR expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_XOR, mana_node_clone($1), $3)); }
				| expression tALSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_LSH, mana_node_clone($1), $3)); }
				| expression tARSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_RSH, mana_node_clone($1), $3)); }
				| expression '+' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ADD, $1, $3); }
				| expression '-' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_SUB, $1, $3); }
				| expression '*' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_MUL, $1, $3); }
				| expression '/' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_DIV, $1, $3); }
				| expression '%' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_REM, $1, $3); }
				| expression tPOW expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_POW, $1, $3); }
				| expression '&' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_AND, $1, $3); }
				| expression '|' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_OR, $1, $3); }
				| expression '^' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_XOR, $1, $3); }
				| expression tLSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_LSH, $1, $3); }
				| expression tRSHFT expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_RSH, $1, $3); }
				| expression '>' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_GT, $1, $3); }
				| expression tGE expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_GE, $1, $3); }
				| expression '<' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_LS, $1, $3); }
				| expression tLE expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_LE, $1, $3); }
				| expression tEQ expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_EQ, $1, $3); }
				| expression tNE expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_NE, $1, $3); }
				| tINC expression
					{
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $2, mana_node_create_node(MANA_NODE_TYPE_ADD, mana_node_clone($2), node));
					}
				| tDEC expression
					{
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $2, mana_node_create_node(MANA_NODE_TYPE_SUB, mana_node_clone($2), node));
					}
				| expression tINC	%prec tUINC
					{
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_ADD, mana_node_clone($1), node));
					}
				| expression tDEC	%prec tUDEC
					{
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						$$ = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, $1, mana_node_create_node(MANA_NODE_TYPE_SUB, mana_node_clone($1), node));
					}
				| expression '?' expression ':' expression
					{
						$$ = mana_node_create_node(MANA_NODE_TYPE_EXPRESSION_IF, $3, $5);
						$$->condition = $1;
					}
				| primary
				;

primary			: '-' expression	%prec tUMINUS
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_NEG, $2, NULL); }
				| '+' expression	%prec tUPLUS
					{ $$ = $2; }
				| '!' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_LNOT, $2, NULL); }
				| '~' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_NOT, $2, NULL); }
				| tSIZEOF '(' variable_type ')'
					{
						int max_char = (1 << (8 * CBSZ - 1)) - 1;
						int min_char = -1 << (8 * CBSZ - 1);
						int max_short = (1 << (8 * SBSZ - 1)) - 1;
						int min_short = -1 << (8 * SBSZ - 1);
						$$ = mana_node_allocate(MANA_NODE_TYPE_CONST);
						$$->digit = $3->memory_size;
						if($$->digit <= max_char && $$->digit >= min_char)
							$$->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						else if($$->digit <= max_short && $$->digit >= min_short)
							$$->type = mana_type_get(MANA_DATA_TYPE_SHORT);
						else
							$$->type = mana_type_get(MANA_DATA_TYPE_INT);
					}
				| tIDENTIFIER '(' call_args ')'
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_CALL, mana_node_create_function($1), $3); }
				| constant
				| left_hand
				;

constant		: tFALSE
					{ $$ = mana_node_create_digit(0); }
				| tTRUE
					{ $$ = mana_node_create_digit(1); }
				| tPRIORITY
					{
						$$ = mana_node_allocate(MANA_NODE_TYPE_PRIORITY);
						$$->type = mana_type_get(MANA_DATA_TYPE_INT);
					}
				| tSELF
					{
						$$ = mana_node_allocate(MANA_NODE_TYPE_SELF);
						$$->etc = 0;
						$$->type = mana_type_get(MANA_DATA_TYPE_ACTOR);
					}
				| tSENDER
					{
						$$ = mana_node_allocate(MANA_NODE_TYPE_SENDER);
						$$->type = mana_type_get(MANA_DATA_TYPE_ACTOR);
					}
				| tNIL
					{
						$$ = mana_node_allocate(MANA_NODE_TYPE_CONST);
						$$->type = mana_type_get(MANA_DATA_TYPE_NIL);
					}
				| tDIGIT
					{ $$ = mana_node_create_digit($1); }
				| tREAL
					{ $$ = mana_node_create_real($1); }
				| tSTRING
					{ $$ = mana_node_create_string($1); }
				;

left_hand		: left_hand '.' tIDENTIFIER
					{ $$ = mana_node_create_member($1, $3); }
				| left_hand '.' tIDENTIFIER '(' call_args ')'
					{ $$ = mana_node_create_call_member($1, $3, $5); }
				| left_hand '[' expression ']'
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ARRAY, $1, $3); }
				| tIDENTIFIER
					{ $$ = mana_node_create_leaf($1); }
				| '(' expression ')'
					{ $$ = $2; }
				;

case_block		: tCASE expression ':'
					{ mana_jump_switch_case($2); }
				  statements case
				;
case			: /* empty */
				| case_block
				| tDEFAULT ':'
					{ mana_jump_switch_default(); }
				  statements
				;

decl_args		: /* empty */
					{ $$ = 0; }
				| decl_arg
					{ $$ = 1; }
				| decl_args ',' decl_arg
					{ $$ = $1 + 1; }
				;
decl_arg		: variable_type declarator
					{ mana_symbol_allocate_memory($2, $1, MANA_MEMORY_TYPE_PARAMETER); }
				;

call_args		: /* empty */
					{ $$ = 0; }
				| expression
				| call_args ',' expression
					{ $$ = mana_node_create_node(MANA_NODE_TYPE_ARGUMENT, $1, $3); }
				;
%%

/*!
 * @brief	mana_print error message
 * @param	message		error message
 */
void yyerror(char* message)
{
	yynerrs ++;
	mana_error(mana_lexer_get_current_filename(), mana_lexer_get_current_line(), message);
}

/*!
 * initialize parser
 */
void mana_parser_initialize(void)
{
	mana_static_block_opend = false;

	/* レジスタ割り当て処理を初期化 */
	mana_register_initialzie();

	{
		/* vec2 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec2", mana_symbol_close_structure("vec2"));

		/* vec3 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec3", mana_symbol_close_structure("vec3"));

		/* vec4 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("w", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec4", mana_symbol_close_structure("vec4"));

		/* color */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("r", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("g", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("b", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("a", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("color", mana_symbol_close_structure("color"));
	}

	{
		mana_symbol_entry* symbol;

		/* int getUerData() */
		symbol = mana_symbol_create_function("getUserData");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setUserData(int data) */
		symbol = mana_symbol_create_function("setUserData");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("data", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));
	
		/* pointer getUserPointer() */
		symbol = mana_symbol_create_function("getUserPointer");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_pointer);

		/* void setUserPointer(pointer address) */
		symbol = mana_symbol_create_function("setUserPointer");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("address", NULL, false), mana_type_pointer, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int debug() */
		symbol = mana_symbol_create_function("debug");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setTickCount(int count) */
		symbol = mana_symbol_create_function("setTickCount");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void wait(float) */
		symbol = mana_symbol_create_function("wait");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("second", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void waitFrame(int) */
		symbol = mana_symbol_create_function("waitFrame");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("frame", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void srand(int count) */
		symbol = mana_symbol_create_function("srand");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int rand(int, int) */
		symbol = mana_symbol_create_function("rand");
		symbol->number_of_parameters = 2;
		mana_symbol_allocate_memory(mana_symbol_create_identification("min", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("max", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* float frand() */
		symbol = mana_symbol_create_function("frand");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sin(float) */
		symbol = mana_symbol_create_function("sin");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float cos(float) */
		symbol = mana_symbol_create_function("cos");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float atan2(float, float) */
		symbol = mana_symbol_create_function("atan2");
		symbol->number_of_parameters = 2;
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float tan(float) */
		symbol = mana_symbol_create_function("tan");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float angleMod(float) */
		symbol = mana_symbol_create_function("angleMod");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sqrt(float) */
		symbol = mana_symbol_create_function("sqrt");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* actor getActor(string) */
		symbol = mana_symbol_create_function("getActor");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("name", NULL, false), mana_type_string, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_ACTOR));
	}
}

/*!
 * shutdown parser
 */
void mana_parser_finalize(void)
{
	/* レジスタ割り当て処理を終了 */
	mana_register_finalize();
}

mana_symbol_entry* mana_parser_get_actor_symbol_entry_pointer()
{
	return mana_actor_symbol_entry_pointer;
}
