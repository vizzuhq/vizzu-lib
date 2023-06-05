#include "affinetransform.h"

#include "circle.h"
#include "line.h"
#include "polygon.h"
#include "rect.h"

using namespace Geom;

AffineTransform::AffineTransform() :
	m{
		{ 1,0,0 },
		{ 0,1,0 }
	}
{}

AffineTransform::AffineTransform(
	double m00, double m01, double m02,
	double m10, double m11, double m12) :
	m {
		{ m00, m01, m02 },
		{ m10, m11, m12 }
	}
{}

AffineTransform::AffineTransform(Point offset, double scale, double angle) :
	m {
		{ cos(angle) * scale, sin(angle) * scale, offset.x },
		{ - sin(angle) * scale, cos(angle) * scale, offset.y }
	}
{}

AffineTransform AffineTransform::inverse() const
{
	auto det = m[0][0]*m[1][1] - m[1][0]*m[0][1];
	return AffineTransform(
		m[1][1] / det, 
		-m[0][1] / det, 
		(m[0][1]*m[1][2] - m[0][2]*m[1][1]) / det,
		-m[1][0] / det, 
		m[0][0] / det,
		(m[0][2]*m[1][0] - m[0][0]*m[1][2]) / det);
}

bool AffineTransform::operator==(const AffineTransform &other) const
{
	return m[0][0] == other.m[0][0]
		&& m[0][1] == other.m[0][1]
		&& m[0][2] == other.m[0][2]
		&& m[1][0] == other.m[1][0]
		&& m[1][1] == other.m[1][1]
		&& m[1][2] == other.m[1][2];
}

bool AffineTransform::transforms() const
{
	return *this != AffineTransform();
}

AffineTransform AffineTransform::operator*(const AffineTransform &other) const
{
	AffineTransform res;
	const auto &o = other;
	res.m[0][0] = m[0][0]*o.m[0][0] + m[0][1]*o.m[1][0];
	res.m[0][1] = m[0][0]*o.m[0][1] + m[0][1]*o.m[1][1];
	res.m[0][2] = m[0][0]*o.m[0][2] + m[0][1]*o.m[1][2] + m[0][2];
	res.m[1][0] = m[1][0]*o.m[0][0] + m[1][1]*o.m[1][0];
	res.m[1][1] = m[1][0]*o.m[0][1] + m[1][1]*o.m[1][1];
	res.m[1][2] = m[1][0]*o.m[0][2] + m[1][1]*o.m[1][2] + m[1][2];
	return res;
}

Geom::Point AffineTransform::operator()(const Geom::Point &p) const
{
	return Geom::Point(
		p.x * m[0][0] + p.y * m[0][1] + m[0][2],
		p.x * m[1][0] + p.y * m[1][1] + m[1][2]
	);
}

Geom::Line AffineTransform::operator()(const Geom::Line &original) const
{
	return Geom::Line((*this)(original.begin), (*this)(original.end));
}

Geom::Polygon AffineTransform::operator()(const Geom::Polygon &original) const
{
	Geom::Polygon res;
	for (auto point : original.points)
		res.add((*this)(point));
	return res;
}

void AffineTransform::shift(const Geom::Point &offset)
{
	m[0][2] += offset.x;
	m[1][2] += offset.y;
}