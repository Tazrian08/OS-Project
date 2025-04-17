#include "shell.h"

Command **parse_line(char *line, int *num_commands) {
    char *saveptr1, *saveptr2, *saveptr3;
    char *cmd_str, *token;
    Command **commands = malloc(MAX_ARGS * sizeof(Command*));
    *num_commands = 0;
    int prev_was_and = 0;

    // Split by semicolon first
    cmd_str = strtok_r(line, ";", &saveptr1);
    while (cmd_str != NULL) {
        // Skip leading whitespace
        while (*cmd_str == ' ' || *cmd_str == '\t') cmd_str++;
        
        if (strlen(cmd_str) > 0) {
            // Split by && operator
            char *and_cmd = strtok_r(cmd_str, "&", &saveptr2);
            while (and_cmd != NULL) {
                // Skip leading whitespace
                while (*and_cmd == ' ' || *and_cmd == '\t') and_cmd++;
                
                if (strlen(and_cmd) > 0) {
                    Command *cmd = malloc(sizeof(Command));
                    cmd->args = malloc(MAX_ARGS * sizeof(char*));
                    cmd->input_file = NULL;
                    cmd->output_file = NULL;
                    cmd->append_output = 0;
                    cmd->pipe_to_next = 0;
                    cmd->wait_for_previous = prev_was_and;  // Set based on previous &&
                    
                    int arg_count = 0;
                    
                    // Parse individual command tokens
                    token = strtok_r(and_cmd, " \t", &saveptr3);
                    while (token != NULL) {
                        if (strcmp(token, "<") == 0) {
                            // Input redirection
                            token = strtok_r(NULL, " \t", &saveptr3);
                            if (token != NULL) {
                                cmd->input_file = strdup(token);
                            }
                        }
                        else if (strcmp(token, ">") == 0) {
                            // Output redirection
                            token = strtok_r(NULL, " \t", &saveptr3);
                            if (token != NULL) {
                                cmd->output_file = strdup(token);
                                cmd->append_output = 0;
                            }
                        }
                        else if (strcmp(token, ">>") == 0) {
                            // Append output redirection
                            token = strtok_r(NULL, " \t", &saveptr3);
                            if (token != NULL) {
                                cmd->output_file = strdup(token);
                                cmd->append_output = 1;
                            }
                        }
                        else {
                            cmd->args[arg_count++] = strdup(token);
                        }
                        token = strtok_r(NULL, " \t", &saveptr3);
                    }
                    cmd->args[arg_count] = NULL;
                    
                    commands[(*num_commands)++] = cmd;
                }
                
                // Check if next token starts with &
                char *next = strtok_r(NULL, "&", &saveptr2);
                prev_was_and = (next != NULL);
                and_cmd = next;
            }
            prev_was_and = 0;  // Reset at semicolon boundary
        }
        
        cmd_str = strtok_r(NULL, ";", &saveptr1);
    }

    return commands;
}