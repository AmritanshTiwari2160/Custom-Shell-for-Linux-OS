%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>

int yylex(void);
void yyerror(const char *s);

// Direct syscall to create a directory
void run_make_directory(const char *path) {
    long ret = syscall(SYS_mkdir, path, 0755);
    if (ret == 0) {
        printf("Directory '%s' created\n", path);
    } else {
        fprintf(stderr,
                "make_directory: cannot create '%s': %s\n",
                path, strerror(errno));
    }
}
%}

%union {
    char *str;
}

%token <str> ARG
%token MAKE_DIRECTORY
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    MAKE_DIRECTORY NEWLINE
        { run_make_directory("."); }
  | MAKE_DIRECTORY ARG NEWLINE
        { run_make_directory($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        run_make_directory(argv[1]);
        return 0;
    }
    return yyparse();
}