%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

void show_file(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("display_content");
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}
%}

%union {
    char *str;
}

%token <str> ARG
%token DISPLAY_CONTENT
%token NEWLINE

%%

input:
    | input line
;

line:
    DISPLAY_CONTENT ARG NEWLINE
        { show_file($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        show_file(argv[1]);
        return 0;
    }
    return yyparse();
}