#include "affinetransform.h"

#include "circle.h"
#include "line.h"
#include "polygon.h"
#include "rect.h"

using namespace Geom;

AffineTransform::AffineTransform()
	: offset(0,0), scale(1.0), rotate(0.0)
{}

AffineTransform::AffineTransform(Point offset, double scale) :
    offset(offset),
    scale(scale),
    rotate(0.0)
{}

AffineTransform::AffineTransform(Rect from, Rect to)
{
	rotate = 0.0;
	scale = std::max(to.size.x / from.size.x, to.size.y / from.size.y);
	offset = to.pos - from.pos * scale;
	to.pos = from.pos * scale + offset;
	to.size = from.size * scale;
}

AffineTransform AffineTransform::inverse() const
{
	return AffineTransform(offset * (-1 / scale), 1.0 / scale);
}

bool AffineTransform::transforms() const
{
	return !(scale == 1.0 && offset.isNull());
}

AffineTransform &AffineTransform::operator+=(const Point &offset)
{
	this->offset = this->offset + offset;
	return *this;
}

AffineTransform &AffineTransform::operator*=(double scale)
{
	this->scale *= scale;
	return *this;
}

AffineTransform &AffineTransform::operator*=(const AffineTransform &other)
{
	offset = other(offset);
	scale = other(scale);
	return *this;
}

AffineTransform AffineTransform::operator+(const Point &offset) const
{
	return AffineTransform(this->offset + offset, scale);
}

AffineTransform AffineTransform::operator*(double scale) const
{
	return AffineTransform(offset, this->scale * scale);
}

AffineTransform AffineTransform::operator*(const AffineTransform &other) const
{
	return AffineTransform(other(offset), other(scale));
}

bool AffineTransform::operator==(const AffineTransform &other) const
{
	return scale == other.scale && offset == other.offset;
}

double AffineTransform::operator()(double original) const
{
	return original * scale;
}

Geom::Point AffineTransform::operator()(const Geom::Point &original) const
{
	return original * scale + offset;
}

Geom::Size AffineTransform::operator()(const Geom::Size &original) const
{
	return original * scale;
}

Geom::Rect AffineTransform::operator()(const Geom::Rect &original) const
{
	return Geom::Rect((*this)(original.pos), (*this)(original.size));
}

Geom::Line AffineTransform::operator()(const Geom::Line &original) const
{
	return Geom::Line((*this)(original.begin), (*this)(original.end));
}

Geom::Circle AffineTransform::operator()(const Geom::Circle &original) const
{
	return Geom::Circle((*this)(original.center), (*this)(original.radius));
}

Geom::Polygon AffineTransform::operator()(const Geom::Polygon &original) const
{
	Geom::Polygon res;
	for (auto point : original.points)
		res.add((*this)(point));
	return res;
}
