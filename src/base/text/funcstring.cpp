#include "funcstring.h"

#include "smartstring.h"

namespace Text
{

FuncString::FuncString(std::string code, bool throwOnError)
{
	Text::SmartString::trim(code);

	if (code.empty()) return;

	std::array<std::string_view, 2> parts{};
	if (auto where = code.find('('); where != std::string::npos) {
		parts[0] = std::string_view{code}.substr(0, where);
		parts[1] = std::string_view{code}.substr(where + 1);
	}

	if (parts[1].empty() || parts[1].back() != ')') {
		if (!throwOnError) return;

		throw std::logic_error("invalid function format");
	}

	parts[1].remove_suffix(1);

	name = parts[0];
	params.emplace_back(parts[1]);

	SmartString::trim(name);

	for (auto &param : params) { SmartString::trim(param); }
}

}