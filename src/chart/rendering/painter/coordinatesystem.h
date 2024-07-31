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
	    const ::Anim::Interpolated<Gen::CoordSystem> &coordSystem);
	[[nodiscard]] Geom::Point convert(const Geom::Point &p) const;
	[[nodiscard]] double horConvert(double length) const;
	[[nodiscard]] double verConvert(double length) const;
	[[nodiscard]] Geom::Point getOriginal(const Geom::Point &p) const;

	[[nodiscard]] const Math::FuzzyBool &getPolar() const
	{
		return polar;
	}

protected:
	bool zoomOut{};
	Math::FuzzyBool polar;

	[[nodiscard]] Geom::Size mappedSize() const;
};

class CompoundTransform
{
public:
	CompoundTransform() = default;
	explicit CompoundTransform(const Geom::Rect &rect,
	    double angle = 0.0,
	    const ::Anim::Interpolated<Gen::CoordSystem> &coordSystem =
	        ::Anim::Interpolated{Gen::CoordSystem::cartesian},
	    Math::FuzzyBool keepAspectRatio = {});
	[[nodiscard]] Geom::Point convert(const Geom::Point &p) const;
	[[nodiscard]] double horConvert(double length) const;
	[[nodiscard]] double verConvert(double length) const;
	[[nodiscard]] Geom::Line convertDirectionAt(
	    const Geom::Line &vec) const;
	[[nodiscard]] Geom::Point getOriginal(const Geom::Point &p) const;
	[[nodiscard]] Geom::Rect getRect() const;
	[[nodiscard]] double getAngle() const;

private:
	PolarDescartesTransform polarDescartes;
	Geom::Rect rect;
	double angle{};
	Math::FuzzyBool keepAspectRatio;
	double cosAngle{};
	double sinAngle{};

	void setAngle(double value);
	[[nodiscard]] Geom::Point rotate(const Geom::Point &point,
	    bool invert = false,
	    const Geom::Point &center = Geom::Point{0.5, 0.5}) const;
	[[nodiscard]] Geom::Size rotatedSize() const;
	[[nodiscard]] Geom::Size alignedSize() const;
	[[nodiscard]] Geom::Point align(const Geom::Point &point) const;
	[[nodiscard]] Geom::Point deAlign(const Geom::Point &point) const;
};

using CoordinateSystem = CompoundTransform;

}

#endif
