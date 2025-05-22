%option noyywrap
%{
#include "my_editor.tab.h"
#include <string.h>
%}

%%
"my_editor"            { return MY_EDITOR; }
[a-zA-Z0-9_./_-]+      {
                          yylval.str = strdup(yytext);
                          return ARG;
                       }
"\n"                   { return NEWLINE; }
[ \t\r]+               { /* skip whitespace */ }
.                      { /* ignore everything else */ }
%%
