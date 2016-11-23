/*!
 * mana (library)
 *
 * @file	mana_lzss.c
 * @brief	LZSSに関するソースファイル
 * @detail
 * LZSS - Lempel-Ziv sliding window compression
 * 技術評論社 C言語による最新アルゴリズム辞典 から
 * ウィンドウバッファを使用しない方法に改変しました
 *
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#include "mana_lzss.h"
#include <string.h>

#define RING_BUFFER_SIZE (4096)
#define MATCH_SIZE (18)
#define NIL (RING_BUFFER_SIZE)
#define RING_MASK_8 (RING_BUFFER_SIZE - 1)

#define MAGIC "LzSs"
#define VERSION 1
#define DICTONARY 1

/*************************************************************************/
typedef struct mana_lzss_header
{
	char magic[4];
	unsigned char version;
	unsigned char dictonary;
	unsigned short crc;
	size_t extract_size;
	size_t compressed_size;
}mana_lzss_header;

/*************************************************************************/
extern const char* mana_lzss_get_magic_word()
{
	return MAGIC;
}

/*************************************************************************/
extern size_t mana_lzss_get_magic_word_size()
{
	return 4;
}

/*************************************************************************/
extern unsigned char mana_lzss_get_version()
{
	return VERSION;
}

/*************************************************************************/
extern unsigned char mana_lzss_get_dictonary()
{
	return DICTONARY;
}

/*************************************************************************/
extern mana_bool mana_lzss_is_valid(const void* compressed_data_pointer)
{
	const mana_lzss_header* header;
	if(compressed_data_pointer == NULL)
		return MANA_FALSE;
	header = (const mana_lzss_header*)(compressed_data_pointer);
	return mana_lzss_is_compressed(compressed_data_pointer) && header->version == VERSION && header->dictonary == DICTONARY;
}

/*************************************************************************/
extern mana_bool mana_lzss_is_compressed(const void* compressed_data_pointer)
{
	assert(compressed_data_pointer);
	return memcmp(MAGIC, compressed_data_pointer, 4 - 1) == 0;
}

/*************************************************************************/
extern size_t mana_lzss_get_extract_data_size(const void* compressed_data_pointer)
{
	assert(compressed_data_pointer);
	return ((const mana_lzss_header*)compressed_data_pointer)->extract_size;
}

/*************************************************************************/
extern size_t mana_lzss_get_compressed_data_size(const void* compressed_data_pointer)
{
	assert(compressed_data_pointer);
	return ((const mana_lzss_header*)compressed_data_pointer)->compressed_size;
}

/*************************************************************************/
extern void mana_lzss_extract(void* extract_data_pointer, const void* compressed_data_pointer)
{
	mana_lzss_header* header;
	unsigned char *s, *d, *lim, *dstart;
	int flags, cnt;

	assert(extract_data_pointer);
	assert(compressed_data_pointer);

	d = (unsigned char*)extract_data_pointer;
	header = (mana_lzss_header*)compressed_data_pointer;

	flags = cnt = 0;

	s = (unsigned char*)(header + 1);
	lim = s + header->compressed_size;
	dstart = d;

	for(;;)
	{
		if(!cnt)
		{
			cnt = 8;
			if(s >= lim)
				break;
			flags = *s++;
		}
		if(flags & 1)
		{
			if(s >= lim)
				break;
			*d++ = *s++;
		}
		else
		{
			unsigned char *limd, *p;
			int i, j;
			if(s >= lim)
				break;
			j = *s++;
			i = *s++;
			j |= (i & 0xf0) << 4;
			limd = d + (i & 0xf) + 3;
			p = d - ((d - dstart - (j - (RING_BUFFER_SIZE - MATCH_SIZE))) & RING_MASK_8);
			while(p < dstart)
			{
				*d++ = 0;
				p++;
			}
			while(d < limd)
			{
				*d++ = *p++;
			}
		}
		flags >>= 1;
		cnt--;
	}
}
