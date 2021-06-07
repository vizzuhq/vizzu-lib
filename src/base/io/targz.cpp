#include "targz.h"

using namespace IO;
using namespace IO::TarGz;

Reader::Reader(const char *begin, size_t size) :
    Tar::Reader(initStream(begin, size))
{}

Reader::~Reader()
{
	delete stream;
}

std::istream &Reader::initStream(const char *begin, size_t size)
{
	stream = new std::stringstream();
	GZip::Reader gzipReader(begin, size, *stream);
	return *stream;
}

Writer::Writer(std::ostream &out)
	: Tar::Builder(rawData), stream(out)
{}

void Writer::close()
{
	Tar::Builder::close();
	auto data = rawData.str();
	GZip::Writer compressor(data.c_str(), data.size(), stream);
}
