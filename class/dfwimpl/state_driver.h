#ifndef APP_STATE_DRIVER_H
#define APP_STATE_DRIVER_H

//std
#include <memory>

//Framework
#include <class/state_driver_interface.h>

//local.
#include "app_config.h"
#include "../controllers/states.h"

//tools
#include <class/ttf_manager.h>

//Controllers.
#include "../controllers/ambient.h"
#include "../controllers/idle.h"
//new controller includes here.

//Specific app_config
#include "../app/style.h"

namespace app
{

class state_driver:
	public dfw::state_driver_interface {

	public:
							state_driver(dfw::kernel& kernel, app::app_config& config);

	virtual void					common_pre_loop_input(dfw::input& input, float delta);
	virtual void					common_pre_loop_step(float delta);
	virtual void					common_loop_input(dfw::input& input, float delta);
	virtual void					common_loop_step(float delta);
	virtual void					prepare_state(int, int);
	virtual float					get_max_timestep() const {return 0.03f;}

	private:

	void						prepare_video(dfw::kernel&);
	void						prepare_audio(dfw::kernel&);
	void						prepare_input(dfw::kernel&);
	void						prepare_resources(dfw::kernel&);
	void						register_controllers(dfw::kernel&);
	void 						virtualize_input(dfw::input& input);
	void						manage_graphic_resources();

	//references
	app::app_config&				config;
	tools::log&					log;
	dfw::kernel&				kernel_ref;
	tools::ttf_manager			ttf_manager;

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller					c_ambient;
	ptr_controller					c_idle;
	//controller instances here.

	//properties...
	std::unique_ptr<app::style>		style;
};

}
#endif
