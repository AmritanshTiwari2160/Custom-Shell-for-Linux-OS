%option noyywrap
%{
#include "remove_file.tab.h"
#include <string.h>
%}

%%

"remove_file"           { return REMOVE_FILE; }
[a-zA-Z0-9_./_-]+       {
                            yylval.str = strdup(yytext);
                            return ARG;
                        }
"\n"                    { return NEWLINE; }
[ \t\r]+                { /* skip whitespace */ }
.                       { /* ignore */ }
%%