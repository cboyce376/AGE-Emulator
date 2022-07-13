#include <iostream>
#include <map>
#include <string>

#include "emulator.hpp"

int main(int argc, char* argv[]) {
	std::string path = argv[1];
	emulator emulator;
	emulator.init(path);
	emulator.execute();
	
	

	return 0;
}
