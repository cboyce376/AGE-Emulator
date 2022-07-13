#ifndef GPU_H
#define GPU_H


#include <SDL2/SDL.h>
#include <string>

#include "memory.hpp"

//#include <SDL2/SDL_opengl>

class GPU
{
	public:
	
		GPU(memory&);
		~GPU();
		int createScreen(std::string);
		int renderScreen();
		void scanLine();
		
		uint8_t frameBuffer[160 * 144 * 4];
	
		uint32_t bgfb[256 * 256];

	private:
		memory* m;
	
		void getTileMap();
	
		SDL_Renderer *renderer;
  		SDL_Texture *texture;

		
};

#endif