#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#if USE_ASIO_ALONE
#include <asio.hpp>
#else
#include <boost/asio.hpp>
namespace asio {
	using namespace boost::asio;
	using boost::system::error_code;
}
#endif

struct File_info {
	typedef unsigned long long Size_type;
	Size_type filesize;
	size_t filename_size;
	File_info() : filesize(0), filename_size(0) {}
};

#endif