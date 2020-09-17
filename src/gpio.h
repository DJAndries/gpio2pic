#ifndef GPIO_H
#define GPIO_H

#include "config.h"

int init_gpio(gpio2pic_config* config);

int flip_clock();

void release_gpio();

int set_data_line(int value);

int set_pgm_line(int value);

int set_mclr_line(int value);

int get_data_line();

int switch_data_to_input();

int switch_data_to_output();

#endif
