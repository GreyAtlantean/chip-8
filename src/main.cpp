#include <iostream>

#include "../include/renderer.h"

int main (int argc, char *argv[]) {

	Renderer app;
	app.init();

	for (int i = 0; i < 100; i++) {

	app.draw();
	}
	app.quit();
	
	std::cout << "test" << std::endl;

	return 0;
}
