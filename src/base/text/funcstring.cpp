#include "funcstring.h"

#include "smartstring.h"

namespace Text
{

FuncString::FuncString(std::string code, bool throwOnError)
{
	Text::SmartString::trim(code);

	if (code.empty()) return;

	auto parts = SmartString::split(code, '(');

	if (parts.size() != 2 || parts[1].empty()
	    || parts[1].back() != ')') {
		if (!throwOnError) return;

		throw std::logic_error("invalid function format");
	}

	parts[1].pop_back();

	name = parts[0];
	params = SmartString::split(parts[1], ',');

	SmartString::trim(name);

	for (auto &param : params) { SmartString::trim(param); }
}

}