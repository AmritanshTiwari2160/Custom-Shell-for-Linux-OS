%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int yylex(void);
void yyerror(const char *s);

// system-call helper to launch VS Code
void run_vscode_open(const char *path) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "code \"%s\" &", path);
    system(cmd);
}
%}

%union {
    char *str;
}

%token VSCODE_OPEN
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    VSCODE_OPEN NEWLINE
        { run_vscode_open("."); }
  | VSCODE_OPEN ARG NEWLINE
        { run_vscode_open($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

// Allow `vscode_open <file>` on CLI too
int main(int argc, char **argv) {
    if (argc > 1) {
        run_vscode_open(argv[1]);
        return 0;
    }
    return yyparse();
}
