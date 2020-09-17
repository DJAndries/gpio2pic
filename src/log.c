#include "log.h"
#include <stdio.h>
#include <stdarg.h>

const char log_level_txt[][16] = {
	"",
	"ERROR",
	"INFO",
	"DEBUG"
};

const char log_level_colors[][8] = {
	"",
	"\x1B[31m",
	"\x1B[33m",
	"\x1B[32m"
};

void dlog(int level, const char* msg, ...) {
	va_list args;
	char fmsg[512];

#ifndef IS_DEBUG
	if (level >= LOG_DEBUG) {
		return;
	}
#endif

	va_start(args, msg);
	vsprintf(fmsg, msg, args);
	va_end(args);

	fprintf(level != LOG_ERROR ? stdout : stderr, "[%s%s\x1B[0m] %s\n", log_level_colors[level], log_level_txt[level], fmsg);
}

