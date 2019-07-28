#include "size_calculator.h"

using namespace app;

//TODO: Erase
#include <iostream>

void size_calculator::fill(rect _pic, rect _canvas, rect& _pos, rect& _clip) {

	//This is the ratio we must have for our image...
	double display_ratio=(double)_canvas.w / (double)_canvas.h;

	_clip.w=_pic.w;
	_clip.h=_pic.h;

	//We must get the smallest dimension for the picture, so that's the one
	//that fills up the display. So far, we just adjust so that we clip on 
	//the smallest dimension.
	if(_pic.w < _pic.h) { //A high- picture...

		_clip.h=(double)_clip.w / display_ratio;
		_clip.y=(_pic.h - _clip.h) / 2;
	}
	else { //A wide picture...

		_clip.w=(double)_clip.h * display_ratio;
		_clip.x=(_pic.w - _clip.w) / 2;
	}

	//The beautiful part is that openGL will resize the image so that it fits:
	//no need for us to calculate new width / height.

	//This just must be done... There's no calculation involved.
	_pos=rect(0, 0, _canvas.w, _canvas.h);
}

void size_calculator::letterbox(rect _pic, rect _canvas, rect& _pos, rect& _clip) {

	//The clip is the full picture...
	_clip=_pic;

	//The thing now is to calculate the position...
	//TODO.


	return fill(_pic, _canvas, _pos, _clip);
}
