#include <app/exif_reader.h>

#include <arpa/inet.h>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace app;

exif_reader::exif_reader():
	is_big_endian_machine{htonl(47) == 47} {

}

exif_reader::tag_set exif_reader::read(const std::string& _path) const {

	exif_reader::tag_set result;

	std::ifstream infile(_path);
	if(!infile) {

		return result.set_error_message("could not open file");
	}

	std::array<std::uint8_t, 128> buffer{};
	buffer.fill(0);

	//Identify JPG header.
	read_into_buffer(infile, buffer, 2);
	if(buffer[0]!=0xff || buffer[1]!=0xd8) {

		return result.set_error_message("bad jpeg header");
	}

	//This will be the start of the TIFF data, where all offsets must be applied.
	auto tiff_offset{-1}, ifd_offset{0};
	bool file_is_big_endian=false;

	//We should be expecting markers and markers...
	while(true) {

		read_into_buffer(infile, buffer, 2);
		if(buffer[0]!=0xff) {

			return result.set_error_message("bad section marker, expecting 0xff");
		}

		uint8_t marker_type=buffer[1];
		//This would be the "beginning of image data marker, so we are done"...
		if(marker_type==0xda) {

			break;
		}

		//let us read the size of the marker now, which is two bytes, big endian (motorola style)
		infile.read((char *)buffer.data(), 2);
		auto length=to_uint16(buffer.data(), 2);
		length-=2; //Because we already read two

		if(marker_type==0xe1) {

			//Now, we should read Exif+0x00+0x00
			read_into_buffer(infile, buffer, 6);
			//TODO: memcmp???
			if(buffer[0]!='E' || buffer[1]!='x' || buffer[2]!='i' || buffer[3]!='f'
				|| buffer[4]!=0x00 || buffer[5]!=0x00
			) {

				return result.set_error_message("bad exif marker");
			}

			//Moving into the TIFF header...
			//Next, we are looking for MM or II for motorola or intel stuff. This will be TIFFoffset...
			tiff_offset=infile.tellg();

			read_into_buffer(infile, buffer, 2);
			if(buffer[0]=='I' && buffer[1]=='I') {

				file_is_big_endian=false;
			}
			else if(buffer[0]=='M' && buffer[1]=='M') {

				file_is_big_endian=true;
			}
			else {
				
				return result.set_error_message("bad endianess marker");
			}

			//Let us now read the static mark...
			read_into_buffer(infile, buffer, 2);
			if(file_is_big_endian) {

				if(buffer[0]!=0x00 || buffer[1]!=0x2a) {

					return result.set_error_message("bad tag in big endian file");
				}
			}
			else {

				if(buffer[0]!=0x2a || buffer[1]!=0x00) {

					return result.set_error_message("bad tag in little endian file");
				}
			}

			//Read the offset now...
			read_into_buffer(infile, buffer, 4);
			ifd_offset=buffer_to_integer(buffer.data(), 4, file_is_big_endian);

			//travel to offset...
			infile.seekg(tiff_offset+ifd_offset, infile.beg);

			//Moving into the TIFF header.
			read_into_buffer(infile, buffer, 2);

			int data_count=buffer_to_integer(buffer.data(), 2, file_is_big_endian);

			for(int i=0; i< data_count; i++) {

				//Tag type...
				read_into_buffer(infile, buffer, 2);
				int type=buffer_to_integer(buffer.data(), 2, file_is_big_endian);

				//Format...
				read_into_buffer(infile, buffer, 2);
				int format=buffer_to_integer(buffer.data(), 2, file_is_big_endian);

				//Components...
				read_into_buffer(infile, buffer, 4);
				int components=buffer_to_integer(buffer.data(), 4, file_is_big_endian);

				//Data
				read_into_buffer(infile, buffer, 4);
				int data_offset=buffer_to_integer(buffer.data(), 4, file_is_big_endian);

				if(type==306) {

					if(components < 4) {

						//TODO:
						return result.set_error_message("unimplemented reading from small component");
					}

					infile.seekg(tiff_offset+data_offset, infile.beg);
					read_into_buffer(infile, buffer, components);
					std::string date_str{buffer.data(), buffer.data()+components};
					result.set_date(date_str);
				}
			}
		}
		else {

			//...
		}

		infile.seekg(length, infile.cur);
	}

	return result.make_valid();
}

std::uint16_t exif_reader::to_uint16(void * _buffer, std::size_t _size) const {

	std::uint16_t result{0};
	memcpy(&result, _buffer, _size);
	return ntohs(result);
}

void exif_reader::read_into_buffer(std::ifstream& _stream, std::array<std::uint8_t, 128>& _buffer, std::size_t _size) const {

	_buffer.fill(0);
	_stream.read((char *)_buffer.data(), _size);
}

int exif_reader::buffer_to_integer(std::uint8_t * _buffer, std::size_t _size, bool _file_big_endian) const {

	int result=0;

	if(_file_big_endian) {

		if(is_big_endian_machine) {

			std::memcpy(&result, _buffer, sizeof(int));
			return result;
		}
	}
	else {
		if(!is_big_endian_machine) {

			std::memcpy(&result, _buffer, sizeof(int));
			return result;
		}
	}

	//Wasteful and inefficient xD!
	std::vector<std::uint8_t> vec(_buffer, _buffer+_size);
	std::reverse(std::begin(vec), std::end(vec));
	std::memcpy(&result, vec.data(), sizeof(int));

	return result;
}

/*
#include <iomanip>

template <typename T>
void print_buffer(T data, std::size_t length) {

	auto * ptr=data;
	std::cout<<std::hex<<std::setfill('0');
	for(std::size_t i=0; i<length; i++, ptr++) {

		std::cout<<std::hex<<std::setw(2)<<(int)*ptr<<" ";
	}

	std::cout<<std::endl;
}



*/
