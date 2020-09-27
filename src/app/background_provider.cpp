#include <app/background_provider.h>
#include <app/env.h>
#include <app/exif_reader.h>

#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace app;

background_provider::background_provider(bool _shuffle)
	:shuffle(_shuffle) {

	load_defaults();
}

app::background background_provider::get() {

	return defaults[index];
}

void background_provider::prev() {

	index=index==0 
		? defaults.size() -1
		: index-1;
}

void background_provider::next() {

	index=index==defaults.size() -1 
		? 0
		: index+1;
}

void background_provider::load_defaults() {

	std::ifstream f(app::get_data_dir()+std::string{"app/default_pics.dat"});
	if(!f) {
		throw std::runtime_error("unable to load default pictures from .ambient-device/data/app/default_pics.dat");
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

		if(3!=pieces.size()) {
			//TODO: Should just log and go on ahead.
			throw std::runtime_error("invalid default pictures line '"+line+"'");
		}

		const std::string pic_path=app::get_pic_dir()+pieces[0];

		app::exif_reader er;
		auto res=er.read(pic_path);
		const std::string date=res.is_valid()
			? res.get_date()
			: "";

		defaults.push_back({pic_path, pieces[1], pieces[2], date});
	}

	if(!defaults.size()) {
		throw std::runtime_error("default images are needed");
	}

	if(shuffle) {
		shuffle_contents();
	}
}

void background_provider::shuffle_contents() {

	if(1==defaults.size()) {
		return;
	}

	std::srand(std::time(nullptr));
	std::random_shuffle(std::begin(defaults), std::end(defaults), [](int _v){return std::rand()%_v;});
}
