#include <stdarg.h>      // va_list
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "utility.h"

#ifdef GEN_TRACE_FILE
FILE *_apptrace;
#endif

// Print work information
void echo(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char str[200];
    sprintf(str, "[%lu] ", pthread_self());
    strcat(str, format);
    vfprintf(stderr, str, args);
    va_end(args);
}

// Print error information
int errexit(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    echo(format, args);
    va_end(args);
    exit(1);
}
