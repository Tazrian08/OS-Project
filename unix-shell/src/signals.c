#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "shell.h"
#include "signals.h"

// Define the global variable
volatile sig_atomic_t running_cmd = 0;

void set_running_cmd(int status) {
    running_cmd = status;
}

void handle_sigint(int sig) {
    (void)sig;  // Avoid unused parameter warning
    if (running_cmd) {
        printf("\n");
    } else {
        printf("\nsh> ");
    }
    fflush(stdout);
}

void setup_signals(void) {
    struct sigaction sa;
    
    // Initialize sigaction struct
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Ignore SIGQUIT and SIGTSTP
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}