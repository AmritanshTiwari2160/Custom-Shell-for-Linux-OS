%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int yylex(void);
void yyerror(const char *s);

void run_make_file(const char* filename) {
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("make_file");
    } else {
        close(fd);
        printf("File created: %s\n", filename);
    }
}
%}

%union {
    char *str;
}

%token <str> ARG
%token MAKE_FILE
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    MAKE_FILE ARG NEWLINE
        { run_make_file($2); free($2); }
  | MAKE_FILE NEWLINE
        { printf("No file name provided\n"); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        run_make_file(argv[1]);
        return 0;
    }
    return yyparse();
}