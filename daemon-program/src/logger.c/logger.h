#ifndef LOGGER_H
#define LOGGER_H

#include <syslog.h>

typedef enum {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_DEBUG
} LogLevel;

void init_logger(const char *ident);
void log_message(LogLevel level, const char *message);
void close_logger();

#endif // LOGGER_H