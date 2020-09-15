#pragma once

//app
#include <app/clock.h>
#include <app/background_provider.h>
#include <app/style.h>
#include "states.h"

//libdansdl2
#include <ldv/texture.h>
#include <ldv/rect.h>
#include <ldv/bitmap_representation.h>
#include <ldv/ttf_representation.h>

//framework
#include <dfwimpl/app_config.h>
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

	                            controller_ambient(lm::logger&, ldtools::ttf_manager&, const app::app_config&, const app::style&, app::clock&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/) {}
	virtual bool                can_leave_state() const {return true;}

	private:

	void                        update_clock_text();
	void                        load_new_image();
	void                        update_text();
	void                        setup_text_resources();
	void                        setup_graphic_resources(const app::style&);
	void                        set_picture_text(const std::string&);
	void                        draw_vignette(ldv::screen&);
	void                        align_view();
	void                        second_tick();
	void                        minute_tick();

	//references...
	lm::logger&                 log;
	const app::clock&           clock;

	//properties
	std::unique_ptr<ldv::texture>   bg_texture,
	                                vignette_texture;
	bool                            update_view=true,
	                                with_vignette,
	                                show_seconds,
	                                show_picture_counter,
	                                lazy_render,
	                                letterbox_pictures;
	time_t                          stamp;
	bool                            paused=false,
	                                update_clock=false;
	int                             seconds_between_pictures,
	                                clock_margin_horizontal,
	                                clock_margin_vertical;
	ldv::rect                       display_box;

	ldv::bitmap_representation      background,
	                                overlay;
	ldv::ttf_representation         clock_rep,
	                                pic_info_rep;
	app::background_provider        background_provider;
};

}
