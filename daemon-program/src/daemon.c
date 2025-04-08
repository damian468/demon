#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include "daemon.h"
#include "file_scanner.h"
#include "signal_handler.h"

static volatile int keep_running = 1;

void handle_signal(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        keep_running = 0;
    } else if (signal == SIGUSR1) {
        // Trigger an immediate scan
        syslog(LOG_INFO, "Received SIGUSR1, triggering immediate scan.");
        scan_files();
    }
}

int main(int argc, char *argv[]) {
    // Initialize syslog
    openlog("daemon_program", LOG_PID | LOG_CONS, LOG_DAEMON);
    
    // Set up signal handlers
    setup_signal_handlers(handle_signal);

    // Process command-line arguments for filename fragments and verbosity
    if (argc < 2) {
        syslog(LOG_ERR, "No filename fragments provided.");
        exit(EXIT_FAILURE);
    }

    // Start the daemon functionality
    syslog(LOG_INFO, "Daemon started with arguments:");
    for (int i = 1; i < argc; i++) {
        syslog(LOG_INFO, "Fragment: %s", argv[i]);
    }

    while (keep_running) {
        // Periodically scan the file system
        scan_files(argv + 1, argc - 1);
        sleep(10); // Sleep for a specified interval
    }

    // Cleanup and exit
    syslog(LOG_INFO, "Daemon stopping.");
    closelog();
    return 0;
}