#ifndef IO_STREAM
#define IO_STREAM

#include <cstdint>
#include <iostream>
#include <type_traits>

namespace IO
{

class IStream
{
public:
	IStream(std::istream *stream);
	void setStream(std::istream *stream);

	template <typename T>
	typename std::enable_if<
		std::is_arithmetic<T>::value
		|| std::is_enum<T>::value,
		IStream
	>::type&
	operator>>(T &val) { stream->read((char*)&val, sizeof(T)); return *this; }

	bool eof() const { return stream->eof(); }
	bool good() const { return stream->good(); }
	explicit operator bool() const { return stream != nullptr; }

protected:
	std::istream *stream;
};

class OStream
{
public:
	OStream(std::ostream *stream);
	void setStream(std::ostream *stream);

	template <typename T>
	typename std::enable_if<
		std::is_arithmetic<T>::value
		|| std::is_enum<T>::value,
		OStream
	>::type&
	operator<<(const T &val) { stream->write((char*)&val, sizeof(T)); return *this; }

	void inject(const char *data, size_t size) { stream->write(data, size); }

	bool eof() const { return stream->eof(); }
	bool good() const { return stream->good(); }
	explicit operator bool() const { return stream != nullptr; }

protected:
	std::ostream *stream;
};

}

#endif
