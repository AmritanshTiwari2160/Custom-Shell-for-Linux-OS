%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
extern void yy_scan_string(const char *);
%}

%union {
    int val;
}

%token <val> NUMBER
%token PLUS MINUS MUL DIV LPAREN RPAREN EOL
%token CALC_EXPR

%type <val> expression

%left PLUS MINUS
%left MUL DIV
%right UMINUS

%%

input:
    /* empty */
  | input line
;

line:
    /* GUI form: command + expression + newline */
    CALC_EXPR expression EOL    { printf("%d\n", $2); }

    /* GUI form without trailing newline */
  | CALC_EXPR expression        { printf("%d\n", $2); }

    /* CLI form: expression + newline */
  | expression EOL              { printf("%d\n", $1); }

    /* CLI form: bare expression */
  | expression                  { printf("%d\n", $1); }
;

expression:
      NUMBER                          { $$ = $1; }
    | expression PLUS expression      { $$ = $1 + $3; }
    | expression MINUS expression     { $$ = $1 - $3; }
    | expression MUL expression       { $$ = $1 * $3; }
    | expression DIV expression       {
                                          if ($3 == 0) {
                                            fprintf(stderr,"Error: division by zero\n");
                                            $$ = 0;
                                          } else $$ = $1 / $3;
                                        }
    | MINUS expression  %prec UMINUS   { $$ = -$2; }
    | LPAREN expression RPAREN         { $$ = $2; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        char buf[512];
        snprintf(buf, sizeof(buf), "%s\n", argv[1]);
        yy_scan_string(buf);
        yyparse();
        return 0;
    }
    return yyparse();
}