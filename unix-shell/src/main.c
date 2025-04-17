
#include "shell.h"

int main(void) {
    // Setup signal handlers
    setup_signals();
    
    // Main shell loop
    shell_loop();
    
    return EXIT_SUCCESS;
}