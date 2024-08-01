#include "coordinatesystem.h"

#include <algorithm>
#include <cmath>
#include <numbers>

#include "base/anim/interpolated.h"
#include "base/geom/line.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/math/floating.h"
#include "base/math/interpolation.h"
#include "chart/options/coordsystem.h"

namespace Vizzu::Draw
{

PolarDescartesTransform::PolarDescartesTransform(
    const Anim::Interpolated<Gen::CoordSystem> &coordSystem) :
    zoomOut{true},
    polar(coordSystem.factor<double>(Gen::CoordSystem::polar))
{}

Geom::Point PolarDescartesTransform::convert(
    const Geom::Point &p) const
{
	if (polar == false) return p;

	auto mapped = mappedSize();
	auto usedAngle = Math::interpolate(0.0,
	    2.0 * std::numbers::pi,
	    static_cast<double>(polar));
	auto hEquidist = mapped.area() / std::numbers::pi;
	auto yCircTop = 1.0 - mapped.y;
	auto radius = mapped.x / usedAngle - hEquidist;
	const Geom::Point center(0.5, yCircTop - radius);

	auto angle = std::numbers::pi / 2.0 + (0.5 - p.x) * usedAngle;

	auto converted =
	    center + Geom::Point::Polar(radius + p.y * mapped.y, angle);

	if (zoomOut) {
		auto zoomFactor = static_cast<double>(polar) - 0.5;
		zoomFactor = 0.75 + zoomFactor * zoomFactor;
		return Geom::Point{.5, .5}
		     + (converted - Geom::Point{.5, .5}) * zoomFactor;
	}
	return converted;
}

double PolarDescartesTransform::horConvert(double length) const
{
	return mappedSize().x * length;
}

double PolarDescartesTransform::verConvert(double length) const
{
	return mappedSize().y * length;
}

Geom::Point PolarDescartesTransform::getOriginal(
    const Geom::Point &p) const
{
	if (polar == false) return p;
	if (polar == true) {
		const Geom::Point center(0.5, 0.5);
		auto polar = (p - center).toPolar();

		polar.y = -polar.y + 3 * std::numbers::pi / 2;
		if (polar.y < 0) polar.y += 2 * std::numbers::pi;
		if (polar.y > 2 * std::numbers::pi)
			polar.y -= 2 * std::numbers::pi;

		return {polar.y / (2 * std::numbers::pi), 2 * polar.x};
	}

	Geom::Point pZoomed = p;
	if (zoomOut) {
		auto zoomFactor = static_cast<double>(polar) - 0.5;
		zoomFactor = 0.75 + zoomFactor * zoomFactor;
		pZoomed = Geom::Point{.5, .5}
		        + (p - Geom::Point{.5, .5}) / zoomFactor;
	}
	auto mapped = mappedSize();
	auto usedAngle = Math::interpolate(0.0,
	    2.0 * std::numbers::pi,
	    static_cast<double>(polar));
	auto hEquidist = mapped.area() / std::numbers::pi;
	auto yCircTop = 1.0 - mapped.y;
	auto radius = mapped.x / usedAngle - hEquidist;
	const Geom::Point center(0.5, yCircTop - radius);

	auto polar = (pZoomed - center).toPolar();
	polar.y = polar.y - std::numbers::pi / 2.0;
	while (polar.y < std::numbers::pi)
		polar.y += 2 * std::numbers::pi;
	while (polar.y > std::numbers::pi)
		polar.y -= 2 * std::numbers::pi;
	return {0.5 - polar.y / usedAngle, (polar.x - radius) / mapped.y};
}

Geom::Size PolarDescartesTransform::mappedSize() const
{
	return {1.0, (2.0 - static_cast<double>(polar)) / 2.0};
}

CompoundTransform::CompoundTransform(const Geom::Rect &rect,
    double angle,
    const Anim::Interpolated<Gen::CoordSystem> &coordSystem,
    Math::FuzzyBool keepAspectRatio) :
    polarDescartes(coordSystem),
    rect(rect),
    keepAspectRatio(keepAspectRatio)
{
	setAngle(angle);
}

void CompoundTransform::setAngle(double value)
{
	angle = value
	      - static_cast<double>(polarDescartes.getPolar())
	            * std::numbers::pi;
	cosAngle = cos(angle);
	sinAngle = sin(angle);
}

double CompoundTransform::getAngle() const
{
	return angle
	     + static_cast<double>(polarDescartes.getPolar())
	           * std::numbers::pi;
}

Geom::Point CompoundTransform::convert(const Geom::Point &p) const
{
	auto transformed = polarDescartes.convert(p);
	auto rotated = rotate(transformed);
	auto aligned = align(rotated);
	return rect.pos
	     + rect.size * Geom::Point{aligned.x, 1 - aligned.y};
}

Geom::Line CompoundTransform::convertDirectionAt(
    const Geom::Line &vec) const
{
	const auto small = .00000000001;

	auto base = vec.begin;
	auto dir = vec.getDirection();
	auto maxDir = dir.chebyshev();
	auto smallDir = (maxDir != 0.0) ? dir / maxDir * small
	                                : Geom::Point{0.0, small};
	auto end = base + smallDir;

	auto baseConverted = convert(base);
	auto endConverted = convert(end);

	auto dirConverted = (endConverted - baseConverted).normalized();

	endConverted = baseConverted + dirConverted;

	return {baseConverted, endConverted};
}

double CompoundTransform::horConvert(double length) const
{
	return (rotatedSize() * alignedSize()).x
	     * polarDescartes.horConvert(length);
}

double CompoundTransform::verConvert(double length) const
{
	return (rotatedSize() * alignedSize()).y
	     * polarDescartes.verConvert(length);
}

Geom::Point CompoundTransform::getOriginal(const Geom::Point &p) const
{
	auto relative = (p - rect.pos) / rect.size;
	relative = Geom::Point{relative.x, 1.0 - relative.y};
	relative = deAlign(relative);
	auto rotated = rotate(relative, true);
	return polarDescartes.getOriginal(rotated);
}

Geom::Rect CompoundTransform::getRect() const { return rect; }

Geom::Point CompoundTransform::rotate(const Geom::Point &point,
    bool invert,
    const Geom::Point &center) const
{
	auto centered = point - center;

	const Geom::Point rotated(cosAngle * centered.x
	                              + (invert ? 1 : -1) * sinAngle
	                                    * centered.y,
	    (invert ? -1 : 1) * sinAngle * centered.x
	        + cosAngle * centered.y);

	return center + rotated;
}

Geom::Size CompoundTransform::rotatedSize() const
{
	auto cosAng = cos(getAngle());
	auto sinAng = sin(getAngle());

	auto cosAbs = std::signbit(cosAng) ? -cosAng : cosAng;
	auto sinAbs = std::signbit(sinAng) ? -sinAng : sinAng;

	auto x = cosAbs * rect.size.x + sinAbs * rect.size.y;
	auto y = cosAbs * rect.size.y + sinAbs * rect.size.x;

	return {x, y};
}

Geom::Size CompoundTransform::alignedSize() const
{
	auto aspectRatio = rect.size.aspectRatio();
	if (Math::Floating::is_zero(aspectRatio)) aspectRatio = 1.0;

	auto minAspectRatio = std::min(aspectRatio, 1.0);

	auto factor = static_cast<double>(
	    polarDescartes.getPolar() || keepAspectRatio);

	auto aspectRatioVer =
	    Math::interpolate(1.0, minAspectRatio, factor);

	auto aspectRatioHor =
	    Math::interpolate(1.0, minAspectRatio / aspectRatio, factor);

	return {aspectRatioHor, aspectRatioVer};
}

Geom::Point CompoundTransform::align(const Geom::Point &point) const
{
	const Geom::Point half(0.5, 0.5);
	return half + alignedSize() * (point - half);
}

Geom::Point CompoundTransform::deAlign(const Geom::Point &point) const
{
	const Geom::Point half(0.5, 0.5);
	return half + (point - half) / alignedSize();
}

}