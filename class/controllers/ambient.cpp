#include "ambient.h"

//TODO: Remove
#include <source/file_utils.h>
#include <source/string_utils.h>

//local
#include "../input.h"

using namespace app;

controller_ambient::controller_ambient(tools::log& _log,
	tools::ttf_manager& _ttf_manager,
	const app::app_config& _app_config,
	const app::style& _style)
	:log(_log)
	, with_overlay(_app_config.bool_from_path("config:app:with_overlay"))
	, seconds_between_pictures{_app_config.int_from_path("config:app:seconds_between_pictures")}
	, clock_margin_horizontal{_style.get_clock_horizontal_margin()}
	, clock_margin_vertical{_style.get_clock_vertical_margin()}
	, display_box{_style.get_container_box()}
	, clock_rep{
		_ttf_manager.get("clockfont", _style.get_clock_font_size()),
		_style.get_clock_font_color(),
		"00:00"
	}
	, pic_info_rep{
		_ttf_manager.get("textfont", _style.get_secondary_font_size()),
		_style.get_secondary_font_color(),
		"..."
	}
	{

	setup_text_resources();
	setup_graphic_resources(_style);

	stamp=std::time(nullptr);
}

void controller_ambient::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input_app::escape)) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input_app::space) ) {

		set_state(t_states::state_idle);
	}

	if(clock.has_changed(std::time(nullptr))) {
		update_clock();
	}

	auto now=std::time(nullptr);
	if(now-stamp >= seconds_between_pictures) {
		stamp=now;
		load_new_image();
	}

//TODO: Fuck you. Really.
//	if(_input().is_event_keyboard_down() || _input().is_event_mouse_button_down() || _input().is_event_mouse_movement()) {
//		set_state(t_states::state_idle);
//	}
}

void controller_ambient::draw(ldv::screen& screen, int /*fps*/) {

	if(!update_view) {
		return;
	}

	update_view=false;

	align_view();

	screen.clear(ldv::rgba8(0, 0, 0, 0));

	background.draw(screen);

	if(with_overlay) {
		draw_overlay(screen);
	}

	clock_rep.draw(screen);
	pic_info_rep.draw(screen);
}

void controller_ambient::awake(dfw::input&) {

	load_new_image();
	update_clock();
}

void controller_ambient::load_new_image() {

	//TODO: This should actually be a thread....
	auto bg=background_provider.get();
	ldv::image img{bg.get_path()};

	bg_texture.reset(new ldv::texture(img));

	assert(nullptr!=bg_texture.get());
	auto& tex=*bg_texture.get();
	background={tex, display_box, {0,0, tex.get_w(), tex.get_h()} };

	//Set the author, date and description of the pics too..
	std::string txt=bg.get_description()+" by <"+bg.get_author()+">, "+bg.get_date();
	set_picture_text(txt);

	update_view=true;
}

void controller_ambient::update_clock() {

	clock.update_time();
	clock_rep.set_text(clock.get_time());
	update_view=true;
}

void controller_ambient::setup_text_resources() {

	clock_rep.set_text_align(ldv::ttf_representation::text_align::right);
	pic_info_rep.set_text_align(ldv::ttf_representation::text_align::right);
}

void controller_ambient::set_picture_text(const std::string& _txt) {

	//Most useless method ever.
	pic_info_rep.set_text(_txt);
}

void controller_ambient::setup_graphic_resources(const app::style& _style) {

	//TODO: We should have a "texture_from_path"...
	overlay_texture.reset(new ldv::texture{ldv::image{"data/bitmap/overlay.png"}});
	overlay={*overlay_texture.get(),
		{0,0, _style.get_container_box().w, overlay_texture->get_h()},
		{0,0, overlay_texture->get_w(), overlay_texture->get_h()}
	};
	overlay.set_blend(ldv::representation::blends::alpha);
	overlay.set_alpha(255);

	//TODO: Fuck you.
	overlay_hack_texture.reset(new ldv::texture{ldv::image{"data/bitmap/overlay_hack.png"}});
}

void controller_ambient::draw_overlay(ldv::screen& _screen) {

	//TODO: Fuck you....
	overlay.set_texture(*overlay_texture.get());
	overlay.align(display_box, {
		ldv::representation_alignment::h::inner_left,
		ldv::representation_alignment::v::inner_top
	});
	overlay.draw(_screen);

	//TODO: A single stupid line appears... Check the implementation.
	overlay.set_texture(*overlay_hack_texture.get());
	overlay.align(display_box, {
		ldv::representation_alignment::h::inner_left,
		ldv::representation_alignment::v::inner_bottom,
	});
	overlay.draw(_screen);
}

void controller_ambient::align_view() {

	//Allows alignment of textual elements, so the clock can be aligned
	//according to the screen and the text according to the clock, regardless
	//of their update order.
	clock_rep.align(
		display_box,
		ldv::representation_alignment{
			ldv::representation_alignment::h::inner_right,
			ldv::representation_alignment::v::inner_bottom,
			clock_margin_horizontal,
			clock_margin_vertical
		}
	);

	pic_info_rep.align(
		clock_rep,
		ldv::representation_alignment{
			ldv::representation_alignment::h::inner_right,
			ldv::representation_alignment::v::outer_bottom,
		}
	);
}
