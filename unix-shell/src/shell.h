#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64
#define HISTORY_SIZE 1000

// Command structure
typedef struct {
    char **args;           // Command arguments
    char *input_file;      // Input redirection file
    char *output_file;     // Output redirection file
    int append_output;     // Flag for append output (>>)
    int pipe_to_next;      // Flag for piping to next command
} Command;

// Function declarations
void shell_loop(void);
char *read_line(void);
Command **parse_line(char *line, int *num_commands);
int execute_commands(Command **commands, int num_commands);
void add_to_history(const char *command);
void show_history(void);
void setup_signals(void);

int is_builtin(const char *cmd);
int execute_builtin(Command *command);

#endif