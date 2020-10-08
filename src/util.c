#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "log.h"

int request_cmd(FILE* in_stream, const char* prompt, char* cmd, char* arg1, char* arg2) {
	char* line;
	size_t line_len;
	int result;

	if (in_stream == stdin) {
		line = readline(prompt);
		if (line == 0) return 1;
	} else {
		line = malloc(CMD_LINE_SZ);
		if (line == 0) return 1;
		if (fgets(line, CMD_LINE_SZ, in_stream) == 0) {
			free(line);
			return 1;
		}
	}

	line_len = strlen(line);
	if (line_len == 0 || line_len > CMD_LINE_SZ) {
		free(line);
		return 1;
	}

	if (arg2 == 0) {
		result = sscanf(line, "%s %s\n", cmd, arg1);
	} else {
		result = sscanf(line, "%s %s %s\n", cmd, arg1, arg2);
	}
	if (result < 3 && arg2) arg2[0] = 0;
	if (result < 2) arg1[0] = 0;

	if (in_stream == stdin) add_history(line);
	free(line);

	if (result <= 0) {
		cmd[0] = 0;
		return 2;
	}
	return 0;
}

int parse_hex_data(char* arg, uint16_t* data) {
	if (arg[0] == 0) {
		dlog(LOG_ERROR, "Data not provided");
		return 1;
	}
	if (sscanf(arg, "%hx", (unsigned short*)data) != 1) {
		dlog(LOG_ERROR, "Unable to parse hex data");
		return 2;
	}
	return 0;
}
