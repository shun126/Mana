/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"

/*
 * mana (compiler)
 *
 * @file	mana_parser.y
 * @brief	意味解析に関するソースファイル
 * @detail	このファイルは意味解析ノードに関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_CODE_H___)
#include "mana_code.h"
#endif
#if !defined(___MANA_DATA_H___)
#include "mana_data.h"
#endif
#if !defined(___MANA_JUMP_H___)
#include "mana_jump.h"
#endif
#if !defined(___MANA_LINKER_H___)
#include "mana_linker.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_NODE_H___)
#include "mana_node.h"
#endif
#if !defined(___MANA_REGISTER_H___)
#include "mana_register.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYERROR_VERBOSE

static mana_symbol_entry* mana_actor_symbol_entry_pointer;
static mana_symbol_entry* mana_function_symbol_entry_pointer;
static int mana_allocated_size;
static int mana_static_block_opend;
int yynerrs;


/* Line 371 of yacc.c  */
#line 120 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\\\mana_parser.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "mana_parser.h".  */
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
/* Line 387 of yacc.c  */
#line 53 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"

	int digit;
	float real;
	char* string;
	mana_node* node;
	mana_symbol_entry* symbol;
	mana_type_description* type;


/* Line 387 of yacc.c  */
#line 250 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\\\mana_parser.c"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 278 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\\\mana_parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1827

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  102
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  179
/* YYNRULES -- Number of states.  */
#define YYNSTATES  383

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    85,     2,     2,     2,    82,    75,     2,
      96,    97,    80,    78,   100,    79,   101,    81,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,    93,
      70,    62,    69,    63,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    98,     2,    99,    74,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    94,    73,    95,    86,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    65,    66,    67,
      68,    71,    72,    76,    77,    83,    84,    87,    88,    89,
      90,    91,    92
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,    10,    13,    16,    19,    25,
      30,    35,    41,    42,    51,    52,    59,    60,    64,    67,
      72,    75,    79,    83,    88,    93,    97,   101,   104,   106,
     109,   110,   117,   118,   125,   126,   133,   134,   135,   143,
     144,   147,   151,   154,   158,   162,   166,   167,   174,   175,
     178,   182,   183,   188,   189,   195,   197,   199,   201,   203,
     206,   210,   214,   215,   220,   221,   226,   231,   232,   241,
     242,   249,   250,   253,   255,   256,   261,   262,   271,   272,
     273,   281,   282,   291,   296,   297,   301,   302,   306,   310,
     313,   316,   320,   324,   327,   330,   335,   340,   345,   350,
     356,   366,   376,   384,   386,   389,   392,   394,   397,   400,
     404,   408,   412,   416,   420,   424,   428,   432,   436,   440,
     444,   448,   452,   456,   460,   464,   468,   472,   476,   480,
     484,   488,   492,   496,   500,   504,   508,   512,   516,   520,
     523,   526,   529,   532,   538,   540,   543,   546,   549,   552,
     557,   562,   564,   566,   568,   570,   572,   574,   576,   578,
     580,   582,   584,   588,   595,   600,   602,   606,   607,   614,
     615,   617,   618,   623,   624,   626,   630,   633,   634,   636
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     103,     0,    -1,   104,    -1,    -1,   104,   108,    93,    -1,
     104,   110,    -1,   104,   118,    -1,   104,   122,    -1,   104,
       8,     6,     5,    93,    -1,   104,    11,     5,    93,    -1,
     104,    12,     5,    93,    -1,   104,    13,   126,   124,    93,
      -1,    -1,   104,    48,    49,     3,   105,    94,   107,    95,
      -1,    -1,   104,    48,   106,    94,   107,    95,    -1,    -1,
     107,   108,    93,    -1,   126,   109,    -1,   126,   109,    62,
     146,    -1,   126,   124,    -1,     9,     6,     3,    -1,     9,
       6,     4,    -1,     9,     6,    79,     3,    -1,     9,     6,
      79,     4,    -1,     9,     6,     5,    -1,     9,     6,     6,
      -1,    10,     6,    -1,     6,    -1,     6,   127,    -1,    -1,
      15,     6,   111,    94,   116,    95,    -1,    -1,    17,     6,
     112,    94,   116,    95,    -1,    -1,    19,     6,   113,    94,
     116,    95,    -1,    -1,    -1,    18,     6,    36,   114,     6,
     115,   129,    -1,    -1,   116,   117,    -1,    18,     6,    93,
      -1,   108,    93,    -1,    11,     5,    93,    -1,    12,     5,
      93,    -1,    20,     6,    93,    -1,    -1,    14,   119,     6,
      94,   120,    95,    -1,    -1,   120,   121,    -1,   126,   109,
      93,    -1,    -1,   126,   124,   123,   129,    -1,    -1,     6,
     125,    96,   154,    97,    -1,    16,    -1,     6,    -1,     7,
      -1,   128,    -1,   128,   127,    -1,    98,     3,    99,    -1,
      98,     6,    99,    -1,    -1,    94,   130,   137,    95,    -1,
      -1,    39,    96,   132,   133,    -1,   146,    93,   146,    93,
      -1,    -1,   126,   109,    62,   146,    93,   134,   146,    93,
      -1,    -1,    40,    96,   146,    97,   136,   138,    -1,    -1,
     137,   138,    -1,   135,    -1,    -1,   135,    38,   139,   138,
      -1,    -1,    45,    96,   146,    97,   140,    94,   150,    95,
      -1,    -1,    -1,    46,   141,    96,   146,    97,   142,   138,
      -1,    -1,    37,   143,   138,    46,    96,   146,    97,    93,
      -1,   131,   146,    97,   138,    -1,    -1,    41,   144,   138,
      -1,    -1,    35,   145,   138,    -1,    33,     6,    93,    -1,
       6,    64,    -1,    43,    93,    -1,    43,   146,    93,    -1,
      44,   146,    93,    -1,    29,    93,    -1,    30,    93,    -1,
      34,    96,    97,    93,    -1,    42,    96,    97,    93,    -1,
      50,    96,    97,    93,    -1,    51,    96,    97,    93,    -1,
      47,    96,   156,    97,    93,    -1,    27,    96,   146,   100,
     146,    36,     6,    97,    93,    -1,    27,    96,   146,   100,
     146,   100,     5,    97,    93,    -1,    28,    96,   146,   100,
     146,    97,    93,    -1,   129,    -1,   108,    93,    -1,   146,
      93,    -1,    93,    -1,     1,    93,    -1,     1,    95,    -1,
     149,    62,   146,    -1,   146,    66,   146,    -1,   146,    65,
     146,    -1,   146,    56,   146,    -1,   146,    55,   146,    -1,
     146,    54,   146,    -1,   146,    53,   146,    -1,   146,    52,
     146,    -1,   146,    61,   146,    -1,   146,    60,   146,    -1,
     146,    59,   146,    -1,   146,    58,   146,    -1,   146,    57,
     146,    -1,   146,    78,   146,    -1,   146,    79,   146,    -1,
     146,    80,   146,    -1,   146,    81,   146,    -1,   146,    82,
     146,    -1,   146,    83,   146,    -1,   146,    75,   146,    -1,
     146,    73,   146,    -1,   146,    74,   146,    -1,   146,    77,
     146,    -1,   146,    76,   146,    -1,   146,    69,   146,    -1,
     146,    72,   146,    -1,   146,    70,   146,    -1,   146,    71,
     146,    -1,   146,    68,   146,    -1,   146,    67,   146,    -1,
      90,   146,    -1,    89,   146,    -1,   146,    90,    -1,   146,
      89,    -1,   146,    63,   146,    64,   146,    -1,   147,    -1,
      79,   146,    -1,    78,   146,    -1,    85,   146,    -1,    86,
     146,    -1,    84,    96,   126,    97,    -1,     6,    96,   156,
      97,    -1,   148,    -1,   149,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    -1,    25,    -1,    26,    -1,     3,    -1,
       4,    -1,     5,    -1,   149,   101,     6,    -1,   149,   101,
       6,    96,   156,    97,    -1,   149,    98,   146,    99,    -1,
       6,    -1,    96,   146,    97,    -1,    -1,    31,   146,    64,
     151,   137,   152,    -1,    -1,   150,    -1,    -1,    32,    64,
     153,   137,    -1,    -1,   155,    -1,   154,   100,   155,    -1,
     126,   109,    -1,    -1,   146,    -1,   156,   100,   146,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   118,   118,   121,   122,   123,   124,   125,   126,   128,
     130,   132,   135,   134,   150,   149,   155,   157,   160,   162,
     169,   171,   173,   175,   177,   179,   181,   208,   212,   214,
     219,   218,   230,   229,   241,   240,   252,   261,   251,   273,
     274,   277,   279,   281,   283,   285,   290,   289,   299,   300,
     303,   308,   307,   317,   316,   324,   326,   339,   342,   343,
     349,   351,   371,   370,   377,   376,   382,   390,   389,   402,
     401,   407,   408,   410,   413,   412,   420,   419,   438,   442,
     437,   449,   448,   459,   468,   467,   478,   477,   487,   492,
     497,   499,   501,   503,   505,   507,   509,   511,   513,   515,
     517,   519,   521,   523,   524,   525,   527,   528,   530,   533,
     535,   537,   539,   541,   543,   545,   547,   549,   551,   553,
     555,   557,   559,   561,   563,   565,   567,   569,   571,   573,
     575,   577,   579,   581,   583,   585,   587,   589,   591,   593,
     600,   607,   614,   621,   626,   629,   631,   633,   635,   637,
     652,   654,   655,   658,   660,   662,   667,   673,   678,   683,
     685,   687,   691,   693,   695,   697,   699,   704,   703,   707,
     708,   710,   709,   715,   716,   718,   721,   726,   727,   728
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tDIGIT", "tREAL", "tSTRING",
  "tIDENTIFIER", "tTYPE", "tALIAS", "tDEFINE", "tUNDEF", "tINCLUDE",
  "tIMPORT", "tNATIVE", "tSTRUCT", "tACTOR", "tACTOR2", "tPHANTOM",
  "tACTION", "tMODULE", "tEXTEND", "tFALSE", "tTRUE", "tPRIORITY", "tSELF",
  "tSENDER", "tNIL", "tREQUEST", "tJOIN", "tBREAK", "tCONTINUE", "tCASE",
  "tDEFAULT", "GOTO", "tHALT", "tLOCK", "tDC", "tDO", "tELSE", "tFOR",
  "tIF", "tLOOP", "tYIELD", "tRETURN", "tROLLBACK", "tSWITCH", "tWHILE",
  "tPRINT", "tSTATIC", "tALLOCATE", "tCOMPLY", "tREFUSE", "tAMOD", "tADIV",
  "tAMUL", "tASUB", "tAADD", "tARSHFT", "tALSHFT", "tAXOR", "tAOR",
  "tAAND", "'='", "'?'", "':'", "tOR", "tAND", "tNE", "tEQ", "'>'", "'<'",
  "tLE", "tGE", "'|'", "'^'", "'&'", "tRSHFT", "tLSHFT", "'+'", "'-'",
  "'*'", "'/'", "'%'", "tPOW", "tSIZEOF", "'!'", "'~'", "tUMINUS",
  "tUPLUS", "tDEC", "tINC", "tUDEC", "tUINC", "';'", "'{'", "'}'", "'('",
  "')'", "'['", "']'", "','", "'.'", "$accept", "program", "declarations",
  "$@1", "$@2", "allocate_declarations", "declaration", "declarator",
  "actor", "$@3", "$@4", "$@5", "$@6", "$@7", "actions", "action",
  "struct", "$@8", "struct_members", "struct_member", "function", "$@9",
  "function_header", "@10", "variable_type", "variable_sizes",
  "variable_size", "block", "$@11", "for_prefix", "$@12", "for_initialize",
  "$@13", "if_prefix", "@14", "statements", "statement", "@15", "@16",
  "$@17", "$@18", "@19", "@20", "$@21", "expression", "primary",
  "constant", "left_hand", "case_block", "$@22", "case", "$@23",
  "decl_args", "decl_arg", "call_args", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    61,    63,    58,   317,   318,   319,   320,    62,
      60,   321,   322,   124,    94,    38,   323,   324,    43,    45,
      42,    47,    37,   325,   326,    33,   126,   327,   328,   329,
     330,   331,   332,    59,   123,   125,    40,    41,    91,    93,
      44,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   102,   103,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   105,   104,   106,   104,   107,   107,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   109,   109,
     111,   110,   112,   110,   113,   110,   114,   115,   110,   116,
     116,   117,   117,   117,   117,   117,   119,   118,   120,   120,
     121,   123,   122,   125,   124,   126,   126,   126,   127,   127,
     128,   128,   130,   129,   132,   131,   133,   134,   133,   136,
     135,   137,   137,   138,   139,   138,   140,   138,   141,   142,
     138,   143,   138,   138,   144,   138,   145,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   147,   147,   147,   147,   147,
     147,   147,   147,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   149,   149,   149,   149,   149,   151,   150,   152,
     152,   153,   152,   154,   154,   154,   155,   156,   156,   156
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     3,     2,     2,     2,     5,     4,
       4,     5,     0,     8,     0,     6,     0,     3,     2,     4,
       2,     3,     3,     4,     4,     3,     3,     2,     1,     2,
       0,     6,     0,     6,     0,     6,     0,     0,     7,     0,
       2,     3,     2,     3,     3,     3,     0,     6,     0,     2,
       3,     0,     4,     0,     5,     1,     1,     1,     1,     2,
       3,     3,     0,     4,     0,     4,     4,     0,     8,     0,
       6,     0,     2,     1,     0,     4,     0,     8,     0,     0,
       7,     0,     8,     4,     0,     3,     0,     3,     3,     2,
       2,     3,     3,     2,     2,     4,     4,     4,     4,     5,
       9,     9,     7,     1,     2,     2,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     5,     1,     2,     2,     2,     2,     4,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     6,     4,     1,     3,     0,     6,     0,
       1,     0,     4,     0,     1,     3,     2,     0,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,    56,    57,     0,     0,     0,     0,
       0,     0,    46,     0,    55,     0,     0,     0,    14,     0,
       5,     6,     7,     0,     0,     0,    27,     0,     0,     0,
       0,    30,    32,     0,    34,     0,     0,     4,    28,    18,
      20,     0,    21,    22,    25,    26,     0,     9,    10,    53,
       0,     0,     0,     0,    36,     0,    12,    16,     0,     0,
      29,    58,     0,     0,     8,    23,    24,    11,    48,    39,
      39,     0,    39,     0,     0,     0,     0,   173,    59,   159,
     160,   161,   165,   153,   154,   155,   156,   157,   158,     0,
       0,     0,     0,     0,     0,     0,     0,    19,   144,   151,
     152,    62,    52,     0,     0,     0,    37,     0,    16,    15,
       0,     0,    60,    61,     0,     0,   174,   177,   146,   145,
       0,   147,   148,   140,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   142,   141,     0,     0,
       0,    71,    47,    49,     0,     0,     0,     0,     0,    31,
       0,    40,    33,     0,    35,     0,    17,    20,    28,   176,
      54,     0,   178,     0,     0,   166,   116,   115,   114,   113,
     112,   121,   120,   119,   118,   117,     0,   111,   110,   138,
     137,   133,   135,   136,   134,   129,   130,   128,   132,   131,
     122,   123,   124,   125,   126,   127,   109,     0,   162,     0,
       0,     0,     0,     0,     0,    42,    38,    13,   175,   150,
       0,   149,     0,   164,   177,     0,   165,     0,     0,     0,
       0,     0,     0,    86,    81,     0,     0,    84,     0,     0,
       0,     0,    78,     0,     0,     0,   106,    63,     0,   103,
       0,    73,    72,     0,    50,    43,    44,    41,    45,   179,
     143,     0,   107,   108,    89,     0,     0,    93,    94,     0,
       0,     0,     0,    64,     0,     0,     0,    90,     0,     0,
       0,     0,   177,     0,     0,   104,     0,    74,   105,   163,
       0,     0,    88,     0,    87,     0,     0,     0,    85,     0,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,   165,     0,    65,     0,    69,    96,    76,
       0,     0,    97,    98,    83,    75,     0,     0,     0,     0,
       0,     0,     0,    79,    99,     0,     0,     0,     0,     0,
       0,    70,     0,     0,     0,     0,   102,     0,     0,    66,
       0,     0,    80,     0,     0,    82,    67,     0,    77,   100,
     101,     0,   167,     0,    71,    68,     0,     0,   170,   168,
     171,    71,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    73,    36,    74,   258,    39,    20,    52,
      53,    55,    71,   173,   104,   171,    21,    30,   103,   163,
      22,    63,    40,    59,   111,    60,    61,   259,   161,   260,
     306,   325,   371,   261,   341,   219,   262,   318,   342,   291,
     353,   282,   285,   281,   263,    98,    99,   100,   361,   374,
     379,   381,   115,   116,   183
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -372
static const yytype_int16 yypact[] =
{
    -372,    13,   253,  -372,  -372,  -372,    33,    38,    43,    52,
      58,   157,  -372,    97,  -372,   108,   133,   135,    98,    50,
    -372,  -372,  -372,   159,   161,    55,  -372,    74,    78,   176,
     183,  -372,  -372,   160,  -372,   197,   107,  -372,   -72,   140,
     113,   115,  -372,  -372,  -372,  -372,    15,  -372,  -372,  -372,
     116,   118,   121,   123,  -372,   124,  -372,  -372,     2,   114,
    -372,   122,   672,   125,  -372,  -372,  -372,  -372,  -372,  -372,
    -372,   217,  -372,   130,   110,   127,   128,   157,  -372,  -372,
    -372,  -372,   129,  -372,  -372,  -372,  -372,  -372,  -372,   672,
     672,   137,   672,   672,   672,   672,   672,  1606,  -372,  -372,
      48,  -372,  -372,   102,     5,    36,  -372,    95,  -372,  -372,
     136,   159,  -372,  -372,   228,   -47,  -372,   672,   -83,   -83,
     157,   -83,   -83,  -372,  -372,   912,   672,   672,   672,   672,
     672,   672,   672,   672,   672,   672,   672,   672,   672,   672,
     672,   672,   672,   672,   672,   672,   672,   672,   672,   672,
     672,   672,   672,   672,   672,   672,  -372,  -372,   672,   672,
     229,  -372,  -372,  -372,   228,   231,   235,   239,   240,  -372,
     148,  -372,  -372,   125,  -372,   126,  -372,  -372,   122,  -372,
    -372,   157,  1606,    40,   150,  -372,  1645,  1645,  1645,  1681,
    1681,  1715,  1715,   300,   300,   300,  1528,   484,   484,   635,
     635,  1737,  1737,  1737,  1737,   213,   213,   665,    72,    72,
     103,   103,    39,    39,    39,    39,   394,   864,   152,   403,
     162,   163,   180,   181,   182,  -372,  -372,  -372,  -372,  -372,
     672,  -372,   672,  -372,   672,   -58,    34,   154,   156,   184,
     186,   247,   158,  -372,  -372,   185,   187,  -372,   188,   644,
     672,   189,  -372,   190,   201,   203,  -372,  -372,   207,  -372,
     672,   242,  -372,  1234,  -372,  -372,  -372,  -372,  -372,  1606,
     394,    94,  -372,  -372,  -372,   672,   672,  -372,  -372,   211,
     208,   595,   595,  -372,   672,   595,   209,  -372,  1276,  1318,
     672,   219,   672,   210,   221,  -372,   958,  -372,  -372,  -372,
     766,   815,  -372,   226,  -372,   236,   153,  1004,  -372,   227,
    -372,  -372,  1050,   672,   106,   232,   233,   595,   595,   672,
     672,  -372,   225,     3,   228,  -372,  1360,  -372,  -372,  -372,
    1096,   234,  -372,  -372,  -372,  -372,   717,  1142,   672,   260,
     672,   595,   249,  -372,  -372,   339,   350,   263,  1188,   672,
    1402,  -372,   331,   595,   267,   287,  -372,   294,  1444,  -372,
     672,   293,  -372,   301,   302,  -372,  -372,  1567,  -372,  -372,
    -372,   672,  -372,  1486,  -372,  -372,   307,   334,  -372,  -372,
    -372,  -372,   499
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -372,  -372,  -372,  -372,  -372,   291,    20,  -113,  -372,  -372,
    -372,  -372,  -372,  -372,   -34,  -372,  -372,  -372,  -372,  -372,
    -372,  -372,    12,  -372,    18,   344,  -372,   -61,  -372,  -372,
    -372,  -372,  -372,  -372,  -372,  -371,  -137,  -372,  -372,  -372,
    -372,  -372,  -372,  -372,   -62,  -372,  -372,  -372,    35,  -372,
    -372,  -372,  -372,   237,  -230
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -173
static const yytype_int16 yytable[] =
{
      97,   179,   102,   376,   271,    75,   156,   157,    76,   -56,
     382,     4,     5,     3,     7,     8,   165,   166,    65,    66,
      23,    14,    19,   167,   -53,   168,    58,   118,   119,    29,
     121,   122,   123,   124,   125,   272,   105,   273,   107,    24,
     -56,    50,     4,     5,    25,     7,     8,   165,   166,    26,
     180,   220,    14,   181,   167,   182,   168,    27,    42,    43,
      44,    45,   314,    28,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   110,   114,   216,   217,   274,   117,
     169,     4,     5,    31,     7,     8,   165,   166,     4,     5,
     158,    14,   226,   167,    32,   168,     4,     5,    14,     7,
       8,   164,   155,   177,   170,   170,    14,   170,   156,   157,
     117,   172,     4,     5,    46,     7,     8,   229,   184,    33,
     230,    34,    14,    37,   304,   305,   159,    35,   308,   160,
     150,   151,   152,   153,   154,   155,    79,    80,    81,   323,
       5,   156,   157,     4,     5,    38,    41,    47,   269,    14,
     270,    48,   182,    14,    83,    84,    85,    86,    87,    88,
     334,   335,    49,   152,   153,   154,   155,   288,   289,    51,
     174,   299,   156,   157,   230,   110,    54,   162,   296,   114,
      56,    57,    62,   331,   351,   109,   230,   -51,    64,    67,
      77,   339,    68,   300,   301,    69,   362,    70,    72,   101,
      58,   227,   307,   106,   108,   117,   112,   113,   312,   176,
     182,    89,    90,   120,   178,   218,   221,    91,    92,    93,
     222,   225,    94,    95,   326,   223,   224,   231,   234,    96,
     275,   330,   276,   279,   280,   264,   265,   336,   337,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,   266,   267,   268,   348,   277,   350,   278,
     297,   283,   322,   284,   286,   290,   292,   358,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   293,   367,   294,
     295,    18,   156,   157,   302,   303,   309,   315,   235,   373,
      79,    80,    81,   236,     5,   313,     7,     8,   316,   321,
     328,   338,   349,    14,   324,   332,   333,   344,    83,    84,
      85,    86,    87,    88,   237,   238,   239,   240,   360,   377,
     241,   242,   243,   352,   244,   354,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   355,   356,   254,   255,  -173,
    -173,  -173,   360,   136,   363,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   364,    89,    90,   365,   368,   156,
     157,    91,    92,    93,   369,   370,    94,    95,   380,   175,
     256,   101,  -169,    96,   235,    78,    79,    80,    81,   236,
       5,   378,     7,     8,     0,     0,     0,     0,   228,    14,
       0,     0,     0,     0,    83,    84,    85,    86,    87,    88,
     237,   238,   239,   240,     0,     0,   241,   242,   243,     0,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,     0,   254,   255,     0,     0,   136,     0,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
       0,    89,    90,   156,   157,     0,     0,    91,    92,    93,
       0,     0,    94,    95,     0,     0,   256,   101,   257,    96,
     235,     0,    79,    80,    81,   236,     5,     0,     7,     8,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
      83,    84,    85,    86,    87,    88,   237,   238,   239,   240,
       0,     0,   241,   242,   243,     0,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,     0,   254,
     255,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
       0,     0,     0,   156,   157,     0,     0,    89,    90,     0,
       0,     0,     0,    91,    92,    93,     0,     0,    94,    95,
       0,     0,   256,   101,  -172,    96,   235,     0,    79,    80,
      81,   236,     5,     0,     7,     8,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,    83,    84,    85,    86,
      87,    88,   237,   238,   239,   240,     0,     0,   241,   242,
     243,     0,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,     0,   254,   255,    79,    80,    81,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
      88,     0,     0,    89,    90,    79,    80,    81,    82,    91,
      92,    93,     0,     0,    94,    95,     0,     0,   256,   101,
       0,    96,     0,    83,    84,    85,    86,    87,    88,     0,
       0,     0,  -173,  -173,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     0,
       0,     0,    89,    90,   156,   157,     0,     0,    91,    92,
      93,     0,     0,    94,    95,     0,     0,   287,     0,     0,
      96,   148,   149,   150,   151,   152,   153,   154,   155,     0,
      89,    90,     0,   345,   156,   157,    91,    92,    93,     0,
       0,    94,    95,     0,     0,     0,     0,     0,    96,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,     0,
     136,     0,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,     0,     0,     0,     0,     0,   156,   157,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,     0,   136,
       0,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,     0,     0,   156,   157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,     0,   136,     0,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     0,
       0,     0,     0,     0,   156,   157,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,     0,   136,     0,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
       0,     0,     0,   156,   157,     0,     0,     0,     0,     0,
       0,     0,     0,   233,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,     0,   136,     0,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,     0,     0,
       0,   156,   157,     0,     0,     0,     0,     0,     0,   185,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       0,   136,     0,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,     0,     0,   156,   157,     0,
       0,     0,     0,     0,     0,   317,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,     0,   136,     0,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
       0,     0,     0,   156,   157,     0,     0,     0,     0,     0,
       0,   327,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,     0,   136,     0,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,     0,     0,     0,   156,
     157,     0,     0,     0,     0,     0,     0,   329,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,     0,   136,
       0,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,     0,     0,   156,   157,     0,     0,     0,
       0,     0,     0,   343,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,     0,   136,     0,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,     0,     0,
       0,   156,   157,     0,     0,     0,     0,     0,     0,   347,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       0,   136,     0,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,     0,     0,   156,   157,     0,
       0,     0,     0,     0,     0,   357,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,     0,   136,     0,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
       0,     0,     0,   156,   157,     0,     0,   298,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,     0,   136,
       0,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,     0,     0,   156,   157,     0,     0,   310,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       0,   136,     0,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,     0,     0,   156,   157,     0,
       0,   311,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,     0,   136,     0,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,     0,     0,     0,   156,
     157,     0,     0,   340,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,     0,   136,     0,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,     0,     0,
       0,   156,   157,     0,     0,   359,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,     0,   136,     0,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
       0,     0,     0,   156,   157,     0,     0,   366,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,     0,   136,
       0,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,     0,     0,   156,   157,     0,     0,   375,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       0,   136,   232,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,     0,     0,   156,   157,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,     0,
     136,   372,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,     0,     0,     0,     0,     0,   156,   157,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,     0,   136,
       0,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,     0,     0,   156,   157,  -173,  -173,  -173,
     129,   130,   131,   132,   133,   134,   135,     0,   136,     0,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     0,
       0,     0,     0,     0,   156,   157,  -173,  -173,   131,   132,
     133,   134,   135,     0,   136,     0,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,     0,     0,     0,     0,     0,
     156,   157,  -173,  -173,   133,   134,   135,     0,   136,     0,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     0,
       0,     0,     0,     0,   156,   157,  -173,  -173,  -173,  -173,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,     0,     0,     0,     0,     0,   156,   157
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-372)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-173)))

static const yytype_int16 yycheck[] =
{
      62,   114,    63,   374,   234,     3,    89,    90,     6,     6,
     381,     6,     7,     0,     9,    10,    11,    12,     3,     4,
       2,    16,     2,    18,    96,    20,    98,    89,    90,    11,
      92,    93,    94,    95,    96,    93,    70,    95,    72,     6,
       6,    29,     6,     7,     6,     9,    10,    11,    12,     6,
      97,   164,    16,   100,    18,   117,    20,     5,     3,     4,
       5,     6,   292,     5,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    74,    77,   158,   159,    64,    96,
      95,     6,     7,     6,     9,    10,    11,    12,     6,     7,
      62,    16,   173,    18,     6,    20,     6,     7,    16,     9,
      10,   103,    83,   111,   104,   105,    16,   107,    89,    90,
      96,    95,     6,     7,    79,     9,    10,    97,   120,     6,
     100,     6,    16,    93,   281,   282,    98,    49,   285,   101,
      78,    79,    80,    81,    82,    83,     3,     4,     5,     6,
       7,    89,    90,     6,     7,     6,     5,    93,   230,    16,
     232,    93,   234,    16,    21,    22,    23,    24,    25,    26,
     317,   318,     6,    80,    81,    82,    83,   249,   250,     6,
      95,    97,    89,    90,   100,   175,    36,    95,   260,   181,
       3,    94,    62,    97,   341,    95,   100,    94,    93,    93,
      96,   324,    94,   275,   276,    94,   353,    94,    94,    94,
      98,    95,   284,     6,    94,    96,    99,    99,   290,    93,
     292,    78,    79,    96,     6,     6,     5,    84,    85,    86,
       5,    93,    89,    90,   306,     6,     6,    97,    96,    96,
      96,   313,    96,     6,    96,    93,    93,   319,   320,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    93,    93,    93,   338,    93,   340,    93,
      38,    96,    46,    96,    96,    96,    96,   349,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    96,   360,    96,
      93,    48,    89,    90,    93,    97,    97,    97,     1,   371,
       3,     4,     5,     6,     7,    96,     9,    10,    97,    93,
      93,    96,    62,    16,   306,    93,    93,    93,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    94,    37,     6,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     5,    93,    50,    51,    59,
      60,    61,    31,    63,    97,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    97,    78,    79,    93,    95,    89,
      90,    84,    85,    86,    93,    93,    89,    90,    64,   108,
      93,    94,    95,    96,     1,    61,     3,     4,     5,     6,
       7,   376,     9,    10,    -1,    -1,    -1,    -1,   181,    16,
      -1,    -1,    -1,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    -1,    33,    34,    35,    -1,
      37,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    -1,    50,    51,    -1,    -1,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    78,    79,    89,    90,    -1,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    -1,    -1,    93,    94,    95,    96,
       1,    -1,     3,     4,     5,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    -1,    33,    34,    35,    -1,    37,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    50,
      51,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      -1,    -1,    93,    94,    95,    96,     1,    -1,     3,     4,
       5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    -1,    33,    34,
      35,    -1,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    -1,    50,    51,     3,     4,     5,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    23,    24,    25,
      26,    -1,    -1,    78,    79,     3,     4,     5,     6,    84,
      85,    86,    -1,    -1,    89,    90,    -1,    -1,    93,    94,
      -1,    96,    -1,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    78,    79,    89,    90,    -1,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    -1,    -1,    93,    -1,    -1,
      96,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      78,    79,    -1,    36,    89,    90,    84,    85,    86,    -1,
      -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    96,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    97,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    -1,    93,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    93,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    93,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    -1,    93,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    89,    90,    -1,    -1,    93,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    -1,    93,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    93,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    89,    90,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      89,    90,    57,    58,    59,    60,    61,    -1,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    89,    90,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   103,   104,     0,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    48,   108,
     110,   118,   122,   126,     6,     6,     6,     5,     5,   126,
     119,     6,     6,     6,     6,    49,   106,    93,     6,   109,
     124,     5,     3,     4,     5,     6,    79,    93,    93,     6,
     124,     6,   111,   112,    36,   113,     3,    94,    98,   125,
     127,   128,    62,   123,    93,     3,     4,    93,    94,    94,
      94,   114,    94,   105,   107,     3,     6,    96,   127,     3,
       4,     5,     6,    21,    22,    23,    24,    25,    26,    78,
      79,    84,    85,    86,    89,    90,    96,   146,   147,   148,
     149,    94,   129,   120,   116,   116,     6,   116,    94,    95,
     108,   126,    99,    99,   126,   154,   155,    96,   146,   146,
      96,   146,   146,   146,   146,   146,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    63,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    89,    90,    62,    98,
     101,   130,    95,   121,   126,    11,    12,    18,    20,    95,
     108,   117,    95,   115,    95,   107,    93,   124,     6,   109,
      97,   100,   146,   156,   126,    97,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,     6,   137,
     109,     5,     5,     6,     6,    93,   129,    95,   155,    97,
     100,    97,    64,    99,    96,     1,     6,    27,    28,    29,
      30,    33,    34,    35,    37,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    50,    51,    93,    95,   108,   129,
     131,   135,   138,   146,    93,    93,    93,    93,    93,   146,
     146,   156,    93,    95,    64,    96,    96,    93,    93,     6,
      96,   145,   143,    96,    96,   144,    96,    93,   146,   146,
      96,   141,    96,    96,    96,    93,   146,    38,    93,    97,
     146,   146,    93,    97,   138,   138,   132,   146,   138,    97,
      93,    93,   146,    96,   156,    97,    97,    97,   139,   100,
     100,    93,    46,     6,   126,   133,   146,    97,    93,    97,
     146,    97,    93,    93,   138,   138,   146,   146,    96,   109,
      93,   136,   140,    97,    93,    36,   100,    97,   146,    62,
     146,   138,    94,   142,     6,     5,    93,    97,   146,    93,
      31,   150,   138,    97,    97,    93,    93,   146,    95,    93,
      93,   134,    64,   146,   151,    93,   137,    32,   150,   152,
      64,   153,   137
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 8:
/* Line 1792 of yacc.c  */
#line 127 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_alias((yyvsp[(3) - (5)].string), (yyvsp[(4) - (5)].string)); }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 129 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { if(! mana_lexer_open((yyvsp[(3) - (4)].string), MANA_FALSE)){ YYABORT; } }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 131 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { if(! mana_lexer_open((yyvsp[(3) - (4)].string), MANA_TRUE)){ YYABORT; } }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 133 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_set_native_function((yyvsp[(4) - (5)].symbol), (yyvsp[(3) - (5)].type)); }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 135 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_static_block_opend = MANA_TRUE;
						mana_allocated_size = mana_symbol_get_static_memory_address() + (yyvsp[(4) - (4)].digit);
					}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 140 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						int address = mana_symbol_get_static_memory_address();
						if(address >= mana_allocated_size)
						{
							mana_compile_error("static variable range over");
						}
						mana_symbol_set_static_memory_address(mana_allocated_size);
						mana_static_block_opend = MANA_FALSE;
					}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 150 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_static_block_opend = MANA_TRUE; }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 152 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_static_block_opend = MANA_FALSE; }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 161 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_allocate_memory((yyvsp[(2) - (2)].symbol), (yyvsp[(1) - (2)].type), MANA_MEMORY_TYPE_NORMAL); }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 163 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						if((yyvsp[(2) - (4)].symbol)->class_type != MANA_CLASS_TYPE_VARIABLE_LOCAL)
							mana_compile_error("can initialize variable in local space only");
						mana_symbol_allocate_memory((yyvsp[(2) - (4)].symbol), (yyvsp[(1) - (4)].type), MANA_MEMORY_TYPE_NORMAL);
						mana_linker_expression(mana_node_create_node(MANA_NODE_TYPE_ASSIGN, mana_node_create_leaf((yyvsp[(2) - (4)].symbol)->name), (yyvsp[(4) - (4)].node)), MANA_TRUE);
					}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 170 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_prototype((yyvsp[(2) - (2)].symbol), (yyvsp[(1) - (2)].type)); }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 172 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_const_int((yyvsp[(2) - (3)].string), (yyvsp[(3) - (3)].digit)); }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 174 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_const_float((yyvsp[(2) - (3)].string), (yyvsp[(3) - (3)].real)); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 176 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_const_int((yyvsp[(2) - (4)].string), - (yyvsp[(4) - (4)].digit)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 178 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_const_float((yyvsp[(2) - (4)].string), - (yyvsp[(4) - (4)].real)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 180 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_const_string((yyvsp[(2) - (3)].string), (yyvsp[(3) - (3)].string)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 182 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_entry* symbol = mana_symbol_lookup((yyvsp[(3) - (3)].string));
						if(symbol)
						{
							switch(symbol->class_type)
							{
							case MANA_CLASS_TYPE_CONSTANT_INT:
								mana_symbol_create_const_int((yyvsp[(2) - (3)].string), symbol->address);
								break;
							case MANA_CLASS_TYPE_CONSTANT_FLOAT:
								mana_symbol_create_const_float((yyvsp[(2) - (3)].string), symbol->hloat);
								break;
							case MANA_CLASS_TYPE_CONSTANT_STRING:
								mana_symbol_create_const_string((yyvsp[(2) - (3)].string), symbol->string);
								break;
							case MANA_CLASS_TYPE_ALIAS:
								mana_symbol_create_alias((yyvsp[(2) - (3)].string), symbol->string);
								break;
							default:
								mana_compile_error("invalid identifier used as a define name");
								break;
							}
						}else{
							mana_compile_error("identifier %s is not defined", (yyvsp[(3) - (3)].string));
						}
					}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 209 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_destroy((yyvsp[(2) - (2)].string)); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 213 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.symbol) = mana_symbol_create_identification((yyvsp[(1) - (1)].string), NULL, mana_static_block_opend); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 215 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.symbol) = mana_symbol_create_identification((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].type), mana_static_block_opend); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 219 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_actor_symbol_entry_pointer = mana_symbol_lookup((yyvsp[(2) - (2)].string));
						mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
					}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 224 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_set_type((yyvsp[(2) - (6)].string), mana_symbol_close_actor((yyvsp[(2) - (6)].string), NULL, NULL, MANA_FALSE));
						mana_actor_symbol_entry_pointer = NULL;
					}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 230 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_actor_symbol_entry_pointer = mana_symbol_lookup((yyvsp[(2) - (2)].string));
						mana_symbol_open_actor(mana_actor_symbol_entry_pointer);
					}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 235 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_set_type((yyvsp[(2) - (6)].string), mana_symbol_close_actor((yyvsp[(2) - (6)].string), NULL, NULL, MANA_TRUE));
						mana_actor_symbol_entry_pointer = NULL;
					}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 241 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_open_module();
					}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 245 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_set_type((yyvsp[(2) - (6)].string), mana_symbol_close_module((yyvsp[(2) - (6)].string)));
					}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 252 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_entry* symbol;

						symbol = mana_symbol_lookup((yyvsp[(2) - (3)].string));

						mana_symbol_open_actor2(symbol);
						mana_actor_symbol_entry_pointer = symbol;
					}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 261 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_function_symbol_entry_pointer = mana_symbol_create_function((yyvsp[(5) - (5)].string));
						mana_symbol_open_function(MANA_TRUE, mana_function_symbol_entry_pointer, mana_type_get(MANA_DATA_TYPE_VOID));
					}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 266 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_close_function(mana_function_symbol_entry_pointer);
						mana_symbol_close_actor2();
						mana_actor_symbol_entry_pointer = NULL;
					}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 278 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_create_prototype(mana_symbol_create_function((yyvsp[(2) - (3)].string)), mana_type_get(MANA_DATA_TYPE_VOID)); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 282 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { if(! mana_lexer_open((yyvsp[(2) - (3)].string), MANA_FALSE)){ YYABORT; } }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 284 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { if(! mana_lexer_open((yyvsp[(2) - (3)].string), MANA_TRUE)){ YYABORT; } }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 286 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_extend_module((yyvsp[(2) - (3)].string)); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 290 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_open_structure();
					}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 294 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_set_type((yyvsp[(3) - (6)].string), mana_symbol_close_structure((yyvsp[(3) - (6)].string)));
					}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 304 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_allocate_memory((yyvsp[(2) - (3)].symbol), (yyvsp[(1) - (3)].type), MANA_MEMORY_TYPE_NORMAL); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 308 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_open_function(MANA_FALSE, (yyvsp[(2) - (2)].symbol), (yyvsp[(1) - (2)].type));
						mana_function_symbol_entry_pointer = (yyvsp[(2) - (2)].symbol);
					}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 313 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_close_function((yyvsp[(2) - (4)].symbol)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 317 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.symbol) = mana_symbol_create_function((yyvsp[(1) - (1)].string)); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 319 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						(yyval.symbol) = (yyvsp[(2) - (5)].symbol);
						(yyval.symbol)->number_of_parameters = (yyvsp[(4) - (5)].digit);
					}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 325 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.type) = mana_type_get(MANA_DATA_TYPE_ACTOR); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 327 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_entry* symbol = mana_symbol_lookup((yyvsp[(1) - (1)].string));
						if(symbol)
						{
							(yyval.type) = symbol->type;
						}
						else
						{
							mana_compile_error("incomplete type name %s", (yyvsp[(1) - (1)].string));
							(yyval.type) = mana_type_get(MANA_DATA_TYPE_INT);
						}
					}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 344 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						assert((yyvsp[(1) - (2)].type)->component != (yyvsp[(2) - (2)].type));
						(yyvsp[(1) - (2)].type)->component = (yyvsp[(2) - (2)].type);
					}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 350 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.type) = mana_type_create_array((yyvsp[(2) - (3)].digit)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 352 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_entry* symbol = mana_symbol_lookup((yyvsp[(2) - (3)].string));
						if(symbol)
						{
							if(symbol->class_type == MANA_CLASS_TYPE_CONSTANT_INT)
							{
								(yyval.type) = mana_type_create_array(symbol->address);
							}else{
								mana_compile_error("invalid size information on parameter");
								(yyval.type) = 0;
							}
						}else{
							mana_compile_error("identifier %s is not defined", (yyvsp[(2) - (3)].string));
							(yyval.type) = 0;
						}
					}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 371 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_open_block(MANA_FALSE); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 373 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_close_block(); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 377 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_open_block(MANA_FALSE); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 379 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.digit) = (yyvsp[(4) - (4)].digit); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 383 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {	/* 'for(variable = expression' の形式 */
						mana_linker_expression((yyvsp[(1) - (4)].node), MANA_TRUE);
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
						(yyval.digit) = mana_code_get_pc();
						mana_jump_break(mana_linker_condition((yyvsp[(3) - (4)].node), MANA_TRUE));
					}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 390 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {	/* 'for(type variable = expression' の形式 */
						mana_symbol_allocate_memory((yyvsp[(2) - (5)].symbol), (yyvsp[(1) - (5)].type), MANA_MEMORY_TYPE_NORMAL);
						mana_linker_expression(mana_node_create_node(MANA_NODE_TYPE_ASSIGN, mana_node_create_leaf((yyvsp[(2) - (5)].symbol)->name), (yyvsp[(4) - (5)].node)), MANA_TRUE);
					}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 395 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_FOR);
						(yyval.digit) = mana_code_get_pc();
						mana_jump_break(mana_linker_condition((yyvsp[(7) - (8)].node), MANA_TRUE));
					}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 402 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.digit) = mana_linker_condition((yyvsp[(3) - (4)].node), MANA_TRUE); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 404 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.digit) = (yyvsp[(5) - (6)].digit); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 411 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_replace_all((yyvsp[(1) - (1)].digit), mana_code_get_pc()); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 413 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						(yyval.digit) = mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
						mana_code_replace_all((yyvsp[(1) - (2)].digit), mana_code_get_pc());
					}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 418 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_replace_all((yyvsp[(3) - (4)].digit), mana_code_get_pc()); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 420 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_type_description* type = (yyvsp[(3) - (4)].node)->type;

						mana_linker_expression((yyvsp[(3) - (4)].node), MANA_FALSE);
						(yyval.digit) = mana_code_get_pc();
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_SWITCH);
						mana_code_set_opecode_and_operand(MANA_IL_BRA, -1);
						mana_jump_open_switch(type);
					}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 430 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc()));
						mana_code_replace_all((yyvsp[(5) - (8)].digit), mana_code_get_pc());
						mana_jump_switch_build();
						mana_jump_close_chain();
						mana_jump_close_switch();
					}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 438 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_WHILE);
					}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 442 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_jump_break(mana_linker_condition((yyvsp[(4) - (5)].node), MANA_TRUE)); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 444 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
						mana_jump_close_chain();
					}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 449 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_DO);
						(yyval.digit) = mana_code_get_pc();
					}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 454 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_close_continue_only();
						mana_code_replace_all(mana_linker_condition((yyvsp[(6) - (8)].node), MANA_FALSE), (yyvsp[(2) - (8)].digit));
						mana_jump_close_chain();
					}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 460 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_close_continue_only();
						mana_linker_expression((yyvsp[(2) - (4)].node), MANA_TRUE);
						mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc()));
						mana_jump_close_chain();
						mana_symbol_close_block();
					}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 468 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOOP);
						(yyval.digit) = mana_code_get_pc();
					}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 473 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_code_set_opecode_and_operand(MANA_IL_BRA, (yyvsp[(2) - (3)].digit));
						mana_jump_close_chain();
					}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 478 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_open_chain(MANA_JUMP_CHAIN_STATE_LOCK);
						mana_code_set_opecode(MANA_IL_NONPREEMPTIVE);
					}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 483 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_jump_close_chain();
						mana_code_set_opecode(MANA_IL_PREEMPTIVE);
					}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 488 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_entry* symbol = mana_symbol_create_label((yyvsp[(2) - (3)].string));
						symbol->etc = mana_code_set_opecode_and_operand(MANA_IL_BRA, symbol->etc);
					}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 493 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_symbol_entry* symbol = mana_symbol_create_label((yyvsp[(1) - (2)].string));
						symbol->address = mana_code_get_pc();
					}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 498 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_linker_return(mana_function_symbol_entry_pointer, NULL); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 500 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_linker_return(mana_function_symbol_entry_pointer, (yyvsp[(2) - (3)].node)); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 502 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_linker_rollback((yyvsp[(2) - (3)].node)); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 504 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_break(mana_code_get_pc())); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 506 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_set_opecode_and_operand(MANA_IL_BRA, mana_jump_continue(mana_code_get_pc())); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 508 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_set_opecode(MANA_IL_HALT); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 510 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_set_opecode(MANA_IL_YIELD); }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 512 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_set_opecode(MANA_IL_COMPLY); }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 514 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_code_set_opecode(MANA_IL_REFUSE); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 516 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_linker_call_print((yyvsp[(3) - (5)].node)); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 518 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_add_request(MANA_IL_REQ, (yyvsp[(3) - (9)].node), (yyvsp[(5) - (9)].node), (yyvsp[(7) - (9)].string)); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 520 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_add_request(MANA_IL_REQ, (yyvsp[(3) - (9)].node), (yyvsp[(5) - (9)].node), (yyvsp[(7) - (9)].string)); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 522 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_add_join((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 526 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_linker_expression((yyvsp[(1) - (2)].node), MANA_TRUE); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 529 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { yyerrok; yyclearin; }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 531 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { yyerrok; yyclearin; }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 534 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 536 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_LAND, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 538 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_LOR, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 540 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_ADD, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 542 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_SUB, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 544 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_MUL, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 546 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_DIV, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 548 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_REM, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 550 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_AND, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 552 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_OR, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 554 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_XOR, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 556 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_LSH, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 558 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (3)].node), mana_node_create_node(MANA_NODE_TYPE_RSH, mana_node_clone((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node))); }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 560 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ADD, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 562 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_SUB, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 564 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_MUL, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 566 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_DIV, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 568 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_REM, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 570 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_POW, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 572 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_AND, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 574 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_OR, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 576 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_XOR, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 578 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_LSH, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 580 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_RSH, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 582 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_GT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 584 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_GE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 586 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_LS, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 588 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_LE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 590 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_EQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 592 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_NE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 594 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						(yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(2) - (2)].node), mana_node_create_node(MANA_NODE_TYPE_ADD, mana_node_clone((yyvsp[(2) - (2)].node)), node));
					}
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 601 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						(yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(2) - (2)].node), mana_node_create_node(MANA_NODE_TYPE_SUB, mana_node_clone((yyvsp[(2) - (2)].node)), node));
					}
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 608 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						(yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (2)].node), mana_node_create_node(MANA_NODE_TYPE_ADD, mana_node_clone((yyvsp[(1) - (2)].node)), node));
					}
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 615 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						mana_node* node = mana_node_allocate(MANA_NODE_TYPE_CONST);
						node->digit = 1;
						node->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						(yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ASSIGN, (yyvsp[(1) - (2)].node), mana_node_create_node(MANA_NODE_TYPE_SUB, mana_node_clone((yyvsp[(1) - (2)].node)), node));
					}
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 622 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						(yyval.node) = mana_node_create_node(MANA_NODE_TYPE_EXPRESSION_IF, (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node));
						(yyval.node)->condition = (yyvsp[(1) - (5)].node);
					}
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 630 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_NEG, (yyvsp[(2) - (2)].node), NULL); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 632 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = (yyvsp[(2) - (2)].node); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 634 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_LNOT, (yyvsp[(2) - (2)].node), NULL); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 636 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_NOT, (yyvsp[(2) - (2)].node), NULL); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 638 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						int max_char = (1 << (8 * CBSZ - 1)) - 1;
						int min_char = -1 << (8 * CBSZ - 1);
						int max_short = (1 << (8 * SBSZ - 1)) - 1;
						int min_short = -1 << (8 * SBSZ - 1);
						(yyval.node) = mana_node_allocate(MANA_NODE_TYPE_CONST);
						(yyval.node)->digit = (yyvsp[(3) - (4)].type)->memory_size;
						if((yyval.node)->digit <= max_char && (yyval.node)->digit >= min_char)
							(yyval.node)->type = mana_type_get(MANA_DATA_TYPE_CHAR);
						else if((yyval.node)->digit <= max_short && (yyval.node)->digit >= min_short)
							(yyval.node)->type = mana_type_get(MANA_DATA_TYPE_SHORT);
						else
							(yyval.node)->type = mana_type_get(MANA_DATA_TYPE_INT);
					}
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 653 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_CALL, mana_node_create_function((yyvsp[(1) - (4)].string)), (yyvsp[(3) - (4)].node)); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 659 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_digit(0); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 661 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_digit(1); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 663 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						(yyval.node) = mana_node_allocate(MANA_NODE_TYPE_PRIORITY);
						(yyval.node)->type = mana_type_get(MANA_DATA_TYPE_INT);
					}
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 668 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						(yyval.node) = mana_node_allocate(MANA_NODE_TYPE_SELF);
						(yyval.node)->etc = 0;
						(yyval.node)->type = mana_type_get(MANA_DATA_TYPE_ACTOR);
					}
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 674 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						(yyval.node) = mana_node_allocate(MANA_NODE_TYPE_SENDER);
						(yyval.node)->type = mana_type_get(MANA_DATA_TYPE_ACTOR);
					}
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 679 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    {
						(yyval.node) = mana_node_allocate(MANA_NODE_TYPE_CONST);
						(yyval.node)->type = mana_type_get(MANA_DATA_TYPE_NIL);
					}
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 684 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_digit((yyvsp[(1) - (1)].digit)); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 686 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_real((yyvsp[(1) - (1)].real)); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 688 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_string((yyvsp[(1) - (1)].string)); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 692 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_member((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].string)); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 694 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_call_member((yyvsp[(1) - (6)].node), (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].node)); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 696 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ARRAY, (yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 698 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_leaf((yyvsp[(1) - (1)].string)); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 700 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 704 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_jump_switch_case((yyvsp[(2) - (3)].node)); }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 710 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_jump_switch_default(); }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 715 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.digit) = 0; }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 717 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.digit) = 1; }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 719 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.digit) = (yyvsp[(1) - (3)].digit) + 1; }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 722 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { mana_symbol_allocate_memory((yyvsp[(2) - (2)].symbol), (yyvsp[(1) - (2)].type), MANA_MEMORY_TYPE_PARAMETER); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 726 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = 0; }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 729 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"
    { (yyval.node) = mana_node_create_node(MANA_NODE_TYPE_ARGUMENT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;


/* Line 1792 of yacc.c  */
#line 3235 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\\\mana_parser.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 731 "C:\\Users\\smoriya\\Documents\\Visual Studio 2015\\Projects\\Application\\Mana\\compiler\\mana_parser.y"


/*!
 * @brief	mana_print error message
 * @param	message		error message
 */
void yyerror(char* message)
{
	yynerrs ++;
	mana_error(mana_lexer_get_current_filename(), mana_lexer_get_current_line(), message);
}

/*!
 * initialize parser
 */
void mana_parser_initialize(void)
{
	mana_static_block_opend = MANA_FALSE;

	/* レジスタ割り当て処理を初期化 */
	mana_register_initialzie();

	{
		/* vec2 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec2", mana_symbol_close_structure("vec2"));

		/* vec3 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec3", mana_symbol_close_structure("vec3"));

		/* vec4 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("w", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("vec4", mana_symbol_close_structure("vec4"));

		/* color */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("r", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("g", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("b", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("a", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_set_type("color", mana_symbol_close_structure("color"));
	}

	{
		mana_symbol_entry* symbol;

		/* int getUerData() */
		symbol = mana_symbol_create_function("getUserData");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setUserData(int data) */
		symbol = mana_symbol_create_function("setUserData");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("data", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));
	
		/* pointer getUserPointer() */
		symbol = mana_symbol_create_function("getUserPointer");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_pointer);

		/* void setUserPointer(pointer address) */
		symbol = mana_symbol_create_function("setUserPointer");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("address", NULL, MANA_FALSE), mana_type_pointer, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int debug() */
		symbol = mana_symbol_create_function("debug");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setTickCount(int count) */
		symbol = mana_symbol_create_function("setTickCount");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void wait(float) */
		symbol = mana_symbol_create_function("wait");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("second", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void waitFrame(int) */
		symbol = mana_symbol_create_function("waitFrame");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("frame", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void srand(int count) */
		symbol = mana_symbol_create_function("srand");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int rand(int, int) */
		symbol = mana_symbol_create_function("rand");
		symbol->number_of_parameters = 2;
		mana_symbol_allocate_memory(mana_symbol_create_identification("min", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("max", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* float frand() */
		symbol = mana_symbol_create_function("frand");
		symbol->number_of_parameters = 0;
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sin(float) */
		symbol = mana_symbol_create_function("sin");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float cos(float) */
		symbol = mana_symbol_create_function("cos");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float atan2(float, float) */
		symbol = mana_symbol_create_function("atan2");
		symbol->number_of_parameters = 2;
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float tan(float) */
		symbol = mana_symbol_create_function("tan");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float angleMod(float) */
		symbol = mana_symbol_create_function("angleMod");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sqrt(float) */
		symbol = mana_symbol_create_function("sqrt");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, MANA_FALSE), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* actor getActor(string) */
		symbol = mana_symbol_create_function("getActor");
		symbol->number_of_parameters = 1;
		mana_symbol_allocate_memory(mana_symbol_create_identification("name", NULL, MANA_FALSE), mana_type_string, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_set_native_function(symbol, mana_type_get(MANA_DATA_TYPE_ACTOR));
	}
}

/*!
 * shutdown parser
 */
void mana_parser_finalize(void)
{
	/* レジスタ割り当て処理を終了 */
	mana_register_finalize();
}

mana_symbol_entry* mana_parser_get_actor_symbol_entry_pointer()
{
	return mana_actor_symbol_entry_pointer;
}
