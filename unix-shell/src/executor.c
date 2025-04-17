#include "shell.h"

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