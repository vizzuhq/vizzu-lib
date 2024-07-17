#ifndef GEOM_LINE
#define GEOM_LINE

#include <tuple>

#include "point.h"

namespace Geom
{

struct Line
{
	Point begin;
	Point end;

	[[nodiscard]] Point center() const { return (begin + end) / 2.0; }

	[[nodiscard]] Point getDirection() const { return end - begin; }

	[[nodiscard]] bool isPoint() const { return begin == end; }

	[[nodiscard]] Geom::Line extend(double length) const
	{
		auto p = end - begin;
		auto d = p.abs();
		if (d == 0) return *this;
		auto m = length / d;
		return {begin, begin + (p * m)};
	}

	Geom::Line operator*(double value) const
	{
		return {begin * value, end * value};
	}

	Geom::Line operator+(const Geom::Line &other) const
	{
		return {begin + other.begin, end + other.end};
	}

	void shift(const Geom::Point &offset)
	{
		begin = begin + offset;
		end = end + offset;
	}

	[[nodiscard]] Geom::Point at(double t) const
	{
		return begin + (end - begin) * t;
	}

	[[nodiscard]] Geom::Line segment(double t0, double t1) const
	{
		return {at(t0), at(t1)};
	}

	[[nodiscard]] double length() const
	{
		return (end - begin).abs();
	}

	[[nodiscard]] double distance(const Point &point) const
	{
		auto projection = ((point - begin).dot(getDirection()))
		                / (length() * length());

		projection =
		    std::clamp(projection, 0.0, 1.0, Math::Floating::less);

		auto nearestPoint = at(projection);

		return (nearestPoint - point).abs();
	}
};

}

#endif
