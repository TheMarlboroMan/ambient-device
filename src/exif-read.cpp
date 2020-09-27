#include <app/exif_reader.h>
#include <iostream>

int main(int argc, char ** argv) {

	if(2 != argc) {

		std::cerr<<"use: exif-read file"<<std::endl;
		return 1;
	}

	app::exif_reader reader;
	auto result=reader.read(argv[1]);
	if(!result.is_valid()) {

		std::cerr<<"could not read image: "<<result.get_error_message()<<std::endl;
		return 1;
	}

	std::cout<<"date is :"<<result.get_date()<<std::endl;
	return 1;
}
