#ifndef IO_LOG
#define IO_LOG

#include <functional>
#include <string>
#include <sstream>
#include <memory>
#include <type_traits>

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
	~LogRecord() { Log(stream.str()); }

	template<typename T>
	typename std::enable_if_t<!std::is_constructible<std::string, T>::value,
	LogRecord>
	&operator<<(const T &value) {
		stream << value << " ";
		return *this;
	}

	template<typename T>
	typename std::enable_if_t<std::is_constructible<std::string, T>::value,
	LogRecord>
	&operator<<(const T &value) {
		stream << (std::string)value << " ";
		return *this;
	}

private:
	std::stringstream stream;
};

inline LogRecord log() { return LogRecord(); }

}

#endif
