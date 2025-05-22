%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int yylex(void);
void yyerror(const char *s);

void remove_directory(const char* path) {
    if (rmdir(path) == 0) {
        printf("Directory removed: %s\n", path);
    } else {
        perror("remove_dir");
    }
}
%}

%union {
    char *str;
}

%token <str> ARG
%token REMOVE_DIR
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    REMOVE_DIR ARG NEWLINE
        { remove_directory($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        remove_directory(argv[1]);
        return 0;
    }
    return yyparse();
}