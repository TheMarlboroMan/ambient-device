#include <app/style.h>

using namespace app;

style::style(
	int _clock, 
	int _secondary, 
	int _idle,
	int _clock_vertical_margin, 
	int _clock_horizontal_margin,
	unsigned int _container_w, 
	unsigned int _container_h,
	ldv::rgba_color _clock_color
)
	:clock_font_size{_clock},
	secondary_font_size{_secondary}, 
	idle_font_size{_idle},
	clock_vertical_margin{_clock_vertical_margin},
	clock_horizontal_margin{_clock_horizontal_margin},
	container_w{_container_w},
	container_h{_container_h},
	clock_font_color{_clock_color}
	{

}
