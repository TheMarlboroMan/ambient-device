#include <app/env.h>

#include <stdlib.h>

std::string app::get_data_dir() {

	return std::string{getenv("HOME")}+"/.ambient-device/data/";
}

std::string app::get_log_dir() {

	return std::string{getenv("HOME")}+"/.ambient-device/logs/";
}