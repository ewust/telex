// JAH 4/2011

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#include "logger.h"

#if !defined(NOLOG)

static enum LogLevel log_output_level = LOG_INFO;
static FILE *log_output_stream = NULL;
static char *log_level_name[] = 
  { "FATAL", "ERROR", "WARN ", "INFO ", "DEBUG", "TRACE" };

int LogLogVA(enum LogLevel level, const char *loggerName, const char *logMessage, va_list args)
{	
	if (log_output_stream && level <= log_output_level) {
		char *levelName;		
		if (level < 0 || level >= sizeof(log_level_name)) {
			levelName = "UNKNOWN";
		} else {
			levelName = log_level_name[level];
		}

		struct timeval now;
		char timestamp[256];
		gettimeofday(&now, NULL);
		time_t sec = now.tv_sec;
		struct tm* ptm = localtime(&sec);
		strftime(timestamp, 20, "%b %d %H:%M:%S", ptm);
		fprintf(log_output_stream, "%s.%03ld [%s] ", 
			timestamp, now.tv_usec/1000, levelName);
		
		if (loggerName) {
			fprintf(log_output_stream, "%s: ", loggerName);
		}
		if (logMessage) {
			vfprintf(log_output_stream, logMessage, args);
		}
		if (loggerName || logMessage) {
			fputs("\n", log_output_stream);
		}
	}
	return 0;
}

void LogOutputStream(FILE *stream)
{
	log_output_stream = stream;
}

void LogOutputLevel(enum LogLevel level)
{
	log_output_level = level;
}

int LogLog(enum LogLevel level, const char *name, const char *message, ...) {
	va_list va; va_start(va, message);
	int ret = LogLogVA(level, name, message, va);
    va_end(va);
    return ret;
}
int LogFatal(const char *name, const char *message, ...) {
	va_list va; va_start(va, message);
	int ret = LogLogVA(LOG_FATAL, name, message, va);
    va_end(va);
    return ret;
}
int LogError(const char *name, const char *message, ...) {
	va_list va; va_start(va, message);
	int ret = LogLogVA(LOG_ERROR, name, message, va);
    va_end(va);
    return ret;
}
int LogWarn(const char *name, const char *message, ...) {
	va_list va; va_start(va, message);
	int ret = LogLogVA(LOG_WARN, name, message, va);
    va_end(va);
    return ret;
}
int LogInfo(const char *name, const char *message, ...) {
	va_list va; va_start(va, message);
	int ret = LogLogVA(LOG_INFO, name, message, va);
    va_end(va);
    return ret;
}
int LogDebug(const char *name, const char *message, ...) {
	va_list va; va_start(va, message);
	int ret = LogLogVA(LOG_DEBUG, name, message, va);
    va_end(va);
    return ret;
}
int LogTrace(const char *name, const char *message, ...) {
	va_list va; va_start(va, message);
    int ret = LogLogVA(LOG_TRACE, name, message, va);
    va_end(va);
    return ret;
}

void HexDump(enum LogLevel level, const char *loggerName, const char *message, void *in, int len)
{
	int bufsize = strlen(message)+len*5+70;
	char *str = malloc(bufsize), *p = str;	
	assert(str);
	char *data = in;    
    int i, j;	
	snprintf(p, bufsize, "%s (%u bytes)\n", message, len);	
	p += strlen(p);
	for (i = 0; i < len; i += 16) {
		for (j = i; j < i+16 && j < len; j++) {
			snprintf(p, 3, "%02X ", (unsigned char)data[j]);
			p += 3;
		}
		for (; j < i+16 + 1; j++) {
			strncpy(p, "   ", 3);
			p += 3;
		}
		for (j = i; j < i+16 && j < len; j++) {
			unsigned char c = (unsigned char)data[j];
			*(p++) = ((c >= ' ' && c <= '~') ? c : '.');
		}        
		if (i + 16 < len) {
			strncpy(p, "\n", strlen("\n"));
			p += strlen("\n");
		}
	}
	*p = '\0';
	LogLog(level, loggerName, str);
	free(str);
}

#endif
