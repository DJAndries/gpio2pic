#include "debug_lst.h"
#include <stdio.h>
#include <string.h>
#include "log.h"

#define LINE_INFO_START ".line"

int lst_resolve_addr(const char* lst_filename, uint16_t addr, char* source_line, char* asm_line) {
	FILE* fp;
	char line[LST_LINE_SZ];
	uint16_t parsed_addr;
	unsigned int value;
	char* line_info;

	if ((fp = fopen(lst_filename, "r")) == 0) {
		dlog(LOG_ERROR, "Failed to open lst file");
		return -1;
	}

	source_line[0] = 0;
	asm_line[0] = 0;

	while (fgets(line, LST_LINE_SZ, fp)) {
		if ((line_info = strstr(line, LINE_INFO_START))) {
			strcpy(source_line, line_info + 1);
			continue;
		}
		if (sscanf(line, "%hx %x %[^\n]s", &parsed_addr, &value, asm_line) < 3) {
			continue;
		}
		if (parsed_addr != addr) {
			continue;
		}

		fclose(fp);
		return 0;
	}

	fclose(fp);

	return 1;
}

int lst_resolve_source_line(const char* lst_filename, const char* source_file, unsigned int line_num, uint16_t* addr) {
	FILE* fp;
	char line[LST_LINE_SZ];
	char search_str[LST_LINE_SZ];
	uint16_t parsed_addr;
	unsigned int value;
	char* line_info = 0;

	if ((fp = fopen(lst_filename, "r")) == 0) {
		dlog(LOG_ERROR, "Failed to open lst file");
		return -1;
	}

	sprintf(search_str, ".line\t%u; \"%s\"", line_num, source_file);

	while (fgets(line, LST_LINE_SZ, fp)) {
		if (!line_info) {
			line_info = strstr(line, search_str);
		} else {
			if (sscanf(line, "%hx %x", &parsed_addr, &value) == 2) {
				*addr = parsed_addr;
				fclose(fp);
				return 0;
			}
		}
	}

	fclose(fp);

	dlog(LOG_ERROR, "Failed to find source file/line number");

	return 1;
}
