#include "clock.h"

using namespace app;

clock::clock() {

	update_time();
}

void clock::update_time() {

	//Substract the modulo 60 to fix it to hour and minute.
	now=time(nullptr);
	now-=now % 60;
}

bool clock::has_changed(time_t _time) const {

	//The clock will have advanced when _time is an exact minute, larger than
	//the current time.

	time_t adjusted=_time % 60;
	return !adjusted && _time > now;
}

std::string	clock::get_time() const {

	time_t rawtime=0;
	struct tm * timeinfo=nullptr;
	char buffer[10];

	time(&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(buffer,sizeof(buffer),"%H:%M",timeinfo);

	return std::string{buffer};
}
