%option noyywrap
%{
#include "make_file.tab.h"
#include <string.h>
%}

%%

"make_file"          return MAKE_FILE;
[ \t]+               /* skip whitespace */
\n                   return NEWLINE;
[^\n \t]+            {
                        yylval.str = strdup(yytext);
                        return ARG;
                    }

.                    /* skip invalid characters */

%%