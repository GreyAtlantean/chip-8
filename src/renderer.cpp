#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>

#include "../include/renderer.h"




Renderer::Renderer() {
	
}

Renderer::~Renderer() {
}

int Renderer::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL Init Error:" << SDL_GetError() << std::endl;
		return 1;			
	}
	
	window = SDL_CreateWindow(
		"chip-8",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		1920,
		1080, 
		0
	);

	if (window == nullptr) {
		std::cout << "SDL Window Error:" << SDL_GetError() << std::endl;
		return 1;			
	}

	renderer = SDL_CreateRenderer(
		window,
		-1, 
		SDL_RENDERER_ACCELERATED
	);

	if (renderer == nullptr) {
		std::cout << "SDL Renderer Error:" << SDL_GetError() << std::endl;
		return 1;			
	}

	return 0;

}

int Renderer::draw(Screen screen) {
	SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	draw_screen(screen);

	SDL_RenderPresent(renderer);
	
	SDL_Delay(3);

	return 0;
}

int Renderer::draw_screen(Screen screen) {
	int tile_size = 15;

	SDL_Rect tile;

	tile.x = 0;
	tile.y = 0;
	tile.w = tile_size;
	tile.h = tile_size;

	SDL_Colour on  = {0xCC, 0xCC, 0x11, SDL_ALPHA_OPAQUE};
	SDL_Colour off = {0x66, 0x66, 0x00, SDL_ALPHA_OPAQUE};

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			tile.x = x * tile_size;
			tile.y = y * tile_size;
			
			SDL_Colour c;
			if (screen.pixels[x][y]) {
				c = on;	
			} else {
				c = off;
			}

			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
			SDL_RenderFillRect(renderer, &tile);
		}
	}
	return 0;
}

int Renderer::quit() {
	
	SDL_Delay(5000);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
