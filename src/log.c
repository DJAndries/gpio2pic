#include "log.h"
#include <stdio.h>
#include <stdarg.h>

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

	fprintf(level != LOG_ERROR ? stdout : stderr, "[gpio2pic] %s\n", fmsg);
}

