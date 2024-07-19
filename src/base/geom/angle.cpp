#include "angle.h"

#include <cmath>
#include <stdexcept>

#include "base/text/valueunit.h"

namespace Geom
{

template <int max> double CircularAngle<max>::degToRad(double deg)
{
	return M_PI * deg / 180.0;
}

template <int max> double CircularAngle<max>::radToDeg(double rad)
{
	return 180.0 * rad / M_PI;
}

template <int max>
CircularAngle<max> CircularAngle<max>::fromStr(
    const Text::ValueUnit &parser)
{
	const auto &unit = parser.getUnit();

	if (unit == "deg") { return Deg(parser.getValue()); }
	if (unit == "grad") { return Grad(parser.getValue()); }
	if (unit == "turn") { return Turn(parser.getValue()); }
	if (unit == "rad" || unit.empty()) {
		return CircularAngle<max>(parser.getValue());
	}

	throw std::logic_error("invalid angle unit: " + unit);
}

template <int max> CircularAngle<max>::operator std::string() const
{
	return std::to_string(value) + "rad";
}

template <int max>
CircularAngle<max> CircularAngle<max>::Deg(double value)
{
	return CircularAngle<max>(degToRad(value));
}

template <int max>
CircularAngle<max> CircularAngle<max>::Grad(double value)
{
	return CircularAngle<max>(M_PI * value / 200.0);
}

template <int max>
CircularAngle<max> CircularAngle<max>::Turn(double value)
{
	return CircularAngle<max>(2.0 * M_PI * value);
}

template <int max> double CircularAngle<max>::deg() const
{
	return radToDeg(value);
}

template <int max> double CircularAngle<max>::turn() const
{
	return value / (2.0 * M_PI);
}

template <int max> void CircularAngle<max>::sanitize()
{
	auto angleInDeg = deg();
	angleInDeg = fmod(angleInDeg, max);
	if (std::signbit(angleInDeg)) angleInDeg += max;
	value = degToRad(angleInDeg);
}

template <int max>
bool CircularAngle<max>::operator==(
    const CircularAngle<max> &other) const
{
	return value == other.value;
}

template <int max>
CircularAngle<max> CircularAngle<max>::operator*(double factor) const
{
	return CircularAngle<max>(value * factor);
}

template <int max>
CircularAngle<max> CircularAngle<max>::operator+(
    const CircularAngle<max> &other) const
{
	return CircularAngle<max>(value + other.value);
}

template class CircularAngle<180>;
template class CircularAngle<360>;

}