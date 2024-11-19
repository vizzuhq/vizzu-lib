#ifndef VIZZU_OPTIONS_SPLIT_H
#define VIZZU_OPTIONS_SPLIT_H

#include <stdexcept>
#include <string>

#include "base/text/valueunit.h"

namespace Vizzu::Gen
{
struct Split
{
	Base::AutoParam<double> value{};

	static Split fromString(const std::string &str)
	{
		Split res;
		if (str == "true")
			res.value.setAuto(1.0 / 15.0);
		else if (str != "false") {
			const Text::ValueUnit vu(str);
			if (vu.getUnit() != "%")
				throw std::logic_error(
				    "invalid split unit: " + vu.getUnit());

			res.value = Base::AutoParam{vu.getValue() / 100.0};
		}
		return res;
	}

	[[nodiscard]] std::string toString() const
	{
		if (value.isAuto()) return value ? "true" : "false";
		return std::to_string(*value) + "%";
	}

	[[nodiscard]] explicit operator bool() const
	{
		return static_cast<bool>(value);
	}

	[[nodiscard]] double getPercent() const { return *value; }
};
}

#endif
