#include "duration.h"

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

#include "base/text/valueunit.h"

namespace Anim
{

Duration::Duration(double nanosec) :
    Base(static_cast<int64_t>(nanosec))
{
	if (nanosec >= static_cast<double>(
	        std::numeric_limits<int64_t>::max())
	    || nanosec <= static_cast<double>(
	           std::numeric_limits<int64_t>::min()))
		throw std::logic_error("time duration is too big");
}

Duration::Duration(const Text::ValueUnit &valueUnit) :
    Base((valueUnit.getUnit() == "ms" ? MSec(valueUnit.getValue())
                                      : Sec(valueUnit.getValue()))
             .count())
{
	if (valueUnit.getUnit() != "ms" && valueUnit.getUnit() != "s"
	    && !valueUnit.getUnit().empty()) [[unlikely]]
		throw std::logic_error("invalid time unit");
}

Duration::Duration(const std::string &str) :
    Duration(Text::ValueUnit{str})
{}

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

Duration::operator double() const
{
	return static_cast<double>(count());
}

double Duration::msec() const
{
	return static_cast<double>(*this) / 1000.0 / 1000.0;
}

double Duration::sec() const { return msec() / 1000.0; }

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

}