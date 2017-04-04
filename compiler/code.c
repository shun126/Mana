/*!
mana (compiler)

@file	code.c
@brief	���߃Z�N�V�����Ɋւ���\�[�X�t�@�C��
@detail	���̃t�@�C���͖��߃Z�N�V�����Ɋ֌W����\�[�X�t�@�C���ł��B
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_CODE_H___)
#include "code.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif

/*! ���߃Z�N�V�����̃y�[�W�T�C�Y */
#define MANA_CODE_ALLOCATE_PAGE_SIZE	(4096)

/*! ���߃^�C�v */
typedef struct mana_code_type
{
	int32_t address;			/*!< �A�h���X */
	uint8_t code;				/*!< ���߃R�[�h */
} mana_code_type;

/*! ���߃Z�N�V���� */
typedef struct mana_code
{
	mana_code_type* buffer;		/*!< ���߃^�C�v�o�b�t�@�[ */
	int32_t used_size;			/*!< ���߃^�C�v�o�b�t�@�[�g�p�T�C�Y */
	int32_t allocated_size;		/*!< ���߃^�C�v�o�b�t�@�[�m�ۃT�C�Y */
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
static int32_t mana_code_set(uint8_t code, int32_t address)
{
	int32_t pc = mana_code_get_pc();
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
void mana_code_set_char(int8_t value)
{
	mana_code_set(value, -1);
}

/*!
 * @param[in]	value	�l
 */
void mana_code_set_short(int16_t value)
{
	uint8_t* p = (uint8_t*)&value;
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
void mana_code_set_integer(int32_t value)
{
	uint8_t* p = (uint8_t*)&value;
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
void mana_code_set_unsigned_char(uint8_t value)
{
	mana_code_set_char((int8_t)value);
}

/*!
 * @param[in]	value	�l
 */
void mana_code_set_unsigned_short(uint16_t value)
{
	mana_code_set_short((int16_t)value);
}

/*!
 * @param[in]	value	�l
 */
void mana_code_set_unsigned_integer(uint32_t value)
{
	mana_code_set_integer((int32_t)value);
}

/*!
 * @param[in]	value	�l
 */
void mana_code_set_float(float value)
{
	uint8_t* p = (uint8_t*)&value;
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
void mana_code_set_opecode(uint8_t code)
{
	mana_code_set(code, -1);
}

/*!
 * @param[in]	code	�I�y�R�[�h
 * @param[in]	address	�I�y�����h
 */
int32_t mana_code_set_opecode_and_operand(uint8_t code, int32_t address)
{
	int32_t pc;
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
int32_t mana_code_set_opecode_and_two_operands(uint8_t code, int32_t address, int32_t size)
{
	int32_t pc;
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
void mana_code_replace_opecode(int32_t address, uint8_t code)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");
	mana_code_buffer.buffer[address].code = code;
}

/*!
 * @param[in]	address		�A�h���X
 * @param[in]	new_address	�u��������A�h���X
 */
void mana_code_replace_address(int32_t address, int32_t new_address)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");

	mana_code_buffer.buffer[address+0].code = (uint8_t)(new_address >> 24);
	mana_code_buffer.buffer[address+1].code = (uint8_t)(new_address >> 16);
	mana_code_buffer.buffer[address+2].code = (uint8_t)(new_address >> 8);
	mana_code_buffer.buffer[address+3].code = (uint8_t)(new_address);
}

/*!
 * @param[in]	address		�A�h���X
 * @param[in]	new_address	�u��������A�h���X
 */
void mana_code_replace_all(int32_t address, int32_t new_address)
{
	while(address >= 0)
	{
		int32_t next_address;

		MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");

		next_address = mana_code_buffer.buffer[address].address;
		mana_code_buffer.buffer[address+0].address = -1;
		mana_code_buffer.buffer[address+1].code = (uint8_t)(new_address >> 24);
		mana_code_buffer.buffer[address+2].code = (uint8_t)(new_address >> 16);
		mana_code_buffer.buffer[address+3].code = (uint8_t)(new_address >> 8);
		mana_code_buffer.buffer[address+4].code = (uint8_t)(new_address);
		address = next_address;
	}
}

/*!
 * @param[out]	file		�t�@�C�����ʎq
 * @retval		true	�o�͐���
 * @retval		false	�o�͎��s
 */
int32_t mana_code_write(mana_stream* stream)
{
	int32_t i;
	for(i = 0; i < mana_code_get_size(); i ++)
	{
		mana_stream_push_unsigned_char(stream, mana_code_buffer.buffer[i].code);
	}
	return true;
}

/*!
 * @param[out]	file	�t�@�C�����ʎq
 * @return		�o�͂����T�C�Y
 */
int32_t mana_code_copy(void* buffer)
{
	uint8_t* code = (uint8_t*)buffer;
	int32_t i;
	for(i = 0; i < mana_code_buffer.used_size; i ++)
	{
		*code ++ = mana_code_buffer.buffer[i].code;
	}
	return i;
}

/*!
 * @return	���߃Z�N�V�����̃T�C�Y
 */
int32_t mana_code_get_pc(void)
{
	return mana_code_get_size();
}

/*!
 * @return	���߃Z�N�V�����̃T�C�Y
 */
int32_t mana_code_get_size(void)
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
uint8_t mana_code_getcode(int32_t address)
{
	MANA_VERIFY(address >= 0 && address < mana_code_get_size(), "Out of range");
	return mana_code_buffer.buffer[address].code;
}

/*!
 * @param[in]	address		�A�h���X
 * @return		int32_t�^���l
 */
int32_t mana_code_get_integer(int32_t address)
{
	int32_t value;
	uint8_t* p = (uint8_t*)&value;
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
float mana_code_get_float(int32_t address)
{
	float value;
	uint8_t* p = (uint8_t*)&value;
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
