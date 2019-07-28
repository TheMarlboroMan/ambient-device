#include "size_calculator.h"

using namespace app;

//TODO: Erase
#include <iostream>

void size_calculator::calculate(rect _pic, rect _canvas, rect& _pos, rect& _clip) {

	//TODO: actually, this is likely something like "image to screen ratio"...

	//...center,
	auto pt=center(_pic, _canvas);
	_pos.x=pt.x;
	_pos.y=pt.y;

	//...after centering, pos.x and pos.y are the differences from the canvas.
	//If both are zero, the image and the canvas are the same: nothing 
	//must be done.
	if(_pos.x==0 && _pos.y==0) {
		_pos=_pic;
		_clip=_canvas;
		return;	
	}

	//And now the generalization... We care only about filling the screen, so
	//we must see which magnitude must be adjusted to the canvas (the other
	//will be adjusted in proportion). 

	double ratio=(double)_pic.w / (double)_pic.h;

std::cout<<_pos.x<<" "<<_pos.y<<" ratio "<<ratio<<std::endl;

//TODO: I dont think this works on "abs"...
//w diff is larger than h diff (which is... negative), so it follows that we
//must make w be screen-wide and clip h. That is "resize horiz".

//TODO: Nope... we need to clip to the screen!

	if(_pos.x >= _pos.y) {

std::cout<<"vertical will be clipped"<<std::endl;

		//TODO: Soooooo... something about rule of threes.

		//must adjust the horizontal axis.
		_clip.w=_canvas.w < _pic.w ? _canvas.w : _pic.w;
		_clip.h=(double)_clip.w / ratio;
	}
	else {
std::cout<<"horizontal will be clipped"<<std::endl;
		//must adjust the vertical axis.
		_clip.h=_canvas.h < _pic.h ? _canvas.h : _pic.h;
		_clip.w=(double)_clip.h / ratio;
	}

	//TODO: sooooo... 

//TODO: More terrible stuff: clip as much as you'd like, the pic might be smaller than the display!

//FUCKING SURPRISE: Clipping to the monitor :/...
std::cout<<"will clip to "<<_clip.w<<" x "<<_clip.h<<std::endl;

	//TODO: Where are "clip.x" and "clip.y"??
	pt=center(_clip, _canvas);
	_clip.x=pt.x;
	_clip.y=pt.y;

	_pos=rect(0, 0, _canvas.w, _canvas.h);

	//TODO.
	//_clip=rect(0,0,_pic.w, _pic.h);	
}

point size_calculator::center(rect _pic, rect _canvas) {

	int x=(_canvas.x+_canvas.w / 2)-(_pic.w/2);
	int y=(_canvas.y+_canvas.h / 2)-(_pic.h/2);

	return point{x,y};
}

