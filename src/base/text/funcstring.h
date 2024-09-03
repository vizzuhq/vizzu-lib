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

	[[nodiscard]] static FuncString fromString(
	    const std::string &code)
	{
		return FuncString{code, true};
	}

	explicit FuncString(std::string code, bool throwOnError = false);

	[[nodiscard]] bool isEmpty() const { return name.empty(); }
	[[nodiscard]] const std::string &getName() const { return name; }
	[[nodiscard]] const Params &getParams() const { return params; }

private:
	std::string name;
	Params params;
};

}

#endif
