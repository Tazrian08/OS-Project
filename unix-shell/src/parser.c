#include "shell.h"

Command **parse_line(char *line, int *num_commands) {
    char *saveptr1, *saveptr2;
    char *cmd_str, *token;
    Command **commands = malloc(MAX_ARGS * sizeof(Command*));
    *num_commands = 0;

    // Split by semicolon first
    cmd_str = strtok_r(line, ";", &saveptr1);
    while (cmd_str != NULL) {
        // Skip leading whitespace
        while (*cmd_str == ' ' || *cmd_str == '\t') cmd_str++;
        
        if (strlen(cmd_str) > 0) {
            Command *cmd = malloc(sizeof(Command));
            cmd->args = malloc(MAX_ARGS * sizeof(char*));
            cmd->input_file = NULL;
            cmd->output_file = NULL;
            cmd->append_output = 0;
            cmd->pipe_to_next = 0;
            cmd->wait_for_previous = 0;
            
            int arg_count = 0;
            
            // Parse individual command tokens
            token = strtok_r(cmd_str, " \t", &saveptr2);
            while (token != NULL) {
                cmd->args[arg_count++] = strdup(token);
                token = strtok_r(NULL, " \t", &saveptr2);
            }
            cmd->args[arg_count] = NULL;
            
            commands[(*num_commands)++] = cmd;
        }
        
        cmd_str = strtok_r(NULL, ";", &saveptr1);
    }

    return commands;
}