#ifndef APP_BACKGROUND_PROVIDER_H
#define APP_BACKGROUND_PROVIDER_H

#include <vector>
#include <string>

#include "background.h"

namespace app {

//!This class will return a path to a new background file each time it is
//!requested.
class background_provider {

	public:

	//!Returns a background image instance.
									background_provider();
	background						get() const;

	private:

	void							load_defaults();

	//!So far, this will be implemented as a queue. We can inject context later.
	mutable std::vector<background>		defaults;
};

}


#endif
