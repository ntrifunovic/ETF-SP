// File: log.cpp

#include "log.h"

#ifdef LOG

FILE* logFile = stdout;

static int count = 0;

void vlog(const char* format, va_list &arglist) {
	fprintf(logFile, "%d|LOG:\t", ++count);
	vfprintf(logFile, format, arglist);
	fprintf(logFile, "\n");
}

#endif // LOG
