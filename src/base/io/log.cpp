#include "log.h"

#include <ctime>

IO::Log &IO::Log::getInstance()
{
	static Log log{};
	return log;
}

IO::LogRecord::LogRecord() : content("[vizzu] ")
{
	if (Log::getInstance().timestamp) {
		content += "[YYYY-mm-ddTHH:MM:SSZ] ";
		const std::time_t now{std::time(nullptr)};
		content[9
		        + std::strftime(content.data() + 9,
		            21,
		            "%Y-%m-%dT%H:%M:%SZ",
		            std::gmtime(&now))] = ']';
	}
}

void IO::Log::set(Log::LogFunc f) { getInstance().logFunc = f; }

void IO::Log::setEnabled(bool value)
{
	getInstance().enabled = value;
}

void IO::Log::setTimestamp(bool value)
{
	getInstance().timestamp = value;
}

void IO::Log::print(const std::string &msg)
{
	auto &l = getInstance();
	if (l.enabled && l.logFunc) l.logFunc(msg);
}
