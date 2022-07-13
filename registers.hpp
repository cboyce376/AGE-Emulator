#ifndef REGISTERS_H
#define REGISTERS_H

#include <iostream>
#include <vector>

#include <string>
#include <map>

class registers
{
	public:
		registers();
		~registers();
	
		int set8Reg(std::string, uint8_t);
		int set16Reg(std::string, uint16_t);
		uint8_t get8Reg(std::string);
		uint16_t get16Reg(std::string);
	
		
		uint16_t getSP();
		uint16_t incSP();
		uint16_t decSP();
	
		uint16_t getPC();
		uint16_t incPC();
		uint16_t decPC();
	
	private:	
		struct Registers {
			uint16_t af;
			uint16_t bc;
			uint16_t de;
			uint16_t hl;
			uint16_t sp;
			uint64_t pc;
		};
	
		Registers r;	
	
		
};

#endif