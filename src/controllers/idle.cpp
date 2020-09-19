#include <controllers/idle.h>
#include <input/input.h>

#include <tools/file_utils.h>
#include <tools/json.h>

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

	//TODO: This is repeated... could have app::tools::load_menu.
	std::string config_file_path=app::get_data_dir();
				config_file_path+="app/menu.json";

	auto json_document=tools::parse_json_string(
		tools::dump_file(config_file_path)
	);

	tools::options_menu_from_json(
		json_document["main_menu"],
		menu
	);

	menu_translation["10_start"]="Start";
	menu_translation["20_settings"]="Settings";
	menu_translation["30_exit"]="Exit";
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

	if(_input.is_input_down(input_app::down)) {

		current_key=menu.adjacent_key(current_key, decltype(menu)::browse_dir::next);
	}
	else if(_input.is_input_down(input_app::up)) {

		current_key=menu.adjacent_key(current_key, decltype(menu)::browse_dir::previous);
	}
	else if (_input.is_input_down(input_app::space)) {

		if(current_key=="10_start") {
			set_state(t_states::state_ambient);
		}
		else if(current_key=="20_settings") {
			set_state(t_states::state_settings);
		}
		else if(current_key=="30_exit") {
			set_leave(true);
		}

		return;
	}
}

void controller_idle::awake(dfw::input&) {

	current_key="10_start";
}

void controller_idle::draw(ldv::screen& _screen, int /*fps*/) {

	_screen.clear(ldv::rgba8(0, 0, 0, 0));

	const int font_size=style.get_idle_font_size();
	assert(ttf_manager.exists("default", font_size));

	ldv::ttf_representation text_menu{
		ttf_manager.get("default", font_size),
		ldv::rgba8(255, 255, 255, 255),
		""
	};

	text_menu.set_blend(ldv::representation::blends::alpha);

	int y=10;

	for(const auto& key : menu.get_keys()) {

		text_menu.set_alpha(key==current_key ? 255 : 128);

		text_menu.set_text(menu_translation[key]);
		text_menu.go_to({0, y});
		text_menu.align(
			_screen.get_rect(), 
			{
				ldv::representation_alignment::h::center,
				ldv::representation_alignment::v::none,
				0,
				20
			}
		);
		text_menu.draw(_screen);

		y+=font_size * 1.5;
	}

	clock_rep.align(
		_screen.get_rect(), {
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::inner_bottom,
			0, 20
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
