/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once

int yynerrs = 0;

extern int yylex(mana::Parser::value_type* yylval);
