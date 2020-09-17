#ifndef CMD_H
#define CMD_H

#include <stddef.h>
#include <stdint.h>

int write_to_user_data(uint16_t data);

int write_to_prog_data(uint16_t data);

int read_from_user_data(uint16_t* data);

int read_from_prog_data(uint16_t* data);

int set_prog_mode(char on);

int trigger_reset();

int control_exec(int is_running);

int begin_programming();

int inc_addr();

int load_config_data(uint16_t data);

int protected_erase();

#endif
