%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

int yylex(void);
void yyerror(const char *s);

void run_ls(const char* path) {
    DIR *d = opendir(path);
    if (!d) {
        perror("my_ls");
        return;
    }
    struct dirent *ent;
    while ((ent = readdir(d))) {
        // Skip “.” and “..”
        if (strcmp(ent->d_name, ".") == 0 ||
            strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        printf("%s\n", ent->d_name);
    }
    closedir(d);
}
%}

%union {
    char *str;
}

%token <str> ARG
%token MY_LS
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    MY_LS NEWLINE
        { run_ls("."); }
  | MY_LS ARG NEWLINE
        { run_ls($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

// If invoked with an argument, treat it like `my_ls <arg>`;
// otherwise fall back to parsing stdin.
int main(int argc, char **argv) {
    if (argc > 1) {
        run_ls(argv[1]);
        return 0;
    }
    return yyparse();
}