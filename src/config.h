#ifndef CONFIG_H
#define CONFIG_H

struct gpio2pic_config {
	unsigned int clock_pin;
	int clock_invert;
	unsigned int data_pin;
	int data_invert;
	unsigned int pgm_pin;
	int pgm_invert;
	unsigned int mclr_pin;
	int mclr_invert;
};
typedef struct gpio2pic_config gpio2pic_config;

int load_config(gpio2pic_config* config);

#endif
