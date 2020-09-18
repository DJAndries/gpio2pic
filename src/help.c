#include "help.h"
#include <stdio.h>

const char help_full_output[] =
	"\n"
	"\x1B[35mHigh-level programming commands:\x1B[0m\n"
	"hex <hex filename> - Program hex file\n"
	"hexwatch <hex filename> - Program hex file and start. Repeat when file changes.\n"
	"hexstart <hex filename> - Program hex file and start. PIC reset after CTRL-C.\n"
	"\n"
	"\x1B[35mBasic control commands:\x1B[0m\n"
	"reset - Perform reset (MCLR high)\n"
	"start - Start program (MCLR low)\n"
	"r - Repeat last command\n"
	"exit - Exit gpio2pic\n"
	"run_batch <text filename with commands> - Run list of commands from file\n"
	"\n"
	"\x1B[35mLow-level programming commands:\x1B[0m\n"
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
