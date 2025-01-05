#include "include/logger.h"
#include <stdarg.h>

int LOGGING_ENABLED = 0;

void log_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}