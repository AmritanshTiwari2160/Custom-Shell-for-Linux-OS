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

// syscall wrapper for chdir
void run_switch_to(const char *path) {
    long ret = syscall(SYS_chdir, path);
    if (ret == 0) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd))) {
            printf("Directory changed to: %s\n", cwd);
        } else {
            perror("getcwd");
        }
    } else {
        fprintf(stderr, "switch_to: cannot change to '%s': %s\n",
                path, strerror(errno));
    }
}
%}

%union {
    char *str;
}

%token SWITCH_TO
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    SWITCH_TO NEWLINE
        { run_switch_to("."); }
  | SWITCH_TO ARG NEWLINE
        { run_switch_to($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        run_switch_to(argv[1]);
        return 0;
    }
    return yyparse();
}
