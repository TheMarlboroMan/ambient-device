#include "background_provider.h"

#include <fstream>
#include <vector>
#include <stdexcept>

using namespace app;

background_provider::background_provider() {

	load_defaults();
}

app::background background_provider::get() const {

	auto last=defaults[defaults.size()-1];
	defaults.pop_back();
	defaults.insert(std::begin(defaults), last);
	return last;
}

//TODO: erase.
#include <iostream>

void background_provider::load_defaults() {

	std::ifstream f("data/app/default_pics.dat");
	if(!f) {
		throw std::runtime_error("unable to load default pictures from data/app/default_pics.dat");
	}

	std::string line;

	while(std::getline(f, line)) {

		std::vector<std::string> pieces;

		size_t begin=0, pos=0;

		//TODO: Terrible condition...
		while(true) {

			//Data is separated by tabs...
			pos=line.find("\t", pos);
			pieces.push_back(line.substr(begin, pos-begin));

			if(std::string::npos==pos) {
				break;
			}

			begin=++pos;
		}

		if(4!=pieces.size()) {
			throw std::runtime_error("invalid default pictures line '"+line+"'");
		}

		defaults.push_back({pieces[0], pieces[1], pieces[2], pieces[3]});
	}
}
