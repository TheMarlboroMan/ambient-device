#pragma once

#include <ctime>
#include <string>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <mutex>

namespace app {

class clock {

	public:

	//Callbacks give and take nothing.
	using callback=std::function<void ()>;

	//!Class constructor and destructor.
	                                    clock();
	                                    ~clock();

	//!Returns the current hour and minute as a string (optionally seconds)
	std::string                         get_time(bool) const;

	void                                subscribe_second(const std::string, callback);
	void                                unsubscribe_second(const std::string);
	bool                                is_subscribed_second(const std::string) const;
	void                                subscribe_minute(const std::string, callback);
	void                                unsubscribe_minute(const std::string);
	bool                                is_subscribed_minute(const std::string) const;

	bool                                is_running() const {return running;}

	//!Starts the clock thread.
	void                                start();

	//!Requests the stopping the clock thread.
	void                                stop();


	private:

	void                                tick();

	std::mutex                          mtx;
	std::thread                         tick_thread;

	bool                                running=false; //!Is own thread is running...
	bool                                stop_requested=false;

	time_t                              last_tick,
	                                    fixed_to_minute;

	std::map<std::string, callback>     second_clients;
	std::map<std::string, callback>     minute_clients;

};

}
