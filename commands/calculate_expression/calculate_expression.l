%option noyywrap
%{
#include "calculate_expression.tab.h"
#include <stdlib.h>
%}

%%

"calculate_expression"    { return CALC_EXPR; }
[0-9]+                    { yylval.val = atoi(yytext); return NUMBER; }
[ \t]+                    { /* skip whitespace */ }
"+"                       { return PLUS; }
"-"                       { return MINUS; }
"*"                       { return MUL; }
"/"                       { return DIV; }
"("                       { return LPAREN; }
")"                       { return RPAREN; }
"\n"                      { return EOL; }
.                         { /* ignore anything else */ }
%%