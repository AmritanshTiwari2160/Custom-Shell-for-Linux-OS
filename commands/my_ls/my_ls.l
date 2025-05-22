%option noyywrap
%{
#include "my_ls.tab.h"
#include <string.h>
%}

%%

"my_ls"                       { return MY_LS; }
[a-zA-Z0-9_./]+               {
                                 yylval.str = strdup(yytext);
                                 return ARG;
                              }
"\n"                          { return NEWLINE; }
[ \t\r]+                      { /* skip whitespace */ }
.                             { /* ignore everything else */ }

%%
