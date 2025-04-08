#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <signal.h>

void setup_signal_handlers(void);
void handle_sigusr1(int signo);

#endif // SIGNAL_HANDLER_H