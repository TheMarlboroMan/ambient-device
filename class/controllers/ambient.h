#ifndef CONTROLLER_AMBIENT_H
#define CONTROLLER_AMBIENT_H

//std
#include <chrono>
#include <memory>

//libdansdl2
#include <def_video.h>

//framework
#include <class/controller_interface.h>

//tools
#include <class/ttf_manager.h>

//local
#include "states.h"
#include "../dfwimpl/app_config.h"

//app
#include "../app/clock.h"
#include "../app/background_provider.h"
#include "../app/style.h"

namespace app
{

class controller_ambient:
	public dfw::controller_interface {

	public:

							controller_ambient(tools::log&, tools::ttf_manager&, const app::app_config&, const app::style&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/);
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	void						update_clock();
	void						load_new_image();
	void						setup_text_resources();
	void						setup_graphic_resources(const app::style&);
	void						set_picture_text(const std::string&);
	void						draw_overlay(ldv::screen&);
	void						align_view();

	//references...
	tools::log&					log;

	//properties
	app::clock						clock;
	std::unique_ptr<ldv::texture>	bg_texture,
									overlay_texture;
	bool							update_view=true,
									with_overlay;
	time_t							stamp;
	int								seconds_between_pictures,
									clock_margin_horizontal,
									clock_margin_vertical;
	ldv::rect						display_box;

	ldv::bitmap_representation 		background,
									overlay;
	ldv::ttf_representation 		clock_rep,
									pic_info_rep;
	app::background_provider		background_provider;
	
};

}

#endif
