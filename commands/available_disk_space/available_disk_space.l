%option noyywrap
%{
#include "available_disk_space.tab.h"
#include <string.h>
%}

%%

"available_disk_space"     { return AVAILABLE_DISK_SPACE; }
[a-zA-Z0-9_./_-]+          {
                               yylval.str = strdup(yytext);
                               return ARG;
                           }
"\n"                       { return NEWLINE; }
[ \t\r]+                   { /* skip whitespace */ }
.                          { /* ignore */ }

%%
