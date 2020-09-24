#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

int halt();

int start_debug();

int stop_debug();

int get_break_addr(uint16_t* addr);

int next();

int continue_exec();

int break_addr(uint16_t addr);
			
int mem_access(uint16_t addr, uint8_t* value);

#endif
