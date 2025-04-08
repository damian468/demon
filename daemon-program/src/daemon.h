#ifndef DAEMON_H
#define DAEMON_H

#include <stdbool.h>

// Function to start the daemon
void start_daemon(int argc, char *argv[]);

// Function to manage the sleep/wake cycle of the daemon
void manage_sleep_cycle(bool *running);

#endif // DAEMON_H