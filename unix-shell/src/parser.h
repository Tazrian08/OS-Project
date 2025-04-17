#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARG_LENGTH 100
#define MAX_NUM_ARGS 10

typedef struct Command {
    char *args[MAX_NUM_ARGS];
    int num_args;
    bool background; // Indicates if the command should run in the background
    char *input_redirect; // Input redirection file
    char *output_redirect; // Output redirection file
    bool append; // Indicates if output should be appended
} Command;

Command *parse_command(char *input);
void free_command(Command *cmd);

#endif // PARSER_H