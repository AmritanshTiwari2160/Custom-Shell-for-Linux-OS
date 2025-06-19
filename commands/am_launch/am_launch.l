%option noyywrap
%{
#include "am_launch.tab.h"
#include <string.h>
%}

%%

"am_launch"               { return AM_LAUNCH; }
[^\n\t ]+                 {
                              yylval.str = strdup(yytext);
                              return ARG;
                          }
"\n"                      { return NEWLINE; }
[ \t\r]+                  { /* skip whitespace */ }
.                         { /* ignore anything else */ }

%%
