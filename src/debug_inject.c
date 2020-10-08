#include <unistd.h>
#include <stdint.h>
#include "debug_inject.h"
#include "hex.h"
#include "cmd.h"
#include "log.h"

#define DEBUG_FW_FILE_LOCAL "firmware/debug.hex"
#define DEBUG_FW_FILE_INSTALL "/usr/local/share/gpio2pic/firmware/debug.hex"
#define DEBUG_BOOTSTRAP_FW_FILE_LOCAL "firmware/debug_bootstrap.hex"
#define DEBUG_BOOTSTRAP_FW_FILE_INSTALL "/usr/local/share/gpio2pic/firmware/debug_bootstrap.hex"

#define BOOTSTRAP_LEN 19

static int set_debug_flag() {
	size_t i = 0;
	uint16_t config;
	if (load_config_data(0x3FFF)) {
		dlog(LOG_DEBUG, "Debug inject: failed to load config data");
		return 1;
	}
	for (i = 0; i < 7; i++) {
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 2;
		}
	}
	if (read_from_prog_data(&config)) {
		dlog(LOG_DEBUG, "Debug inject: failed to read from config");
		return 3;
	}
	config &= 0x37FF;
	if (write_to_prog_data(config)) {
		dlog(LOG_DEBUG, "Debug inject: failed to write to config");
		return 4;
	}
	if (begin_programming()) {
		dlog(LOG_DEBUG, "Debug inject: failed to prog config");
		return 5;
	}
	return 0;
}

static int find_nop_slide(size_t* noop_addr) {
	size_t i, noop_size = 0;
	uint16_t read_value;
	for (i = 0; i < 4; i++) {
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 1;
		}
	}
	for (i = 0; i < 64 && noop_size < BOOTSTRAP_LEN; i++) {
		if (read_from_prog_data(&read_value)) {
			dlog(LOG_DEBUG, "Debug inject: failed to read interrupt data");
			return 2;
		}
		if (read_value == 0) {
			if (noop_size == 0) {
				*noop_addr = i + 4;
				noop_size = 1;
			} else {
				noop_size++;
			}
		} else {
			noop_size = 0;
		}
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 1;
		}
	}
	return !noop_size;
}

static int shift_interrupt_intro(size_t noop_addr) {
	size_t i;
	uint16_t intro_data[64];
	size_t shift_size = noop_addr - 4;

	if (trigger_reset()) {
		dlog(LOG_DEBUG, "Debug inject: failed to reset prog mode");
		return 1;
	}

	for (i = 0; i < 4; i++) {
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 2;
		}
	}
	/* read all interrupt intro bytes, up until the noop slide */
	for (i = 0; i < shift_size; i++) {
		if (read_from_prog_data(intro_data + i)) {
			dlog(LOG_DEBUG, "Debug inject: failed to read interrupt data");
			return 3;
		}
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 2;
		}
	}

	if (trigger_reset()) {
		dlog(LOG_DEBUG, "Debug inject: failed to reset prog mode");
		return 1;
	}
	/* skip to end of memory space reserved for the debug bootstrap */
	for (i = 0; i < BOOTSTRAP_LEN + 4; i++) {
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 2;
		}
	}
	/* write those bytes, resulting in a shift of BOOTSTRAP_LEN bytes */
	for (i = 0; i < shift_size; i++) {
		if (write_to_prog_data(intro_data[i])) {
			dlog(LOG_DEBUG, "Debug inject: failed to write interrupt shift");
			return 4;
		}
		if (begin_programming()) {
			dlog(LOG_DEBUG, "Debug inject: failed to prog interrupt shift");
			return 5;
		}
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 2;
		}
	}
	return 0;
}

static int prep_inject_interrupt() {
	size_t noop_addr = 0;
	if (set_prog_mode(1)) {
		dlog(LOG_DEBUG, "Debug inject: failed to enable prog mode");
		return 1;
	}

	if (find_nop_slide(&noop_addr)) {
		set_prog_mode(0);
		dlog(LOG_ERROR, "Debug inject: failed to find nop slide. "
				"Ensure 19 consecutive nops are included near top of interrupt.");
		return 2;
	}

	if (shift_interrupt_intro(noop_addr)) {
		set_prog_mode(0);
		return 3;
	}

	if (set_debug_flag()) {
		set_prog_mode(0);
		return 4;
	}

	if (set_prog_mode(0)) {
		dlog(LOG_DEBUG, "Debug inject: failed to disable prog mode");
		return 5;
	}

	return 0;
}

int inject_debugger() {
	if (program_hex_file(access(DEBUG_FW_FILE_LOCAL, F_OK) == 0 ?
				DEBUG_FW_FILE_LOCAL : DEBUG_FW_FILE_INSTALL)) {
		dlog(LOG_ERROR, "Debug inject: Failed to program debug fw");
		return 1;
	}
	if (prep_inject_interrupt()) {
		dlog(LOG_ERROR, "Debug inject: Failed to prepare interrupt bootstrap injection");
		return 2;
	}
	if (program_hex_file(access(DEBUG_BOOTSTRAP_FW_FILE_LOCAL, F_OK) == 0 ?
				DEBUG_BOOTSTRAP_FW_FILE_LOCAL : DEBUG_BOOTSTRAP_FW_FILE_INSTALL)) {
		dlog(LOG_ERROR, "Debug inject: Failed to inject interrupt bootstrap");
		return 3;
	}
	
	return 0;
}
