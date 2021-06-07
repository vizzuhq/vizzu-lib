
#ifndef IO_RANGESTREAM
#define IO_RANGESTREAM

#include <cstdint>
#include <iostream>
#include <streambuf>
#include <algorithm>

namespace IO
{

class RangeIStream: public std::istream
{
public:

	RangeIStream(std::istream &stream,
				std::streampos begin,
				size_t size)
		: std::basic_ios<char>(&buffer),
		  std::istream(&buffer),
		  buffer(stream, begin, size)
	{
		rdbuf(&buffer);
	}

	RangeIStream(const RangeIStream &other)
		: std::basic_ios<char>(&buffer),
		  std::istream(&buffer),
		  buffer(other.buffer)
	{
		rdbuf(&buffer);
	}

private:

	struct RangeBuffer : public std::streambuf
	{
		RangeBuffer(std::istream &stream,
					std::streampos begin,
					size_t size)
			: stream(stream), pos(0), size(size)
		{
			stream.seekg(begin);
		}

		int_type underflow() override
		{
			if (pos >= size)
				return traits_type::eof();
			else
				return stream.peek();
		}

		int_type uflow() override
		{
			if (pos >= size)
				return traits_type::eof();
			else {
				pos++;
				return stream.get();
			}
		}

		std::streamsize xsgetn(char* s, std::streamsize count) override
		{
			if (pos >= size) return 0;
			
			auto needed = std::min(size - pos, (size_t)count);
			stream.read(s, needed);

			auto readed = stream.gcount();
			pos += readed;
			return readed;
		}

		std::istream &stream;
		size_t pos;
		size_t size;
	};

	RangeBuffer buffer;
};

}

#endif
