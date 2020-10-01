#ifndef DEBUG_LST_H
#define DEBUG_LST_H

#include <stdint.h>

#define LST_LINE_SZ 1024

int resolve_lst_line(const char* lst_filename, uint16_t addr, char* source_line, char* asm_line);

#endif
