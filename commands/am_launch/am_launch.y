%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

// Launch the given application via system()
void run_am_launch(const char *app) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "%s &", app);
    if (system(cmd) == -1) {
        perror("am_launch");
    }
}
%}

%union {
    char *str;
}

%token AM_LAUNCH
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    AM_LAUNCH NEWLINE
        { 
          fprintf(stderr, "Usage: am_launch <application>\n"); 
        }
  | AM_LAUNCH ARG NEWLINE
        { 
          run_am_launch($2); 
          free($2); 
        }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

// Allow CLI invocation `am_launch <app>`
int main(int argc, char **argv) {
    if (argc > 1) {
        run_am_launch(argv[1]);
        return 0;
    }
    return yyparse();
}
