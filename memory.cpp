#include "memory.hpp"

#include <iostream>
#include <iomanip>

memory::memory() {

}

memory::~memory() {
	
}

void memory::save16(uint32_t location, uint16_t data) {
	save8(location, (uint8_t)data>>8);
	save8(location + 1, (uint8_t)data);
}

void memory::save8(uint32_t location, uint8_t data) {
	if (location == 0x9800) {
		std::cout << "Setting 0x9800 to be: " << std::hex << data; 
	}
	if (location > 65535) {
		exit(0);
	}
	mem[location] = data;
}

uint8_t memory::get8(uint32_t location) {
	return uint8_t(mem[location]);
}
