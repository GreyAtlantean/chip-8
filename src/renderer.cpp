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

int Renderer::draw() {
	SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
	
	SDL_Delay(3);

	return 0;
}

int Renderer::quit() {
	
	SDL_Delay(2000);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
