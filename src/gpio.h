#ifndef GPIO_H
#define GPIO_H

int init_gpio(int clock_line_offset, int data_line_offset, int pgm_line_offset, int mclr_line_offset, char inv_out);

int flip_clock();

void release_gpio();

int set_data_line(int value);

int set_pgm_line(int value);

int set_mclr_line(int value);

int get_data_line();

int switch_data_to_input();

int switch_data_to_output();

#endif
