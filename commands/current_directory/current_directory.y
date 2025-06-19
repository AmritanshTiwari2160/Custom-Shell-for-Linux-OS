%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <limits.h>

int yylex(void);
void yyerror(const char *s);

// Direct syscall wrapper for getcwd
void run_current_directory() {
    char cwd[PATH_MAX];
    long ret = syscall(SYS_getcwd, cwd, sizeof(cwd));
    if (ret < 0) {
        fprintf(stderr, "current_directory: error: %s\n", strerror(errno));
    } else {
        // syscall returns length on success
        printf("%s\n", cwd);
    }
}
%}

%token CURRENT_DIRECTORY
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    CURRENT_DIRECTORY NEWLINE
        { run_current_directory(); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    // also allow CLI usage without newline
    if (argc > 1 && strcmp(argv[1], "current_directory") == 0) {
        run_current_directory();
        return 0;
    }
    return yyparse();
}