#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <pthread.h>
#include <stdio.h>

#define RET_ERR -1
#define RET_OK  0

#define GEN_TRACE_FILE
#ifdef GEN_TRACE_FILE
extern FILE *_apptrace;
#define TRACE_FUNC_BEGIN fprintf(_apptrace, "%s [%lu] [ENTER] %s @ %s:%d\n", \
        __TIME__, pthread_self(), __FUNCTION__, __FILE__, __LINE__); \
        fflush(_apptrace);
#define TRACE_FUNC_LEAVE fprintf(_apptrace, "%s [%lu] [LEAVE] %s @ %s:%d\n", \
        __TIME__, pthread_self(), __FUNCTION__, __FILE__, __LINE__); \
        fflush(_apptrace);
#define TRACE_FUNC_RET_D(RET) fprintf(_apptrace, "%s [%lu] [LEAVE] %s @ %s:%d <ret: %d>\n", \
        __TIME__, pthread_self(), __FUNCTION__, __FILE__, __LINE__, RET); \
        fflush(_apptrace);
#define TRACE_FUNC_RET_F(RET) fprintf(_apptrace, "%s [%lu] [LEAVE] %s @ %s:%f <ret: %d>\n", \
        __TIME__, pthread_self(), __FUNCTION__, __FILE__, __LINE__, RET); \
        fflush(_apptrace);
#define TRACE_FUNC_RET_S(RET) fprintf(_apptrace, "%s [%lu] [LEAVE] %s @ %s:%s <ret: %d>\n", \
        __TIME__, pthread_self(), __FUNCTION__, __FILE__, __LINE__, RET); \
        fflush(_apptrace);
#define TRACE_MAIN_BEGIN \
    _apptrace = fopen("./server.trc", "a"); \
    if (!_apptrace) \
    { \
        fprintf(stderr, "Open trace file failed !"); \
        TRACE_FUNC_LEAVE; return RET_ERR; \
    } \
    fprintf(_apptrace, "-------- TRACE BEGIN --------\n"); \
    TRACE_FUNC_BEGIN
#define TRACE_MAIN_LEAVE \
    TRACE_FUNC_LEAVE \
    fprintf(_apptrace, "-------- TRACE END --------\n"); \
    fclose(_apptrace);
#else
#define TRACE_FUNC_BEGIN
#define TRACE_FUNC_LEAVE
#define TRACE_FUNC_RET_D(RET)
#define TRACE_FUNC_RET_F(RET)
#define TRACE_FUNC_RET_S(RET)
#define TRACE_MAIN_BEGIN
#define TRACE_MAIN_LEAVE
#endif

int errexit(const char* format, ...);
void echo(const char* format, ...);

#endif
