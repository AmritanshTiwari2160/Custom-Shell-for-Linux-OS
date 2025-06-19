%option noyywrap
%{
#include "current_directory.tab.h"
%}

%%

"current_directory"  { return CURRENT_DIRECTORY; }
"\n"                 { return NEWLINE; }
[ \t\r]+             { /* skip whitespace */ }
.                    { /* ignore everything else */ }
%%