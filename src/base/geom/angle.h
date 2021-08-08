#ifndef MATH_ANGLE
#define MATH_ANGLE

#include <string>

namespace Geom
{

class Angle
{
public:
	static double degToRad(double deg);
	static double radToDeg(double rad);

	explicit Angle(double value) : value(value) { sanitize(); }
	explicit Angle(const std::string &str);

	static Angle Deg(double value);
	static Angle Grad(double value);
	static Angle Turn(double value);
	double deg() const;
	double rad() const { return value; }

	explicit operator std::string() const; 

private:
	double value;

	void sanitize();
};

Angle interpolate(Angle op0, Angle op1, double factor);

}

#endif
