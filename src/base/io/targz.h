#ifndef IO_TARGZREADER
#define IO_TARGZREADER

#include <vector>
#include <iostream>
#include <sstream>

#include "tarreader.h"
#include "tarbuilder.h"
#include "memorystream.h"
#include "gzip.h"

namespace IO
{
namespace TarGz
{

class Reader : public Tar::Reader {
public:
	Reader(const char *begin, size_t size);
	~Reader();
protected:
	std::stringstream *stream;
	std::istream &initStream(const char *begin, size_t size);
};

class Writer : public Tar::Builder {
public:
	Writer(std::ostream &out);

	void close() override;

protected:
	std::ostream &stream;
	std::stringstream rawData;
};


}
}

#endif
