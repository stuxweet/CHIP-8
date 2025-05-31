#include "chip8.h"
#include <iostream>

Chip8::Chip8()
{
	// initialize registers and memory
	for (int i = 0; i < 4096; ++i) {
		memory[i] = 0;
	}

	for (int i = 0; i < 16; ++i) {
		V[i] = 0;
		stack[i] = 0;
		keypad[i] = 0;
	}

	for (int i = 0; i < 64 * 32; ++i) {
		display[i] = 0;
	}

	I = 0;
	sp = 0;
	pc = 0x200;

	opcode = 0;

	delay_timer = 0;
	sound_timer = 0;

	// load fontset into memory
	uint8_t fontset[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	for (int i = 0; i < 80; ++i) {
		memory[i] = fontset[i];
	}
}

Chip8::~Chip8()
{
}

void Chip8::loadROM(const std::string& filename)
{
	FILE* rom = fopen(filename.c_str(), "rb");

	if (!rom) {
		std::cerr << "Error: could not open the ROM file: " << filename << std::endl;
		return;
	}

	fseek(rom, 0, SEEK_END); // move to the end of the file
	long size = ftell(rom);  // get the size
	rewind(rom);			 // rewind to the beginning

	if (size > 4096 - 512) { // 4096 is the total memory size, 512 is the reserved space for the interpreter
		std::cerr << "ROM size exceeds memory limit." << std::endl;
		fclose(rom);
		return;
	}

	// load ROM into memory, starting address = 0x200 in Chip-8
	// 0x200 is the standard starting address for CHIP-8 programs
	// there's no need to allocate memory dynamically since we have a fixed-size array
	size_t read = fread(&memory[0x200], 1, size, rom);
	if (read != size) {
		std::cerr << "Error reading the ROM file." << std::endl;
	}

	fclose(rom);
}
