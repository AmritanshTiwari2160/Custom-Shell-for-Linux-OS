%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void run_my_editor(const char *path);
int yylex(void);
void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}
%}

%union {
    char *str;
}

%token MY_EDITOR
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    MY_EDITOR NEWLINE
      { 
        fprintf(stderr, "Usage: my_editor <filename>\n"); 
      }
  | MY_EDITOR ARG NEWLINE
      { 
        run_my_editor($2); 
        free($2);
      }
;

%%

void run_my_editor(const char *path) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("my_editor: fork");
        return;
    }
    if (pid == 0) {
        // Child: exec gedit (replace with nano or your editor if you like)
        execlp("gedit", "gedit", path, (char *)NULL);
        // If execlp fails:
        fprintf(stderr, "my_editor: cannot launch editor '%s': %s\n",
                path, strerror(errno));
        _exit(1);
    } else {
        // Parent: wait for editor to exit
        int status;
        waitpid(pid, &status, 0);
    }
}

int main(int argc, char **argv) {
    if (argc > 1) {
        run_my_editor(argv[1]);
        return 0;
    }
    return yyparse();
}