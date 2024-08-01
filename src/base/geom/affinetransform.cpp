#include "affinetransform.h"

#include <cmath>
#include <stdexcept>
#include <string>

#include "base/conv/auto_json.h"
#include "base/math/floating.h"

#include "line.h"
#include "point.h"
#include "polygon.h"

namespace Geom
{

AffineTransform::AffineTransform() :
    m{Row{1.0, 0.0, 0.0}, Row{0.0, 1.0, 0.0}}
{}

AffineTransform::AffineTransform(double m00,
    double m01,
    double m02,
    double m10,
    double m11,
    double m12) :
    m{Row{m00, m01, m02}, Row{m10, m11, m12}}
{}

AffineTransform::AffineTransform(Point offset,
    double scale,
    double angle) :
    m{Row{cos(angle) * scale, sin(angle) * scale, offset.x},
        Row{-sin(angle) * scale, cos(angle) * scale, offset.y}}
{}

AffineTransform AffineTransform::inverse() const
{
	auto det = m[0][0] * m[1][1] - m[1][0] * m[0][1];

	if (Math::Floating::is_zero(det))
		throw std::logic_error(
		    "attempted inversion of singular matrix");

	return {m[1][1] / det,
	    -m[0][1] / det,
	    (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det,
	    -m[1][0] / det,
	    m[0][0] / det,
	    (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / det};
}

bool AffineTransform::transforms() const
{
	return *this != AffineTransform();
}

AffineTransform &AffineTransform::operator*=(
    const AffineTransform &other)
{
	const auto &[o0, o1] = other.m;

	m = {Row{m[0][0] * o0[0] + m[0][1] * o1[0],
	         m[0][0] * o0[1] + m[0][1] * o1[1],
	         m[0][0] * o0[2] + m[0][1] * o1[2] + m[0][2]},
	    Row{m[1][0] * o0[0] + m[1][1] * o1[0],
	        m[1][0] * o0[1] + m[1][1] * o1[1],
	        m[1][0] * o0[2] + m[1][1] * o1[2] + m[1][2]}};

	return *this;
}

Geom::Point AffineTransform::operator()(
    const Geom::Point &original) const
{
	return {original.x * m[0][0] + original.y * m[0][1] + m[0][2],
	    original.x * m[1][0] + original.y * m[1][1] + m[1][2]};
}

Geom::Line AffineTransform::operator()(
    const Geom::Line &original) const
{
	return {(*this)(original.begin), (*this)(original.end)};
}

Geom::Polygon AffineTransform::operator()(
    const Geom::Polygon &original) const
{
	Geom::Polygon res;
	for (auto point : original.points) res.add((*this)(point));
	return res;
}

void AffineTransform::shift(const Geom::Point &offset)
{
	m[0][2] += offset.x;
	m[1][2] += offset.y;
}

std::string AffineTransform::toJSON() const
{
	return Conv::toJSON(m);
}

}