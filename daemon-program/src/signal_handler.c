#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "signal_handler.h"
#include "logger.h"

void handle_sigusr1(int sig) {
    log_message("Received SIGUSR1: Triggering immediate scan.", LOG_INFO);
    // Code to trigger the file scan would go here
}

void setup_signal_handlers() {
    struct sigaction sa;
    sa.sa_handler = handle_sigusr1;
    sa.sa_flags = 0; // No special flags
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}