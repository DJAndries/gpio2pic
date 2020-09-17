#include "config.h"
#include <stdio.h>
#include <string.h>
#include "log.h"

#define CLOCK "clock"
#define DATA "data"
#define PGM "pgm"
#define MCLR "mclr"
#define INVERT "invert"
#define CONFIG_FILENAME "/etc/gpio2pic.conf"
#define LINE_SZ 128

int process_config_row(gpio2pic_config* config, char* func_name, unsigned int pin, char* invert_mode) {
	int invert;

	if (pin > 100) {
		dlog(LOG_ERROR, "Config pin must be between 0 and 100");
		return 1;
	}

	invert = !strcmp(INVERT, invert_mode);

	if (strcmp(func_name, CLOCK) == 0) {
		config->clock_pin = pin;
		config->clock_invert = invert;
	} else if (strcmp(func_name, DATA) == 0) {
		config->data_pin = pin;
		config->data_invert = invert;
	} else if (strcmp(func_name, PGM) == 0) {
		config->pgm_pin = pin;
		config->pgm_invert = invert;
	} else if (strcmp(func_name, MCLR) == 0) {
		config->mclr_pin = pin;
		config->mclr_invert = invert;
	} else {
		dlog(LOG_ERROR, "Unknown config function name: %s", func_name);
		return 2;
	}
	return 0;
}

int load_config(gpio2pic_config* config) {
	char line[LINE_SZ], func_name[LINE_SZ], invert_mode[LINE_SZ];
	unsigned int pin; 
	size_t i;
	FILE* fp = fopen(CONFIG_FILENAME, "r");

	config->clock_pin = 17;
	config->clock_invert = 1;
	config->data_pin = 27;
	config->data_invert = 0;
	config->pgm_pin = 5;
	config->pgm_invert = 1;
	config->mclr_pin = 6;
	config->mclr_invert = 1;

	if (fp == 0) {
		dlog(LOG_DEBUG, "Failed to open config file, ignoring");
		return 0;
	}

	for (i = 1; fgets(line, LINE_SZ, fp); i++) {
		if (line[0] == '#' || line[0] == '\n') continue;

		if (sscanf(line, "%s %u %s", func_name, &pin, invert_mode) != 3) {
			dlog(LOG_ERROR, "Failed to parse config line %u", i);
			fclose(fp);
			return 1;
		}

		if (process_config_row(config, func_name, pin, invert_mode)) {
			fclose(fp);
			return 2;
		}
	}

	fclose(fp);
	return 0;
}
