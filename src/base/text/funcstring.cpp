#include "funcstring.h"

#include "smartstring.h"

namespace Text
{

FuncString::FuncString(std::string code, bool throwOnError)
{
	Text::SmartString::trim(code);

	if (code.empty()) return;

	std::array<std::string, 2> parts{};
	if (auto where = code.find('('); where != std::string::npos) {
		parts[0] = code.substr(0, where);
		parts[1] = code.substr(where + 1);
	}

	if (parts[1].empty() || parts[1].back() != ')') {
		if (!throwOnError) return;

		throw std::logic_error("invalid function format");
	}

	parts[1].pop_back();
	SmartString::trim(parts[1]);

	name = parts[0];
	params = SmartString::split(parts[1], ',');

	SmartString::trim(name);

	for (auto &param : params) { SmartString::trim(param); }
}

}