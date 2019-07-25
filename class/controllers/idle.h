#ifndef CONTROLLER_IDLE_H
#define CONTROLLER_IDLE_H

//std
#include <cmath>

//libdansdl2
#include <def_video.h>

//framework
#include <class/controller_interface.h>

//tools
#include <class/ttf_manager.h>

//local
#include "states.h"
#include "../dfwimpl/app_config.h"

namespace app
{

class controller_idle:
	public dfw::controller_interface {

	public:

							controller_idle(tools::log&, tools::ttf_manager&, const app::app_config&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	//references...
	tools::log&					log;
	const tools::ttf_manager&	ttf_manager;
	const app::app_config&		config;

	//properties
};

}

#endif
