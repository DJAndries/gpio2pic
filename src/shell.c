#include "shell.h"
#include "log.h"
#include "cmd.h"
#include "hex.h"
#include "help.h"
#include "util.h"
#include "debug_inject.h"
#include "debug_shell.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOAD_PROG "loadp"
#define LOAD_DATA "loadd"
#define LOAD_CONFIG "loadc"
#define READ_PROG "readp"
#define READ_DATA "readd"
#define INC_ADDR "inc"
#define BEGIN_PROG "prog"
#define PROG_RESET "prog_reset"
#define RESET "reset"
#define START "start"
#define HEX_START "hexstart"
#define ENTER_PROG_MODE "enterp"
#define EXIT_PROG_MODE "exitp"
#define PROG_HEX "hex"
#define PROG_HEX_DEV "hexwatch"
#define PROTECTED_ERASE "protected_erase"
#define INJECT_DEBUGGER "inject_debugger"
#define RUN_BATCH "runbatch"
#define DEBUG "debug"
#define EXIT "exit"
#define HELP "help"

static char cmd[CMD_LINE_SZ];
static char arg[CMD_LINE_SZ];
static uint16_t data;

static volatile int is_watching = 0;

static void int_handler(int v) {
	is_watching = 0;
}

static int process_cmd();

static int run_batch_file() {
	FILE* batch_fd;
	char batch_filename[CMD_LINE_SZ];
	if (arg[0] == 0) {
		dlog(LOG_ERROR, "Batch filename not provided");
		return 1;
	}
	strcpy(batch_filename, arg);
	if ((batch_fd = fopen(batch_filename, "r")) == 0) {
		dlog(LOG_ERROR, "Failed to open batch file");
		return 2;
	}
	while (request_cmd(batch_fd, cmd, arg) != 1) {
		printf("gpio2pic (%s)> %s %s\n", batch_filename, cmd, arg);
		process_cmd();
	}
	fclose(batch_fd);
	strcpy(cmd, "runbatch");
	strcpy(arg, batch_filename);
	return 0;
}

static int process_hex_file() {
	if (arg[0] == 0) { 
		dlog(LOG_ERROR, "Hex filename not provided");
		return 1;
	}
	return program_hex_file(arg);
}

static int prog_start_hex_file() {
	dlog(LOG_INFO, "Programming...");
	if (process_hex_file()) return 1;
	signal(SIGINT, int_handler);
	is_watching = 1;
	dlog(LOG_INFO, "Starting. Use CTRL-C to reset");
	control_exec(1);
	while (is_watching) {
		usleep(250000);
	}
	control_exec(0);
	signal(SIGINT, 0);
	return 0;
}

static int watch_hex_file() {
	time_t last_mod = 0;
	struct stat stat_result;
	is_watching = 1;
	signal(SIGINT, int_handler);
	dlog(LOG_INFO, "Watching file. Use CTRL-C to stop watching.");
	while (is_watching) {
		if (stat(arg, &stat_result)) {
			dlog(LOG_ERROR, "Cannot stat hex file");
			signal(SIGINT, 0);
			return 1;
		}
		if (last_mod > 0 && last_mod != stat_result.st_mtime) {
			dlog(LOG_INFO, "File change detected. Resetting...");
			control_exec(0);
			dlog(LOG_INFO, "Programming...");
			if (process_hex_file()) {
				signal(SIGINT, 0);
				return 2;
			}
			dlog(LOG_INFO, "Starting...");
			control_exec(1);
		}
		last_mod = stat_result.st_mtime;
		usleep(250000);
	}
	signal(SIGINT, 0);
	return 0;
}

static int process_cmd() {
	if (strcmp(cmd, LOAD_PROG) == 0) {
		if (parse_hex_data(arg, &data)) return 2;
		write_to_prog_data(data);
	} else if (strcmp(cmd, LOAD_DATA) == 0) {
		if (parse_hex_data(arg, &data)) return 2;
		write_to_user_data(data);
	} else if (strcmp(cmd, READ_PROG) == 0) {
		if (read_from_prog_data(&data)) return 3;
		dlog(LOG_INFO, "Read data: %x", data);
	} else if (strcmp(cmd, READ_DATA) == 0) {
		if (read_from_user_data(&data)) return 3;
		dlog(LOG_INFO, "Read data: %x", data);
	} else if (strcmp(cmd, INC_ADDR) == 0) {
		inc_addr();
	} else if (strcmp(cmd, BEGIN_PROG) == 0) {
		begin_programming();
	} else if (strcmp(cmd, ENTER_PROG_MODE) == 0) {
		set_prog_mode(1);
	} else if (strcmp(cmd, EXIT_PROG_MODE) == 0) {
		set_prog_mode(0);
	} else if (strcmp(cmd, RUN_BATCH) == 0) {
		run_batch_file();
	} else if (strcmp(cmd, PROG_HEX) == 0) {
		process_hex_file();
	} else if (strcmp(cmd, INJECT_DEBUGGER) == 0) {
		inject_debugger();
	} else if (strcmp(cmd, PROG_HEX_DEV) == 0) {
		dlog(LOG_INFO, "Programming...");
		if (process_hex_file() == 0) {
			dlog(LOG_INFO, "Starting...");
			control_exec(1);
			watch_hex_file();
		}
	} else if (strcmp(cmd, LOAD_CONFIG) == 0) {
		if (parse_hex_data(arg, &data)) return 2;
		load_config_data(data);
	} else if (strcmp(cmd, PROG_RESET) == 0) {
		trigger_reset();
	} else if (strcmp(cmd, HEX_START) == 0) {
		prog_start_hex_file();
	} else if (strcmp(cmd, PROTECTED_ERASE) == 0) {
		protected_erase();
	} else if (strcmp(cmd, RESET) == 0) {
		control_exec(0);
	} else if (strcmp(cmd, START) == 0) {
		control_exec(1);
	} else if (strcmp(cmd, DEBUG) == 0) {
		start_debug_shell();
	} else if (strcmp(cmd, EXIT) == 0) {
		return -1;
	} else if (strcmp(cmd, HELP) == 0) {
		print_full_help();
	} else {
		dlog(LOG_ERROR, "Command not recognized");
		return 1;
	}
	return 0;
}

int start_shell() {
	while (1) {
		printf("\x1B[34mgpio2pic\x1B[0m> ");
		if (request_cmd(stdin, cmd, arg)) {
			dlog(LOG_ERROR, "Bad command input");
			continue;
		}

		if (process_cmd() < 0) break;
	}

	return 0;
}
