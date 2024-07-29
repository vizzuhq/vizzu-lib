#ifndef TEXT_NATURALCMP
#define TEXT_NATURALCMP

#include <string>

#include "immutable_string.h"

namespace Text
{

class NaturalCmp
{
public:
	explicit NaturalCmp(bool ignoreCase = true,
	    bool ignoreSpace = true);
	[[nodiscard]] bool operator()(const immutable_string &,
	    const immutable_string &) const;

private:
	bool ignoreCase;
	bool ignoreSpace;
	[[nodiscard]] std::weak_ordering cmp(const char *,
	    const char *) const;
	[[nodiscard]] std::weak_ordering cmpChar(const char &,
	    const char &) const;
	[[nodiscard]] static std::weak_ordering cmpNum(const char *&,
	    const char *&);
	static void skipSpaces(const char *&);
};

}

#endif
