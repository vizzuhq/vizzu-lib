#include "rect.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <compare>

#include "base/math/floating.h"

#include "point.h"

namespace Geom
{

Rect Rect::Ident() { return {Point(), Size::Identity()}; }

Rect Rect::boundary(const Rect &rect) const
{
	using Math::Floating::less;
	Rect res = positive();
	const Rect other = rect.positive();
	res.setLeft(std::min(res.left(), other.left(), less));
	res.setBottom(std::min(res.bottom(), other.bottom(), less));
	res.setRight(std::max(res.right(), other.right(), less));
	res.setTop(std::max(res.top(), other.top(), less));
	return res;
}

Point Rect::normalize(const Point &p) const
{
	using Math::Floating::is_zero;
	Point res;
	if (!is_zero(size.x)) res.x = (p.x - pos.x) / size.x;
	if (!is_zero(size.y)) res.y = (p.y - pos.y) / size.y;
	return res;
}

Size Rect::normalize(const Size &s) const
{
	using Math::Floating::is_zero;
	Size res;
	if (!is_zero(size.x)) res.x = s.x / size.x;
	if (!is_zero(size.y)) res.y = s.y / size.y;
	return res;
}

Rect Rect::normalize(const Rect &rect) const
{
	return {normalize(rect.pos), normalize(rect.size)};
}

Rect Rect::positive() const
{
	Rect res = *this;
	if (std::signbit(res.size.x)) {
		res.size.x = -res.size.x;
		res.pos.x -= res.size.x;
	}
	if (std::signbit(res.size.y)) {
		res.size.y = -res.size.y;
		res.pos.y -= res.size.y;
	}
	return res;
}

Rect Rect::operator*(double factor) const
{
	return {pos * factor, {size * factor}};
}

Rect Rect::operator+(const Rect &other) const
{
	return {pos + other.pos, {size + other.size}};
}

bool Rect::contains(const Point &p) const
{
	return hSize().includes(p.x) && vSize().includes(p.y);
}

Rect Rect::intersection(const Rect &rect) const
{
	using Math::Floating::less;
	auto xLeft = std::max(this->left(), rect.left(), less);
	auto xRight = std::min(this->right(), rect.right(), less);
	auto yBottom = std::max(this->bottom(), rect.bottom(), less);
	auto yTop = std::min(this->top(), rect.top(), less);

	if (!less(xLeft, xRight) || !less(yBottom, yTop)) { return {}; }
	return {Point{xLeft, yBottom},
	    Size{xRight - xLeft, yTop - yBottom}};
}

bool Rect::intersects(const Rect &r) const
{
	using Math::Floating::is_zero;
	using std::strong_order;
	auto first = strong_order(right(), r.left());
	auto second = strong_order(r.right(), left());
	auto third = strong_order(top(), r.bottom());
	auto fourth = strong_order(r.top(), bottom());

	auto isOutside = is_lt(first) || is_lt(second) || is_lt(third)
	              || is_lt(fourth)
	              || ((is_eq(first) || is_eq(second))
	                  && !is_zero(width()) && !is_zero(r.width()))
	              || ((is_eq(third) || is_eq(fourth))
	                  && !is_zero(height()) && !is_zero(r.height()));
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