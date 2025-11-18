%option noyywrap
%{
#include "parser.tab.h"
#include <cstdlib>
#include <cstdio>
%}

DIGIT   [0-9]

%%
[ \t\r]+           ;
\n                 return '\n';
{DIGIT}+           {
                      yylval.ival = std::atoi(yytext);
                      return NUMBER;
                    }
[()+\-*/]          return yytext[0];
.                  {
                      std::fprintf(stderr, "unknown token: %s\n", yytext);
                    }
%%
