#ifndef IO_LOG
#define IO_LOG

#include <functional>
#include <string>
#include <memory>
#include <type_traits>

#include "base/conv/tostring.h"

namespace IO
{

class Log
{
public:
	typedef std::function<void(const std::string&)> LogFunc;

	static void set(LogFunc f);
	Log(const std::string &msg);

private:
	static LogFunc logFunc;
};

class LogRecord
{
public:
	LogRecord() = default;
	LogRecord(LogRecord &&) = default;
	~LogRecord() { Log { content }; }

	template<typename T>
	LogRecord& operator<<(const T &value) {
		content += Conv::toString(value);
		return *this;
	}

private:
	std::string content;
};

inline LogRecord log() { return LogRecord(); }

}

#endif
