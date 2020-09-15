#include <app/clock.h>

//TODO:
#include <iostream>

#include <stdexcept>

using namespace app;

clock::clock() {

	last_tick=time(nullptr);
	fixed_to_minute=last_tick;
	fixed_to_minute-=fixed_to_minute % 60;
}

clock::~clock() {

	if(running) {

		stop();
		tick_thread.join();
	}
}

std::string	clock::get_time(bool _with_seconds) const {

	time_t rawtime=0;
	time(&rawtime);
	struct tm * timeinfo=localtime(&rawtime);

	std::string format=_with_seconds
		? "%H:%M:%S"
		: "%H:%M";

	char buffer[20];
	strftime(buffer, sizeof(buffer), format.c_str(), timeinfo);

	return std::string{buffer};
}

void clock::start() {

	running=true;
	tick_thread=std::thread(&clock::tick, this);
}

void clock::stop() {

	std::lock_guard lock{mtx};
	stop_requested=true;
}

void clock::tick() {

	while(!stop_requested) {

		{
			std::lock_guard lock{mtx};


			//Check a second passed...
			time_t now=time(nullptr);
			if(now > last_tick) {

				for(auto fn :second_clients) {
					fn.second();
				}
			}

			//Check a minute passed...
			//The clock will have advanced when _time is an exact minute, larger than
			//the current time...
			time_t adjusted=now % 60;
			if(!adjusted && now > fixed_to_minute) {
				for(auto fn :minute_clients) {
					fn.second();
				}
			}

			//Adjust time.
			last_tick=time(nullptr);
			fixed_to_minute=last_tick;
			fixed_to_minute-=fixed_to_minute % 60;
		} //Ensures the destruction of the lock before sleeping...

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	second_clients.clear();
	minute_clients.clear();

	running=false;
}

void clock::subscribe_second(const std::string _name, callback _fn) {

	std::lock_guard lock{mtx};

	if(second_clients.count(_name)) {

		throw std::runtime_error(std::string{"second subscriber already exists with name "}+_name);
	}

	second_clients[_name]=_fn;
}

void clock::unsubscribe_second(const std::string _name) {

	std::lock_guard lock{mtx};

	if(!second_clients.count(_name)) {

		throw std::runtime_error(std::string{"second subscriber does not exist for name "}+_name);
	}

	second_clients.erase(_name);
}

bool clock::is_subscribed_second(const std::string _name) const {

	return second_clients.count(_name);
}

void clock::subscribe_minute(const std::string _name, callback _fn) {

	std::lock_guard lock{mtx};

	if(minute_clients.count(_name)) {

		throw std::runtime_error(std::string{"minute subscriber already exists with name "}+_name);
	}

	minute_clients[_name]=_fn;
}

void clock::unsubscribe_minute(const std::string _name) {

	std::lock_guard lock{mtx};

	if(!second_clients.count(_name)) {

		throw std::runtime_error(std::string{"minute subscriber does not exist for name "}+_name);
	}

	minute_clients.erase(_name);
}

bool clock::is_subscribed_minute(const std::string _name) const {

	return minute_clients.count(_name);
}
