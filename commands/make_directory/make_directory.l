%option noyywrap
%{
#include "make_directory.tab.h"
%}

%%
"make_directory"          { return MAKE_DIRECTORY; }
[a-zA-Z0-9_./-]+    { yylval.str = strdup(yytext); return ARG; }
[ \t\n]+            { /* skip whitespace */ }
.                   { return yytext[0]; }
%%
