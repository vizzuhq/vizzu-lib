
#ifndef IO_MEMORYSTREAM
#define IO_MEMORYSTREAM

#include <cstdint>
#include <iostream>
#include <memory>

namespace IO
{

#if (!defined(__GNUC__) || defined(__clang__) || __GNUC__ >= 5)

class MemoryIStream: public std::istream
{
public:

	MemoryIStream(const uint8_t* data, size_t length)
	: std::istream(&buffer),
	  buffer(data, length)
	{
		rdbuf(&buffer);
	}

	MemoryIStream(MemoryIStream &&other)
	: std::istream(std::move(other)),
	  buffer(other.buffer)
	{
	}

private:

	class MemoryBuffer: public std::basic_streambuf<char>
	{
	public:

		MemoryBuffer(const uint8_t* data, size_t length)
		{
			setg((char*)data, (char*)data, (char*)data + length);
		}

		MemoryBuffer(const std::shared_ptr<uint8_t> &data, size_t length)
			: data(data)
		{
			setg((char*)data.get(), (char*)data.get(), (char*)data.get() + length);
		}

		MemoryBuffer(const MemoryBuffer &other)
			: std::basic_streambuf<char>(other),
			  data(other.data)
		{}

		pos_type seekpos(pos_type sp, std::ios_base::openmode which) override 
		{
			return seekoff(sp - pos_type(off_type(0)), std::ios_base::beg, which);
		}

		pos_type seekoff(off_type off,
			std::ios_base::seekdir dir,
			std::ios_base::openmode which = std::ios_base::in) override
		{
			if (which != std::ios_base::in)
				throw std::logic_error("only seeking of read ptr is supported");

			if (dir == std::ios_base::cur)
				gbump((int)off);
			else if (dir == std::ios_base::end)
				setg(eback(), egptr() + off, egptr());
			else if (dir == std::ios_base::beg)
				setg(eback(), eback() + off, egptr());
			return gptr() - eback();
		}

	private:
		std::shared_ptr<uint8_t> data;
	};

	MemoryBuffer buffer;
};

#endif

}

#endif
