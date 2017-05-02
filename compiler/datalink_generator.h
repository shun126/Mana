/*!
mana (compiler)

@file	datalink_generator.h
@brief	データセクションに関するヘッダファイル
@detail	このファイルはデータセクションに関係するヘッダファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_DATALINK_GENERATOR_H___)
#define ___MANA_DATALINK_GENERATOR_H___

#include <libmana.h>

//! initialize datalink object
extern void datalink_generator_initialize(void);

//! finalize datalink object
extern void datalink_generator_finalize(void);

/*!
get data count in datalink
 @return	data count
*/
extern size_t datalink_generator_get_number_of_files(void);

/*!
append data
@param[in]	file_name	file name
@return		index number. error if return value is negative.
*/
extern int32_t datalink_generator_append(const char* file_name);

/*!
write datalink file
@param[in]	file	file descriptor
@retval		TRUE	success
@retval		FALSE	write failed
*/
extern bool datalink_generator_write_data(mana_stream* stream);

#endif
