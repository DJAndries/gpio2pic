#ifndef CMD_H
#define CMD_H

#include <stddef.h>
#include <stdint.h>

void init_cmd(int period_ms_init);

int write_to_user_data(uint16_t data);

int write_to_prog_data(uint16_t data);

int read_from_user_data(uint16_t* data);

int read_from_prog_data(uint16_t* data);

int set_prog_mode(char on);

int begin_programming();

int inc_addr();

#endif