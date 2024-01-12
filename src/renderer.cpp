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
	
	SDL_Colour on  = {0xFF, 0xFF, 0x33, SDL_ALPHA_OPAQUE};
	SDL_Colour off = {0x88, 0x88, 0x22, SDL_ALPHA_OPAQUE};

	int tile_size = 15;

	SDL_Rect tile;

	tile.x = 0;
	tile.y = 0;
	tile.w = 64 * tile_size;
	tile.h = 32 * tile_size;
	
	SDL_SetRenderDrawColor(renderer, off.r, off.g, off.b, off.a);
	SDL_RenderFillRect(renderer, &tile);

	SDL_SetRenderDrawColor(renderer, on.r, on.g, on.b, on.a);

	tile.w = tile_size;
	tile.h = tile_size;

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			tile.x = x * tile_size;
			tile.y = y * tile_size;
	
			if (screen.pixels[x][y]) {
				SDL_RenderFillRect(renderer, &tile);
			}

		}
	}
	return 0;
}

int Renderer::quit() {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
