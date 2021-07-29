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
	Geom::Point offset;
	double scale;
	double rotate;

	AffineTransform();
	AffineTransform(Geom::Point offset, 
		double scale = 1.0, 
		double rotate = 0.0);
	AffineTransform(Geom::Rect from, Geom::Rect to);

	AffineTransform inverse() const;
	bool transforms() const;

	AffineTransform &operator+=(const Geom::Point &offset);
	AffineTransform &operator*=(double scale);
	AffineTransform &operator*=(const AffineTransform &other);
	AffineTransform operator+(const Geom::Point &offset) const;
	AffineTransform operator*(double scale) const;
	AffineTransform operator*(const AffineTransform &other) const;
	bool operator==(const AffineTransform &other) const;

	double operator()(double original) const;
	Geom::Point operator()(const Geom::Point &original) const;
	Geom::Size operator()(const Geom::Size &original) const;
	Geom::Rect operator()(const Geom::Rect &original) const;
	Geom::Line operator()(const Geom::Line &original) const;
	Geom::Circle operator()(const Geom::Circle &original) const;
	Geom::Polygon operator()(const Geom::Polygon &original) const;
};

}

#endif
