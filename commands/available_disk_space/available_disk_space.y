%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/statfs.h>

int yylex(void);
void yyerror(const char *s);

// Direct syscall to get FS stats
void run_available_disk_space(const char *path) {
    struct statfs fs;
    long ret = syscall(SYS_statfs, path, &fs);
    if (ret < 0) {
        fprintf(stderr,
                "available_disk_space: cannot statfs '%s': %s\n",
                path, strerror(errno));
        return;
    }
    unsigned long long total = (unsigned long long)fs.f_blocks * fs.f_bsize;
    unsigned long long avail = (unsigned long long)fs.f_bavail * fs.f_bsize;
    printf("Total System Disk Space: %llu bytes\nAvailable Disk Space: %llu bytes\n",
       total, avail);
}
%}

%union {
    char *str;
}

%token AVAILABLE_DISK_SPACE
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    AVAILABLE_DISK_SPACE NEWLINE
        { run_available_disk_space("."); }
  | AVAILABLE_DISK_SPACE ARG NEWLINE
        { run_available_disk_space($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        run_available_disk_space(argv[1]);
        return 0;
    }
    return yyparse();
}
