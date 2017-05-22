// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 1 "Parser.yy"

/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Main.h"
#include "Symbol.h"
#include "TypeDescriptor.h"
//#include "generator.h"
#include "Lexer.h"
//#include "linker.h"
//#include "main.h"
//#include "pre_resolver.h"

#include "SyntaxNode.h"
#include "Parser.hpp"

#include "ParserDeclaration.inl"
#include <memory>
#include <string_view>

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
#define YYERROR_VERBOSE
#endif


#line 74 "Parser.cpp"


#include "Parser.hpp"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 38 "Parser.yy"
namespace mana {
#line 153 "Parser.cpp"

  /// Build a parser object.
  Parser::Parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->kind ())
    {
      case symbol_kind::S_tREAL: // tREAL
        value.copy< mana::float_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tDIGIT: // tDIGIT
        value.copy< mana::int_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_declarations: // declarations
      case symbol_kind::S_allocate_declarations: // allocate_declarations
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_actor: // actor
      case symbol_kind::S_actions: // actions
      case symbol_kind::S_action: // action
      case symbol_kind::S_struct: // struct
      case symbol_kind::S_struct_members: // struct_members
      case symbol_kind::S_struct_member: // struct_member
      case symbol_kind::S_function: // function
      case symbol_kind::S_variable_type: // variable_type
      case symbol_kind::S_block: // block
      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_primary: // primary
      case symbol_kind::S_constant: // constant
      case symbol_kind::S_left_hand: // left_hand
      case symbol_kind::S_cases: // cases
      case symbol_kind::S_case: // case
      case symbol_kind::S_arg_calls: // arg_calls
      case symbol_kind::S_arg_decls: // arg_decls
      case symbol_kind::S_variable_decl: // variable_decl
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_variable_sizes: // variable_sizes
      case symbol_kind::S_variable_size: // variable_size
        value.copy< std::shared_ptr<mana::SyntaxNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tTYPE: // tTYPE
        value.copy< std::shared_ptr<mana::TypeDescriptor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tSTRING: // tSTRING
      case symbol_kind::S_tIDENTIFIER: // tIDENTIFIER
        value.copy< std::string_view > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_tREAL: // tREAL
        value.move< mana::float_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_tDIGIT: // tDIGIT
        value.move< mana::int_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_declarations: // declarations
      case symbol_kind::S_allocate_declarations: // allocate_declarations
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_actor: // actor
      case symbol_kind::S_actions: // actions
      case symbol_kind::S_action: // action
      case symbol_kind::S_struct: // struct
      case symbol_kind::S_struct_members: // struct_members
      case symbol_kind::S_struct_member: // struct_member
      case symbol_kind::S_function: // function
      case symbol_kind::S_variable_type: // variable_type
      case symbol_kind::S_block: // block
      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_primary: // primary
      case symbol_kind::S_constant: // constant
      case symbol_kind::S_left_hand: // left_hand
      case symbol_kind::S_cases: // cases
      case symbol_kind::S_case: // case
      case symbol_kind::S_arg_calls: // arg_calls
      case symbol_kind::S_arg_decls: // arg_decls
      case symbol_kind::S_variable_decl: // variable_decl
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_variable_sizes: // variable_sizes
      case symbol_kind::S_variable_size: // variable_size
        value.move< std::shared_ptr<mana::SyntaxNode> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_tTYPE: // tTYPE
        value.move< std::shared_ptr<mana::TypeDescriptor> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_tSTRING: // tSTRING
      case symbol_kind::S_tIDENTIFIER: // tIDENTIFIER
        value.move< std::string_view > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_tREAL: // tREAL
        value.YY_MOVE_OR_COPY< mana::float_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tDIGIT: // tDIGIT
        value.YY_MOVE_OR_COPY< mana::int_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_declarations: // declarations
      case symbol_kind::S_allocate_declarations: // allocate_declarations
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_actor: // actor
      case symbol_kind::S_actions: // actions
      case symbol_kind::S_action: // action
      case symbol_kind::S_struct: // struct
      case symbol_kind::S_struct_members: // struct_members
      case symbol_kind::S_struct_member: // struct_member
      case symbol_kind::S_function: // function
      case symbol_kind::S_variable_type: // variable_type
      case symbol_kind::S_block: // block
      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_primary: // primary
      case symbol_kind::S_constant: // constant
      case symbol_kind::S_left_hand: // left_hand
      case symbol_kind::S_cases: // cases
      case symbol_kind::S_case: // case
      case symbol_kind::S_arg_calls: // arg_calls
      case symbol_kind::S_arg_decls: // arg_decls
      case symbol_kind::S_variable_decl: // variable_decl
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_variable_sizes: // variable_sizes
      case symbol_kind::S_variable_size: // variable_size
        value.YY_MOVE_OR_COPY< std::shared_ptr<mana::SyntaxNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tTYPE: // tTYPE
        value.YY_MOVE_OR_COPY< std::shared_ptr<mana::TypeDescriptor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tSTRING: // tSTRING
      case symbol_kind::S_tIDENTIFIER: // tIDENTIFIER
        value.YY_MOVE_OR_COPY< std::string_view > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_tREAL: // tREAL
        value.move< mana::float_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tDIGIT: // tDIGIT
        value.move< mana::int_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_declarations: // declarations
      case symbol_kind::S_allocate_declarations: // allocate_declarations
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_actor: // actor
      case symbol_kind::S_actions: // actions
      case symbol_kind::S_action: // action
      case symbol_kind::S_struct: // struct
      case symbol_kind::S_struct_members: // struct_members
      case symbol_kind::S_struct_member: // struct_member
      case symbol_kind::S_function: // function
      case symbol_kind::S_variable_type: // variable_type
      case symbol_kind::S_block: // block
      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_primary: // primary
      case symbol_kind::S_constant: // constant
      case symbol_kind::S_left_hand: // left_hand
      case symbol_kind::S_cases: // cases
      case symbol_kind::S_case: // case
      case symbol_kind::S_arg_calls: // arg_calls
      case symbol_kind::S_arg_decls: // arg_decls
      case symbol_kind::S_variable_decl: // variable_decl
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_variable_sizes: // variable_sizes
      case symbol_kind::S_variable_size: // variable_size
        value.move< std::shared_ptr<mana::SyntaxNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tTYPE: // tTYPE
        value.move< std::shared_ptr<mana::TypeDescriptor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tSTRING: // tSTRING
      case symbol_kind::S_tIDENTIFIER: // tIDENTIFIER
        value.move< std::string_view > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_tREAL: // tREAL
        value.copy< mana::float_t > (that.value);
        break;

      case symbol_kind::S_tDIGIT: // tDIGIT
        value.copy< mana::int_t > (that.value);
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_declarations: // declarations
      case symbol_kind::S_allocate_declarations: // allocate_declarations
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_actor: // actor
      case symbol_kind::S_actions: // actions
      case symbol_kind::S_action: // action
      case symbol_kind::S_struct: // struct
      case symbol_kind::S_struct_members: // struct_members
      case symbol_kind::S_struct_member: // struct_member
      case symbol_kind::S_function: // function
      case symbol_kind::S_variable_type: // variable_type
      case symbol_kind::S_block: // block
      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_primary: // primary
      case symbol_kind::S_constant: // constant
      case symbol_kind::S_left_hand: // left_hand
      case symbol_kind::S_cases: // cases
      case symbol_kind::S_case: // case
      case symbol_kind::S_arg_calls: // arg_calls
      case symbol_kind::S_arg_decls: // arg_decls
      case symbol_kind::S_variable_decl: // variable_decl
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_variable_sizes: // variable_sizes
      case symbol_kind::S_variable_size: // variable_size
        value.copy< std::shared_ptr<mana::SyntaxNode> > (that.value);
        break;

      case symbol_kind::S_tTYPE: // tTYPE
        value.copy< std::shared_ptr<mana::TypeDescriptor> > (that.value);
        break;

      case symbol_kind::S_tSTRING: // tSTRING
      case symbol_kind::S_tIDENTIFIER: // tIDENTIFIER
        value.copy< std::string_view > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_tREAL: // tREAL
        value.move< mana::float_t > (that.value);
        break;

      case symbol_kind::S_tDIGIT: // tDIGIT
        value.move< mana::int_t > (that.value);
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_declarations: // declarations
      case symbol_kind::S_allocate_declarations: // allocate_declarations
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_actor: // actor
      case symbol_kind::S_actions: // actions
      case symbol_kind::S_action: // action
      case symbol_kind::S_struct: // struct
      case symbol_kind::S_struct_members: // struct_members
      case symbol_kind::S_struct_member: // struct_member
      case symbol_kind::S_function: // function
      case symbol_kind::S_variable_type: // variable_type
      case symbol_kind::S_block: // block
      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_primary: // primary
      case symbol_kind::S_constant: // constant
      case symbol_kind::S_left_hand: // left_hand
      case symbol_kind::S_cases: // cases
      case symbol_kind::S_case: // case
      case symbol_kind::S_arg_calls: // arg_calls
      case symbol_kind::S_arg_decls: // arg_decls
      case symbol_kind::S_variable_decl: // variable_decl
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_variable_sizes: // variable_sizes
      case symbol_kind::S_variable_size: // variable_size
        value.move< std::shared_ptr<mana::SyntaxNode> > (that.value);
        break;

      case symbol_kind::S_tTYPE: // tTYPE
        value.move< std::shared_ptr<mana::TypeDescriptor> > (that.value);
        break;

      case symbol_kind::S_tSTRING: // tSTRING
      case symbol_kind::S_tIDENTIFIER: // tIDENTIFIER
        value.move< std::string_view > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_tREAL: // tREAL
        yylhs.value.emplace< mana::float_t > ();
        break;

      case symbol_kind::S_tDIGIT: // tDIGIT
        yylhs.value.emplace< mana::int_t > ();
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_declarations: // declarations
      case symbol_kind::S_allocate_declarations: // allocate_declarations
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_actor: // actor
      case symbol_kind::S_actions: // actions
      case symbol_kind::S_action: // action
      case symbol_kind::S_struct: // struct
      case symbol_kind::S_struct_members: // struct_members
      case symbol_kind::S_struct_member: // struct_member
      case symbol_kind::S_function: // function
      case symbol_kind::S_variable_type: // variable_type
      case symbol_kind::S_block: // block
      case symbol_kind::S_statements: // statements
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_primary: // primary
      case symbol_kind::S_constant: // constant
      case symbol_kind::S_left_hand: // left_hand
      case symbol_kind::S_cases: // cases
      case symbol_kind::S_case: // case
      case symbol_kind::S_arg_calls: // arg_calls
      case symbol_kind::S_arg_decls: // arg_decls
      case symbol_kind::S_variable_decl: // variable_decl
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_variable_sizes: // variable_sizes
      case symbol_kind::S_variable_size: // variable_size
        yylhs.value.emplace< std::shared_ptr<mana::SyntaxNode> > ();
        break;

      case symbol_kind::S_tTYPE: // tTYPE
        yylhs.value.emplace< std::shared_ptr<mana::TypeDescriptor> > ();
        break;

      case symbol_kind::S_tSTRING: // tSTRING
      case symbol_kind::S_tIDENTIFIER: // tIDENTIFIER
        yylhs.value.emplace< std::string_view > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: line
#line 94 "Parser.yy"
                                        {
						if (yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ())
						{
							yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()->Dump();
						}

						auto globalSemanticAnalyzer = mana::GetSystemHolder().GetGlobalSemanticAnalyzer();
						globalSemanticAnalyzer->Resolve(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ());

						auto codeGenerator = mana::GetSystemHolder().GetCodeGenerator();
						codeGenerator->generator_genearte_code(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (), true);

						auto globalAddressResolver = mana::GetSystemHolder().GetCodeGenerator()->GetGlobalAddressResolver();
						globalAddressResolver->ResolveAddress();
						//globalAddressResolver->mana_linker_resolve_address();
					}
#line 971 "Parser.cpp"
    break;

  case 3: // line: %empty
#line 113 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 977 "Parser.cpp"
    break;

  case 4: // line: declarations line
#line 115 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::Bind(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 983 "Parser.cpp"
    break;

  case 5: // declarations: actor
#line 118 "Parser.yy"
                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 989 "Parser.cpp"
    break;

  case 6: // declarations: struct
#line 119 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 995 "Parser.cpp"
    break;

  case 7: // declarations: function
#line 120 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1001 "Parser.cpp"
    break;

  case 8: // declarations: declaration ';'
#line 121 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1007 "Parser.cpp"
    break;

  case 9: // declarations: tNATIVE variable_type tIDENTIFIER '(' arg_decls ')' ';'
#line 124 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateNativeFunction(yystack_[5].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[4].value.as < std::string_view > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1013 "Parser.cpp"
    break;

  case 10: // declarations: tALLOCATE tDIGIT '{' allocate_declarations '}'
#line 127 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclareMemory(yystack_[3].value.as < mana::int_t > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1019 "Parser.cpp"
    break;

  case 11: // declarations: tSTATIC '{' allocate_declarations '}'
#line 129 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclareStaticMemory(0, yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1025 "Parser.cpp"
    break;

  case 12: // declarations: tSTATIC tALLOCATE tDIGIT '{' allocate_declarations '}'
#line 131 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclareStaticMemory(yystack_[3].value.as < mana::int_t > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1031 "Parser.cpp"
    break;

  case 13: // declarations: tALIAS tIDENTIFIER tSTRING ';'
#line 134 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateAlias(yystack_[2].value.as < std::string_view > (), yystack_[1].value.as < std::string_view > ()); }
#line 1037 "Parser.cpp"
    break;

  case 14: // declarations: tINCLUDE tSTRING ';'
#line 137 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; if(! lexer_open(yystack_[1].value.as < std::string_view > (), false)){ YYABORT; } }
#line 1043 "Parser.cpp"
    break;

  case 15: // declarations: tIMPORT tSTRING ';'
#line 139 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; if(! lexer_open(yystack_[1].value.as < std::string_view > (), true)){ YYABORT; } }
#line 1049 "Parser.cpp"
    break;

  case 16: // allocate_declarations: %empty
#line 144 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 1055 "Parser.cpp"
    break;

  case 17: // allocate_declarations: variable_decl ';' allocate_declarations
#line 146 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::Bind(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1061 "Parser.cpp"
    break;

  case 18: // declaration: variable_decl
#line 149 "Parser.yy"
                          { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1067 "Parser.cpp"
    break;

  case 19: // declaration: tDEFINE tIDENTIFIER tDIGIT
#line 151 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateConstantNode(yystack_[1].value.as < std::string_view > (), yystack_[0].value.as < mana::int_t > ()); }
#line 1073 "Parser.cpp"
    break;

  case 20: // declaration: tDEFINE tIDENTIFIER tREAL
#line 153 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateConstantNode(yystack_[1].value.as < std::string_view > (), yystack_[0].value.as < mana::float_t > ()); }
#line 1079 "Parser.cpp"
    break;

  case 21: // declaration: tDEFINE tIDENTIFIER '-' tDIGIT
#line 155 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateConstantNode(yystack_[2].value.as < std::string_view > (), -yystack_[0].value.as < mana::int_t > ()); }
#line 1085 "Parser.cpp"
    break;

  case 22: // declaration: tDEFINE tIDENTIFIER '-' tREAL
#line 157 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateConstantNode(yystack_[2].value.as < std::string_view > (), -yystack_[0].value.as < mana::float_t > ()); }
#line 1091 "Parser.cpp"
    break;

  case 23: // declaration: tDEFINE tIDENTIFIER tSTRING
#line 159 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = CreateConstantNode(yystack_[1].value.as < std::string_view > (), yystack_[0].value.as < std::string_view > ()); }
#line 1097 "Parser.cpp"
    break;

  case 24: // declaration: tDEFINE tIDENTIFIER tIDENTIFIER
#line 161 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = CreateDefineNode(yystack_[1].value.as < std::string_view > (), yystack_[0].value.as < std::string_view > ()); }
#line 1103 "Parser.cpp"
    break;

  case 25: // declaration: tUNDEF tIDENTIFIER
#line 163 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = CreateUndefineNode(yystack_[0].value.as < std::string_view > ()); }
#line 1109 "Parser.cpp"
    break;

  case 26: // actor: tACTOR tIDENTIFIER '{' actions '}'
#line 167 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateActor(yystack_[3].value.as < std::string_view > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1115 "Parser.cpp"
    break;

  case 27: // actor: tPHANTOM tIDENTIFIER '{' actions '}'
#line 169 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreatePhantom(yystack_[3].value.as < std::string_view > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1121 "Parser.cpp"
    break;

  case 28: // actor: tMODULE tIDENTIFIER '{' actions '}'
#line 171 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateModule(yystack_[3].value.as < std::string_view > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1127 "Parser.cpp"
    break;

  case 29: // actions: %empty
#line 175 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 1133 "Parser.cpp"
    break;

  case 30: // actions: action actions
#line 177 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::Bind(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1139 "Parser.cpp"
    break;

  case 31: // action: tACTION tIDENTIFIER block
#line 181 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateAction(yystack_[1].value.as < std::string_view > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1145 "Parser.cpp"
    break;

  case 32: // action: tEXTEND tIDENTIFIER ';'
#line 183 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateExtend(yystack_[1].value.as < std::string_view > ()); }
#line 1151 "Parser.cpp"
    break;

  case 33: // action: declaration ';'
#line 184 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1157 "Parser.cpp"
    break;

  case 34: // struct: tSTRUCT tIDENTIFIER '{' struct_members '}'
#line 188 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateStruct(yystack_[3].value.as < std::string_view > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1163 "Parser.cpp"
    break;

  case 35: // struct_members: %empty
#line 192 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 1169 "Parser.cpp"
    break;

  case 36: // struct_members: struct_member struct_members
#line 194 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::Bind(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1175 "Parser.cpp"
    break;

  case 37: // struct_member: variable_decl ';'
#line 197 "Parser.yy"
                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1181 "Parser.cpp"
    break;

  case 38: // function: variable_type tIDENTIFIER '(' arg_decls ')' block
#line 201 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateInternalFunction(yystack_[5].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[4].value.as < std::string_view > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1187 "Parser.cpp"
    break;

  case 39: // variable_type: tACTOR2
#line 205 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateActorTypeDescription(); }
#line 1193 "Parser.cpp"
    break;

  case 40: // variable_type: tIDENTIFIER
#line 207 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateTypeDescription(yystack_[0].value.as < std::string_view > ()); }
#line 1199 "Parser.cpp"
    break;

  case 41: // variable_type: tTYPE
#line 209 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateTypeDescription(yystack_[0].value.as < std::shared_ptr<mana::TypeDescriptor> > ()); }
#line 1205 "Parser.cpp"
    break;

  case 42: // block: '{' statements '}'
#line 213 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateBlock(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1211 "Parser.cpp"
    break;

  case 43: // statements: %empty
#line 217 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 1217 "Parser.cpp"
    break;

  case 44: // statements: statement statements
#line 219 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::Bind(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1223 "Parser.cpp"
    break;

  case 45: // statement: tIF '(' expression ')' statement
#line 222 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateIf(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (), nullptr); }
#line 1229 "Parser.cpp"
    break;

  case 46: // statement: tIF '(' expression ')' statement tELSE statement
#line 224 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateIf(yystack_[4].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1235 "Parser.cpp"
    break;

  case 47: // statement: tSWITCH '(' expression ')' '{' cases '}'
#line 226 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSwitch(yystack_[4].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1241 "Parser.cpp"
    break;

  case 48: // statement: tWHILE '(' expression ')' statement
#line 228 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateWhile(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1247 "Parser.cpp"
    break;

  case 49: // statement: tDO statement tWHILE '(' expression ')' ';'
#line 230 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDoWhile(yystack_[5].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1253 "Parser.cpp"
    break;

  case 50: // statement: tFOR '(' expression ';' expression ';' expression ')' statement
#line 232 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateFor(yystack_[6].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[4].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1259 "Parser.cpp"
    break;

  case 51: // statement: tFOR '(' variable_decl ';' expression ';' expression ')' statement
#line 234 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateFor(yystack_[6].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[4].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1265 "Parser.cpp"
    break;

  case 52: // statement: tLOOP statement
#line 236 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLoop(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1271 "Parser.cpp"
    break;

  case 53: // statement: tLOCK statement
#line 238 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLock(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1277 "Parser.cpp"
    break;

  case 54: // statement: GOTO tIDENTIFIER ';'
#line 240 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateGoto(yystack_[1].value.as < std::string_view > ()); }
#line 1283 "Parser.cpp"
    break;

  case 55: // statement: tIDENTIFIER ':'
#line 242 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLabel(yystack_[1].value.as < std::string_view > ()); }
#line 1289 "Parser.cpp"
    break;

  case 56: // statement: tRETURN ';'
#line 244 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateReturn(nullptr); }
#line 1295 "Parser.cpp"
    break;

  case 57: // statement: tRETURN expression ';'
#line 246 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateReturn(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1301 "Parser.cpp"
    break;

  case 58: // statement: tROLLBACK expression ';'
#line 248 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateRollback(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1307 "Parser.cpp"
    break;

  case 59: // statement: tBREAK ';'
#line 250 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateBreak(); }
#line 1313 "Parser.cpp"
    break;

  case 60: // statement: tCONTINUE ';'
#line 252 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateContinue(); }
#line 1319 "Parser.cpp"
    break;

  case 61: // statement: tHALT '(' ')' ';'
#line 254 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateHalt(); }
#line 1325 "Parser.cpp"
    break;

  case 62: // statement: tYIELD '(' ')' ';'
#line 256 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateYield(); }
#line 1331 "Parser.cpp"
    break;

  case 63: // statement: tCOMPLY '(' ')' ';'
#line 258 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateComply(); }
#line 1337 "Parser.cpp"
    break;

  case 64: // statement: tREFUSE '(' ')' ';'
#line 260 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateRefuse(); }
#line 1343 "Parser.cpp"
    break;

  case 65: // statement: tPRINT '(' arg_calls ')' ';'
#line 262 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreatePrint(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1349 "Parser.cpp"
    break;

  case 66: // statement: tREQUEST '(' expression ',' expression tDC tIDENTIFIER ')' ';'
#line 264 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateRequest(yystack_[6].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[4].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::string_view > ()); }
#line 1355 "Parser.cpp"
    break;

  case 67: // statement: tJOIN '(' expression ',' expression ')' ';'
#line 266 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateJoin(yystack_[4].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1361 "Parser.cpp"
    break;

  case 68: // statement: block
#line 267 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1367 "Parser.cpp"
    break;

  case 69: // statement: declaration ';'
#line 268 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1373 "Parser.cpp"
    break;

  case 70: // statement: expression ';'
#line 269 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1379 "Parser.cpp"
    break;

  case 71: // statement: ';'
#line 271 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 1385 "Parser.cpp"
    break;

  case 72: // statement: error ';'
#line 273 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; yyerrok; yyclearin; }
#line 1391 "Parser.cpp"
    break;

  case 73: // statement: error '}'
#line 275 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; yyerrok; yyclearin; }
#line 1397 "Parser.cpp"
    break;

  case 74: // expression: left_hand '=' expression
#line 279 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1403 "Parser.cpp"
    break;

  case 75: // expression: expression tAND expression
#line 281 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLogicalAnd(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1409 "Parser.cpp"
    break;

  case 76: // expression: expression tOR expression
#line 283 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLogicalOr(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1415 "Parser.cpp"
    break;

  case 77: // expression: expression tAADD expression
#line 285 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateAddAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1421 "Parser.cpp"
    break;

  case 78: // expression: expression tASUB expression
#line 287 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSubAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1427 "Parser.cpp"
    break;

  case 79: // expression: expression tAMUL expression
#line 289 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateMulAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1433 "Parser.cpp"
    break;

  case 80: // expression: expression tADIV expression
#line 291 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDivAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1439 "Parser.cpp"
    break;

  case 81: // expression: expression tAMOD expression
#line 293 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateModAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1445 "Parser.cpp"
    break;

  case 82: // expression: expression tAAND expression
#line 295 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateAndAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1451 "Parser.cpp"
    break;

  case 83: // expression: expression tAOR expression
#line 297 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateOrAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1457 "Parser.cpp"
    break;

  case 84: // expression: expression tAXOR expression
#line 299 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateXorAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1463 "Parser.cpp"
    break;

  case 85: // expression: expression tALSHFT expression
#line 301 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLeftShiftAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1469 "Parser.cpp"
    break;

  case 86: // expression: expression tARSHFT expression
#line 303 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateRightShiftAndAssign(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1475 "Parser.cpp"
    break;

  case 87: // expression: expression '+' expression
#line 305 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateAdd(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1481 "Parser.cpp"
    break;

  case 88: // expression: expression '-' expression
#line 307 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSub(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1487 "Parser.cpp"
    break;

  case 89: // expression: expression '*' expression
#line 309 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateMul(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1493 "Parser.cpp"
    break;

  case 90: // expression: expression '/' expression
#line 311 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDiv(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1499 "Parser.cpp"
    break;

  case 91: // expression: expression '%' expression
#line 313 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateMod(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1505 "Parser.cpp"
    break;

  case 92: // expression: expression tPOW expression
#line 315 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreatePow(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1511 "Parser.cpp"
    break;

  case 93: // expression: expression '&' expression
#line 317 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateAnd(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1517 "Parser.cpp"
    break;

  case 94: // expression: expression '|' expression
#line 319 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateOr(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1523 "Parser.cpp"
    break;

  case 95: // expression: expression '^' expression
#line 321 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateXor(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1529 "Parser.cpp"
    break;

  case 96: // expression: expression tLSHFT expression
#line 323 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLeftShift(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1535 "Parser.cpp"
    break;

  case 97: // expression: expression tRSHFT expression
#line 325 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateRightShift(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1541 "Parser.cpp"
    break;

  case 98: // expression: expression '>' expression
#line 327 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateGT(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1547 "Parser.cpp"
    break;

  case 99: // expression: expression tGE expression
#line 329 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateGE(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1553 "Parser.cpp"
    break;

  case 100: // expression: expression '<' expression
#line 331 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLS(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1559 "Parser.cpp"
    break;

  case 101: // expression: expression tLE expression
#line 333 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateLE(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1565 "Parser.cpp"
    break;

  case 102: // expression: expression tEQ expression
#line 335 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateEQ(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1571 "Parser.cpp"
    break;

  case 103: // expression: expression tNE expression
#line 337 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateNE(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1577 "Parser.cpp"
    break;

  case 104: // expression: tINC expression
#line 339 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateIncrement(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1583 "Parser.cpp"
    break;

  case 105: // expression: tDEC expression
#line 341 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDecrement(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1589 "Parser.cpp"
    break;

  case 106: // expression: expression tINC
#line 343 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateIncrement(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1595 "Parser.cpp"
    break;

  case 107: // expression: expression tDEC
#line 345 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDecrement(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1601 "Parser.cpp"
    break;

  case 108: // expression: expression '?' expression ':' expression
#line 347 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateExpressionIf(yystack_[4].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1607 "Parser.cpp"
    break;

  case 109: // expression: primary
#line 348 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1613 "Parser.cpp"
    break;

  case 110: // primary: '-' expression
#line 352 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateNegative(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1619 "Parser.cpp"
    break;

  case 111: // primary: '+' expression
#line 354 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1625 "Parser.cpp"
    break;

  case 112: // primary: '!' expression
#line 356 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateNot(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1631 "Parser.cpp"
    break;

  case 113: // primary: '~' expression
#line 358 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateComplement1(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1637 "Parser.cpp"
    break;

  case 114: // primary: tSIZEOF '(' variable_type ')'
#line 360 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSizeOf(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1643 "Parser.cpp"
    break;

  case 115: // primary: tIDENTIFIER '(' arg_calls ')'
#line 362 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateCall(yystack_[3].value.as < std::string_view > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1649 "Parser.cpp"
    break;

  case 116: // primary: constant
#line 363 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1655 "Parser.cpp"
    break;

  case 117: // primary: left_hand
#line 364 "Parser.yy"
                                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1661 "Parser.cpp"
    break;

  case 118: // constant: tFALSE
#line 368 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateInteger(0); }
#line 1667 "Parser.cpp"
    break;

  case 119: // constant: tTRUE
#line 370 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateInteger(1); }
#line 1673 "Parser.cpp"
    break;

  case 120: // constant: tPRIORITY
#line 372 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreatePriority(); }
#line 1679 "Parser.cpp"
    break;

  case 121: // constant: tSELF
#line 374 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSelf(); }
#line 1685 "Parser.cpp"
    break;

  case 122: // constant: tSENDER
#line 376 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSender(); }
#line 1691 "Parser.cpp"
    break;

  case 123: // constant: tNIL
#line 378 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateNil(); }
#line 1697 "Parser.cpp"
    break;

  case 124: // constant: tDIGIT
#line 380 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateInteger(yystack_[0].value.as < mana::int_t > ()); }
#line 1703 "Parser.cpp"
    break;

  case 125: // constant: tREAL
#line 382 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateFloat(yystack_[0].value.as < mana::float_t > ()); }
#line 1709 "Parser.cpp"
    break;

  case 126: // constant: tSTRING
#line 384 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateString(yystack_[0].value.as < std::string_view > ()); }
#line 1715 "Parser.cpp"
    break;

  case 127: // left_hand: left_hand '.' tIDENTIFIER
#line 388 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateMemberVariable(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::string_view > ()); }
#line 1721 "Parser.cpp"
    break;

  case 128: // left_hand: left_hand '.' tIDENTIFIER '(' arg_calls ')'
#line 390 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateMemberFunction(yystack_[5].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[3].value.as < std::string_view > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1727 "Parser.cpp"
    break;

  case 129: // left_hand: left_hand '[' expression ']'
#line 392 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateArray(yystack_[3].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1733 "Parser.cpp"
    break;

  case 130: // left_hand: tIDENTIFIER
#line 394 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateIdentifier(yystack_[0].value.as < std::string_view > ()); }
#line 1739 "Parser.cpp"
    break;

  case 131: // left_hand: '(' expression ')'
#line 396 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1745 "Parser.cpp"
    break;

  case 132: // cases: case
#line 399 "Parser.yy"
                          { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1751 "Parser.cpp"
    break;

  case 133: // cases: cases case
#line 401 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::BindCaseNode(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1757 "Parser.cpp"
    break;

  case 134: // case: tCASE expression ':' statements
#line 404 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSwitchCaseNode(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1763 "Parser.cpp"
    break;

  case 135: // case: tDEFAULT ':' statements
#line 406 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateSwitchDefaultNode(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1769 "Parser.cpp"
    break;

  case 136: // arg_calls: %empty
#line 410 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 1775 "Parser.cpp"
    break;

  case 137: // arg_calls: expression
#line 412 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateArgumentNode(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1781 "Parser.cpp"
    break;

  case 138: // arg_calls: expression ',' arg_calls
#line 414 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateArgumentNode(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1787 "Parser.cpp"
    break;

  case 139: // arg_decls: %empty
#line 418 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = nullptr; }
#line 1793 "Parser.cpp"
    break;

  case 140: // arg_decls: variable_decl
#line 420 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclareArgumentNode(yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1799 "Parser.cpp"
    break;

  case 141: // arg_decls: arg_decls ',' variable_decl
#line 422 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclareArgumentNode(yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1805 "Parser.cpp"
    break;

  case 142: // variable_decl: variable_type declarator
#line 427 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclareVariableNode(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (), nullptr); }
#line 1811 "Parser.cpp"
    break;

  case 143: // variable_decl: variable_type declarator '=' expression
#line 429 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclareVariableNode(yystack_[3].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[2].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1817 "Parser.cpp"
    break;

  case 144: // declarator: tIDENTIFIER
#line 434 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclaratorNode(yystack_[0].value.as < std::string_view > ()); }
#line 1823 "Parser.cpp"
    break;

  case 145: // declarator: tIDENTIFIER variable_sizes
#line 436 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = mana::CreateDeclaratorNode(yystack_[1].value.as < std::string_view > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1829 "Parser.cpp"
    break;

  case 146: // variable_sizes: variable_size
#line 439 "Parser.yy"
                  { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > (); }
#line 1835 "Parser.cpp"
    break;

  case 147: // variable_sizes: variable_size variable_sizes
#line 441 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = 	mana::Bind(yystack_[1].value.as < std::shared_ptr<mana::SyntaxNode> > (), yystack_[0].value.as < std::shared_ptr<mana::SyntaxNode> > ()); }
#line 1841 "Parser.cpp"
    break;

  case 148: // variable_size: '[' tDIGIT ']'
#line 444 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = 	mana::CreateVariableSizeNode(yystack_[1].value.as < mana::int_t > ()); }
#line 1847 "Parser.cpp"
    break;

  case 149: // variable_size: '[' tIDENTIFIER ']'
#line 446 "Parser.yy"
                                        { yylhs.value.as < std::shared_ptr<mana::SyntaxNode> > () = 	mana::CreateVariableSizeNode(yystack_[1].value.as < std::string_view > ()); }
#line 1853 "Parser.cpp"
    break;


#line 1857 "Parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
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


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const short Parser::yypact_ninf_ = -235;

  const signed char Parser::yytable_ninf_ = -44;

  const short
  Parser::yypact_[] =
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

  const unsigned char
  Parser::yydefact_[] =
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

  const short
  Parser::yypgoto_[] =
  {
    -235,  -235,   371,  -235,   -54,   117,  -235,   -40,  -235,  -235,
     322,  -235,  -235,     1,  -103,  -234,  -197,   -65,  -235,  -235,
    -235,  -235,    62,  -225,   326,    68,  -235,   351,  -235
  };

  const short
  Parser::yydefgoto_[] =
  {
       0,    16,    17,    18,    57,   241,    20,    76,    77,    21,
      70,    71,    22,    58,   242,   243,   244,   245,   109,   110,
     111,   330,   331,   182,    85,    24,    42,    63,    64
  };

  const short
  Parser::yytable_[] =
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

  const short
  Parser::yycheck_[] =
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

  const unsigned char
  Parser::yystos_[] =
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

  const unsigned char
  Parser::yyr1_[] =
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

  const signed char
  Parser::yyr2_[] =
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


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "tDIGIT", "tREAL",
  "tSTRING", "tIDENTIFIER", "tTYPE", "tALIAS", "tDEFINE", "tUNDEF",
  "tINCLUDE", "tIMPORT", "tNATIVE", "tSTRUCT", "tACTOR", "tACTOR2",
  "tPHANTOM", "tACTION", "tMODULE", "tEXTEND", "tFALSE", "tTRUE",
  "tPRIORITY", "tSELF", "tSENDER", "tNIL", "tREQUEST", "tJOIN", "tBREAK",
  "tCONTINUE", "tCASE", "tDEFAULT", "GOTO", "tHALT", "tLOCK", "tDC", "tDO",
  "tELSE", "tFOR", "tIF", "tLOOP", "tYIELD", "tRETURN", "tROLLBACK",
  "tSWITCH", "tWHILE", "tPRINT", "tSTATIC", "tALLOCATE", "tCOMPLY",
  "tREFUSE", "tAMUL", "tADIV", "tAMOD", "tAADD", "tASUB", "tALSHFT",
  "tARSHFT", "tAAND", "tAOR", "tAXOR", "'='", "'?'", "':'", "tAND", "tOR",
  "tEQ", "tNE", "'>'", "tGE", "tLE", "'<'", "'|'", "'^'", "'&'", "tLSHFT",
  "tRSHFT", "'+'", "'-'", "'*'", "'/'", "'%'", "tPOW", "tSIZEOF", "'!'",
  "'~'", "tUPLUS", "tUMINUS", "tINC", "tDEC", "tUINC", "tUDEC", "';'",
  "'('", "')'", "'{'", "'}'", "','", "'.'", "'['", "']'", "$accept",
  "program", "line", "declarations", "allocate_declarations",
  "declaration", "actor", "actions", "action", "struct", "struct_members",
  "struct_member", "function", "variable_type", "block", "statements",
  "statement", "expression", "primary", "constant", "left_hand", "cases",
  "case", "arg_calls", "arg_decls", "variable_decl", "declarator",
  "variable_sizes", "variable_size", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,    93,    93,   113,   114,   118,   119,   120,   121,   123,
     126,   128,   130,   133,   136,   138,   144,   145,   149,   150,
     152,   154,   156,   158,   160,   162,   166,   168,   170,   175,
     176,   180,   182,   184,   187,   192,   193,   197,   200,   204,
     206,   208,   212,   217,   218,   221,   223,   225,   227,   229,
     231,   233,   235,   237,   239,   241,   243,   245,   247,   249,
     251,   253,   255,   257,   259,   261,   263,   265,   267,   268,
     269,   270,   272,   274,   278,   280,   282,   284,   286,   288,
     290,   292,   294,   296,   298,   300,   302,   304,   306,   308,
     310,   312,   314,   316,   318,   320,   322,   324,   326,   328,
     330,   332,   334,   336,   338,   340,   342,   344,   346,   348,
     351,   353,   355,   357,   359,   361,   363,   364,   367,   369,
     371,   373,   375,   377,   379,   381,   383,   387,   389,   391,
     393,   395,   399,   400,   403,   405,   410,   411,   413,   418,
     419,   421,   426,   428,   433,   435,   439,   440,   443,   445
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
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
    // Last valid token kind.
    const int code_max = 332;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 38 "Parser.yy"
} // mana
#line 2725 "Parser.cpp"

#line 448 "Parser.yy"


#include "ParserDefinition.inl"
