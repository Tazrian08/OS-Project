#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

#define BUFFER_SIZE 1024

char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t chars_read;

    chars_read = getline(&line, &bufsize, stdin);
    
    if (chars_read == -1) {
        if (feof(stdin)) {
            printf("\n");
            exit(EXIT_SUCCESS);
        } else {
            perror("getline");
            exit(EXIT_FAILURE);
        }
    }

    // Remove trailing newline
    if (chars_read > 0 && line[chars_read - 1] == '\n') {
        line[chars_read - 1] = '\0';
    }

    return line;
}

#include "shell.h"

void shell_loop(void) {
    char *line;
    Command **commands;
    int num_commands;

    while (1) {
        printf("sh> ");
        fflush(stdout);

        line = read_line();
        if (!line) continue;

        // Handle built-in exit command
        if (strcmp(line, "exit") == 0) {
            free(line);
            break;
        }

        commands = parse_line(line, &num_commands);
        if (commands) {
            execute_commands(commands, num_commands);
            // Free commands
            for (int i = 0; i < num_commands; i++) {
                if (commands[i]->args) {
                    for (int j = 0; commands[i]->args[j]; j++)
                        free(commands[i]->args[j]);
                    free(commands[i]->args);
                }
                free(commands[i]->input_file);
                free(commands[i]->output_file);
                free(commands[i]);
            }
            free(commands);
        }
        free(line);
    }
}