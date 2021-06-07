
#include "gzip.h"

#include <array>
#include <ctime>
#include <stdexcept>

using namespace IO;
using namespace IO::GZip;

void Processor::setupZStream()
{
	stream.zalloc = nullptr;
	stream.zfree = nullptr;
	stream.opaque = nullptr;
	stream.avail_in = 0;
	stream.next_in = nullptr;
	if (init() != Z_OK)
		throw std::logic_error("Decompression failed.");
}

void Processor::processStream(const char *begin, size_t size)
{
	stream.avail_in = size;
	stream.next_in = (Bytef *)begin;
	do {
		std::array<Bytef, buffSize> out;
		stream.avail_out = buffSize;
		stream.next_out = out.data();
		int state = process();
		checkStreamError(state);
		auto outSize = (int)buffSize - stream.avail_out;
		if (outSize > 0) output.write((const char*)out.data(), outSize);
		if (isEnd(state)) {
			end();
			return;
		}
	} while (true);
}

void Processor::checkStreamError(int errorCode)
{
	switch (errorCode) {
	case Z_STREAM_ERROR:
	case Z_NEED_DICT:
	case Z_DATA_ERROR:
	case Z_MEM_ERROR:
		end();
		throw std::logic_error("Compression/decompression failed.");
	}
}

Reader::Reader(const char *begin, size_t size, std::ostream &out)
	: Processor(out)
{
	setupZStream();
	processStream(begin, size);
}

int Reader::init()
{
	return inflateInit2(&stream, MAX_WBITS + 16);
}

int Reader::process()
{
	return inflate(&stream, Z_NO_FLUSH);
}

bool Reader::isEnd(int state)
{
	return state == Z_STREAM_END;
}

void Reader::end()
{
	inflateEnd(&stream);
}

Writer::Writer(const char *begin, size_t size, std::ostream &out)
	: Processor(out)
{
	setupZStream();
	processStream(begin, size);
}

int Writer::init()
{
	return deflateInit2(&stream, 6, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
}

int Writer::process()
{
	return deflate(&stream, Z_FINISH);
}

bool Writer::isEnd(int state)
{
	return state == Z_STREAM_END;
}

void Writer::end()
{
	deflateEnd(&stream);
}
