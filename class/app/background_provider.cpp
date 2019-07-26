#include "background_provider.h"

#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace app;

background_provider::background_provider(bool _shuffle) {

	load_defaults(_shuffle);
}

app::background background_provider::get() const {

	auto result=defaults[index];
	if(++index >= defaults.size()-1) {
		index=0;
	}
 
	return result;
}

void background_provider::load_defaults(bool _shuffle) {

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

	if(_shuffle) {
		std::srand(std::time(nullptr));
		std::random_shuffle(std::begin(defaults), std::end(defaults), [](int _v){return std::rand()%_v;});
	}
}
