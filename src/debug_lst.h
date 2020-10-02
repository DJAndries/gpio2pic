#ifndef DEBUG_LST_H
#define DEBUG_LST_H

#include <stdint.h>

#define LST_LINE_SZ 1024

int lst_resolve_addr(const char* lst_filename, uint16_t addr, char* source_line, char* asm_line);

int lst_resolve_source_line(const char* lst_filename, const char* source_file, unsigned int line_num, uint16_t* addr);

#endif
