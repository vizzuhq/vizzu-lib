#ifndef MATH_ANGLE
#define MATH_ANGLE

#include <string>

namespace Geom
{

template <int max> class CircularAngle
{
public:
	static double degToRad(double deg);
	static double radToDeg(double rad);

	CircularAngle() : value(0) {}
	explicit CircularAngle(double value) : value(value) { sanitize(); }
	explicit CircularAngle(const std::string &str);

	static CircularAngle Deg(double value);
	static CircularAngle Grad(double value);
	static CircularAngle Turn(double value);
	double deg() const;
	double rad() const { return value; }
	double turn() const;

	explicit operator std::string() const;
	bool operator==(const CircularAngle &other) const;
	CircularAngle operator*(double factor) const;
	CircularAngle operator+(const CircularAngle &other) const;

private:
	double value;

	void sanitize();
};

template <int max> 
CircularAngle<max> interpolate(
	CircularAngle<max> op0, 
	CircularAngle<max> op1, 
	double factor);

typedef CircularAngle<180> Angle180;
typedef CircularAngle<360> Angle360;
typedef Angle360 Angle;

}

#endif
