#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>


class memory
{
	public:
		memory();
		~memory();
	
		void save16(uint32_t ,uint16_t);
		void save8(uint32_t ,uint8_t);
		uint8_t get8(uint32_t location);
	
		uint8_t mem[65535];
	
	private:
		//uint8_t mem[65535];
};

#endif