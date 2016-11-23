/*
 * mana (compiler)
 *
 * @file	mana_symbol.c
 * @brief	レジスタ割り当てに関するソースファイル
 * @detail	このファイルはレジスタ割り当てに関するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
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
#include <assert.h>
#include <string.h>

#define MANA_SYMBOL_BLOCK_TABLE_SIZE	(256)
#define MANA_SYMBOL_HASHTABLE_SIZE		(1021)

#define MANA_SYMBOL_IS_ACTOR_OR_STRUCTER_OPENED()	(mana_symbol_actor_or_structure_level > 0)
#define MANA_SYMBOL_IS_FUNCTION_OPENED()			(mana_symbol_is_function_opened)

static char* mana_symbol_class_type_id_name[MANA_CLASS_TYPE_NUMBER_OF] = {
	"new symbol",										/* 未割り当て関数 */
	"typedef",											/* 型定義 */
	"prototype function",								/* プロトタイプ関数宣言 */
	"function",											/* 関数 */
	"native function",									/* 外部関数 */
	"member function",									/* メンバー関数(アクション) */
	"static variable",									/* スタティック変数 */
	"global variable",									/* グローバル変数 */
	"instance variable",								/* インスタンス変数 */
	"local variable",									/* ローカル変数 */
	"constant int",										/* 整数 */
	"constant float",									/* 実数 */
	"constant string",									/* 文字列 */
	"alias",											/* データ参照 */
};

static char* mana_symbol_data_type_id_name[MANA_DATA_TYPE_NUMBER_OF] = {
	"void",												/* void型 */
	"char",												/* char型 */
	"short",											/* short型 */
	"int",												/* int型 */
	"float",											/* float型 */
	"reference",										/* reference型 */
	"array",											/* array型 */
	"struct",											/* struct型 */
	"actor",											/* actor型 */
	"module",											/* module型 */
	"nil",												/* nil型 */
	"incomplete",										/* 宣言が未完了の型 */
};

static struct mana_symbol_block_table
{
	int allocp;
	mana_symbol_entry* head;
} mana_symbol_block_table[MANA_SYMBOL_BLOCK_TABLE_SIZE];

static mana_symbol_entry* mana_symbol_hash_table[MANA_SYMBOL_HASHTABLE_SIZE];
static int mana_symbol_actor_or_structure_level;
static int mana_symbol_function_block_level;
static int mana_symbol_block_level;
static int mana_symbol_is_function_opened;

static int mana_symbol_static_memory_address;
static int mana_symbol_global_memory_address;
static int mana_symbol_actor_memory_address;
static int mana_symbol_local_memory_address;
static int mana_symbol_max_local_memory_address;

int mana_symbol_frame_size_list;
int mana_symbol_return_address_list;

static mana_symbol_entry* mana_symbol_create(char* name, mana_symbol_class_type_id class_type);
static mana_symbol_entry* mana_symbol_create_entry(char* name, mana_symbol_class_type_id class_type, int address);

void mana_symbol_initialize(void)
{
	int i;

	mana_symbol_actor_or_structure_level = 0;
	mana_symbol_function_block_level = 0;
	mana_symbol_block_level = 0;
	mana_symbol_is_function_opened = MANA_FALSE;

	mana_symbol_static_memory_address = 0;
	mana_symbol_global_memory_address = 0;
	mana_symbol_local_memory_address = 0;
	mana_symbol_max_local_memory_address = 0;

	for(i = 0; i < MANA_SYMBOL_BLOCK_TABLE_SIZE; i ++)
	{
		mana_symbol_block_table[i].allocp = 0;
		mana_symbol_block_table[i].head = NULL;
	}
	for(i = 0; i < MANA_SYMBOL_HASHTABLE_SIZE; i++)
	{
		mana_symbol_hash_table[i] = NULL;
	}
}

void mana_symbol_finalize(void)
{
}

static int mana_symbol_align_size(int X, int Y)
{
	return (X + Y - 1) / Y * Y;
}

int mana_symbol_is_valid_variable(mana_symbol_entry* symbol)
{
	if(	symbol &&
		symbol->class_type != MANA_CLASS_TYPE_VARIABLE_STATIC &&
		symbol->class_type != MANA_CLASS_TYPE_VARIABLE_GLOBAL &&
		symbol->class_type != MANA_CLASS_TYPE_VARIABLE_ACTOR &&
		symbol->class_type != MANA_CLASS_TYPE_VARIABLE_LOCAL &&
		symbol->class_type != MANA_CLASS_TYPE_CONSTANT_INT &&
		symbol->class_type != MANA_CLASS_TYPE_CONSTANT_FLOAT &&
		symbol->class_type != MANA_CLASS_TYPE_CONSTANT_STRING
	){
		mana_compile_error("non-variable name '%s'", symbol->name);
		return MANA_FALSE;
	}else{
		return MANA_TRUE;
	}
}

int mana_symbol_get_static_memory_address()
{
	return mana_symbol_static_memory_address;
}

void mana_symbol_set_static_memory_address(int size)
{
	if(mana_symbol_static_memory_address < size)
		mana_symbol_static_memory_address = size;
}

int mana_symbol_get_global_memory_address()
{
	return mana_symbol_global_memory_address;
}

void mana_symbol_set_global_memory_address(int size)
{
	if(mana_symbol_global_memory_address < size)
		mana_symbol_global_memory_address = size;
}

static int mana_symbol_get_hash_value(char* name)
{
	unsigned h = 0;

	while(*name != '\0')
	{
		unsigned g;

		h = (h << 4) + (unsigned)(*name);
		g = h & 0xf0000000;
		if(g)
			h = (h ^ g >> 24) ^ g;

		name++;
	}

	return (h % MANA_SYMBOL_HASHTABLE_SIZE);
}

void mana_symbol_open_block(int reset_max_frame_memory_address)
{
	if(mana_symbol_block_level <= 0 || reset_max_frame_memory_address)
		mana_symbol_max_local_memory_address = mana_symbol_local_memory_address;

	if(++mana_symbol_block_level >= MANA_SYMBOL_BLOCK_TABLE_SIZE)
	{
		MANA_BUG("symbol block buffer overrun!");
	}

	mana_symbol_block_table[mana_symbol_block_level].head = NULL;
	mana_symbol_block_table[mana_symbol_block_level].allocp = mana_symbol_local_memory_address;
}

void mana_symbol_close_block(void)
{
	mana_symbol_entry* symbol;

	assert(mana_symbol_block_level >= 0);

	/* 1) check and update hash table */
	for(symbol = mana_symbol_block_table[mana_symbol_block_level].head; symbol; symbol = symbol->next)
	{
		int hash_value = mana_symbol_get_hash_value(symbol->name);

		if(mana_symbol_hash_table[hash_value] == symbol)
			mana_symbol_hash_table[hash_value] = symbol->hash_chain;

		if(symbol->class_type == MANA_CLASS_TYPE_TYPEDEF && (symbol->type)->tcons == MANA_DATA_TYPE_INCOMPLETE)
		{
			mana_compile_error("incomplete type name %s", symbol->name);
		}
	}

	/* 2) calculate frame buffer size */
	if(mana_symbol_local_memory_address > mana_symbol_max_local_memory_address)
	{
		mana_symbol_max_local_memory_address = mana_symbol_local_memory_address;
	}
	mana_symbol_local_memory_address = mana_symbol_block_table[mana_symbol_block_level].allocp;

	mana_symbol_block_level--;
}

mana_symbol_entry* mana_symbol_lookup(char* name)
{
	mana_symbol_entry* symbol;
	int hash_value;

	hash_value = mana_symbol_get_hash_value(name);

	symbol = mana_symbol_hash_table[hash_value];
	while(symbol && strcmp(symbol->name, name) != 0)
	{
		symbol = symbol->hash_chain;
	}
	return symbol;
}

mana_symbol_entry* mana_symbol_lookup_or_create_dummy(char* name)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol == NULL)
	{
		mana_compile_error("reference to undeclared identifier '%s'", name);

		symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_VARIABLE_LOCAL, 0);
		symbol->type = mana_type_get(MANA_DATA_TYPE_INT);
	}
	return symbol;
}

static mana_symbol_entry* mana_symbol_create_with_level(char* name, mana_symbol_class_type_id class_type, int level)
{
	mana_symbol_entry* symbol;
	int hash_value;

	hash_value = mana_symbol_get_hash_value(name);

	symbol = (mana_symbol_entry*)mana_calloc(1, sizeof(mana_symbol_entry));
	symbol->class_type = class_type;
	symbol->define_level = level;
	symbol->name = name;
	symbol->next = mana_symbol_block_table[level].head;
	symbol->hash_chain = mana_symbol_hash_table[hash_value];
	mana_symbol_hash_table[hash_value] = symbol;
	mana_symbol_block_table[level].head = symbol;

	return symbol;
}

static mana_symbol_entry* mana_symbol_create(char* name, mana_symbol_class_type_id class_type)
{
	return mana_symbol_create_with_level(name, class_type, mana_symbol_block_level);
}

void mana_symbol_destroy(char* name)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol)
		symbol->name = 0;
}

static mana_symbol_entry* mana_symbol_create_entry(char* name, mana_symbol_class_type_id class_type, int address)
{
	mana_symbol_entry* symbol;

	symbol = mana_symbol_create(name, class_type);
	symbol->address = address;

	return symbol;
}

mana_symbol_entry* mana_symbol_create_alias(char* name, char* filename)
{
	mana_symbol_entry* symbol;
	char path[_MAX_PATH];

	symbol = mana_symbol_lookup(name);
	if(symbol)
		mana_compile_error("duplicated declaration '%s'", name);

	symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_ALIAS, -1);
	symbol->type = mana_type_get(MANA_DATA_TYPE_INT);

	if(_fullpath(path, filename, sizeof(path)))
	{
		symbol->string = mana_pool_set(path);
	}else{
		mana_compile_error("unable to open \"%s\"", filename);
		symbol->string = filename;
	}

	return symbol;
}

mana_symbol_entry* mana_symbol_create_const_int(char* name, int value)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol)
		mana_compile_error("duplicated declaration '%s'", name);

	symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_CONSTANT_INT, value);
	symbol->type = mana_type_get(MANA_DATA_TYPE_INT);

	if(mana_variable_header_file)
	{
		fprintf(mana_variable_header_file, "#define _%s %d\n", name, value);
	}

	return symbol;
}

mana_symbol_entry* mana_symbol_create_const_float(char* name, float value)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol)
		mana_compile_error("duplicated declaration '%s'", name);

	symbol = mana_symbol_create(name, MANA_CLASS_TYPE_CONSTANT_FLOAT);
	symbol->type = mana_type_get(MANA_DATA_TYPE_FLOAT);
	symbol->hloat = value;

	if(mana_variable_header_file)
	{
		fprintf(mana_variable_header_file, "#define _%s %f\n", name, value);
	}

	return symbol;
}

mana_symbol_entry* mana_symbol_create_const_string(char* name, char* value)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol)
		mana_compile_error("duplicated declaration '%s'", name);

	symbol = mana_symbol_create(name, MANA_CLASS_TYPE_CONSTANT_STRING);
	symbol->string = value;
	symbol->type = mana_type_string;

	if(mana_variable_header_file)
	{
		fprintf(mana_variable_header_file, "#define _%s \"%s\"\n", name, value);
	}

	return symbol;
}

/*
mana_symbol_entry* mana_symbol_create_type(char* name)
{
	mana_symbol_entry* symbol;

	symbol = mana_symbol_lookup(name);
	if(symbol)
	{
		if(symbol->class_type == MANA_CLASS_TYPE_TYPEDEF)
			return symbol;
		mana_compile_error("invalid identifier used as a type name");
	}

	symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_TYPEDEF, 0);
	symbol->type = mana_type_create(MANA_DATA_TYPE_INCOMPLETE, NULL, NULL);

	return symbol;
}
*/

mana_symbol_entry* mana_symbol_create_identification(char* name, mana_type_description* type, int static_variable)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol == NULL || symbol->define_level < mana_symbol_block_level)
	{
		if(static_variable)
		{
			symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_VARIABLE_STATIC, 0);
		}
		else if(mana_symbol_block_level <= 0)
		{
			symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_VARIABLE_GLOBAL, 0);
		}
		else if(!MANA_SYMBOL_IS_FUNCTION_OPENED())
		{
			symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_VARIABLE_ACTOR, 0);
		}
		else
		{
			symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_VARIABLE_LOCAL, 0);
		}
		symbol->type = type;
	}
	else
	{
		mana_compile_error("duplicated declaration '%s'", name);
	}

	return symbol;
}

mana_symbol_entry* mana_symbol_create_label(char* name)
{
	mana_symbol_entry* symbol;

	symbol = mana_symbol_lookup(name);
	if(symbol == NULL)
	{
		symbol = mana_symbol_create_with_level(name, MANA_CLASS_TYPE_LABEL, mana_symbol_function_block_level);
		symbol->address = -1;
		symbol->etc = -1;
	}

	return symbol;
}

mana_symbol_entry* mana_symbol_create_function(char* name)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol == NULL)
		symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_NEW_SYMBOL, 0);

	if(MANA_SYMBOL_IS_ACTOR_OR_STRUCTER_OPENED())
	{
		/* action */
		mana_symbol_open_block(MANA_TRUE);
	}
	else
	{
		/*
		 * function(p1, p2);
		 *
		 * +--------------------+
		 * | 4	Return Address	|
		 * | 8	p1				|
		 * | 12 p2				|
		 * | 16 local variables |
		 * |		 :			|
		 * +--------------------+
		 */
		mana_symbol_open_block(MANA_FALSE);
		mana_symbol_local_memory_address += sizeof(void*);
	}

	mana_symbol_is_function_opened = MANA_TRUE;

	return symbol;
}

void mana_symbol_create_prototype(mana_symbol_entry* symbol, mana_type_description* type)
{
	/* 1) check */
	if(type->tcons == MANA_DATA_TYPE_INCOMPLETE)
		mana_compile_error("incomplete data type is used");

	/* 2) initialize function's symbol entry */
	symbol->class_type = MANA_CLASS_TYPE_PROTOTYPE_FUNCTION;
	symbol->type = type;
	symbol->address = -1;
	symbol->parameter_list = mana_symbol_block_table[mana_symbol_block_level].head;

	/* 3) all close blocks */
	mana_symbol_is_function_opened = MANA_FALSE;

	/* 4) mana_symbol_open_blockがmana_symbol_create_functionで呼ばれるので
	 * mana_symbol_create_prototypeにmana_symbol_open_blockを呼んでいなくても
	 * ここでmana_symbol_close_blockを呼び出す必要がある。
	 */
	mana_symbol_close_block();
}

void mana_symbol_open_function(int is_action, mana_symbol_entry* function, mana_type_description* type)
{
	/*
	 * 事前にmana_symbol_create_functionが呼ばれるのでmana_symbol_open_block
	 * を呼び出さなくて良い
	 */
	mana_symbol_entry* symbol;

	if(type->tcons == MANA_DATA_TYPE_INCOMPLETE)
	{
		mana_compile_error("incomplete data type is used");
	}

	/* レジスタ割り当て処理をクリア */
	mana_register_clear();

	if(function->class_type == MANA_CLASS_TYPE_PROTOTYPE_FUNCTION)
	{
		if(function->type == type)
		{
			mana_symbol_entry* p1;
			mana_symbol_entry* p2;

			p1 = function->parameter_list;
			p2 = mana_symbol_block_table[mana_symbol_block_level].head;

			while(p1 && p2)
			{
				if(mana_type_compare(p1->type, p2->type) == 0)
				{
					mana_compile_error("parameter specification unmatched");
				}
				p1 = p1->next;
				p2 = p2->next;
			}
			if(p1 != NULL || p2 != NULL)
			{
				mana_compile_error("no. of parameters unmatched to the prototype");
			}
		}
		else
		{
			mana_compile_error("function type unmatched to the prototype");
		}
		mana_code_replace_all(function->address, mana_code_get_pc());
	}
	else if(function->class_type == MANA_CLASS_TYPE_NEW_SYMBOL)
	{
		function->type = type;
	}
	else
	{
		mana_compile_error("the function already declared");
	}

	/* シンボルの設定 */
	function->address = mana_code_get_pc();
	function->etc = is_action;
	function->parameter_list = mana_symbol_block_table[mana_symbol_block_level].head;

	mana_symbol_function_block_level = mana_symbol_block_level;

	/* frame bufferの確保する命令を発行 */
	mana_symbol_frame_size_list = mana_code_set_opecode_and_operand(MANA_IL_ALLOCATE, -1);

	if(!function->etc)
	{
		/* return addressをframe bufferに保存する命令を発行 */
		mana_code_set_opecode(MANA_IL_SAVE_RETURN_ADDRESS);
	}

	/* returnのジャンプ先リンクを初期化 */
	mana_symbol_return_address_list = -1;

	/* パラメータの設定 */
	for(symbol = function->parameter_list; symbol; symbol = symbol->next)
	{
		mana_code_set_opecode_and_operand(MANA_IL_LOAD_FRAME_ADDRESS, symbol->address);

		switch((symbol->type)->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
			mana_code_set_opecode(MANA_IL_STORE_CHAR);
			break;

		case MANA_DATA_TYPE_SHORT:
			mana_code_set_opecode(MANA_IL_STORE_SHORT);
			break;

		case MANA_DATA_TYPE_INT:
			mana_code_set_opecode(MANA_IL_STORE_INTEGER);
			break;

		case MANA_DATA_TYPE_FLOAT:
			mana_code_set_opecode(MANA_IL_STORE_FLOAT);
			break;

		case MANA_DATA_TYPE_ACTOR:
			mana_code_set_opecode(MANA_IL_STORE_INTEGER);
			break;

		default:
			if((symbol->type)->memory_size <= 0)
				mana_compile_error("missing size information on parameter");
			mana_code_set_opecode_and_operand(MANA_IL_STORE_DATA, (symbol->type)->memory_size);
			break;
		}
	}
}

void mana_symbol_close_function(mana_symbol_entry* function)
{
	if(MANA_SYMBOL_IS_ACTOR_OR_STRUCTER_OPENED())
		mana_data_set(function->name);

	/* gotoのジャンプ先を更新 */
	{
		mana_symbol_entry* symbol;

		assert(mana_symbol_function_block_level == mana_symbol_block_level);

		for(symbol = mana_symbol_block_table[mana_symbol_block_level].head; symbol; symbol = symbol->next)
		{
			if(symbol->class_type == MANA_CLASS_TYPE_LABEL)
			{
				if(symbol->address < 0)
					mana_compile_error("label '%s' used but not defined", symbol->name);

				mana_code_replace_all(symbol->etc, symbol->address);
			}
		}
	}

	/* returnのジャンプ先を更新 */
	mana_code_replace_all(mana_symbol_return_address_list, mana_code_get_pc());

	/* 直後のジャンプは削除 */
	if(mana_symbol_return_address_list >= 0)
	{
		mana_code_reduce(5/*int pebble_get_instruction_size(unsigned char* program)*/);
	}

	if(!function->etc)
	{
		/* return addressをレジスタに復帰する命令を発行 */
		mana_code_set_opecode(MANA_IL_LOAD_RETURN_ADDRESS);
	}

	/* free命令の発行 */
	mana_symbol_frame_size_list = mana_code_set_opecode_and_operand(MANA_IL_FREE, mana_symbol_frame_size_list);

	/* return命令の発行 */
	if(MANA_SYMBOL_IS_ACTOR_OR_STRUCTER_OPENED())
	{
		function->class_type = MANA_CLASS_TYPE_MEMBER_FUNCTION;
		mana_code_set_opecode((unsigned char)MANA_IL_RETURN_FROM_ACTION);
	}
	else
	{
		MANA_VERIFY(function->type, "type description is null pointer");
		if(function->type->tcons != MANA_DATA_TYPE_VOID)
		{
			if(! function->used)
				mana_compile_error("meaningless return value specification");
		}
		mana_code_set_opecode((unsigned char)MANA_IL_RETURN_FROM_FUNCTION);
		function->class_type = MANA_CLASS_TYPE_FUNCTION;
	}

	/*
	 * frame bufferのサイズを更新
	 *
	 * mana_symbol_open_blockがmana_symbol_create_functionで呼ばれるので
	 * mana_symbol_open_functionにmana_symbol_open_blockを呼んでいなくても
	 * ここでmana_symbol_close_blockを呼び出す必要がある。
	 */
	mana_symbol_close_block();
	mana_symbol_is_function_opened = MANA_FALSE;
	mana_code_replace_all(mana_symbol_frame_size_list,
		mana_symbol_align_size(mana_symbol_max_local_memory_address, FBSZ));
}

void mana_symbol_set_native_function(mana_symbol_entry* function, mana_type_description* type)
{
	/* 1) check */
	if(mana_symbol_block_level > 1)
		mana_compile_error("the prototype declaration ignored");

	if(type->tcons == MANA_DATA_TYPE_INCOMPLETE)
		mana_compile_error("incomplete data type is used");

	/* 2) initialize function's symbol entry */
	function->class_type = MANA_CLASS_TYPE_NATIVE_FUNCTION;
	function->type = type;
	function->parameter_list = mana_symbol_block_table[mana_symbol_block_level].head;

	/* 3) close block
	 *
	 * mana_symbol_open_blockがmana_symbol_create_functionで呼ばれるので
	 * ここでmana_symbol_close_blockを呼び出す必要がある。
	 */
	mana_symbol_close_block();
	mana_symbol_is_function_opened = MANA_FALSE;
}

void mana_symbol_set_type(char* name, mana_type_description* type)
{
	mana_symbol_entry* symbol;

	symbol = mana_symbol_lookup(name);
	if(symbol == NULL)
	{
		symbol = mana_symbol_create_entry(name, MANA_CLASS_TYPE_TYPEDEF, 0);
		symbol->type = type;
	}
	else if(symbol->type == type)
	{
		return;
	}
	else if(symbol->class_type == MANA_CLASS_TYPE_TYPEDEF && (symbol->type)->tcons == MANA_DATA_TYPE_INCOMPLETE)
	{
		if(symbol == (mana_symbol_entry*)type->component || type->tcons == MANA_DATA_TYPE_INCOMPLETE)
		{
			mana_compile_error("illegal reference to an incomplete name");
		}
		else
		{
			*(symbol->type) = *type;
		}
	}
	else
	{
		mana_compile_error("invalid identifier used as a type name");
	}

	type->name = name;
}

/*****************************************************************************/
/* structuer */
void mana_symbol_open_structure(void)
{
	/* 1) output header */
	if(mana_variable_header_file)
		mana_symbol_print_header();

	/* 2) open block */
	mana_symbol_open_block(MANA_FALSE);
	mana_symbol_actor_or_structure_level++;
	mana_symbol_actor_memory_address = 0;
}

mana_type_description* mana_symbol_close_structure(char* name)
{
	mana_symbol_entry* symbol;
	mana_type_description* type;
	int max_sligment_size;

	/* 最も大きいサイズのタイプにアライメントを合わせる */
	symbol = mana_symbol_block_table[mana_symbol_block_level].head;
	max_sligment_size = 0;
	while(symbol)
	{
		if(max_sligment_size < symbol->type->alignment_memory_size)
			max_sligment_size = symbol->type->alignment_memory_size;
		symbol = symbol->next;
	}

	/* 1) mana_symbol_entry*をmana_type_description*として代入しています
	 * 参照先でmana_symbol_entry*にキャストしています。
	 * TODO:危険なのでちゃんとメンバーを追加しましょう
	 */
	type = (mana_type_description*)mana_symbol_block_table[mana_symbol_block_level].head;

	/* 2) create new type description */
	type = mana_type_create(MANA_DATA_TYPE_STRUCT, type, NULL);
	type->name = name;
	type->alignment_memory_size = max_sligment_size;

	mana_symbol_actor_or_structure_level--;
	mana_symbol_close_block();

	type->memory_size = mana_symbol_align_size(mana_symbol_actor_memory_address, max_sligment_size);

	/* 3) output header */
	if(mana_variable_header_file)
		mana_symbol_print_footer(name, type);

	return type;
}

/*****************************************************************************/
/* actor */
static void mana_symbol_open_actor_register_member(mana_symbol_entry* symbol)
{
	int hash_value;

	if(symbol->next)
		mana_symbol_open_actor_register_member(symbol->next);

	hash_value = mana_symbol_get_hash_value(symbol->name);

	symbol->hash_chain = mana_symbol_hash_table[hash_value];
	mana_symbol_hash_table[hash_value] = symbol;
}

void mana_symbol_open_actor(mana_symbol_entry* symbol)
{
	if(mana_symbol_block_level != 0)
		mana_compile_error("the actor declaration ignored");

	mana_symbol_open_block(MANA_FALSE);
	mana_symbol_actor_or_structure_level++;

	if(symbol)
	{
		mana_type_description* type;

		for(type = symbol->type; type->tcons == MANA_DATA_TYPE_ARRAY; type = type->component)
			;

		/* typeがactorではない場合、続行不可能 */
		if(type->tcons != MANA_DATA_TYPE_ACTOR && type->tcons != MANA_DATA_TYPE_MODULE)
		{
			mana_compile_error("%s is NOT actor!", symbol->name);
		}
		else
		{
			/* mana_symbol_close_blockでmana_symbol_hash_tableを開放する */
			mana_symbol_block_table[mana_symbol_block_level].head = (mana_symbol_entry*)type->component;

			/* シンボルリストの末端からhashに登録 */
			mana_symbol_open_actor_register_member((mana_symbol_entry*)type->component);
		}
	}

	/* instance変数サイズの再計算 */
	mana_symbol_actor_memory_address = (symbol && symbol->type) ? symbol->type->memory_size : 0;
}

mana_type_description* mana_symbol_close_actor(char* name, char* parent, mana_type_description* td, int phantom)
{
	mana_symbol_entry* symbol;
	mana_type_description* type;
	mana_type_description* parent_type;

	symbol = mana_symbol_lookup(name);
	if(symbol)
	{
		for(type = symbol->type; type->tcons == MANA_DATA_TYPE_ARRAY; type = type->component)
			;
		if(type->tcons == MANA_DATA_TYPE_ACTOR)
		{
			/* mana_symbol_entry*をmana_type_description*として代入しています
			 * 参照先でmana_symbol_entry*にキャストしています。
			 *
			 * @TODO	危険なのでちゃんとメンバーを追加しましょう
			 */
			type->component = (mana_type_description*)(mana_symbol_block_table[mana_symbol_block_level].head);

			/* actor and phantom check */
			if(type->share.actor.phantom != phantom)
				mana_compile_error("already declared %s", type->share.actor.phantom ? "a phantom" : "an actor");

			/* @TODO	actorの宣言が二つある場合、ワーニングを出す？ */
			goto SKIP;
		}
	}

	mana_data_set(name);

	if(parent)
	{
		mana_symbol_entry* parent_symbol = mana_symbol_lookup(parent);
		parent_type = parent_symbol ? parent_symbol->type : NULL;
	}
	else
	{
		parent_type = NULL;
	}

	/* mana_symbol_entry*をmana_type_description*として代入しています
	 * 参照先でmana_symbol_entry*にキャストしています。
	 * TODO:危険なのでちゃんとメンバーを追加しましょう
	 */
	type = mana_type_create(MANA_DATA_TYPE_ACTOR,
		(mana_type_description*)(mana_symbol_block_table[mana_symbol_block_level].head), parent_type);
	type->name = name;
	type->alignment_memory_size = IBSZ;
	type->share.actor.phantom = phantom;

SKIP:
	mana_symbol_actor_or_structure_level--;
	mana_symbol_close_block();

	type->memory_size = mana_symbol_align_size(mana_symbol_actor_memory_address, IBSZ);

	if(td)
	{
		mana_type_description* nested_type;
		for(nested_type = td; nested_type->component; nested_type = nested_type->component)
			;
		nested_type->component = type;
		return td;
	}
	else
	{
		return type;
	}
}

void mana_symbol_open_actor2(mana_symbol_entry* symbol)
{
	if(!symbol)
		mana_compile_error("name followed by \"::\" must be an actor");

	mana_symbol_open_actor(symbol);
}

void mana_symbol_close_actor2(void)
{
	mana_symbol_actor_or_structure_level--;
	mana_symbol_close_block();
}

/*****************************************************************************/
/* module */
void mana_symbol_open_module(void)
{
	if(mana_symbol_block_level)
		mana_compile_error("the module declaration ignored");
	mana_symbol_open_block(MANA_FALSE);
	mana_symbol_actor_or_structure_level++;
	mana_symbol_actor_memory_address = 0;
}

mana_type_description* mana_symbol_close_module(char* name)
{
	mana_type_description* type;

	/* mana_symbol_entry*をmana_type_description*として代入しています
	 * 参照先でmana_symbol_entry*にキャストしています。
	 * TODO:危険なのでちゃんとメンバーを追加しましょう
	 */
	type = (mana_type_description*)(mana_symbol_block_table[mana_symbol_block_level].head);
	type = mana_type_create(MANA_DATA_TYPE_MODULE, type, NULL);
	type->name = name;
	type->alignment_memory_size = IBSZ;

	mana_symbol_actor_or_structure_level--;
	mana_symbol_close_block();

	type->memory_size = mana_symbol_align_size(mana_symbol_actor_memory_address, IBSZ);

	return type;
}

void mana_symbol_extend_module(char* name)
{
	mana_symbol_entry* symbol = mana_symbol_lookup(name);
	if(symbol && symbol->type && symbol->type->tcons == MANA_DATA_TYPE_MODULE)
	{
		mana_symbol_entry* action_symbol = (mana_symbol_entry*)(symbol->type->component);

		/* シンボルリストの末端からhashに登録 */
		if(action_symbol)
		{
			mana_symbol_entry* last_symbol = action_symbol;
			while(last_symbol->next)
			{
				last_symbol = last_symbol->next;
			}
			last_symbol->next = mana_symbol_block_table[mana_symbol_block_level].head;

			/* mana_symbol_close_blockでmana_symbol_hash_tableを開放する為 */
			mana_symbol_block_table[mana_symbol_block_level].head = action_symbol;

			mana_symbol_open_actor_register_member(action_symbol);
		}
	}
	else
	{
		mana_compile_error("name followed by \"::\" must be a module");
	}
}

/*****************************************************************************/
/* request */
void mana_symbol_add_request(unsigned char opcode, mana_node* level, mana_node* actor, char* action)
{
	mana_linker_expression(level, MANA_FALSE);

	if(actor && actor->type)
	{
		switch(actor->type->tcons)
		{
		case MANA_DATA_TYPE_ACTOR:
			mana_linker_expression(actor, MANA_FALSE);
			mana_code_set_opecode_and_operand(opcode, mana_data_set(action));
			return;

		case MANA_DATA_TYPE_REFERENCE:
			if(strcmp(actor->type->name, "string") == 0)
			{
				switch(opcode)
				{
				case MANA_IL_REQ:
					opcode = MANA_IL_DYNAMIC_REQ;
					break;
				case MANA_IL_REQWS:
					opcode = MANA_IL_DYNAMIC_REQWS;
					break;
				case MANA_IL_REQWE:
					opcode = MANA_IL_DYNAMIC_REQWE;
					break;
				default:
					goto ABORT;
				}
				mana_linker_expression(actor, MANA_FALSE);
				mana_code_set_opecode_and_operand(opcode, mana_data_set(action));
				return;
			}

		default:
			break;
		}
	}
ABORT:
	mana_compile_error("incompatible type of operand");
}

void mana_symbol_add_join(mana_node* level, mana_node* actor)
{
	if(actor && actor->type)
	{
		switch(actor->type->tcons)
		{
		case MANA_DATA_TYPE_REFERENCE:
			if(strcmp(actor->type->name, "string") != 0)
				break;

		case MANA_DATA_TYPE_ACTOR:
			mana_linker_expression(actor, MANA_FALSE);
			mana_linker_expression(level, MANA_FALSE);
			mana_code_set_opecode(MANA_IL_JOIN);
			return;

		default:
			break;
		}
	}
/*ABORT:*/
	mana_compile_error("incompatible type of operand");
}

int mana_symbol_get_number_of_actors(void)
{
	mana_symbol_entry* symbol;
	int level;
	int count = 0;

	for(level = 0; level < MANA_SYMBOL_BLOCK_TABLE_SIZE; level++)
	{
		for(symbol = mana_symbol_block_table[level].head; symbol; symbol = symbol->next)
		{
			assert(symbol->type);

			if(symbol->class_type == MANA_CLASS_TYPE_TYPEDEF)
			{
				switch(symbol->type->tcons)
				{
				case MANA_DATA_TYPE_ACTOR:
					count++;
					break;

				case MANA_DATA_TYPE_ARRAY:
					if(symbol->type->component && symbol->type->component->tcons == MANA_DATA_TYPE_ACTOR)
						count++;
					break;

				default:
					break;
				}
			}
		}
	}
	return count;
}

void mana_symbol_allocate_memory(mana_symbol_entry* symbol, mana_type_description* type, mana_symbol_memory_type_id parameter)
{
	if(type->tcons == MANA_DATA_TYPE_INCOMPLETE || type->tcons == MANA_DATA_TYPE_VOID)
	{
		mana_compile_error("incomplete type name or void is used for declraration");
		type = mana_type_get(MANA_DATA_TYPE_INT);
	}

	if(mana_variable_header_file)
	{
		mana_symbol_print_entry(symbol, type);
	}

	if(symbol->type == NULL)								/* 配列型以外の変数 ? */
	{
		symbol->type = type;								/* 型の設定 */
	}
	else if(symbol->type == type)							/* エラーの可能性がある */
	{
	}
	else
	{														/* 配列型の変数の処理 */
		mana_type_set_array(symbol->type, type);			/* 配列型リストの設定 */
		type = symbol->type;
	}

	if(type->memory_size > 0)
	{
		switch(symbol->class_type)
		{
		case MANA_CLASS_TYPE_VARIABLE_STATIC:
			symbol->address = mana_symbol_align_size(mana_symbol_static_memory_address, type->alignment_memory_size);
			mana_symbol_static_memory_address = symbol->address + type->memory_size;
			break;

		case MANA_CLASS_TYPE_VARIABLE_GLOBAL:
			symbol->address = mana_symbol_align_size(mana_symbol_global_memory_address, type->alignment_memory_size);
			mana_symbol_global_memory_address = symbol->address + type->memory_size;
			break;

		case MANA_CLASS_TYPE_VARIABLE_ACTOR:
			symbol->address = mana_symbol_align_size(mana_symbol_actor_memory_address, type->alignment_memory_size);
			mana_symbol_actor_memory_address = symbol->address + type->memory_size;
			break;

		case MANA_CLASS_TYPE_VARIABLE_LOCAL:
			/* ローカル変数は後ろから確保される為、他の変数と計算が反対になるので注意 */
			symbol->address = mana_symbol_align_size(mana_symbol_local_memory_address, type->alignment_memory_size) + type->memory_size;
			mana_symbol_local_memory_address = symbol->address;
			break;

		default:
			MANA_BUG("ivalid class type detect");
			break;
		}
	}
	else
	{
		mana_compile_error("no storage allocated");
	}

	/* 仮引数の表示の格納 */
	symbol->attrib = parameter;
}

static void mana_symbol_check_undefine_recursive(char* parent_name, mana_symbol_entry* symbol)
{
	while(symbol)
	{
		switch(symbol->class_type)
		{
		case MANA_CLASS_TYPE_PROTOTYPE_FUNCTION:
			if(parent_name)
				mana_compile_error("there's no definition of %s::%s", parent_name, symbol->name);
			else
				mana_compile_error("there's no definition of %s", symbol->name);
			break;

		case MANA_CLASS_TYPE_TYPEDEF:
			if(symbol->type->tcons == MANA_DATA_TYPE_ACTOR)
				mana_symbol_check_undefine_recursive(symbol->name, (mana_symbol_entry*)(symbol->type->component));
			break;

		default:
			break;
		}

		symbol = symbol->next;
	}
}

void mana_symbol_check_undefine(void)
{
	mana_symbol_check_undefine_recursive(NULL, mana_symbol_block_table[0].head);
}

void mana_symbol_print_header(void)
{
	if(mana_variable_header_file)
	{
		int i;

		for(i = 0; i <= mana_symbol_block_level; i++)
		{
			fputc('\t', mana_variable_header_file);
		}
		fprintf(mana_variable_header_file, "typedef struct {\n");
	}
}

void mana_symbol_print_footer(char* name, mana_type_description* type)
{
	int i;

	for(i = 0; i <= mana_symbol_block_level; i++)
	{
		fputc('\t', mana_variable_header_file);
	}
	fprintf(mana_variable_header_file, "}%s; /* %d byte(s) */\n", name, type->memory_size);
}

static void mana_symbol_print_entry_core(mana_symbol_entry* symbol, mana_type_description* type)
{
	int i;

	for(i = 0; i <= mana_symbol_block_level; i++)
	{
		fputc('\t', mana_variable_header_file);
	}
	fprintf(mana_variable_header_file, "%s\t%s", type->name, symbol->name);
	if(symbol->type != NULL)
	{
		mana_type_description* symbol_type;
		for(symbol_type = symbol->type; symbol_type; symbol_type = symbol_type->component)
		{
			fprintf(mana_variable_header_file, "[%d]", symbol_type->number_of_elements);
		}
	}
	fprintf(mana_variable_header_file, ";\n");
}

void mana_symbol_print_entry(mana_symbol_entry* symbol, mana_type_description* type)
{
	if(mana_symbol_block_level <= 0)
	{
		mana_symbol_print_entry_core(symbol, type);
	}
	else if(MANA_SYMBOL_IS_ACTOR_OR_STRUCTER_OPENED())
	{
		mana_symbol_print_entry_core(symbol, type);
	}
}

void mana_symbol_print_dummy_global_variable(size_t size)
{
	if(mana_variable_header_file)
	{
		unsigned d1 = (unsigned)ftell(mana_variable_header_file);
		unsigned d2 = (unsigned)rand();
		int i;

		for(i = 0; i <= mana_symbol_block_level; i++)
		{
			fputc('\t', mana_variable_header_file);
		}
		fprintf(mana_variable_header_file, "char\tdummy_%x_%x[%d];\n", d1, d2, (int)size);
	}
}

static int mana_symbol_get_number_of_actions(mana_type_description* type)
{
	mana_symbol_entry* symbol;
	int count = 0;

	for(symbol = (mana_symbol_entry*)type->component; symbol; symbol = symbol->next)
	{
		if(symbol->class_type == MANA_CLASS_TYPE_MEMBER_FUNCTION)
		{
			count++;
		}
	}
	return count;
}

/*****************************************************************************/
/* output */
static int mana_symbol_write_actor_infomation_data(
	FILE* out, mana_symbol_entry* symbol, mana_type_description* type, int number_of_elements)
{
	mana_actor_info_header actor_info;
	int number_of_actions;

	number_of_actions = mana_symbol_get_number_of_actions(type);
	if(number_of_actions > (1 << (8 * sizeof(actor_info.number_of_actions))))
	{
		mana_linker_error("Too much actions in %s.\n", symbol->name);
		return MANA_FALSE;
	}

	memset(&actor_info, 0, sizeof(mana_actor_info_header));
	actor_info.name = mana_data_get(symbol->name);
	actor_info.number_of_actions = (unsigned short)number_of_actions;
	actor_info.number_of_instances = (unsigned char)number_of_elements;
	actor_info.variable_size = type->memory_size;

	if(actor_info.name == (unsigned int)-1)
	{
		mana_linker_error("Can't find actor '%s'.\n", symbol->name);
		return MANA_FALSE;
	}

	if(type->share.actor.phantom)
	{
		actor_info.flag |= MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM;
	}

	if(fwrite(&actor_info, sizeof(mana_actor_info_header), 1, out) != 1)
	{
		mana_linker_error("File write failed.\n");
		return MANA_FALSE;
	}

	{
		mana_symbol_entry* component_symbol;
		for(component_symbol = (mana_symbol_entry*)type->component; component_symbol; component_symbol = component_symbol->next)
		{
			if(component_symbol->class_type == MANA_CLASS_TYPE_MEMBER_FUNCTION)
			{
				mana_action_info_header action_info;

				memset(&action_info, 0, sizeof(mana_action_info_header));
				action_info.name = mana_data_get(component_symbol->name);
				action_info.address = component_symbol->address;

				if(action_info.name == (unsigned int)-1)
				{
					mana_linker_error("Can't find action '%s'.\n", component_symbol->name);
					return MANA_FALSE;
				}

				if(fwrite(&action_info, sizeof(mana_action_info_header), 1, out) != 1)
				{
					mana_linker_error("File write failed.\n");
					return MANA_FALSE;
				}
			}
		}
	}
	return MANA_TRUE;
}

int mana_symbol_write_actor_infomation(FILE* out)
{
	mana_symbol_entry* symbol;

	for(symbol = mana_symbol_block_table[0].head; symbol; symbol = symbol->next)
	{
		MANA_VERIFY(symbol->type, "Null pointer error in mana_symbol_write_actor_infomation");
		switch(symbol->type->tcons)
		{
		case MANA_DATA_TYPE_ACTOR:
			if(symbol->class_type == MANA_CLASS_TYPE_TYPEDEF && symbol->type->tcons == MANA_DATA_TYPE_ACTOR)
			{
				if(!mana_symbol_write_actor_infomation_data(out, symbol, symbol->type, 1))
					return MANA_FALSE;
			}
			break;

		case MANA_DATA_TYPE_ARRAY:
			{
				mana_type_description* nested_type = symbol->type->component;
				int number_of_elements = symbol->type->number_of_elements;
				while(nested_type)
				{
					switch(nested_type->tcons)
					{
					case MANA_DATA_TYPE_ACTOR:
						if(symbol->class_type == MANA_CLASS_TYPE_TYPEDEF && nested_type->tcons == MANA_DATA_TYPE_ACTOR)
							if(!mana_symbol_write_actor_infomation_data(out, symbol, nested_type, number_of_elements))
								return MANA_FALSE;
						goto ESCAPE;

					case MANA_DATA_TYPE_ARRAY:
						number_of_elements *= nested_type->number_of_elements;
						break;

					default:
						break;
					}
					nested_type = nested_type->component;
				}
			}
ESCAPE:
		default:
			break;
		}
	}
	return MANA_TRUE;
}

/*****************************************************************************/
/* dump */
static void mana_type_dump_core(FILE* fp, mana_type_description* type)
{
	if(type)
	{
		fprintf(fp, "[%s]", mana_symbol_data_type_id_name[type->tcons]);
		if(	type->tcons == MANA_DATA_TYPE_ACTOR ||
			type->tcons == MANA_DATA_TYPE_MODULE ||
			type->tcons == MANA_DATA_TYPE_STRUCT)
			type = type->component;
		else
			type = 0;
		mana_type_dump_core(fp, type);
	}
}

static void mana_symbol_dump_core(FILE* fp, int level, mana_symbol_entry* symbol)
{
	while(symbol)
	{
		int i;

		for(i = 0; i < level; i++)
			fputc(' ', fp);

		fprintf(fp, "%s %s %08x ", mana_symbol_class_type_id_name[symbol->class_type], symbol->name, symbol->address);

		if(symbol->type)
			mana_type_dump_core(fp, symbol->type);

		fputc('\n', fp);

		if(symbol->type)
		{
			switch(symbol->type->tcons)
			{
				case MANA_DATA_TYPE_ACTOR:
				case MANA_DATA_TYPE_MODULE:
				case MANA_DATA_TYPE_STRUCT:
					if(symbol->type->component)
					{
						mana_symbol_dump_core(fp, level + 1, (mana_symbol_entry*)symbol->type->component);
					}
					break;

				default:
					break;
			}
		}

		if(symbol->parameter_list)
			mana_symbol_dump_core(fp, level + 1, symbol->parameter_list);

		symbol = symbol->next;
	}
}


void mana_symbol_dump(FILE* fp)
{
	mana_symbol_dump_core(fp, 0, mana_symbol_block_table[0].head);
}
