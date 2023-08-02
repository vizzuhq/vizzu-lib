#ifndef AUTOPARAM_H
#define AUTOPARAM_H

#include <optional>
#include <stdexcept>
#include <string>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"

namespace Vizzu::Base
{

template <typename Type> struct AutoParam
{
public:
	AutoParam() : autoSet(true) {}

	AutoParam(const Type &value) : autoSet(false), value(value) {}

	AutoParam(std::optional<Type> value) :
	    autoSet(false),
	    value(std::move(value))
	{}

	explicit AutoParam(const std::string &s)
	{
		if (s == "null")
			*this = AutoParam(std::nullopt);
		else if (s == "auto")
			*this = AutoParam();
		else
			*this = AutoParam(Conv::parse<Type>(s));
	}

	explicit operator std::string() const
	{
		if (autoSet)
			return "auto";
		if (!value)
			return "null";
		return Conv::toString(*value);
	}

	explicit operator bool() const { return static_cast<bool>(value); }

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

	bool operator==(const AutoParam &other) const
	{
		return value == other.value && autoSet == other.autoSet;
	}

private:
	bool autoSet;
	std::optional<Type> value;
};

using AutoBool = AutoParam<bool>;

}

#endif
