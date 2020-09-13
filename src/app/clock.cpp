#include <app/clock.h>

using namespace app;

clock::clock() {

	update_time();
}

void clock::update_time() {

	//Substract the modulo 60 to fix it to hour and minute.
	now=time(nullptr);
	fixed_to_minute=now;
	fixed_to_minute-=fixed_to_minute % 60;
}

bool clock::has_changed(time_t _time, bool _each_second) const {

	if(_each_second ) {

		return _time > now;
	}

	//The clock will have advanced when _time is an exact minute, larger than
	//the current time...

	time_t adjusted=_time % 60;
	return !adjusted && _time > fixed_to_minute;
}

std::string	clock::get_time(bool _with_seconds) const {

	time_t rawtime=0;
	struct tm * timeinfo=nullptr;
	char buffer[20];

	time(&rawtime);
	timeinfo=localtime(&rawtime);

	std::string format=_with_seconds
		? "%H:%M:%S"
		: "%H:%M";

	strftime(buffer, sizeof(buffer), format.c_str(), timeinfo);

	return std::string{buffer};
}
