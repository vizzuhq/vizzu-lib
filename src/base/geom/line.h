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
	Line(const Point &begin, const Point &end)
		: begin(begin), end(end)
	{}

	Point center() const {
		return (begin + end) / 2.0;
	}

	Point getDirection() const {
		return end - begin;
	}

	bool isPoint() const {
		return begin == end;
	}

	Geom::Line extend(double length) const {
		auto p = end - begin;
		auto m = length / sqrt(p.x * p.x + p.y * p.y);
		return Line(begin, begin + (p * m));
	}

	Geom::Line operator*(double value) const {
		return Geom::Line(begin * value, end * value);
	}

	Geom::Line operator+(const Geom::Line &other) const {
		return Geom::Line(begin + other.begin, end + other.end);
	}

	explicit operator std::string() const {
		return "{"
			"\"begin\":" + (std::string)begin + ","
			"\"end\":" + (std::string)end + 
		"}";
	}
};

}

#endif
