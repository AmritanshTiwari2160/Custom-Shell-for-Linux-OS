%option noyywrap
%{
#include "move_object.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%

"move_object"          { return MOVE_OBJECT; }
[^\n\t ]+              { yylval.str = strdup(yytext); return ARG; }
"\n"                   { return NEWLINE; }
[ \t\r]+               { /* skip whitespace */ }
.                      { /* ignore */ }
%%
