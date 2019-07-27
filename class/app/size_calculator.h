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



class size_calculator {

	public:

	void	calculate(rect _pic, rect _canvas, rect& _pos, rect& _clip) {
		//TODO: center,
		//Calculate...
		//Recenter.

		_pos=rect(0, 0, _canvas.w, _canvas.h);
		_clip=rect(0, 0, _pic.w, _pic.h);
	};

	private:
};

}

#endif
