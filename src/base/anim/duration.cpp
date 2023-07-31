#include "duration.h"

#include <stdexcept>

#include "base/text/valueunit.h"

using namespace Anim;

Duration::Duration(double nanosec)
{
	if (nanosec >= static_cast<double>(
	        std::numeric_limits<int64_t>::max())
	    || nanosec <= static_cast<double>(
	           std::numeric_limits<int64_t>::min()))
		throw std::logic_error("time duration is too big");
	*this = Duration(static_cast<int64_t>(nanosec));
}

Duration::Duration(const std::string &str)
{
	const Text::ValueUnit valueUnit(str);

	if (valueUnit.getUnit() == "ms")
		*this = MSec(valueUnit.getValue());

	else if (valueUnit.getUnit() == "s"
	         || valueUnit.getUnit().empty())
		*this = Sec(valueUnit.getValue());

	else
		throw std::logic_error("invalid time unit");
}

Duration Duration::Sec(double sec)
{
	return Duration(sec * 1000.0 * 1000.0 * 1000.0);
}

Duration Duration::MSec(double millisec)
{
	return Duration(millisec * 1000.0 * 1000.0);
}

Duration::operator std::string() const
{
	return std::to_string(sec()) + "s";
}

Duration::operator double() const { return count(); }

double Duration::msec() const { return count() / 1000.0 / 1000.0; }

double Duration::sec() const
{
	return count() / 1000.0 / 1000.0 / 1000.0;
}

Duration &Duration::operator+=(const Duration &other)
{
	*this = *this + other;
	return *this;
}

Duration &Duration::operator-=(const Duration &other)
{
	*this = *this - other;
	return *this;
}

Duration Duration::operator-(const Duration &other) const
{
	return Duration(
	    static_cast<double>(*this) - static_cast<double>(other));
}

Duration Duration::operator+(const Duration &other) const
{
	return Duration(
	    static_cast<double>(*this) + static_cast<double>(other));
}

double Duration::operator/(const Duration &other) const
{
	return static_cast<double>(*this) / static_cast<double>(other);
}

Duration Duration::operator*(double other) const
{
	return Duration(static_cast<double>(*this) * other);
}

bool Duration::operator==(const Duration &other) const
{
	return static_cast<double>(*this) == static_cast<double>(other);
}

bool Duration::operator<=(const Duration &other) const
{
	return static_cast<double>(*this) <= static_cast<double>(other);
}

bool Duration::operator<(const Duration &other) const
{
	return static_cast<double>(*this) < static_cast<double>(other);
}

bool Duration::operator>=(const Duration &other) const
{
	return static_cast<double>(*this) >= static_cast<double>(other);
}

bool Duration::operator>(const Duration &other) const
{
	return static_cast<double>(*this) > static_cast<double>(other);
}