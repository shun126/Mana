/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
//#include "SystemHolder.h"
#if 0
#include "../executor/Datalink.h"
#include "node.h"
#include "pool.h"


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


/* external variables */
extern int32_t mana_debug;
extern int32_t mana_release;
extern FILE* mana_variable_header_file;

extern FILE* yyin;
extern FILE* yyout;
#endif

namespace mana
{
	extern const char* GetInputFilename();
	extern const char* GetOutputFilename();
	extern const char* GetTargetFilename();

	//extern SystemHolder& GetSystemHolder();
}
