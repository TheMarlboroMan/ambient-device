#include <app/size_calculator.h>

using namespace app;

#include <iostream>

double size_calculator::ratio(double _w, double _h) const {

	return _w / _h;
}

//!Calculates the _pos and _clip of a picture so it fills the full canvas (
//!wich would be the display).
void size_calculator::fill(rect _pic, rect _canvas, rect& _pos, rect& _clip) {

	//Position: this is easy: all we want is for the picture to take the full
	//canvas.
	_pos=_canvas;

	auto calculate=[this](
		unsigned int _pic_dim,
		unsigned int _pic_cross_dim,
		unsigned int _canvas_dim,
		unsigned int _canvas_cross_dim,
		unsigned int &_clip_dim,
		unsigned int &_clip_cross_dim,
		int &_clip_pos,
		int &_clip_cross_pos
	) {
		double scale=ratio(_pic_dim, _canvas_dim);
		unsigned display_cross_dim=_canvas_cross_dim * scale;

		//Clipping to full dimension... The cross dimension? The display one.
		_clip_dim=_pic_dim;
		_clip_cross_dim=display_cross_dim;
		_clip_pos=0;

		//And center.
		_clip_cross_pos=(_pic_cross_dim - display_cross_dim) / 2;
	};

	//Ratio relationship will tell us the dominant dimension of the picture.
	double 	image_ratio=ratio(_pic.w, _pic.h),
			display_ratio=ratio(_canvas.w, _canvas.h);

	//This means that we need to use the full height for the clip and center in
	//the width....
	if(image_ratio > display_ratio) {

		calculate(_pic.h, _pic.w, _canvas.h, _canvas.w, _clip.h, _clip.w, _clip.y, _clip.x);
	}
	//Here we work with the height.
	else {

		calculate(_pic.w, _pic.h, _canvas.w, _canvas.h, _clip.w, _clip.h, _clip.x, _clip.y);
	}
}

void size_calculator::letterbox(rect _pic, rect _canvas, rect& _pos, rect& _clip) {

	//This is easy: we want the full picture...
	_clip=_pic;

	//This should be always the same: in a portrait picture we want the pic height
	//to be scaled to the display height and the width to adapt. On landscape
	//pics we want the pic width to be scaled to the display width and the
	//height to adapt.

	double 	display_ratio=ratio(_canvas.w, _canvas.h),
			image_ratio=ratio(_pic.w, _pic.h);

	double scale=display_ratio > image_ratio
		? ratio(_canvas.h, _pic.h)
		: ratio(_canvas.w, _pic.w);

	_pos.w=(double) _pic.w * scale;
	_pos.h=(double) _pic.h * scale;

	//portrait
	if(display_ratio > image_ratio) {
		_pos.y=0;
		_pos.x=(_canvas.w - _pos.w) / 2;
	}
	//landscape
	else {
		_pos.y=(_canvas.h - _pos.h) / 2;
		_pos.x=0;
	}
}
