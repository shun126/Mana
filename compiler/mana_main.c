/*
 * mana (compiler)
 *
 * @file	mana_main.c
 * @brief	メインループに関するソースファイル
 * @detail	このファイルはメインループに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
#endif
#if !defined(___MANA_DATALINK_GENERATOR_H___)
#include "mana_datalink_generator.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "mana_jump.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
#endif
#if !defined(___MANA_VERSION_H___)
#include "mana_version.h"
#endif
#include <libmana.h>
#if defined(_MSC_VER)
#include <windows.h>
#include <crtdbg.h>
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE	(2048)

static char mana_input_filename[_MAX_PATH];
static char mana_output_filename[_MAX_PATH];
int32_t mana_debug;
int32_t mana_release;
FILE* mana_variable_header_file;

#if !defined(_MSC_VER)
/* range copy */
static void rcopy(char* dp, char* sp, char* ep)
{
	while(sp < ep)
		*dp ++ = *sp ++;
	*dp ++ = '\0';
}

/* reverse find */
static char* rfind(char* sp, char* ep, int32_t c)
{
	while(*ep != (int8_t)c)
	{
		if(sp == ep)
			return NULL;
		-- ep;
	}
	return ep;
}

void _makepath(char* path, char* drive, char* dir, char* file, char* ext)
{
	strcpy(path, drive);
	strcat(path, dir);
	strcat(path, file);
	strcat(path, ext);
}

void _splitpath(char* sptr, char* drive, char* dir, char* file, char* ext)
{
	char* tptr;
	char* aptr;
	char* bptr;

	*drive = *dir = *file = *ext = '\0';
	tptr = sptr;
	bptr = sptr + strlen(sptr);

	aptr = strchr(sptr, ':');
	if(aptr)
	{
		rcopy(drive, sptr, aptr+1);
		sptr = aptr + 1;
	}

	tptr = rfind(tptr, bptr, '/');
	if(tptr)
	{
		aptr = rfind(tptr, bptr, '.');
		if(aptr)
		{
			rcopy(ext, aptr, bptr);
			bptr = aptr;
		}
		if(tptr < bptr)
			rcopy(file, tptr + 1, bptr);
		bptr = tptr;
	}
	else
	{
		tptr = sptr;
		aptr = rfind(tptr, bptr, '.');
		if(aptr)
		{
			rcopy(ext, aptr, bptr);
			bptr = aptr;
		}
		rcopy(file, tptr, bptr);
		bptr = tptr;
	}

	if(sptr < bptr)
	{
		rcopy(dir, sptr, bptr + 1);
	}
}

char* _fullpath(char* out, char* in, int32_t size)
{
	realpath(in, out);
	return out;
}

#endif

void mana_error(char* filename, int32_t line, char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif
#if defined(_MSC_VER)
	mana_print("%s(%d): error: %s\n", filename, line, string);
#else
	mana_print("%s:%d: error: %s\n", filename, line, string);
#endif
}

void mana_compile_error(char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	yyerror(string);
}

void mana_compile_warning(char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

#if defined(_MSC_VER)
	mana_print("%s(%d): warning: %s\n", mana_lexer_get_current_filename(), mana_lexer_get_current_line(), string);
#else
	mana_print("%s(%d): warning: %s\n", mana_lexer_get_current_filename(), mana_lexer_get_current_line(), string);
#endif
}

void mana_linker_error(char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	mana_print("%s: error: %s\n", mana_output_filename, string);
}

void mana_linker_warning(char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	mana_print("%s: warning: %s\n", mana_output_filename, string);
}

void mana_fatal(char* format, ...)
{
	char string[MANA_COMPILER_MAX_MESSAGE_BUFFER_SIZE];

	va_list argptr;
	va_start(argptr, format);
#if defined(__STDC_WANT_SECURE_LIB__)
	vsprintf_s(string, sizeof(string), format, argptr);
#else
	vsprintf(string, format, argptr);
#endif

	mana_print("%s: fatal: %s\n", mana_output_filename, string);
	yynerrs++;
}

void mana_fatal_no_memory(void)
{
	mana_fatal("no memory error");
}

static bool mana_test_execute(void* program)
{
	bool result = false;
	mana_initialize();

	mana_plugin_regist(".");

	{
		mana* mana_instance = mana_create();
		if(mana_instance)
		{
			if(mana_load_program(mana_instance, program, true))
			{
				unsigned long long update_time = mana_get_micro_secound();
				double update_secound;
				do{
					unsigned long long current_update_time = mana_get_micro_secound();
					update_secound = (double)(current_update_time - update_time) / 1000000.f;
					update_time = current_update_time;
				} while (mana_run(mana_instance, (float)update_secound));
			}
			else
			{
				result = 1;
			}
			mana_destroy(mana_instance);
		}
		else
		{
			result = 1;
		}
	}

	mana_finalize();

	return result;
}

int32_t mana_compile(void)
{
	int32_t result = 0;

	mana_datalink_generator_initialize();
	mana_code_initialize();
	mana_data_initialzie();
	mana_jump_initialize();
	mana_node_initialize();
	mana_symbol_initialize();
	mana_type_initialize();

	if(mana_variable_header_file)
	{
		fprintf(mana_variable_header_file, "#if !defined(___MANA_GLOBAL_H___)\n");
		fprintf(mana_variable_header_file, "#define ___MANA_GLOBAL_H___\n");
		fprintf(mana_variable_header_file, "typedef struct mana_global\n{\n");
	}
	result = mana_lexer_initialize(mana_input_filename);
	if(result)
	{
		result = (yyparse() != 0 || yynerrs != 0);
	}
	if(mana_variable_header_file)
	{
		fprintf(mana_variable_header_file, "}mana_global;\n");
		fprintf(mana_variable_header_file, "#endif\n");
		fclose(mana_variable_header_file);
		mana_variable_header_file = NULL;
	}

	mana_stream* stream = mana_stream_create();

	if(result == 0)
	{
		mana_file_header header;

		if(mana_debug)
		{
			FILE* log;
			char filename[_MAX_PATH];
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];

#if defined(__STDC_WANT_SECURE_LIB__)
			_splitpath_s(mana_input_filename, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
			_makepath_s(filename, sizeof(filename), drive, dir, fname, ".log");
#else
			_splitpath(mana_input_filename, drive, dir, fname, ext);
			_makepath(filename, drive, dir, fname, ".log");
#endif

#if defined(__STDC_WANT_SECURE_LIB__)
			if(fopen_s(&log, filename, "wt") == 0)
#else
			if((log = fopen(filename, "wt")) != NULL)
#endif
			{
				{
					fprintf(log, "Symbol Table\n\n");
					mana_symbol_dump(log);
					fprintf(log, "\n\n");
				}
				{
					int32_t size = mana_code_get_size();
					void* buffer = mana_malloc(size);
					if(buffer)
					{
						char* data = mana_data_get_buffer();
						int32_t i = 0;

						mana_code_copy(buffer);
						while(i < size)
						{
							const char* text = mana_get_instruction_text(data, buffer, i);
							fprintf(log, "%s\n", text);
							i += mana_get_instruction_size(&((uint8_t*)buffer)[i]);
#if defined(_DEBUG)
							fflush(log);
#endif
						}
						mana_free(buffer);
					}
				}
				fclose(log);
			}
		}

		mana_symbol_check_undefine();


		memset(&header, 0, sizeof(mana_file_header));
		memcpy(&header.header, MANA_SIGNATURE, sizeof(header.header));
		header.major_version = MANA_MAJOR_VERSION;
		header.minor_version = MANA_MINOR_VERSION;
		if(mana_datalink_generator_get_number_of_files() > 0)
			header.flag |= MANA_HEADER_FLAG_RESOURCE;
		if(mana_is_big_endian())
			header.flag |= MANA_HEADER_FLAG_BIG_ENDIAN;
		header.number_of_actors = mana_symbol_get_number_of_actors();
		header.size_of_constant_pool = mana_data_get_size();
		header.size_of_instruction_pool = mana_code_get_size();
		header.size_of_static_memory = mana_symbol_get_static_memory_address();
		header.size_of_global_memory = mana_symbol_get_global_memory_address();
		header.random_seed_number = (uint32_t)time(NULL);

		mana_stream_push_data(stream, &header, sizeof(header));

		if(! mana_symbol_write_actor_infomation(stream))
		{
			result = 1;
			goto ESCAPE;
		}
		
		mana_data_write(stream);

		mana_code_write(stream);

		if(mana_datalink_generator_get_number_of_files() > 0)
		{
			size_t position = mana_stream_get_used_size(stream) % MANA_DATALINK_ALIGNMENT_SIZE;
			if(position > 0)
			{
				srand((unsigned)time(NULL));
				position = MANA_DATALINK_ALIGNMENT_SIZE - position;
				for(size_t i = 0; i < position; i ++)
				{
					mana_stream_push_unsigned_char(stream, (uint8_t)rand());
				}
			}
			if(! mana_datalink_generator_write_data(stream))
			{
				result = 1;
				goto ESCAPE;
			}
		}

		if(result == 0)
		{
			if(mana_output_filename[0] != '\0')
			{
				if(!mana_stream_save(stream, mana_output_filename))
				{
					mana_fatal("%s open failed.", mana_output_filename);
					result = 1;
				}
			}
			else
			{
				mana_test_execute(mana_stream_get_buffer(stream));
			}
		}
	}
ESCAPE:
	if(mana_output_filename[0] != '\0' && result != 0)
	{
		remove(mana_output_filename);
	}

	mana_stream_destroy(stream);

	mana_datalink_generator_finalize();
	mana_code_finalize();
	mana_data_finalize();
	mana_jump_finalize();
	mana_node_finalize();
	mana_symbol_finalize();
	mana_type_finalize();
	mana_lexer_finalize();

	return result;
}

/**
 * mana_print title
 */
static void print_title()
{
	mana_print("mana %s.%s (%04d-%02d-%02d)\n",
		MANA_BUILD_MAJOR_VERSION, MANA_BUILD_MINOR_VERSION,
		MANA_BUILD_YEAR, MANA_BUILD_MONTH, MANA_BUILD_DAY);
}

/**
 * mana_print copyright holder's name
 */
static void print_copyright()
{
	mana_print("mana - Copyright (C) 2002-%04d Shun Moriya\n", MANA_BUILD_YEAR);
}

/**
 * mana_print usage
 */
static void print_usage()
{
	mana_print("usage:mana [switch] infile\n");
	mana_print("            -o filename     specify output file name\n");
	mana_print("            -i dirname      specify program header directory name\n");
	mana_print("            --help          print this message\n");
	mana_print("            --copyright     print copyright holder\n");
	mana_print("            --version       print the version\n");
	mana_print("\n\nReport bugs to <shun@mnu.sakura.ne.jp>\n");
}

/**
 * parse arguments
 * @param	argc	number of argument
 * @param	argv	argument
 * @return	error code.
 */
static int32_t parse_arguments(int32_t argc, char* argv[])
{
	char* cmdptr;
	int32_t cmdcnt;

	if(argc < 2)
	{
		mana_print("No input files\n");
		return false;
	}else{
		mana_input_filename[0] = '\0';
		mana_output_filename[0] = '\0';
		mana_variable_header_file = NULL;
		mana_debug = false;
		mana_release = false;

		for(cmdcnt = 1; cmdcnt < argc; cmdcnt ++)
		{
			cmdptr = argv[cmdcnt];
			if(*cmdptr == '-')
			{
				cmdptr ++;
				switch(*cmdptr)
				{
				case 'o':
					cmdcnt ++;
					if(cmdcnt >= argc)
					{
						mana_print("no output file name\n");
						return false;
					}
					else
					{
#if defined(__STDC_WANT_SECURE_LIB__)
						strcpy_s(mana_output_filename, sizeof(mana_output_filename), argv[cmdcnt]);
#else
						strcpy(mana_output_filename, argv[cmdcnt]);
#endif
					}
					break;

				case 'i':
					{
						int32_t length;
						char filename[_MAX_PATH];

						if(cmdcnt+1 < argc && *argv[cmdcnt+1] != '-')
						{
							cmdcnt++;
#if defined(__STDC_WANT_SECURE_LIB__)
							strcpy_s(filename, sizeof(filename), argv[cmdcnt]);
#else
							strcpy(filename, argv[cmdcnt]);
#endif
						}else{
							filename[0] = '\0';
						}
						length = strlen(filename);
#if defined(_MSC_VER)
						if(length > 0 && filename[length - 1] != '\\')
						{
#if defined(__STDC_WANT_SECURE_LIB__)
							strcat_s(filename, sizeof(filename), "\\");
#else
							strcat(filename, "\\");
#endif
						}
#else
						if(length > 0 && filename[length - 1] != '/')
						{
							strcat(filename, "/");
						}
#endif
#if defined(__STDC_WANT_SECURE_LIB__)
						strcat_s(filename, sizeof(filename), "mana_global.h");
#else
						strcat(filename, "mana_global.h");
#endif

#if defined(__STDC_WANT_SECURE_LIB__)
						if(fopen_s(&mana_variable_header_file, filename, "wt") != 0)
#else
						if((mana_variable_header_file = fopen(filename, "wt")) == NULL)
#endif
						{
							mana_print("'%s' open failed.\n", filename);
							return false;
						}
					}
					break;

				case '-':
					cmdptr ++;
					if(strcmp(cmdptr, "copyright") == 0)
					{
						print_copyright();
						return false;
					}
					else if(strcmp(cmdptr, "version") == 0)
					{
						print_title();
						return false;
					}
					else if(strcmp(cmdptr, "help") == 0)
					{
						print_usage();
						return false;
					}
					else if(strcmp(cmdptr, "debug") == 0)
					{
						mana_debug = true;
						break;
					}
					else if(strcmp(cmdptr, "release") == 0)
					{
						mana_release = true;
						break;
					}

				default:
					mana_print("unrecognized option\n");
					return false;
				}
			}
			else if(mana_input_filename[0] == '\0')
			{
#if defined(__STDC_WANT_SECURE_LIB__)
				strcpy_s(mana_input_filename, sizeof(mana_input_filename), cmdptr);
#else
				strcpy(mana_input_filename, cmdptr);
#endif
			}
			else
			{
				mana_print("unrecognized option\n");
				return false;
			}
		}
#if 0
		if(mana_output_filename[0] == '\0')
		{
			int8_t drive[_MAX_DRIVE];
			int8_t dir[_MAX_DIR];
			int8_t fname[_MAX_FNAME];
			int8_t ext[_MAX_EXT];

#if defined(__STDC_WANT_SECURE_LIB__)
			_splitpath_s(mana_input_filename, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
			_makepath_s(mana_output_filename, sizeof(mana_output_filename), drive, dir, fname, ".mx");
#else
			_splitpath(mana_input_filename, drive, dir, fname, ext);
			_makepath(mana_output_filename, drive, dir, fname, ".mx");
#endif
		}
#endif
		return true;
	}
}

/**
 * main
 * @param	argc	number of argument
 * @param	argv	argument
 * @return	if return value is true, compile was complete.
 */
int32_t main(int32_t argc, char* argv[])
{
	int32_t result = 1;

#if defined(_DEBUG) && defined(_MSC_VER)
	_CrtMemState stOldMemState;
	_CrtMemState stNewMemState;
	_CrtMemState stDiffMemState;
	_CrtMemCheckpoint(&stOldMemState);
#endif

	if(parse_arguments(argc, argv))
	{
		result = mana_compile();
	}

#if defined(_DEBUG) && defined(_MSC_VER)
	_CrtMemCheckpoint(&stNewMemState);
	if(_CrtMemDifference(&stDiffMemState, &stOldMemState, &stNewMemState))
	{
		_RPT0(_CRT_WARN,"Memory leaked!\n\n");
		_CrtMemDumpStatistics(&stDiffMemState);
		_CrtDumpMemoryLeaks();
	}
#endif

	return result;
}
