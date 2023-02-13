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
#include "SyntaxNode.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"
#include "ParsingDriver.h"
#include <stdio.h>
#include <stdint.h>

#if defined(MANA_TARGET_WINDOWS)
#include <io.h>
#define fileno _fileno
#define isatty _isatty
#define YY_NO_UNISTD_H
#elif defined(MANA_TARGET_UNIX)
#include <unistd.h>
#endif

#include "LexerDeclaration.inl"

#define YY_DECL int yylex(mana::Parser::value_type* yylval)

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
<INITIAL>"&&"		return mana::Parser::token::tAND;
<INITIAL>"||"		return mana::Parser::token::tOR;
<INITIAL>"<<"		return mana::Parser::token::tLSHFT;
<INITIAL>">>"		return mana::Parser::token::tRSHFT;
<INITIAL>"<="		return mana::Parser::token::tLE;
<INITIAL>"=="		return mana::Parser::token::tEQ;
<INITIAL>"!="		return mana::Parser::token::tNE;
<INITIAL>">="		return mana::Parser::token::tGE;
<INITIAL>"++"		return mana::Parser::token::tINC;
<INITIAL>"--"		return mana::Parser::token::tDEC;
<INITIAL>"+="		return mana::Parser::token::tAADD;
<INITIAL>"-="		return mana::Parser::token::tASUB;
<INITIAL>"*="		return mana::Parser::token::tAMUL;
<INITIAL>"/="		return mana::Parser::token::tADIV;
<INITIAL>"%="		return mana::Parser::token::tAMOD;
<INITIAL>"&="		return mana::Parser::token::tAAND;
<INITIAL>"|="		return mana::Parser::token::tAOR;
<INITIAL>"^="		return mana::Parser::token::tAXOR;
<INITIAL>"<<="		return mana::Parser::token::tALSHFT;
<INITIAL>">>="		return mana::Parser::token::tARSHFT;
<INITIAL>"**"		return mana::Parser::token::tPOW;
<INITIAL>"::"		return mana::Parser::token::tDC;

<INITIAL>"#line"	BEGIN LINE;

<INITIAL>"define"	return mana::Parser::token::tDEFINE;
<INITIAL>"undef"	return mana::Parser::token::tUNDEF;

<INITIAL>"include"	return mana::Parser::token::tINCLUDE;
<INITIAL>"import"	return mana::Parser::token::tIMPORT;

<INITIAL>"void"		{ yylval->build<std::shared_ptr<mana::TypeDescriptor>>() = mana::GetVoidTypeDescriptor(); return mana::Parser::token::tTYPE; }
<INITIAL>"char"		{ yylval->build<std::shared_ptr<mana::TypeDescriptor>>() = mana::GetInt8TypeDescriptor(); return mana::Parser::token::tTYPE; }
<INITIAL>"short"	{ yylval->build<std::shared_ptr<mana::TypeDescriptor>>() = mana::GetInt16TypeDescriptor(); return mana::Parser::token::tTYPE; }
<INITIAL>"int"		{ yylval->build<std::shared_ptr<mana::TypeDescriptor>>() = mana::GetInt32TypeDescriptor(); return mana::Parser::token::tTYPE; }
<INITIAL>"float"	{ yylval->build<std::shared_ptr<mana::TypeDescriptor>>() = mana::GetFloat32TypeDescriptor(); return mana::Parser::token::tTYPE; }
<INITIAL>"string"	{ yylval->build<std::shared_ptr<mana::TypeDescriptor>>() = mana::GetStringTypeDescriptor(); return mana::Parser::token::tTYPE; }
<INITIAL>"pointer"	{ yylval->build<std::shared_ptr<mana::TypeDescriptor>>() = mana::GetReferenceTypeDescriptor(); return mana::Parser::token::tTYPE; }

<INITIAL>"native"	return mana::Parser::token::tNATIVE;
<INITIAL>"struct"	return mana::Parser::token::tSTRUCT;
<INITIAL>"actor"	return mana::Parser::token::tACTOR;
<INITIAL>"Actor"	return mana::Parser::token::tACTOR2;
<INITIAL>"phantom"	return mana::Parser::token::tPHANTOM;
<INITIAL>"action"	return mana::Parser::token::tACTION;
<INITIAL>"module"	return mana::Parser::token::tMODULE;
<INITIAL>"extend"	return mana::Parser::token::tEXTEND;
<INITIAL>"static"	return mana::Parser::token::tSTATIC;
<INITIAL>"allocate"	return mana::Parser::token::tALLOCATE;

<INITIAL>"false"	return mana::Parser::token::tFALSE;
<INITIAL>"true"		return mana::Parser::token::tTRUE;
<INITIAL>"nil"		return mana::Parser::token::tNIL;
<INITIAL>"priority"	return mana::Parser::token::tPRIORITY;
<INITIAL>"self"		return mana::Parser::token::tSELF;
<INITIAL>"sender"	return mana::Parser::token::tSENDER;

<INITIAL>"break"	return mana::Parser::token::tBREAK;
<INITIAL>"continue"	return mana::Parser::token::tCONTINUE;
<INITIAL>"case"		return mana::Parser::token::tCASE;
<INITIAL>"default"	return mana::Parser::token::tDEFAULT;
<INITIAL>"goto"		return mana::Parser::token::GOTO;
<INITIAL>"halt"		return mana::Parser::token::tHALT;
<INITIAL>"comply"	return mana::Parser::token::tCOMPLY;
<INITIAL>"refuse"	return mana::Parser::token::tREFUSE;
<INITIAL>"lock"		return mana::Parser::token::tLOCK;
<INITIAL>"do"		return mana::Parser::token::tDO;
<INITIAL>"else"		return mana::Parser::token::tELSE;
<INITIAL>"for"		return mana::Parser::token::tFOR;
<INITIAL>"if"		return mana::Parser::token::tIF;
<INITIAL>"join"		return mana::Parser::token::tJOIN;
<INITIAL>"loop"		return mana::Parser::token::tLOOP;
<INITIAL>"return"	return mana::Parser::token::tRETURN;
<INITIAL>"rollback"	return mana::Parser::token::tROLLBACK;
<INITIAL>"switch"	return mana::Parser::token::tSWITCH;
<INITIAL>"while"	return mana::Parser::token::tWHILE;
<INITIAL>"print"	return mana::Parser::token::tPRINT;
<INITIAL>"request"	return mana::Parser::token::tREQUEST;
<INITIAL>"yield"	return mana::Parser::token::tYIELD;

<INITIAL>"sizeof"	return mana::Parser::token::tSIZEOF;

<INITIAL>{digit}+	{ yylval->build<mana::int_t>() = mana::ToInt(yytext); return mana::Parser::token::tDIGIT; }
<INITIAL>{real}		{ yylval->build<mana::float_t>() = mana::ToFloat(yytext); return mana::Parser::token::tREAL; }
<INITIAL>{id}		{ yylval->build<std::string_view>() = mana::ToString(yytext); return mana::Parser::token::tIDENTIFIER; }
<INITIAL>{string}	{ yylval->build<std::string_view>() = mana::LiteralToString(yytext, yyleng); return mana::Parser::token::tSTRING; }
<INITIAL>{other}	{ if(yytext[0] != '\r') mana::CompileError({ "Illegal char '", std::string_view(&yytext[0], 1), "' ignored\n" }); }

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
		mana::mLexer->SetCurrentFilename(filename);
	}
}
<LINE>\n			BEGIN INITIAL;
<LINE>.				;

<HEX>[A-Fa-f0-9]+	{ yylval->build<mana::int_t>() = mana::ToIntHex(yytext); BEGIN INITIAL; return mana::Parser::token::tDIGIT; }
<HEX>{ws}+			;
<HEX>{other}		{ mana::CompileError({ "Illegal char '", std::string_view(&yytext[0], 1), "' ignored\n" }); }

<BIN>[01_]+			{ yylval->build<mana::int_t>() = mana::mLexer->binary(yytext); BEGIN INITIAL; return mana::Parser::token::tDIGIT; }
<BIN>{ws}+			;
<BIN>{other}		{ mana::CompileError({ "Illegal char '", std::string_view(&yytext[0], 1), "' ignored\n" }); }

%%

#include "LexerDefinition.inl"
#include "LexerPrivateFunction.inl"

int yywrap()
{
	return mana::lexer_close();
}
