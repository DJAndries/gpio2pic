#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "util.h"
#include "cmd.h"
#include "debug_lst.h"
#include "log.h"
#include "gpio.h"
#include "debug.h"
#include "debug_lst.h"

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

static int display_break_info(const char* lst_filename) {
	int result;
	uint16_t addr;
	char source_line[LST_LINE_SZ];
	char asm_line[LST_LINE_SZ];

	if (get_break_addr(&addr)) {
		dlog(LOG_ERROR, "Could not retrieve break addr");
		return 1;
	}

	dlog(LOG_INFO, "Break addr: %hx", addr);

	if (lst_filename) {
		result = resolve_lst_line(lst_filename, addr, source_line, asm_line);
		if (result < 0) {
			return 2;
		} else if (result == 0) {
			if (source_line[0]) {
				/* get rid of new line */
				source_line[strlen(source_line) - 1] = 0;
				dlog(LOG_INFO, "Source line: %s", source_line);
			}
			dlog(LOG_INFO, "Instruction: %s", asm_line);
		}
	}

	return 0;
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
		is_inactive = 0;
		return 1;
	}
	return 0;
}

int start_debug_shell(const char* lst_filename) {
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
			if (display_break_info(lst_filename)) {
				return 2;
			}
		}

		printf("\x1B[35mgpio2pic debug\x1B[0m> ");
		if (request_cmd(stdin, cmd, arg, arg)) {
			dlog(LOG_ERROR, "Bad debug command input");
			continue;
		}

		if (process_cmd() < 0) break;
	}

	is_inactive = 1;
	if (stop_debug()) return 1;
	
	return 0;
}
