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
	const app::app_config& _app_config,
	const app::style& _style
):
	log(_log),
	ttf_manager(_ttf_manager),
	config(_app_config),
	style{_style},
	current_key{"10_seconds_between_pictures"}
{

	std::string config_file_path=app::get_data_dir();
				config_file_path+="app/menu.json";

	auto json_document=tools::parse_json_string(
		tools::dump_file(config_file_path)
	);

	tools::options_menu_from_json(
		json_document["menu"],
		menu
	);

	
}

void controller_settings::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input_app::escape)) {
		set_leave(true);
		return;
	}

//adjacent_key(const tkey& _key, browse_dir _dir) const {
//browse(const tkey& _key, browse_dir _dir) {
//get_int
//get_bool

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
	align_rect.w/=2;

	for(const auto& key : menu.get_keys()) {

		text_menu.set_alpha(key==current_key ? 255 : 128);

		//Left side...
		text_menu.set_text_align(ldv::ttf_representation::text_align::right);
		text_menu.set_text(key+":");
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

