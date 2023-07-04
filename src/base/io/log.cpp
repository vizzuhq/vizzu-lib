#include "log.h"

#include <ctime>

using namespace IO;

namespace
{
static bool enabled = true;
static Log::LogFunc logFunc = [](std::string const &s)
{
	puts(s.c_str());
};

}

LogRecord::LogRecord() : content("[vizzu] [YYYY-mm-ddTHH:MM:SSZ] ")
{
	std::time_t now;
	std::time(&now);
	std::strftime(content.data() + 9,
	    21,
	    "%Y-%m-%dT%H:%M:%SZ",
	    std::gmtime(&now));
	content[9 + 20] = ']';
}

void Log::set(Log::LogFunc f) { logFunc = std::move(f); }

void Log::setEnabled(bool value) { enabled = value; }

void Log::print(const std::string &msg)
{
	if (logFunc) logFunc(msg);
}
