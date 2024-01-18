#include "rect.h"

#include <algorithm>

namespace Geom
{

Rect Rect::Ident() { return {Geom::Point(), Geom::Size::Identity()}; }

Rect Rect::CenteredMax()
{
	return {Geom::Point::Min() / 2, {Geom::Size::Max()}};
}

Rect Rect::boundary(const Rect &rect) const
{
	Rect res = positive();
	const Rect other = rect.positive();
	res.setLeft(std::min(res.left(), other.left()));
	res.setBottom(std::min(res.bottom(), other.bottom()));
	res.setRight(std::max(res.right(), other.right()));
	res.setTop(std::max(res.top(), other.top()));
	return res;
}

Rect Rect::boundary(const Point &p) const
{
	Rect res = positive();
	res.setLeft(std::min(res.left(), p.x));
	res.setBottom(std::min(res.bottom(), p.y));
	res.setRight(std::max(res.right(), p.x));
	res.setTop(std::max(res.top(), p.y));
	return res;
}

Point Rect::normalize(const Point &p) const
{
	Point res;
	if (size.x != 0) res.x = (p.x - pos.x) / size.x;
	if (size.y != 0) res.y = (p.y - pos.y) / size.y;
	return res;
}

Size Rect::normalize(const Size &s) const
{
	Size res;
	if (size.x != 0) res.x = s.x / size.x;
	if (size.y != 0) res.y = s.y / size.y;
	return res;
}

Rect Rect::normalize(const Rect &rect) const
{
	return {normalize(rect.pos), normalize(rect.size)};
}

Rect Rect::positive() const
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

Rect Rect::operator*(double factor) const
{
	return {pos * factor, {size * factor}};
}

Rect Rect::operator+(const Geom::Rect &other) const
{
	return {pos + other.pos, {size + other.size}};
}

bool Rect::contains(const Point &p) const
{
	return hSize().includes(p.x) && vSize().includes(p.y);
}

Rect Rect::intersection(const Rect &rect) const
{
	auto xLeft = std::max(this->left(), rect.left());
	auto xRight = std::min(this->right(), rect.right());
	auto yBottom = std::max(this->bottom(), rect.bottom());
	auto yTop = std::min(this->top(), rect.top());
	if (xLeft >= xRight || yBottom >= yTop) { return {}; }
	return {Point{xLeft, yBottom},
	    Size{xRight - xLeft, yTop - yBottom}};
}

bool Rect::intersects(const Rect &r) const
{
	auto isOutside = right() < r.left() || r.right() < left()
	              || top() < r.bottom() || r.top() < bottom();
	return !isOutside;
}

Point Rect::center() const { return pos + size / 2.0; }

Rect Rect::popBottom(double length)
{
	Rect res = *this;
	res.size.y = length;
	pos.y += length;
	size.y -= length;
	return res;
}

Rect Rect::popTop(double length)
{
	Rect res = *this;
	res.pos.y = size.y - length;
	res.size.y = length;
	size.y -= length;
	return res;
}

Rect Rect::popLeft(double length)
{
	Rect res = *this;
	res.size.x = length;
	pos.x += length;
	size.x -= length;
	return res;
}

Rect Rect::popRight(double length)
{
	Rect res = *this;
	res.pos.x = size.x - length;
	res.size.x = length;
	size.x -= length;
	return res;
}

std::array<Point, 4> Rect::points() const
{
	return {Point{left(), bottom()},
	    Point{right(), bottom()},
	    Point{right(), top()},
	    Point{left(), top()}};
}

}