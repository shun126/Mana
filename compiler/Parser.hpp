/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    tDIGIT = 258,
    tREAL = 259,
    tSTRING = 260,
    tIDENTIFIER = 261,
    tTYPE = 262,
    tALIAS = 263,
    tDEFINE = 264,
    tUNDEF = 265,
    tINCLUDE = 266,
    tIMPORT = 267,
    tNATIVE = 268,
    tSTRUCT = 269,
    tACTOR = 270,
    tACTOR2 = 271,
    tPHANTOM = 272,
    tACTION = 273,
    tMODULE = 274,
    tEXTEND = 275,
    tFALSE = 276,
    tTRUE = 277,
    tPRIORITY = 278,
    tSELF = 279,
    tSENDER = 280,
    tNIL = 281,
    tREQUEST = 282,
    tJOIN = 283,
    tBREAK = 284,
    tCONTINUE = 285,
    tCASE = 286,
    tDEFAULT = 287,
    GOTO = 288,
    tHALT = 289,
    tLOCK = 290,
    tDC = 291,
    tDO = 292,
    tELSE = 293,
    tFOR = 294,
    tIF = 295,
    tLOOP = 296,
    tYIELD = 297,
    tRETURN = 298,
    tROLLBACK = 299,
    tSWITCH = 300,
    tWHILE = 301,
    tPRINT = 302,
    tSTATIC = 303,
    tALLOCATE = 304,
    tCOMPLY = 305,
    tREFUSE = 306,
    tAMUL = 307,
    tADIV = 308,
    tAMOD = 309,
    tAADD = 310,
    tASUB = 311,
    tALSHFT = 312,
    tARSHFT = 313,
    tAAND = 314,
    tAOR = 315,
    tAXOR = 316,
    tAND = 317,
    tOR = 318,
    tEQ = 319,
    tNE = 320,
    tGE = 321,
    tLE = 322,
    tLSHFT = 323,
    tRSHFT = 324,
    tPOW = 325,
    tSIZEOF = 326,
    tUPLUS = 327,
    tUMINUS = 328,
    tINC = 329,
    tDEC = 330,
    tUINC = 331,
    tUDEC = 332
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 32 "Parser.yy" /* yacc.c:1909  */

	const char* mString;
	mana::int_t mInt;
	mana::float_t mReal;
	mana::SyntaxNode* mNode;
	mana::TypeDescriptor* mTypeDescriptor;

#line 140 "Parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
