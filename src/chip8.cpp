#include "../include/chip8.h"

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <random>


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

	for (int i = 0; i < 200; i++) {
		rend.draw(disp);
	}
}

Chip8::~Chip8() {

	rend.quit();

	std::cout << "\n\n GOODBYE \n\n";
}

int Chip8::run() {
	bool running = true;

	for (int i = 0; i < 50000; i++) {
		Chip8::fetch();
		Chip8::decode();
		Chip8::execute();
	}
/*
	while (running) {
		// TODO make this loop execute at 60Hz
		Chip8::fetch();
		Chip8::decode();
		Chip8::execute();
	}
*/
	return 0;
}

void Chip8::fetch() {
	currOP = 0x0000;
	
	currOP = (chip_mem[chip_PC] << 8 | chip_mem[chip_PC + 1]);

	chip_PC += 2;
}

void Chip8::decode() {
	opFields.type = (currOP & 0xf000) >> 12;
	opFields.x    = (currOP & 0x0f00) >>  8;
	opFields.y    = (currOP & 0x00f0) >>  4;
	opFields.n    = (currOP & 0x000f);

	opFields.nn   = (currOP & 0x00ff);
	opFields.nnn  = (currOP & 0x0fff);
}


void Chip8::execute() {

	switch (opFields.type) {
		case 0x0:
			if (opFields.nnn == 0x0E0) {  // 00E0 - clear screen
				disp.reset_screen();
			} else if (opFields.nnn == 0x0EE) { // 00EE - return from subroutine
				chip_PC = chip_stack.top();
				chip_stack.pop();
			} 
			break;
		case 0x1:
			// 1NNN - jump to NNN
			chip_PC = opFields.nnn;
			break;
		case 0x2:
			// 2NNN - go to subroutine
			chip_stack.push(chip_PC);
			chip_PC = opFields.nnn;
			break;
		case 0x3:
			// 3XNN skip instruction if NN = VX
			if (opFields.nn == registers[opFields.x]) {
				chip_PC += 2;
			}
			break;
		case 0x4:
			// 4XNN skip instruction if NN != VX
			if (opFields.nn != registers[opFields.x]) {
				chip_PC += 2;
			}
			break;
		case 0x5:
			// 5XY0 skip if VX == VY
			if (registers[opFields.x] == registers[opFields.y]) {
				chip_PC += 2;
			}
			break;
		case 0x6:
			// 6XNN - set VX to NN
			registers[opFields.x] = opFields.nn;
			break;
		case 0x7:
			// 7XNN - add NN to VX
			registers[opFields.x] += opFields.nn;
			break;
		case 0x8:
			switch (opFields.n) {
				case 0x0: // 8XY0 set VX to VY
					registers[opFields.x] = registers[opFields.y];
					break;
				case 0x1: // 8XY1 VX = VX | VY
					registers[opFields.x] |= registers[opFields.y];
					break;
				case 0x2: // 8XY2 VX = VX & VY
					registers[opFields.x] &= registers[opFields.y];
					break;
				case 0x3: // 8XY3 VX = VX xor VY
					registers[opFields.x] ^= registers[opFields.y];
					break;
				case 0x4: // 8XY4 VX = VX + VY
					{
						unsigned x = (unsigned)registers[opFields.x] + (unsigned)registers[opFields.y];
						registers[0xf] = (x > 0xff) ? 1 : 0;
						registers[opFields.x] += registers[opFields.y];
						break;
					}
				case 0x5: // 8XY5 VX = VX - VY also set Vf if X > Y 
					registers[0xf] = (registers[opFields.x] > registers[opFields.y]) ? 1 : 0;
					registers[opFields.x] -= registers[opFields.y];
					break;
				case 0x6: // 8XY6 shift VX to the right 
					registers[0xf] = registers[opFields.x] & 0x1;
					registers[opFields.x] >>= 1;
					break;
				case 0x7: // 8XY7 VX = VY - VX also set VF if Y > X
					registers[0xf] = (registers[opFields.y] > registers[opFields.x]) ? 1 : 0;
					registers[opFields.x] = registers[opFields.y] - registers[opFields.x];
					break;
				case 0xe: // 8XYE shift VX to the left
					registers[0xf] = registers[opFields.x] & 0x8;
					registers[opFields.x] <<= 1;
					break;
			}
			break;
		case 0x9: // 9XY0 - skip
			if (registers[opFields.x] != registers[opFields.y]) {
				chip_PC += 2;
			}
			break;
		case 0xA: // ANNN - set index to NNN
			chip_I = opFields.nnn;
			break;
		case 0xB: // BNN - jump with offset
			chip_PC = opFields.nnn + registers[0];
			break;
		case 0xC: // CXNN - random number & NN
			registers[opFields.x] = chip_random() & opFields.nn;
			break;
		case 0xD: // DXYN
			{
				unsigned x = registers[opFields.x] & 63;
				unsigned y = registers[opFields.y] & 31;
				registers[0xF] = 0;

				for (uint8_t row = 0; row < opFields.n; row++) {
					unsigned sprite = chip_mem[chip_I + row];
					for (uint8_t col = 0; col < 8; col++) {
						// if the current bit is on in the sprite
						if ((sprite & 0x80) != 0) {
							// toggle the pixel - if it was on already set Vf to 1
							if (disp.pixels[x + col][y + row] != 0) {
								registers[0xF] = 1;
							}
							disp.pixels[x + col][y + row] ^= 1;
						}
						if (x + col >= disp.screen_w) {
							break;
						}
					// shift the sprite one to the left	
					sprite <<= 1;
					}
					if (y + row >= disp.screen_h) {
						break;
					}
				}
				rend.draw(disp);
				break;
			}
		case 0xE: // TODO INPUT
			break;
		case 0xF: 
			switch (opFields.nn) {
				case 0x07: // FX07 - set VX to delay timer
					registers[opFields.x] = delayTimer;
					break;
				case 0x0A: // FX0A - get key TODO
					chip_PC -= 2;
					break;
				case 0x15: // FX15 - set delay timer to VX
					delayTimer = registers[opFields.x];
					break;
				case 0x18: // FX18 - set sound timer to VX
					soundTimer = registers[opFields.x];
					break;
				case 0x1E: // FX1E - add to index
					chip_I += registers[opFields.x];
					registers[0xf] = chip_I > 0xfff ? 1 : 0;
					break;
				case 0x29: // FX29 Font char
					chip_I = fontMemStart + registers[opFields.x] * 5;
					break;
				case 0x33: // FX33 Binary coded decimal conversion 
					{
						uint8_t x = registers[opFields.x];
						chip_mem[(chip_I + 0) & 0xfff] = x / 100;
						chip_mem[(chip_I + 1) & 0xfff] = (x % 100) / 10;
						chip_mem[(chip_I + 2) & 0xfff] = x % 10;
						break;
					}
				case 0x55: // FX55 - Store memory
					std::cout << "CHIP I at: " << chip_I << std::endl;
					std::cout << "op field is : " << opFields.x << std::endl;
					for (unsigned i = 0; i <= opFields.x; i++) {
						std::cout << "i is " << i << std::endl;
						chip_mem[((chip_I + i) & 0xfff)] = registers[i];
					}
					break;
				case 0x65: // FX65 - Load memory
					for (unsigned i = 0; i <= opFields.x; i++) {
						registers[i] = chip_mem[((chip_I + i) & 0xfff)];
					}
					break;
			}
	}

	delayTimer--;
	soundTimer--;
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

uint8_t Chip8::chip_random() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<uint8_t> dist(0, 255);

	return dist(rng);
}
