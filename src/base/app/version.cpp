#include "base/app/version.h"

#include <string>

#include "base/app/git.h"

namespace App
{

std::string Version::buildTimestamp()
{
	return __DATE__ " " __TIME__;
}

Version::operator std::string() const
{
	return std::to_string(major) + "." + std::to_string(minor) + "."
	     + std::to_string(patch) + "-" + std::to_string(Git::depth)
	     + "-" + Git::hash + "-" + "[" + buildTimestamp() + "]";
}

}