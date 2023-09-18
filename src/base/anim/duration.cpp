#include "duration.h"

#include <stdexcept>

#include "base/text/valueunit.h"

Anim::Duration::Duration(double nanosec) :
    Base(static_cast<int64_t>(nanosec))
{
	if (nanosec >= static_cast<double>(
	        std::numeric_limits<int64_t>::max())
	    || nanosec <= static_cast<double>(
	           std::numeric_limits<int64_t>::min()))
		throw std::logic_error("time duration is too big");
}

Anim::Duration::Duration(const Text::ValueUnit &valueUnit) :
    Base((valueUnit.getUnit() == "ms" ? MSec(valueUnit.getValue())
          : valueUnit.getUnit() == "s" || valueUnit.getUnit().empty()
              ? Sec(valueUnit.getValue())
              : throw std::logic_error("invalid time unit"))
             .count())
{}

Anim::Duration::Duration(const std::string &str) :
    Duration(Text::ValueUnit{str})
{}

Anim::Duration Anim::Duration::Sec(double sec)
{
	return Duration(sec * 1000.0 * 1000.0 * 1000.0);
}

Anim::Duration Anim::Duration::MSec(double millisec)
{
	return Duration(millisec * 1000.0 * 1000.0);
}

Anim::Duration::operator std::string() const
{
	return std::to_string(sec()) + "s";
}

Anim::Duration::operator double() const
{
	return static_cast<double>(count());
}

double Anim::Duration::msec() const
{
	return static_cast<double>(*this) / 1000.0 / 1000.0;
}

double Anim::Duration::sec() const { return msec() / 1000.0; }

Anim::Duration &Anim::Duration::operator+=(const Duration &other)
{
	*this = *this + other;
	return *this;
}

Anim::Duration &Anim::Duration::operator-=(const Duration &other)
{
	*this = *this - other;
	return *this;
}

Anim::Duration Anim::Duration::operator-(const Duration &other) const
{
	return Duration(
	    static_cast<double>(*this) - static_cast<double>(other));
}

Anim::Duration Anim::Duration::operator+(const Duration &other) const
{
	return Duration(
	    static_cast<double>(*this) + static_cast<double>(other));
}

double Anim::Duration::operator/(const Duration &other) const
{
	return static_cast<double>(*this) / static_cast<double>(other);
}

Anim::Duration Anim::Duration::operator*(double other) const
{
	return Duration(static_cast<double>(*this) * other);
}

bool Anim::Duration::operator==(const Duration &other) const
{
	return static_cast<double>(*this) == static_cast<double>(other);
}

bool Anim::Duration::operator<=(const Duration &other) const
{
	return static_cast<double>(*this) <= static_cast<double>(other);
}

bool Anim::Duration::operator<(const Duration &other) const
{
	return static_cast<double>(*this) < static_cast<double>(other);
}

bool Anim::Duration::operator>=(const Duration &other) const
{
	return static_cast<double>(*this) >= static_cast<double>(other);
}

bool Anim::Duration::operator>(const Duration &other) const
{
	return static_cast<double>(*this) > static_cast<double>(other);
}