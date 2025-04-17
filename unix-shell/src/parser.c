#include "shell.h"

Command **parse_line(char *line, int *num_commands) {
    char *commands_str = strdup(line);
    char *command_token;
    Command **commands = malloc(MAX_ARGS * sizeof(Command*));
    *num_commands = 0;

    // Split by semicolon first
    command_token = strtok(commands_str, ";");
    while (command_token) {
        Command *cmd = malloc(sizeof(Command));
        cmd->args = malloc(MAX_ARGS * sizeof(char*));
        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->append_output = 0;
        cmd->pipe_to_next = 0;

        char *arg = strtok(command_token, " \t\r\n");
        int arg_count = 0;

        while (arg) {
            if (strcmp(arg, "<") == 0) {
                arg = strtok(NULL, " \t\r\n");
                if (arg) cmd->input_file = strdup(arg);
            }
            else if (strcmp(arg, ">") == 0) {
                arg = strtok(NULL, " \t\r\n");
                if (arg) cmd->output_file = strdup(arg);
            }
            else if (strcmp(arg, ">>") == 0) {
                arg = strtok(NULL, " \t\r\n");
                if (arg) {
                    cmd->output_file = strdup(arg);
                    cmd->append_output = 1;
                }
            }
            else if (strcmp(arg, "|") == 0) {
                cmd->pipe_to_next = 1;
                break;
            }
            else {
                cmd->args[arg_count++] = strdup(arg);
            }
            arg = strtok(NULL, " \t\r\n");
        }
        cmd->args[arg_count] = NULL;
        commands[(*num_commands)++] = cmd;

        command_token = strtok(NULL, ";");
    }

    free(commands_str);
    return commands;
}