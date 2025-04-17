#ifndef HISTORY_H
#define HISTORY_H

#define MAX_HISTORY 100

void add_to_history(const char *command);
void print_history();
void clear_history();

#endif // HISTORY_H