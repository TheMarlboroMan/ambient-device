#pragma once

#include <vector>
#include <string>

#include "background.h"

namespace app {

//!This class will return a path to a new background file each time it is
//!requested.
class background_provider {

	public:

									background_provider(bool);
	//!Returns a background image instance.
	background						get();

	std::size_t						get_index() const {return index;}
	std::size_t						size() const {return defaults.size();}

	//!Advances the current index (wraps around)
	void							next();

	//!Decrements the current index (wraps around).
	void							prev();

	private:

	void							load_defaults();
	void							shuffle_contents();

	//!Indicates whether or not the pictures will be shuffled on load and after the
	//!queue is done.
	bool							shuffle;

	//!So far, this will be implemented as a queue. We can inject context later.
	std::vector<background>		defaults;
	size_t				index=0;
};

}

