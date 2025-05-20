#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

void trim_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

void start_shell() {
    char input[1024];

    while (1) {
        printf("AmriShell> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break; // EOF (Ctrl+D)
        }

        trim_newline(input);

        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strncmp(input, "my_ls", 5) == 0) {
            char command[1060];
            snprintf(command, sizeof(command), "./commands/my_ls/my_ls_exec %s", input);
            system(command);
        } else {
            printf("Unknown command: %s\n", input);
        }
    }
}