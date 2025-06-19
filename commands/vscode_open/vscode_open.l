%option noyywrap
%{
#include "vscode_open.tab.h"
#include <string.h>
%}

%%

"vscode_open"            { return VSCODE_OPEN; }
[^\n\t ]+                {
                             yylval.str = strdup(yytext);
                             return ARG;
                         }
"\n"                      { return NEWLINE; }
[ \t\r]+                  { /* skip whitespace */ }
.                         { /* ignore anything else */ }

%%
