#include "debug.h"
#include <unistd.h>
#include "gpio.h"
#include "log.h"
#include "cmd.h"

static void general_delay(int multi) {
	usleep(500 * multi);
}

int halt() {
	if (flip_clock()) return 1;
	general_delay(1);
	if (flip_clock()) return 2;
	general_delay(1);
	return 0;
}

int next() {
	if (flip_clock()) return 1;
	if (switch_data_to_output()) return 2;
	general_delay(1);
	if (set_data_line(0)) {
		switch_data_to_input();
		return 3;
	}
	if (flip_clock()) {
		switch_data_to_input();
		return 4;
	}
	general_delay(1);
	if (switch_data_to_input()) return 5;
	return 0;
}

int continue_exec() {
	return break_addr(0);
}

int break_addr(uint16_t addr) {
	size_t i;
	for (i = 0; i < 30; i++) {
		if (flip_clock()) {
			switch_data_to_input();
			return 1;
		}
		if (i == 0) {
			if (switch_data_to_output()) return 2;
		}
		if (i % 2 == 0) {
			if (set_data_line(i < 4 ? 1 : (addr & 0x01))) {
				switch_data_to_input();
				return 3;
			}
			if (i >= 4) addr >>= 1;
		}
		general_delay(1);
	}
	general_delay(8);
	if (switch_data_to_input()) return 5;
	return 0;
}

int mem_access(uint16_t addr, uint8_t* value) {
	size_t i, bit_count;
	int linevalue;
	bit_count = 0;
	*value = 0;
	for (i = 0; i < 22; i++) {
		if (flip_clock()) {
			switch_data_to_input();
			return 1;
		}
		if (i == 0) {
			if (switch_data_to_output()) return 2;
		}
		if (i % 2 == 0) {
			if (i == 0) {
				linevalue = 1;
			} else if (i == 2) {
				linevalue = 0;
			} else if (i == 4) {
				linevalue = (addr >> 8) & 0x01;
			} else {
				linevalue = addr & 0x01;
				addr >>= 1;
			}
			if (set_data_line(linevalue)) {
				switch_data_to_input();
				return 3;
			}
		}
		general_delay(8);
	}
	general_delay(8);
	if (switch_data_to_input()) return 5;
	for (i = 0; i < 16; i++) {
		if (flip_clock()) {
			return 1;
		}
		if (i % 2 == 1) {
			if ((linevalue = get_data_line()) < 0) {
				return 3;
			}
			*value |= linevalue << bit_count++;
		}
		general_delay(8);
	}
	return 0;
}

int start_debug() {
	if (control_exec(0)) {
		dlog(LOG_ERROR, "Failed to reset");
		return 1;
	}
	general_delay(2);
	if (switch_data_to_input()) {
		dlog(LOG_ERROR, "Failed to switch data to input");
		return 2;
	}
	general_delay(2);
	if (control_exec(1)) {
		dlog(LOG_ERROR, "Failed to start");
		return 3;
	}
	return 0;
}

int stop_debug() {
	if (control_exec(0)) {
		dlog(LOG_ERROR, "Failed to reset");
		return 1;
	}
	if (switch_data_to_output()) {
		dlog(LOG_ERROR, "Failed to switch data to output");
		return 2;
	}
	return 0;
}

int get_break_addr(uint16_t* addr) {
	size_t i, bit_count;
	int data_val;
	bit_count = 0;
	*addr = 0;
	for (i = 0; i < 26; i++) {
		if (flip_clock()) return 1;
		if (i % 2 == 1) {
			if ((data_val = get_data_line()) < 0) return 2;
			*addr |= data_val << bit_count++;
		}
		general_delay(1);
	}
	return 0;
}
