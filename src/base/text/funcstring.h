#ifndef TEXT_FUNCSTRING
#define TEXT_FUNCSTRING

#include <string>
#include <vector>

namespace Text
{

class FuncString
{
public:
	typedef std::vector<std::string> Params;

	explicit FuncString(std::string code, bool throwOnError = true);
	bool isEmpty() const { return name.empty(); }
	const std::string &getName() const { return name; }
	const Params &getParams() const { return params; }

private:
	std::string name;
	Params params;
};

}

#endif
