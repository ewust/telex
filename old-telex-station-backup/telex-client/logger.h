// Basic Multi-level Logging

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <stdarg.h>

enum LogLevel { LOG_FATAL, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG, LOG_TRACE,
				NUM_LOGLEVELS };

int LogLogVA(enum LogLevel level, const char *loggerName, const char *logMessage, va_list args);
int LogLog(enum LogLevel level, const char *loggerName, const char *logMessage, ...);

int LogFatal(const char *loggerName, const char *logMessage, ...);
int LogError(const char *loggerName, const char *logMessage, ...);
int LogWarn (const char *loggerName, const char *logMessage, ...);
int LogInfo (const char *loggerName, const char *logMessage, ...);
int LogDebug(const char *loggerName, const char *logMessage, ...);
int LogTrace(const char *loggerName, const char *logMessage, ...);

void LogOutputStream(FILE *stream);
void LogOutputLevel(enum LogLevel level);

void HexDump(enum LogLevel level, const char *loggerName, const char *message, void *in, int len);

/******************************************************************************
// Example:

#include "logger.h"

void main(int argc, char *argv[])
{
  LogOutputLevel( LOG_WARN ); // show warnings and more severe
  LogOutputStream( stderr );  // output to stderr

  LogTrace("example", "main() started");
  if (argc > 1) {
    LogFatal("example", "Args are prohibited, but you have %d", argc);
  }
}

******************************************************************************/

//
// Specify -DNOLOG to remove logging code for benchmarking
//
#if defined(NOLOG)
   inline static void __LOG_NOOP(void) {};
#  define LogFatal(...) (__LOG_NOOP())
#  define LogError(...) (__LOG_NOOP())
#  define LogWarn(...) (__LOG_NOOP())
#  define LogInfo(...) (__LOG_NOOP())
#  define LogDebug(...) (__LOG_NOOP())
#  define LogTrace(...) (__LOG_NOOP())
#  define LogLog(...) (__LOG_NOOP())
#  define LogLogVA(...) (__LOG_NOOP())
#  define LogOutputStream(...) (__LOG_NOOP())
#  define LogOutputLevel(...) (__LOG_NOOP())
#  define HexDump(...) (__LOG_NOOP())
#endif

#endif //_LOGGER_H_
