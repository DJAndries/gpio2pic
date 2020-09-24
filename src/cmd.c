#include "cmd.h"
#include <unistd.h>
#include <time.h>
#include "gpio.h"
#include "log.h"

#define LOAD_USER_DATA 0x03
#define LOAD_PROG_DATA 0x02
#define READ_USER_DATA 0x05
#define READ_PROG_DATA 0x04
#define BULK_ERASE_1 0x01
#define BULK_ERASE_2 0x07
#define LOAD_CONFIG_DATA 0x0
#define INC_ADDR 0x06
#define BEGIN_PROGRAM 0x08
#define CMD_BITS 6
#define DATA_BITS 16

static void period_wait() {
	struct timespec begin;
	struct timespec req;
	long wait_ns = 0;

	if (clock_gettime(CLOCK_REALTIME, &begin)) {
		return;
	}

	while (1) {
		if (clock_gettime(CLOCK_REALTIME, &req)) {
			return;
		}

		if (req.tv_sec > begin.tv_sec) {
			wait_ns = 1000000000;
		}
		wait_ns = wait_ns + req.tv_nsec - begin.tv_nsec;

		if (wait_ns >= 100) {
			return;
		}
	}
}

static int send_with_clock(uint16_t data, size_t bit_length) {
	size_t i;
	for (i = 0; i < (bit_length * 2); i++) {
		if (flip_clock()) return 1;
		if (i % 2 == 0) {
			if (set_data_line(data & 0x01)) return 2;
			data >>= 1;
		}
		period_wait();
	}
	return 0;
}

static int read_with_clock(uint16_t* data) {
	size_t i, bit_count;
	int data_val;
	bit_count = 0;
	*data = 0;
	for (i = 0; i < 32; i++) {
		if (flip_clock()) return 2;
		if (i == 2) {
			if (switch_data_to_input()) return 1;	
		} else if (i > 2 && i < 30 && i % 2 == 1) {
			if ((data_val = get_data_line()) < 0) return 4;
			*data |= data_val << bit_count++;
		} else if (i == 30) {
			if (switch_data_to_output()) return 3;
		}
		period_wait();
	}
	return 0;
}

int write_to_user_data(uint16_t data) {
	data <<= 1;
	if (send_with_clock(LOAD_USER_DATA, CMD_BITS)) return 1;
	if (send_with_clock(data, DATA_BITS)) return 2;
	return 0;
}

int write_to_prog_data(uint16_t data) {
	data <<= 1;
	if (send_with_clock(LOAD_PROG_DATA, CMD_BITS)) return 1;
	if (send_with_clock(data, DATA_BITS)) return 2;
	return 0;
}

int read_from_user_data(uint16_t* data) {
	if (send_with_clock(READ_USER_DATA, CMD_BITS)) return 1;
	if (read_with_clock(data)) return 2;
	return 0;
}

int read_from_prog_data(uint16_t* data) {
	if (send_with_clock(READ_PROG_DATA, CMD_BITS)) return 1;
	if (read_with_clock(data)) return 2;
	return 0;
}

int set_prog_mode(char on) {
	if (on) {
		if (set_pgm_line(1)) return 1;
		usleep(10);
		if (set_mclr_line(1)) return 1;
	} else {
		if (set_mclr_line(0)) return 1;
		usleep(10);
		if (set_pgm_line(0)) return 1;
	}
	return 0;
}

int trigger_reset() {
	if (set_mclr_line(0)) return 1;
	usleep(10000);
	if (set_mclr_line(1)) return 1;
	usleep(10000);
	return 0;
}

int control_exec(int is_running) {
	return set_mclr_line(is_running);
}

int begin_programming() {
	if (send_with_clock(BEGIN_PROGRAM, CMD_BITS)) return 1;
	usleep(4500);
	return 0;
}

int inc_addr() {
	if (send_with_clock(INC_ADDR, CMD_BITS)) return 1;
	return 0;
}

int load_config_data(uint16_t data) {
	data <<= 1;
	if (send_with_clock(LOAD_CONFIG_DATA, CMD_BITS)) return 1;
	if (send_with_clock(data, DATA_BITS)) return 2;
	return 0;
}

int protected_erase() {
	size_t i;
	if (load_config_data(0x3FFF)) return 1;
	for (i = 0; i < 7; i++) {
		if (inc_addr()) return 2;
	}
	if (send_with_clock(BULK_ERASE_1, CMD_BITS)) return 3;
	if (send_with_clock(BULK_ERASE_2, CMD_BITS)) return 3;
	if (begin_programming()) return 4;
	usleep(8000);
	if (send_with_clock(BULK_ERASE_1, CMD_BITS)) return 3;
	if (send_with_clock(BULK_ERASE_2, CMD_BITS)) return 3;
	return 0;
}
