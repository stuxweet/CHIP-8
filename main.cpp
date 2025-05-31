#include <iostream>
#include "chip8.h"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <rom_file>" << std::endl;
		return 1;
	};

	std::string romFile = argv[1];
	Chip8 chip8;
	chip8.loadROM(romFile);
}

