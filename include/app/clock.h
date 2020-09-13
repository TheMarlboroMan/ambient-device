#pragma once

#include <ctime>
#include <string>

namespace app {

class clock {

	public:

	//!Class constructor.
										clock();

	//!Updates the current moment.
	void								update_time();

	//!Will return true if the parameter is a change of minute/hour with respect
	//to "now". Optionally it can report a change of seconds.
	bool								has_changed(time_t, bool) const;

	//!Returns the current hour and minute as a string (optionally seconds)
	std::string							get_time(bool) const;

	private:

	time_t								now,
									fixed_to_minute;
};

}
