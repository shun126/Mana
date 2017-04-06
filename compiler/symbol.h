/*!
mana (compiler)

@file	symbol.h
@brief	レジスタ割り当てに関するヘッダーファイル
@detail	このファイルはレジスタ割り当てに関するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_SYMBOL_H___)
#define ___MANA_SYMBOL_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <libmana.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum symbol_memory_type_id
{
	MEMORY_TYPE_NORMAL,						//!< 変数型
	MEMORY_TYPE_PARAMETER					//!< 引数型
} symbol_memory_type_id;

typedef enum symbol_class_type_id
{
	SYMBOL_CLASS_TYPE_NEW_SYMBOL,			//!< 未割り当て関数
	SYMBOL_CLASS_TYPE_TYPEDEF,				//!< 型定義
	SYMBOL_CLASS_TYPE_FUNCTION,				//!< 関数
	SYMBOL_CLASS_TYPE_NATIVE_FUNCTION,		//!< 外部関数
	SYMBOL_CLASS_TYPE_MEMBER_FUNCTION,		//!< メンバー関数(アクション)
	SYMBOL_CLASS_TYPE_VARIABLE_STATIC,		//!< スタティックグローバル変数
	SYMBOL_CLASS_TYPE_VARIABLE_GLOBAL,		//!< グローバル変数
	SYMBOL_CLASS_TYPE_VARIABLE_ACTOR,		//!< インスタンス変数
	SYMBOL_CLASS_TYPE_VARIABLE_LOCAL,		//!< ローカル変数
	SYMBOL_CLASS_TYPE_CONSTANT_INT,			//!< 整数
	SYMBOL_CLASS_TYPE_CONSTANT_FLOAT,		//!< 実数
	SYMBOL_CLASS_TYPE_CONSTANT_STRING,		//!< 文字列
	SYMBOL_CLASS_TYPE_ALIAS,				//!< データ参照
	SYMBOL_CLASS_TYPE_LABEL,				//!< ラベル
	SYMBOL_CLASS_TYPE_NUMBER_OF				//!< symbol_class_type_id項目数
} symbol_class_type_id;

typedef enum symbol_data_type_id
{
	SYMBOL_DATA_TYPE_VOID,					//!< void型
	SYMBOL_DATA_TYPE_CHAR,					//!< int8_t型
	SYMBOL_DATA_TYPE_SHORT,					//!< int16_t型
	SYMBOL_DATA_TYPE_INT,					//!< int32_t型
	SYMBOL_DATA_TYPE_FLOAT,					//!< float型
	SYMBOL_DATA_TYPE_REFERENCE,				//!< reference型
	SYMBOL_DATA_TYPE_ARRAY,					//!< array型
	SYMBOL_DATA_TYPE_STRUCT,				//!< struct型
	SYMBOL_DATA_TYPE_ACTOR,					//!< actor型
	SYMBOL_DATA_TYPE_MODULE,				//!< module型
	SYMBOL_DATA_TYPE_NIL,					//!< nil型
	SYMBOL_DATA_TYPE_INCOMPLETE,			//!< 宣言が未完了の型
	SYMBOL_DATA_TYPE_NUMBER_OF				//!< symbol_data_type_idの項目数
} symbol_data_type_id;

typedef enum node_id
{
	NODE_ARRAY,								//!< variable[argument] =
	NODE_ASSIGN,							//!< =
	NODE_CALL_ARGUMENT,						//!< 引数（呼び出し）
	NODE_DECLARE_ARGUMENT,					//!< 引数（宣言）
	NODE_CONST,								//!< 定数
	NODE_CALL,								//!< 関数呼び出し
	NODE_ADD,								//!< 加算
	NODE_SUB,								//!< 減算
	NODE_MUL,								//!< 乗算
	NODE_DIV,								//!< 除算
	NODE_REM,								//!< 余剰
	NODE_NEG,								//!< ±符号反転
	NODE_POW,								//!< べき乗
	NODE_NOT,								//!< ~
	NODE_AND,								//!< &
	NODE_OR,								//!< |
	NODE_XOR,								//!< ^
	NODE_LSH,								//!< <<
	NODE_RSH,								//!< >>
	NODE_LS,								//!< <
	NODE_LE,								//!< <=
	NODE_EQ,								//!< ==
	NODE_NE,								//!< !=
	NODE_GE,								//!< >=
	NODE_GT,								//!< >
	NODE_STRING,							//!< 文字列
	NODE_I2F,								//!< 整数から実数へ変換
	NODE_F2I,								//!< 実数から整数へ変換
	NODE_LOR,								//!< ||
	NODE_LAND,								//!< &&
	NODE_LNOT,								//!< !
	NODE_HALT,								//!< halt
	NODE_YIELD,								//!< yield
	NODE_REQUEST,							//!< req
	NODE_COMPLY,							//!< comply (req許可)
	NODE_REFUSE,							//!< refuse (req拒否)
	NODE_JOIN,								//!< join
	NODE_SENDER,							//!< sender (actor)
	NODE_SELF,								//!< self (actor)
	NODE_PRIORITY,							//!< priority (integer)
	NODE_EXPRESSION_IF,						//!< 三項演算子 '?'
	NODE_PRINT,								//!< print
	NODE_RETURN,							//!< return
	NODE_ROLLBACK,							//!< rollback

	NODE_BLOCK,								//!< ブロック
	NODE_IF,								//!< if
	NODE_SWITCH,							//!< switch
	NODE_CASE,								//!< case
	NODE_DEFAULT,							//!< default
	NODE_WHILE,								//!< while
	NODE_DO,								//!< do while
	NODE_FOR,								//!< for
	NODE_LOOP,								//!< loop
	NODE_LOCK,								//!< lock
	NODE_GOTO,								//!< goto
	NODE_LABEL,								//!< label
	NODE_BREAK,								//!< break
	NODE_CONTINUE,							//!< continue

	NODE_IDENTIFIER,
	NODE_TYPE_DESCRIPTION,
	NODE_DECLARATOR,

	NODE_DECLARE_VARIABLE,
	NODE_SIZEOF,

	NODE_DECLARE_ACTOR,						//!< アクターの宣言
	NODE_DECLARE_PHANTOM,
	NODE_DECLARE_MODULE,
	NODE_DECLARE_STRUCT,
	NODE_DECLARE_ACTION,
	NODE_DECLARE_EXTEND,
	NODE_DECLARE_ALLOCATE,
	NODE_DECLARE_STATIC,
	NODE_DEFINE_ALIAS,
	NODE_DECLARE_NATIVE_FUNCTION,
	NODE_DECLARE_FUNCTION,

	NODE_DEFINE_CONSTANT,
	NODE_UNDEFINE_CONSTANT,

	NODE_MEMBER_FUNCTION,
	NODE_MEMBER_VARIABLE,

	NODE_VARIABLE_SIZE,
} node_id;

typedef struct symbol_entry
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct symbol_entry* next;				//!< 次のシンボルへのリンク
	struct symbol_entry* hash_chain;		//!< ハッシュチェイン
	struct symbol_entry* parameter_list;	//!< パラメータへのリンク
	struct type_description* type;			//!< type_descriptionのリンク
	symbol_class_type_id class_type;		//!< シンボルの種類ID
	symbol_memory_type_id attrib;			//!< シンボルのメモリの種類
	const char* name;						//!< 名称
	int32_t address;						//!< アドレス
	int32_t etc;							//!< 汎用(廃止予定)
	float hloat;							//!< 浮動小数点保存バッファ
	const char* string;						//!< 文字列保存バッファ
	int32_t define_level;					//!< 定義レベル
	int32_t number_of_parameters;			//!< パラメータの数
	bool override;							//!< 上書き許可
	bool used;								//!< 参照フラグ

	struct symbol_entry* link;				//!< 解放用ポインタ
} symbol_entry;

typedef struct type_description
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct type_description* parent;		//!< 親の型へのリンク(継承がなくなったので未使用)
	struct type_description* component;		//!< 配列、参照型の元の型へのリンク
	symbol_data_type_id tcons;				//!< 型のID
	const char* name;						//!< 型の名称
	int32_t number_of_elements;				//!< 配列のサイズ
	int32_t memory_size;					//!< メモリサイズ
	int32_t alignment_memory_size;			//!< アライメントサイズ

	union share
	{
		struct actor
		{
			int32_t phantom;				//!< phantom?
		}actor;
	}share;

	struct type_description* link;			//!< 解放用ポインタ
} type_description;

typedef struct node_entry
{
#if defined(_DEBUG)
	char magic[8];
#endif
	node_id id;								//!< ノードID
	struct symbol_entry* symbol;			//!< シンボルへのポインタ
	struct type_description* type;			//!< 型へのポインタ
	struct node_entry* left;				//!< 左
	struct node_entry* right;				//!< 右
	struct node_entry* body;				//!< 右
	struct node_entry* next;				//!< 次
	int32_t etc;							//!< その他
	int32_t digit;							//!< 整数
	float real;								//!< 少数
	const char* string;						//!< 文字列へのポインタ

	const char* filename;					//!< ファイル名
	int32_t line;							//!< 行番号
	struct node_entry* link;				//!< 解放用ポインタ
} node_entry;

/*****************************************************************************/
extern int32_t mana_symbol_frame_size_list;				/*!< action,functionのフレームサイズリスト */
extern int32_t mana_symbol_return_address_list;			/*!< action,functionのリターンアドレスリスト */

/*****************************************************************************/
extern void mana_symbol_initialize(void);
extern void mana_symbol_finalize(void);

extern int32_t mana_symbol_open_block(const bool reset_max_frame_memory_address);
extern int32_t mana_symbol_close_block(void);

extern symbol_entry* mana_symbol_get_head_symbol(void);

extern int32_t mana_symbol_is_valid_variable(symbol_entry*);

extern int32_t mana_symbol_get_static_memory_address();
extern void mana_symbol_set_static_memory_address(const int32_t size);

extern int32_t mana_symbol_get_global_memory_address();
extern void mana_symbol_set_global_memory_address(const int32_t size);

extern symbol_entry* mana_symbol_lookup(const char* name);
extern symbol_entry* mana_symbol_lookup_or_create_dummy(const char* name);
extern symbol_entry* mana_symbol_create_alias(const char*, const char*);
extern symbol_entry* mana_symbol_create_const_int(const char*, const int32_t);
extern symbol_entry* mana_symbol_create_const_float(const char*, const float);
extern symbol_entry* mana_symbol_create_const_string(const char*, const char*);
extern symbol_entry* mana_symbol_create_variable(const char* name, type_description* type, const bool static_variable);
extern symbol_entry* mana_symbol_create_label(const char*);

extern void mana_symbol_destroy(const char* name);

// function
extern symbol_entry* mana_symbol_create_function(const char* name);
extern void mana_symbol_open_function(node_entry* node, const bool is_action);
extern void mana_symbol_close_function(node_entry* node, const bool is_action);

extern void mana_symbol_begin_native_function_registration();
extern void mana_symbol_commit_native_function_registration(symbol_entry* function, type_description* type);

////////////////////////////////////////////////////////////////////////////////////////////////////
// struct
extern void mana_symbol_open_structure(void);
extern void mana_symbol_close_structure(const char* name);

////////////////////////////////////////////////////////////////////////////////////////////////////
// actor
/*!
アクターシンボルの登録を開始します
@param[in]	symbol	NULLならば新規作成
*/
extern void mana_symbol_begin_registration_actor(symbol_entry* symbol);

/*!
アクターシンボルの登録を確定します
*/
extern void mana_symbol_commit_registration_actor(const char* name, const char* parent, type_description* td, const bool phantom);

/*!
アクターのシンボル登録を開始します
@param[in]	symbol	
*/
extern void mana_symbol_open_actor(const char* name);
extern void mana_symbol_close_actor();

////////////////////////////////////////////////////////////////////////////////////////////////////
// module
extern void mana_symbol_begin_registration_module(symbol_entry* symbol);
extern void mana_symbol_commit_registration_module(const char* name);

extern void mana_symbol_open_module(symbol_entry* symbol);
extern void mana_symbol_close_module(const char* name);

extern void mana_symbol_extend_module(const char* name);

////////////////////////////////////////////////////////////////////////////////////////////////////
extern void mana_symbol_set_type(const char* name, type_description* type);

extern int32_t mana_symbol_get_number_of_actors(void);

extern void mana_symbol_add_request(uint8_t opcode, node_entry* level, node_entry* actor, const char* action);
extern void mana_symbol_add_join(node_entry* level, node_entry* actor);

extern void mana_symbol_allocate_memory(symbol_entry*, type_description*, symbol_memory_type_id);
extern void mana_symbol_check_undefine(void);

extern void mana_symbol_print_header(void);
extern void mana_symbol_print_footer(const char*, type_description*);
extern void mana_symbol_print_entry(symbol_entry*, type_description*);
extern void mana_symbol_print_dummy_global_variable(size_t size);

extern bool mana_symbol_write_actor_infomation(mana_stream* stream);

extern void mana_symbol_dump(FILE*);
extern void mana_symbol_dump_function_symbol_from_address(FILE* log, const int32_t address);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
