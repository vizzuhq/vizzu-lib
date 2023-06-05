#ifndef GEOM_AFFINETRANSFORM
#define GEOM_AFFINETRANSFORM

#include "point.h"

namespace Geom
{

struct Size;
struct Rect;
class Circle;
struct Line;
class Polygon;

class AffineTransform
{
public:
	double m[2][3];

	AffineTransform();
	AffineTransform(
		double m00, double m01, double m02,
		double m10, double m11, double m12);
	AffineTransform(Geom::Point offset, 
		double scale = 1.0, 
		double angle = 0.0);

	AffineTransform inverse() const;
	bool transforms() const;
	void shift(const Geom::Point &offset);

	AffineTransform operator*(const AffineTransform &other) const;
	bool operator==(const AffineTransform &other) const;

	Geom::Point operator()(const Geom::Point &original) const;
	Geom::Line operator()(const Geom::Line &original) const;
	Geom::Polygon operator()(const Geom::Polygon &original) const;
};

}

#endif
