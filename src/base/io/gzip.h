#ifndef IO_GZIP
#define IO_GZIP

#include <iostream>
#include <string>
#include <zlib.h>

namespace IO
{
namespace GZip
{

class Processor {
public:
	Processor(std::ostream &out) : output(out) {}
	virtual ~Processor() {}
protected:
	enum { buffSize = 128*1024 };
	z_stream stream;
	std::ostream &output;
	void checkStreamError(int errorCode);
	void setupZStream();
	void processStream(const char *begin, size_t size);
	virtual int init() = 0;
	virtual int process() = 0;
	virtual bool isEnd(int state) = 0;
	virtual void end() = 0;
};

class Reader : public Processor {
public:
	Reader(const char *begin, size_t size, std::ostream &out);
protected:
	int init() override;
	int process() override;
	bool isEnd(int state) override;
	void end() override;
};

class Writer : public Processor
{
public:
	Writer(const char *begin, size_t size, std::ostream &out);
private:
	int init() override;
	int process() override;
	bool isEnd(int state) override;
	void end() override;
};

}
}

#endif
