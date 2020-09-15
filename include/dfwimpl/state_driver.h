#pragma once

//local.
#include "app_config.h"
#include <controllers/states.h>
#include <app/style.h>
#include <app/clock.h>

//Framework
#include <dfw/state_driver_interface.h>

//tools
#include <ldtools/ttf_manager.h>

//Controllers.
#include <controllers/ambient.h>
#include <controllers/idle.h>
//new controller includes here.

//std
#include <memory>

namespace app
{

class state_driver:
	public dfw::state_driver_interface {

	public:
	                            state_driver(dfw::kernel& kernel, app::app_config& config);

	virtual void                common_pre_loop_input(dfw::input& input, float delta);
	virtual void                common_pre_loop_step(float delta);
	virtual void                common_loop_input(dfw::input& input, float delta);
	virtual void                common_loop_step(float delta);
	virtual void                prepare_state(int, int);
	virtual float               get_max_timestep() const {return 0.03f;}

	private:

	void                        prepare_video(dfw::kernel&);
	void                        prepare_audio(dfw::kernel&);
	void                        prepare_input(dfw::kernel&);
	void                        prepare_resources(dfw::kernel&);
	void                        register_controllers(dfw::kernel&);
	void                        virtualize_input(dfw::input& input);
	void                        manage_graphic_resources();

	//references
	app::app_config&            config;
	lm::logger&                 log;
	dfw::kernel&                kernel_ref;
	ldtools::ttf_manager        ttf_manager;

	//properties...
	std::unique_ptr<app::style> style;
	app::clock                  clock; //must be destroyed BEFORE the controllers.

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller              c_ambient;
	ptr_controller              c_idle;
	//controller instances here.


};

}
