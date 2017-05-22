/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "Parser.yy" /* yacc.c:339  */

/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Main.h"
#include "SymbolEntry.h"
#include "TypeDescriptor.h"
//#include "generator.h"
#include "Lexer.h"
//#include "linker.h"
//#include "main.h"
//#include "pre_resolver.h"

#include "ParserDeclaration.inl"
#include <memory>

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
#define YYERROR_VERBOSE
#endif

#line 94 "Parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
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
   by #include "Parser.hpp".  */
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
#line 32 "Parser.yy" /* yacc.c:355  */

	const char* mString;
	mana::int_t mInt;
	mana::float_t mReal;
	mana::SyntaxNode* mNode;
	mana::TypeDescriptor* mTypeDescriptor;

#line 220 "Parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 237 "Parser.cpp" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

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
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  38
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1744

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  102
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  149
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  351

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    85,     2,     2,     2,    82,    75,     2,
      94,    95,    80,    78,    98,    79,    99,    81,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,    93,
      72,    62,    69,    63,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   100,     2,   101,    74,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    96,    73,    97,    86,     2,     2,     2,
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
      68,    70,    71,    76,    77,    83,    84,    87,    88,    89,
      90,    91,    92
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    91,    91,   107,   108,   112,   113,   114,   115,   117,
     120,   122,   124,   127,   130,   132,   138,   139,   143,   144,
     146,   148,   150,   152,   155,   158,   163,   165,   167,   172,
     173,   177,   179,   181,   184,   189,   190,   194,   197,   201,
     203,   205,   209,   214,   215,   218,   220,   222,   224,   226,
     228,   230,   232,   234,   236,   238,   240,   242,   244,   246,
     248,   250,   252,   254,   256,   258,   260,   262,   264,   265,
     266,   267,   269,   271,   275,   277,   279,   281,   283,   285,
     287,   289,   291,   293,   295,   297,   299,   301,   303,   305,
     307,   309,   311,   313,   315,   317,   319,   321,   323,   325,
     327,   329,   331,   333,   335,   337,   339,   341,   343,   345,
     348,   350,   352,   354,   356,   358,   360,   361,   364,   366,
     368,   370,   372,   374,   376,   378,   380,   384,   386,   388,
     390,   392,   396,   397,   400,   402,   407,   408,   410,   415,
     416,   418,   423,   425,   430,   432,   436,   437,   440,   442
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
  "tPRINT", "tSTATIC", "tALLOCATE", "tCOMPLY", "tREFUSE", "tAMUL", "tADIV",
  "tAMOD", "tAADD", "tASUB", "tALSHFT", "tARSHFT", "tAAND", "tAOR",
  "tAXOR", "'='", "'?'", "':'", "tAND", "tOR", "tEQ", "tNE", "'>'", "tGE",
  "tLE", "'<'", "'|'", "'^'", "'&'", "tLSHFT", "tRSHFT", "'+'", "'-'",
  "'*'", "'/'", "'%'", "tPOW", "tSIZEOF", "'!'", "'~'", "tUPLUS",
  "tUMINUS", "tINC", "tDEC", "tUINC", "tUDEC", "';'", "'('", "')'", "'{'",
  "'}'", "','", "'.'", "'['", "']'", "$accept", "program", "line",
  "declarations", "allocate_declarations", "declaration", "actor",
  "actions", "action", "struct", "struct_members", "struct_member",
  "function", "variable_type", "block", "statements", "statement",
  "expression", "primary", "constant", "left_hand", "cases", "case",
  "arg_calls", "arg_decls", "variable_decl", "declarator",
  "variable_sizes", "variable_size", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    61,    63,    58,   317,   318,   319,   320,    62,
     321,   322,    60,   124,    94,    38,   323,   324,    43,    45,
      42,    47,    37,   325,   326,    33,   126,   327,   328,   329,
     330,   331,   332,    59,    40,    41,   123,   125,    44,    46,
      91,    93
};
# endif

#define YYPACT_NINF -235

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-235)))

#define YYTABLE_NINF -44

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-44)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     535,  -235,  -235,    -1,    10,    14,     2,    19,    11,    22,
      44,  -235,    48,    65,   -45,    57,   107,  -235,   535,    21,
    -235,  -235,  -235,   112,  -235,   122,    40,  -235,    38,    45,
     130,    60,    69,    70,    71,   147,    11,    86,  -235,  -235,
    -235,   -33,    93,    90,  -235,  -235,  -235,  -235,    29,  -235,
    -235,    94,    11,    46,    46,    46,    96,    67,   183,   100,
      11,    11,   110,  -235,   104,   514,  -235,  -235,  -235,    11,
      99,    11,   115,   191,   205,   119,   116,    46,   121,   123,
      11,  -235,   104,    11,   127,    54,  -235,   113,   118,  -235,
    -235,  -235,  -235,   131,  -235,  -235,  -235,  -235,  -235,  -235,
     514,   514,   134,   514,   514,   514,   514,   514,  1459,  -235,
    -235,   -51,    59,  -235,  -235,  -235,   133,   124,  -235,  -235,
    -235,  -235,  -235,   135,  -235,  -235,   133,    11,  -235,  -235,
     514,   -31,   -31,    11,   -31,   -31,  -235,  -235,   777,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,  -235,
    -235,   514,   224,   514,   140,   340,  -235,  -235,  -235,  -235,
    -235,   636,   139,   142,  -235,  1498,  1498,  1498,  1534,  1534,
    1568,  1568,   236,   236,   236,  1381,  1592,  1592,  1616,  1616,
    1638,  1638,  1638,  1638,  1654,  1654,    97,    43,    43,   120,
     120,   -20,   -20,   -20,   -20,   331,   148,   597,  -235,   -85,
      51,   149,   151,   158,   161,   235,   162,   437,   437,   163,
     164,   437,   168,   486,   514,   185,   187,   197,   198,   204,
    -235,   207,  -235,   223,   243,  1129,   514,  -235,  -235,   514,
     514,  -235,  -235,  -235,  -235,   514,   514,  -235,  -235,   230,
     160,  -235,   278,   137,   514,  -235,   239,  -235,  1171,  1213,
     514,   514,   514,   240,   247,  -235,  -235,  -235,  -235,  -235,
     331,   253,   683,   730,  -235,   237,   244,    16,  1255,   258,
     821,   259,  -235,  -235,   865,   909,   260,   261,   264,  -235,
     514,   514,  -235,   514,   514,   514,   437,  -235,   257,   437,
     265,  -235,  -235,   558,   953,   997,  1297,  1339,   321,    41,
    -235,  -235,   354,   279,   283,   514,   514,   437,   514,   307,
     -29,  -235,   293,  -235,  -235,  1041,  1085,  -235,  1420,   243,
    -235,  -235,   285,   437,   437,   243,  -235,  -235,  -235,  -235,
    -235
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    40,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     2,     3,     0,
       5,     6,     7,     0,    18,     0,     0,    25,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,     1,     4,
       8,   144,   142,     0,    19,    20,    23,    24,     0,    14,
      15,     0,    35,    29,    29,    29,     0,     0,     0,     0,
      16,   139,     0,   145,   146,     0,    13,    21,    22,   139,
       0,    35,     0,     0,     0,     0,     0,    29,     0,     0,
      16,    11,   144,    16,     0,     0,   140,     0,     0,   147,
     124,   125,   126,   130,   118,   119,   120,   121,   122,   123,
       0,     0,     0,     0,     0,     0,     0,     0,   143,   109,
     116,   117,     0,    34,    36,    37,     0,     0,    33,    26,
      30,    27,    28,     0,    17,    10,     0,     0,   148,   149,
     136,   111,   110,     0,   112,   113,   104,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
     107,     0,     0,     0,     0,     0,    31,    32,    12,    38,
     141,   137,     0,     0,   131,    79,    80,    81,    77,    78,
      85,    86,    82,    83,    84,     0,    75,    76,   102,   103,
      98,    99,   101,   100,    94,    95,    93,    96,    97,    87,
      88,    89,    90,    91,    92,    74,   127,     0,     9,     0,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    68,     0,     0,     0,   136,   115,   114,     0,
     136,   129,    72,    73,    55,     0,     0,    59,    60,     0,
       0,    53,     0,     0,     0,    52,     0,    56,     0,     0,
       0,     0,   136,     0,     0,    69,    42,    44,    70,   138,
     108,     0,     0,     0,    54,     0,     0,   130,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,     0,   128,
       0,     0,    61,     0,     0,     0,     0,    62,     0,     0,
       0,    63,    64,     0,     0,     0,     0,     0,    45,     0,
      48,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,     0,    67,    49,     0,     0,    46,     0,     0,
      47,   133,     0,     0,     0,     0,   135,    66,    50,    51,
     134
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -235,  -235,   371,  -235,   -54,   117,  -235,   -40,  -235,  -235,
     322,  -235,  -235,     1,  -103,  -234,  -197,   -65,  -235,  -235,
    -235,  -235,    62,  -225,   326,    68,  -235,   351,  -235
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    16,    17,    18,    57,   241,    20,    76,    77,    21,
      70,    71,    22,    58,   242,   243,   244,   245,   109,   110,
     111,   330,   331,   182,    85,    24,    42,    63,    64
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,    23,   328,   329,    35,    25,    84,    28,   252,    30,
     277,   171,   253,   176,    78,    79,    26,     1,     2,    23,
      27,   279,   -40,   179,    29,   281,   123,    11,    31,   124,
     261,   262,    67,    68,   265,   131,   132,   120,   134,   135,
     136,   137,   138,    44,    45,    46,    47,   296,   172,   173,
      32,    36,     1,     2,    33,     4,     5,   -40,   169,   170,
      37,    61,    11,   168,    73,   181,    74,    62,   340,   169,
     170,    34,   328,   329,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,    59,   346,   215,    38,   217,   318,
     130,   350,   320,    87,    40,   254,    88,    19,    41,    48,
      72,   163,   164,   165,   166,   167,   168,    43,    59,    86,
     337,    49,   169,   170,   183,    19,    51,    86,    50,    72,
      90,    91,    92,   287,     2,   130,   348,   349,    59,   126,
      56,    59,   127,    11,   174,    65,    52,   127,    94,    95,
      96,    97,    98,    99,    81,    53,    54,    55,   268,   269,
      75,    75,    75,   161,   162,   163,   164,   165,   166,   167,
     168,   181,    60,    66,   280,   181,   169,   170,    69,    82,
     282,   283,    80,    83,    75,   180,   113,   116,   288,   290,
     165,   166,   167,   168,    62,   294,   295,   181,   115,   169,
     170,   117,   118,   119,   128,   100,   101,   177,   121,   129,
     122,   102,   103,   104,   125,   130,   105,   106,   133,   175,
     216,   107,   178,   218,   247,   313,   314,   248,   315,   316,
     317,   259,   250,   255,   219,   256,    90,    91,    92,   220,
       2,   257,     4,     5,   258,   285,   260,   263,   264,    11,
     335,   336,   266,   338,    94,    95,    96,    97,    98,    99,
     221,   222,   223,   224,   -43,   -43,   225,   226,   227,   270,
     228,   271,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   272,   273,   238,   239,   -44,   -44,   -44,   274,   149,
     275,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     276,   100,   101,   284,   286,   169,   170,   102,   103,   104,
     302,   289,   105,   106,   291,   297,   240,   107,   303,   175,
     -43,   219,   298,    90,    91,    92,   220,     2,   299,     4,
       5,   305,   307,   319,   311,   310,    11,   312,   321,   327,
     332,    94,    95,    96,    97,    98,    99,   221,   222,   223,
     224,   339,   333,   225,   226,   227,   334,   228,   347,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   342,    39,
     238,   239,   341,   114,   149,   112,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    89,     0,     0,   100,   101,
     169,   170,     0,     0,   102,   103,   104,     0,     0,   105,
     106,     0,     0,   240,   107,     0,   175,   -43,   219,     0,
      90,    91,    92,   220,     2,     0,     4,     5,     0,     0,
       0,     0,     0,    11,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,   221,   222,   223,   224,     0,     0,
     225,   226,   227,     0,   228,     0,   229,   230,   231,   232,
     233,   234,   235,   236,   237,     0,     0,   238,   239,    90,
      91,    92,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,   100,   101,    90,    91,    92,
      93,   102,   103,   104,     0,     0,   105,   106,     0,     0,
     240,   107,     0,   175,     0,    94,    95,    96,    97,    98,
      99,     1,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   100,   101,     0,     0,     0,     0,
     102,   103,   104,     0,     0,   105,   106,     0,     0,   267,
     107,     0,     0,    14,    15,     0,     0,     0,     0,     0,
       0,     0,   100,   101,   322,     0,     0,     0,   102,   103,
     104,     0,     0,   105,   106,     0,     0,     0,   107,     0,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
       0,   149,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,     0,     0,     0,     0,     0,   169,   170,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
     149,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,     0,     0,     0,     0,     0,   169,   170,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   251,   149,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
       0,     0,     0,     0,     0,   169,   170,     0,     0,     0,
       0,     0,     0,     0,   246,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,     0,   149,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,     0,     0,     0,
       0,     0,   169,   170,     0,     0,     0,     0,     0,     0,
       0,   300,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,   149,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,     0,     0,     0,     0,     0,   169,
     170,     0,     0,     0,     0,     0,     0,     0,   301,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
     149,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,     0,     0,     0,     0,     0,   169,   170,     0,     0,
       0,     0,   184,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,     0,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,     0,     0,     0,     0,     0,
     169,   170,     0,     0,     0,     0,   306,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,   149,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,     0,
       0,     0,     0,     0,   169,   170,     0,     0,     0,     0,
     308,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,   149,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,     0,     0,     0,     0,     0,   169,   170,
       0,     0,     0,     0,   309,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,     0,   149,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,     0,     0,     0,
       0,     0,   169,   170,     0,     0,     0,     0,   323,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
     149,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,     0,     0,     0,     0,     0,   169,   170,     0,     0,
       0,     0,   324,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,     0,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,     0,     0,     0,     0,     0,
     169,   170,     0,     0,     0,     0,   343,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,   149,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,     0,
       0,     0,     0,     0,   169,   170,     0,     0,     0,     0,
     344,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,   149,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,     0,     0,     0,     0,     0,   169,   170,
       0,     0,   278,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,     0,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,     0,     0,     0,     0,     0,
     169,   170,     0,     0,   292,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,     0,   149,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,     0,     0,     0,
       0,     0,   169,   170,     0,     0,   293,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,   149,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,     0,
       0,     0,     0,     0,   169,   170,     0,     0,   304,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
     149,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,     0,     0,     0,     0,     0,   169,   170,     0,     0,
     325,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,   149,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,     0,     0,     0,     0,     0,   169,   170,
       0,     0,   326,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,     0,   149,   249,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,     0,     0,     0,     0,     0,
     169,   170,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,   149,   345,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,     0,     0,     0,     0,     0,   169,
     170,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,   149,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,     0,     0,     0,     0,     0,   169,   170,
     -44,   -44,   -44,   142,   143,   144,   145,   146,   147,   148,
       0,   149,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,     0,     0,     0,     0,     0,   169,   170,   -44,
     -44,   144,   145,   146,   147,   148,     0,   149,     0,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,     0,     0,
       0,     0,     0,   169,   170,   -44,   -44,   146,   147,   148,
       0,   149,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,     0,     0,     0,     0,     0,   169,   170,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,     0,     0,     0,     0,
       0,   169,   170,   -44,   -44,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
       0,     0,     0,     0,     0,   169,   170,   -44,   -44,   -44,
     -44,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,     0,     0,     0,     0,     0,   169,   170,   160,
     161,   162,   163,   164,   165,   166,   167,   168,     0,     0,
       0,     0,     0,   169,   170
};

static const yytype_int16 yycheck[] =
{
      65,     0,    31,    32,    49,     6,    60,     5,    93,     8,
     244,    62,    97,   116,    54,    55,     6,     6,     7,    18,
       6,   246,     6,   126,     5,   250,    80,    16,     6,    83,
     227,   228,     3,     4,   231,   100,   101,    77,   103,   104,
     105,   106,   107,     3,     4,     5,     6,   272,    99,   100,
       6,    96,     6,     7,     6,     9,    10,     6,    89,    90,
       3,    94,    16,    83,    18,   130,    20,   100,    97,    89,
      90,     6,    31,    32,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,    36,   339,   171,     0,   173,   306,
      94,   345,   309,     3,    93,    64,     6,     0,     6,    79,
      52,    78,    79,    80,    81,    82,    83,     5,    60,    61,
     327,    93,    89,    90,   133,    18,     6,    69,    93,    71,
       3,     4,     5,     6,     7,    94,   343,   344,    80,    95,
       3,    83,    98,    16,    95,    62,    96,    98,    21,    22,
      23,    24,    25,    26,    97,    96,    96,    96,   233,   234,
      53,    54,    55,    76,    77,    78,    79,    80,    81,    82,
      83,   246,    96,    93,   249,   250,    89,    90,    94,     6,
     255,   256,    96,    93,    77,   127,    97,     6,   263,   264,
      80,    81,    82,    83,   100,   270,   271,   272,    93,    89,
      90,     6,    93,    97,   101,    78,    79,    93,    97,   101,
      97,    84,    85,    86,    97,    94,    89,    90,    94,    96,
       6,    94,    97,    93,    95,   300,   301,    95,   303,   304,
     305,     6,    94,    94,     1,    94,     3,     4,     5,     6,
       7,    93,     9,    10,    93,    95,    94,    94,    94,    16,
     325,   326,    94,   328,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    94,
      37,    94,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    94,    94,    50,    51,    59,    60,    61,    94,    63,
      93,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      97,    78,    79,    93,    46,    89,    90,    84,    85,    86,
      93,   263,    89,    90,    95,    95,    93,    94,    94,    96,
      97,     1,    95,     3,     4,     5,     6,     7,    95,     9,
      10,    93,    93,    96,    93,    95,    16,    93,    93,    38,
       6,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    64,    93,    33,    34,    35,    93,    37,    93,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    95,    18,
      50,    51,   330,    71,    63,    69,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    64,    -1,    -1,    78,    79,
      89,    90,    -1,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    -1,    -1,    93,    94,    -1,    96,    97,     1,    -1,
       3,     4,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      33,    34,    35,    -1,    37,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    -1,    50,    51,     3,
       4,     5,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    23,
      24,    25,    26,    -1,    -1,    78,    79,     3,     4,     5,
       6,    84,    85,    86,    -1,    -1,    89,    90,    -1,    -1,
      93,    94,    -1,    96,    -1,    21,    22,    23,    24,    25,
      26,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    -1,    -1,    93,
      94,    -1,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    36,    -1,    -1,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    -1,    -1,    -1,    94,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,   101,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,
      -1,    -1,    95,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      89,    90,    -1,    -1,    -1,    -1,    95,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      95,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,
      -1,    -1,    -1,    -1,    95,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    95,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,
      -1,    -1,    95,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      89,    90,    -1,    -1,    -1,    -1,    95,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,
      95,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,
      -1,    -1,    93,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      89,    90,    -1,    -1,    93,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    63,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    -1,    93,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    93,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,
      93,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,
      -1,    -1,    93,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      89,    90,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,
      90,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90,    57,    58,    59,    60,    61,
      -1,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    89,    90,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    89,    90,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    89,    90
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    19,    48,    49,   103,   104,   105,   107,
     108,   111,   114,   115,   127,     6,     6,     6,     5,     5,
     115,     6,     6,     6,     6,    49,    96,     3,     0,   104,
      93,     6,   128,     5,     3,     4,     5,     6,    79,    93,
      93,     6,    96,    96,    96,    96,     3,   106,   115,   127,
      96,    94,   100,   129,   130,    62,    93,     3,     4,    94,
     112,   113,   127,    18,    20,   107,   109,   110,   109,   109,
      96,    97,     6,    93,   106,   126,   127,     3,     6,   129,
       3,     4,     5,     6,    21,    22,    23,    24,    25,    26,
      78,    79,    84,    85,    86,    89,    90,    94,   119,   120,
     121,   122,   126,    97,   112,    93,     6,     6,    93,    97,
     109,    97,    97,   106,   106,    97,    95,    98,   101,   101,
      94,   119,   119,    94,   119,   119,   119,   119,   119,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    63,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    89,
      90,    62,    99,   100,    95,    96,   116,    93,    97,   116,
     127,   119,   125,   115,    95,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,     6,   119,    93,     1,
       6,    27,    28,    29,    30,    33,    34,    35,    37,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    50,    51,
      93,   107,   116,   117,   118,   119,    98,    95,    95,    64,
      94,   101,    93,    97,    64,    94,    94,    93,    93,     6,
      94,   118,   118,    94,    94,   118,    94,    93,   119,   119,
      94,    94,    94,    94,    94,    93,    97,   117,    93,   125,
     119,   125,   119,   119,    93,    95,    46,     6,   119,   127,
     119,    95,    93,    93,   119,   119,   125,    95,    95,    95,
      98,    98,    93,    94,    93,    93,    95,    93,    95,    95,
      95,    93,    93,   119,   119,   119,   119,   119,   118,    96,
     118,    93,    36,    95,    95,    93,    93,    38,    31,    32,
     123,   124,     6,    93,    93,   119,   119,   118,   119,    64,
      97,   124,    95,    95,    95,    64,   117,    93,   118,   118,
     117
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   102,   103,   104,   104,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   106,   106,   107,   107,
     107,   107,   107,   107,   107,   107,   108,   108,   108,   109,
     109,   110,   110,   110,   111,   112,   112,   113,   114,   115,
     115,   115,   116,   117,   117,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     120,   120,   120,   120,   120,   120,   120,   120,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   122,   122,   122,
     122,   122,   123,   123,   124,   124,   125,   125,   125,   126,
     126,   126,   127,   127,   128,   128,   129,   129,   130,   130
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     2,     7,
       5,     4,     6,     4,     3,     3,     0,     3,     1,     3,
       3,     4,     4,     3,     3,     2,     5,     5,     5,     0,
       2,     3,     3,     2,     5,     0,     2,     2,     6,     1,
       1,     1,     3,     0,     2,     5,     7,     7,     5,     7,
       9,     9,     2,     2,     3,     2,     2,     3,     3,     2,
       2,     4,     4,     4,     4,     5,     9,     7,     1,     2,
       2,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     5,     1,
       2,     2,     2,     2,     4,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     6,     4,
       1,     3,     1,     2,     4,     3,     0,     1,     3,     0,
       1,     3,     2,     4,     1,     2,     1,     2,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

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
      yychar = yylex ();
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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 92 "Parser.yy" /* yacc.c:1646  */
    {
						if((yyvsp[0].mNode)) (yyvsp[0].mNode)->Dump();

						auto globalSemanticAnalyzer = mana::GetSystemHolder().GetGlobalSemanticAnalyzer();
						globalSemanticAnalyzer->Resolve((yyvsp[0].mNode));

						auto codeGenerator = mana::GetSystemHolder().GetCodeGenerator();
						codeGenerator->generator_genearte_code((yyvsp[0].mNode), true);

						auto globalAddressResolver = mana::GetSystemHolder().GetCodeGenerator()->GetGlobalAddressResolver();
						globalAddressResolver->mana_linker_resolve_address();
					}
#line 1848 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 107 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 1854 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 109 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::Bind((yyvsp[-1].mNode), (yyvsp[0].mNode)); }
#line 1860 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 118 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateNativeFunction((yyvsp[-5].mNode), (yyvsp[-4].mString), (yyvsp[-2].mNode)); }
#line 1866 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 121 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclareMemory((yyvsp[-3].mInt), (yyvsp[-1].mNode)); }
#line 1872 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 123 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclareStaticMemory(0, (yyvsp[-1].mNode)); }
#line 1878 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 125 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclareStaticMemory((yyvsp[-3].mInt), (yyvsp[-1].mNode)); }
#line 1884 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 128 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateAlias((yyvsp[-2].mString), (yyvsp[-1].mString)); }
#line 1890 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 131 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; if(! lexer_open((yyvsp[-1].mString), false)){ YYABORT; } }
#line 1896 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 133 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; if(! lexer_open((yyvsp[-1].mString), true)){ YYABORT; } }
#line 1902 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 138 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 1908 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 140 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::Bind((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 1914 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 145 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateConstantNode((yyvsp[-1].mString), (yyvsp[0].mInt)); }
#line 1920 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 147 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateConstantNode((yyvsp[-1].mString), (yyvsp[0].mReal)); }
#line 1926 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 149 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateConstantNode((yyvsp[-2].mString), -(yyvsp[0].mInt)); }
#line 1932 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 151 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateConstantNode((yyvsp[-2].mString), -(yyvsp[0].mReal)); }
#line 1938 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 153 "Parser.yy" /* yacc.c:1646  */
    { //$$ = node_create_node(NODE_DEFINE_CONSTANT, nullptr, nullptr, nullptr); $$->mString = $3;
					 }
#line 1945 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 156 "Parser.yy" /* yacc.c:1646  */
    {// $$ = node_create_node(NODE_DEFINE_CONSTANT, nullptr, nullptr, nullptr); $$->mString = $3;
					 }
#line 1952 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 159 "Parser.yy" /* yacc.c:1646  */
    {// $$ = node_create_node(NODE_UNDEFINE_CONSTANT, nullptr, nullptr, nullptr); $$->mString = $2;
					 }
#line 1959 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 164 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateActor((yyvsp[-3].mString), (yyvsp[-1].mNode)); }
#line 1965 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 166 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreatePhantom((yyvsp[-3].mString), (yyvsp[-1].mNode)); }
#line 1971 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 168 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateModule((yyvsp[-3].mString), (yyvsp[-1].mNode)); }
#line 1977 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 172 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 1983 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 174 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::Bind((yyvsp[-1].mNode), (yyvsp[0].mNode)); }
#line 1989 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 178 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateAction((yyvsp[-1].mString), (yyvsp[0].mNode)); }
#line 1995 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 180 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateExtend((yyvsp[-1].mString)); }
#line 2001 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 185 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateStruct((yyvsp[-3].mString), (yyvsp[-1].mNode)); }
#line 2007 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 189 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 2013 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 191 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::Bind((yyvsp[-1].mNode), (yyvsp[0].mNode)); }
#line 2019 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 198 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateInternalFunction((yyvsp[-5].mNode), (yyvsp[-4].mString), (yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2025 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 202 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateActorTypeDescription(); }
#line 2031 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 204 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateTypeDescription((yyvsp[0].mString)); }
#line 2037 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 206 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateTypeDescription((yyvsp[0].mTypeDescriptor)); }
#line 2043 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 210 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateBlock((yyvsp[-1].mNode)); }
#line 2049 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 214 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 2055 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 216 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::Bind((yyvsp[-1].mNode), (yyvsp[0].mNode)); }
#line 2061 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 219 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateIf((yyvsp[-2].mNode), (yyvsp[0].mNode), nullptr); }
#line 2067 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 221 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateIf((yyvsp[-4].mNode), (yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2073 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 223 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSwitch((yyvsp[-4].mNode), (yyvsp[-1].mNode)); }
#line 2079 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 225 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateWhile((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2085 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 227 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDoWhile((yyvsp[-5].mNode), (yyvsp[-2].mNode)); }
#line 2091 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 229 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateFor((yyvsp[-6].mNode), (yyvsp[-4].mNode), (yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2097 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 231 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateFor((yyvsp[-6].mNode), (yyvsp[-4].mNode), (yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2103 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 233 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLoop((yyvsp[0].mNode)); }
#line 2109 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 235 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLock((yyvsp[0].mNode)); }
#line 2115 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 237 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateGoto((yyvsp[-1].mString)); }
#line 2121 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 239 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLabel((yyvsp[-1].mString)); }
#line 2127 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 241 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateReturn(nullptr); }
#line 2133 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 243 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateReturn((yyvsp[-1].mNode)); }
#line 2139 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 245 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateRollback((yyvsp[-1].mNode)); }
#line 2145 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 247 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateBreak(); }
#line 2151 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 249 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateContinue(); }
#line 2157 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 251 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateHalt(); }
#line 2163 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 253 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateYield(); }
#line 2169 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 255 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateComply(); }
#line 2175 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 257 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateRefuse(); }
#line 2181 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 259 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreatePrint((yyvsp[-2].mNode)); }
#line 2187 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 261 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateRequest((yyvsp[-6].mNode), (yyvsp[-4].mNode), (yyvsp[-2].mString)); }
#line 2193 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 263 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateJoin((yyvsp[-4].mNode), (yyvsp[-2].mNode)); }
#line 2199 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 268 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 2205 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 270 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; yyerrok; yyclearin; }
#line 2211 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 272 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; yyerrok; yyclearin; }
#line 2217 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 276 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2223 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 278 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLogicalAnd((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2229 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 280 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLogicalOr((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2235 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 282 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateAddAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2241 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 284 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSubAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2247 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 286 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateMulAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2253 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 288 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDivAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2259 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 290 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateModAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2265 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 292 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateAndAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2271 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 294 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateOrAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2277 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 296 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateXorAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2283 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 298 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLeftShiftAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2289 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 300 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateRightShiftAndAssign((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2295 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 302 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateAdd((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2301 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 304 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSub((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2307 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 306 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateMul((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2313 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 308 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDiv((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2319 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 310 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateMod((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2325 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 312 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreatePow((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2331 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 314 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateAnd((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2337 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 316 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateOr((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2343 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 318 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateXor((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2349 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 320 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLeftShift((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2355 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 322 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateRightShift((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2361 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 324 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateGT((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2367 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 326 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateGE((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2373 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 328 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLS((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2379 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 330 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateLE((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2385 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 332 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateEQ((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2391 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 334 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateNE((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2397 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 336 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateIncrement((yyvsp[0].mNode)); }
#line 2403 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 338 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDecrement((yyvsp[0].mNode)); }
#line 2409 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 340 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateIncrement((yyvsp[-1].mNode)); }
#line 2415 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 342 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDecrement((yyvsp[-1].mNode)); }
#line 2421 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 344 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateExpressionIf((yyvsp[-4].mNode), (yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2427 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 349 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateNegative((yyvsp[0].mNode)); }
#line 2433 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 351 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = (yyvsp[0].mNode); }
#line 2439 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 353 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateNot((yyvsp[0].mNode)); }
#line 2445 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 355 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateComplement1((yyvsp[0].mNode)); }
#line 2451 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 357 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSizeOf((yyvsp[-1].mNode)); }
#line 2457 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 359 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateCall((yyvsp[-3].mString), (yyvsp[-1].mNode)); }
#line 2463 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 365 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateInteger(0); }
#line 2469 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 367 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateInteger(1); }
#line 2475 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 369 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreatePriority(); }
#line 2481 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 371 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSelf(); }
#line 2487 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 373 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSender(); }
#line 2493 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 375 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateNil(); }
#line 2499 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 377 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateInteger((yyvsp[0].mInt)); }
#line 2505 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 379 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateFloat((yyvsp[0].mReal)); }
#line 2511 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 381 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateString((yyvsp[0].mString)); }
#line 2517 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 385 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateMemberVariable((yyvsp[-2].mNode), (yyvsp[0].mString)); }
#line 2523 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 387 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateMemberFunction((yyvsp[-5].mNode), (yyvsp[-3].mString), (yyvsp[-1].mNode)); }
#line 2529 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 389 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateArray((yyvsp[-3].mNode), (yyvsp[-1].mNode)); }
#line 2535 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 391 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateIdentifier((yyvsp[0].mString)); }
#line 2541 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 393 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = (yyvsp[-1].mNode); }
#line 2547 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 398 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::BindCaseNode((yyvsp[-1].mNode), (yyvsp[0].mNode)); }
#line 2553 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 401 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSwitchCaseNode((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2559 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 403 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateSwitchDefaultNode((yyvsp[0].mNode)); }
#line 2565 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 407 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 2571 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 409 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateArgumentNode((yyvsp[0].mNode)); }
#line 2577 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 411 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateArgumentNode((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2583 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 415 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = nullptr; }
#line 2589 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 417 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclareArgumentNode((yyvsp[0].mNode)); }
#line 2595 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 419 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclareArgumentNode((yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2601 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 424 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclareVariableNode((yyvsp[-1].mNode), (yyvsp[0].mNode), nullptr); }
#line 2607 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 426 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclareVariableNode((yyvsp[-3].mNode), (yyvsp[-2].mNode), (yyvsp[0].mNode)); }
#line 2613 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 431 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclaratorNode((yyvsp[0].mString)); }
#line 2619 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 433 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = mana::CreateDeclaratorNode((yyvsp[-1].mString), (yyvsp[0].mNode)); }
#line 2625 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 438 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = 	mana::Bind((yyvsp[-1].mNode), (yyvsp[0].mNode)); }
#line 2631 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 441 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = 	mana::CreateVariableSizeNode((yyvsp[-1].mInt)); }
#line 2637 "Parser.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 443 "Parser.yy" /* yacc.c:1646  */
    { (yyval.mNode) = 	mana::CreateVariableSizeNode((yyvsp[-1].mString)); }
#line 2643 "Parser.cpp" /* yacc.c:1646  */
    break;


#line 2647 "Parser.cpp" /* yacc.c:1646  */
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 445 "Parser.yy" /* yacc.c:1906  */


#include "ParserDefinition.inl"

/*!
print error message
@param	message		error message
*/
void yyerror(const char* message)
{
	mana::CompileError(message);
}
