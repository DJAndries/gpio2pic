#include <string.h>
#include "gpio.h"
#include "cmd.h"
#include "shell.h"
#include "log.h"
#include "config.h"
#include "hex.h"
#include "help.h"

int main(int argc, char** argv) {
	char* prog_filename = 0;
	gpio2pic_config config;

	if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			print_basic_help();
			return 0;
		}
		prog_filename = argv[1];
	}

	if (load_config(&config)) {
		return 1;
	}

	dlog(LOG_DEBUG, "MCLR: Pin: %u Invert: %d", config.mclr_pin, config.mclr_invert);
	dlog(LOG_DEBUG, "PGM: Pin: %u Invert: %d", config.pgm_pin, config.pgm_invert);
	dlog(LOG_DEBUG, "Data: Pin: %u Invert: %d", config.data_pin, config.data_invert);
	dlog(LOG_DEBUG, "Clock: Pin: %u Invert: %d", config.clock_pin, config.clock_invert);

	if (init_gpio(&config)) {
		dlog(LOG_ERROR, "Failed to init gpio");
		return 2;
	}
	
	if (prog_filename) {
		if (program_hex_file(argv[1])) {
			dlog(LOG_ERROR, "Failed to program");
			release_gpio();
			return 3;
		}
	} else {
		start_shell();
	}

	release_gpio();
	return 0;
}
