#pragma once

//app
#include "../app/clock.h"
#include "../app/background_provider.h"
#include "../app/style.h"
#include "states.h"
#include "../dfwimpl/app_config.h"

//libdansdl2
#include <ldv/texture.h>
#include <ldv/rect.h>
#include <ldv/bitmap_representation.h>
#include <ldv/ttf_representation.h>

//framework
#include <dfw/controller_interface.h>

//tools
#include <ldtools/ttf_manager.h>

#include <lm/logger.h>

//std
#include <chrono>
#include <memory>


namespace app
{

class controller_ambient:
	public dfw::controller_interface {

	public:

	                            controller_ambient(lm::logger&, ldtools::ttf_manager&, const app::app_config&, const app::style&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/) {}
	virtual bool                can_leave_state() const {return true;}

	private:

	void						update_clock();
	void						load_new_image();
	void						setup_text_resources();
	void						setup_graphic_resources(const app::style&);
	void						set_picture_text(const std::string&);
	void						draw_overlay(ldv::screen&);
	void						align_view();

	//references...
	lm::logger&					log;

	//properties
	app::clock						clock;
	std::unique_ptr<ldv::texture>	bg_texture,
									overlay_texture,
									overlay_hack_texture;
	bool							update_view=true,
									with_overlay,
									show_seconds,
									lazy_render,
									letterbox_pictures;
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

