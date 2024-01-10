#include <SDL2/SDL.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	int init();
	int quit();

	int draw();
	

private:
	SDL_Window* window;
	SDL_Renderer *renderer;

	
};

