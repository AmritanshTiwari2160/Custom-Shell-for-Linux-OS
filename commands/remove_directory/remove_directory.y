%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

// forward decl
int yylex(void);
void yyerror(const char *s);

// Recursively remove a path (file or directory)
int remove_path(const char *path) {
    struct stat st;
    if (lstat(path, &st) < 0) {
        fprintf(stderr, "remove_directory: cannot stat '%s': %s\n",
                path, strerror(errno));
        return -1;
    }

    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir(path);
        if (!d) {
            fprintf(stderr, "remove_directory: cannot open '%s': %s\n",
                    path, strerror(errno));
            return -1;
        }
        struct dirent *ent;
        char child[PATH_MAX];
        while ((ent = readdir(d))) {
            // skip "." and ".."
            if (strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name, "..")==0)
                continue;
            snprintf(child, sizeof(child), "%s/%s", path, ent->d_name);
            if (remove_path(child) < 0) {
                closedir(d);
                return -1;
            }
        }
        closedir(d);
        // now directory is empty—remove it
        if (rmdir(path) < 0) {
            fprintf(stderr, "remove_directory: cannot remove directory '%s': %s\n",
                    path, strerror(errno));
            return -1;
        }
    } else {
        // plain file or symlink: unlink
        if (unlink(path) < 0) {
            fprintf(stderr, "remove_directory: cannot remove file '%s': %s\n",
                    path, strerror(errno));
            return -1;
        }
    }
    return 0;
}

// Action entry-point
void run_remove_dir(const char *path) {
    if (remove_path(path) == 0) {
        printf("Removed '%s'\n", path);
    } else {
        // errors already printed in remove_path()
    }
}
%}

%union {
    char *str;
}

%token REMOVE_DIRECTORY
%token <str> ARG
%token NEWLINE

%%

input:
    /* empty */
  | input line
;

line:
    REMOVE_DIRECTORY NEWLINE
        { run_remove_dir("."); }
  | REMOVE_DIRECTORY ARG NEWLINE
        { run_remove_dir($2); free($2); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        run_remove_dir(argv[1]);
        return 0;
    }
    return yyparse();
}