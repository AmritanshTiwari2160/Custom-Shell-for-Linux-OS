%{
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int yylex(void);
void yyerror(const char *s);

// syscall wrapper to rename (move) src → dst
void run_move_object(const char *src, const char *dst) {
    if (rename(src, dst) < 0) {
        fprintf(stderr, "move_object: cannot move '%s' to '%s': %s\n",
                src, dst, strerror(errno));
    }
}
%}

%union {
    char *str;
}

%token MOVE_OBJECT
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    MOVE_OBJECT ARG ARG NEWLINE
        { run_move_object($2, $3); free($2); free($3); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc == 3) {
        run_move_object(argv[1], argv[2]);
        return 0;
    }
    return yyparse();
}