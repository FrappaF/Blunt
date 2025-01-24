#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

extern int LOGGING_ENABLED;

#define LOG_PRINT(...)                    \
    do                                    \
    {                                     \
        if (LOGGING_ENABLED)              \
            fprintf(stdout, __VA_ARGS__); \
    } while (0)

void log_error(const char *format, ...);

#endif // LOGGER_H