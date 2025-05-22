%option noyywrap
%{
#include "display_content.tab.h"
#include <string.h>
%}

%%

"display_content"          return DISPLAY_CONTENT;
[ \t]+               /* skip whitespace */
\n                   return NEWLINE;
[^\n \t]+            {
                        yylval.str = strdup(yytext);
                        return ARG;
                    }

. { }                    

%%
