// File: log.h

#ifndef _LOG_H_
#define _LOG_H_

#ifdef LOGFILE
#define LOG
#endif

#ifdef LOG3
#define LOG2
#endif

#ifdef LOG2
#define LOG1
#endif

#ifdef LOG1
#define LOG
#endif

#ifdef DEBUG
#define LOG
#endif

#define _stringify(x) #x
#define stringify(x) _stringify(x)

#ifdef LOG
#include <stdio.h>
#include <stdarg.h>

extern FILE* logFile;

#endif

inline void initLog() {
  #ifdef LOGFILE
  logFile = fopen(stringify(LOGFILE), "w");
  #endif
}

inline void closeLog() {
  #ifdef LOGFILE
  fclose(logFile);
  #endif
}

#ifdef LOG
void vlog(const char* format, va_list &arglist);
#endif

// Posto je inline nece se ugraditi kada nije definisan LOG
inline void log(const char* format, ...) {
  #ifdef LOG
  va_list args;
  va_start(args, format);
  vlog(format, args);
  va_end(args);
  #endif
}

// log1 - log najveceg prioriteta 
// Posto je inline nece se ugraditi kada nije definisan LOG1
inline void log1(const char* format, ...) {
  #ifdef LOG1
  va_list args;
  va_start(args, format);
  vlog(format, args);
  va_end(args);
  #endif
}

// Posto je inline nece se ugraditi kada nije definisan LOG2
inline void log2(const char* format, ...) {
  #ifdef LOG2
  va_list args;
  va_start(args, format);
  vlog(format, args);
  va_end(args);
  #endif
}

// log3 - log najmanjeg prioriteta 
// Posto je inline nece se ugraditi kada nije definisan LOG3
inline void log3(const char* format, ...) {
  #ifdef LOG3
  va_list args;
  va_start(args, format);
	vlog(format, args);
  va_end(args);
  #endif
}
#endif // _LOG_H_
