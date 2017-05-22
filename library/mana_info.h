/*!
mana (library)

@file	mana_info.h
@brief	manaコンパイラとmanaライブラリで共通利用するヘッダーファイル
@detail	このファイルはmanaコンパイラとmanaライブラリで共通利用するヘッダーファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_INFO_H___)
#define ___MANA_INFO_H___

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
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

/*! manaファイルヘッダー */
typedef struct mana_file_header
{
	int8_t header[4];									/*!< 署名 */
	uint8_t major_version;					/*!< メジャーバージョン */
	uint8_t minor_version;					/*!< マイナーバージョン */
	uint8_t pad;								/*!< パディング */
	uint8_t flag;								/*!< フラグ */
/*
mana_file_header :: flag

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  |   |   |   |   |   |   |   |
  |   |   |   |   |   |   |   +---	MANA_HEADER_FLAG_RESOURCE
  |   |   |   |   |   |   +-------	MANA_HEADER_FLAG_64BIT
  |   |   |   |   |   +-----------  MANA_HEADER_FLAG_BIG_ENDIAN
  |   |   |   |   +---------------
  |   |   |   +-------------------
  |   |   +-----------------------
  |   +---------------------------
  +-------------------------------	MANA_HEADER_FLAG_COMPILED
*/
#define MANA_HEADER_FLAG_RESOURCE	0x01			/*!< フラグ：リソースセクションあり */
#define MANA_HEADER_FLAG_64BIT		0x02			/*!< フラグ：64ビットでコンパイルされた */
#define MANA_HEADER_FLAG_BIG_ENDIAN	0x04			/*!< フラグ：ビックエンディアンでコンパイルされた */
#define MANA_HEADER_FLAG_COMPILED	0x80			/*!< フラグ：コンパイル済み */

	uint32_t number_of_actors;					/*!< アクター数 */
	uint32_t size_of_constant_pool;				/*!< 定数セクションサイズ */
	uint32_t size_of_instruction_pool;			/*!< 命令セクションサイズ */
	uint32_t size_of_static_memory;				/*!< スタティックメモリサイズ */
	uint32_t size_of_global_memory;				/*!< グローバルメモリサイズ */
	uint32_t random_seed_number;				/*!< 乱数の種 */
}mana_file_header;

/*! manaファイル内のアクター情報ヘッダー */
typedef struct mana_actor_info_header
{
	uint8_t flag;								/*!< フラグ */
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

	uint8_t number_of_instances;				/*!< インスタンス数（未使用） */
	uint16_t number_of_actions;				/*!< アクション数 */
	uint32_t name;								/*!< アクター名 */
	uint32_t variable_size;						/*!< インスタンス変数サイズ */
}mana_actor_info_header;

/*! manaファイル内のアクション情報ヘッダー */
typedef struct mana_action_info_header
{
	uint32_t name;								/*!< アクション名 */
	intptr_t address;							/*!< アドレス */
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
extern void mana_srand(const uint32_t seed);

/*! 乱数を取得 */
extern int32_t mana_rand(void);

/*! ０から１までの乱数 */
#define mana_frand()	(((float)rand() & 0x7FFF) / 32767.0f)

/*! 角度をラジアンに変換 */
#define mana_to_radian(degree)	(degree * (MANA_PI / 180.f))

/*! ラジアンを角度に変換 */
#define mana_to_degree(radian)	(radian * (180.0f / MANA_PI))

/*! メモリの状態取得 */
extern int32_t mana_is_big_endian(void);

/*! int16_t型のエンディアンを変換します */
extern int16_t mana_swap_endian_short(const int16_t value);

/*! uint16_t型のエンディアンを変換します */
extern uint16_t mana_swap_endian_unsigned_short(const uint16_t value);

/*! int32_t型のエンディアンを変換します */
extern int32_t mana_swap_endian_integer(const int32_t value);

/*! uint32_t型のエンディアンを変換します */
extern uint32_t mana_swap_endian_unsigned_integer(const uint32_t value);

/*! float型のエンディアンを変換します */
extern float mana_swap_endian_float(const float value);

/*! コンソールに文字列を出力します */
extern void mana_print_debug(const char* format, ...);

/*! 文字列の中から文字列を検索 */
extern int32_t mana_string_find(const int8_t text[], const int8_t pattern[]);

/*! パフォーマンス情報をマイクロ秒で取得します */
extern uint64_t mana_get_micro_secound(void);

/*! ファイルサイズを取得 */
extern long mana_get_file_size(const char* filename);

/*! ファイルを読み込みます(エンディアン変換には対応していません) */
extern int32_t mana_read(void** buffer, size_t* size, const char* filename);

/*! 命令のサイズを取得 */
extern int32_t mana_get_instruction_size(const uint8_t* program);

/*! オペコードとオペランドの文字列を取得 */
extern const char* mana_get_instruction_text(const char* data, const void* program, const int32_t address);

#if defined(_DEBUG)
/*! コンソールに文字列を出力 */
#define MANA_PRINT(...)		mana_print_debug(__VA_ARGS__)
/*! コンソールに文字列を出力(デバッグビルドのみ) */
#define MANA_TRACE(...)		mana_print_debug(__VA_ARGS__)
/*! コンソールに文字列を出力 */
#define MANA_WARNING(...)	{ mana_print_debug(__VA_ARGS__); }
/*! コンソールに文字列を出力後強制終了 */
#define MANA_ERROR(...)		{ mana_print_debug(__VA_ARGS__); abort(); }
/*! 値の真偽を調べ、偽なら警告表示(デバッグビルドのみ) */
#define MANA_CHECK(EXP)		if(!(EXP)){ mana_print_debug("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); }
/*! 値の真偽を調べ、偽なら強制終了(デバッグビルドのみ) */
#define MANA_ASSERT(EXP)	if(!(EXP)){ mana_print_debug("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); abort(); }
/*! 値の真偽を調べ、偽なら強制終了(デバッグビルドのみ) */
#define MANA_ASSERT_MESSAGE(EXP, m) if(!(EXP)){ mana_print_debug("%s: %s(%d): %s\n", #EXP, __FILE__, __LINE__, m); abort(); }
//! 強制終了
#define MANA_BUG(m) { mana_print_debug("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#else
/*! コンソールに文字列を出力 */
#define MANA_PRINT(...)		printf(__VA_ARGS__)
/*! コンソールに文字列を出力(デバッグビルドのみ) */
#define MANA_TRACE(...)		((void)0)
/*! コンソールに文字列を出力 */
#define MANA_WARNING(...)	{ printf(__VA_ARGS__); }
/*! コンソールに文字列を出力後強制終了 */
#define MANA_ERROR(...)		{ printf(__VA_ARGS__); abort(); }
/*! 値の真偽を調べ、偽なら警告表示(デバッグビルドのみ) */
#define MANA_CHECK(EXP)		((void)0)
/*! 値の真偽を調べ、偽なら強制終了(デバッグビルドのみ) */
#define MANA_ASSERT(EXP)	((void)0)
/*! 値の真偽を調べ、偽なら強制終了(デバッグビルドのみ) */
#define MANA_ASSERT_MESSAGE(EXP, m) ((void)0)
//! 強制終了
#define MANA_BUG(m) { printf("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#endif
/*! 値の真偽を調べ、偽なら強制終了 */
#define MANA_VERIFY(EXP) \
if(!(EXP)){ \
	MANA_PRINT("%s(%d): (%s)\n", __FILE__, __LINE__, #EXP); \
	abort(); \
}
#define MANA_VERIFY_MESSAGE(EXP, ...) \
if(!(EXP)){ \
	MANA_PRINT("%s(%d): ", __FILE__, __LINE__); \
	MANA_PRINT(__VA_ARGS__); \
	MANA_PRINT(" (%s)\n", #EXP); \
	abort(); \
}
#define MANA_UNUSED_VAR(var)	((void)&var);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
