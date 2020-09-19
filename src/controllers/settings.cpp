#include <controllers/settings.h>
#include <input/input.h>
#include <app/env.h>

#include <tools/file_utils.h>
#include <tools/json.h>

#include <ldv/ttf_representation.h>

using namespace app;


controller_settings::controller_settings(
	lm::logger& _log,
	ldtools::ttf_manager& _ttf_manager,
	app::app_config& _app_config,
	const app::style& _style
):
	log(_log),
	ttf_manager(_ttf_manager),
	config(_app_config),
	style{_style},
	current_key{"10_seconds_between_pictures"}
{

	//TODO: This is repeated.
	std::string config_file_path=app::get_data_dir();
				config_file_path+="app/menu.json";

	auto json_document=tools::parse_json_string(
		tools::dump_file(config_file_path)
	);

	tools::options_menu_from_json(
		json_document["settings"],
		menu
	);

	menu_translation["10_seconds_between_pictures"]="Seconds between pictures";
	menu_translation["20_shuffle_pictures"]="Shuffle pictures";
	menu_translation["30_with_vignette"]="With vignette";
	menu_translation["40_letterbox_pictures"]="Letterbox pictures";
	menu_translation["50_show_seconds"]="Show clock seconds";
	menu_translation["60_show_picture_counter"]="Show picture counter";
	menu_translation["70_lazy_render"]="Lazy rendering";
	menu_translation["80_save_and_exit"]="Save and exit";
	menu_translation["90_exit_without_saving"]="Exit without saving";
}

void controller_settings::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input_app::escape)) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input_app::down)) {

		current_key=menu.adjacent_key(current_key, decltype(menu)::browse_dir::next);
	}
	else if(_input.is_input_down(input_app::up)) {

		current_key=menu.adjacent_key(current_key, decltype(menu)::browse_dir::previous);
	}
	else if(_input.is_input_down(input_app::right)) {

		menu.browse(current_key, decltype(menu)::browse_dir::next);
	}
	else if(_input.is_input_down(input_app::left)) {

		menu.browse(current_key, decltype(menu)::browse_dir::previous);
	}
	else if(_input.is_input_down(input_app::space)) {

		if(current_key=="80_save_and_exit") {

			save_config();
			set_state(t_states::state_idle);
		}
		else if(current_key=="90_exit_without_saving") {

			set_state(t_states::state_idle);
		}
	}
}

void controller_settings::awake(dfw::input& /*input*/) {

	menu.set("10_seconds_between_pictures", config.int_from_path("app:seconds_between_pictures"));
	menu.set("20_shuffle_pictures", config.bool_from_path("app:shuffle_pictures"));
	menu.set("30_with_vignette", config.bool_from_path("app:with_vignette"));
	menu.set("40_letterbox_pictures", config.bool_from_path("app:letterbox_pictures"));
	menu.set("50_show_seconds", config.bool_from_path("app:show_seconds"));
	menu.set("60_show_picture_counter", config.bool_from_path("app:show_picture_counter"));
	menu.set("70_lazy_render", config.bool_from_path("app:lazy_render"));

	current_key="10_seconds_between_pictures";
}

void controller_settings::draw(ldv::screen& _screen, int /*fps*/) {

	const int font_size=style.get_idle_font_size();

	ldv::ttf_representation text_menu{
		ttf_manager.get("default", font_size),
		ldv::rgba8(255, 255, 255, 255),
		""
	};
	text_menu.set_blend(ldv::representation::blends::alpha);

	_screen.clear(ldv::rgba8(0, 0, 0, 0));

	int y=10;

	auto align_rect=_screen.get_rect();
	align_rect.w=(align_rect.w / 4) * 3; //Three thirds of the display width.

	for(const auto& key : menu.get_keys()) {

		text_menu.set_alpha(key==current_key ? 255 : 128);

		//Left side...
		text_menu.set_text_align(ldv::ttf_representation::text_align::right);
		text_menu.set_text(menu_translation[key]+":");
		text_menu.go_to({0, y});
		text_menu.align(
			align_rect, 
			{
				ldv::representation_alignment::h::inner_right,
				ldv::representation_alignment::v::none,
				10, 0
			}
		);
		text_menu.draw(_screen);

		if(key=="80_save_and_exit" || key=="90_exit_without_saving") {

			y+=font_size * 1.5;
			continue;
		}

		//Right side...
		text_menu.set_text_align(ldv::ttf_representation::text_align::left);

		if(key=="10_seconds_between_pictures") {

			text_menu.set_text(std::to_string(menu.get_int(key)));
		}
		else {
			
			text_menu.set_text(menu.get_bool(key) ? "yes" : "no");
		}

		text_menu.go_to({0, y});
		text_menu.align(
			align_rect, 
			{
				ldv::representation_alignment::h::outer_right,
				ldv::representation_alignment::v::none,
				10, 0
			}
		);
		text_menu.draw(_screen);

		y+=font_size * 1.5;
	}
}

void controller_settings::save_config() {

	config.set("app:seconds_between_pictures", menu.get_int("10_seconds_between_pictures"));
	config.set("app:shuffle_pictures", menu.get_bool("20_shuffle_pictures"));
	config.set("app:with_vignette", menu.get_bool("30_with_vignette"));
	config.set("app:letterbox_pictures", menu.get_bool("40_letterbox_pictures"));
	config.set("app:show_seconds", menu.get_bool("50_show_seconds"));
	config.set("app:show_picture_counter", menu.get_bool("60_show_picture_counter"));
	config.set("app:lazy_render", menu.get_bool("70_lazy_render"));
	config.save();
}
