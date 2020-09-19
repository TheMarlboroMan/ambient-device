#pragma once

//local
#include "states.h"
#include <app/style.h>

#include <dfwimpl/app_config.h>
#include <dfw/controller_interface.h>

#include <ldtools/ttf_manager.h>
#include <lm/logger.h>
#include <tools/options_menu.h>

#include <string>

namespace app
{

class controller_settings:
	public dfw::controller_interface {

	public:

								controller_settings(lm::logger&, ldtools::ttf_manager&, app::app_config&, const app::style&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/);
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	void                        save_config();

	//references...
	lm::logger&                 log;
	const ldtools::ttf_manager& ttf_manager;
	app::app_config&            config;
	const app::style&           style;

	//properties
	tools::options_menu<std::string>    menu;
	std::string                         current_key;
	std::map<std::string, std::string>  menu_translation;
};

}

