#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>

#define CMD_LINE_SZ 256

int request_cmd(FILE* in_stream, const char* prompt, char* cmd, char* arg1, char* arg2);

int parse_hex_data(char* arg, uint16_t* data);

#endif
