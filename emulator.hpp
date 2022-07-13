#ifndef EMULATOR_H
#define EMULATOR_H

#include <memory>


#include "CPU.hpp"

class emulator
{
	public:
		emulator();
		~emulator();
		int init(std::string);
		void execute();
	private:
		std::unique_ptr<CPU> cpu;
	
};


#endif