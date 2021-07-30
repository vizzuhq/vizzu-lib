#ifndef MATH_ANGLE
#define MATH_ANGLE

#include <string>

namespace Math
{

class Angle
{
public:
	explicit Angle(double value) : value(value) { sanitize(); }
	explicit Angle(const std::string &str);

	static Angle Deg(double value);
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
