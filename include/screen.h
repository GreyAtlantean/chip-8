#ifndef SCREEN_H_
#define SCREEN_H_

#include <vector>

class Screen {
	public:
		Screen(int width, int height);		
		std::vector<std::vector<short>> pixels;
	private:
		int screen_w;
		int screen_h;
};


#endif
