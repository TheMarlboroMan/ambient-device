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

	void	calculate(rect _pic, rect _canvas, rect& _pos, rect& _clip);

	private:

	point	center(rect _pic, rect _canvas);
};

}

#endif
