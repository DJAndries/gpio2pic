#include "shell.h"
#include "log.h"
#include "cmd.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define LINE_SZ 256
#define LOAD_PROG "loadp"
#define LOAD_DATA "loadd"
#define READ_PROG "readp"
#define READ_DATA "readd"
#define INC_ADDR "inc"
#define BEGIN_PROG "prog"
#define ENTER_PROG_MODE "enterp"
#define EXIT_PROG_MODE "exitp"
#define RUN_BATCH "runbatch"
#define EXIT "exit"
#define REPEAT "r"

static char cmd[LINE_SZ];
static char arg[LINE_SZ];
static uint16_t data;

static int process_cmd();

static int request_cmd(FILE* in_stream) {
	char line[LINE_SZ];
	int result;
	if (fgets(line, LINE_SZ, in_stream) == 0) {
		return 1;
	}
	if (strcmp(line, REPEAT "\n") == 0) {
		return 0;
	}
	result = sscanf(line, "%s %s\n", cmd, arg);
	if (result == 0) {
		cmd[0] = 0;
		arg[0] = 0;
		return 2;
	} else if (result == 1) {
		arg[0] = 0;
	}
	return 0;
}

static int parse_hex_data() {
	if (arg[0] == 0) {
		dlog(LOG_ERROR, "Data not provided");
		return 1;
	}
	if (sscanf(arg, "%hx", &data) != 1) {
		dlog(LOG_ERROR, "Unable to parse hex data");
		return 2;
	}
	return 0;
}

static int run_batch_file() {
	FILE* batch_fd;
	char batch_filename[LINE_SZ];
	if (arg[0] == 0) {
		dlog(LOG_ERROR, "Batch filename not provided");
		return 1;
	}
	strcpy(batch_filename, arg);
	if ((batch_fd = fopen(batch_filename, "r")) == 0) {
		dlog(LOG_ERROR, "Failed to open batch file");
		return 2;
	}
	while (request_cmd(batch_fd) != 1) {
		printf("gpio2pic (%s)> %s %s\n", batch_filename, cmd, arg);
		process_cmd();
	}
	fclose(batch_fd);
	strcpy(cmd, "runbatch");
	strcpy(arg, batch_filename);
	return 0;
}

static int process_cmd() {
	if (strcmp(cmd, LOAD_PROG) == 0) {
		if (parse_hex_data()) return 2;
		write_to_prog_data(data);
	} else if (strcmp(cmd, LOAD_DATA) == 0) {
		if (parse_hex_data()) return 2;
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
	} else if (strcmp(cmd, EXIT) == 0) {
		return -1;
	} else {
		dlog(LOG_ERROR, "Command not recognized");
		return 1;
	}
	return 0;
}

int start_shell() {
	while (1) {
		printf("gpio2pic> ");
		if (request_cmd(stdin)) {
			dlog(LOG_ERROR, "Bad command input");
			continue;
		}

		if (process_cmd() < 0) break;
	}

	return 0;
}
