#include "rect.h"

#include <algorithm>

Geom::Rect Geom::Rect::Ident()
{
	return {Geom::Point(), Geom::Size::Identity()};
}

Geom::Rect Geom::Rect::CenteredMax()
{
	return {Geom::Point::Min() / 2, Geom::Size::Max()};
}

Geom::Rect::Rect(const Point &pos) : pos(pos), size{0, 0} {}

Geom::Rect::Rect(const Point &pos, const Point &size) :
    pos(pos),
    size(size)
{}

Geom::Rect::Rect(double x, double y, double sx, double sy) :
    pos(x, y),
    size{sx, sy}
{}

Geom::Rect::Rect(const Line &diagonal) :
    pos(diagonal.begin),
    size(diagonal.getDirection())
{}

Geom::Rect Geom::Rect::boundary(const Rect &rect) const
{
	Rect res = positive();
	const Rect other = rect.positive();
	res.setLeft(std::min(res.left(), other.left()));
	res.setBottom(std::min(res.bottom(), other.bottom()));
	res.setRight(std::max(res.right(), other.right()));
	res.setTop(std::max(res.top(), other.top()));
	return res;
}

Geom::Rect Geom::Rect::boundary(const Point &p) const
{
	Rect res = positive();
	res.setLeft(std::min(res.left(), p.x));
	res.setBottom(std::min(res.bottom(), p.y));
	res.setRight(std::max(res.right(), p.x));
	res.setTop(std::max(res.top(), p.y));
	return res;
}

Geom::Point Geom::Rect::normalize(const Point &p) const
{
	Point res;
	if (size.x != 0) res.x = (p.x - pos.x) / size.x;
	if (size.y != 0) res.y = (p.y - pos.y) / size.y;
	return res;
}

Geom::Size Geom::Rect::normalize(const Size &s) const
{
	Size res;
	if (size.x != 0) res.x = s.x / size.x;
	if (size.y != 0) res.y = s.y / size.y;
	return res;
}

Geom::Rect Geom::Rect::normalize(const Rect &rect) const
{
	return {normalize(rect.pos), normalize(rect.size)};
}

Geom::Rect Geom::Rect::positive() const
{
	Rect res = *this;
	if (res.size.x < 0) {
		res.size.x = -res.size.x;
		res.pos.x -= res.size.x;
	}
	if (res.size.y < 0) {
		res.size.y = -res.size.y;
		res.pos.y -= res.size.y;
	}
	return res;
}

Geom::Rect Geom::Rect::operator*(double factor) const
{
	return {pos * factor, size * factor};
}

Geom::Rect Geom::Rect::operator+(const Geom::Rect &other) const
{
	return {pos + other.pos, size + other.size};
}

bool Geom::Rect::contains(const Point &p) const
{
	return hSize().includes(p.x) && vSize().includes(p.y);
}

Geom::Rect Geom::Rect::intersection(const Rect &rect) const
{
	auto xLeft = std::max(this->left(), rect.left());
	auto xRight = std::min(this->right(), rect.right());
	auto yBottom = std::max(this->bottom(), rect.bottom());
	auto yTop = std::min(this->top(), rect.top());
	if (xLeft >= xRight || yBottom >= yTop) { return {}; }
	return {Point{xLeft, yBottom},
	    Point{xRight - xLeft, yTop - yBottom}};
}

bool Geom::Rect::intersects(const Rect &r) const
{
	auto isOutside = right() < r.left() || r.right() < left()
	              || top() < r.bottom() || r.top() < bottom();
	return !isOutside;
}

Geom::Point Geom::Rect::center() const { return pos + size / 2.0; }

Geom::Rect Geom::Rect::popBottom(double length)
{
	Rect res = *this;
	res.size.y = length;
	pos.y += length;
	size.y -= length;
	return res;
}

Geom::Rect Geom::Rect::popTop(double length)
{
	Rect res = *this;
	res.pos.y = size.y - length;
	res.size.y = length;
	size.y -= length;
	return res;
}

Geom::Rect Geom::Rect::popLeft(double length)
{
	Rect res = *this;
	res.size.x = length;
	pos.x += length;
	size.x -= length;
	return res;
}

Geom::Rect Geom::Rect::popRight(double length)
{
	Rect res = *this;
	res.pos.x = size.x - length;
	res.size.x = length;
	size.x -= length;
	return res;
}

std::array<Geom::Point, 4> Geom::Rect::points() const
{
	return {Point{left(), bottom()},
	    Point{right(), bottom()},
	    Point{right(), top()},
	    Point{left(), top()}};
}
