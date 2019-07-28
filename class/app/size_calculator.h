#ifndef APP_SIZE_CALCULATOR_H
#define APP_SIZE_CALCULATOR_H

namespace app {

struct rect {

			rect(int _x=0, int _y=0, unsigned _w=0, unsigned _h=0)
		:x(_x), y(_y), w(_w), h(_h) {
	}

	int		x, y;
	unsigned	w, h;
};

struct point {
	int		x, y;
};

class size_calculator {

	public:

	//!Writes to _pos and _clip so the image fills the display.
	void	fill(rect _pic, rect _canvas, rect& _pos, rect& _clip);
	//!Writes to _pos and _clip so the image is letterboxes in the display.
	void	letterbox(rect _pic, rect _canvas, rect& _pos, rect& _clip);

	private:

};

}

#endif
