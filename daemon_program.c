#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define DEFAULT_SCAN_INTERVAL 60

// Global variables
volatile sig_atomic_t trigger_scan = 0;

// Signal handler for SIGUSR1
void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        trigger_scan = 1;
        syslog(LOG_INFO, "Received SIGUSR1, triggering immediate scan.");
    }
}

// Function to log the current time
void log_time() {
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    syslog(LOG_INFO, "Current time: %s", time_str);
}

// Recursive file scanner
void scan_directory(const char *dir_path, const char *pattern) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        syslog(LOG_ERR, "Cannot open directory: %s", dir_path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recursively scan subdirectories
                scan_directory(full_path, pattern);
            } else if (strstr(entry->d_name, pattern)) {
                // Log matching files
                log_time();
                syslog(LOG_INFO, "Found matching file: %s (pattern: %s)", full_path, pattern);
            }
        } else {
            syslog(LOG_ERR, "Cannot access file: %s", full_path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-v] <filename_fragment> [scan_interval]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int verbose = 0;
    const char *pattern = NULL;
    int scan_interval = DEFAULT_SCAN_INTERVAL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (isdigit(argv[i][0])) {
            scan_interval = atoi(argv[i]);
        } else {
            pattern = argv[i];
        }
    }

    if (!pattern) {
        fprintf(stderr, "Error: No filename fragment provided.\n");
        exit(EXIT_FAILURE);
    }

    // Open syslog
    openlog("daemon_program", LOG_PID | LOG_CONS, LOG_USER);

    if (verbose) {
        syslog(LOG_INFO, "Verbose mode enabled.");
    }

    // Set up signal handling
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    // Daemonize the process
    if (daemon(0, 0) == -1) {
        perror("daemon");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Daemon started. Scanning every %d seconds.", scan_interval);

    // Main loop
    while (1) {
        if (trigger_scan) {
            trigger_scan = 0;
        } else {
            sleep(scan_interval);
        }

        syslog(LOG_INFO, "Starting scan...");
        scan_directory("/", pattern);
        syslog(LOG_INFO, "Scan completed.");
    }

    closelog();
    return 0;
}