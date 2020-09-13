#ifndef LOG_H
#define LOG_H

#define LOG_ERROR 1
#define LOG_INFO 2
#define LOG_DEBUG 3

void dlog(int level, const char* msg, ...);

#endif
