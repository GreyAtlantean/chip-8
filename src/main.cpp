#include "../include/chip8.h"

int main (int argc, char *argv[]) {

	Chip8 emu;

	if (emu.load_rom("../roms/tests/4-flags.ch8") != 0) {
		return 1;
	}
	
	//emu.print_mem();
	//emu.print_font();
	emu.run();	
	

	return 0;
}
