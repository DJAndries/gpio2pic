#include <stdio.h>
#include "log.h"
#include "cmd.h"

static int read_hex_entry() {

}

int program_hex_file(const char* filename) {
	FILE* fp;
	
	if ((fp = fopen(filename, "r")) == 0) {
		dlog(LOG_ERROR, "Failed to open hex file");
		return 1;
	}

	while (read_hex_entry()) {

	}
	fread(fp);
}
