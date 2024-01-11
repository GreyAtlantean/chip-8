#include "../include/screen.h"
#include <vector>


Screen::Screen(int width, int height) : pixels(width, std::vector<short>(height))  {
	screen_w = width;
	screen_h = height;
}

void Screen::reset_screen() {
	for (int y = 0; y < screen_h; y++) {
		for (int x = 0; x < screen_w; x++) {
			pixels[x][y] = 0;
		}
	}
}
