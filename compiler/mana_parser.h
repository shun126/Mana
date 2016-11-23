/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_C_USERS_SMORIYA_DOCUMENTS_VISUAL_STUDIO_2015_PROJECTS_APPLICATION_MANA_COMPILER_MANA_PARSER_H_INCLUDED
# define YY_YY_C_USERS_SMORIYA_DOCUMENTS_VISUAL_STUDIO_2015_PROJECTS_APPLICATION_MANA_COMPILER_MANA_PARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
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
     tAMOD = 307,
     tADIV = 308,
     tAMUL = 309,
     tASUB = 310,
     tAADD = 311,
     tARSHFT = 312,
     tALSHFT = 313,
     tAXOR = 314,
     tAOR = 315,
     tAAND = 316,
     tOR = 317,
     tAND = 318,
     tNE = 319,
     tEQ = 320,
     tLE = 321,
     tGE = 322,
     tRSHFT = 323,
     tLSHFT = 324,
     tPOW = 325,
     tSIZEOF = 326,
     tUMINUS = 327,
     tUPLUS = 328,
     tDEC = 329,
     tINC = 330,
     tUDEC = 331,
     tUINC = 332
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 53 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"

	int digit;
	float real;
	char* string;
	mana_node* node;
	mana_symbol_entry* symbol;
	mana_type_description* type;


/* Line 2058 of yacc.c  */
#line 144 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\\\mana_parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_C_USERS_SMORIYA_DOCUMENTS_VISUAL_STUDIO_2015_PROJECTS_APPLICATION_MANA_COMPILER_MANA_PARSER_H_INCLUDED  */
