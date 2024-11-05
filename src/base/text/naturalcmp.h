#ifndef TEXT_NATURALCMP
#define TEXT_NATURALCMP

#include <string>

namespace Text
{

class NaturalCmp
{
public:
	explicit NaturalCmp(bool ignoreCase = true,
	    bool ignoreSpace = true);

	[[nodiscard]] bool operator()(const std::string &lhs,
	    const std::string &rhs) const
	{
		return std::is_lt(cmp(lhs, rhs));
	}

	[[nodiscard]] std::weak_ordering cmp(const std::string &lhs,
	    const std::string &rhs) const
	{
		return cmp(lhs.c_str(), rhs.c_str());
	}

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
