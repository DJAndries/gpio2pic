#include <stdint.h>
#include "debug_inject.h"
#include "hex.h"
#include "cmd.h"
#include "log.h"

#define DEBUG_FW_FILE "firmware/debug.hex"

#define BOOTSTRAP_LEN 3
const uint16_t bootstrap_code[] = {
	/* bsf pclath, 3 */
	0x158A,
	/* bsf pclath, 4 */
	0x160A,
	/* call 0x1F40 */
	0x2740
};

static int add_interrupt_bootstrap(size_t noop_addr) {
	size_t i;
	if (trigger_reset()) {
		dlog(LOG_DEBUG, "Debug inject: failed to reset prog mode");
		return 1;
	}
	for (i = 0; i < noop_addr; i++) {
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to inc addr");
			return 2;
		}
	}
	for (i = 0; i < BOOTSTRAP_LEN; i++) {
		if (write_to_prog_data(bootstrap_code[i])) {
			dlog(LOG_DEBUG, "Debug inject: failed to write interrupt bootstrap");
			return 3;
		}
		if (begin_programming()) {
			dlog(LOG_DEBUG, "Debug inject: failed to prog interrupt bootstrap");
			return 4;
		}
		if (inc_addr()) {
			dlog(LOG_DEBUG, "Debug inject: failed to incr addr");
			return 5;
		}
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
	for (i = 0; i < 26 && noop_size < BOOTSTRAP_LEN; i++) {
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

static int inject_interrupt() {
	size_t noop_addr = 0;
	if (set_prog_mode(1)) {
		dlog(LOG_DEBUG, "Debug inject: failed to enable prog mode");
		return 1;
	}

	if (find_nop_slide(&noop_addr)) {
		set_prog_mode(0);
		dlog(LOG_ERROR, "Debug inject: failed to find nop slide. "
				"Ensure 6 consecutive nops are included near top of interrupt.");
		return 3;
	}

	if (add_interrupt_bootstrap(noop_addr)) {
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
	if (program_hex_file(DEBUG_FW_FILE)) {
		dlog(LOG_ERROR, "Debug inject: Failed to program debug fw");
		return 1;
	}
	if (inject_interrupt()) {
		dlog(LOG_ERROR, "Debug inject: Failed to inject interrupt bootstrap");
		return 2;
	}
	return 0;
}
