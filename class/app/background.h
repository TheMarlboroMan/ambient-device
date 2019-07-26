#ifndef APP_BACKGROUND_H
#define APP_BACKGROUND_H

#include <string>

namespace app {

struct background {

	//TODO: Make private, create constructor.
	std::string				path,
							author,
							description,
							date;

	const std::string		get_path() const {return path;}
	const std::string		get_author() const {return author;}
	const std::string		get_description() const {return description;}
	const std::string		get_date() const {return date;}
};

}

#endif
