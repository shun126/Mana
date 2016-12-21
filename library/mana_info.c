/*!
 * mana (compiler/library)
 *
 * @file	mana_info.c
 * @brief	manaコンパイラとmanaライブラリで共通利用するソースファイル
 * @detail	このファイルはmanaコンパイラとmanaライブラリで共通利用するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#if !defined(___MANA_INFO_H___)
#include "mana_info.h"
#endif
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include "platform/cocoa/mana_cocoa.h"
#endif

static uint32_t mana_random_seed = (uint32_t)(19720126);

/*!
 * @param[in]	seed	乱数の種
 */
extern void mana_srand(const uint32_t seed)
{
	mana_random_seed = seed;
}

/*!
 * @return	乱数（0から32767）
 */
int32_t mana_rand(void)
{
	mana_random_seed = mana_random_seed * 1103515245L + 1234;
	return (int32_t)(mana_random_seed / 65536L) % 32768U;
}

/*!
 * @retval	!= 0	ビックエンディアン
 * @retval	== 0	リトルエンディアン
 */
int32_t mana_is_big_endian(void)
{
	int32_t one = 1;
	const char* pointer = (const char*)&one;
	return pointer[0] == 0;
}

/*!
 * @param[in]	value	値
 * @return		スワップされた値
 */
int16_t mana_swap_endian_short(const int16_t value)
{
	return (value << 8 & 0xFF00) | (value >> 8 & 0x00FF);
}

/*!
 * @param[in]	value	値
 * @return		スワップされた値
 */
uint16_t mana_swap_endian_unsigned_short(const uint16_t value)
{
	return (value << 8 & 0xFF00) | (value >> 8 & 0x00FF);
}

/*!
 * @param[in]	value	値
 * @return		スワップされた値
 */
int32_t mana_swap_endian_integer(const int32_t value)
{
	return (value << 24 & 0xFF000000) | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | (value >> 24 & 0x000000FF);
}

/*!
 * @param[in]	value	値
 * @return		スワップされた値
 */
uint32_t mana_swap_endian_unsigned_integer(const uint32_t value)
{
	return (value << 24 & 0xFF000000) | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | (value >> 24 & 0x000000FF);
}

/*!
 * @param[in]	value	値
 * @return		スワップされた値
 */
float mana_swap_endian_float(const float value)
{
	return (float)mana_swap_endian_unsigned_integer((uint32_t)value);
}

/*!
 * @param[in]	format	printfと同様の書式
 */
void mana_print_debug(const char* format, ...)
{
	va_list argptr;
	char* message = NULL;
	int32_t size = 0;
	int32_t length;

	do{
		size += 256;

		message = (char*)mana_realloc(message, size);

		va_start(argptr, format);

#if __STDC_WANT_SECURE_LIB__
		length = vsprintf_s(message, size, format, argptr);
#else
		length = vsprintf(message, format, argptr);
#endif
		va_end(argptr);

	}while(length <= 0 || length > size);

#if defined(_WIN32) || defined(_WIN64)
	OutputDebugStringA(message);
#else
	fprintf(stderr, "%s", message);
#endif

	mana_free(message);
}

/*!
 * @param[in]	text		文字列
 * @param[in]	pattern		検索する文字列
 * @retval		>= 0		見つかった位置（先頭からのオフセット値）
 * @retval		<  0		見つからなかった
 */
int32_t mana_string_find(const int8_t text[], const int8_t pattern[])
{
	int32_t i, j, k, c;

	c = pattern[0];
	i = 0;
	while(text[i] != 0)
	{
		if(text[i++] == c)
		{
			k = i;
			j = 1;
			while(text[k] == pattern[j] && pattern[j] != 0)
			{
				k++;
				j++;
			}
			if(pattern[j] == '\0')
			{
				return k - j;
			}
		}
	}

	return -1;
}

/*!
 * @return	マイクロ秒
 */
unsigned long long mana_get_micro_secound()
{
#if defined(WIN32)
	LARGE_INTEGER frequency, counter;

	if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&counter))
		return 0;

	return (unsigned long long)(counter.QuadPart * 1000000 / frequency.QuadPart);
#else
	struct timeval current;
	return (gettimeofday(&current, NULL) == 0) ? current.tv_usec : 0;
#endif
}

/*!
 * @param[in]	filename	ファイル名
 * @return		ファイルサイズ
 */
long mana_get_file_size(const char* filename)
{
	long size = 0L;
	FILE* fp;

#if defined(__STDC_WANT_SECURE_LIB__)
	if(fopen_s(&fp, filename, "rb") == 0)
#else
	if((fp = fopen(filename, "rb")) != NULL)
#endif
	{
		if(fseek(fp, 0L, SEEK_END) == 0)
			size = ftell(fp);
		fclose(fp);
	}

	return size;
}

/*!
 * @param[out]	読み込まれたバッファ
 * @param[out]	読み込まれたサイズ
 * @param[in]	ファイル名
 * @retval	!= 0	読み込み成功
 * @retval	== 0	読み込み失敗
 */
int32_t mana_read(void** buffer, size_t* size, const char* filename)
{
	int32_t result = 0;
	FILE* file;

#if __STDC_WANT_SECURE_LIB__
	if(fopen_s(&file, filename, "rb") == 0)
#else
	if((file = fopen(filename, "rb")) != NULL)
#endif
	{
		if(fseek(file, 0L, SEEK_END) == 0)
		{
			long buffer_size = ftell(file);
			if(buffer_size > 0)
			{
				rewind(file);

				*buffer = mana_malloc(buffer_size);

				if(fread(*buffer, 1, buffer_size, file) == (size_t)buffer_size)
				{
					*size = (size_t)buffer_size;
					result = 1;
				}
				else
				{
					mana_free(buffer);
					*buffer = NULL;
				}
			}
		}
		fclose(file);
	}

	return result;
}

/*!
 * @param[in]	program	命令セクション内のアドレス
 * @return		オペコードとオペランドの和
 */
int32_t mana_get_instruction_size(const uint8_t* program)
{
	/* 最終的には配列にしよう */

#define SIZE_OF_POINTER	sizeof(uint32_t)

	switch(*program)
	{
	/* thread */
	case MANA_IL_RESTART:				return 1;
	case MANA_IL_HALT:					return 1;
	case MANA_IL_YIELD:					return 1;
	case MANA_IL_NONPREEMPTIVE:			return 1;
	case MANA_IL_PREEMPTIVE:			return 1;

	/* constant */
	case MANA_IL_PUSH_ZERO_INTEGER:		return 1;
	case MANA_IL_PUSH_ZERO_FLOAT:		return 1;
	case MANA_IL_PUSH_CHAR:				return 1 + sizeof(int8_t);
	case MANA_IL_PUSH_SHORT:			return 1 + sizeof(int16_t);
	case MANA_IL_PUSH_INTEGER:			return 1 + sizeof(int32_t);
	case MANA_IL_PUSH_FLOAT:			return 1 + sizeof(float);
	case MANA_IL_PUSH_STRING:			return 1 + SIZE_OF_POINTER;
	case MANA_IL_PUSH_PRIORITY:			return 1;
	case MANA_IL_PUSH_ACTOR:			return 1 + sizeof(int32_t);
	case MANA_IL_PUSH_SELF:				return 1;
	case MANA_IL_PUSH_SENDER:			return 1;

	/* stack */
	case MANA_IL_ALLOCATE:				return 1 + sizeof(int32_t);
	case MANA_IL_FREE:					return 1 + sizeof(int32_t);
	case MANA_IL_DUPLICATE:				return 1;
	case MANA_IL_DUPLICATE_DATA:		return 1 + sizeof(int32_t);
	case MANA_IL_REMOVE:				return 1;
	case MANA_IL_REMOVE_DATA:			return 1 + sizeof(int32_t);
	case MANA_IL_LOAD_STATIC_ADDRESS:	return 1 + sizeof(int32_t);
	case MANA_IL_LOAD_GLOBAL_ADDRESS:	return 1 + sizeof(int32_t);
	case MANA_IL_LOAD_FRAME_ADDRESS:	return 1 + sizeof(int32_t);
	case MANA_IL_LOAD_SELF_ADDRESS:		return 1 + sizeof(int32_t);

	/* memory operation */
	case MANA_IL_LOAD_CHAR:				return 1;
	case MANA_IL_LOAD_SHORT:			return 1;
	case MANA_IL_LOAD_INTEGER:			return 1;
	case MANA_IL_LOAD_FLOAT:			return 1;
	case MANA_IL_LOAD_REFFRENCE:		return 1;
	case MANA_IL_LOAD_DATA:				return 1 + SIZE_OF_POINTER;
	case MANA_IL_STORE_CHAR:			return 1;
	case MANA_IL_STORE_SHORT:			return 1;
	case MANA_IL_STORE_INTEGER:			return 1;
	case MANA_IL_STORE_FLOAT:			return 1;
	case MANA_IL_STORE_REFFRENCE:		return 1;
	case MANA_IL_STORE_DATA:			return 1 + SIZE_OF_POINTER;

	/* jump */
	case MANA_IL_BEQ:					return 1 + sizeof(int32_t);
	case MANA_IL_BNE:					return 1 + sizeof(int32_t);
	case MANA_IL_BRA:					return 1 + sizeof(int32_t);
	case MANA_IL_BSR:					return 1 + sizeof(int32_t);
	case MANA_IL_REQ:					return 1 + sizeof(int32_t);
	case MANA_IL_REQWS:					return 1 + sizeof(int32_t);
	case MANA_IL_REQWE:					return 1 + sizeof(int32_t);
	case MANA_IL_DYNAMIC_REQ:			return 1 + sizeof(int32_t);
	case MANA_IL_DYNAMIC_REQWS:			return 1 + sizeof(int32_t);
	case MANA_IL_DYNAMIC_REQWE:			return 1 + sizeof(int32_t);
	case MANA_IL_LOAD_RETURN_ADDRESS:	return 1;
	case MANA_IL_SAVE_RETURN_ADDRESS:	return 1;
	case MANA_IL_RETURN_FROM_FUNCTION:	return 1;
	case MANA_IL_RETURN_FROM_ACTION:	return 1;
	case MANA_IL_ROLLBACK:				return 1;
	case MANA_IL_CALL:					return 1 + sizeof(int32_t) + sizeof(int16_t) + sizeof(int16_t) + (program[6] * sizeof(int16_t));

	/* caluclation */
	case MANA_IL_ADD_INTEGER:			return 1;
	case MANA_IL_DIV_INTEGER:			return 1;
	case MANA_IL_MINUS_INTEGER:			return 1;
	case MANA_IL_MOD_INTEGER:			return 1;
	case MANA_IL_MUL_INTEGER:			return 1;
	case MANA_IL_SUB_INTEGER:			return 1;

	case MANA_IL_ADD_FLOAT:				return 1;
	case MANA_IL_DIV_FLOAT:				return 1;
	case MANA_IL_MINUS_FLOAT:			return 1;
	case MANA_IL_MOD_FLOAT:				return 1;
	case MANA_IL_MUL_FLOAT:				return 1;
	case MANA_IL_SUB_FLOAT:				return 1;

	case MANA_IL_AND:					return 1;
	case MANA_IL_EOR:					return 1;
	case MANA_IL_LAND:					return 1;
	case MANA_IL_LOR:					return 1;
	case MANA_IL_LNOT:					return 1;
	case MANA_IL_NOT:					return 1;
	case MANA_IL_OR:					return 1;
	case MANA_IL_SHL:					return 1;
	case MANA_IL_SHR:					return 1;

	case MANA_IL_INT2FLOAT:				return 1;
	case MANA_IL_FLOAT2INT:				return 1;

	/* compare */
	case MANA_IL_COMPARE_EQ_INTEGER:	return 1;
	case MANA_IL_COMPARE_NE_INTEGER:	return 1;
	case MANA_IL_COMPARE_GE_INTEGER:	return 1;
	case MANA_IL_COMPARE_GT_INTEGER:	return 1;
	case MANA_IL_COMPARE_LE_INTEGER:	return 1;
	case MANA_IL_COMPARE_LS_INTEGER:	return 1;

	case MANA_IL_COMPARE_EQ_FLOAT:		return 1;
	case MANA_IL_COMPARE_NE_FLOAT:		return 1;
	case MANA_IL_COMPARE_GE_FLOAT:		return 1;
	case MANA_IL_COMPARE_GT_FLOAT:		return 1;
	case MANA_IL_COMPARE_LE_FLOAT:		return 1;
	case MANA_IL_COMPARE_LS_FLOAT:		return 1;

	case MANA_IL_COMPARE_EQ_DATA:		return 1 + SIZE_OF_POINTER;
	case MANA_IL_COMPARE_NE_DATA:		return 1 + SIZE_OF_POINTER;
	case MANA_IL_COMPARE_GE_DATA:		return 1 + SIZE_OF_POINTER;
	case MANA_IL_COMPARE_GT_DATA:		return 1 + SIZE_OF_POINTER;
	case MANA_IL_COMPARE_LE_DATA:		return 1 + SIZE_OF_POINTER;
	case MANA_IL_COMPARE_LS_DATA:		return 1 + SIZE_OF_POINTER;

	/* inner function */
	case MANA_IL_PRINT:					return 1 + sizeof(int32_t);

	case MANA_IL_JOIN:					return 1;
	case MANA_IL_COMPLY:				return 1;
	case MANA_IL_REFUSE:				return 1;

	default:
		assert(0);
		return 1;
	}
}

static int8_t get_char(const void* program, const int32_t address)
{
	const char* instruction = (const char*)program;
	return instruction[address];
}

static int16_t get_short(const void* program, const int32_t address)
{
	const uint8_t* instruction;
	uint8_t* pointer;
	int16_t value;

	instruction = (const uint8_t*)program;
	pointer = (uint8_t*)&value;

	if(mana_is_big_endian())
	{
		pointer[0] = instruction[address + 0];
		pointer[1] = instruction[address + 1];
	}
	else
	{
		pointer[0] = instruction[address + 1];
		pointer[1] = instruction[address + 0];
	}
	return value;
}

static int32_t get_integer(const void* program, const int32_t address)
{
	const uint8_t* instruction;
	uint8_t* pointer;
	int32_t value;

	instruction = (const uint8_t*)program;
	pointer = (uint8_t*)&value;

	if(mana_is_big_endian())
	{
		pointer[0] = instruction[address + 0];
		pointer[1] = instruction[address + 1];
		pointer[2] = instruction[address + 2];
		pointer[3] = instruction[address + 3];
	}
	else
	{
		pointer[0] = instruction[address + 3];
		pointer[1] = instruction[address + 2];
		pointer[2] = instruction[address + 1];
		pointer[3] = instruction[address + 0];
	}
	return value;
}

static float get_float(const void* program, const int32_t address)
{
	const uint8_t* instruction;
	uint8_t* pointer;
	float value;

	instruction = (const uint8_t*)program;
	pointer = (uint8_t*)&value;

	if(mana_is_big_endian())
	{
		pointer[0] = instruction[address + 0];
		pointer[1] = instruction[address + 1];
		pointer[2] = instruction[address + 2];
		pointer[3] = instruction[address + 3];
	}
	else
	{
		pointer[0] = instruction[address + 3];
		pointer[1] = instruction[address + 2];
		pointer[2] = instruction[address + 1];
		pointer[3] = instruction[address + 0];
	}
	return value;
}

static const char* get_string(const void* program, const int32_t address, const void* buffer)
{
	const char* data = (const char*)buffer;
	const int32_t index = get_integer(program, address);
	return index >= 0 ? &data[index] : "";
}

/*
static void* get_data(void* program, int32_t address, void* buffer)
{
	const uint8_t* data = (const uint8_t*)buffer;
	int32_t index = get_integer(program, address);
	return index >= 0 ? &data[index] : NULL;
}
*/

/*!
 * @param[in]	data	データセクション先頭アドレス
 * @param[in]	program	命令セクション内のアドレス
 * @param[in]	address	命令セクション内のプログラムカウンタ
 * @return		オペコードとオペランドの文字列。文字列は共通領域を使うので、
 * char* mana_get_instruction_text(char* data, void* program, int32_t address)
 * を呼び出すと上書きされます。
 */
const char* mana_get_instruction_text(const char* data, const void* program, const int32_t address)
{
	uint8_t opecode;
	static char text[256];

	opecode = ((const uint8_t*)program)[address];
	switch(opecode)
	{
	/* thread */
	case MANA_IL_RESTART:				sprintf(text, "%08x:stop", address); break;
	case MANA_IL_HALT:					sprintf(text, "%08x:halt", address); break;
	case MANA_IL_YIELD:					sprintf(text, "%08x:yield", address); break;
	case MANA_IL_NONPREEMPTIVE:			sprintf(text, "%08x:lock", address); break;
	case MANA_IL_PREEMPTIVE:			sprintf(text, "%08x:unlock", address); break;

	/* constant */
	case MANA_IL_PUSH_ZERO_INTEGER:		sprintf(text, "%08x:push zero (integer)", address); break;
	case MANA_IL_PUSH_ZERO_FLOAT:		sprintf(text, "%08x:push zero (float)", address); break;
	case MANA_IL_PUSH_CHAR:				sprintf(text, "%08x:push %d (int8_t)", address, get_char(program, address + 1)); break;
	case MANA_IL_PUSH_SHORT:			sprintf(text, "%08x:push %d (int16_t)", address, get_short(program, address + 1)); break;
	case MANA_IL_PUSH_INTEGER:			sprintf(text, "%08x:push %d (integer)", address, get_integer(program, address + 1)); break;
	case MANA_IL_PUSH_FLOAT:			sprintf(text, "%08x:push %f (float)", address, get_float(program, address + 1)); break;
	case MANA_IL_PUSH_STRING:			sprintf(text, "%08x:push %x (reference)", address, get_integer(program, address + 1)); break;
	case MANA_IL_PUSH_PRIORITY:			sprintf(text, "%08x:push run-level", address); break;
	case MANA_IL_PUSH_ACTOR:			sprintf(text, "%08x:push actor '%s'", address, get_string(program, address + 1, data)); break;
	case MANA_IL_PUSH_SELF:				sprintf(text, "%08x:push self", address); break;
	case MANA_IL_PUSH_SENDER:			sprintf(text, "%08x:push sender", address); break;

	/* stack */
	case MANA_IL_ALLOCATE:				sprintf(text, "%08x:allocate %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_FREE:					sprintf(text, "%08x:release  %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_DUPLICATE:				sprintf(text, "%08x:duplicate", address); break;
	case MANA_IL_DUPLICATE_DATA:		sprintf(text, "%08x:duplicate %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_REMOVE:				sprintf(text, "%08x:remove", address); break;
	case MANA_IL_REMOVE_DATA:			sprintf(text, "%08x:remove %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_LOAD_STATIC_ADDRESS:	sprintf(text, "%08x:load effective address %08x (static)", address, get_integer(program, address + 1)); break;
	case MANA_IL_LOAD_GLOBAL_ADDRESS:	sprintf(text, "%08x:load effective address %08x (global)", address, get_integer(program, address + 1)); break;
	case MANA_IL_LOAD_SELF_ADDRESS:		sprintf(text, "%08x:load effective address %08x (self)", address, get_integer(program, address + 1)); break;
	case MANA_IL_LOAD_FRAME_ADDRESS:	sprintf(text, "%08x:load effective address %08x (frame)", address, get_integer(program, address + 1)); break;

	/* memory */
	case MANA_IL_LOAD_CHAR:				sprintf(text, "%08x:load (int8_t)", address); break;
	case MANA_IL_LOAD_SHORT:			sprintf(text, "%08x:load (int16_t)", address); break;
	case MANA_IL_LOAD_INTEGER:			sprintf(text, "%08x:load (int32_t)", address); break;
	case MANA_IL_LOAD_FLOAT:			sprintf(text, "%08x:load (float)", address); break;
	case MANA_IL_LOAD_REFFRENCE:		sprintf(text, "%08x:load (reference)", address); break;
	case MANA_IL_LOAD_DATA:				sprintf(text, "%08x:load (data) %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_STORE_CHAR:			sprintf(text, "%08x:store (int8_t)", address); break;
	case MANA_IL_STORE_SHORT:			sprintf(text, "%08x:store (int16_t)", address); break;
	case MANA_IL_STORE_INTEGER:			sprintf(text, "%08x:store (int32_t)", address); break;
	case MANA_IL_STORE_FLOAT:			sprintf(text, "%08x:store (float)", address); break;
	case MANA_IL_STORE_REFFRENCE:		sprintf(text, "%08x:store (reference)", address); break;
	case MANA_IL_STORE_DATA:			sprintf(text, "%08x:store (data) %d byte(s)", address, get_integer(program, address + 1)); break;

	/* jump */
	case MANA_IL_BEQ:					sprintf(text, "%08x:beq %08x", address, get_integer(program, address + 1)); break;
	case MANA_IL_BNE:					sprintf(text, "%08x:bne %08x", address, get_integer(program, address + 1)); break;
	case MANA_IL_BRA:					sprintf(text, "%08x:bra %08x", address, get_integer(program, address + 1)); break;
	case MANA_IL_BSR:					sprintf(text, "%08x:bsr %08x", address, get_integer(program, address + 1)); break;
	case MANA_IL_CALL:					sprintf(text, "%08x:call '%s' %d %d %d", address, get_string(program, address + 1, data), get_char(program, address + 5), get_char(program, address + 6), get_short(program, address + 7)); break;
	case MANA_IL_REQ:					sprintf(text, "%08x:req '%s'", address, get_string(program, address + 1, data)); break;
	case MANA_IL_REQWS:					sprintf(text, "%08x:reqws '%s'", address, get_string(program, address + 1, data)); break;
	case MANA_IL_REQWE:					sprintf(text, "%08x:reqwe '%s'", address, get_string(program, address + 1, data)); break;
	case MANA_IL_DYNAMIC_REQ:			sprintf(text, "%08x:dynamic req '%s'", address, get_string(program, address + 1, data)); break;
	case MANA_IL_DYNAMIC_REQWS:			sprintf(text, "%08x:dynamic reqws '%s'", address, get_string(program, address + 1, data)); break;
	case MANA_IL_DYNAMIC_REQWE:			sprintf(text, "%08x:dynamic reqwe '%s'", address, get_string(program, address + 1, data)); break;
	case MANA_IL_LOAD_RETURN_ADDRESS:	sprintf(text, "%08x:load ra", address); break;
	case MANA_IL_SAVE_RETURN_ADDRESS:	sprintf(text, "%08x:save ra", address); break;
	case MANA_IL_RETURN_FROM_FUNCTION:	sprintf(text, "%08x:return from function", address); break;
	case MANA_IL_RETURN_FROM_ACTION:	sprintf(text, "%08x:return from action", address); break;
	case MANA_IL_ROLLBACK:				sprintf(text, "%08x:rollback", address); break;

	/* caluclation */
	case MANA_IL_ADD_INTEGER:			sprintf(text, "%08x:add (integer)", address); break;
	case MANA_IL_DIV_INTEGER:			sprintf(text, "%08x:div (integer)", address); break;
	case MANA_IL_MINUS_INTEGER:			sprintf(text, "%08x:neg (integer)", address); break;
	case MANA_IL_MOD_INTEGER:			sprintf(text, "%08x:mod (integer)", address); break;
	case MANA_IL_MUL_INTEGER:			sprintf(text, "%08x:mul (integer)", address); break;
	case MANA_IL_SUB_INTEGER:			sprintf(text, "%08x:sub (integer)", address); break;

	case MANA_IL_ADD_FLOAT:				sprintf(text, "%08x:add (float)", address); break;
	case MANA_IL_DIV_FLOAT:				sprintf(text, "%08x:div (float)", address); break;
	case MANA_IL_MINUS_FLOAT:			sprintf(text, "%08x:neg (float)", address); break;
	case MANA_IL_MOD_FLOAT:				sprintf(text, "%08x:mod (float)", address); break;
	case MANA_IL_MUL_FLOAT:				sprintf(text, "%08x:mul (float)", address); break;
	case MANA_IL_SUB_FLOAT:				sprintf(text, "%08x:sub (float)", address); break;

	case MANA_IL_AND:					sprintf(text, "%08x:and", address); break;
	case MANA_IL_EOR:					sprintf(text, "%08x:eor", address); break;
	case MANA_IL_LAND:					sprintf(text, "%08x:and", address); break;
	case MANA_IL_LOR:					sprintf(text, "%08x:or", address); break;
	case MANA_IL_LNOT:					sprintf(text, "%08x:!", address); break;
	case MANA_IL_NOT:					sprintf(text, "%08x:~", address); break;
	case MANA_IL_OR:					sprintf(text, "%08x:or", address); break;
	case MANA_IL_SHL:					sprintf(text, "%08x:shift left", address); break;
	case MANA_IL_SHR:					sprintf(text, "%08x:shift right", address); break;

	case MANA_IL_INT2FLOAT:				sprintf(text, "%08x:cast (float)", address); break;
	case MANA_IL_FLOAT2INT:				sprintf(text, "%08x:cast (int32_t)", address); break;

	/* compare */
	case MANA_IL_COMPARE_EQ_INTEGER:	sprintf(text, "%08x:== (integer)", address); break;
	case MANA_IL_COMPARE_NE_INTEGER:	sprintf(text, "%08x:!= (integer)", address); break;
	case MANA_IL_COMPARE_GE_INTEGER:	sprintf(text, "%08x:>= (integer)", address); break;
	case MANA_IL_COMPARE_GT_INTEGER:	sprintf(text, "%08x:> (integer)", address); break;
	case MANA_IL_COMPARE_LE_INTEGER:	sprintf(text, "%08x:< (integer)", address); break;
	case MANA_IL_COMPARE_LS_INTEGER:	sprintf(text, "%08x:< (integer)", address); break;

	case MANA_IL_COMPARE_EQ_FLOAT:		sprintf(text, "%08x:== (float)", address); break;
	case MANA_IL_COMPARE_NE_FLOAT:		sprintf(text, "%08x:!= (float)", address); break;
	case MANA_IL_COMPARE_GE_FLOAT:		sprintf(text, "%08x:>= (float)", address); break;
	case MANA_IL_COMPARE_GT_FLOAT:		sprintf(text, "%08x:> (float)", address); break;
	case MANA_IL_COMPARE_LE_FLOAT:		sprintf(text, "%08x:<= (float)", address); break;
	case MANA_IL_COMPARE_LS_FLOAT:		sprintf(text, "%08x:< (float)", address); break;

	case MANA_IL_COMPARE_EQ_DATA:		sprintf(text, "%08x:== %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_COMPARE_NE_DATA:		sprintf(text, "%08x:!= %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_COMPARE_GE_DATA:		sprintf(text, "%08x:>= %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_COMPARE_GT_DATA:		sprintf(text, "%08x:> %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_COMPARE_LE_DATA:		sprintf(text, "%08x:<= %d byte(s)", address, get_integer(program, address + 1)); break;
	case MANA_IL_COMPARE_LS_DATA:		sprintf(text, "%08x:< %d byte(s)", address, get_integer(program, address + 1)); break;

	/* inner function */
	case MANA_IL_PRINT:					sprintf(text, "%08x:print argc=%d", address, get_integer(program, address + 1)); break;

	case MANA_IL_JOIN:					sprintf(text, "%08x:join", address); break;
	case MANA_IL_COMPLY:				sprintf(text, "%08x:comply", address); break;
	case MANA_IL_REFUSE:				sprintf(text, "%08x:refuse", address); break;

	default:
		assert(0);
		sprintf(text, "invalid opecode");
		break;
	}
	return text;
}
