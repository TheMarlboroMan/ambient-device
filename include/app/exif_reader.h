#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <vector>

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

		tag_set&                make_valid() {
			valid=true;
			return *this;
		}

		tag_set&                set_error_message(const std::string& _val) {

			error_message=_val;
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

	struct data_index {

		int                    type,
		                       format,
		                       components,
		                       data_offset;
	};

	const bool                 is_big_endian_machine;
	std::uint16_t              to_uint16(void *, std::size_t) const;
	void                       read_into_buffer(std::ifstream&, std::vector<std::uint8_t>& _buffer, std::size_t _size) const;
	int                        buffer_to_integer(std::uint8_t *, std::size_t, bool) const;
	std::string                string_from_data_block(std::ifstream&, const data_index&, std::vector<std::uint8_t>&, int) const;

};
}
