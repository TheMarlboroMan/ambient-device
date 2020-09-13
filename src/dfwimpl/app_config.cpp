#include <dfwimpl/app_config.h>

using namespace app;

app_config::app_config(): base_config(get_file_path()) {

}


dfw::input_description app::input_description_from_config_token(const rapidjson::Value& tok) {

	if(!tok.IsArray()) {
		throw std::runtime_error("app::input_description_from_config_token, token is not vector");
	}
	const auto& v=tok.GetArray();

	if(v.Size() != 3) throw std::runtime_error("app::input_description_from_config_token, vector has not size of 3");
	return dfw::input_description{input_description_type_from_int(v[0].GetInt()), v[2].GetInt(), v[1].GetInt()};
}

std::vector<int> app::config_token_from_input_description(const dfw::input_description& id) {

	return std::vector<int>{
		input_description_int_from_type(id.type),
		id.device,
		id.code
	};
}

dfw::input_description::types app::input_description_type_from_int(int v) {

	switch(v) {
		case 0: return dfw::input_description::types::keyboard; break;
		case 1: return dfw::input_description::types::mouse; break;
		case 2: return dfw::input_description::types::joystick; break;
	}

	return dfw::input_description::types::none;
}

int app::input_description_int_from_type(dfw::input_description::types t) {

	switch(t) {
		case dfw::input_description::types::keyboard: 	return 0; break;
		case dfw::input_description::types::mouse: 	return 1; break;
		case dfw::input_description::types::joystick: 	return 2; break;
		case dfw::input_description::types::none: 	return 3; break;
	}

	return 3;
}
