#include <controllers/idle.h>
#include <input/input.h>

#include <ldv/ttf_representation.h>

using namespace app;


controller_idle::controller_idle(
	lm::logger& _log,
	ldtools::ttf_manager& _ttf_manager,
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

//#include <source/file_utils.h>
//#include "../../../curl_request/src/curl_request.cpp"

void controller_idle::draw(ldv::screen& _screen, int /*fps*/) {

//	tools::curl_request req("http://www.caballorenoir.net/undev/media/entradas/entrada_10.jpg");
//	std::string funk=req.send().get_response_body();

//	std::string funk=tools::dump_file("../entrada_13.jpg");
//	std::vector<unsigned char> raw(std::begin(funk), std::end(funk));
//	ldv::image img(raw);

//	ldv::image img(
//		reinterpret_cast<const unsigned char * const>(funk.c_str()),
//		funk.size()
//	);

//	ldv::texture tex(img);

//	ldv::rect r{0,0, tex.get_w(), tex.get_h()};
//	ldv::bitmap_representation bmp(tex, r, r);

	_screen.clear(ldv::rgba8(0, 0, 0, 0));

//	bmp.go_to({0,0});
//	bmp.draw(_screen);

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
