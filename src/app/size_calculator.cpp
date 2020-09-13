#include <app/size_calculator.h>

using namespace app;

#include <iostream>

double size_calculator::ratio(double _w, double _h) const {

	return _w / _h;
}

//TODO: Look, same implementation as ratio XD!.
double size_calculator::scale_factor(double _a, double _b) const {

	return _a / _b;
}

void size_calculator::fill(rect _pic, rect _canvas, rect& _pos, rect& _clip) {

	//Position: this is easy: all we want is for the picture to take the full
	//canvas.
	_pos=_canvas;

	//Ratio relationship will tell us the dominant dimension of the picture.
	double 	image_ratio=ratio(_pic.w, _pic.h),
			display_ratio=ratio(_canvas.w, _canvas.h);

	//This means that we need to use the full height for the clip and center in
	//the width....
	if(image_ratio > display_ratio) {

		//Scale the display according to the picture h.
		double scale=scale_factor(_pic.h, _canvas.h);
		auto display=rect(0, 0, _canvas.w * scale, _canvas.h * scale);

		//Clipping to full height... The width? The display one.
		_clip.h=_pic.h;
		_clip.w=display.w;
		_clip.y=0;

		//Aaaaand center.
		_clip.x=(_pic.w - display.w) / 2;
	}
	//Here we work with the height.
	else {
		//TODO: Good, now refactor this shit and make it as generic as possible,
		//since the algorithm is... the same :/.

		//Scale the display according to the picture h.
		double scale=scale_factor(_pic.w, _canvas.w);
		auto display=rect(0, 0, _canvas.w * scale, _canvas.h * scale);

		//Clipping to full height... The width? The display one.
		_clip.w=_pic.w;
		_clip.h=display.h;
		_clip.x=0;

		//Please, center...
		_clip.y=(_pic.h - display.h) / 2;
	}

}

void size_calculator::letterbox(rect _pic, rect _canvas, rect& _pos, rect& _clip) {

	//The clip is the full picture...
	_clip=_pic;

	//The thing now is to calculate the position...
	//TODO.


	return fill(_pic, _canvas, _pos, _clip);
}
