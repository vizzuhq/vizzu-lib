#include "rect.h"

#include <algorithm>

using namespace Geom;

Rect Rect::Ident()
{
	return Geom::Rect(Geom::Point(), Geom::Size::Identity());
}

Rect Rect::CenteredMax()
{
	return Geom::Rect(Geom::Point::Min()/2, Geom::Size::Max());
}

Rect::Rect(const Point &pos)
	: pos(pos), size(0,0)
{}

Rect::Rect(const Point &pos, const Point &size)
	: pos(pos), size(size)
{
}

Rect::Rect(double x, double y, double sx, double sy)
	: pos(x, y), size(sx, sy)
{
}

Rect::Rect(const Line &diagonal)
	: pos(diagonal.begin), size(diagonal.getDirection())
{
}

Rect Rect::boundary(const Rect &rect) const
{
	Rect res = positive();
	Rect other = rect.positive();
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
	return Rect(normalize(rect.pos), normalize(rect.size));
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
	return Rect(pos * factor, size * factor);
}

Rect Rect::operator+(const Geom::Rect &other) const
{
	return Rect(pos + other.pos, size + other.size);
}

bool Rect::contains(const Point &p) const
{
	return	p.x >= pos.x && p.x <= pos.x + size.x
			&& p.y >= pos.y && p.y <= pos.y + size.y;
}

Rect Rect::intersection(const Rect &rect) const
{
	double xLeft = std::max(this->left(), rect.left());
	double yBottom = std::max(this->bottom(), rect.bottom());

	double xRight = std::min(this->right(), rect.right());
	double yTop = std::min(this->top(), rect.top());

	if (xLeft >= xRight || yBottom >= yTop) {
		return Rect();
	} else {
		Point pos(xLeft, yBottom);
		Point size(xRight - xLeft, yTop - yBottom);
		return Rect(pos, size);
	}
}

bool Rect::intersects(const Rect &r) const
{
	auto isOutside = right() < r.left() || r.right() < left()
	              || top() < r.bottom() || r.top() < bottom();
	return !isOutside;
}

Point Rect::center() const
{
	return pos + size / 2.0;
}

Rect Rect::popBottom(double length) {
	Rect res = *this;
	res.size.y = length;
	pos.y += length;
	size.y -= length;
	return res;
}

Rect Rect::popTop(double length) {
	Rect res = *this;
	res.pos.y = size.y - length;
	res.size.y = length;
	size.y -= length;
	return res;
}

Rect Rect::popLeft(double length) {
	Rect res = *this;
	res.size.x = length;
	pos.x += length;
	size.x -= length;
	return res;
}

Rect Rect::popRight(double length) {
	Rect res = *this;
	res.pos.x = size.x - length;
	res.size.x = length;
	size.x -= length;
	return res;
}
