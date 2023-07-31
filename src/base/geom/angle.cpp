#include "angle.h"

#include <cmath>
#include <stdexcept>

#include "base/text/valueunit.h"

using namespace Geom;

template <int max> double CircularAngle<max>::degToRad(double deg)
{
	return M_PI * deg / 180.0;
}

template <int max> double CircularAngle<max>::radToDeg(double rad)
{
	return 180.0 * rad / M_PI;
}

template <int max>
CircularAngle<max>::CircularAngle(const std::string &str)
{
	const Text::ValueUnit parser(str);
	const auto& unit = parser.getUnit();

	if (unit == "deg") { *this = Deg(parser.getValue()); }
	else if (unit == "grad") {
		*this = Grad(parser.getValue());
	}
	else if (unit == "turn") {
		*this = Turn(parser.getValue());
	}
	else if (unit == "rad" || unit.empty()) {
		*this = CircularAngle<max>(parser.getValue());
	}
	else
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
	if (angleInDeg < 0) angleInDeg += max;
	value = degToRad(angleInDeg);
}

template <int max>
bool Geom::CircularAngle<max>::operator==(
    const CircularAngle<max> &other) const
{
	return value == other.value;
}

template <int max>
Geom::CircularAngle<max> Geom::CircularAngle<max>::operator*(
    double factor) const
{
	return CircularAngle<max>(value * factor);
}

template <int max>
Geom::CircularAngle<max> Geom::CircularAngle<max>::operator+(
    const CircularAngle<max> &other) const
{
	return CircularAngle<max>(value + other.value);
}

template class Geom::CircularAngle<180>;
template class Geom::CircularAngle<360>;
