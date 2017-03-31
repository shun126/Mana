%{
/*
 * mana (compiler)
 *
 * @file	mana_lexer.l
 * @brief	字句解析に関するソースファイル
 * @detail	字句解析に関係するソースファイルです。
 * @author	Shun Moriya
 * @date	2003-
 */

#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_NODE_H___)
#include "node.h"
#endif
#if !defined(___MANA_POOL_H___)
#include "pool.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif

#if defined(__APPLE__)
#include "y.tab.h"
#else
#include "parser.h"
#endif

#if defined(_MSC_VER)
#define YY_NO_UNISTD_H
#include <direct.h>
#include <io.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NEST_LEVEL	(16)

/*!
 * include file environment stack
 */
static struct mana_lexer_status
{
	YY_BUFFER_STATE m_State;
	char* m_pszName;
	int32_t m_iLine;
} mana_lexer_status[MAX_NEST_LEVEL];

static int32_t mana_lexer_status_pointer = -1;
static char* mana_lexer_filename;
int32_t yylineno;

static char* mana_lexer_get_string(void);
static int32_t mana_lexer_from_binary(char*);

%}

ws			[ \t]
sym			[~!@%^&*()+\-\=|/?.,<>;:{}[\]]
digit		[0-9]
letter		[a-zA-Z_?]
id			{letter}({letter}|{digit})*
real		{digit}+"."{digit}*((E|e)("+"|"-")?{digit}+)?
string		\"([^\"\\\n]|\\.)*[\"\n]
other		.

%start		COMMENT_BLOCK COMMENT_LINE LINE HEX BIN

%%
<INITIAL>"//"		{ BEGIN COMMENT_LINE; }
<INITIAL>"/*"		{ BEGIN COMMENT_BLOCK; }
<INITIAL>"0x"		{ yymore(); BEGIN HEX; }
<INITIAL>"0b"		{ yymore(); BEGIN BIN; }
<INITIAL>{sym}		return yytext[0];
<INITIAL>{ws}+		;
<INITIAL>"\n"		yylineno++;
<INITIAL>"&&"		return tAND;
<INITIAL>"||"		return tOR;
<INITIAL>"<<"		return tLSHFT;
<INITIAL>">>"		return tRSHFT;
<INITIAL>"<="		return tLE;
<INITIAL>"=="		return tEQ;
<INITIAL>"!="		return tNE;
<INITIAL>">="		return tGE;
<INITIAL>"++"		return tINC;
<INITIAL>"--"		return tDEC;
<INITIAL>"+="		return tAADD;
<INITIAL>"-="		return tASUB;
<INITIAL>"*="		return tAMUL;
<INITIAL>"/="		return tADIV;
<INITIAL>"%="		return tAMOD;
<INITIAL>"&="		return tAAND;
<INITIAL>"|="		return tAOR;
<INITIAL>"^="		return tAXOR;
<INITIAL>"<<="		return tALSHFT;
<INITIAL>">>="		return tARSHFT;
<INITIAL>"**"		return tPOW;
<INITIAL>"::"		return tDC;

<INITIAL>"#line"	BEGIN LINE;

<INITIAL>"alias"	return tALIAS;
<INITIAL>"define"	return tDEFINE;
<INITIAL>"undef"	return tUNDEF;

<INITIAL>"include"	return tINCLUDE;
<INITIAL>"import"	return tIMPORT;

<INITIAL>"void"		{ yylval.type = mana_type_get(MANA_DATA_TYPE_VOID); return tTYPE; }
<INITIAL>"char"		{ yylval.type = mana_type_get(MANA_DATA_TYPE_CHAR); return tTYPE; }
<INITIAL>"short"	{ yylval.type = mana_type_get(MANA_DATA_TYPE_SHORT); return tTYPE; }
<INITIAL>"int"		{ yylval.type = mana_type_get(MANA_DATA_TYPE_INT); return tTYPE; }
<INITIAL>"float"	{ yylval.type = mana_type_get(MANA_DATA_TYPE_FLOAT); return tTYPE; }
<INITIAL>"string"	{ yylval.type = mana_type_string; return tTYPE; }
<INITIAL>"pointer"	{ yylval.type = mana_type_pointer; return tTYPE; }

<INITIAL>"native"	return tNATIVE;
<INITIAL>"struct"	return tSTRUCT;
<INITIAL>"actor"	return tACTOR;
<INITIAL>"Actor"	return tACTOR2;
<INITIAL>"phantom"	return tPHANTOM;
<INITIAL>"action"	return tACTION;
<INITIAL>"module"	return tMODULE;
<INITIAL>"extend"	return tEXTEND;
<INITIAL>"static"	return tSTATIC;
<INITIAL>"allocate"	return tALLOCATE;

<INITIAL>"false"	return tFALSE;
<INITIAL>"true"		return tTRUE;
<INITIAL>"nil"		return tNIL;
<INITIAL>"priority"	return tPRIORITY;
<INITIAL>"self"		return tSELF;
<INITIAL>"sender"	return tSENDER;

<INITIAL>"break"	return tBREAK;
<INITIAL>"continue"	return tCONTINUE;
<INITIAL>"case"		return tCASE;
<INITIAL>"default"	return tDEFAULT;
<INITIAL>"goto"		return GOTO;
<INITIAL>"halt"		return tHALT;
<INITIAL>"comply"	return tCOMPLY;
<INITIAL>"refuse"	return tREFUSE;
<INITIAL>"lock"		return tLOCK;
<INITIAL>"do"		return tDO;
<INITIAL>"else"		return tELSE;
<INITIAL>"for"		return tFOR;
<INITIAL>"if"		return tIF;
<INITIAL>"join"		return tJOIN;
<INITIAL>"loop"		return tLOOP;
<INITIAL>"return"	return tRETURN;
<INITIAL>"rollback"	return tROLLBACK;
<INITIAL>"switch"	return tSWITCH;
<INITIAL>"while"	return tWHILE;
<INITIAL>"print"	return tPRINT;
<INITIAL>"request"	return tREQUEST;
<INITIAL>"yield"	return tYIELD;

<INITIAL>"sizeof"	return tSIZEOF;

<INITIAL>{digit}+	{
	yylval.digit = atoi(yytext);
	return tDIGIT;
}
<INITIAL>{real}		{
	yylval.real = (float)atof(yytext);
	return tREAL;
}
<INITIAL>{id}		{
	yylval.string = mana_pool_set(yytext);
	return tIDENTIFIER;
}
<INITIAL>{string}	{
	yylval.string = mana_pool_set(mana_lexer_get_string());
	return tSTRING;
}
<INITIAL>{other}	{
	mana_print("%s(%d): error: Illegal char '%c' ignored\n",
		mana_lexer_get_current_filename(), mana_lexer_get_current_line(), yytext[0]);
}

<COMMENT_BLOCK>"/*"	{
	mana_compile_error("comment nest error");
}
<COMMENT_BLOCK>"*/"	BEGIN INITIAL;
<COMMENT_BLOCK>\n	yylineno ++;
<COMMENT_BLOCK>{other}	;

<COMMENT_LINE>"\n"	{
	yylineno ++;
	BEGIN INITIAL;
}
<COMMENT_LINE>{other}	;

<LINE>[0-9]+		yylineno = atoi((char*)yytext);
<LINE>{string}		{
	static char filename[_MAX_PATH];
	if(yyleng > 2)
	{
		memcpy(filename, &yytext[1], yyleng-2);
		filename[yyleng-2] = '\0';
		mana_lexer_filename = filename;
	}
}
<LINE>\n			BEGIN INITIAL;
<LINE>.				;

<HEX>[A-Fa-f0-9]+	{
#if defined(__STDC_WANT_SECURE_LIB__)
	sscanf_s(yytext, "%x", &yylval.digit);
#else
	sscanf(yytext, "%x", &yylval.digit);
#endif
	BEGIN INITIAL;
	return tDIGIT;
}
<HEX>{ws}+			;
<HEX>{other}		{
	mana_print("%s(%d): error: Illegal char '%c' ignored\n",
		mana_lexer_get_current_filename(), mana_lexer_get_current_line(), yytext[0]);
}

<BIN>[01_]+			{
	yylval.digit = mana_lexer_from_binary(yytext);
	BEGIN INITIAL;
	return tDIGIT;
}
<BIN>{ws}+			;
<BIN>{other}		{
	mana_print("%s(%d): error: Illegal char '%c' ignored\n",
		mana_lexer_get_current_filename(), mana_lexer_get_current_line(), yytext[0]);
}

%%

/*!
 * get string
 * return	string pointer
 */
static char* mana_lexer_get_string(void)
{
	char *p, *q;

	if(yytext[yyleng-1] != '\"')
	{
		mana_compile_error("unterminated string literal");
	}
	for(p = &yytext[1], q = yytext; p < yytext + (yyleng - 1); p++, q++)
	{
		switch(*p)
		{
		case '\\':
			switch(*(++p))
			{
			case 'n':	*q = '\n';	break;
			case 't':	*q = '\t';	break;
			case '\\':	*q = '\\';	break;
			}
			break;

		case '\n':
			yylineno ++;

		default:
			*q = *p;
			break;
		}
	}
	*q = '\0';
	yyleng = q - yytext;
	return yytext;
}

/*!
 * translate to binary from text
 * @param	p		text pointer
 * @return	integer number.
 */
static int32_t mana_lexer_from_binary(char* p)
{
	int32_t t0;
	int32_t t1;
	char* sp;

	p += 2;		/* '0b'ﾃ・εｯ竏づｩﾃｶﾃｳﾃ津・｣ｿﾃ厩ﾃ鵜ﾃ叡ﾃ益 */
	sp = p;

	t0 = 0;
	t1 = 0;
	while(*p != '\0')
	{
		if(*p == '_')
		{
			p ++;
			continue;
		}
		if(*p != '0' && *p != '1')
			break;
		p ++;
		t0 ++;
	}
	while(t0 > 0)
	{
		if(*sp == '_')
		{
			sp ++;
			continue;
		}
		t1 |= (*sp ++ - '0') << (t0 - 1);
		t0 --;
	}
	return t1;
}

/*!
 * check scanning complete.
 * @retval	true	scanning complete.
 * @retval	false	still need scanning.
 */
int32_t yywrap()
{
	return mana_lexer_close();
}

/*!
 * initialize scanner
 * @param	pFilename	file name pointer
 * @retval	true	success
 * @retval	false	failed
 */
int32_t mana_lexer_initialize(char* pFilename)
{
	mana_pool_initialize();
	mana_parser_initialize();
	yylineno = 1;
	return mana_lexer_open(pFilename, true);
}

/*!
 * finalize scanner
 */
void mana_lexer_finalize(void)
{
	if(mana_lexer_status_pointer > 0)
	{
		while(! yywrap())
			;
	}
	yylex_destroy();

	mana_parser_finalize();
	mana_pool_finalize();
}

/*!
 * check file opened
 * @param	path		file name pointer
 * @retval	true	file already opened
 * @retval	false	not yet
 */
static int32_t mana_lexer_is_already_opened(char* path)
{
	static char* pathList[2048];
	static int32_t pathCounter = 0;
	int32_t i;

	for(i = 0; i < pathCounter; i ++)
	{
		if(pathList[i] == path)
			return false;
	}
	pathList[pathCounter] = path;
	if(++pathCounter >= 2048)
		mana_compile_error("too many include files");

	return true;
}

/*!
 * open file
 * @param	filename	file name pointer
 * @param	check		
 * @retval	true	success
 * @retval	false	failed
 */
int32_t mana_lexer_open(char* filename, int32_t check)
{
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char* full_path;

	if(_fullpath(path, filename, _MAX_PATH) == NULL)
	{
		mana_compile_error("unable to open \"%s\"", filename);
		return false;
	}

	if(mana_debug)
		printf("lexer: %s\n", path);
/*
#if defined(_MSC_VER)
	full_path = path;
	while(*full_path)
	{
		*full_path = toupper(*full_path);
		full_path++;
	}
#endif
*/
	full_path = mana_pool_set(path);

	if(check)
	{
		if(! mana_lexer_is_already_opened(full_path))
		{
			return true;
		}
	}

#if defined(__STDC_WANT_SECURE_LIB__)
	if(fopen_s(&yyin, full_path, "rt") != 0)
#else
	if((yyin = fopen(full_path, "rt")) == NULL)
#endif
	{
		mana_compile_error("unable to open \"%s\"", full_path);
		return false;
	}
	if(++mana_lexer_status_pointer >= MAX_NEST_LEVEL)
	{
		mana_compile_error("too many include files");
		return false;
	}
	mana_lexer_status[mana_lexer_status_pointer].m_State = YY_CURRENT_BUFFER;
	mana_lexer_status[mana_lexer_status_pointer].m_pszName = full_path;
	mana_lexer_status[mana_lexer_status_pointer].m_iLine = yylineno;

	yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

	mana_lexer_filename = mana_lexer_status[mana_lexer_status_pointer].m_pszName;
	yylineno = 1;

	/* set currect directory */
#if defined(__STDC_WANT_SECURE_LIB__)
	_splitpath_s(path, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
	_makepath_s(path, sizeof(path), drive, dir, "", "");
#else
	_splitpath(path, drive, dir, fname, ext);
	_makepath(path, drive, dir, "", "");
#endif
	_chdir(path);

	return true;
}

/*!
 * close file
 * @retval	true	scanning complete.
 * @retval	false	still need scanning.
 */
int32_t mana_lexer_close()
{
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	yy_delete_buffer(YY_CURRENT_BUFFER);

	if(mana_lexer_status_pointer <= 0)
	{
		return true;
	}
	else
	{
		yy_switch_to_buffer(mana_lexer_status[mana_lexer_status_pointer].m_State);

		mana_lexer_filename = mana_lexer_status[mana_lexer_status_pointer-1].m_pszName;
		yylineno = mana_lexer_status[mana_lexer_status_pointer].m_iLine;
		
		mana_lexer_status_pointer --;

		/* set currect directory */
#if defined(__STDC_WANT_SECURE_LIB__)
		_splitpath_s(mana_lexer_filename, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
		_makepath_s(path, sizeof(path), drive, dir, "", "");
#else
		_splitpath(mana_lexer_filename, drive, dir, fname, ext);
		_makepath(path, drive, dir, "", "");
#endif
		_chdir(path);

		return false;
	}
}

/*!
get current file name
@return	current file name pointer
*/
const char* mana_lexer_get_current_filename(void)
{
	return mana_lexer_filename;
}

/*!
set current file name
@param[in]	filename	current file name pointer
*/
void mana_lexer_set_current_filename(const char* filename)
{
	mana_lexer_filename = filename;
}

/*!
get current line number
@return	current line number
*/
int32_t mana_lexer_get_current_line(void)
{
	return yylineno;
}

/*!
set current line number
@return	current line number
*/
void mana_lexer_set_current_line(const int32_t lineno)
{
	yylineno = lineno;
}
