#include "../include/chip8.h"

int main (int argc, char *argv[]) {

	Chip8 emu;

	if (emu.load_rom("../roms/nontest/chip8-roms-master/games/Cave.ch8") != 0) {
		return 1;
	}
	
	//emu.print_mem();
	//emu.print_font();
	emu.run();	
	

	return 0;
}
