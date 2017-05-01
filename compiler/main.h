/*!
mana (compiler)

@file	main.h
@brief	メインループに関するヘッダファイル
@detail	このファイルはメインループに関係するヘッダファイルです。
@author	Shun Moriya
@date	2003-
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
#if !defined(___NODE_H___)
#include "node.h"
#endif
#if !defined(___MANA_POOL_H___)
#include "pool.h"
#endif

#if !defined(true)
#define true 1
#endif

#if !defined(false)
#define false 0
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
extern void _makepath(char* path, const char* drive, const char* dir, const char* file, const char* ext);
extern void _splitpath(const char* sptr, char* drive, char* dir, char* file, char* ext);
extern char* _fullpath(char* out, const char* in, const size_t size);
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

extern int32_t yyparse(void);
extern void parser_initialize(void);
extern void parser_finalize(void);



extern void mana_compile_init(void);
extern void mana_compile_exit(void);
extern int32_t mana_generator(void);

extern void mana_output_global_memory_map(char*);

extern const char* mana_get_output_filename();

/* external variables */
extern int32_t mana_debug;
extern int32_t mana_release;
extern FILE* mana_variable_header_file;

extern FILE* yyin;
extern FILE* yyout;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
