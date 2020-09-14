#include <gpiod.h>
#include "log.h"

#define CONSUMER_NAME "gpio2pic"

static struct gpiod_chip* chip;
static struct gpiod_line* clock_line;
static struct gpiod_line* data_line;
static struct gpiod_line* pgm_line;
static struct gpiod_line* mclr_line;
static char clock_status;
static char invert_output;

int init_gpio(int clock_line_offset, int data_line_offset, int pgm_line_offset, int mclr_line_offset, char inv_out) {
	clock_status = 0;
	invert_output = inv_out;

	/* hardcoded gpio chip for now */
	if ((chip = gpiod_chip_open("/dev/gpiochip0")) == 0) {
		return 1;	
	}

	if ((clock_line = gpiod_chip_get_line(chip, clock_line_offset)) == 0) {
		gpiod_chip_close(chip);
		return 2;
	}

	if ((data_line = gpiod_chip_get_line(chip, data_line_offset)) == 0) {
		gpiod_chip_close(chip);
		return 3;
	}

	if ((pgm_line = gpiod_chip_get_line(chip, pgm_line_offset)) == 0) {
		gpiod_chip_close(chip);
		return 4;
	}

	if ((mclr_line = gpiod_chip_get_line(chip, mclr_line_offset)) == 0) {
		gpiod_chip_close(chip);
		return 5;
	}

	if (gpiod_line_request_output(clock_line, CONSUMER_NAME, inv_out)) {
		gpiod_chip_close(chip);
		return 6;
	}

	if (gpiod_line_request_output(data_line, CONSUMER_NAME, 0)) {
		gpiod_line_release(clock_line);
		gpiod_chip_close(chip);
		return 7;
	}

	if (gpiod_line_request_output(pgm_line, CONSUMER_NAME, inv_out)) {
		gpiod_line_release(data_line);
		gpiod_line_release(clock_line);
		gpiod_chip_close(chip);
		return 8;
	}

	if (gpiod_line_request_output(mclr_line, CONSUMER_NAME, inv_out)) {
		gpiod_line_release(pgm_line);
		gpiod_line_release(data_line);
		gpiod_line_release(clock_line);
		gpiod_chip_close(chip);
		return 9;
	}
	
	return 0;
}

int flip_clock() {
	clock_status = !clock_status;
	dlog(LOG_DEBUG, "Clock going %s", clock_status ? "HIGH" : "LOW");
	return gpiod_line_set_value(clock_line, invert_output ? !clock_status : clock_status);
}

int set_data_line(int value) {
	dlog(LOG_DEBUG, "Data going %s", value ? "HIGH" : "LOW");
	return gpiod_line_set_value(data_line, value);
}

int set_pgm_line(int value) {
	dlog(LOG_DEBUG, "PGM going %s", value ? "HIGH" : "LOW");
	return gpiod_line_set_value(pgm_line, invert_output ? !value : value);
}

int set_mclr_line(int value) {
	dlog(LOG_DEBUG, "MCLR going %s", value ? "HIGH" : "LOW");
	return gpiod_line_set_value(mclr_line, invert_output ? !value : value);
}

int get_data_line() {
	int value = gpiod_line_get_value(data_line);
	dlog(LOG_DEBUG, "Reading value %d", value);
	return value;
}

int switch_data_to_input() {
	gpiod_line_release(data_line);
	dlog(LOG_DEBUG, "Switching data to INPUT");
	return gpiod_line_request_input(data_line, CONSUMER_NAME);
}

int switch_data_to_output() {
	gpiod_line_release(data_line);
	dlog(LOG_DEBUG, "Switching data to OUTPUT");
	return gpiod_line_request_output(data_line, CONSUMER_NAME, 0);
}

void release_gpio() {
	gpiod_line_release(clock_line);
	gpiod_line_release(data_line);
	gpiod_chip_close(chip);
}
