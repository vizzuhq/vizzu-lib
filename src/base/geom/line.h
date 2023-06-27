#ifndef GEOM_LINE
#define GEOM_LINE

#include "point.h"

namespace Geom
{

struct Line
{
	Point begin;
	Point end;

	Line() {}
	Line(const Point &begin, const Point &end) :
	    begin(begin),
	    end(end)
	{}

	Point center() const { return (begin + end) / 2.0; }

	Point getDirection() const { return end - begin; }

	bool isPoint() const { return begin == end; }

	Geom::Line extend(double length) const
	{
		auto p = end - begin;
		auto d = p.abs();
		if (d == 0) return *this;
		auto m = length / d;
		return Line(begin, begin + (p * m));
	}

	Geom::Line operator*(double value) const
	{
		return Geom::Line(begin * value, end * value);
	}

	Geom::Line operator+(const Geom::Line &other) const
	{
		return Geom::Line(begin + other.begin, end + other.end);
	}

	void shift(const Geom::Point &offset)
	{
		begin = begin + offset;
		end = end + offset;
	}

	Geom::Point at(double t) const
	{
		return begin + (end - begin) * t;
	}

	Geom::Line segment(double t0, double t1) const
	{
		return Geom::Line(at(t0), at(t1));
	}

	explicit operator std::string() const
	{
		return "{"
		       "\"begin\":"
		     + static_cast<std::string>(begin)
		     + ","
		       "\"end\":"
		     + static_cast<std::string>(end) + "}";
	}
};

}

#endif
