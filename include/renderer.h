#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL2/SDL.h>

#include "screen.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	int init();
	int quit();

	int draw(Screen screen);

	int draw_screen(Screen screen);

private:
	SDL_Window* window;
	SDL_Renderer *renderer;

	
};

#endif // RENDERER_H_
