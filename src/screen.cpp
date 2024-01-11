#include "../include/screen.h"


Screen::Screen(int width, int height) : pixels(width, std::vector<short>(height))  {
	screen_w = width;
	screen_h = height;
}
