#include <dfwimpl/state_driver.h>
#include <app/env.h>

#include <input/input.h>
#include <controllers/states.h>

//tools
#include <tools/string_utils.h>

#include <lm/sentry.h>

//std
#include <algorithm>


using namespace app;

state_driver::state_driver(dfw::kernel& kernel, app::app_config& c)
	:state_driver_interface(t_states::state_idle),
	config(c), log(kernel.get_log()), kernel_ref(kernel) {

	lm::log(log, lm::lvl::info)<<"setting state check function..."<<std::endl;
	states.set_function([](int v){
		return v > state_min && v < state_max;
	});

	lm::log(log, lm::lvl::info)<<"init state driver building: preparing video..."<<std::endl;
	prepare_video(kernel);

	lm::log(log, lm::lvl::info)<<"preparing audio..."<<std::endl;
	prepare_audio(kernel);

	lm::log(log, lm::lvl::info)<<"preparing input..."<<std::endl;
	prepare_input(kernel);

	lm::log(log, lm::lvl::info)<<"preparing resources..."<<std::endl;
	prepare_resources(kernel);

	lm::log(log, lm::lvl::info)<<"registering graphic resources..."<<std::endl;
	manage_graphic_resources();

	lm::log(log, lm::lvl::info)<<"registering controllers..."<<std::endl;
	register_controllers(kernel);

	lm::log(log, lm::lvl::info)<<"virtualizing input..."<<std::endl;
	virtualize_input(kernel.get_input());

	lm::log(log, lm::lvl::info)<<"state driver fully constructed"<<std::endl;
}

void state_driver::prepare_video(dfw::kernel& kernel) {

	kernel.init_video_system({
		config.int_from_path("video:window_w_px"),
		config.int_from_path("video:window_h_px"),
		config.int_from_path("video:window_w_logical"),
		config.int_from_path("video:window_h_logical"),
		config.string_from_path("video:window_title"),
		config.bool_from_path("video:window_show_cursor"),
		config.get_screen_vsync()
	});

	auto& screen=kernel.get_screen();
	screen.set_fullscreen(config.bool_from_path("video:fullscreen"));
}

void state_driver::prepare_audio(dfw::kernel& kernel) {

	kernel.init_audio_system({
		config.get_audio_ratio(),
		config.get_audio_out(),
		config.get_audio_buffers(),
		config.get_audio_channels(),
		config.get_audio_volume(),
		config.get_music_volume()
	});
}

void state_driver::prepare_input(dfw::kernel& kernel) {

	using namespace dfw;

	std::vector<input_pair> pairs{
		{{input_description::types::keyboard, SDL_SCANCODE_ESCAPE, 0}, input_app::escape},
		{input_description_from_config_token(config.token_from_path("input:left")), input_app::left},
		{input_description_from_config_token(config.token_from_path("input:right")), input_app::right},
		{input_description_from_config_token(config.token_from_path("input:up")), input_app::up},
		{input_description_from_config_token(config.token_from_path("input:down")), input_app::down},
		{input_description_from_config_token(config.token_from_path("input:space")), input_app::space},
		{input_description_from_config_token(config.token_from_path("input:pause")), input_app::pause}
	};

	kernel.init_input_system(pairs);
}

void state_driver::prepare_resources(dfw::kernel& /*kernel*/) {

/*
	dfw::resource_loader r_loader(kernel.get_video_resource_manager(), kernel.get_audio_resource_manager());

	r_loader.generate_textures(tools::explode_lines_from_file(std::string("data/resources/textures.txt")));
	r_loader.generate_sounds(tools::explode_lines_from_file(std::string("data/resources/audio.txt")));
	r_loader.generate_music(tools::explode_lines_from_file(std::string("data/resources/music.txt")));
*/
}

void state_driver::register_controllers(dfw::kernel& /*_kernel*/) {

	auto reg=[this](ptr_controller& _ptr, int _i, dfw::controller_interface * _ci) {
		_ptr.reset(_ci);
		register_controller(_i, *_ptr);
	};

	reg(c_ambient, t_states::state_ambient, new controller_ambient(log, ttf_manager, config, *style.get()));
	reg(c_idle, t_states::state_idle, new controller_idle(log, ttf_manager, config, *style.get()));
	//register controllers here.
}

void state_driver::prepare_state(int _next, int /*current*/) {

	lm::log(log, lm::lvl::info)<<"next state will be "<<_next<<std::endl;

	auto box=style->get_container_box();

	int w=_next==t_states::state_ambient
		? box.w
		: config.int_from_path("video:window_w_px");

	int h=_next==t_states::state_ambient
		? box.h
		: config.int_from_path("video:window_h_px");

	lm::log(log, lm::lvl::info)<<"Setting logical screen size to "<<w<<"x"<<h<<"..."<<std::endl;
	kernel_ref.get_screen().set_logical_size(w, h);

	lm::log(log, lm::lvl::info)<<"Setting physical screen size to "<<w<<"x"<<h<<"..."<<std::endl;
	kernel_ref.get_screen().set_size(w, h);

	lm::log(log, lm::lvl::info)<<"Setting fullscreen status to "<<(int)(_next==t_states::state_ambient)<<"..."<<std::endl;

	auto win=kernel_ref.get_screen().get_window();
	SDL_SetWindowFullscreen(win,
		_next==t_states::state_ambient
			? SDL_WINDOW_FULLSCREEN
			: 0
	);
	//kernel_ref.get_screen().set_fullscreen(_next==t_states::state_ambient);

}

void state_driver::common_pre_loop_input(dfw::input& input, float /*delta*/) {

	if(input().is_event_joystick_connected()) {
		lm::log(log, lm::lvl::info)<<"New joystick detected..."<<std::endl;
		virtualize_input(input);
	}
}

void state_driver::common_loop_input(dfw::input& /*input*/, float /*delta*/) {

}

void state_driver::common_pre_loop_step(float /*delta*/) {

}

void state_driver::common_loop_step(float /*delta*/) {

}

void state_driver::virtualize_input(dfw::input& input) {

	lm::log(log, lm::lvl::info)<<"trying to virtualize "<<input().get_joysticks_size()<<" controllers..."<<std::endl;

	for(size_t i=0; i < input().get_joysticks_size(); ++i) {
		input().virtualize_joystick_hats(i);
		input().virtualize_joystick_axis(i, 15000);
		lm::log(log, lm::lvl::info)<<"Joystick virtualized "<<i<<std::endl;
	}
}

void state_driver::manage_graphic_resources() {

	//Start by getting the current screen resolution...
	auto di=ldv::get_display_info();

	//Now calculate the relative font sizes...
	int idle_font_size=config.int_from_path("app:default_font_size"),
		percent=config.int_from_path("app:clock_font_size_percent"),
		clock_font_size=(di.h * percent) / 100,
		secondary_font_size=(di.h * (percent / 3) ) / 100;

	//Some work on colours...
	int r=config.int_from_path("app:text_color_red"),
		g=config.int_from_path("app:text_color_green"),
		b=config.int_from_path("app:text_color_blue"),
		a=config.int_from_path("app:text_color_alpha");

	//And margins...
	int margin_horizontal=(di.w * config.int_from_path("app:clock_horizontal_margin_percent")) / 100,
		margin_vertical=(di.h * config.int_from_path("app:clock_vertical_margin_percent")) / 100;

	//Now we can create the "style" thing, which will be passed along to the
	//controllers...

	style.reset(new app::style(
		clock_font_size,
		secondary_font_size,
		idle_font_size,
		margin_vertical,
		margin_horizontal,
		di.w,
		di.h,
		ldv::rgba8(r, g, b, a)
	));

	//Finally, we can register fonts.
	const std::string font_path=app::get_data_dir()+"fonts/",
		idle_font_path=font_path+config.string_from_path("app:default_font_filename"),
		clock_font_path=font_path+config.string_from_path("app:clock_font_filename");

	ttf_manager.insert("default", style->get_idle_font_size(), idle_font_path);
	ttf_manager.insert("textfont", style->get_secondary_font_size(), clock_font_path);
	ttf_manager.insert("clockfont", style->get_clock_font_size(), clock_font_path);
}
