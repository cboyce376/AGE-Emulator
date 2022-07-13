#include <SDL2/SDL.h>
#include <string>
#include <cstring>

#include <iostream>
#include <iomanip>

#include "GPU.hpp"

#define HEX( x ) std::setw(2) << std::setfill('0') << std::hex << (0xFF & x)
#define HEX16( x ) std::setw(4) << std::setfill('0') << std::hex << (0xFFFF & x)

GPU::GPU(memory& mem) {
	m = &mem;
	
}

GPU::~GPU() {
	
}

int GPU::createScreen(std::string romName) {
	SDL_Init(SDL_INIT_VIDEO);
	//SDL_CreateWindow(romName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	
	renderer = SDL_CreateRenderer(SDL_CreateWindow("romName", 0, 0, 800, 720, SDL_WINDOW_SHOWN), -1, SDL_RENDERER_PRESENTVSYNC);
  	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 256, 256);// 160, 144);
	//    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	//memset(frameBuffer, 0x333333FF, 160*144*4);
	memset(bgfb, 0x000000FF, 256 * 256 * 4);
	
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //SDL_RenderClear(renderer);

    uint8_t* pPixels;
    int pitch = 0;
    //SDL_LockTexture(texture, nullptr, (void**)&pPixels, &pitch);

    // Render Game
    //uint8_t* pData = frameBuffer;
    //memcpy(pPixels, pData, 160 * 144 * 4);

    //SDL_UnlockTexture(texture);

    //SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // Update window
    //SDL_RenderPresent(renderer);
	
	return 1;
}

void GPU::scanLine() {
	std::cout << "GPU SCAN LINE\n";
	uint8_t lyVal = ((m->get8(0xff44) + 1) % 153);
	m->save8(0xFF44, lyVal);
	
	uint8_t LCDC = m->get8(0xff40);
	uint8_t bgsl = (LCDC & 0x10);
	
	/*
	uint8_t scrollY = m->get8(0xFF42);
	uint8_t scrollX = m->get8(0xFF43);
	uint8_t windowY = m->get8(0xFF4A);
	uint8_t windowX = m->get8(0xFF4B);
	*/
	
	uint16_t tileLocations = 0x8000;
	uint8_t tileSize = 0x10;
	
	int8_t offset;
	if (bgsl) {
		tileLocations = 0x8800;
		offset = -127;
	} else
		offset = 0;

	uint16_t map1Start = 0x9800;
	uint16_t map1End = 0x9BFF;
	
	std::cout << "Looping tile data\n";

	for (uint16_t tileID = map1Start; tileID <= map1End; tileID++) {

	uint16_t tileData = tileLocations + ((offset + m->get8(tileID)));// * tileSize);
	
	//uint16_t spriteData[8];
	if (tileID % 32 == 0) {
		std::cout << "\n";
	}
	std::cout << HEX(m->get8(tileID)) << " ";
	
	for (int i = 0; i < 8; i++) {
		uint8_t pixData = 0x00;
		uint32_t colour;
		pixData = (tileData & (0x1 << (8 + i))) >> (7 + i) | (tileData & (0x1 << i)) >> i;
		if (pixData == 0x00) {
			colour = 0xFFFFFFFF;
		} else if (pixData == 0x01) {
			colour = 0xCCCCCCFF;
		} else if (pixData == 0x02) {
			colour = 0x777777FF;
		} else if (pixData == 0x03) {
			colour = 0x000000FF;
		} else {
			std::cout << "Incorrect colour for the screen!!!";
			exit(0);	
		}
		
		memset(bgfb + (8 * (tileID - map1Start)), colour, 4);
	}
	
	if (lyVal == 143) {
		//IF |= 1;
		SDL_UpdateTexture(texture, 0, bgfb, 640);
		SDL_RenderCopy(renderer, texture, 0, 0);
		SDL_RenderPresent(renderer);
		//SDL_Event event;
		//while (SDL_PollEvent(&event))
		//	if (event.type == SDL_QUIT)
		//		return 0;
	}
	}
	std::cout << "\n";	

}

void GPU::getTileMap() {
	size_t map1Start = 0x9800;
	size_t map1End = 0x9BFF;
	
	size_t map2Start = 0x9C00;
	size_t map2End = 0x9FFF;
	
	uint8_t spriteMap[map1End - map1Start];
	
}


int GPU::renderScreen() {
	return 1;
}
