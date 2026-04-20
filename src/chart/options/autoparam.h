#ifndef AUTOPARAM_H
#define AUTOPARAM_H

#include <optional>
#include <stdexcept>
#include <string>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"

namespace Vizzu::Base
{

template <typename Type, bool nullable = false> struct AutoParam
{
	AutoParam() : autoSet(true) {}

	explicit AutoParam(const Type &value)
	    requires(!std::is_same_v<Type, std::string>)
	    : value(value)
	{}

	explicit AutoParam(std::optional<Type> value) :
	    value(std::move(value))
	{}

	explicit AutoParam(const std::string &s) : autoSet(s == "auto")
	{
		if (!autoSet) {
			if constexpr (nullable) {
				value = Conv::parse<std::optional<Type>>(s);
			}
			else {
				value = Conv::parse<Type>(s);
			}
		}
	}

	[[nodiscard]] auto toString() const
	    requires(requires { Conv::toString(std::declval<Type>()); })
	{
		if (autoSet) return decltype(Conv::toString(value)){"auto"};
		if constexpr (nullable)
			return Conv::toString(value);
		else
			return Conv::toString(*value);
	}

	[[nodiscard]] explicit operator bool() const
	{
		return static_cast<bool>(value);
	}

	const Type &operator*() const { return *value; }

	[[nodiscard]] bool isAuto() const { return autoSet; }

	void setAuto(std::optional<Type> value)
	{
		if (isAuto()) this->value = std::move(value);
	}

	[[nodiscard]] Type getValue(const Type &autoValue) const
	{
		return isAuto() ? autoValue : *value;
	}

	[[nodiscard]] std::optional<Type> getValue() const
	{
		return isAuto() ? std::nullopt : value;
	}

	bool operator==(const Type &other) const
	{
		return value == other;
	}

	bool operator==(const AutoParam &other) const = default;

	[[nodiscard]] const std::optional<Type> &getValueOrAuto() const
	{
		return value;
	}

	void makeAuto() { autoSet = true; }

private:
	bool autoSet{};
	std::optional<Type> value;
};

using AutoBool = AutoParam<bool>;

}

#endif
