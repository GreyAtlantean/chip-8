#include "../include/chip8.h"

int main (int argc, char *argv[]) {

	Chip8 emu;

	if (argc == 2) {
		if (emu.load_rom(argv[1])) {
			return 1;
		}
	} else if (emu.load_rom("../roms/tests/2-ibm-logo.ch8") != 0) {
		return 1;
	}
	
	emu.run();	

	return 0;
}
