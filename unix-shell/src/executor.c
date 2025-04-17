#include "shell.h"

int is_builtin(const char *cmd) {
    return (strcmp(cmd, "history") == 0 || 
            strcmp(cmd, "exit") == 0 || 
            strcmp(cmd, "cd") == 0);
}

int execute_builtin(Command *command) {
    if (strcmp(command->args[0], "history") == 0) {
        show_history();
        return 1;
    }
    else if (strcmp(command->args[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(command->args[0], "cd") == 0) {
        if (command->args[1] == NULL) {
            // If no argument is provided, change to HOME directory
            char *home = getenv("HOME");
            if (home == NULL || chdir(home) < 0) {
                perror("cd");
                return 1;
            }
        } else {
            if (chdir(command->args[1]) < 0) {
                perror("cd");
                return 1;
            }
        }
        return 1;
    }
    return 0;
}

int execute_commands(Command **commands, int num_commands) {
    int i;
    int prev_pipe_read = -1;

    for (i = 0; i < num_commands; i++) {
        int pipe_fds[2];
        
        if (commands[i]->pipe_to_next && pipe(pipe_fds) < 0) {
            perror("pipe");
            return -1;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        }

        if (pid == 0) {  // Child process
            // Add this check before execvp
            if (is_builtin(commands[i]->args[0])) {
                execute_builtin(commands[i]);
                exit(EXIT_SUCCESS);
            }

            // Handle input redirection
            if (commands[i]->input_file) {
                int fd = open(commands[i]->input_file, O_RDONLY);
                if (fd < 0) {
                    perror("open input");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (prev_pipe_read != -1) {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }

            // Handle output redirection
            if (commands[i]->output_file) {
                int flags = O_WRONLY | O_CREAT;
                if (commands[i]->append_output)
                    flags |= O_APPEND;
                else
                    flags |= O_TRUNC;
                
                int fd = open(commands[i]->output_file, flags, 0644);
                if (fd < 0) {
                    perror("open output");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (commands[i]->pipe_to_next) {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }

            execvp(commands[i]->args[0], commands[i]->args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        // Parent process
        if (prev_pipe_read != -1)
            close(prev_pipe_read);

        if (commands[i]->pipe_to_next) {
            close(pipe_fds[1]);
            prev_pipe_read = pipe_fds[0];
        } else {
            prev_pipe_read = -1;
        }

        if (!commands[i]->pipe_to_next) {
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}