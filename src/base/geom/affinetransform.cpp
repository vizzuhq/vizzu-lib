#include "affinetransform.h"

#include "base/conv/auto_json.h"

#include "circle.h"
#include "line.h"
#include "polygon.h"
#include "rect.h"

Geom::AffineTransform::AffineTransform() :
    m{Row{1.0, 0.0, 0.0}, Row{0.0, 1.0, 0.0}}
{}

Geom::AffineTransform::AffineTransform(double m00,
    double m01,
    double m02,
    double m10,
    double m11,
    double m12) :
    m{Row{m00, m01, m02}, Row{m10, m11, m12}}
{}

Geom::AffineTransform::AffineTransform(Point offset,
    double scale,
    double angle) :
    m{Row{cos(angle) * scale, sin(angle) * scale, offset.x},
        Row{-sin(angle) * scale, cos(angle) * scale, offset.y}}
{}

Geom::AffineTransform Geom::AffineTransform::inverse() const
{
	auto det = m[0][0] * m[1][1] - m[1][0] * m[0][1];

	if (det == 0.0)
		throw std::logic_error(
		    "attempted inversion of singular matrix");

	return {m[1][1] / det,
	    -m[0][1] / det,
	    (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det,
	    -m[1][0] / det,
	    m[0][0] / det,
	    (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / det};
}

bool Geom::AffineTransform::transforms() const
{
	return *this != AffineTransform();
}

Geom::AffineTransform Geom::AffineTransform::operator*(
    const AffineTransform &other) const
{
	const auto &[o0, o1] = other.m;
	return AffineTransform{
	    Matrix{Row{m[0][0] * o0[0] + m[0][1] * o1[0],
	               m[0][0] * o0[1] + m[0][1] * o1[1],
	               m[0][0] * o0[2] + m[0][1] * o1[2] + m[0][2]},
	        Row{m[1][0] * o0[0] + m[1][1] * o1[0],
	            m[1][0] * o0[1] + m[1][1] * o1[1],
	            m[1][0] * o0[2] + m[1][1] * o1[2] + m[1][2]}}};
}

Geom::Point Geom::AffineTransform::operator()(
    const Geom::Point &original) const
{
	return {original.x * m[0][0] + original.y * m[0][1] + m[0][2],
	    original.x * m[1][0] + original.y * m[1][1] + m[1][2]};
}

Geom::Line Geom::AffineTransform::operator()(
    const Geom::Line &original) const
{
	return {(*this)(original.begin), (*this)(original.end)};
}

Geom::Polygon Geom::AffineTransform::operator()(
    const Geom::Polygon &original) const
{
	Geom::Polygon res;
	for (auto point : original.points) res.add((*this)(point));
	return res;
}

void Geom::AffineTransform::shift(const Geom::Point &offset)
{
	m[0][2] += offset.x;
	m[1][2] += offset.y;
}

std::string Geom::AffineTransform::toJSON() const
{
	return Conv::toJSON(m);
}
