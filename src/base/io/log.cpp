#include "log.h"

using namespace IO;

Log::LogFunc Log::logFunc;

void Log::set(Log::LogFunc f)
{
	logFunc = std::move(f);
}

Log::Log(const std::string &msg)
{
	if (logFunc) logFunc(msg);
}
