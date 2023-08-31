#ifndef GEOM_POINT
#define GEOM_POINT

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>

#include "base/math/tolerance.h"
#include "base/math/trig.h"

namespace Geom
{

struct Point
{
	double x;
	double y;

	Point() { x = y = 0.0; }

	static Point Invalid() { return {NAN, NAN}; }

	static Point Max()
	{
		return {std::numeric_limits<double>::max(),
		    std::numeric_limits<double>::max()};
	}

	static Point Min()
	{
		return {std::numeric_limits<double>::lowest(),
		    std::numeric_limits<double>::lowest()};
	}

	static Point Ident(bool horizontal)
	{
		return {horizontal ? 1.0 : 0.0, horizontal ? 0.0 : 1.0};
	}

	Point(double x, double y) : x(x), y(y) {}

	static Point Polar(double radius, double angle)
	{
		return {radius * cos(angle), radius * sin(angle)};
	}

	static Point X(double x) { return {x, 0}; }

	static Point Y(double y) { return {0, y}; }

	Point operator*(double factor) const
	{
		return {x * factor, y * factor};
	}

	Point operator/(double divisor) const
	{
		if (divisor == 0) return Invalid();
		return {x / divisor, y / divisor};
	}

	Point operator+(const Point &other) const
	{
		return {x + other.x, y + other.y};
	}

	Point operator-(const Point &other) const
	{
		return {x - other.x, y - other.y};
	}

	Point operator*(const Point &other) const
	{
		return {x * other.x, y * other.y};
	}


	[[nodiscard]] double dot(const Point &other) const
	{
		return x * other.x + y * other.y;
	}

	Point operator/(const Point &other) const
	{
		if (other.x == 0 || other.y == 0) return Invalid();
		return {x / other.x, y / other.y};
	}

	double operator^(const Point &p) const
	{
		return x * p.y - y * p.x;
	}

	[[nodiscard]] Point flip() const { return {y, x}; }

	[[nodiscard]] Point flipX() const { return {-x, y}; }

	[[nodiscard]] Point flipY() const { return {x, -y}; }

	[[nodiscard]] Point xComp() const { return {x, 0}; }

	[[nodiscard]] Point yComp() const { return {0, y}; }

	[[nodiscard]] Point comp(bool horizontal) const
	{
		return horizontal ? xComp() : yComp();
	}

	[[nodiscard]] Point mainComp() const
	{
		return fabs(x) >= fabs(y) ? xComp() : yComp();
	}

	[[nodiscard]] Point subComp() const
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

	[[nodiscard]] double abs() const
	{
		if (x == 0.0) return fabs(y);
		if (y == 0.0) return fabs(x);
		return sqrt(x * x + y * y);
	}

	[[nodiscard]] double manhattan() const
	{
		return ::fabs(x) + ::fabs(y);
	}

	[[nodiscard]] double chebyshev() const
	{
		return std::max(::fabs(x), ::fabs(y));
	}

	[[nodiscard]] double sqrAbs() const { return x * x + y * y; }

	[[nodiscard]] double angle() const
	{
		if (y == 0) return x >= 0 ? 0.0 : M_PI;
		if (x == 0) return y >= 0 ? M_PI / 2.0 : -M_PI / 2;
		return atan2f(static_cast<float>(y), static_cast<float>(x));
	}

	[[nodiscard]] Point toPolar() const { return {abs(), angle()}; }

	bool operator==(const Point &other) const
	{
		return x == other.x && y == other.y;
	}

	[[nodiscard]] bool isNull() const { return x == 0 && y == 0; }

	[[nodiscard]] bool isValid() const
	{
		return !std::isnan(x) && !std::isnan(y);
	}

	[[nodiscard]] Point rotated(double angle) const;
	[[nodiscard]] Point normalized() const;
	[[nodiscard]] Point normal(bool clockwise) const;

	[[nodiscard]] double getCoord(bool horizontal) const
	{
		return horizontal ? x : y;
	}

	double &getCoord(bool horizontal) { return horizontal ? x : y; }

	[[nodiscard]] Point leftNormal() const { return Point{y, -x}; }

	[[nodiscard]] Point rightNormal() const { return Point{-y, x}; }

	consteval static auto members() {
		return std::tuple{&Point::x, &Point::y};
	}
};

struct Size : Point
{
	using Point::Point;

	static Size Square(double size) { return {size, size}; }

	static Size HorStrip(double height)
	{
		return {std::numeric_limits<double>::max(), height};
	}

	static Size VerStrip(double width)
	{
		return {width, std::numeric_limits<double>::max()};
	}

	static Size Identity() { return {1, 1}; }
	static Size UpperIdentity(double aspectRatio);
	static Size LowerIdentity(double aspectRatio);

	Size() = default;
	explicit Size(const Point &p) : Point(p) {}

	[[nodiscard]] double area() const { return x * y; }

	[[nodiscard]] bool bounds(const Size &s) const
	{
		return s.x <= x && s.y <= y;
	}

	static Size boundary(const Size &s1, const Size &s2)
	{
		return {std::max(s1.x, s2.x), std::max(s1.y, s2.y)};
	}

	static Size section(const Size &s1, const Size &s2)
	{
		return {std::min(s1.x, s2.x), std::min(s1.y, s2.y)};
	}

	[[nodiscard]] bool isSquare(double toleranceFactor = 0.0) const
	{
		if (y == 0) return false;
		return Math::AddTolerance(fabs(x / y), toleranceFactor) == 1;
	}

	[[nodiscard]] double aspectRatio() const { return x / y; }
	[[nodiscard]] double minSize() const { return std::min(x, y); }
	[[nodiscard]] double diagonal() const { return abs(); }
	[[nodiscard]] Size rotatedSize(double angle) const;

	[[nodiscard]] Size verticalHalf() const { return {x, y / 2}; }
	[[nodiscard]] Size horizontalHalf() const { return {x / 2, y}; }

	[[nodiscard]] Size positive() const
	{
		return {x >= 0 ? x : 0, y >= 0 ? y : 0};
	}
};

}

#endif
