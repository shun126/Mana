/*!
mana (compiler)

@file	node.c
@brief	意味解析ノードに関するソースファイル
@detail	このファイルは意味解析ノードに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___NODE_H___)
#include "node.h"
#endif
#if !defined(___SYMBOL_DATA_H___)
#include "data.h"
#endif
#if !defined(___MANA_LEXER_H___)
#include "lexer.h"
#endif
#include <stdarg.h>
#include <string.h>

static node_entry* node_root_pointer = NULL;
static size_t node_total_size = 0;

void node_initialize(void)
{
	node_total_size = 0;
}

void node_finalize(void)
{
	node_entry* n = node_root_pointer;
	while (n)
	{
		node_entry* link = n->link;
		mana_free(n);
		n = link;
	}
	node_root_pointer = NULL;
}

/*!
ノード生成時の初期化
@param[in]	self	ノードオブジェクト
*/
static void node_constructor(node_entry* self)
{
#if defined(_DEBUG)
	static uint32_t count = 0;
	snprintf(self->magic, sizeof(self->magic), "N%d", count);
	++count;
#endif

	self->filename = lexer_get_current_filename();
	self->line = lexer_get_current_line();

	self->link = node_root_pointer;
	node_root_pointer = self;
}

node_entry* node_clone(const node_entry* org)
{
	node_entry* self = NULL;
	if(org)
	{
		self = (node_entry*)mana_malloc(sizeof(node_entry));
		memcpy(self, org, sizeof(node_entry));

		node_constructor(self);

		if(org->left)
			self->left = node_clone(org->left);

		if(org->right)
			self->right = node_clone(org->right);
	}
	return self;
}

node_entry* node_allocate(const node_id id)
{
	node_entry* self = (node_entry*)mana_calloc(sizeof(node_entry), 1);
	self->id = id;

	node_constructor(self);

	return self;
}

node_entry* node_create_node(node_id id, node_entry* left, node_entry* right, node_entry* body)
{
	node_entry* self = node_allocate(id);
	self->type = left ? left->type : NULL;
	self->left = left;
	self->right = right;
	self->body = body;
	return self;
}

node_entry* node_create_digit(const int32_t digit)
{
	int32_t max_char = (1 << (8 * CBSZ - 1)) - 1;
	int32_t min_char = -1 << (8 * CBSZ - 1);
	int32_t max_short = (1 << (8 * SBSZ - 1)) - 1;
	int32_t min_short = -1 << (8 * SBSZ - 1);

	node_entry* new_node = node_allocate(NODE_CONST);
	new_node->digit = digit;
	if(digit <= max_char && digit >= min_char)
		new_node->type = type_get(SYMBOL_DATA_TYPE_CHAR);
	else if(digit <= max_short && digit >= min_short)
		new_node->type = type_get(SYMBOL_DATA_TYPE_SHORT);
	else
		new_node->type = type_get(SYMBOL_DATA_TYPE_INT);

	return new_node;
}

node_entry* node_create_real(const float real)
{
	node_entry* new_node = node_allocate(NODE_CONST);
	new_node->real = real;
	new_node->type = type_get(SYMBOL_DATA_TYPE_FLOAT);
	return new_node;
}

node_entry* node_create_string(const char* string)
{
	node_entry* new_node = node_allocate(NODE_STRING);
	new_node->digit = data_set(string);
	new_node->type = type_string;
	return new_node;
}

node_entry* node_create_declare_function(node_entry* left, const char* identifier, node_entry* argument, node_entry* body)
{
	node_entry* new_node = node_allocate(NODE_DECLARE_FUNCTION);
	new_node->string = identifier;
	new_node->left = left;
	new_node->right = argument;
	new_node->body = body;

	if (argument)
	{
		for (node_entry* p = argument; p; p = p->right)
			++new_node->digit;
	}

	return new_node;
}

node_entry* node_create_declare_native_function(node_entry* left, const char* identifier, node_entry* argument, node_entry* body)
{
	node_entry* new_node = node_allocate(NODE_DECLARE_NATIVE_FUNCTION);
	new_node->string = identifier;
	new_node->left = left;
	new_node->right = argument;
	new_node->body = body;

	if (argument)
	{
		for (node_entry* p = argument; p; p = p->right)
			++new_node->digit;
	}

	return new_node;
}

/*!
キャストノードを作成します
@param[in]	type	キャストする型
@param[in]	self	ノードオブジェクト
@return				ノードオブジェクト
*/
static node_entry* node_create_cast(type_description* type, node_entry* self)
{
	if (type->tcons == self->type->tcons)
		return self;

	node_entry* new_node;

	if (type->tcons == SYMBOL_DATA_TYPE_FLOAT)
	{
		new_node = node_allocate(NODE_I2F);
		new_node->type = type_get(SYMBOL_DATA_TYPE_FLOAT);
		new_node->left = self;
	}
	else
	{
		new_node = node_allocate(NODE_F2I);
		new_node->type = type_get(SYMBOL_DATA_TYPE_INT);
		new_node->left = self;
	}

	return new_node;
}

node_entry* node_cast(type_description* type, node_entry* self)
{
	switch ((self->type)->tcons)
	{
	case SYMBOL_DATA_TYPE_CHAR:
	case SYMBOL_DATA_TYPE_SHORT:
	case SYMBOL_DATA_TYPE_INT:
		if (type->tcons == SYMBOL_DATA_TYPE_FLOAT)
		{
			if (self->id == NODE_CONST)
			{
				self->real = (float)self->digit;
				self->type = type_get(SYMBOL_DATA_TYPE_FLOAT);
			}
			else {
				self = node_create_cast(type_get(SYMBOL_DATA_TYPE_FLOAT), self);
			}
		}
		break;

	case SYMBOL_DATA_TYPE_FLOAT:
		switch (type->tcons)
		{
		case SYMBOL_DATA_TYPE_CHAR:
		case SYMBOL_DATA_TYPE_SHORT:
		case SYMBOL_DATA_TYPE_INT:
			if (self->id == NODE_CONST)
			{
				self->digit = (int32_t)self->real;
				self->type = type_get(SYMBOL_DATA_TYPE_INT);
			}
			else {
				self = node_create_cast(type_get(SYMBOL_DATA_TYPE_INT), self);
			}
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
	return self;
}

size_t node_get_memory_size(node_entry* self)
{
	switch(self->id)
	{
	case NODE_CONST:			/* 定数 */
	case NODE_IDENTIFIER:		/* 変数 */
		/* 参照のactorか、actorの実体か判定できるようにする */
		return self->type->tcons == SYMBOL_DATA_TYPE_ACTOR ? sizeof(void*) : self->type->memory_size;

	case NODE_ARRAY:			/* variable[argument] = */
	case NODE_MEMBER_VARIABLE:			/* X.variable */
	case NODE_NEG:			/* ±符号反転 */
		return self->type->tcons == SYMBOL_DATA_TYPE_ACTOR ? sizeof(void*) : self->type->memory_size;
/*		return node_get_memory_size(self->left);	*/

	case NODE_SELF:			/* self (actor) */
	case NODE_PRIORITY:		/* runlevel (integer) */
		return sizeof(int32_t*);

	case NODE_ASSIGN:			/* = */
	case NODE_CALL_ARGUMENT:		/* 呼び出し側引数 */
	case NODE_CALL:			/* 関数呼び出し */
	case NODE_ADD:			/* 加算 */
	case NODE_SUB:			/* 減算 */
	case NODE_MUL:			/* 乗算 */
	case NODE_DIV:			/* 除算 */
	case NODE_REM:			/* 余剰 */
	case NODE_POW:			/* べき乗 */
	case NODE_NOT:			/* ~ */
	case NODE_LNOT:			/* ! */
	case NODE_AND:			/* & */
	case NODE_OR:				/* | */
	case NODE_XOR:			/* ^ */
	case NODE_LSH:			/* << */
	case NODE_RSH:			/* >> */
	case NODE_LS:				/* < */
	case NODE_LE:				/* <= */
	case NODE_EQ:				/* == */
	case NODE_NE:				/* != */
	case NODE_GE:				/* >= */
	case NODE_GT:				/* > */
	case NODE_STRING:			/* 文字列 */
	case NODE_I2F:			/* 整数から実数へ変換 */
	case NODE_F2I:			/* 実数から整数へ変換 */
	case NODE_LOR:			/* || */
	case NODE_LAND:			/* && */
	case NODE_SENDER:			/* sender (actor) */
	case NODE_EXPRESSION_IF:	/* 三項演算子 '?' */
	default:
#if 1
		return self->type->memory_size;
#else
		mana_compile_error("illigal node type detect");
		return 0;
#endif
	}
}

static bool node_dump_format_flag_ = false;

static void node_dump_format_(FILE* file, const char* format, ...)
{
	if (node_dump_format_flag_)
	{
		node_dump_format_flag_ = false;
		fputc(',', file);
	}

	va_list arg;
	va_start(arg, format);
#if __STDC_WANT_SECURE_LIB__
	vfprintf_s(file, format, arg);
#else
	vfprintf(file, format, arg);
#endif
	va_end(arg);

	node_dump_format_flag_ = true;
}

static void node_dump_(FILE* file, const node_entry* self)
{
	static char* name[] = {
		"NEWLINE",

		"ARRAY",								/*!< variable[argument] = */
		"ASSIGN",								/*!< = */
		"MEMOP",								/*!< X.variable */
		"CALL_ARGUMENT",							/*!< 呼び出し側引数 */
		"DECLARE_ARGUMENT",							/*!< 呼び出し側引数 */
		"CONST",								/*!< 定数 */
		"VARIABLE",							/*!< 変数 */
		"CALL",								/*!< 関数呼び出し */
		"ADD",									/*!< 加算 */
		"SUB",									/*!< 減算 */
		"MUL",									/*!< 乗算 */
		"DIV",									/*!< 除算 */
		"REM",									/*!< 余剰 */
		"NEG",									/*!< ±符号反転 */
		"POW",									/*!< べき乗 */
		"NOT",									/*!< ~ */
		"AND",									/*!< & */
		"OR",									/*!< | */
		"XOR",									/*!< ^ */
		"LSH",									/*!< << */
		"RSH",									/*!< >> */
		"LS",									/*!< < */
		"LE",									/*!< <= */
		"EQ",									/*!< == */
		"NE",									/*!< != */
		"GE",									/*!< >= */
		"GT",									/*!< > */
		"STRING",								/*!< 文字列 */
		"I2F",									/*!< 整数から実数へ変換 */
		"F2I",									/*!< 実数から整数へ変換 */
		"LOR",									/*!< || */
		"LAND",								/*!< && */
		"LNOT",								/*!< ! */
		"HALT",								/*!< halt */
		"YIELD",								/*!< yield */
		"REQUEST",								/*!< req */
		"COMPLY",								/*!< comply (req許可) */
		"REFUSE",								/*!< refuse (req拒否) */
		"JOIN",								/*!< join */
		"SENDER",								/*!< sender (actor) */
		"SELF",								/*!< self (actor) */
		"PRIORITY",							/*!< priority (integer) */
		"EXPRESSION_IF",						/*!< 三項演算子 '?' */
		"PRINT",								/*!< print */
		"RETURN",								/*!< return */
		"ROLLBACK",							/*!< rollback */

		"BLOCK",								/*!< ブロック */
		"IF",									/*!< if */
		"SWITCH",								/*!< switch */
		"CASE",								/*!< case */
		"DEFAULT",								/*!< default */
		"WHILE",								/*!< while */
		"DO",									/*!< do while */
		"FOR",									/*!< for */
		"LOOP",								/*!< loop */
		"LOCK",								/*!< lock */
		"GOTO",								/*!< goto */
		"LABEL",								/*!< label */
		"BREAK",								/*!< break */
		"CONTINUE",							/*!< continue */

		"IDENTIFIER",
		"TYPE_DESCRIPTION",
		"DECLARATOR",

		"VARIABLE_DECLARATION",
		"SIZEOF",

		"DECLARE_ACTOR",						//!< アクターの宣言
		"DECLARE_PHANTOM",
		"DECLARE_MODULE",
		"DECLARE_STRUCT",
		"DECLARE_ACTION",
		"DECLARE_EXTEND",
		"DECLARE_ALLOCATE",
		"DECLARE_STATIC",
		"DECLARE_ALIAS",
		"DECLARE_NATIVE_FUNCTION",
		"DECLARE_FUNCTION",

		"DEFINE_CONSTANT",
		"UNDEFINE_CONSTANT",

		"MEMBER_FUNCTION",
		"MEMBER_VARIABLE",
	};

#if defined(_DEBUG)
	node_dump_format_(file, "\"magic\": \"%s\"", self->magic);
#endif

	if (self->id < sizeof(name) / sizeof(name[0]))
	{
		node_dump_format_(file, "\"name\": \"%s\"", name[self->id]);
	}
	else
	{
		node_dump_format_(file, "\"name\": \"%d\"", self->id);
	}

	if (self->string)
		node_dump_format_(file, "\"string\": \"%s\"", self->string);
	if (self->type)
		node_dump_format_(file, "\"type\": \"%s\"", self->type->name);

	if (self->left)
	{
		node_dump_format_(file, "\"left\": {\n");
		node_dump_format_flag_ = false;
		node_dump_(file, self->left);
		fprintf(file, "}\n");
	}
	if (self->right)
	{
		node_dump_format_(file, "\"right\": {\n");
		node_dump_format_flag_ = false;
		node_dump_(file, self->right);
		fprintf(file, "}\n");
	}
	if (self->body)
	{
		node_dump_format_(file, "\"body\": {\n");
		node_dump_format_flag_ = false;
		node_dump_(file, self->body);
		fprintf(file, "}\n");
	}
	if (self->next)
	{
		node_dump_format_(file, "\"next\": {\n");
		node_dump_format_flag_ = false;
		node_dump_(file, self->next);
		fprintf(file, "}\n");
	}
}

void node_dump(const node_entry* self)
{
	FILE* file;
#if defined(__STDC_WANT_SECURE_LIB__)
	if (fopen_s(&file, "node_entry.json", "wt") == 0)
#else
	file = fopen("node_entry.json", "wt");
	if (file)
#endif
	{
		if (self)
		{
			fputc('{', file);
			node_dump_(file, self);
			fputc('}', file);
		}
		fclose(file);
	}
}
