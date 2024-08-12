#ifndef MATH_ANGLE
#define MATH_ANGLE

#include <cmath>
#include <numbers>
#include <string>

#include "base/text/valueunit.h"

namespace Geom
{

template <int max> class CircularAngle
{
	static CircularAngle fromStr(const Text::ValueUnit &parser);

public:
	static double degToRad(double deg);
	static double radToDeg(double rad);

	CircularAngle() : value(0) {}
	explicit CircularAngle(double value) : value(value)
	{
		sanitize();
	}

	explicit CircularAngle(const std::string &str) :
	    value{fromStr(Text::ValueUnit{str}).value}
	{}

	static CircularAngle Deg(double value);
	static CircularAngle Grad(double value);
	static CircularAngle Turn(double value);
	[[nodiscard]] double deg() const;
	[[nodiscard]] double rad() const { return value; }
	[[nodiscard]] double turn() const;

	explicit operator std::string() const;
	bool operator==(const CircularAngle &other) const;
	CircularAngle operator*(double factor) const;
	CircularAngle operator+(const CircularAngle &other) const;

private:
	double value;

	void sanitize();
};

template <int max>
CircularAngle<max> interpolate(CircularAngle<max> op0,
    CircularAngle<max> op1,
    double factor)
{
	if (factor <= 0.0) return op0;

	if (factor >= 1.0) return op1;

	if (fabs(op0.rad() - op1.rad()) <= std::numbers::pi)
		return CircularAngle<max>(
		    op0.rad() * (1.0 - factor) + op1.rad() * factor);

	if (op0.rad() < op1.rad())
		return CircularAngle<max>(
		    (op0.rad() + 2 * std::numbers::pi) * (1.0 - factor)
		    + op1.rad() * factor);

	return CircularAngle<max>(
	    op0.rad() * (1.0 - factor)
	    + (op1.rad() + 2 * std::numbers::pi) * factor);
}

using Angle180 = CircularAngle<180>;
using Angle360 = CircularAngle<360>;
using Angle = Angle360;

}

#endif
