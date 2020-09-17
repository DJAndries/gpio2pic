#include "gpio.h"
#include <gpiod.h>
#include "log.h"

#define CONSUMER_NAME "gpio2pic"

static gpio2pic_config config;
static struct gpiod_chip* chip;
static struct gpiod_line* clock_line;
static struct gpiod_line* data_line;
static struct gpiod_line* pgm_line;
static struct gpiod_line* mclr_line;
static char clock_status;

int init_gpio(gpio2pic_config* init_config) {
	config = *init_config;
	clock_status = 0;

	/* hardcoded gpio chip for now */
	if ((chip = gpiod_chip_open("/dev/gpiochip0")) == 0) {
		return 1;	
	}

	if ((clock_line = gpiod_chip_get_line(chip, config.clock_pin)) == 0) {
		gpiod_chip_close(chip);
		return 2;
	}

	if ((data_line = gpiod_chip_get_line(chip, config.data_pin)) == 0) {
		gpiod_chip_close(chip);
		return 3;
	}

	if ((pgm_line = gpiod_chip_get_line(chip, config.pgm_pin)) == 0) {
		gpiod_chip_close(chip);
		return 4;
	}

	if ((mclr_line = gpiod_chip_get_line(chip, config.mclr_pin)) == 0) {
		gpiod_chip_close(chip);
		return 5;
	}

	if (gpiod_line_request_output(clock_line, CONSUMER_NAME, config.clock_invert)) {
		gpiod_chip_close(chip);
		return 6;
	}

	if (gpiod_line_request_output(data_line, CONSUMER_NAME, config.data_invert)) {
		gpiod_line_release(clock_line);
		gpiod_chip_close(chip);
		return 7;
	}

	if (gpiod_line_request_output(pgm_line, CONSUMER_NAME, config.pgm_invert)) {
		gpiod_line_release(data_line);
		gpiod_line_release(clock_line);
		gpiod_chip_close(chip);
		return 8;
	}

	if (gpiod_line_request_output(mclr_line, CONSUMER_NAME, config.mclr_invert)) {
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
	return gpiod_line_set_value(clock_line, config.clock_invert ? !clock_status : clock_status);
}

int set_data_line(int value) {
	dlog(LOG_DEBUG, "Data going %s", value ? "HIGH" : "LOW");
	return gpiod_line_set_value(data_line, config.data_invert ? !value : value);
}

int set_pgm_line(int value) {
	dlog(LOG_DEBUG, "PGM going %s", value ? "HIGH" : "LOW");
	return gpiod_line_set_value(pgm_line, config.pgm_invert ? !value : value);
}

int set_mclr_line(int value) {
	dlog(LOG_DEBUG, "MCLR going %s", value ? "HIGH" : "LOW");
	return gpiod_line_set_value(mclr_line, config.mclr_invert ? !value : value);
}

int get_data_line() {
	int value = gpiod_line_get_value(data_line);
	if (config.data_invert) value = !value;
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
	return gpiod_line_request_output(data_line, CONSUMER_NAME, config.data_invert);
}

void release_gpio() {
	gpiod_line_release(clock_line);
	gpiod_line_release(data_line);
	gpiod_chip_close(chip);
}
