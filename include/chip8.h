#ifndef CHIP8_H_
#define CHIP8_H_

#include <array>
#include <cstdint>
#include <stack>

#include "instruction.h"
#include "renderer.h"
#include "screen.h"

class Chip8 {
	public:
		Chip8();
		~Chip8();

		int run_chip();
		int load_rom(const char* fname);

		int print_mem();
		int print_font();

		int run();

	private:
		std::array<uint8_t, 4096> chip_mem = {}; // RAM
		std::array<uint8_t,   16> registers = {}; // Chip registers

		uint16_t chip_PC; // Program Counter	
		uint16_t chip_I;  // Index Register		
		
		uint8_t delayTimer; // Delay timer
		uint8_t soundTimer; // Sound timer
	
		uint16_t currOP; // current opcode
		instruction_fields opFields; // fields for the opcode
		
		std::stack<uint16_t> chip_stack = {}; // Address stack
		std::vector<uint8_t> fontset;

		void fetch();
		void decode();
		void execute();

		unsigned fontMemStart = 0x50;
		unsigned progMemStart = 0x200;

		Screen disp; // Display
		Renderer rend; // Renderer
		
		uint8_t chip_random();

		uint8_t chip_keys[16];

		void handle_input();
		void clear_keys();
		bool is_a_key_pressed();
		
		uint8_t get_key();
		
		bool key_released;
		
		bool should_quit;
	
};

#endif // CHIP8_H_
