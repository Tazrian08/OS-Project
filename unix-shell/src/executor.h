#ifndef EXECUTOR_H
#define EXECUTOR_H

void execute_command(char *command);
void execute_pipeline(char **commands, int num_commands);
void redirect_io(char *input_file, char *output_file, int append);
void handle_signal(int signal);

#endif // EXECUTOR_H