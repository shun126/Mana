/*!
 * mana (library)
 *
 * @file	mana_info.h
 * @brief	manaコンパイラとmanaライブラリで共通利用するヘッダーファイル
 * @detail	このファイルはmanaコンパイラとmanaライブラリで共通利用するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_INFO_H___)
#define ___MANA_INFO_H___

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MANA_SIGNATURE					"MaNa"		/*!< manaファイル署名 */
#define MANA_MAJOR_VERSION				(0)			/*!< manaファイルのメジャーバージョン番号 */
#define MANA_MINOR_VERSION				(12)		/*!< manaファイルのマイナーバージョン番号 */
#define MANA_ACTOR_MAX_INTERRUPT_LEVEL	(32)		/*!< 優先度数 */
#define MANA_FUNCTION_HASH_SIZE			(127)		/*!< 外部関数ハッシュテーブルサイズ(素数が好ましい) */
#define MANA_REGISTER_COUNT				(8)			/*!< レジスタの数 */

/*! @brief	π */
#define MANA_PI (3.1415926535897932384626433832795028841971693993751f)

#define MANA_FALSE 0								/*!< FALSE 偽 */
#define MANA_TRUE  1								/*!< TRUE 真 */
typedef int mana_bool;								/*!< 論理型 */

/*! manaファイルヘッダー */
typedef struct mana_file_header
{
	char header[4];									/*!< 署名 */
	unsigned char major_version;					/*!< メジャーバージョン */
	unsigned char minor_version;					/*!< マイナーバージョン */
	unsigned char pad;								/*!< パディング */
	unsigned char flag;								/*!< フラグ */
/*
mana_file_header :: flag

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  |   |   |   |   |   |   |   |
  |   |   |   |   |   |   |   +---	MANA_HEADER_FLAG_RESOURCE
  |   |   |   |   |   |   +-------	MANA_HEADER_FLAG_BIG_ENDIAN
  |   |   |   |   |   +-----------
  |   |   |   |   +---------------
  |   |   |   +-------------------
  |   |   +-----------------------
  |   +---------------------------
  +-------------------------------	MANA_HEADER_FLAG_COMPILED
*/
#define MANA_HEADER_FLAG_RESOURCE	0x01			/*!< フラグ：リソースセクションあり */
#define MANA_HEADER_FLAG_BIG_ENDIAN	0x02			/*!< フラグ：ビックエンディアンでコンパイルされた */
#define MANA_HEADER_FLAG_COMPILED	0x07			/*!< フラグ：コンパイル済み */

	unsigned int number_of_actors;					/*!< アクター数 */
	unsigned int size_of_constant_pool;				/*!< 定数セクションサイズ */
	unsigned int size_of_instruction_pool;			/*!< 命令セクションサイズ */
	unsigned int size_of_static_memory;				/*!< スタティックメモリサイズ */
	unsigned int size_of_global_memory;				/*!< グローバルメモリサイズ */
	unsigned int random_seed_number;				/*!< 乱数の種 */
}mana_file_header;

/*! manaファイル内のアクター情報ヘッダー */
typedef struct mana_actor_info_header
{
	unsigned char flag;								/*!< フラグ */
/*
mana_actor_info_header :: flag

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  |   |   |   |   |   |   |   |
  |   |   |   |   |   |   |   +---	MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM
  |   |   |   |   |   |   +-------
  |   |   |   |   |   +-----------
  |   |   |   |   +---------------
  |   |   |   +-------------------
  |   |   +-----------------------
  |   +---------------------------
  +-------------------------------
*/
#define MANA_ACTOR_INFO_HEADER_FLAG_PHANTOM	0x01	/*!< フラグ：ファントム */

	unsigned char number_of_instances;				/*!< インスタンス数（未使用） */
	unsigned short number_of_actions;				/*!< アクション数 */
	unsigned int name;								/*!< アクター名 */
	unsigned int variable_size;						/*!< インスタンス変数サイズ */
}mana_actor_info_header;

/*! manaファイル内のアクション情報ヘッダー */
typedef struct mana_action_info_header
{
	unsigned int name;								/*!< アクション名 */
	unsigned int address;							/*!< アドレス */
}mana_action_info_header;

/*!  */
enum mana_intermediate_language
{
	/* thread */
	MANA_IL_RESTART = 0,							/* 00 */
	MANA_IL_HALT,									/* 01 */
	MANA_IL_YIELD,									/* 02 */
	MANA_IL_NONPREEMPTIVE,							/* 03 */
	MANA_IL_PREEMPTIVE,								/* 04 */

	/* jump */
	MANA_IL_BEQ,									/* 05 */
	MANA_IL_BNE,									/* 06 */
	MANA_IL_BRA,									/* 07 */
	MANA_IL_BSR,									/* 08 */
	MANA_IL_CALL,									/* 09 */
	MANA_IL_REQ,									/* 0A */
	MANA_IL_REQWS,									/* 0B */
	MANA_IL_REQWE,									/* 0C */
	MANA_IL_JOIN,									/* 0D */
	MANA_IL_COMPLY,									/* 0E */
	MANA_IL_REFUSE,									/* 0F */
	MANA_IL_LOAD_RETURN_ADDRESS,					/* 10 */
	MANA_IL_SAVE_RETURN_ADDRESS,					/* 11 */
	MANA_IL_RETURN_FROM_FUNCTION,					/* 12 */
	MANA_IL_RETURN_FROM_ACTION,						/* 13 */
	MANA_IL_ROLLBACK,								/* 14 */

	/* constant */
	MANA_IL_PUSH_ZERO_INTEGER,						/* 15 */
	MANA_IL_PUSH_ZERO_FLOAT,						/* 16 */
	MANA_IL_PUSH_CHAR,								/* 17 */
	MANA_IL_PUSH_SHORT,								/* 18 */
	MANA_IL_PUSH_INTEGER,							/* 19 */
	MANA_IL_PUSH_FLOAT,								/* 1A */
	MANA_IL_PUSH_STRING,							/* 1B */
	MANA_IL_PUSH_PRIORITY,							/* 1C */
	MANA_IL_PUSH_ACTOR,								/* 1D */
	MANA_IL_PUSH_SELF,								/* 1E */
	MANA_IL_PUSH_SENDER,							/* 1F */

	/* stack */
	MANA_IL_ALLOCATE,								/* 20 */
	MANA_IL_FREE,									/* 21 */
	MANA_IL_DUPLICATE,								/* 22 */
	MANA_IL_DUPLICATE_DATA,							/* 23 */
	MANA_IL_REMOVE,									/* 24 */
	MANA_IL_REMOVE_DATA,							/* 25 */
	MANA_IL_LOAD_STATIC_ADDRESS,					/* 26 */
	MANA_IL_LOAD_GLOBAL_ADDRESS,					/* 27 */
	MANA_IL_LOAD_FRAME_ADDRESS,						/* 28 */
	MANA_IL_LOAD_SELF_ADDRESS,						/* 29 */

	/* memory operation */
	MANA_IL_LOAD_CHAR,								/* 2A */
	MANA_IL_LOAD_SHORT,								/* 2B */
	MANA_IL_LOAD_INTEGER,							/* 2C */
	MANA_IL_LOAD_FLOAT,								/* 2D */
	MANA_IL_LOAD_REFFRENCE,							/* 2E */
	MANA_IL_LOAD_DATA,								/* 2F */
	MANA_IL_STORE_CHAR,								/* 30 */
	MANA_IL_STORE_SHORT,							/* 31 */
	MANA_IL_STORE_INTEGER,							/* 32 */
	MANA_IL_STORE_FLOAT,							/* 33 */
	MANA_IL_STORE_REFFRENCE,						/* 34 */
	MANA_IL_STORE_DATA,								/* 35 */

	/* caluclation */
	MANA_IL_ADD_INTEGER,							/* 36 */
	MANA_IL_DIV_INTEGER,							/* 37 */
	MANA_IL_MINUS_INTEGER,							/* 38 */
	MANA_IL_MOD_INTEGER,							/* 39 */
	MANA_IL_MUL_INTEGER,							/* 3A */
	MANA_IL_SUB_INTEGER,							/* 3B */

	MANA_IL_ADD_FLOAT,								/* 3C */
	MANA_IL_DIV_FLOAT,								/* 3D */
	MANA_IL_MINUS_FLOAT,							/* 3E */
	MANA_IL_MOD_FLOAT,								/* 3F */
	MANA_IL_MUL_FLOAT,								/* 40 */
	MANA_IL_SUB_FLOAT,								/* 41 */

	MANA_IL_AND,									/* 42 */
	MANA_IL_EOR,									/* 43 */
	MANA_IL_LAND,									/* 44 */
	MANA_IL_LOR,									/* 45 */
	MANA_IL_LNOT,									/* 46 */
	MANA_IL_NOT,									/* 47 */
	MANA_IL_OR,										/* 48 */
	MANA_IL_SHL,									/* 49 */
	MANA_IL_SHR,									/* 4A */

	MANA_IL_INT2FLOAT,								/* 4B */
	MANA_IL_FLOAT2INT,								/* 4C */

	/* compare and branch */
	MANA_IL_COMPARE_EQ_INTEGER,						/* 4D */
	MANA_IL_COMPARE_NE_INTEGER,						/* 4E */
	MANA_IL_COMPARE_GE_INTEGER,						/* 4F */
	MANA_IL_COMPARE_GT_INTEGER,						/* 50 */
	MANA_IL_COMPARE_LE_INTEGER,						/* 51 */
	MANA_IL_COMPARE_LS_INTEGER,						/* 52 */

	MANA_IL_COMPARE_EQ_FLOAT,						/* 53 */
	MANA_IL_COMPARE_NE_FLOAT,						/* 54 */
	MANA_IL_COMPARE_GE_FLOAT,						/* 55 */
	MANA_IL_COMPARE_GT_FLOAT,						/* 56 */
	MANA_IL_COMPARE_LE_FLOAT,						/* 57 */
	MANA_IL_COMPARE_LS_FLOAT,						/* 58 */

	MANA_IL_COMPARE_EQ_DATA,						/* 59 */
	MANA_IL_COMPARE_NE_DATA,						/* 5A */
	MANA_IL_COMPARE_GE_DATA,						/* 5B */
	MANA_IL_COMPARE_GT_DATA,						/* 5C */
	MANA_IL_COMPARE_LE_DATA,						/* 5D */
	MANA_IL_COMPARE_LS_DATA,						/* 5E */

	/* utility */
	MANA_IL_PRINT,									/* 5F */

	/* under discussion */
	MANA_IL_DYNAMIC_REQ,							/* 60 */
	MANA_IL_DYNAMIC_REQWS,							/* 61 */
	MANA_IL_DYNAMIC_REQWE,							/* 62 */

	MANA_IL_MAX_SIZE
};

/*! 乱数を初期化 */
extern void mana_srand(const unsigned int seed);

/*! 乱数を取得 */
extern int mana_rand(void);

/*! ０から１までの乱数 */
#define mana_frand()	(((float)rand() & 0x7FFF) / 32767.0f)

/*! 角度をラジアンに変換 */
#define mana_to_radian(degree)	(degree * (MANA_PI / 180.f))

/*! ラジアンを角度に変換 */
#define mana_to_degree(radian)	(radian * (180.0f / MANA_PI))

/*! メモリの状態取得 */
extern int mana_is_big_endian(void);

/*! short型のエンディアンを変換します */
extern short mana_swap_endian_short(const short value);

/*! unsigned short型のエンディアンを変換します */
extern unsigned short mana_swap_endian_unsigned_short(const unsigned short value);

/*! int型のエンディアンを変換します */
extern int mana_swap_endian_integer(const int value);

/*! unsigned int型のエンディアンを変換します */
extern unsigned int mana_swap_endian_unsigned_integer(const unsigned int value);

/*! float型のエンディアンを変換します */
extern float mana_swap_endian_float(const float value);

/*! コンソールに文字列を出力します */
extern void mana_print_debug(const char* format, ...);

/*! 文字列の中から文字列を検索 */
extern int mana_string_find(const char text[], const char pattern[]);

/*! ファイルサイズを取得 */
extern long mana_get_file_size(const char* filename);

/*! ファイルを読み込みます(エンディアン変換には対応していません) */
extern int mana_read(void** buffer, size_t* size, const char* filename);

/*! 命令のサイズを取得 */
extern int mana_get_instruction_size(const unsigned char* program);

/*! オペコードとオペランドの文字列を取得 */
extern const char* mana_get_instruction_text(const char* data, const void* program, const int address);

#if defined(NDEBUG)
/*! コンソールに文字列を出力 */
#define MANA_PRINT			printf
/*! コンソールに文字列を出力(デバッグビルドのみ) */
#define MANA_TRACE			if(0)printf
/*! コンソールに文字列を出力後強制終了 */
#define MANA_ERROR(...)		{ printf(__VA_ARGS__); abort(); }
/*! 値の真偽を調べ、偽なら警告表示(デバッグビルドのみ) */
#define MANA_CHECK(EXP)		assert(EXP)
/*! 値の真偽を調べ、偽なら強制終了(デバッグビルドのみ) */
#define MANA_ASSERT(EXP)	assert(EXP)
/*! 値の真偽を調べ、偽なら強制終了 */
#define MANA_VERIFY(E, M)	if(!(E)){ printf("%s: %s(%d)\n", M, __FILE__, __LINE__); abort(); }
#else
/*! コンソールに文字列を出力 */
#define MANA_PRINT			mana_print_debug
/*! コンソールに文字列を出力(デバッグビルドのみ) */
#define MANA_TRACE			mana_print_debug
/*! コンソールに文字列を出力後強制終了 */
#define MANA_ERROR(...)		{ mana_print_debug(__VA_ARGS__); abort(); }
/*! 値の真偽を調べ、偽なら警告表示(デバッグビルドのみ) */
#define MANA_CHECK(EXP)		if(!(EXP)){ mana_print_debug("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); }
/*! 値の真偽を調べ、偽なら強制終了(デバッグビルドのみ) */
#define MANA_ASSERT(EXP)	if(!(EXP)){ mana_print_debug("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); abort(); }
/*! 値の真偽を調べ、偽なら強制終了 */
#define MANA_VERIFY(E, M)	if(!(E)){ mana_print_debug("%s: %s(%d)\n", M, __FILE__, __LINE__); abort(); }
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
