#include "idle.h"

//local
#include "../input.h"

using namespace app;

controller_idle::controller_idle(tools::log& _log, 
	tools::ttf_manager& _ttf_manager, 
	const app::app_config& _app_config,
	const app::style& _style)
	:log(_log)
	,ttf_manager(_ttf_manager)
	,config(_app_config)//TODO: Is this useful still???
	,style(_style)
{

}

void controller_idle::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input_app::escape)) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input_app::space)) {
		set_state(t_states::state_ambient);
	}
}

void controller_idle::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 0));

	int font_size=style.get_idle_font_size();
	assert(ttf_manager.exists("default", font_size));
	ldv::ttf_representation fps_text{
		ttf_manager.get("default", font_size), 
		ldv::rgba8(255, 255, 255, 255), 
		"Press space..."
	};

	fps_text.align(
		_screen.get_rect(), {
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

	

	fps_text.draw(_screen);
}
