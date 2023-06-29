#ifndef GEOM_RECT
#define GEOM_RECT

#include <array>
#include <vector>

#include "base/math/range.h"

#include "line.h"
#include "point.h"

namespace Geom
{

struct Rect
{
	Point pos;
	Size size;

	template <class Container>
	static Rect Boundary(const Container &points);

	static Rect Ident();
	static Rect CenteredMax();

	Rect() {}
	explicit Rect(const Point &pos);
	Rect(const Point &pos, const Point &size);
	Rect(const Line &diagonal);
	Rect(double, double, double, double);
	Rect boundary(const Rect &rect) const;
	Rect boundary(const Point &p) const;
	Point normalize(const Point &p) const;
	Size normalize(const Size &s) const;
	Rect normalize(const Rect &rect) const;
	Rect positive() const;

	bool operator==(const Rect &other) const
	{
		return pos == other.pos && size == other.size;
	}

	Rect operator*(double factor) const;
	Rect operator+(const Geom::Rect &other) const;

	double left() const { return pos.x; }
	double right() const { return pos.x + size.x; }
	double top() const { return pos.y + size.y; }
	double bottom() const { return pos.y; }

	double width() const { return size.x; }
	double height() const { return size.y; }

	Math::Range<double> hSize() const
	{
		return Math::Range<double>(left(), right());
	}
	Math::Range<double> vSize() const
	{
		return Math::Range<double>(bottom(), top());
	}

	Math::Range<double> x() const
	{
		return Math::Range<double>(pos.x, pos.x + size.x);
	}
	Math::Range<double> y() const
	{
		return Math::Range<double>(pos.y, pos.y + size.y);
	}

	Point topLeft() const { return Point(left(), top()); }
	Point topRight() const { return Point(right(), top()); }
	Point bottomLeft() const { return Point(left(), bottom()); }
	Point bottomRight() const { return Point(right(), bottom()); }

	void setLeft(double val)
	{
		size.x -= val - pos.x;
		pos.x = val;
	}
	void setRight(double val) { size.x = val - pos.x; }
	void setTop(double val) { size.y = val - pos.y; }
	void setBottom(double val)
	{
		size.y -= val - pos.y;
		pos.y = val;
	}

	void setWidth(double val) { size.x = val; }
	void setHeight(double val) { size.y = val; }

	void setHSize(const Math::Range<double> &range)
	{
		setLeft(range.getMin());
		setRight(range.getMax());
	}

	void setVSize(const Math::Range<double> &range)
	{
		setBottom(range.getMin());
		setTop(range.getMax());
	}

	Rect bottomHalf() const { return Rect(pos, size.verticalHalf()); }
	Rect topHalf() const
	{
		return Rect(Point(left(), center().y), size.verticalHalf());
	}
	Rect leftHalf() const { return Rect(pos, size.horizontalHalf()); }
	Rect rightHalf() const
	{
		return Rect(Point(center().x, bottom()),
		    size.horizontalHalf());
	}

	Line leftSide() const { return Line(topLeft(), bottomLeft()); }
	Line rightSide() const { return Line(topRight(), bottomRight()); }
	Line topSide() const { return Line(topLeft(), topRight()); }
	Line bottomSide() const
	{
		return Line(bottomLeft(), bottomRight());
	}

	Rect intersection(const Rect &rect) const;

	bool contains(const Point &p) const;
	bool intersects(const Rect &r) const;
	Point center() const;

	explicit operator std::string() const
	{
		return "{"
		       "\"pos\":"
		     + static_cast<std::string>(pos)
		     + ","
		       "\"size\":"
		     + static_cast<std::string>(size) + "}";
	}

	Rect outline(const Geom::Size &margin) const
	{
		return Rect(pos - margin, size + margin * 2);
	}

	Rect popBottom(double length);
	Rect popTop(double length);
	Rect popLeft(double length);
	Rect popRight(double length);
};

template <class Container>
Rect Rect::Boundary(const Container &points)
{
	Rect boundary(points[0]);
	for (auto &point : points) boundary = boundary.boundary(point);
	return boundary;
}

}

#endif
