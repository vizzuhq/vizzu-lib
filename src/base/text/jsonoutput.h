#ifndef TEXT_JSONOUTPUT
#define TEXT_JSONOUTPUT

#include <string>
#include <list>
//#include <ostream>

#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

namespace Text
{

static std::string toJSon(const std::list<std::string> &list)
{
	return "["
		+ Text::SmartString::join(
			Text::SmartString::map(
				list, 
				[](const std::string item) {
					return 
						'\"' + Text::SmartString::escape(item, "\\\"") + '\"';
				}),
			",")
		+ "]";
}

}

#endif
