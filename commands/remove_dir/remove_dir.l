%option noyywrap
%{
#include "remove_dir.tab.h"
#include <string.h>
%}

%%

"remove_dir"          return REMOVE_DIR;
[ \t]+               /* skip whitespace */
\n                   return NEWLINE;
[^\n \t]+            {
                        yylval.str = strdup(yytext);
                        return ARG;
                    }

. { }                    

%%
