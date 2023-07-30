#include "log.h"

#include <ctime>

using namespace IO;

namespace
{
bool enabled = true;
bool timestamp = true;
Log::LogFunc logFunc = [](std::string const &s)
{
	puts(s.c_str());
};

}

LogRecord::LogRecord() : content("[vizzu] ")
{
	if (timestamp) {
		content += "[YYYY-mm-ddTHH:MM:SSZ] ";
		std::time_t now;
		std::time(&now);
		std::strftime(content.data() + 9,
		    21,
		    "%Y-%m-%dT%H:%M:%SZ",
		    std::gmtime(&now));
		content[9 + 20] = ']';
	}
}

void Log::set(Log::LogFunc f) { logFunc = std::move(f); }

void Log::setEnabled(bool value) { enabled = value; }

void Log::setTimestamp(bool value) { timestamp = value; }

void Log::print(const std::string &msg)
{
	if (logFunc) logFunc(msg);
}
