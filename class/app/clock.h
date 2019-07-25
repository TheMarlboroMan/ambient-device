#ifndef APP_CLOCK_H
#define APP_CLOCK_H

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
	//to "now".
	bool								has_changed(time_t) const;

	//!Returns the current hour and minute as a string.
	std::string							get_time() const;

	private:

	time_t								now;
};

}

#endif

