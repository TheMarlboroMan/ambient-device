#ifndef APP_STYLE_H
#define APP_STYLE_H

#include <string>

#include <video/color/color.h>
#include <video/rect/rect.h>

namespace app {

class style {

	public:
						style(int, int, int, int, int, unsigned int, unsigned int, ldv::rgba_color);

	int					get_clock_font_size() const {return clock_font_size;}
	int					get_secondary_font_size() const {return secondary_font_size;}
	int					get_idle_font_size() const {return idle_font_size;}
	int					get_clock_vertical_margin() const {return clock_vertical_margin;}
	int					get_clock_horizontal_margin() const {return clock_horizontal_margin;}
	ldv::rgba_color		get_clock_font_color() const {return clock_font_color;}
	//TODO: Implement it separatedly.
	ldv::rgba_color		get_secondary_font_color() const {return clock_font_color;}
	ldv::rect			get_container_box() const {return ldv::rect{0, 0, container_w, container_h};}

	private:

	int					clock_font_size,
						secondary_font_size,
						idle_font_size,
						clock_vertical_margin,
						clock_horizontal_margin;
	unsigned int		container_w,
						container_h;

	ldv::rgba_color		clock_font_color;
};

}

#endif
