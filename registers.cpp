#include "registers.hpp"

#include <iomanip>

#define HEX( x ) std::setw(2) << std::setfill('0') << std::hex << (0xFF & x)
#define HEX16( x ) std::setw(4) << std::setfill('0') << std::hex << (0xFFFF & x)


registers::registers() {
	r.af = 0x0000;
	r.bc = 0x0013;
	r.de = 0x00D8;
	r.hl = 0x014D;
	r.pc = 0x0100;
	r.sp = 0xFFFE;
	std::cout << "Registers Created\n";
}

registers::~registers() {
}

int registers::set8Reg (std::string regName, uint8_t regValue) {
	switch (regName[0]) {
		case ('a'):
			r.af = r.af & 0x00FF;
			r.af = r.af | (regValue << 8);
			return 0;
		break;
		case ('f'):
			r.af = r.af & 0xFF00;
			r.af = r.af | regValue;
			return 0;
		break;
		case ('b'):
			r.bc = r.bc & 0x00FF;
			r.bc = r.bc | regValue << 8;
			return 0;
		break;
		case ('c'):
			r.bc = r.bc & 0xFF00;
			r.bc = r.bc | regValue;
			return 0;
		break;
		case ('d'):
			r.de = r.de & 0x00FF;
			r.de = r.de | regValue << 8;
			return 0;
		break;
		case ('e'):
			r.de = r.de & 0xFF00;
			r.de = r.de | regValue;
			return 0;
		break;
		case ('h'):
			r.hl = r.hl & 0x00FF;
			r.hl = r.hl | regValue << 8;
			return 0;
		break;
		case ('l'):
			std::cout << "hl before: " << HEX16(r.hl) << "\n";
			r.hl = r.hl & 0xFF00;
			r.hl = r.hl | regValue;
			std::cout << "hl after: " << HEX16(r.hl) << "\n";
			return 0;
		break;
		default:
			std::cout << "Trying to set an invalid register name: " << regName << "\n";
			exit(0);
	}
	return 1;
}

int registers::set16Reg (std::string regName, uint16_t regValue) {
	if (regName == "SP" || regName == "sp") {
		r.sp = regValue;
		return 0;
	} else if (regName == "PC" || regName == "pc") {
		r.pc = regValue;
		return 0;
	} else if (regName == "AF" || regName == "af") {
		r.af = regValue;
		return 0;
	} else if (regName == "BC" || regName == "bc") {
		r.bc = regValue;
		return 0;
	} else if (regName == "DE" || regName == "de") {
		r.de = regValue;
		return 0;
	} else if (regName == "HL" || regName == "hl") {
		r.hl = regValue;
		return 0;
	}
	return 1;
}

uint8_t registers::get8Reg (std::string regName) {
	switch (regName[0]) {
		case ('a'):
			return r.af >> 8;
		break;
		case ('f'):
			return r.af & 0xFF;
		break;
		case ('b'):
			return r.bc >> 8;
		break;
		case ('c'):
			return r.bc & 0xFF;
		break;
		case ('d'):
			return r.de >> 8;
		break;
		case ('e'):
			return r.de & 0xFF;
		break;
		case ('h'):
			return r.hl >> 8;
		break;
		case ('l'):
			return r.hl & 0xFF;
		break;
		default:
			std::cout << "Trying to set an invalid register name: " << regName << "\n";
			exit(0);
	}
	return 1;
}

uint16_t registers::get16Reg (std::string regName) {
	if (regName == "SP" || regName == "sp") {
		return r.sp;
	} else if (regName == "PC" || regName == "pc") {
		return r.pc;
	} else if (regName == "AF" || regName == "af") {
		return r.af;
	} else if (regName == "BC" || regName == "bc") {
		return r.bc;
	} else if (regName == "DE" || regName == "de") {
		return r.de;
	} else if (regName == "HL" || regName == "hl") {
		return r.hl;
	}
	return 1;
}

uint16_t registers::getSP() {
	return r.sp;	
}

uint16_t registers::incSP() {
	return ++r.sp;	
}

uint16_t registers::decSP() {
	return --r.sp;	
}

uint16_t registers::getPC() {
	return r.pc;	
}

uint16_t registers::incPC() {
	return ++r.pc;	
}

uint16_t registers::decPC() {
	return --r.pc;	
}