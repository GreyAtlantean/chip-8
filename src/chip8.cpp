#include "../include/chip8.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <fstream>


Chip8::Chip8() : disp(64, 32){
	
	// Set Program Counter
	chip_PC = progMemStart;
	chip_I = 0x0;
	// Zero the RAM	
	for (auto& byte : chip_mem) {
		byte = 0x0;
	}
	// Zero the registers
	for (auto& byte : registers) {
		byte = 0x0;
	}

	// from https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
	fontset = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	// copy fontset into memory
	unsigned offset = 0x00;
	for (const uint8_t& byte : fontset) {
		chip_mem[fontMemStart + offset++] = byte;
	}
	
	// Init SDL stuff
	rend.init();

	for (int i = 0; i < 100; i++) {
		rend.draw(disp);
	}
}

Chip8::~Chip8() {

	rend.quit();

	std::cout << "\n\n GOODBYE \n\n";
}

int Chip8::run() {
	bool running = true;

	while (running) {
		Chip8::fetch();
		Chip8::decode();
		Chip8::execute();
	}

	return 0;
}

void Chip8::fetch() {

}

void Chip8::decode() {

}

void Chip8::execute() {

}

int Chip8::load_rom(const char* fname) {
	unsigned offset = progMemStart;
	
	std::ifstream rom(fname, std::ios::binary);
	if (!rom.is_open()) {
		std::cout << "ROM NOT FOUND: " << fname << std::endl;
		return 1;
	}
	rom.seekg(0, rom.end);
	int len = rom.tellg();
	rom.seekg(0, rom.beg);
		
	if (len > 4096) {
		std::cout << "ROM TOO LARGE: " << len << std::endl;
		rom.close();
		return 2;
	}

	for (std::ifstream f(fname, std::ios::binary); f.good();) {
		chip_mem[offset++ & 0xFFF] = f.get();
	}

	rom.close();
	return 0;
}

int Chip8::print_mem() {
	unsigned count = 0;
	bool toggle = false;
	std::cout << "RAM hex values" << std::endl;
	for (const uint8_t& byte : chip_mem) {
		if (count > progMemStart && !toggle) {
			std::cout << "\n\n";
			std::cout << "PROGRAM MEMORY\n" << std::endl;
			toggle = true;
		}

		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
		if (count % 2 == 0) {
			std::cout << " ";
		}
		if (count % 6 == 0) {
			std::cout << std::endl;
		}
		count++;
	}
	std::cout << '\n';

	return 0;
}


int Chip8::print_font() {
	int count = 1;
	std::cout << "Font hex values" << std::endl;

	for (const uint8_t& byte : fontset) {
		std::cout << std::hex << static_cast<int>(byte) << " ";
		if (count % 5 == 0) {
			std::cout <<  std::endl;
		}
		count++;
	}
	std::cout << '\n';

	return 0;
}
