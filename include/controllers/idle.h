#pragma once

//local
#include "states.h"

#include <app/style.h>
#include <app/clock.h>

#include <dfwimpl/app_config.h>
#include <dfw/controller_interface.h>

#include <ldtools/ttf_manager.h>
#include <lm/logger.h>

#include <ldv/ttf_representation.h>

#include <cmath>

namespace app
{

class controller_idle:
	public dfw::controller_interface {

	public:

								controller_idle(lm::logger&, ldtools::ttf_manager&, const app::app_config&, const app::style&, app::clock&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	void                        second_tick();

	//references...
	lm::logger&                 log;
	const ldtools::ttf_manager& ttf_manager;
	const app::app_config&      config;
	const app::style&           style;
	const app::clock&           clock;

	//properties
	ldv::ttf_representation     clock_rep;
	bool                        update_clock{false};

};

}

