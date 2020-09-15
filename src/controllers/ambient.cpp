#include <controllers/ambient.h>
#include <input/input.h>
#include <app/size_calculator.h>
#include <app/env.h>

#include <tools/string_utils.h>

#include <lm/sentry.h>

using namespace app;

controller_ambient::controller_ambient(
	lm::logger& _log,
	ldtools::ttf_manager& _ttf_manager,
	const app::app_config& _app_config,
	const app::style& _style,
	app::clock& _clock
)
	:log(_log),
	clock{_clock},
	with_vignette(_app_config.bool_from_path("app:with_vignette")),
	show_seconds(_app_config.bool_from_path("app:show_seconds")),
	show_picture_counter(_app_config.bool_from_path("app:show_picture_counter")),
	lazy_render(_app_config.bool_from_path("app:lazy_render")),
	letterbox_pictures(_app_config.bool_from_path("app:letterbox_pictures")),
	seconds_between_pictures{_app_config.int_from_path("app:seconds_between_pictures")},
	clock_margin_horizontal{_style.get_clock_horizontal_margin()},
	clock_margin_vertical{_style.get_clock_vertical_margin()},
	display_box{_style.get_container_box()},
	clock_rep{
		_ttf_manager.get("clockfont", _style.get_clock_font_size()),
		_style.get_clock_font_color(),
		clock.get_time(show_seconds)
	},
	pic_info_rep{
		_ttf_manager.get("textfont", _style.get_secondary_font_size()),
		_style.get_secondary_font_color(),
		"..."
	},
	background_provider(_app_config.bool_from_path("app:shuffle_pictures"))
{

	setup_text_resources();
	setup_graphic_resources(_style);

	_clock.subscribe_second("ambient_second_tick", std::bind(&controller_ambient::second_tick, this));
	_clock.subscribe_minute("ambient_minute_tick", std::bind(&controller_ambient::minute_tick, this));

	stamp=std::time(nullptr);
}

void controller_ambient::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input_app::escape)) {
		set_leave(true);
		return;
	}

	if(update_clock) {

		update_clock_text();
		update_clock=false;
	}

	if(_input.is_input_down(input_app::left)) {

		background_provider.prev();
		load_new_image();
	}
	else if(_input.is_input_down(input_app::right)) {

		background_provider.next();
		load_new_image();
	} else if(_input.is_input_down(input_app::pause)) {

		paused=!paused;
		update_text();

		//unpause restores the stamp.
		if(!paused) {

			stamp=std::time(nullptr);
		}
	}
	else if(
			_input().is_event_keyboard_down()
		|| _input().is_event_mouse_button_down()
//TODO: Fuck you... This doesn't work...
//		|| _input().is_event_mouse_movement()
	) {
		set_state(t_states::state_idle);
		return;
	}

//TODO: I wish the clock could take care of this too...

	auto now=std::time(nullptr);
	if(!paused && now-stamp >= seconds_between_pictures) {
		stamp=now;
		background_provider.next();
		load_new_image();
	}
}

void controller_ambient::draw(ldv::screen& screen, int /*fps*/) {

	if(lazy_render && !update_view) {
		return;
	}

	update_view=false;

	align_view();

	screen.clear(ldv::rgba8(0, 0, 0, 0));

	background.draw(screen);

	if(with_vignette) {
		draw_vignette(screen);
	}

	clock_rep.draw(screen);
	pic_info_rep.draw(screen);
}

void controller_ambient::awake(dfw::input&) {

	if(nullptr==bg_texture.get()) {
		load_new_image();
	}

	stamp=std::time(nullptr);
	update_clock_text();
}

void controller_ambient::load_new_image() {

	auto bg=background_provider.get();

	//TODO: This should actually be a thread....
	try {
		ldv::image img{bg.get_path()};
		bg_texture.reset(new ldv::texture(img));
	}
	catch(std::exception& e) {

		//TODO: should skip, see if any pic can be loaded...
		lm::log(log, lm::lvl::warning)<<"Could not load picture: "<<e.what()<<std::endl;
		return;
	}

	assert(nullptr!=bg_texture.get());
	auto& tex=*bg_texture.get();

	app::size_calculator calc;
	app::rect pos, clip;

	void(app::size_calculator::*fnptr)(rect, rect, rect&, rect&)=letterbox_pictures
		? &app::size_calculator::letterbox
		: &app::size_calculator::fill;

	(calc.*(fnptr))(
		{0, 0, tex.get_w(), tex.get_h()},
		{0, 0, display_box.w, display_box.h},
		pos, clip
	);

	background.set_texture(tex);
	background.set_location({pos.x, pos.y, pos.w, pos.h});
	background.set_clip({clip.x, clip.y, clip.w, clip.h});

	update_text();
	update_view=true;
}

void controller_ambient::update_text() {

	auto bg=background_provider.get();

	//Set the author, date and description of the pics too..
	std::string txt=bg.get_description()+" by "+bg.get_author()+", "+bg.get_date();

	if(show_picture_counter) {

		txt+=" "+std::to_string(background_provider.get_index()+1)+" / "+std::to_string(background_provider.size());
	}

	//TODO: Do this better, draw a symbol somewhere...
	if(paused) {

		txt+=" (paused)";
	}

	set_picture_text(txt);

	update_view=true;
}

void controller_ambient::update_clock_text() {

	clock_rep.set_text(clock.get_time(show_seconds));
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
	vignette_texture.reset(new ldv::texture{ldv::image{app::get_data_dir()+"bitmap/overlay.png"}});
	overlay={*vignette_texture.get(),
		{0,0, _style.get_container_box().w, vignette_texture->get_h()},
		{0,0, vignette_texture->get_w(), vignette_texture->get_h()}
	};
	overlay.set_texture(*vignette_texture.get());
	overlay.set_blend(ldv::representation::blends::alpha);
	overlay.set_alpha(255);
}

void controller_ambient::draw_vignette(ldv::screen& _screen) {

	overlay.set_invert_vertical(false);
	overlay.align(display_box, {
		ldv::representation_alignment::h::inner_left,
		ldv::representation_alignment::v::inner_top
	});
	overlay.draw(_screen);

	overlay.set_invert_vertical(true);
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

void controller_ambient::second_tick() {

	if(!show_seconds) {

		return;
	}

	//This is called from another thread. Let us not even try to update an openGl texture here.
	update_clock=true;
}

void controller_ambient::minute_tick() {

	if(show_seconds) {

		return;
	}

	//See second_tick.
	update_clock=true;
}
