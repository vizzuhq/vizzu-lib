#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include "base/anim/interpolated.h"
#include "base/geom/rect.h"
#include "base/math/fuzzybool.h"
#include "chart/options/coordsystem.h"

namespace Vizzu::Draw
{

class PolarDescartesTransform
{
public:
	PolarDescartesTransform() = default;
	explicit PolarDescartesTransform(
	    ::Anim::Interpolated<Gen::CoordSystem> coordSystem);
	[[nodiscard]] Geom::Point convert(const Geom::Point &p) const;
	[[nodiscard]] double horConvert(double length) const;
	[[nodiscard]] double verConvert(double length) const;
	[[nodiscard]] Geom::Point getOriginal(const Geom::Point &p) const;
	[[nodiscard]] Math::FuzzyBool getPolar() const;
	[[nodiscard]] bool atEndState() const;

protected:
	bool zoomOut{};
	Math::FuzzyBool polar;

	[[nodiscard]] Geom::Size mappedSize() const;
};

class CompoundTransform : public PolarDescartesTransform
{
public:
	CompoundTransform() = default;
	CompoundTransform(Geom::Rect rect,
	    double angle,
	    ::Anim::Interpolated<Gen::CoordSystem> coordSystem,
	    Math::FuzzyBool keepAspectRatio);
	[[nodiscard]] Geom::Point convert(const Geom::Point &p) const;
	[[nodiscard]] double horConvert(double length) const;
	[[nodiscard]] double verConvert(double length) const;
	[[nodiscard]] Geom::Line convertDirectionAt(const Geom::Line &vec) const;
	[[nodiscard]] Geom::Point getOriginal(const Geom::Point &p) const;
	[[nodiscard]] Geom::Rect getRect() const;
	[[nodiscard]] double getAngle() const;
	[[nodiscard]] Geom::Point justRotate(const Geom::Point &p) const;

private:
	Geom::Rect rect;
	double angle{};
	Math::FuzzyBool keepAspectRatio;
	double cosAngle{};
	double sinAngle{};

	void setAngle(double value);
	[[nodiscard]] Geom::Point rotate(const Geom::Point &point,
	    bool invert = false,
	    const Geom::Point &center = Geom::Point(0.5, 0.5)) const;
	[[nodiscard]] Geom::Size rotatedSize() const;
	[[nodiscard]] Geom::Size alignedSize() const;
	[[nodiscard]] Geom::Point align(const Geom::Point &point) const;
	[[nodiscard]] Geom::Point deAlign(const Geom::Point &point) const;
};

using CoordinateSystem = CompoundTransform;

}

#endif
