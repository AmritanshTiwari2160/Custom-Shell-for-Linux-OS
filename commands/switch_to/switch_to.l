%option noyywrap
%{
#include "switch_to.tab.h"
#include <string.h>
%}

%%

"switch_to"         { return SWITCH_TO; }
[a-zA-Z0-9_./_-]+   {
                        yylval.str = strdup(yytext);
                        return ARG;
                    }
"\n"                { return NEWLINE; }
[ \t\r]+            { /* skip */ }
.                   { /* ignore */ }
%%
