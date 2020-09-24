#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "util.h"
#include "cmd.h"
#include "log.h"
#include "gpio.h"
#include "debug.h"

#define NEXT "next"
#define CONTINUE "continue"
#define BREAK "break"
#define READ_RAM "readr"
#define EXIT "exit"

static char cmd[CMD_LINE_SZ];
static char arg[CMD_LINE_SZ];
static uint16_t data;

static volatile int is_halted = 0;
static int is_inactive = 1;

static void int_handler(int v) {
	is_halted = 1;
}

static int process_cmd() {
	uint8_t data_result;
	is_inactive = 1;
	if (strcmp(cmd, NEXT) == 0) {
		next();
	} else if (strcmp(cmd, BREAK) == 0) {
		if (parse_hex_data(arg, &data)) return 2;
		break_addr(data);
	} else if (strcmp(cmd, CONTINUE) == 0) {
		continue_exec();
	} else if (strcmp(cmd, READ_RAM) == 0) {
		if (parse_hex_data(arg, &data)) return 2;
		if (mem_access(data, &data_result) == 0) {
			dlog(LOG_INFO, "Data: %hhx", data_result);
		}
		is_inactive = 0;
	} else if (strcmp(cmd, EXIT) == 0) {
		return -1;
	} else {
		dlog(LOG_ERROR, "Command not recognized");
		is_inactive = 1;
		return 1;
	}
	return 0;
}

int start_debug_shell() {
	uint16_t addr;
	if (start_debug()) return 1;
	
	while (1) {
		if (is_inactive == 1) {
			dlog(LOG_INFO, "Waiting for halt, use CTRL-C to halt");
			signal(SIGINT, int_handler);
			while (get_data_line() == 1) {
				if (is_halted) {
					is_halted = 0;
					halt();
				}
			}
			is_inactive = 0;
			signal(SIGINT, 0);
			if (get_break_addr(&addr)) {
				dlog(LOG_ERROR, "Could not retrieve break addr");
				return 2;
			}
			dlog(LOG_INFO, "Break addr: %hx", addr);
		}

		printf("\x1B[35mgpio2pic debug\x1B[0m> ");
		if (request_cmd(stdin, cmd, arg)) {
			dlog(LOG_ERROR, "Bad debug command input");
			continue;
		}

		if (process_cmd() < 0) break;
	}

	is_inactive = 1;
	if (stop_debug()) return 1;
	
	return 0;
}
