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

namespace app
{

class controller_ambient:
	public dfw::controller_interface {

	public:

						controller_ambient(tools::log&, tools::ttf_manager&, const app::app_config&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/);
	virtual void 				slumber(dfw::input& /*input*/);
	virtual bool				can_leave_state() const {return true;}

	private:

	void						update_clock();
	void						load_text_resources();
	void						free_text_resources();
	void						load_new_image();
	void						free_image_resources();

	//references...
	tools::log&					log;
	tools::ttf_manager&			ttf_manager;
	const app::app_config&		config;

	//properties
	app::clock						clock;
	std::unique_ptr<ldv::texture>	texture;
	std::string						fontpath;
	int								fontsize,
									secondary_fontsize;
	bool							update_view=true;
	time_t							stamp;
	struct	{
		unsigned int 				w=0, 
									h=0;
	} 								display_size;

	ldv::bitmap_representation 		background;

	//A ttf_representation has no correct state without ttf font. The ttf is
	//reloaded each time this controller awakens, hence the pointer.
	std::unique_ptr<ldv::ttf_representation> 		clock_rep,
													pic_info_rep;
	app::background_provider						background_provider;
	
};

}

#endif
