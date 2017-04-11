/*!
mana (compiler)

@file	code.c
@brief	命令セクションに関するソースファイル
@detail	このファイルは命令セクションに関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_CODE_H___)
#include "code.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif

//! 命令セクションのページサイズ
#define MANA_CODE_ALLOCATE_PAGE_SIZE	(4096)

//! 命令タイプ
typedef struct code_type
{
	int32_t address;		//!< アドレス
	uint8_t code;			//!< 命令コード
} code_type;

//! 命令セクション
typedef struct code_entry
{
	code_type* buffer;		//!< 命令タイプバッファー
	int32_t used_size;		//!< 命令タイプバッファー使用サイズ
	int32_t allocated_size;	//!< 命令タイプバッファー確保サイズ
} code_entry;

static code_entry code_entry_instance;

void code_initialize(void)
{
	code_entry_instance.buffer = NULL;
	code_entry_instance.allocated_size = 0;
	code_entry_instance.used_size = 0;
}

void code_finalize(void)
{
	if(code_entry_instance.buffer)
	{
		mana_free(code_entry_instance.buffer);
		code_entry_instance.buffer = NULL;
	}
	code_entry_instance.allocated_size = 0;
	code_entry_instance.used_size = 0;
}

void code_reduce(const size_t size)
{
	if(size > 0)
	{
		code_entry_instance.used_size -= size;
		if(code_entry_instance.used_size < 0)
		{
			code_entry_instance.used_size = 0;
		}
	}
}

/*!
命令セクションに値を設定します

@param[in]	code	オペコード
@param[in]	address	オペコードに関係付けるアドレス
@return		設定したアドレス
*/
static int32_t code_set(const uint8_t code, const int32_t address)
{
	int32_t pc = code_get_pc();
	if(code_entry_instance.used_size >= code_entry_instance.allocated_size)
	{
		code_entry_instance.allocated_size += MANA_CODE_ALLOCATE_PAGE_SIZE;
		code_entry_instance.buffer = (code_type*)mana_realloc(
			code_entry_instance.buffer, code_entry_instance.allocated_size * sizeof(code_type));
	}
	code_entry_instance.buffer[code_entry_instance.used_size].code = code;
	code_entry_instance.buffer[code_entry_instance.used_size].address = address;
	code_entry_instance.used_size++;
	return pc;
}

void code_set_char(const int8_t value)
{
	code_set(value, -1);
}

void code_set_short(const int16_t value)
{
	const uint8_t* p = (const uint8_t*)&value;
	if(mana_is_big_endian())
	{
		code_set(p[0], -1);
		code_set(p[1], -1);
	}
	else
	{
		code_set(p[1], -1);
		code_set(p[0], -1);
	}
}

void code_set_integer(const int32_t value)
{
	const uint8_t* p = (const uint8_t*)&value;
	if(mana_is_big_endian())
	{
		code_set(p[0], -1);
		code_set(p[1], -1);
		code_set(p[2], -1);
		code_set(p[3], -1);
	}
	else
	{
		code_set(p[3], -1);
		code_set(p[2], -1);
		code_set(p[1], -1);
		code_set(p[0], -1);
	}
}

void code_set_unsigned_char(const uint8_t value)
{
	code_set_char((const int8_t)value);
}

void code_set_unsigned_short(const uint16_t value)
{
	code_set_short((const int16_t)value);
}

void code_set_unsigned_integer(const uint32_t value)
{
	code_set_integer((const int32_t)value);
}

void code_set_float(const float value)
{
	uint8_t* p = (uint8_t*)&value;
	if(mana_is_big_endian())
	{
		code_set(p[0], -1);
		code_set(p[1], -1);
		code_set(p[2], -1);
		code_set(p[3], -1);
	}
	else
	{
		code_set(p[3], -1);
		code_set(p[2], -1);
		code_set(p[1], -1);
		code_set(p[0], -1);
	}
}

void code_set_opecode(const uint8_t code)
{
	code_set(code, -1);
}

int32_t code_set_opecode_and_operand(const uint8_t code, const int32_t address)
{
	const int32_t pc = code_get_pc();
	code_set(code, address);
	code_set_integer(address);
	return pc;
}

int32_t code_set_opecode_and_two_operands(const uint8_t code, const int32_t address, const int32_t size)
{
	const int32_t pc = code_get_pc();
	code_set(code, address);
	code_set_integer(address);
	code_set_integer(size);
	return pc;
}

void code_replace_opecode(const int32_t address, const uint8_t code)
{
	MANA_VERIFY_MESSAGE(address >= 0 && address < code_get_size(), "Out of range");
	code_entry_instance.buffer[address].code = code;
}

void code_replace_address(const int32_t address, const int32_t new_address)
{
	MANA_VERIFY_MESSAGE(address >= 0 && address < code_get_size(), "Out of range");

	code_entry_instance.buffer[address+0].code = (const uint8_t)(new_address >> 24);
	code_entry_instance.buffer[address+1].code = (const uint8_t)(new_address >> 16);
	code_entry_instance.buffer[address+2].code = (const uint8_t)(new_address >> 8);
	code_entry_instance.buffer[address+3].code = (const uint8_t)(new_address);
}

void code_replace_all(const int32_t base_address, const int32_t new_address)
{
	int32_t address = base_address;

	while(address >= 0)
	{
		MANA_VERIFY_MESSAGE(address >= 0 && address < code_get_size(), "Out of range");

		const int32_t next_address = code_entry_instance.buffer[address].address;
		code_entry_instance.buffer[address+0].address = -1;
		code_entry_instance.buffer[address+1].code = (const uint8_t)(new_address >> 24);
		code_entry_instance.buffer[address+2].code = (const uint8_t)(new_address >> 16);
		code_entry_instance.buffer[address+3].code = (const uint8_t)(new_address >> 8);
		code_entry_instance.buffer[address+4].code = (const uint8_t)(new_address);
		address = next_address;
	}
}

bool code_write(mana_stream* stream)
{
	for(int32_t i = 0; i < code_get_size(); i ++)
	{
		mana_stream_push_unsigned_char(stream, code_entry_instance.buffer[i].code);
	}
	return true;
}

int32_t code_copy(void* buffer)
{
	uint8_t* code = (uint8_t*)buffer;
	int32_t i;
	for(i = 0; i < code_entry_instance.used_size; i ++)
	{
		*code ++ = code_entry_instance.buffer[i].code;
	}
	return i;
}

int32_t code_get_pc(void)
{
	return code_get_size();
}

int32_t code_get_size(void)
{
	return code_entry_instance.used_size;
}

void* code_get_buffer(void)
{
	return (void*)(code_entry_instance.buffer);
}

uint8_t code_getcode(const int32_t address)
{
	MANA_VERIFY_MESSAGE(address >= 0 && address < code_get_size(), "Out of range");
	return code_entry_instance.buffer[address].code;
}

int32_t code_get_integer(const int32_t address)
{
	int32_t value;
	uint8_t* p = (uint8_t*)&value;
	if(mana_is_big_endian())
	{
		p[0] = code_entry_instance.buffer[address+0].code;
		p[1] = code_entry_instance.buffer[address+1].code;
		p[2] = code_entry_instance.buffer[address+2].code;
		p[3] = code_entry_instance.buffer[address+3].code;
	}
	else
	{
		p[3] = code_entry_instance.buffer[address+0].code;
		p[2] = code_entry_instance.buffer[address+1].code;
		p[1] = code_entry_instance.buffer[address+2].code;
		p[0] = code_entry_instance.buffer[address+3].code;
	}
	return value;
}

float code_get_float(const int32_t address)
{
	float value;
	uint8_t* p = (uint8_t*)&value;
	if(mana_is_big_endian())
	{
		p[0] = code_entry_instance.buffer[address+0].code;
		p[1] = code_entry_instance.buffer[address+1].code;
		p[2] = code_entry_instance.buffer[address+2].code;
		p[3] = code_entry_instance.buffer[address+3].code;
	}
	else
	{
		p[3] = code_entry_instance.buffer[address+0].code;
		p[2] = code_entry_instance.buffer[address+1].code;
		p[1] = code_entry_instance.buffer[address+2].code;
		p[0] = code_entry_instance.buffer[address+3].code;
	}
	return value;
}
