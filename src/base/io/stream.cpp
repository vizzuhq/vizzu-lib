
#include "stream.h"

#include <cstring>

using namespace IO;

IStream::IStream(std::istream *stream)
	: stream(stream)
{
}

void IStream::setStream(std::istream *stream)
{
	this->stream = stream;
}

OStream::OStream(std::ostream *stream)
	: stream(stream)
{
}

void OStream::setStream(std::ostream *stream)
{
	this->stream = stream;
}
