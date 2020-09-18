#include <controllers/idle.h>
#include <input/input.h>

using namespace app;


controller_idle::controller_idle(
	lm::logger& _log,
	ldtools::ttf_manager& _ttf_manager,
	const app::app_config& _app_config,
	const app::style& _style,
	app::clock& _clock
):
	log(_log),
	ttf_manager(_ttf_manager),
	config(_app_config),
	style(_style),
	clock(_clock),
	clock_rep{
		ttf_manager.get("default", style.get_idle_font_size()),
		ldv::rgba8(255, 255, 255, 255),
		clock.get_time(true)
	}
{
	_clock.subscribe_second("idle_tick", std::bind(&controller_idle::second_tick, this));
}

void controller_idle::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input_app::escape)) {
		set_leave(true);
		return;
	}

	if(update_clock) {

		clock_rep.set_text(clock.get_time(true));
		update_clock=false;
	}

	if(_input.is_input_down(input_app::space)) {
		set_state(t_states::state_ambient);
	}
}

void controller_idle::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 0));

	const int font_size=style.get_idle_font_size();
	assert(ttf_manager.exists("default", font_size));

	ldv::ttf_representation user_prompt{
		ttf_manager.get("default", font_size),
		ldv::rgba8(255, 255, 255, 255),
		"Press space..."
	};

	user_prompt.align(
		_screen.get_rect(), {
			ldv::representation_alignment::h::inner_right,
			ldv::representation_alignment::v::inner_bottom,
			10, 10
		}
	);

	user_prompt.draw(_screen);

	clock_rep.align(
		_screen.get_rect(), {
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

	clock_rep.draw(_screen);
}

void controller_idle::second_tick() {

	//This method is called from another thread, so there's no point on
	//trying to work with the textures without invoking races and openGL
	//problems. Let's just raise a flag.

	update_clock=true;
}
