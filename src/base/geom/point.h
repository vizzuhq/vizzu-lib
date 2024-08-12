#ifndef GEOM_POINT
#define GEOM_POINT

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

#include "base/math/floating.h"
#include "base/math/tolerance.h"

namespace Geom
{

struct Point
{
	double x{0.0};
	double y{0.0};

	[[nodiscard]] static Point Invalid() { return {NAN, NAN}; }

	[[nodiscard]] static Point Max()
	{
		return {std::numeric_limits<double>::max(),
		    std::numeric_limits<double>::max()};
	}

	[[nodiscard]] static Point Min()
	{
		return {std::numeric_limits<double>::lowest(),
		    std::numeric_limits<double>::lowest()};
	}

	[[nodiscard]] static Point Ident(bool horizontal)
	{
		return {horizontal ? 1.0 : 0.0, horizontal ? 0.0 : 1.0};
	}

	[[nodiscard]] static Point Polar(double radius, double angle)
	{
		return {radius * cos(angle), radius * sin(angle)};
	}

	[[nodiscard]] static Point X(double x) { return {x, 0}; }

	[[nodiscard]] static Point Y(double y) { return {0, y}; }

	[[nodiscard]] Point operator*(double factor) const
	{
		return {x * factor, y * factor};
	}

	[[nodiscard]] Point operator/(double divisor) const
	{
		if (Math::Floating::is_zero(divisor)) return Invalid();
		return {x / divisor, y / divisor};
	}

	[[nodiscard]] Point operator+(const Point &other) const
	{
		return {x + other.x, y + other.y};
	}

	[[nodiscard]] Point operator-(const Point &other) const
	{
		return {x - other.x, y - other.y};
	}

	[[nodiscard]] Point operator*(const Point &other) const
	{
		return {x * other.x, y * other.y};
	}

	[[nodiscard]] double dot(const Point &other) const
	{
		return x * other.x + y * other.y;
	}

	[[nodiscard]] Point operator/(const Point &other) const
	{
		using Math::Floating::is_zero;
		if (is_zero(other.x) || is_zero(other.y)) return Invalid();
		return {x / other.x, y / other.y};
	}

	[[nodiscard]] double operator^(const Point &p) const
	{
		return x * p.y - y * p.x;
	}

	Point &operator+=(const Point &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Point &operator-=(const Point &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Point &operator*=(double factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

	Point &operator/=(double divisor)
	{
		if (Math::Floating::is_zero(divisor))
			return *this = Invalid();
		x /= divisor;
		y /= divisor;
		return *this;
	}

	Point &operator*=(const Point &other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Point &operator/=(const Point &other)
	{
		using Math::Floating::is_zero;
		if (is_zero(other.x) || is_zero(other.y))
			return *this = Invalid();
		x /= other.x;
		y /= other.y;
		return *this;
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

	double &operator[](size_t index)
	{
		if (index == 0) return x;
		if (index == 1) return y;
		throw std::logic_error(
		    "internal error: point coordinate index out of bounds");
	}

	[[nodiscard]] double abs() const
	{
		using Math::Floating::is_zero;
		if (is_zero(x)) return fabs(y);
		if (is_zero(y)) return fabs(x);
		return sqrt(x * x + y * y);
	}

	[[nodiscard]] double manhattan() const
	{
		return ::fabs(x) + ::fabs(y);
	}

	[[nodiscard]] double chebyshev() const
	{
		return std::max(::fabs(x), ::fabs(y), Math::Floating::less);
	}

	[[nodiscard]] double sqrAbs() const { return x * x + y * y; }

	[[nodiscard]] double angle() const
	{
		using Math::Floating::is_zero;
		if (is_zero(y)) return std::signbit(x) ? M_PI : 0.0;
		if (is_zero(x))
			return std::signbit(y) ? -M_PI / 2.0 : M_PI / 2.0;
		return atan2f(static_cast<float>(y), static_cast<float>(x));
	}

	[[nodiscard]] Point toPolar() const { return {abs(), angle()}; }

	bool operator==(const Point &other) const
	{
		return x == other.x && y == other.y;
	}

	[[nodiscard]] bool isNull() const
	{
		using Math::Floating::is_zero;
		return is_zero(x) && is_zero(y);
	}

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
};

struct Size : Point
{

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

	[[nodiscard]] double area() const { return x * y; }

	[[nodiscard]] bool bounds(const Size &s) const
	{
		using Math::Floating::less;
		return !less(x, s.x) && !less(y, s.y);
	}

	static Size boundary(const Size &s1, const Size &s2)
	{
		using Math::Floating::less;
		return {std::max(s1.x, s2.x, less),
		    std::max(s1.y, s2.y, less)};
	}

	static Size section(const Size &s1, const Size &s2)
	{
		using Math::Floating::less;
		return {std::min(s1.x, s2.x, less),
		    std::min(s1.y, s2.y, less)};
	}

	[[nodiscard]] bool isSquare(double toleranceFactor) const
	{
		using Math::Floating::is_zero;
		if (is_zero(y)) return false;
		return Math::AddTolerance(fabs(x / y), toleranceFactor) == 1;
	}

	[[nodiscard]] double aspectRatio() const
	{
		using Math::Floating::is_zero;
		return is_zero(y) ? 0 : x / y;
	}
	[[nodiscard]] double minSize() const
	{
		return std::min(x, y, Math::Floating::less);
	}
	[[nodiscard]] double diagonal() const { return abs(); }
	[[nodiscard]] Size rotatedSize(double angle) const;

	[[nodiscard]] Size verticalHalf() const { return {x, y / 2}; }
	[[nodiscard]] Size horizontalHalf() const { return {x / 2, y}; }

	[[nodiscard]] Size positive() const
	{
		return {std::signbit(x) ? 0 : x, std::signbit(y) ? 0 : y};
	}
};

}

#endif
