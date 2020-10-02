#include "util.h"
#include <string.h>
#include "log.h"

int request_cmd(FILE* in_stream, char* cmd, char* arg1, char* arg2) {
	char line[CMD_LINE_SZ];
	int result;
	if (fgets(line, CMD_LINE_SZ, in_stream) == 0) {
		return 1;
	}
	if (strcmp(line, REPEAT "\n") == 0) {
		return 0;
	}
	if (arg2 == 0) {
		result = sscanf(line, "%s %s\n", cmd, arg1);
	} else {
		result = sscanf(line, "%s %s %s\n", cmd, arg1, arg2);
	}
	if (result < 3 && arg2) arg2[0] = 0;
	if (result < 2) arg1[0] = 0;
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
