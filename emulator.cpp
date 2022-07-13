
#include "emulator.hpp"


emulator::emulator() {
	cpu = std::make_unique<CPU>();
}

emulator::~emulator() {
	
}

int emulator::init(std::string path) {
	cpu->loadROM(path);
	return 0;
}

void emulator::execute() {
	while (cpu->executing) {
		cpu->readInstruction();
	}
}