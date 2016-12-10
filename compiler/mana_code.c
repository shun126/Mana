/*!
 * mana (compiler)
 *
 * @file	mana_code.c
 * @brief	命令セクションに関するソースファイル
 * @detail	このファイルは命令セクションに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif

/*! 命令セクションのページサイズ */
#define MANA_CODE_ALLOCATE_PAGE_SIZE	(4096)

/*! 命令タイプ */
typedef struct mana_code_type
{
	signed int address;			/*!< アドレス */
	unsigned char code;			/*!< 命令コード */
} mana_code_type;

/*! 命令セクション */
typedef struct mana_code
{
	mana_code_type* buffer;		/*!< 命令タイプバッファー */
	signed int used_size;		/*!< 命令タイプバッファー使用サイズ */
	signed int allocated_size;	/*!< 命令タイプバッファー確保サイズ */
} mana_code;

static mana_code mana_code_buffer;

/*!
 */
void mana_code_initialize(void)
{
	mana_code_buffer.buffer = NULL;
	mana_code_buffer.allocated_size = 0;
	mana_code_buffer.used_size = 0;
}

/*!
 */
void mana_code_finalize(void)
{
	if(mana_code_buffer.buffer)
	{
		mana_free(mana_code_buffer.buffer);
		mana_code_buffer.buffer = NULL;
	}
	mana_code_buffer.allocated_size = 0;
	mana_code_buffer.used_size = 0;
}

/*!
 * @param[in]	size	縮小するサイズ
 */
void mana_code_reduce(size_t size)
{
	if(size > 0)
	{
		mana_code_buffer.used_size -= size;
		if(mana_code_buffer.used_size < 0)
		{
			mana_code_buffer.used_size = 0;
		}
	}
}

/*!
 * 命令セクションに値を設定します
 *
 * @param[in]	code	オペコード
 * @param[in]	address	オペコードに関係付けるアドレス
 * @return		設定したアドレス
 */
static int mana_code_set(unsigned char code, int address)
{
	int pc = mana_code_get_pc();
	if(mana_code_buffer.used_size >= mana_code_buffer.allocated_size)
	{
		mana_code_buffer.allocated_size += MANA_CODE_ALLOCATE_PAGE_SIZE;
		mana_code_buffer.buffer = (mana_code_type*)mana_realloc(
			mana_code_buffer.buffer, mana_code_buffer.allocated_size * sizeof(mana_code_type));
	}
	mana_code_buffer.buffer[mana_code_buffer.used_size].code = code;
	mana_code_buffer.buffer[mana_code_buffer.used_size].address = address;
	mana_code_buffer.used_size++;
	return pc;
}

/*!
 * @param[in]	value	値
 */
void mana_code_set_char(char value)
{
	mana_code_set(value, -1);
}

/*!
 * @param[in]	value	値
 */
void mana_code_set_short(short value)
{
	unsigned char* p = (unsigned char*)&value;
	if(mana_is_big_endian())
	{
		mana_code_set(p[0], -1);
		mana_code_set(p[1], -1);
	}
	else
	{
		mana_code_set(p[1], -1);
		mana_code_set(p[0], -1);
	}
}

/*!
 * @param[in]	value	値
 */
void mana_code_set_integer(int value)
{
	unsigned char* p = (unsigned char*)&value;
	if(mana_is_big_endian())
	{
		mana_code_set(p[0], -1);
		mana_code_set(p[1], -1);
		mana_code_set(p[2], -1);
		mana_code_set(p[3], -1);
	}
	else
	{
		mana_code_set(p[3], -1);
		mana_code_set(p[2], -1);
		mana_code_set(p[1], -1);
		mana_code_set(p[0], -1);
	}
}

/*!
 * @param[in]	value	値
 */
void mana_code_set_unsigned_char(unsigned char value)
{
	mana_code_set_char((char)value);
}

/*!
 * @param[in]	value	値
 */
void mana_code_set_unsigned_short(unsigned short value)
{
	mana_code_set_short((short)value);
}

/*!
 * @param[in]	value	値
 */
void mana_code_set_unsigned_integer(unsigned int value)
{
	mana_code_set_integer((int)value);
}

/*!
 * @param[in]	value	値
 */
void mana_code_set_float(float value)
{
	unsigned char* p = (unsigned char*)&value;
	if(mana_is_big_endian())
	{
		mana_code_set(p[0], -1);
		mana_code_set(p[1], -1);
		mana_code_set(p[2], -1);
		mana_code_set(p[3], -1);
	}
	else
	{
		mana_code_set(p[3], -1);
		mana_code_set(p[2], -1);
		mana_code_set(p[1], -1);
		mana_code_set(p[0], -1);
	}
}

/*!
 * @param[in]	code	オペコード
 */
void mana_code_set_opecode(unsigned char code)
{
	mana_code_set(code, -1);
}

/*!
 * @param[in]	code	オペコード
 * @param[in]	address	オペランド
 */
int mana_code_set_opecode_and_operand(unsigned char code, int address)
{
	int pc;
	pc = mana_code_get_pc();
	mana_code_set(code, address);
	mana_code_set_integer(address);
	return pc;
}

/*!
 * @param[in]	code	オペコード
 * @param[in]	address	第一オペランド
 * @param[in]	size	第二オペランド
 */
int mana_code_set_opecode_and_two_operands(unsigned char code, int address, int size)
{
	int pc;
	pc = mana_code_get_pc();
	mana_code_set(code, address);
	mana_code_set_integer(address);
	mana_code_set_integer(size);
	return pc;
}

/*!
 * @param[in]	address	アドレス
 * @param[in]	code	オペコード
 */
void mana_code_replace_opecode(int address, unsigned char code)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");
	mana_code_buffer.buffer[address].code = code;
}

/*!
 * @param[in]	address		アドレス
 * @param[in]	new_address	置き換えるアドレス
 */
void mana_code_replace_address(int address, int new_address)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");

	mana_code_buffer.buffer[address+0].code = (unsigned char)(new_address >> 24);
	mana_code_buffer.buffer[address+1].code = (unsigned char)(new_address >> 16);
	mana_code_buffer.buffer[address+2].code = (unsigned char)(new_address >> 8);
	mana_code_buffer.buffer[address+3].code = (unsigned char)(new_address);
}

/*!
 * @param[in]	address		アドレス
 * @param[in]	new_address	置き換えるアドレス
 */
void mana_code_replace_all(int address, int new_address)
{
	while(address >= 0)
	{
		int next_address;

		MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");

		next_address = mana_code_buffer.buffer[address].address;
		mana_code_buffer.buffer[address+0].address = -1;
		mana_code_buffer.buffer[address+1].code = (unsigned char)(new_address >> 24);
		mana_code_buffer.buffer[address+2].code = (unsigned char)(new_address >> 16);
		mana_code_buffer.buffer[address+3].code = (unsigned char)(new_address >> 8);
		mana_code_buffer.buffer[address+4].code = (unsigned char)(new_address);
		address = next_address;
	}
}

/*!
 * @param[out]	file		ファイル識別子
 * @retval		MANA_TRUE	出力成功
 * @retval		MANA_FALSE	出力失敗
 */
int mana_code_write(mana_stream* stream)
{
	int i;
	for(i = 0; i < mana_code_get_size(); i ++)
	{
		mana_stream_push_unsigned_char(stream, mana_code_buffer.buffer[i].code);
	}
	return MANA_TRUE;
}

/*!
 * @param[out]	file	ファイル識別子
 * @return		出力したサイズ
 */
int mana_code_copy(void* buffer)
{
	unsigned char* code = (unsigned char*)buffer;
	int i;
	for(i = 0; i < mana_code_buffer.used_size; i ++)
	{
		*code ++ = mana_code_buffer.buffer[i].code;
	}
	return i;
}


/*!
 * @return	命令セクションのサイズ
 */
int mana_code_get_pc(void)
{
	return mana_code_get_size();
}

/*!
 * @return	命令セクションのサイズ
 */
int mana_code_get_size(void)
{
	return mana_code_buffer.used_size;
}

/*!
 * @return	命令セクションバッファのアドレス
 */
void* mana_code_get_buffer(void)
{
	return (void*)(mana_code_buffer.buffer);
}

/*!
 * @param[in]	address		アドレス
 * @return		オペコード
 */
unsigned char mana_code_getcode(int address)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");
	return mana_code_buffer.buffer[address].code;
}

/*!
 * @param[in]	address		アドレス
 * @return		int型数値
 */
int mana_code_get_integer(int address)
{
	int value;
	unsigned char* p = (unsigned char*)&value;
	if(mana_is_big_endian())
	{
		p[0] = mana_code_buffer.buffer[address+0].code;
		p[1] = mana_code_buffer.buffer[address+1].code;
		p[2] = mana_code_buffer.buffer[address+2].code;
		p[3] = mana_code_buffer.buffer[address+3].code;
	}
	else
	{
		p[3] = mana_code_buffer.buffer[address+0].code;
		p[2] = mana_code_buffer.buffer[address+1].code;
		p[1] = mana_code_buffer.buffer[address+2].code;
		p[0] = mana_code_buffer.buffer[address+3].code;
	}
	return value;
}

/*!
 * @param[in]	address		アドレス
 * @return		float型数値
 */
float mana_code_get_float(int address)
{
	float value;
	unsigned char* p = (unsigned char*)&value;
	if(mana_is_big_endian())
	{
		p[0] = mana_code_buffer.buffer[address+0].code;
		p[1] = mana_code_buffer.buffer[address+1].code;
		p[2] = mana_code_buffer.buffer[address+2].code;
		p[3] = mana_code_buffer.buffer[address+3].code;
	}
	else
	{
		p[3] = mana_code_buffer.buffer[address+0].code;
		p[2] = mana_code_buffer.buffer[address+1].code;
		p[1] = mana_code_buffer.buffer[address+2].code;
		p[0] = mana_code_buffer.buffer[address+3].code;
	}
	return value;
}
