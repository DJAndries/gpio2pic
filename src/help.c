#include "help.h"
#include <stdio.h>

const char help_full_output[] =
	"\n"
	"High-level programming commands:\n"
	"hex <hex filename> - Program hex file\n"
	"hexdev <hex filename> - Program hex file and start. Repeat when file changes.\n"
	"\n"
	"Basic control commands:\n"
	"reset - Perform reset (MCLR high)\n"
	"start - Start program (MCLR low)\n"
	"r - Repeat last command\n"
	"exit - Exit gpio2pic\n"
	"run_batch <text filename with commands> - Run list of commands from file\n"
	"\n"
	"Low-level programming commands:\n"
	"loadp <hex data> - load program data\n" 
	"loadd <hex data> - load eeprom data\n"
	"readp - read program data\n"
	"readd - read eeprom data\n"
	"prog - begin erase/programming\n"
	"loadc <hex data> - load configuration data\n"
	"inc - increment address\n"
	"prog_reset - trigger address reset when in programming mode\n"
	"protected_erase - Erase code-protected memory\n"
	"enterp - enter programming mode\n"
	"exitp - exit programming mode\n"
	"\n";

const char help_basic_output[] =
	"interactive usage: gpio2pic (run 'help' to see commands)\n"
	"non-interactive usage: gpio2pic <hex file>\n";

void print_basic_help() {
	printf(help_basic_output);
}

void print_full_help() {
	printf(help_full_output);
}
