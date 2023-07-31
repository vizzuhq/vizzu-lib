#ifndef TEXT_FUNCSTRING
#define TEXT_FUNCSTRING

#include <string>
#include <vector>

namespace Text
{

class FuncString
{
public:
	using Params = std::vector<std::string>;

	explicit FuncString(std::string code, bool throwOnError = true);
	[[nodiscard]] bool isEmpty() const { return name.empty(); }
	[[nodiscard]] const std::string &getName() const { return name; }
	[[nodiscard]] const Params &getParams() const { return params; }

private:
	std::string name;
	Params params;
};

}

#endif
