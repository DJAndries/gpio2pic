#include "gpio.h"
#include "cmd.h"
#include "shell.h"

int main() {
	if (init_gpio(17, 27, 5, 6, 1)) {
		return 1;
	}

	init_cmd(4);

	start_shell();

	release_gpio();
	return 0;
}
