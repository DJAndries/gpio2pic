#include "help.h"
#include <stdio.h>

const char help_full_output[] =
	"\n"
	"\x1B[35mHigh-level programming commands:\x1B[0m\n"
	"hex <hex filename> - Program hex file\n"
	"hexwatch <hex filename> - Program hex file and start. Repeat when file changes.\n"
	"hexstart <hex filename> - Program hex file and start. PIC reset after CTRL-C.\n"
	"hexdebug <hex filename> <lst filename> - Program hex file, inject debugger and start.\n"
	"\n"
	"\x1B[35mBasic control commands:\x1B[0m\n"
	"reset - Perform reset (MCLR high)\n"
	"debug <lst filename> - Start debug mode. Injected debugger is a prerequisite.\n"
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
	"inject_debugger - Injects debugger firmware.\n"
	"prog - begin erase/programming\n"
	"loadc <hex data> - load configuration data\n"
	"inc - increment address\n"
	"prog_reset - trigger address reset when in programming mode\n"
	"protected_erase - Erase code-protected memory\n"
	"enterp - enter programming mode\n"
	"exitp - exit programming mode\n"
	"\n";

const char help_debug_output[] =
	"\n"
	"next - Execute/step over next instruction\n"
	"continue - Resume execution\n"
	"break <hex address> - Set breakpoint at address and resume execution\n"
	"break_src <source file>:<line number> - Set breakpoint at source code line "
	"(if lst is provided at debug start)\n"
	"readr <hex address> - Read ram/register at address\n"
	"exit - Exit debug mode\n"
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

void print_debug_help() {
	printf(help_debug_output);
}
