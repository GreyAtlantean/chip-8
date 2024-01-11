#ifndef SCREEN_H_
#define SCREEN_H_

#include <vector>

class Screen {
	public:
		Screen(int width, int height);		
		std::vector<std::vector<short>> pixels;
		
		void reset_screen();
		unsigned screen_w;
		unsigned screen_h;
};


#endif
