/*
 * mana (compiler)
 *
 * @file	mana_main.h
 * @brief	メインループに関するヘッダファイル
 * @detail	このファイルはメインループに関係するヘッダファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MAIN_H___)
#define ___MANA_MAIN_H___

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(___MANA_DATALINK_H___)
#include "../library/mana_datalink.h"
#endif
#if !defined(___MANA_HASH_H___)
#include "../library/mana_hash.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "../library/mana_info.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "../library/mana_malloc.h"
#endif
#if !defined(___MANA_POOL_H___)
#include "mana_pool.h"
#endif

#if !defined(true)
#define true 1
#endif

#if !defined(false)
#define false 0
#endif

#if !defined(MANA_NIL)
#define MANA_NIL 0
#endif

#if !defined(_MAX_PATH)
#define _MAX_PATH 260
#endif

#if !defined(_MAX_DRIVE)
#define _MAX_DRIVE 3
#endif

#if !defined(_MAX_DIR)
#define _MAX_DIR 256
#endif

#if !defined(_MAX_FNAME)
#define _MAX_FNAME 256
#endif

#if !defined(_MAX_EXT)
#define _MAX_EXT 256
#endif

#if !defined(_MSC_VER)
#define _chdir	chdir
#define _getcwd	getcwd
extern void _makepath(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
extern void _splitpath(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
extern char* _fullpath(int8_t *, int8_t *, int32_t);
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_DEBUG)
extern void mana_print_debug(const char* format, ...);
#define mana_print	mana_print_debug
#define mana_trace	mana_print_debug
#else
#define mana_print	printf
#define mana_trace 1 ? (void)0 : printf
#endif

#define MANA_ASSERT_MESSAGE(e, m) if(!(e)){ mana_print("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#define MANA_BUG(m) { mana_print("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#define MANA_SAFE_FREE(p) if(p){ free(p); p = 0; }

/* external functions */
extern void mana_error(char*, int32_t, char* format, ...);
extern void mana_compile_error(char*, ...);
extern void mana_compile_warning(char*, ...);
extern void mana_linker_error(char*, ...);
extern void mana_linker_warning(char*, ...);
extern void mana_fatal(char*, ...);
extern void mana_fatal_no_memory(void);

extern int32_t yyparse(void);
extern void yyerror(char*);
extern void mana_parser_initialize(void);
extern void mana_parser_finalize(void);


extern int32_t yylex(void);
extern int32_t mana_lexer_initialize(char*);
extern void mana_lexer_finalize(void);
extern int32_t mana_lexer_open(char*, int32_t);
extern int32_t mana_lexer_close(void);
extern char* mana_lexer_get_current_filename(void);
extern int32_t mana_lexer_get_current_line(void);

extern void mana_compile_init(void);
extern void mana_compile_exit(void);
extern int32_t mana_compile(void);

extern void mana_output_global_memory_map(char*);

/* external variables */
extern int32_t mana_debug;
extern int32_t mana_release;
extern FILE* mana_variable_header_file;

extern int32_t yynerrs;
extern FILE* yyin;
extern FILE* yyout;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
