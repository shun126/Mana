/*!
 * mana (compiler)
 *
 * @file	mana_code.c
 * @brief	���߃Z�N�V�����Ɋւ���\�[�X�t�@�C��
 * @detail	���̃t�@�C���͖��߃Z�N�V�����Ɋ֌W����\�[�X�t�@�C���ł��B
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif

/*! ���߃Z�N�V�����̃y�[�W�T�C�Y */
#define MANA_CODE_ALLOCATE_PAGE_SIZE	(4096)

/*! ���߃^�C�v */
typedef struct mana_code_type
{
	signed int address;			/*!< �A�h���X */
	unsigned char code;			/*!< ���߃R�[�h */
} mana_code_type;

/*! ���߃Z�N�V���� */
typedef struct mana_code
{
	mana_code_type* buffer;		/*!< ���߃^�C�v�o�b�t�@�[ */
	signed int used_size;		/*!< ���߃^�C�v�o�b�t�@�[�g�p�T�C�Y */
	signed int allocated_size;	/*!< ���߃^�C�v�o�b�t�@�[�m�ۃT�C�Y */
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
 * @param[in]	size	�k������T�C�Y
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
 * ���߃Z�N�V�����ɒl��ݒ肵�܂�
 *
 * @param[in]	code	�I�y�R�[�h
 * @param[in]	address	�I�y�R�[�h�Ɋ֌W�t����A�h���X
 * @return		�ݒ肵���A�h���X
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
 * @param[in]	value	�l
 */
void mana_code_set_char(char value)
{
	mana_code_set(value, -1);
}

/*!
 * @param[in]	value	�l
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
 * @param[in]	value	�l
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
 * @param[in]	value	�l
 */
void mana_code_set_unsigned_char(unsigned char value)
{
	mana_code_set_char((char)value);
}

/*!
 * @param[in]	value	�l
 */
void mana_code_set_unsigned_short(unsigned short value)
{
	mana_code_set_short((short)value);
}

/*!
 * @param[in]	value	�l
 */
void mana_code_set_unsigned_integer(unsigned int value)
{
	mana_code_set_integer((int)value);
}

/*!
 * @param[in]	value	�l
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
 * @param[in]	code	�I�y�R�[�h
 */
void mana_code_set_opecode(unsigned char code)
{
	mana_code_set(code, -1);
}

/*!
 * @param[in]	code	�I�y�R�[�h
 * @param[in]	address	�I�y�����h
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
 * @param[in]	code	�I�y�R�[�h
 * @param[in]	address	���I�y�����h
 * @param[in]	size	���I�y�����h
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
 * @param[in]	address	�A�h���X
 * @param[in]	code	�I�y�R�[�h
 */
void mana_code_replace_opecode(int address, unsigned char code)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");
	mana_code_buffer.buffer[address].code = code;
}

/*!
 * @param[in]	address		�A�h���X
 * @param[in]	new_address	�u��������A�h���X
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
 * @param[in]	address		�A�h���X
 * @param[in]	new_address	�u��������A�h���X
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
 * @param[out]	file		�t�@�C�����ʎq
 * @retval		MANA_TRUE	�o�͐���
 * @retval		MANA_FALSE	�o�͎��s
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
 * @param[out]	file	�t�@�C�����ʎq
 * @return		�o�͂����T�C�Y
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
 * @return	���߃Z�N�V�����̃T�C�Y
 */
int mana_code_get_pc(void)
{
	return mana_code_get_size();
}

/*!
 * @return	���߃Z�N�V�����̃T�C�Y
 */
int mana_code_get_size(void)
{
	return mana_code_buffer.used_size;
}

/*!
 * @return	���߃Z�N�V�����o�b�t�@�̃A�h���X
 */
void* mana_code_get_buffer(void)
{
	return (void*)(mana_code_buffer.buffer);
}

/*!
 * @param[in]	address		�A�h���X
 * @return		�I�y�R�[�h
 */
unsigned char mana_code_getcode(int address)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");
	return mana_code_buffer.buffer[address].code;
}

/*!
 * @param[in]	address		�A�h���X
 * @return		int�^���l
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
 * @param[in]	address		�A�h���X
 * @return		float�^���l
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
