#include "shell.h"

static char *history[HISTORY_SIZE];
static int history_count = 0;

void add_to_history(const char *command) {
    if (!command || strlen(command) == 0) return;
    
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(command);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i-1] = history[i];
        }
        history[HISTORY_SIZE-1] = strdup(command);
    }
}

void show_history(void) {
    for (int i = 0; i < history_count; i++) {
        printf("%d  %s\n", i + 1, history[i]);
    }
}