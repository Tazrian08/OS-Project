#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <sys/types.h>
#include <string.h>

// Flag to track if a command is currently running
extern volatile sig_atomic_t running_cmd;

// Function prototypes
void handle_sigint(int sig);
void setup_signals(void);
void set_running_cmd(int status);

#endif // SIGNALS_H