#include "ambient.h"

//TODO: Remove
#include <source/file_utils.h>
#include <source/string_utils.h>

//local
#include "../input.h"

using namespace app;

controller_ambient::controller_ambient(tools::log& _log, tools::ttf_manager& _ttf_manager, const app::app_config& _app_config)
	:log(_log)
	, ttf_manager(_ttf_manager)
	, config(_app_config)
	, fontpath{"data/fonts/"+config.string_from_path("config:app:clock_font_filename")}
	{

	stamp=std::time(nullptr);
}

void controller_ambient::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input_app::space)) {
		set_state(t_states::state_idle);
	}

	if(clock.has_changed(std::time(nullptr))) {
		update_clock();
	}

	auto now=std::time(nullptr);
	if(now-stamp >= config.int_from_path("config:app:seconds_between_pictures")) {
		stamp=now;
		//TODO: How about some nice transitions?
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

	log<<"redrawing"<<std::endl;
	screen.clear(ldv::rgba8(0, 0, 0, 0));
	background.draw(screen);

	assert(nullptr!=clock_rep.get());
	clock_rep->draw(screen);

	assert(nullptr!=pic_info_rep.get());
	pic_info_rep->draw(screen);
}

void controller_ambient::awake(dfw::input&) {

	log<<"awakening the ambient controller"<<std::endl;

	//Each time the controller awakens the screen resolution might have changed,
	//so the size is recalculated and related resources are loaded anew.

	auto di=ldv::get_display_info();
	display_size.w=(unsigned)di.w;
	display_size.h=(unsigned)di.h;

	load_text_resources();
	load_new_image();
	update_clock();
}

void controller_ambient::slumber(dfw::input&) {

	log<<"putting the ambient controller to sleep"<<std::endl;

	//Resources are freed once the controller is set to sleep.
	free_text_resources();
	free_image_resources();
}

void controller_ambient::load_new_image() {

	auto bg=background_provider.get();
	ldv::image img{bg.get_path()};

	texture.reset(new ldv::texture(img));

	assert(nullptr!=texture.get());
	auto& tex=*texture.get();
	background={tex, {0, 0, display_size.w, display_size.h}, {0,0, tex.get_w(), tex.get_h()} };

	//Set the author, date and description of the pics too..
	assert(nullptr!=pic_info_rep.get());
	std::string txt=bg.get_description()+" by <"+bg.get_author()+">, "+bg.get_date();
log<<txt<<std::endl;

	pic_info_rep->set_text(txt);
	pic_info_rep->align(
		*(clock_rep.get()),
		ldv::representation_alignment{
			ldv::representation_alignment::h::inner_right,
			ldv::representation_alignment::v::outer_bottom,
			0,
			0
		}
	);

	update_view=true;
}

void controller_ambient::free_image_resources() {

	assert(nullptr!=texture.get());
	texture.reset(nullptr);
}

void controller_ambient::update_clock() {

	clock.update_time();

	assert(nullptr!=clock_rep.get());
	clock_rep->set_text(clock.get_time());

	int margin_horizontal_percent=config.int_from_path("config:app:clock_right_margin_percent"),
		margin_vertical_percent=config.int_from_path("config:app:clock_bottom_margin_percent"),
		margin_horizontal=(display_size.w * margin_horizontal_percent) / 100,
		margin_vertical=(display_size.h * margin_vertical_percent) / 100;

	clock_rep->align(
		{0,0, display_size.w, display_size.h},
		ldv::representation_alignment{
			ldv::representation_alignment::h::inner_right,
			ldv::representation_alignment::v::inner_bottom,
			margin_horizontal,
			margin_vertical
		}
	);

	update_view=true;
}

void controller_ambient::load_text_resources() {

	int percent=config.int_from_path("config:app:clock_font_size_percent");

	fontsize=(display_size.h * percent) / 100;
	secondary_fontsize=(display_size.h * (percent / 2) ) / 100;

	assert(!ttf_manager.exists("clockfont", fontsize));
	ttf_manager.insert("clockfont", fontsize, fontpath);

	assert(!ttf_manager.exists("textfont", secondary_fontsize));
	ttf_manager.insert("textfont", secondary_fontsize, fontpath);

	int r=config.int_from_path("config:app:text_color_red"),
		g=config.int_from_path("config:app:text_color_green"),
		b=config.int_from_path("config:app:text_color_blue"),
		a=config.int_from_path("config:app:text_color_alpha");

	clock_rep.reset(new ldv::ttf_representation{
		ttf_manager.get("clockfont", fontsize),
		ldv::rgba8(r, g, b, a),
		""}
	);

	pic_info_rep.reset(new ldv::ttf_representation{
		ttf_manager.get("textfont", secondary_fontsize),
		ldv::rgba8(r, g, b, a),
		""}
	);
}

void controller_ambient::free_text_resources() {

	assert(ttf_manager.exists("clockfont", fontsize));
	ttf_manager.erase("clockfont", fontsize);

	assert(ttf_manager.exists("textfont", secondary_fontsize));
	ttf_manager.erase("textfont", secondary_fontsize);

	clock_rep.reset(nullptr);
	pic_info_rep.reset(nullptr);
}
