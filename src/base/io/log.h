#ifndef IO_LOG
#define IO_LOG

#include <string>

#include "base/conv/tostring.h"

namespace IO
{

class Log
{
public:
	using LogFunc = void (*)(const std::string &);
	static void set(LogFunc f);
	static void setEnabled(bool);
	static void setTimestamp(bool);

private:
	static void print(const std::string &msg);
	static Log &getInstance();
	Log() = default;
	~Log() = default;

	friend class LogRecord;

	bool enabled = true;
	bool timestamp = true;
	LogFunc logFunc = [](const std::string &s)
	{
		puts(s.c_str());
	};
};

class LogRecord
{
public:
	~LogRecord() { Log::print(content); }

	template <typename T> LogRecord &operator<<(const T &value)
	{
		content += Conv::toString(value);
		return *this;
	}

private:
	friend LogRecord log();

	LogRecord(); // NOLINT
	LogRecord(LogRecord &&) = default;

	std::string content;
};

inline LogRecord log() { return {}; }

}

#endif
