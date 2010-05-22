#include "common.h"
#include <xpwn/libxpwn.h>
#include <stdarg.h>
#include <string.h>

void defaultCallback(const char* Message);

LogMessageCallback logCallback = defaultCallback;

int GlobalLogLevel = 0xFF;


void defaultCallback(const char* Message) {
	printf("%s", Message);
}
//
//void init_libxpwn() {
//	GlobalLogLevel = 0xFF;
//	logCallback = defaultCallback;
//}

void libxpwn_log(LogMessageCallback callback) {
	logCallback = callback;
}

void libxpwn_loglevel(int logLevel) {
	GlobalLogLevel = logLevel;
}

void Log(int level, const char* file, unsigned int line, const char* function, const char* format, ...) {
#ifdef HARD_LOG
	static FILE* logFile = NULL;
	if(logFile == NULL)
		logFile = fopen("log.txt", "w");
#endif

	char mainBuffer[1024];
	char buffer[1024];

	if(level >= GlobalLogLevel) {
		return;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	switch(level) {
		case 0:
		case 1:
			strcpy(mainBuffer, buffer);
			break;
		default:
			snprintf(mainBuffer, sizeof(mainBuffer), "%s:%s:%d: %s", file, function, line, buffer);
	}
	logCallback(mainBuffer);

#ifdef HARD_LOG
	strcat(mainBuffer, "\n");
	fwrite(mainBuffer, 1, strlen(mainBuffer), logFile);
	fflush(logFile);
#endif
}

