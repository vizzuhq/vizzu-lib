#ifndef GEOM_POINT
#define GEOM_POINT

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

#include "base/math/tolerance.h"
#include "base/math/trig.h"

namespace Geom
{

struct Point
{
	double x;
	double y;

	Point() { x = y = 0.0; }

	static Point Invalid() { return Point(NAN, NAN); }

	static Point Max()
	{
		return Point(std::numeric_limits<double>::max(),
		    std::numeric_limits<double>::max());
	}

	static Point Min()
	{
		return Point(std::numeric_limits<double>::lowest(),
		    std::numeric_limits<double>::lowest());
	}

	static Point Ident(bool horizontal)
	{
		return Point(horizontal ? 1 : 0, horizontal ? 0 : 1);
	}

	Point(double x, double y) : x(x), y(y) {}

	static Point Polar(double radius, double angle)
	{
		return Point(radius * cos(angle), radius * sin(angle));
	}

	static Point X(double x) { return Point(x, 0); }

	static Point Y(double y) { return Point(0, y); }

	Point operator*(double factor) const
	{
		return Point(x * factor, y * factor);
	}

	Point operator/(double divisor) const
	{
		if (divisor == 0) return Invalid();
		return Point(x / divisor, y / divisor);
	}

	Point operator+(const Point &other) const
	{
		return Point(x + other.x, y + other.y);
	}

	Point operator-(const Point &other) const
	{
		return Point(x - other.x, y - other.y);
	}

	Point operator*(const Point &other) const
	{
		return Point(x * other.x, y * other.y);
	}

	Point operator/(const Point &other) const
	{
		if (other.x == 0 || other.y == 0) return Invalid();
		return Point(x / other.x, y / other.y);
	}

	double operator^(const Point &p) { return x * p.y - y * p.x; }

	Point flip() const { return Point(y, x); }

	Point flipX() const { return Point(-x, y); }

	Point flipY() const { return Point(x, -y); }

	Point xComp() const { return Point(x, 0); }

	Point yComp() const { return Point(0, y); }

	Point comp(bool horizontal) const
	{
		return horizontal ? xComp() : yComp();
	}

	Point mainComp() const
	{
		return fabs(x) >= fabs(y) ? xComp() : yComp();
	}

	Point subComp() const
	{
		return fabs(x) >= fabs(y) ? yComp() : xComp();
	}

	double &operator[](size_t index)
	{
		if (index == 0) return x;
		if (index == 1) return y;
		throw std::logic_error(
		    "internal error: point coordinate index out of bounds");
	}

	double abs() const
	{
		if (x == 0.0) return fabs(y);
		if (y == 0.0) return fabs(x);
		return sqrt(x * x + y * y);
	}

	double manhattan() const { return ::fabs(x) + ::fabs(y); }

	double chebyshev() const
	{
		return std::max(::fabs(x), ::fabs(y));
	}

	double sqrAbs() const { return x * x + y * y; }

	double angle() const
	{
		if (y == 0) return x >= 0 ? 0.0 : M_PI;
		if (x == 0) return y >= 0 ? M_PI / 2.0 : -M_PI / 2;
		return atan2f(static_cast<float>(y), static_cast<float>(x));
	}

	Point toPolar() const
	{
		return Point(abs(), angle());
	}

	bool operator==(const Point &other) const
	{
		return x == other.x && y == other.y;
	}

	bool isNull() const { return x == 0 && y == 0; }

	bool isValid() const { return !std::isnan(x) && !std::isnan(y); }

	Point rotated(double angle) const;
	Point normalized() const;
	Point normal(bool clockwise) const;

	explicit operator std::string() const
	{
		return "{\"x\":"
		     + (std::isnan(x) ? "null" : std::to_string(x))
		     + ",\"y\":"
		     + (std::isnan(y) ? "null" : std::to_string(y)) + "}";
	}

	double getCoord(bool horizontal) const
	{
		return horizontal ? x : y;
	}

	double &getCoord(bool horizontal) { return horizontal ? x : y; }

	Point leftNormal() const { return Point{y, -x}; }

	Point rightNormal() const { return Point{-y, x}; }
};

struct Size : Point
{
	using Point::Point;

	static Size Square(double size) { return Size(size, size); }

	static Size HorStrip(double height)
	{
		return Size(std::numeric_limits<double>::max(), height);
	}

	static Size VerStrip(double width)
	{
		return Size(width, std::numeric_limits<double>::max());
	}

	static Size Identity() { return Size(1, 1); }
	static Size UpperIdentity(double aspectRatio);
	static Size LowerIdentity(double aspectRatio);

	Size() {}
	Size(const Point &p) : Point(p) {}

	double area() const { return x * y; }

	bool bounds(const Size &s) const { return s.x <= x && s.y <= y; }

	static Size boundary(const Size &s1, const Size &s2)
	{
		return Size(std::max(s1.x, s2.x), std::max(s1.y, s2.y));
	}

	static Size section(const Size &s1, const Size &s2)
	{
		return Size(std::min(s1.x, s2.x), std::min(s1.y, s2.y));
	}

	bool isSquare(double toleranceFactor = 0.0) const
	{
		if (y == 0) return false;
		return Math::addTolerance(fabs(x / y), toleranceFactor) == 1;
	}

	double aspectRatio() const { return x / y; }
	double minSize() const { return std::min(x, y); }
	double diagonal() const { return abs(); }
	Size rotatedSize(double angle) const;

	Size verticalHalf() const { return Size(x, y / 2); }
	Size horizontalHalf() const { return Size(x / 2, y); }

	Size positive() const
	{
		return Size(x >= 0 ? x : 0, y >= 0 ? y : 0);
	}
};

}

#endif
