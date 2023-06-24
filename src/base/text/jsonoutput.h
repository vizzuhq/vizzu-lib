#ifndef TEXT_JSONOUTPUT
#define TEXT_JSONOUTPUT

#include <list>
#include <string>

#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

namespace Text
{

static std::string toJSon(const std::list<std::string> &list)
{
	return "["
	     + Text::SmartString::join(
	         Text::SmartString::map(list,
	             [](const std::string item)
	             {
		             return '\"'
		                  + Text::SmartString::escape(item, "\\\"")
		                  + '\"';
	             }),
	         ",")
	     + "]";
}
/*
class JSonOutput
{
public:
    enum Control { ObjectBegin, ObjectEnd, ListBegin, ListEnd };

    JSonOutput(std::ostream &ostream);

    JSonOutput &operator<<(Control control)
    {
        switch (control) {
            case ObjectBegin: ostream << "{";
            case ObjectEnd: ostream << "}";
            case ListBegin: ostream << "[";
            case ListEnd: ostream << "]";
        }
        return *this;
    }

    JSonOutput &operator<<(const std::string &str)
    {
        ostream << Text::SmartString::escape(str, "\\\"");
        return *this;
    }

private:
    std::ostream &ostream;
};
*/
}

#endif
