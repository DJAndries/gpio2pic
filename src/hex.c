#include "hex.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "log.h"
#include "cmd.h"

#define TYPE_DATA 0x00
#define TYPE_EOF 0x01
#define TYPE_EX_SEG_ADDR 0x02
#define TYPE_STRT_SEG_ADDR 0x03
#define TYPE_EX_LIN_ADDR 0x04
#define TYPE_STRT_LIN_ADDR 0x05
#define EEPROM_START 0x2100

struct hex_entry {
	size_t length;
	size_t address;
	char type;
	char* data;
	char calc_checksum;
};
typedef struct hex_entry hex_entry;

static int validate_checksum(FILE* fp, hex_entry* entry) {
	char checksum_hex[3];
	char checksum;
	if (fread(checksum_hex, sizeof(char), 2, fp) != 2) {
		return 1;
	}
	checksum_hex[2] = 0;
	if (sscanf(checksum_hex, "%hhx", &checksum) != 1) {
		return 2;
	}
	entry->calc_checksum = (~entry->calc_checksum) + 1;
	dlog(LOG_DEBUG, "Calc checksum = %x", entry->calc_checksum);
	if (checksum != entry->calc_checksum) {
		return 3;
	}
	return 0;
}

static int read_hex_entry_data(FILE* fp, hex_entry* entry) {
	size_t i;
	char data_hex[3];
	char dec_data;
	entry->data = malloc(entry->length);
	if (!entry->data) {
		dlog(LOG_DEBUG, "Could not alloc data");
		return 1;
	}
	for (i = 0; i < entry->length; i += 1) {
		if (fread(data_hex, sizeof(char), 2, fp) != 2) {
			dlog(LOG_DEBUG, "Unable to read hex byte of data");
			free(entry->data);
			return 2;
		}
		data_hex[2] = 0;
		if (sscanf(data_hex, "%hhx", &dec_data) != 1) {
			dlog(LOG_DEBUG, "Unable to decode hex byte of data");
			free(entry->data);
			return 3;
		}
		*(entry->data + i) = dec_data;
		entry->calc_checksum += dec_data;
	}
	if (validate_checksum(fp, entry)) {
		dlog(LOG_DEBUG, "Bad checksum");
		free(entry->data);
		return 4;
	}
	return 0;
}

static int read_hex_entry(FILE* fp, hex_entry* entry) {
	entry->calc_checksum = 0;
	char buf[8];
	/* start code */
	if (fread(buf, sizeof(char), 1, fp) != 1) {
		if (feof(fp)) {
			return -1;
		}
		dlog(LOG_DEBUG, "Bad start code");
		return 1;
	}
	if (buf[0] != ':') {
		dlog(LOG_DEBUG, "Bad start code");
		return 1;
	}
	/* byte count */
	if (fread(buf, sizeof(char), 2, fp) != 2) {
		dlog(LOG_DEBUG, "Bad byte count");
		return 2;
	}
	buf[2] = 0;
	if (sscanf(buf, "%x", &entry->length) != 1) {
		dlog(LOG_DEBUG, "Bad byte count hex");
		return 3;
	}
	entry->calc_checksum += entry->length & 0xFF;
	/* byte address */
	if (fread(buf, sizeof(char), 4, fp) != 4) {
		dlog(LOG_DEBUG, "Bad byte address");
		return 4;
	}
	buf[4] = 0;
	if (sscanf(buf, "%x", &entry->address) != 1) {
		dlog(LOG_DEBUG, "Bad byte address hex");
		return 5;
	}
	entry->calc_checksum += (entry->address & 0xFF) + (entry->address >> 8);
	/* record type */
	if (fread(buf, sizeof(char), 2, fp) != 2) {
		dlog(LOG_DEBUG, "Bad record type");
		return 6;
	}
	buf[2] = 0;
	if (sscanf(buf, "%hhx", &entry->type) != 1) {
		dlog(LOG_DEBUG, "Bad entry type hex");
		return 7;
	}
	entry->calc_checksum += entry->type & 0xFF;

	/* read data */
	if (read_hex_entry_data(fp, entry)) {
		return 8;
	}
	/* waste the new line */
	fread(buf, sizeof(char), 1, fp);
	return 0;
}

static int prog_hex_entry(hex_entry* entry, size_t* current_addr) {
	size_t i;
	size_t conv_addr = entry->address / 2;
	char is_eeprom = 0;
	uint16_t data, verify_data;

	if (conv_addr >= EEPROM_START) {
		is_eeprom = 1;
		conv_addr -= EEPROM_START;
	}

	if (entry->type != TYPE_DATA) {
		/* Ignore non data entries */
		return 0;
	}

	if (*current_addr != conv_addr) {
		if (trigger_reset()) {
			dlog(LOG_DEBUG, "Failed to trigger reset");
			return 2;
		}
		*current_addr = conv_addr;
		for (i = 0; i < conv_addr; i++) {
			if (inc_addr()) {
				dlog(LOG_DEBUG, "Failed to increment addr");
				return 2;
			}
		}
	}

	for (i = 0; i < entry->length; i += 2) {
		data = *(entry->data + i) | (*(entry->data + i + 1) << 8);
		if (is_eeprom) {
			if (write_to_user_data(data) || begin_programming() ||
					read_from_user_data(&verify_data)) {
				dlog(LOG_DEBUG, "Failed to read/write to user data");
				return 3;
			}
			if ((verify_data & 0xFF) != data) {
				dlog(LOG_DEBUG, "Failed to verify user data");
				return 4;
			}
		} else {
			if (write_to_prog_data(data) || begin_programming() ||
					read_from_prog_data(&verify_data)) {
				dlog(LOG_DEBUG, "Failed to read/write to prog data");
				return 3;
			}
			if (verify_data != data) {
				dlog(LOG_DEBUG, "Failed to verify prog data. actual = %x, expect = %x",
						verify_data, data);
				return 4;
			}
		}
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Failed to increment addr");
			return 2;
		}
		(*current_addr)++;
	}
	return 0;
}

int program_hex_file(const char* filename) {
	FILE* fp;
	hex_entry entry;
	int read_result;
	size_t current_addr = 0;
	
	if ((fp = fopen(filename, "r")) == 0) {
		dlog(LOG_ERROR, "Failed to open hex file");
		return 1;
	}

	if (set_prog_mode(1)) {
		dlog(LOG_ERROR, "Failed to set prog mode");
		fclose(fp);
		return 2;
	}
	while ((read_result = read_hex_entry(fp, &entry)) == 0) {
		if (prog_hex_entry(&entry, &current_addr)) {
			dlog(LOG_ERROR, "Failed to program hex entry");
			free(entry.data);
			set_prog_mode(0);
			fclose(fp);
			return 4;
		}
		free(entry.data);
	}
	if (read_result > 0) {
		dlog(LOG_ERROR, "Failed to read hex entry");
		set_prog_mode(0);
		fclose(fp);
		return 3;
	}
	fclose(fp);
	if (set_prog_mode(0)) {
		dlog(LOG_ERROR, "Failed to set prog mode");
		return 6;
	}
	return 0;
}
