%{
/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "Lexer.h"
#include "ErrorHandler.h"
#include "Path.h"
#include "SymbolEntry.h"
#include "SyntaxNode.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"
#include "Parser.hpp"

#if defined(MANA_TARGET_WINDOWS)
#include <io.h>
#define fileno _fileno
#define isatty _isatty
#define YY_NO_UNISTD_H
#endif

#include "LexerDeclaration.inl"

static Lexer* mLexer = nullptr;

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

<INITIAL>"void"		{ yylval.mTypeDescriptor = mana::GetVoidTypeDescriptor(); return tTYPE; }
<INITIAL>"char"		{ yylval.mTypeDescriptor = mana::GetInt8TypeDescriptor(); return tTYPE; }
<INITIAL>"short"	{ yylval.mTypeDescriptor = mana::GetInt16TypeDescriptor(); return tTYPE; }
<INITIAL>"int"		{ yylval.mTypeDescriptor = mana::GetInt32TypeDescriptor(); return tTYPE; }
<INITIAL>"float"	{ yylval.mTypeDescriptor = mana::GetFloat32TypeDescriptor(); return tTYPE; }
<INITIAL>"string"	{ yylval.mTypeDescriptor = mana::GetStringTypeDescriptor(); return tTYPE; }
<INITIAL>"pointer"	{ yylval.mTypeDescriptor = mana::GetReferenceTypeDescriptor(); return tTYPE; }

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

<INITIAL>{digit}+	{ yylval.mInt = mana::ToInt(yytext); return tDIGIT; }
<INITIAL>{real}		{ yylval.mReal = mana::ToFloat(yytext); return tREAL; }
<INITIAL>{id}		{ yylval.mString = mana::ToString(yytext); return tIDENTIFIER; }
<INITIAL>{string}	{ yylval.mString = mana::LiteralToString(yytext, yyleng); return tSTRING; }
<INITIAL>{other}	{ if(yytext[0] != '\r') mana::CompileError("Illegal char '%c' ignored\n", yytext[0]); }

<COMMENT_BLOCK>"/*"	{ mana::CompileError("comment nest error"); }
<COMMENT_BLOCK>"*/"	BEGIN INITIAL;
<COMMENT_BLOCK>\n	++yylineno;
<COMMENT_BLOCK>{other}	;

<COMMENT_LINE>"\n"	{ ++yylineno; BEGIN INITIAL; }
<COMMENT_LINE>{other}	;

<LINE>[0-9]+		yylineno = mana::ToInt(yytext);
<LINE>{string}		{
	static char filename[_MAX_PATH];
	if(yyleng > 2)
	{
		memcpy(filename, &yytext[1], yyleng-2);
		filename[yyleng-2] = '\0';
		mLexer->SetCurrentFilename(filename);
	}
}
<LINE>\n			BEGIN INITIAL;
<LINE>.				;

<HEX>[A-Fa-f0-9]+	{ yylval.mInt = mana::ToIntHex(yytext); BEGIN INITIAL; return tDIGIT; }
<HEX>{ws}+			;
<HEX>{other}		{ mana::CompileError("Illegal char '%c' ignored\n", yytext[0]); }

<BIN>[01_]+			{ yylval.mInt = mLexer->binary(yytext); BEGIN INITIAL; return tDIGIT; }
<BIN>{ws}+			;
<BIN>{other}		{ mana::CompileError("Illegal char '%c' ignored\n", yytext[0]); }

%%

#include "LexerDefinition.inl"
#include "LexerPrivateFunction.inl"

int yywrap()
{
	return lexer_close();
}

bool lexer_initialize(const char* filename)
{
	if(mLexer == nullptr)
	{
		mLexer = new Lexer;
		yylineno = 1;
	}
	return mLexer->Open(filename, true);
}

void lexer_finalize(void)
{
	if(mLexer)
	{
		delete mLexer;
		mLexer = nullptr;

		yylex_destroy();
	}
}

bool lexer_open(const char* filename, const bool check)
{
	return mLexer->Open(filename, check);
}

bool lexer_close()
{
	return mLexer->Close();
}

const char* lexer_get_current_filename(void)
{
	return mLexer->GetCurrentFilename();
}

void lexer_set_current_filename(const char* filename)
{
	mLexer->SetCurrentFilename(filename);
}

int lexer_get_current_lineno(void)
{
	return yylineno;
}

void lexer_set_current_lineno(const int lineno)
{
	yylineno = lineno;
}
