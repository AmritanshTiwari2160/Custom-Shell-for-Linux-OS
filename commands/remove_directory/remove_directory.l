%option noyywrap
%{
#include "remove_directory.tab.h"
#include <string.h>
%}

%%

"remove_directory"          return REMOVE_DIRECTORY;
[ \t]+               /* skip whitespace */
\n                   return NEWLINE;
[^\n \t]+            {
                        yylval.str = strdup(yytext);
                        return ARG;
                    }

. { }                    

%%
