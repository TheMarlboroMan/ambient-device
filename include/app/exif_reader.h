#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <array>

namespace app {

//TODO: This is held together with duct tape. 

class exif_reader {

	public:

	class tag_set {
	
		public:

		bool                   is_valid() const {return valid;}
		const std::string&     get_date() const {return date;}
		const std::string&     get_error_message() const {return error_message;}

		tag_set&                set_date(const std::string& _val) {

			date=_val;
			return *this;
		}

		tag_set&                set_error_message(const std::string& _val) {

			date=_val;
			return *this;
		}

		private:
		std::string            date,
		                       error_message;
		bool                   valid{false};
	};

	                           exif_reader();

	//! Reads a file path and returns a result object.
	tag_set                    read(const std::string&) const;

	private:

	const bool                 is_big_endian_machine;
	std::uint16_t              to_uint16(void *, std::size_t) const;
	void                       read_into_buffer(std::ifstream&, std::array<std::uint8_t, 128>& _buffer, std::size_t _size) const;
	int                        buffer_to_integer(std::uint8_t *, std::size_t, bool) const;

};
}
