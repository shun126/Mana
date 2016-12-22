/*
 * mana (compiler)
 *
 * @file	mana_symbol.h
 * @brief	レジスタ割り当てに関するヘッダーファイル
 * @detail	このファイルはレジスタ割り当てに関するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
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

typedef enum mana_symbol_memory_type_id
{
	MANA_MEMORY_TYPE_NORMAL,							/*!< 変数型 */
	MANA_MEMORY_TYPE_PARAMETER							/*!< 引数型 */
} mana_symbol_memory_type_id;

typedef enum mana_symbol_class_type_id
{
	MANA_CLASS_TYPE_NEW_SYMBOL,							/*!< 未割り当て関数 */
	MANA_CLASS_TYPE_TYPEDEF,							/*!< 型定義 */
	MANA_CLASS_TYPE_PROTOTYPE_FUNCTION,					/*!< プロトタイプ関数宣言 */
	MANA_CLASS_TYPE_FUNCTION,							/*!< 関数 */
	MANA_CLASS_TYPE_NATIVE_FUNCTION,					/*!< 外部関数 */
	MANA_CLASS_TYPE_MEMBER_FUNCTION,					/*!< メンバー関数(アクション) */
	MANA_CLASS_TYPE_VARIABLE_STATIC,					/*!< スタティックグローバル変数 */
	MANA_CLASS_TYPE_VARIABLE_GLOBAL,					/*!< グローバル変数 */
	MANA_CLASS_TYPE_VARIABLE_ACTOR,						/*!< インスタンス変数 */
	MANA_CLASS_TYPE_VARIABLE_LOCAL,						/*!< ローカル変数 */
	MANA_CLASS_TYPE_CONSTANT_INT,						/*!< 整数 */
	MANA_CLASS_TYPE_CONSTANT_FLOAT,						/*!< 実数 */
	MANA_CLASS_TYPE_CONSTANT_STRING,					/*!< 文字列 */
	MANA_CLASS_TYPE_ALIAS,								/*!< データ参照 */
	MANA_CLASS_TYPE_LABEL,								/*!< ラベル */
	MANA_CLASS_TYPE_NUMBER_OF							/*!< mana_symbol_class_type_id項目数 */
} mana_symbol_class_type_id;

typedef enum mana_symbol_data_type_id
{
	MANA_DATA_TYPE_VOID,								/*!< void型 */
	MANA_DATA_TYPE_CHAR,								/*!< int8_t型 */
	MANA_DATA_TYPE_SHORT,								/*!< int16_t型 */
	MANA_DATA_TYPE_INT,									/*!< int32_t型 */
	MANA_DATA_TYPE_FLOAT,								/*!< float型 */
	MANA_DATA_TYPE_REFERENCE,							/*!< reference型 */
	MANA_DATA_TYPE_ARRAY,								/*!< array型 */
	MANA_DATA_TYPE_STRUCT,								/*!< struct型 */
	MANA_DATA_TYPE_ACTOR,								/*!< actor型 */
	MANA_DATA_TYPE_MODULE,								/*!< module型 */
	MANA_DATA_TYPE_NIL,									/*!< nil型 */
	MANA_DATA_TYPE_INCOMPLETE,							/*!< 宣言が未完了の型 */
	MANA_DATA_TYPE_NUMBER_OF							/*!< mana_symbol_data_type_idの項目数 */
} mana_symbol_data_type_id;

typedef enum mana_node_type_id
{
	MANA_NODE_TYPE_ARRAY,								/*!< variable[argument] = */
	MANA_NODE_TYPE_ASSIGN,								/*!< = */
	MANA_NODE_TYPE_MEMOP,								/*!< X.variable */
	MANA_NODE_TYPE_ARGUMENT,							/*!< 呼び出し側引数 */
	MANA_NODE_TYPE_CONST,								/*!< 定数 */
	MANA_NODE_TYPE_VARIABLE,							/*!< 変数 */
	MANA_NODE_TYPE_INCOMPLETE,							/*!< 宣言が未完了 */
	MANA_NODE_TYPE_FUNCTION,							/*!< 関数 */
	MANA_NODE_TYPE_CALL,								/*!< 関数呼び出し */
	MANA_NODE_TYPE_ADD,									/*!< 加算 */
	MANA_NODE_TYPE_SUB,									/*!< 減算 */
	MANA_NODE_TYPE_MUL,									/*!< 乗算 */
	MANA_NODE_TYPE_DIV,									/*!< 除算 */
	MANA_NODE_TYPE_REM,									/*!< 余剰 */
	MANA_NODE_TYPE_NEG,									/*!< ±符号反転 */
	MANA_NODE_TYPE_POW,									/*!< べき乗 */
	MANA_NODE_TYPE_NOT,									/*!< ~ */
	MANA_NODE_TYPE_AND,									/*!< & */
	MANA_NODE_TYPE_OR,									/*!< | */
	MANA_NODE_TYPE_XOR,									/*!< ^ */
	MANA_NODE_TYPE_LSH,									/*!< << */
	MANA_NODE_TYPE_RSH,									/*!< >> */
	MANA_NODE_TYPE_LS,									/*!< < */
	MANA_NODE_TYPE_LE,									/*!< <= */
	MANA_NODE_TYPE_EQ,									/*!< == */
	MANA_NODE_TYPE_NE,									/*!< != */
	MANA_NODE_TYPE_GE,									/*!< >= */
	MANA_NODE_TYPE_GT,									/*!< > */
	MANA_NODE_TYPE_STRING,								/*!< 文字列 */
	MANA_NODE_TYPE_I2F,									/*!< 整数から実数へ変換 */
	MANA_NODE_TYPE_F2I,									/*!< 実数から整数へ変換 */
	MANA_NODE_TYPE_LOR,									/*!< || */
	MANA_NODE_TYPE_LAND,								/*!< && */
	MANA_NODE_TYPE_LNOT,								/*!< ! */
	MANA_NODE_TYPE_SENDER,								/*!< sender (actor) */
	MANA_NODE_TYPE_SELF,								/*!< self (actor) */
	MANA_NODE_TYPE_PRIORITY,							/*!< priority (integer) */
	MANA_NODE_TYPE_EXPRESSION_IF,						/*!< 三項演算子 '?' */


	MANA_NODE_TYPE_NEWLINE,								/*!< 改行 */
	MANA_NODE_TYPE_BLOCK,								/*!< ブロック */
	MANA_NODE_TYPE_ASSIGN,								/*!< 代入 */
	MANA_NODE_TYPE_IF,									/*!< if */
	MANA_NODE_TYPE_SWITCH,								/*!< switch */
	MANA_NODE_TYPE_CASE,								/*!< case */
	MANA_NODE_TYPE_DEFAULT,								/*!< default */
	MANA_NODE_TYPE_WHILE,								/*!< while */
	MANA_NODE_TYPE_DO,									/*!< do while */
	MANA_NODE_TYPE_FOR,									/*!< for */
	MANA_NODE_TYPE_LOOP,								/*!< loop */
	MANA_NODE_TYPE_LOCK,								/*!< lock */
	MANA_NODE_TYPE_GOTO,								/*!< goto */
	MANA_NODE_TYPE_LABEL,								/*!< label */
	MANA_NODE_TYPE_RETURN,								/*!< return */
	MANA_NODE_TYPE_ROLLBACK,							/*!< rollback */
	MANA_NODE_TYPE_BREAK,								/*!< break */
	MANA_NODE_TYPE_CONTINUE,							/*!< continue */
	MANA_NODE_TYPE_HALT,								/*!< halt */
	MANA_NODE_TYPE_YIELD,								/*!< yield */
	MANA_NODE_TYPE_REQUEST,								/*!< req */
	MANA_NODE_TYPE_COMPLY,								/*!< comply (req許可) */
	MANA_NODE_TYPE_REFUSE,								/*!< refuse (req拒否) */
	MANA_NODE_TYPE_JOIN,								/*!< join */
	MANA_NODE_TYPE_PRINT,								/*!< print */
} mana_node_type_id;

typedef struct mana_symbol_entry
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct mana_symbol_entry* next;						/*!< 次のシンボルへのリンク */
	struct mana_symbol_entry* hash_chain;				/*!< ハッシュチェイン */
	struct mana_symbol_entry* parameter_list;			/*!< パラメータへのリンク */
	struct mana_type_description* type;					/*!< mana_type_descriptionのリンク */
	mana_symbol_class_type_id class_type;				/*!< シンボルの種類ID */
	mana_symbol_memory_type_id attrib;					/*!< シンボルのメモリの種類 */
	char* name;											/*!< 名称 */
	int32_t address;									/*!< アドレス */
	int32_t etc;										/*!< 汎用(廃止予定) */
	float hloat;										/*!< 浮動小数点保存バッファ */
	char* string;										/*!< 文字列保存バッファ */
	int32_t define_level;								/*!< 定義レベル */
	int32_t number_of_parameters;						/*!< パラメータの数 */
	unsigned used;										/*!< 参照フラグ */
} mana_symbol_entry;

typedef struct mana_type_description
{
#if defined(_DEBUG)
	char magic[8];
#endif
	struct mana_type_description* parent;				/*!< 親の型へのリンク(継承がなくなったので未使用) */
	struct mana_type_description* next;					/*!< 次の型へのリンク */
	struct mana_type_description* component;			/*!< 配列、参照型の元の型へのリンク */
	mana_symbol_data_type_id tcons;						/*!< 型のID */
	char* name;											/*!< 型の名称 */
	int32_t number_of_elements;							/*!< 配列のサイズ */
	int32_t memory_size;								/*!< メモリサイズ */
	int32_t alignment_memory_size;						/*!< アライメントサイズ */

	union share
	{
		struct actor
		{
			int32_t phantom;							/*!< phantom? */
		}actor;
	}share;
} mana_type_description;

typedef struct mana_node
{
#if defined(_DEBUG)
	char magic[8];
#endif
	mana_node_type_id id;								/*!< ノードID */
	struct mana_symbol_entry* symbol;					/*!< シンボルへのポインタ */
	struct mana_type_description* type;					/*!< 型へのポインタ */
	struct mana_node* left;								/*!< 左 */
	struct mana_node* right;							/*!< 右 */
	struct mana_node* condition;						/*!< 条件 */
	int32_t etc;										/*!< その他 */
	int32_t digit;										/*!< 整数 */
	float real;											/*!< 少数 */
	char* string;										/*!< 文字列へのポインタ */
} mana_node;

/*****************************************************************************/
extern int32_t mana_symbol_frame_size_list;				/*!< action,functionのフレームサイズリスト */
extern int32_t mana_symbol_return_address_list;			/*!< action,functionのリターンアドレスリスト */

/*****************************************************************************/
extern void mana_symbol_initialize(void);
extern void mana_symbol_finalize(void);

extern void mana_symbol_open_block(const bool reset_max_frame_memory_address);
extern void mana_symbol_close_block(void);

extern int32_t mana_symbol_is_valid_variable(mana_symbol_entry*);

extern int32_t mana_symbol_get_static_memory_address();
extern void mana_symbol_set_static_memory_address(int32_t size);

extern int32_t mana_symbol_get_global_memory_address();
extern void mana_symbol_set_global_memory_address(int32_t size);

extern mana_symbol_entry* mana_symbol_lookup(char*);
extern mana_symbol_entry* mana_symbol_lookup_or_create_dummy(char*);
extern mana_symbol_entry* mana_symbol_create_alias(char*, char*);
extern mana_symbol_entry* mana_symbol_create_const_int(char*, int32_t);
extern mana_symbol_entry* mana_symbol_create_const_float(char*, float);
extern mana_symbol_entry* mana_symbol_create_const_string(char*, char*);
//extern mana_symbol_entry* mana_symbol_create_type(char*);
extern mana_symbol_entry* mana_symbol_create_identification(char*, mana_type_description*, int32_t static_variable);
extern mana_symbol_entry* mana_symbol_create_label(char*);
extern mana_symbol_entry* mana_symbol_create_function(char*);
extern void mana_symbol_create_prototype(mana_symbol_entry*, mana_type_description*);

extern void mana_symbol_destroy(char* name);

extern void mana_symbol_open_function(int32_t, mana_symbol_entry*, mana_type_description*);
extern void mana_symbol_close_function(mana_symbol_entry*);
extern void mana_symbol_set_native_function(mana_symbol_entry*, mana_type_description*);
extern void mana_symbol_set_type(char*, mana_type_description*);

extern void mana_symbol_open_structure(void);
extern mana_type_description* mana_symbol_close_structure(char*);

extern void mana_symbol_open_actor(mana_symbol_entry*);
extern mana_type_description* mana_symbol_close_actor(char* name, char* parent, mana_type_description* td, int32_t phantom);

extern void mana_symbol_open_actor2(mana_symbol_entry*);
extern void mana_symbol_close_actor2(void);

extern void mana_symbol_open_module(void);
extern mana_type_description* mana_symbol_close_module(char* name);
extern void mana_symbol_extend_module(char* name);

extern int32_t mana_symbol_get_number_of_actors(void);

extern void mana_symbol_add_request(uint8_t opcode, mana_node* level, mana_node* actor, char* action);
extern void mana_symbol_add_join(mana_node* level, mana_node* actor);

extern void mana_symbol_allocate_memory(mana_symbol_entry*, mana_type_description*, mana_symbol_memory_type_id);
extern void mana_symbol_check_undefine(void);

extern void mana_symbol_print_header(void);
extern void mana_symbol_print_footer(char*, mana_type_description*);
extern void mana_symbol_print_entry(mana_symbol_entry*, mana_type_description*);
extern void mana_symbol_print_dummy_global_variable(size_t size);

extern bool mana_symbol_write_actor_infomation(mana_stream* stream);

extern void mana_symbol_dump(FILE*);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
