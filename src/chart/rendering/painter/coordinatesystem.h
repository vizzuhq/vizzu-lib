#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include "base/geom/rect.h"
#include "base/math/fuzzybool.h"

namespace Vizzu
{
namespace Draw
{

class PolarDescartesTransform
{
public:
	PolarDescartesTransform() = default;
	PolarDescartesTransform(Math::FuzzyBool polar);
	Geom::Point convert(const Geom::Point &p) const;
	double horConvert(double length) const;
	double verConvert(double length) const;
	Geom::Point getOriginal(const Geom::Point &p) const;
	Math::FuzzyBool getPolar() const;
	bool atEndState() const;

protected:
	bool zoomOut;
	Math::FuzzyBool polar;

	Geom::Size mappedSize() const;
};

class CompoundTransform :
	public PolarDescartesTransform
{
public:
	CompoundTransform() = default;
	CompoundTransform(Geom::Rect rect,
	    double angle,
	    Math::FuzzyBool polar,
	    Math::FuzzyBool keepAspectRatio);
	Geom::Point convert(const Geom::Point &p) const;
	double horConvert(double length) const;
	double verConvert(double length) const;
	Geom::Point convertAt(const Geom::Point &pos,
	    const Geom::Point &vector) const;
	Geom::Point getOriginal(const Geom::Point &p) const;
	Geom::Rect getRect() const;
	double getAngle() const;
	Geom::Point justRotate(const Geom::Point &p) const;

private:
	Geom::Rect rect;
	double angle;
	Math::FuzzyBool keepAspectRatio;
	double cosAngle;
	double sinAngle;

	void setAngle(double value);
	Geom::Point rotate(const Geom::Point &point, bool invert = false,
					   const Geom::Point &center = Geom::Point(0.5, 0.5)) const;
	Geom::Size rotatedSize() const;
	Geom::Size alignedSize() const;
	Geom::Point align(const Geom::Point &point) const;
	Geom::Point deAlign(const Geom::Point &point) const;
};

typedef CompoundTransform CoordinateSystem;

}
}

#endif
