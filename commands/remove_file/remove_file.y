%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int yylex(void);
void yyerror(const char *s);

// Actual removal logic
void run_remove_file(const char *path) {
    if (unlink(path) == 0) {
        printf("File '%s' removed\n", path);
    } else {
        fprintf(stderr,
                "remove_file: cannot remove '%s': %s\n",
                path, strerror(errno));
    }
}
%}

%union {
    char *str;
}

%token REMOVE_FILE
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    REMOVE_FILE NEWLINE
        { run_remove_file("."); }
  | REMOVE_FILE ARG NEWLINE
        { run_remove_file($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

// Allow `remove_file <path>` on the CLI too
int main(int argc, char **argv) {
    if (argc > 1) {
        run_remove_file(argv[1]);
        return 0;
    }
    return yyparse();
}
